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

#include "FreemapSkRasterModel.h"

#include <cstdlib>
#include <sstream>

using namespace std;
using namespace Kompas::Core;

PLUGIN_REGISTER(Kompas::Plugins::FreemapSkRasterModel,
                "cz.mosra.Kompas.Core.AbstractRasterModel/0.1")

namespace Kompas { namespace Plugins {

#define __base      "base"
#define __tourist   "tourist"
#define __cycling   "cycling"
#define __satellite "satellite"
#define __ski       "ski"
#define __transport "transport"

FreemapSkRasterModel::FreemapSkRasterModel(PluginManager::AbstractPluginManager* manager, const std::string& plugin): KompasRasterModel(manager, plugin), areaOnline(69, 43, 4, 2) {
    /* All zoom levels for online maps */
    for(Zoom i = 7; i != 17; ++i)
        zoomLevelsOnline.insert(i);

    /* All layers and overlays for online maps */
    layersOnline.push_back(__base);
    layersOnline.push_back(__tourist);
    layersOnline.push_back(__cycling);
    layersOnline.push_back(__satellite);

    overlaysOnline.push_back(__ski);
    overlaysOnline.push_back(__transport);
}

string FreemapSkRasterModel::tileUrl(const std::string& layer, Zoom z, const TileCoords& coords) const {
    if(z > 16) return "";

    /* Random server number */
    int servernum = rand()%4+1;

    ostringstream url;
    url << "http://t" << servernum << ".freemap.sk/data/layers/";

    string extension = ".jpeg";
    if(layer == __base)
        url << "presets/A/";
    else if(layer == __tourist)
        url << "presets/T/";
    else if(layer == __cycling)
        url << "presets/C/";
    else if(layer == __satellite)
        url << "presets/O/";
    else if(layer == __ski) {
        url << "ski/";
        extension = ".png";
    } else if(layer == __transport) {
        url << "transport/";
        extension = ".png";
    }

    url << z << '/' << coords.x << '/' << coords.y << extension;

    return url.str();
}

}}
