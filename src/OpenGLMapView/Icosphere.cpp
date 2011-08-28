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

#include "Icosphere.h"

#include "Math/GeometryUtils.h"
#include "MeshBuilder.h"

#include <iostream>

using namespace std;
using namespace Magnum;

namespace Kompas { namespace Plugins {

/* Vertices = normals */
static const Vector4 icoVertices[] = {
    Vector4(0, -0.525731f, 0.850651f),
    Vector4(0.850651f, 0, 0.525731f),
    Vector4(0.850651f, 0, -0.525731f),
    Vector4(-0.850651f, 0, -0.525731f),
    Vector4(-0.850651f, 0, 0.525731f),
    Vector4(-0.525731f, 0.850651f, 0),
    Vector4(0.525731f, 0.850651f, 0),
    Vector4(0.525731f, -0.850651f, 0),
    Vector4(-0.525731f, -0.850651f, 0),
    Vector4(0, -0.525731f, -0.850651f),
    Vector4(0, 0.525731f, -0.850651f),
    Vector4(0, 0.525731f, 0.850651f),
};

/* Vertex indices */
static const GLubyte icoIndices[] = {
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

vector<Vector4> Icosphere::create(size_t subdivisions, const vector<Matrix3>& seams) {
    MeshBuilder<Vector4> builder;
    builder.setData(icoVertices, icoIndices, 12, 60);

    /* Make subdivisions */
    for(size_t subdiv = 0; subdiv != subdivisions; ++subdiv)
        builder.subdivide(interpolator);

    /*
     * For each seam go through all faces and if intersections with the
     * face are found, update original face and add two new.
     *
     *                 vertices[(j+2)%3]
     *                      _-``-_
     *                   _-` orig `-_
     *                _-`            `-_
     *           newVertex1 ------ newVertex2
     *          _-` `  -  _                `-_
     *       _-`  newFace1   `  -  _ newFace2 `-_
     *    _-`                         `  -  _    `-_
     * vertices[j] ------------------- vertices[(j+1)%3]
    */
    MeshBuilder<Vector4>::Face orig, newFace1, newFace2;
    Vector4 newVertex1, newVertex2;
    for(auto it = seams.cbegin(); it != seams.cend(); ++it) {
        size_t faceCount = builder.faces().size();
        for(size_t i = 0; i != faceCount; ++i) {
            /* Go through all face edges and get intersections */
            char intersectionCount = 0;
            for(size_t j = 0; j != 3; ++j) {
                Vector4 a = builder.vertices()[builder.faces()[i].vertices[j]];
                Vector4 b = builder.vertices()[builder.faces()[i].vertices[(j+1)%3]];
                GLfloat t = Math::GeometryUtils<GLfloat>::intersection(*it, a.xyz(), b.xyz());

                /* Edge without intersection, the other two can possibly be with
                   intersection, prepare new faces */
                if(t < 0 || t > 1) {
                    MeshBuilder<Vector4>::VertexPointer newVertex1Pointer, newVertex2Pointer;

                    /* If intersections are on first and third edge, the new
                       vertices are in different order than else */
                    if(intersectionCount == 1) {
                        newVertex1Pointer = builder.vertices().size();
                        newVertex2Pointer = builder.vertices().size()+1;
                    } else {
                        newVertex1Pointer = builder.vertices().size()+1;
                        newVertex2Pointer = builder.vertices().size();
                    }

                    /* Update faces' vertices */
                    orig.vertices[0] = builder.faces()[i].vertices[(j+2)%3];
                    orig.vertices[1] = newVertex1Pointer;
                    orig.vertices[2] = newVertex2Pointer;
                    newFace1.vertices[0] = newVertex1Pointer;
                    newFace1.vertices[1] = builder.faces()[i].vertices[j];
                    newFace1.vertices[2] = builder.faces()[i].vertices[(j+1)%3];
                    newFace2.vertices[0] = newVertex1Pointer;
                    newFace2.vertices[1] = builder.faces()[i].vertices[(j+1)%3];
                    newFace2.vertices[2] = newVertex2Pointer;

                /* Edge with intersection, prepare new vertex */
                } else {
                    Vector4 newVertex((a+(b-a)*t).xyz().normalized());

                    if(intersectionCount++ == 1) newVertex2 = newVertex;
                    else newVertex1 = newVertex;
                }
            }

            /* Add new vertices and faces */
            if(intersectionCount == 2) {
                builder.addVertex(newVertex1);
                builder.addVertex(newVertex2);
                builder.faces()[i] = orig;
                builder.addFace(newFace1);
                builder.addFace(newFace2);
            }
        }
    }

    builder.cleanMesh<3>();
    builder.build(this, _vertexBuffer, Buffer::DrawStatic, Buffer::DrawStatic);

    return builder.vertices();
}

}}
