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

#include "Earth.h"
#include "Light.h"
#include "AbstractProjection.h"

using namespace Magnum;
using namespace Kompas::Core;

namespace Kompas { namespace Plugins {

/* Vertices = normals */
static const GLfloat icosahedron[] = {
    0, -0.525731f, 0.850651f, 1,
    0.850651f, 0, 0.525731f, 1,
    0.850651f, 0, -0.525731f, 1,
    -0.850651f, 0, -0.525731f, 1,
    -0.850651f, 0, 0.525731f, 1,
    -0.525731f, 0.850651f, 0, 1,
    0.525731f, 0.850651f, 0, 1,
    0.525731f, -0.850651f, 0, 1,
    -0.525731f, -0.850651f, 0, 1,
    0, -0.525731f, -0.850651f, 1,
    0, 0.525731f, -0.850651f, 1,
    0, 0.525731f, 0.850651f, 1,
};

/* Vertex indices */
static const GLubyte indices[] = {
    1, 2, 6,
    1, 7, 2,
    3, 4, 5,
    4, 3, 8,
    6, 5, 11,
    5, 6, 10,
    9, 10, 2,
    10, 9, 3,
    7, 8, 9,
    8, 7, 0,
    11, 0, 1,
    0, 11, 4,
    6, 2, 10,
    1, 6, 11,
    3, 5, 10,
    5, 4, 11,
    2, 7, 9,
    7, 1, 0,
    3, 9, 8,
    4, 8, 0
};

Earth::Earth(Light* _light, Object* parent): Object(parent), mesh(Mesh::Triangles, 12, 60, GL_UNSIGNED_BYTE), light(_light), baseColor(1.0f, 1.0f, 0.8f) {

    /* Fill the mesh buffer */
    Buffer* buffer = mesh.addBuffer(true);
    buffer->setData(sizeof(icosahedron), icosahedron, Buffer::DrawStatic);
    mesh.indexBuffer()->setData(sizeof(indices), indices, Buffer::DrawStatic);

    /* Bind shader attribute to the buffer */
    mesh.bindAttribute<Vector4>(buffer, EarthShader::Vertex);

    /* Texture */
    texture.setMagnificationFilter(BaseTexture::Linear);
    texture.setMinificationFilter(BaseTexture::Linear);
    texture.setWrapping(Magnum::Math::Vector2<BaseTexture::Wrapping>(BaseTexture::ClampToEdge, BaseTexture::ClampToEdge));

    /* Texture coordinates buffer */
    textureCoordinates = mesh.addBuffer(false);
    mesh.bindAttribute<Vector2>(textureCoordinates, EarthShader::TextureCoordinates);
}

void Earth::generateTextureCoordinates(const AbstractProjection* projection) {
    Vector2 coordinates[12];

    for(int i = 0; i != 12; ++i) {
        Vector4 vertex(icosahedron+i*4);
        double latitude = asin(static_cast<double>(vertex.y()));
        double length = sqrt(static_cast<double>(vertex.x()*vertex.x()+vertex.z()*vertex.z()));
        double longtitude;
        if(length != 0)
            longtitude = asin(static_cast<double>(vertex.x())/length);
        else
            longtitude = 0;

        if(vertex.z() < 0) {
            if(longtitude < 0) longtitude = -PI - longtitude;
            else longtitude = PI - longtitude;
        }

        Coords<double> c = projection->fromWgs84(Wgs84Coords(latitude*180/PI, longtitude*180/PI));

        coordinates[i] = Vector2(static_cast<GLfloat>(c.x), static_cast<GLfloat>(c.y));
    }

    textureCoordinates->setData(sizeof(coordinates), coordinates, Buffer::DrawStatic);
}

void Earth::draw(const Matrix4& transformationMatrix, const Matrix4& projectionMatrix) {
    texture.bind();

    shader.use();
    shader.setLightUniform(light->position());
    shader.setBaseColorUniform(baseColor);
    shader.setTransformMatrixUniform(transformationMatrix);
    shader.setTransformProjectionMatrixUniform(projectionMatrix*transformationMatrix);
    shader.setNormalMatrixUniform(transformationMatrix.ij(3, 3));
    shader.setTextureUniform(0);

    mesh.draw();
}

}}
