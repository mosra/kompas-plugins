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

#include "WorldMap1630RasterModel.h"

#include "constants.h"

using namespace std;

PLUGIN_REGISTER(Kompas::Plugins::WorldMap1630RasterModel,
                "cz.mosra.Kompas.Core.AbstractRasterModel/0.1")

namespace Kompas { namespace Plugins {

WorldMap1630RasterModel::WorldMap1630RasterModel(PluginManager::AbstractPluginManager* manager, const string& plugin): KompasRasterModel(manager, plugin), areaOnline(0, 0, 1, 1) {
    /*
        width:  1600
        height: 1124
        left:   38
        top:    195, 199 => 197
        right:  1566
        bottom: 966, 970 => 968
        gap:    0
    */
    _projection.setCentralMeridian(-PI/9);
    _projection.setShift(Coords<double>(38/1600.0, 197/1124.0));
    _projection.setStretch(Coords<double>((1566-38)/1600.0, (968-197)/1124.0));

    /* All zoom levels and layers for online map */
    zoomLevelsOnline.insert(0);
    layersOnline.push_back("base");
}

}}
