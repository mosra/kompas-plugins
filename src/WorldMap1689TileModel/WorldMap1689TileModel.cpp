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

#include "AbstractTileModel.h"

#include "constants.h"
#include "StereographicProjection/StereographicProjection.h"

using namespace std;
using namespace Map2X::Core;

namespace Map2X { namespace Plugins {

class WorldMap1689TileModel: public AbstractTileModel {
    public:
        WorldMap1689TileModel(PluginManager::AbstractPluginManager* manager, const std::string& plugin): AbstractTileModel(manager, plugin) {
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
        }

        virtual int features() const { return LoadableFromUrl; }

        virtual const AbstractProjection* projection() const { return &_projection; }

        virtual TileSize tileSize() const { return TileSize(2280, 1967); }

        virtual string copyright() const { return ""; }

        virtual vector<Zoom> zoomLevels() const {
            vector<Zoom> z;
            z.push_back(0);
            return z;
        }

        virtual TileArea area() const { return TileArea(0, 0, 1, 1); }

        virtual vector<string> layers() const {
            vector<string> l;
            l.push_back("Base");
            return l;
        }

        virtual vector<string> overlays() const {
            return vector<string>();
        }

        inline virtual string tileUrl(const string& layer, Zoom z, const TileCoords& coords) const {
            return "http://upload.wikimedia.org/wikipedia/commons/e/e0/World_Map_1689-smaller.jpg";
        }

    private:
        StereographicProjection _projection;
};

}}

PLUGIN_REGISTER(Map2X::Plugins::WorldMap1689TileModel,
                "cz.mosra.Map2X.Core.AbstractTileModel/0.1")
