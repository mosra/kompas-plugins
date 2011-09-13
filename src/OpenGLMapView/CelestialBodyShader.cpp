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

#include "CelestialBodyShader.h"

#include "Utility/Resource.h"

using namespace Kompas::Utility;
using namespace Magnum;

namespace Kompas { namespace Plugins {

CelestialBodyShader::CelestialBodyShader() {
    Resource r("OpenGLMapView");
    Shader* vertexShader = Shader::fromData(Shader::Vertex, r.get("CelestialBodyShader.vert"));
    Shader* fragmentShader = Shader::fromData(Shader::Fragment, r.get("CelestialBodyShader.frag"));

    attachShader(vertexShader);
    attachShader(fragmentShader);

    bindAttribute(Vertex, "vertex");
    bindAttribute(TextureCoordinates, "textureCoordinates");

    link();

    light = uniformLocation("light");
    baseColor = uniformLocation("baseColor");
    transformMatrix = uniformLocation("transformMatrix");
    transformProjectionMatrix = uniformLocation("transformProjectionMatrix");
    normalMatrix = uniformLocation("normalMatrix");
    texture = uniformLocation("textureData");

    delete vertexShader;
    delete fragmentShader;
}

}}
