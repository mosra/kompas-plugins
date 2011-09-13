#ifndef Kompas_Plugins_OpenGLMapViewPrivate_h
#define Kompas_Plugins_OpenGLMapViewPrivate_h
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

#include "Scene.h"

#include <QtOpenGL/QGLWidget>

namespace Magnum {
    class Light;
}

namespace Kompas { namespace Plugins {

class Earth;

class OpenGLMapViewPrivate: public QGLWidget {
    public:
        OpenGLMapViewPrivate(QWidget* parent = 0, const QGLWidget* shareWidget = 0, Qt::WindowFlags f = 0);

        QSize minimumSizeHint() const;

        Magnum::Scene* scene;
        Magnum::Camera* camera;
        Magnum::Light* light;
        Earth* earth;

    protected:
        void initializeGL();
        void paintGL();
        void resizeGL(int w, int h);

        void mousePressEvent(QMouseEvent* event);
        void mouseMoveEvent(QMouseEvent* event);
        void wheelEvent(QWheelEvent* event);

    private:
        QPoint previousMouse;
};

}}

#endif
