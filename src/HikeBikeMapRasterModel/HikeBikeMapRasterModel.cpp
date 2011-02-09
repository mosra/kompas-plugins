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

#include "HikeBikeMapRasterModel.h"

#include <cstdlib>
#include <sstream>

using namespace std;
using namespace Kompas::Core;

PLUGIN_REGISTER(Kompas::Plugins::HikeBikeMapRasterModel,
                "cz.mosra.Kompas.Core.AbstractRasterModel/0.2")

namespace Kompas { namespace Plugins {

#define __hikebike          "hikebike"
#define __hillshading       "hillshading"
#define __lighting          "lighting"
#define __lonvia            "lonvia"
#define __contours          "contours"

HikeBikeMapRasterModel::HikeBikeMapRasterModel(PluginManager::AbstractPluginManager* manager, const std::string& plugin): KompasRasterModel(manager, plugin), areaOnline(0, 0, 1, 1) {
    /* All zoom levels for online maps */
    for(Zoom i = 0; i != 17; ++i)
        zoomLevelsOnline.insert(i);

    /* All layers and overlays for online maps */
    layersOnline.push_back(__hikebike);

    overlaysOnline.push_back(__hillshading);
    overlaysOnline.push_back(__lighting);
    overlaysOnline.push_back(__lonvia);
    overlaysOnline.push_back(__contours);
}

string HikeBikeMapRasterModel::tileUrl(const std::string& layer, Zoom z, const TileCoords& coords) const {
    if(z > 16) return "";

    ostringstream url;

    if(layer == __hikebike)
        url << "http://toolserver.org/tiles/hikebike/";
    else if(layer == __hillshading)
        url << "http://toolserver.org/~cmarqu/hill/";
    else if(layer == __lighting)
        url << "http://toolserver.org/tiles/lighting/";
    else if(layer == __lonvia)
        url << "http://osm.lonvia.de/hiking/";
    else if(layer == __contours)
        url << "http://toolserver.org/~cmarqu/opentiles.com/cmarqu/tiles_contours_8/";
    else return "";

    url << z << '/' << coords.x << '/' << coords.y << ".png";

    return url.str();
}

}}
