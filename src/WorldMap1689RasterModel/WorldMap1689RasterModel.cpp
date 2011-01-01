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

#include "WorldMap1689RasterModel.h"

#include "constants.h"

using namespace std;

PLUGIN_REGISTER(Kompas::Plugins::WorldMap1689RasterModel,
                "cz.mosra.Kompas.Core.AbstractRasterModel/0.1")

namespace Kompas { namespace Plugins {

WorldMap1689RasterModel::WorldMap1689RasterModel(PluginManager::AbstractPluginManager* manager, const string& plugin): KompasRasterModel(manager, plugin), areaOnline(0, 0, 1, 1) {
    /*
        width:  2280
        height: 1967
        left:   139
        top:    470, 468 => 469
        right:  2145
        bottom: 1491, 1489 => 1490
        gap:    0

        stretch.x = (right - left)/width
        stretch.y = (bottom - top)/height
    */
    _projection.setCentralMeridian(-PI/9);
    _projection.setShift(Coords<double>(139/2280.0, 469/1967.0));
    _projection.setStretch(Coords<double>((2145-139)/2280.0, (1490-469)/1967.0));

    /* All zoom levels and layers for online map */
    zoomLevelsOnline.insert(0);
    layersOnline.push_back("base");
}

}}
