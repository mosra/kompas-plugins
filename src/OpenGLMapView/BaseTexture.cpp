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

#include "BaseTexture.h"

using namespace Magnum;

namespace Kompas { namespace Plugins {

void BaseTexture::setTexture(QImage image) {
    if(image.format() != QImage::Format_RGB888)
        image = image.convertToFormat(QImage::Format_RGB888);

    setData(0, RGB, Math::Vector2<GLsizei>(image.width(), image.height()), RGB, static_cast<const GLubyte*>(image.constBits()));
}

}}
