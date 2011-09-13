#ifndef Kompas_QtGui_OpenGLMapView_h
#define Kompas_QtGui_OpenGLMapView_h
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

#include "AbstractMapView.h"

namespace Kompas { namespace Plugins {

class OpenGLMapViewPrivate;

/**
@brief 3D OpenGL view of the map

Currently supports only loading tile from base zoom, zooming with mouse wheel
and dragging the view via mouse.
*/
class OpenGLMapView: public QtGui::AbstractMapView {
    Q_OBJECT

    public:
        OpenGLMapView(PluginManager::AbstractPluginManager* manager = 0, const std::string& plugin = "");

        unsigned int zoom() const { return _zoom; }
        Core::LatLonCoords coords(const QPoint& pos = QPoint()) { return Core::LatLonCoords(); }
        Core::AbsoluteArea<double> viewedArea(const QRect& area = QRect()) { return Core::AbsoluteArea<double>(); }
        QString layer() const { return _layer; }
        QStringList overlays() const { return QStringList(); }

    public slots:
        void updateRasterModel();
        bool zoomIn(const QPoint& pos = QPoint()) { return false; }
        bool zoomOut(const QPoint& pos = QPoint()) { return false; }
        bool zoomTo(Core::Zoom zoom, const QPoint& pos = QPoint()) { return false; }
        bool setCoords(const Kompas::Core::LatLonCoords& coords, const QPoint& pos = QPoint()) { return false; }
        bool move(int x, int y);
        bool setLayer(const QString& layer);
        bool addOverlay(const QString& layer) { return false; }
        bool removeOverlay(const QString& overlay) { return false; }

    private:
        OpenGLMapViewPrivate* view;
        Core::TileArea area;
        QString _layer;
        Core::Zoom _zoom;

        void tileData(const QString& layer, Core::Zoom z, const Kompas::Core::TileCoords& coords, const QByteArray& data);
        void tileLoading(const QString& layer, Core::Zoom z, const Kompas::Core::TileCoords& coords) {}
        void tileNotFound(const QString& layer, Core::Zoom z, const Kompas::Core::TileCoords& coords) {}
};

}}

#endif
