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

using namespace std;
using namespace Map2X::Core;

namespace Map2X { namespace Plugins {

class WorldMap1689TileModel: public AbstractTileModel {
    public:
        WorldMap1689TileModel(PluginManager::AbstractPluginManager* manager, const std::string& plugin): AbstractTileModel(manager, plugin) {}

        virtual int features() const { return LoadableFromUrl; }

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
};

}}

PLUGIN_REGISTER(Map2X::Plugins::WorldMap1689TileModel,
                "cz.mosra.Map2X.Core.AbstractTileModel/0.1")
