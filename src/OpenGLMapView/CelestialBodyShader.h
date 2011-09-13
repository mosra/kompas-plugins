#ifndef Kompas_Plugins_CelestialBodyShader_h
#define Kompas_Plugins_CelestialBodyShader_h
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

#include "AbstractShaderProgram.h"

namespace Kompas { namespace Plugins {

class CelestialBodyShader: public Magnum::AbstractShaderProgram {
    public:
        enum Attribute {
            Vertex,
            TextureCoordinates
        };

        CelestialBodyShader();

        inline void setLightUniform(const Magnum::Vector3& _light) {
            setUniform(light, _light);
        }

        inline void setBaseColorUniform(const Magnum::Vector3& _color) {
            setUniform(baseColor, _color);
        }

        inline void setTransformMatrixUniform(const Magnum::Matrix4& matrix) {
            setUniform(transformMatrix, matrix);
        }

        inline void setTransformProjectionMatrixUniform(const Magnum::Matrix4& matrix) {
            setUniform(transformProjectionMatrix, matrix);
        }

        inline void setNormalMatrixUniform(const Magnum::Matrix3& matrix) {
            setUniform(normalMatrix, matrix);
        }

        inline void setTextureUniform(GLint level) {
            setUniform(texture, level);
        }

    private:
        GLint light, baseColor, transformMatrix, transformProjectionMatrix, normalMatrix, texture;
};

}}

#endif
