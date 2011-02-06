#ifndef Kompas_Plugins_MoonCelestialBody_h
#define Kompas_Plugins_MoonCelestialBody_h
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
 * @brief Class Kompas::Plugins::MoonCelestialBody
 */

#include "AbstractCelestialBody.h"

namespace Kompas { namespace Plugins {

/**
 * @brief Moon
 */
class MoonCelestialBody: public Core::AbstractCelestialBody {
    public:
        /** @copydoc Core::AbstractCelestialBody::AbstractCelestialBody */
        MoonCelestialBody(PluginManager::AbstractPluginManager* manager = 0, const std::string& plugin = ""): AbstractCelestialBody(manager, plugin) {}

        inline virtual double equatorialRadius() const { return  1738100; }
        inline virtual double polarRadius() const { return 1736000; }
        inline virtual double flattening() const { return 0.00121; }
};

}}

#endif
