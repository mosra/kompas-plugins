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

#include "MapQuestOpenRasterModel.h"

#include <cstdlib>
#include <sstream>

using namespace std;
using namespace Kompas::Core;

PLUGIN_REGISTER(Kompas::Plugins::MapQuestOpenRasterModel,
                "cz.mosra.Kompas.Core.AbstractRasterModel/0.1")

namespace Kompas { namespace Plugins {

#define __base "base"

MapQuestOpenRasterModel::MapQuestOpenRasterModel(PluginManager::AbstractPluginManager* manager, const std::string& plugin): KompasRasterModel(manager, plugin), areaOnline(0, 0, 1, 1) {
    /* All zoom levels for online maps */
    for(Zoom i = 0; i != 17; ++i)
        zoomLevelsOnline.insert(i);

    /* All layers and overlays for online maps */
    layersOnline.push_back(__base);
}

string MapQuestOpenRasterModel::tileUrl(const std::string& layer, Zoom z, const TileCoords& coords) const {
    ostringstream url;

    int server = rand()%4+1;

    url << "http://otile" << server;

    if(layer == __base)
        url << ".mqcdn.com/tiles/1.0.0/osm/";
    else return "";

    url << z << '/' << coords.x << '/' << coords.y << ".png";

    return url.str();
}

}}
