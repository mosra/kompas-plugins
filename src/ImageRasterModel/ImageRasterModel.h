#ifndef Kompas_Plugins_ImageRasterModel_h
#define Kompas_Plugins_ImageRasterModel_h
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

/** @file
 * @brief Class Kompas::Plugins::ImageRasterModel
 */

#include "AbstractRasterModel.h"
#include <QtCore/QString>
#include <QtGui/QImage>
#include <QtGui/QPainter>

class QImage;
class QPainter;

namespace Kompas { namespace Plugins {

/**
 * @brief Image raster model
 */
class ImageRasterModel: public Core::AbstractRasterModel {
    public:
        /** @copydoc Plugins::KompasRasterModel::KompasRasterModel */
        ImageRasterModel(PluginManager::AbstractPluginManager* manager = 0, const std::string& plugin = "");

        inline virtual int features() const {
            return SingleLayer|SingleZoom|WriteableFormat|WriteOnly;
        }
        inline virtual std::vector< std::string > fileExtensions() const {
            return _fileExtensions;
        }
        inline virtual Core::TileSize tileSize() const {
            return _tileSize;
        }
        inline virtual std::set<Core::Zoom> zoomLevels() const {
            return _zoomLevels;
        }
        inline virtual Core::TileArea area() const {
            return Core::TileArea(0, 0, 1, 1);
        }
        inline virtual std::vector<std::string> layers() const {
            return _layers;
        }

        inline virtual int addPackage(const std::string& filename) { return -1; }
        inline virtual int packageCount() const { return 0; }
        inline virtual std::string tileFromPackage(const std::string& layer, Core::Zoom z, const Core::TileCoords& coords) { return ""; }

        virtual bool initializePackage(const std::string& filename, const Core::TileSize& tileSize, const std::vector<Core::Zoom>& zoomLevels, const Core::TileArea& area, const std::vector<std::string>& layers, const std::vector<std::string>& overlays);
        virtual bool tileToPackage(const std::string& layer, Core::Zoom z, const Core::TileCoords& coords, const std::string&
        data);
        virtual bool finalizePackage();

    private:
        struct Output {
            QString filename;
            Core::TileSize tileSize;
            Core::TileArea area;
            QImage image;
            QPainter painter;

            Output(const QString& filename, const Core::TileSize& tileSize, const Core::TileArea& area);
        };

        std::vector<std::string> _fileExtensions;
        Core::TileSize _tileSize;
        std::set<Core::Zoom> _zoomLevels;
        std::vector<std::string> _layers;

        Output* output;
};

}}

#endif
