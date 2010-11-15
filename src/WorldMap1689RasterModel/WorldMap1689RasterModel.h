#ifndef Map2X_Plugins_WorldMap1689RasterModel_h
#define Map2X_Plugins_WorldMap1689RasterModel_h
/*
    Copyright © 2007, 2008, 2009, 2010 Vladimír Vondruš <mosra@centrum.cz>

    This file is part of Map2X.

    Map2X is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License version 3
    only, as published by the Free Software Foundation.

    Map2X is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License version 3 for more details.
*/

/** @file
 * @brief Class Map2X::Plugins::WorldMap1689RasterModel
 */

#include "Map2XRasterModel/Map2XRasterModel.h"

#include "constants.h"
#include "StereographicProjection/StereographicProjection.h"

using namespace std;
using namespace Map2X::Core;

namespace Map2X { namespace Plugins {

/**
 * @brief World map from 1689
 *
 * Single-zoom, single-image map image.
 * @todo Divide into smaller parts, use also larger file.
 * @todo Similar plugin for http://upload.wikimedia.org/wikipedia/commons/b/b5/Nova_totius_Terrarum_Orbis_geographica_ac_hydrographica_tabula_(Hendrik_Hondius)_balanced.jpg
 */
class WorldMap1689RasterModel: public Map2XRasterModel {
    public:
        /** @copydoc Plugins::Map2XRasterModel::Map2XRasterModel */
        WorldMap1689RasterModel(PluginManager::AbstractPluginManager* manager, const std::string& plugin);

        inline virtual int features() const {
            return Map2XRasterModel::features()|LoadableFromUrl|NonConvertableFormat;
        }
        virtual double zoomStep() const { return 2; }
        virtual const AbstractProjection* projection() const { return &_projection; }
        virtual TileSize tileSize() const { return TileSize(2280, 1967); }

        inline virtual std::vector<Core::Zoom> zoomLevels() const {
            return online() ? zoomLevelsOnline : Map2XRasterModel::zoomLevels();
        }
        inline virtual Core::TileArea area() const {
            return online() ? areaOnline : Map2XRasterModel::area();
        }
        virtual std::vector<std::string> layers() const {
            return online() ? layersOnline : Map2XRasterModel::layers();
        }
        virtual std::vector<std::string> overlays() const {
            return online() ? overlaysOnline : Map2XRasterModel::overlays();
        }

        inline virtual string tileUrl(const string& layer, Zoom z, const TileCoords& coords) const {
            return "http://upload.wikimedia.org/wikipedia/commons/e/e0/World_Map_1689-smaller.jpg";
        }

    private:
        StereographicProjection _projection;
        std::vector<Core::Zoom> zoomLevelsOnline;
        Core::TileArea areaOnline;
        std::vector<std::string> layersOnline, overlaysOnline;
};

}}

#endif
