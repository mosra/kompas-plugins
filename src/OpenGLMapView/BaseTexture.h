#ifndef Kompas_BaseTexture_h
#define Kompas_BaseTexture_h
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

#include <QtGui/QImage>

#include "Texture.h"

namespace Kompas { namespace Plugins {

class BaseTexture: public Magnum::Texture2D {
    public:
        void setTexture(QImage image);
};

}}

#endif