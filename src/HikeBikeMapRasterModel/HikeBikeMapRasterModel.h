#ifndef Kompas_Plugins_HikeBikeMapRasterModel_h
#define Kompas_Plugins_HikeBikeMapRasterModel_h
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
 * @brief Class Kompas::Plugins::HikeBikeMapRasterModel
 */

#include "KompasRasterModel/KompasRasterModel.h"
#include "MercatorProjection/MercatorProjection.h"

namespace Kompas { namespace Plugins {

/**
 * @brief Hike & Bike Map raster model
 */
class HikeBikeMapRasterModel: public KompasRasterModel {
    public:
        /** @copydoc Plugins::KompasRasterModel::KompasRasterModel */
        HikeBikeMapRasterModel(PluginManager::AbstractPluginManager* manager = 0, const std::string& plugin = "");

        inline virtual int features() const {
            return KompasRasterModel::features()|MultipleFileFormat|LoadableFromUrl|NonConvertableFormat|ConvertableCoords;
        }
        inline virtual const Core::AbstractProjection* projection() const
            { return &_projection; }
        inline virtual Core::TileSize tileSize() const
            { return Core::TileSize(256,256); }
        inline virtual std::string copyright() const
            { return "© OpenStreetMap and contributors, CC-BY-SA."; }

        inline virtual std::set<Core::Zoom> zoomLevels() const {
            return online() ? zoomLevelsOnline : KompasRasterModel::zoomLevels();
        }
        inline virtual Core::TileArea area() const {
            return online() ? areaOnline : KompasRasterModel::area();
        }
        virtual std::vector<std::string> layers() const {
            return online() ? layersOnline : KompasRasterModel::layers();
        }
        virtual std::vector<std::string> overlays() const {
            return online() ? overlaysOnline : KompasRasterModel::overlays();
        }

        virtual std::string tileUrl(const std::string& layer, Core::Zoom z, const Core::TileCoords& coords) const;

    private:
        MercatorProjection _projection;

        std::set<Core::Zoom> zoomLevelsOnline;
        Core::TileArea areaOnline;
        std::vector<std::string> layersOnline, overlaysOnline;
};

}}

#endif