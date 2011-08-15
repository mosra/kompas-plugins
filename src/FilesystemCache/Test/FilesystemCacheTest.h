#ifndef Kompas_Plugins_Test_FilesystemCacheTest_h
#define Kompas_Plugins_Test_FilesystemCacheTest_h
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

#include <QtCore/QObject>
#include "FilesystemCache/FilesystemCache.h"

namespace Kompas { namespace Plugins { namespace Test {

class FilesystemCacheTest: public QObject {
    Q_OBJECT

    class Cache: public FilesystemCache {
        public:
            inline Cache(PluginManager::AbstractPluginManager* manager = 0, const std::string& plugin = ""): FilesystemCache(manager, plugin) {}

            inline std::string get(const std::string& key) {
                return FilesystemCache::get(key);
            }
            inline bool set(const std::string& key, const std::string& data) {
                return FilesystemCache::set(key, data);
            }
    };

    private slots:
        void save();
        void loadSave();
        void optimize();
        void purge();
};

}}}

#endif
