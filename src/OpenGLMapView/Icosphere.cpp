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

Icosphere::Icosphere(size_t subdivisions): IndexedMesh(Mesh::Triangles, 0, 0, GL_UNSIGNED_BYTE) {
    MeshBuilder<Vector4> builder;
    builder.setData(icoVertices, icoIndices, 12, 60);

    /* Make subdivisions */
    for(size_t subdiv = 0; subdiv != subdivisions; ++subdiv) {
        builder.subdivide(interpolator);
        builder.cleanMesh();
    }

    _vertexBuffer = addBuffer(true);
    builder.build(this, _vertexBuffer, Buffer::DrawStatic, Buffer::DrawStatic);

    _vertices = builder.fixedVertices();
}

}}
