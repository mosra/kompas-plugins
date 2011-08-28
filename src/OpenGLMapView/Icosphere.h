#ifndef Kompas_Plugins_Icosphere_h
#define Kompas_Plugins_Icosphere_h
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

#include "IndexedMesh.h"

namespace Kompas { namespace Plugins {

class Icosphere: public Magnum::IndexedMesh {
    public:
        Icosphere(size_t subdivisions);

        /** @brief Buffer with icosphere vertices */
        inline Magnum::Buffer* vertexBuffer() const { return _vertexBuffer; }

        inline const std::vector<Magnum::Vector4>& vertices() const { return _vertices; }

    private:
        Magnum::Buffer* _vertexBuffer;
        std::vector<Magnum::Vector4> _vertices;

        static inline Magnum::Vector4 interpolator(Magnum::Vector4 a, Magnum::Vector4 b) {
            return (a+b).xyz().normalized();
        }
};

}}

#endif
