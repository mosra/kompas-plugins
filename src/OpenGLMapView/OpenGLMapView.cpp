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

#include "OpenGLMapView.h"
#include "OpenGLMapViewPrivate.h"

#include <QtGui/QHBoxLayout>

#include "TileDataThread.h"
#include "CelestialBody.h"
#include "MainWindow.h"

PLUGIN_REGISTER(Kompas::Plugins::OpenGLMapView,
                "cz.mosra.Kompas.QtGui.AbstractMapView/0.2")

using namespace Kompas::Core;
using namespace Kompas::Utility;
using namespace Kompas::QtGui;
using namespace Magnum;

namespace Kompas { namespace Plugins {

OpenGLMapView::OpenGLMapView(PluginManager::AbstractPluginManager* manager, const std::string& plugin): AbstractMapView(manager, plugin) {
    /* Wrapping layout around actual OpenGL widget */
    view = new OpenGLMapViewPrivate;
    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(view);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);

    /* Setup the view */
    updateRasterModel();
}

void OpenGLMapView::updateRasterModel() {
    if(!isReady()) return;

    /* The only possible way to call initializeGL. Hope the blind draw wouldn't
       do any harm. */
    if(!view->celestialBody) view->updateGL();

    /* Reset zoom and layer */
    _zoom = pow2(31);
    _layer.clear();

    /* Generate texture coordinates, get default model layer and zoom */
    Locker<const AbstractRasterModel> rasterModel = MainWindow::instance()->rasterModelForRead();
    if(rasterModel()->projection())
        view->celestialBody->generateTextureCoordinates(rasterModel()->projection());
    area = rasterModel()->area();
    _zoom = *rasterModel()->zoomLevels().begin();
    QString layer = QString::fromStdString(rasterModel()->layers()[0]);
    rasterModel.unlock();

    /* Set map layer */
    setLayer(layer);
}

bool OpenGLMapView::move(int x, int y) {
    if(!isReady() || !view->celestialBody) return false;

    /* Jumpy skips, currently cannot move exactly given distance */
    if(x != 0) {
        if(x < 0) view->celestialBody->rotate(PI/20, Vector3::yAxis(), false);
        else view->celestialBody->rotate(-PI/20, Vector3::yAxis(), false);
    }
    if(y != 0) {
        if(y < 0) view->celestialBody->rotate(PI/20, Vector3::xAxis());
        else view->celestialBody->rotate(-PI/20, Vector3::xAxis());
    }

    view->updateGL();
    return true;
}

bool OpenGLMapView::setLayer(const QString& layer) {
    if(!isReady() || !view->celestialBody) return false;

    if(_layer == layer) return true;

    /* Set layer and get tile data */
    _layer = layer;
    /** @todo Get tile data for whole visible area, not just left top tile */
    tileDataThread->getTileData(_layer, _zoom, TileCoords(area.x, area.y));

    emit layerChanged(_layer);
    return true;
}

void OpenGLMapView::tileData(const QString& layer, Core::Zoom z, const Kompas::Core::TileCoords& coords, const QByteArray& data) {
    /* Set texture from given data */
    /** @todo Compose texture from all tiles */
    view->celestialBody->texture.setTexture(QImage::fromData(data));

    view->updateGL();
}

}}
