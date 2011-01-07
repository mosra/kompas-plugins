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

class OpenGLMapView: public QtGui::AbstractMapView {
    Q_OBJECT

    public:
        OpenGLMapView(PluginManager::AbstractPluginManager* manager = 0, const std::string& plugin = "");

        virtual unsigned int zoom() const { return _zoom; }
        virtual Core::Wgs84Coords coords(const QPoint& pos = QPoint()) { return Core::Wgs84Coords(); }
        virtual Core::AbsoluteArea<double> viewedArea(const QRect& area = QRect()) { return Core::AbsoluteArea<double>(); }
        virtual QString layer() const { return _layer; }
        virtual QStringList overlays() const { return QStringList(); }

    public slots:
        virtual void updateRasterModel();
        virtual bool zoomIn(const QPoint& pos = QPoint()) { return false; }
        virtual bool zoomOut(const QPoint& pos = QPoint()) { return false; }
        virtual bool zoomTo(Core::Zoom zoom, const QPoint& pos = QPoint()) { return false; }
        virtual bool setCoords(const Kompas::Core::Wgs84Coords& coords, const QPoint& pos = QPoint()) { return false; }
        virtual bool move(int x, int y);
        virtual bool setLayer(const QString& layer);
        virtual bool addOverlay(const QString& layer) { return false; }
        virtual bool removeOverlay(const QString& overlay) { return false; }

    private:
        OpenGLMapViewPrivate* view;
        Core::TileArea area;
        QString _layer;
        Core::Zoom _zoom;

        virtual void tileData(const QString& layer, Core::Zoom z, const Kompas::Core::TileCoords& coords, const QByteArray& data);
        virtual void tileLoading(const QString& layer, Core::Zoom z, const Kompas::Core::TileCoords& coords) {}
        virtual void tileNotFound(const QString& layer, Core::Zoom z, const Kompas::Core::TileCoords& coords) {}
};

}}

#endif
