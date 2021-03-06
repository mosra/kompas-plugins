#ifndef Kompas_Plugins_WorldMap1630RasterModel_h
#define Kompas_Plugins_WorldMap1630RasterModel_h
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
 * @brief Class Kompas::Plugins::WorldMap1630RasterModel
 */

#include "KompasRasterModel/KompasRasterModel.h"

#include "StereographicProjection/StereographicProjection.h"

using namespace std;
using namespace Kompas::Core;

namespace Kompas { namespace Plugins {

/**
 * @brief World map from 1630
 *
 * Single-zoom, single-image map image.
 * @todo Divide into smaller parts, use also larger file from Wikipedia.
 */
class WorldMap1630RasterModel: public KompasRasterModel {
    public:
        /** @copydoc Plugins::KompasRasterModel::KompasRasterModel */
        WorldMap1630RasterModel(PluginManager::AbstractPluginManager* manager, const std::string& plugin);

        inline int features() const {
            return KompasRasterModel::features()|LoadableFromUrl|NonConvertableFormat|ConvertableCoords;
        }
        const AbstractProjection* projection() const { return &_projection; }
        inline std::string celestialBody() const
            { return "EarthCelestialBody"; }
        TileSize tileSize() const { return TileSize(1600, 1124); }

        inline std::set<Core::Zoom> zoomLevels() const {
            return online() ? zoomLevelsOnline : KompasRasterModel::zoomLevels();
        }
        inline Core::TileArea area() const {
            return online() ? areaOnline : KompasRasterModel::area();
        }
        std::vector<std::string> layers() const {
            return online() ? layersOnline : KompasRasterModel::layers();
        }
        std::vector<std::string> overlays() const {
            return online() ? overlaysOnline : KompasRasterModel::overlays();
        }

        inline string tileUrl(const string& layer, Zoom z, const TileCoords& coords) const {
            return "http://lh6.ggpht.com/_KZOijM4jJGU/TIqWkyXcrDI/AAAAAAAAAJs/CQRASGLnWrw/Nova_totius_Terrarum_Orbis_geographica_ac_hydrographica_tabula_(Hendrik_Hondius)_balanced_orig.jpg";
        }

    private:
        StereographicProjection _projection;
        std::set<Core::Zoom> zoomLevelsOnline;
        Core::TileArea areaOnline;
        std::vector<std::string> layersOnline, overlaysOnline;
};

}}

#endif
