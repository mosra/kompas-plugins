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

#include <QtGui/QMouseEvent>

#include "Light.h"
#include "CelestialBody.h"
#include "MainWindow.h"

using namespace Magnum;
using namespace Kompas::Core;
using namespace Kompas::QtGui;

namespace Kompas { namespace Plugins {

OpenGLMapViewPrivate::OpenGLMapViewPrivate(QWidget* parent, const QGLWidget* shareWidget, Qt::WindowFlags f): QGLWidget(QGLFormat(QGL::DoubleBuffer|QGL::Rgba|QGL::StencilBuffer|QGL::DepthBuffer), parent), celestialBody(0) {
    /** @todo Is this really working? */
    QGLFormat fmt = format();
    fmt.setProfile(QGLFormat::CoreProfile);
    setFormat(fmt);
}

QSize OpenGLMapViewPrivate::minimumSizeHint() const {
    /* Prevent crashes when rendering to zero size context */
    return QSize(50, 50);
}

void OpenGLMapViewPrivate::initializeGL() {
    glewInit();

    scene = new Scene();
    scene->setClearColor(0.1f, 0.1f, 0.1f, 1);

    camera = new Camera(scene);
    camera->setPerspective(35.0f*PI/180, 0.001f, 100);
    camera->translate(0, 0, 3.5f);
    scene->setCamera(camera);
    scene->setFeature(Scene::DepthTest, true);

    light = new Light(scene);
    light->translate(-15, 15, 60);

    celestialBody = new CelestialBody(light, scene);
}

void OpenGLMapViewPrivate::resizeGL(int width, int height) {
    scene->setViewport(width, height);
}

void OpenGLMapViewPrivate::paintGL() {
    scene->draw();
}

void OpenGLMapViewPrivate::mousePressEvent(QMouseEvent* event) {
    previousMouse = event->pos();
}

void OpenGLMapViewPrivate::mouseMoveEvent(QMouseEvent* event) {
    /** @todo Clamp to (-1, 1) */
    /** @todo More precise rotation, based on mouse position relative to planet */
    double yAngle = asin(static_cast<double>(event->x())/width()-0.5)
        -asin(static_cast<double>(previousMouse.x())/width()-0.5);
    double xAngle = asin(static_cast<double>(event->y())/height()-0.5)
        -asin(static_cast<double>(previousMouse.y())/height()-0.5);

    /* Rotate and save current mouse position */
    celestialBody->rotate(yAngle, Vector3::yAxis(), false);
    celestialBody->rotate(xAngle, Vector3::xAxis());
    previousMouse = event->pos();

    updateGL();
}

void OpenGLMapViewPrivate::wheelEvent(QWheelEvent* event) {
    event->accept();

    /* Distance between the planet (unit size) and near camera clipping plane */
    GLfloat distance = camera->transformation().at(3).z()-1-camera->near();

    /* Move 15% of the distance back or forward */
    if(event->delta() > 0)
        distance *= 1 - 1/0.85f;
    else
        distance *= 1 - 0.85f;
    camera->translate(0, 0, distance);

    updateGL();
}

}}
