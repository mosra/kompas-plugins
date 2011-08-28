#ifndef Kompas_Plugins_MarsCelestialBody_h
#define Kompas_Plugins_MarsCelestialBody_h
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
 * @brief Class Kompas::Plugins::MarsCelestialBody
 */

#include "AbstractCelestialBody.h"

namespace Kompas { namespace Plugins {

/**
 * @brief Mars
 */
class MarsCelestialBody: public Core::AbstractCelestialBody {
    public:
        /** @copydoc Core::AbstractCelestialBody::AbstractCelestialBody */
        MarsCelestialBody(PluginManager::AbstractPluginManager* manager = 0, const std::string& plugin = ""): AbstractCelestialBody(manager, plugin) {}

        inline double equatorialRadius() const { return  3396200; }
        inline double polarRadius() const { return 3376200; }
        inline double flattening() const { return 0.00589; }
};

}}

#endif
