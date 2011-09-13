/*
    Copyright © 2007, 2008, 2009, 2010, 2011 Vladimír Vondruš <mosra@centrum.cz>

    This file is part of Kompas.

    Kompas is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License version 3
    only, as published by the Free Software Foundation.

    Kompas is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License version 3 for more details.
*/

#include "CelestialBody.h"
#include "Light.h"
#include "AbstractProjection.h"
#include "AbstractRasterModel.h"
#include "MainWindow.h"

using namespace std;
using namespace Magnum;
using namespace Kompas::Core;
using namespace Kompas::Utility;
using namespace Kompas::QtGui;

namespace Kompas { namespace Plugins {

CelestialBody::CelestialBody(Light* _light, Object* parent): Object(parent), light(_light), baseColor(1.0f, 1.0f, 0.8f) {

    /* Bind shader attribute to the buffer */
    icosphere.bindAttribute<Vector4>(icosphere.vertexBuffer(), CelestialBodyShader::Vertex);

    /* Texture */
    texture.setMagnificationFilter(BaseTexture::Linear);
    texture.setMinificationFilter(BaseTexture::Linear);
    texture.setWrapping(Magnum::Math::Vector2<BaseTexture::Wrapping>(BaseTexture::ClampToEdge, BaseTexture::ClampToEdge));

    /* Texture coordinates buffer */
    textureCoordinates = icosphere.addBuffer(false);
    icosphere.bindAttribute<Vector2>(textureCoordinates, CelestialBodyShader::TextureCoordinates);
}

void CelestialBody::generateTextureCoordinates(const AbstractProjection* projection) {
    /* Create seam planes from projection */
    vector<LatLonCoords> s = projection->seams();
    double x, y, z;
    vector<Matrix3> seams;
    seams.reserve(s.size()*2/3);
    for(size_t i = 0; i < s.size(); i += 3) {
        Matrix3 first, second;
        for(size_t j = 0; j != 3; ++j) {
            s[i+j].toPointOnSphere(&x, &y, &z);
            first.set(j, Vector3(x, y, z));
        }

        /* Create two parallel planes in smallest possible distance around the seam */
        Vector3 translation = Vector3::cross(first.at(1)-first.at(0), first.at(2)-first.at(0)).normalized()*EPSILON;
        for(size_t j = 0; j != 3; ++j) {
            second.set(j, first.at(j)+translation);
            first.set(j, first.at(j)-translation);
        }
        seams.push_back(first);
        seams.push_back(second);
    }
    vector<Vector4> vertices = icosphere.create(4, seams);

    /* How much of the space is occupied with actual map */
    Locker<const AbstractRasterModel> rasterModel = MainWindow::instance()->rasterModelForRead();
    double wholeSize = pow2(*rasterModel()->zoomLevels().begin());
    TileArea currentArea = rasterModel()->area();
    rasterModel.unlock();

    Area<double, double> area(currentArea.x, currentArea.y, currentArea.w, currentArea.h);
    area = area/wholeSize;

    Vector2* coordinates = new Vector2[icosphere.vertexCount()];

    size_t i = 0;
    for(vector<Vector4>::const_iterator it = vertices.begin(); it != vertices.end(); ++it) {
        double latitude = asin(static_cast<double>(it->y()));
        double longitude;

        /* North / south pole - prevent division by zero length, falling Y
           coordinate out of (-1, 1) range */
        if(abs(abs(it->y())-1) < EPSILON) {
            latitude = it->y() > 0 ? PI/2 : -PI/2;
            longitude = 0;

        /* -90* / 90* - prevent falling X coordinate out of (-1, 1) range */
        } else if(abs(it->z()) < EPSILON) {
            longitude = it->x() > 0 ? PI/2 : -PI/2;

        /* Everything else */
        } else longitude = asin(static_cast<double>(it->x())/sqrt(static_cast<double>(it->x()*it->x()+it->z()*it->z())));

        /* For positive Z longitude is in (-90°, 90°), for neagtive it is in
           (-90°, -180°), (90°, 180°) */
        if(it->z() < 0) {
            if(longitude < 0) longitude = -PI - longitude;
            else longitude = PI - longitude;
        }

        Coords<double> c = projection->fromLatLon(LatLonCoords(latitude*180/PI, longitude*180/PI));
        c.x = (c.x-area.x)/area.w;
        c.y = (c.y-area.y)/area.h;

        coordinates[i++] = Vector2(static_cast<GLfloat>(c.x), static_cast<GLfloat>(c.y));
    }

    textureCoordinates->setData(icosphere.vertexCount()*sizeof(Vector2), coordinates, Buffer::DrawStatic);
    delete coordinates;
}

void CelestialBody::draw(const Matrix4& transformationMatrix, const Matrix4& projectionMatrix) {
    texture.bind();

    shader.use();
    shader.setLightUniform(light->position());
    shader.setBaseColorUniform(baseColor);
    shader.setTransformMatrixUniform(transformationMatrix);
    shader.setTransformProjectionMatrixUniform(projectionMatrix*transformationMatrix);
    shader.setNormalMatrixUniform(transformationMatrix.ij(3, 3));
    shader.setTextureUniform(0);

    icosphere.draw();
}

}}
