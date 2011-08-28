#ifndef Kompas_Plugins_Earth_h
#define Kompas_Plugins_Earth_h
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

#include "Object.h"
#include "EarthShader.h"
#include "BaseTexture.h"
#include "Icosphere.h"

namespace Magnum {
    class Light;
}

namespace Kompas {

namespace Core {
    class AbstractProjection;
}

namespace Plugins {

class Earth: public Magnum::Object {
    public:
        Earth(Magnum::Light* _light, Object* parent = 0);

        void draw(const Magnum::Matrix4& transformationMatrix, const Magnum::Matrix4& projectionMatrix);

        void generateTextureCoordinates(const Core::AbstractProjection* projection);

        BaseTexture texture;

    private:
        EarthShader shader;
        Icosphere icosphere;
        Magnum::Light* light;
        Magnum::Vector3 baseColor;
        Magnum::Buffer* textureCoordinates;
};

}}

#endif
