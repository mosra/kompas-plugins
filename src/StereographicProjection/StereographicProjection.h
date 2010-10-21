#ifndef Map2X_Plugins_StereographicProjection_h
#define Map2X_Plugins_StereographicProjection_h
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

/** @file
 * @brief Class Map2X::Plugins::StereographicProjection
 */

#include "AbstractProjection.h"

namespace Map2X { namespace Plugins {

/**
 * @brief Plugin for stereographic map projection
 *
 * For maps divided into two hemispheres with poles on top and bottom. There
 * can be set
 */
class StereographicProjection: public Core::AbstractProjection {
    public:
        StereographicProjection(PluginManager::AbstractPluginManager* manager = 0, const std::string& plugin = ""):
            AbstractProjection(manager, plugin), stretch(1, 1), shift(0, 0), gap(0), centralMeridian(0) {}

        virtual Core::Coords<double> fromWgs84(const Core::Wgs84Coords& coords) const;
        virtual Core::Wgs84Coords toWgs84(const Core::Coords<double>& coords) const;

        /**
         * @brief Set map stretch
         *
         * By default the two hemispheres ocuupies whole map, so stretch is set
         * to (1, 1). If these hemispheres are smaller, set it to lower values.
         * Note that the gap between hemispheres is not counted into that, this
         * is set separately via setGap().
         */
        inline void setStretch(const Core::Coords<double>& _stretch)
            { stretch = _stretch; }

        /**
         * @brief Set map shift
         *
         * By default the two hemispheres touch left and top map border, so
         * shift is set to (0, 0). The values are in range 0 - 1 (portion of
         * map size in given direction).
         */
        inline void setShift(const Core::Coords<double>& _shift)
            { shift = _shift; }

        /**
         * @brief Set gap between hemispheres
         *
         * By default the gap is set to 0. The value is in range 0 - 1 (portion
         * of map width).
         */
        inline void setGap(double _gap)
            { gap = _gap; }

        /**
         * @brief Set central meridian
         *
         * By default the hemispheres are split on 0th meridian (Greenwich).
         * Value is in radians, positive value is east, negative west.
         */
        inline void setCentralMeridian(double _meridian)
            { centralMeridian = _meridian; }

    private:
        Core::Coords<double> stretch, shift;
        double gap, centralMeridian;
};

}}

#endif
