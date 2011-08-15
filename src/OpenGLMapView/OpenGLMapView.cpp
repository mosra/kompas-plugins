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
#include "Earth.h"
#include "MainWindow.h"

PLUGIN_REGISTER(Kompas::Plugins::OpenGLMapView,
                "cz.mosra.Kompas.QtGui.AbstractMapView/0.2")

using namespace Kompas::Core;
using namespace Kompas::Utility;
using namespace Kompas::QtGui;
using namespace Magnum;

namespace Kompas { namespace Plugins {

OpenGLMapView::OpenGLMapView(PluginManager::AbstractPluginManager* manager, const std::string& plugin): AbstractMapView(manager, plugin) {
    view = new OpenGLMapViewPrivate;
    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(view);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);
}

void OpenGLMapView::updateRasterModel() {
    if(!isReady() || !view->earth) return;

    _zoom = pow2(31);
    _layer.clear();

    Locker<const AbstractRasterModel> rasterModel = MainWindow::instance()->rasterModelForRead();
    if(rasterModel()->projection())
        view->earth->generateTextureCoordinates(rasterModel()->projection());
    area = rasterModel()->area();
    _zoom = *rasterModel()->zoomLevels().begin();
    QString layer = QString::fromStdString(rasterModel()->layers()[0]);
    rasterModel.unlock();

    setLayer(layer);
}

bool OpenGLMapView::move(int x, int y) {
    if(!isReady() || !view->earth) return false;

    if(x != 0) {
        if(x < 0) view->earth->rotate(PI/20, Vector3::yAxis(), false);
        else view->earth->rotate(-PI/20, Vector3::yAxis(), false);
    }

    if(y != 0) {
        if(y < 0) view->earth->rotate(PI/20, Vector3::xAxis());
        else view->earth->rotate(-PI/20, Vector3::xAxis());
    }

    view->updateGL();

    return true;
}

bool OpenGLMapView::setLayer(const QString& layer) {
    if(!isReady() || !view->earth) return false;

    if(_layer == layer) return true;

    _layer = layer;
    tileDataThread->getTileData(_layer, _zoom, TileCoords(area.x, area.y));

    emit layerChanged(_layer);
    return true;
}

void OpenGLMapView::tileData(const QString& layer, Core::Zoom z, const Kompas::Core::TileCoords& coords, const QByteArray& data) {
    view->earth->texture.setTexture(QImage::fromData(data));
    view->updateGL();
}

}}
