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

#include "OpenGLMapViewPrivate.h"

#include "Light.h"
#include "Earth.h"
#include "MainWindow.h"

using namespace Magnum;
using namespace Kompas::Core;
using namespace Kompas::QtGui;

namespace Kompas { namespace Plugins {

OpenGLMapViewPrivate::OpenGLMapViewPrivate(QWidget* parent, const QGLWidget* shareWidget, Qt::WindowFlags f): QGLWidget(QGLFormat(QGL::DoubleBuffer|QGL::Rgba|QGL::StencilBuffer|QGL::DepthBuffer), parent), earth(0) {
    QGLFormat fmt = format();
    fmt.setProfile(QGLFormat::CoreProfile);

    setFormat(fmt);
}

QSize OpenGLMapViewPrivate::minimumSizeHint() const {
    return QSize(50, 50);
}

void OpenGLMapViewPrivate::initializeGL() {
    glewInit();

    scene.setClearColor(0.1f, 0.1f, 0.1f, 1);

    camera = new Camera(&scene);
    camera->setPerspective(35.0f*PI/180, 0.1f, 100);
    camera->translate(0, 0, 3.5f);
    scene.setCamera(camera);
    scene.setFeature(Scene::DepthTest, true);

    light = new Light(&scene);
    light->translate(-15, 15, 60);

    earth = new Earth(light, &scene);
    const AbstractRasterModel* model = MainWindow::instance()->lockRasterModelForRead();
    if(model && model->projection())
        earth->generateTextureCoordinates(model->projection());
    MainWindow::instance()->unlockRasterModel();
}

void OpenGLMapViewPrivate::resizeGL(int width, int height) {
    scene.setViewport(width, height);
}

void OpenGLMapViewPrivate::paintGL() {
    scene.draw();
}

}}
