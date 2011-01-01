#ifndef Kompas_Plugins_StereographicProjection_h
#define Kompas_Plugins_StereographicProjection_h
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
 * @brief Class Kompas::Plugins::StereographicProjection
 */

#include "AbstractProjection.h"

namespace Kompas { namespace Plugins {

/**
@brief Plugin for stereographic map projection

Simplified for maps divided into two hemispheres with poles on top and
bottom, western hemisphere on left, eastern on right. Customizable shift and
stretch, central meridian (on which are the hemispheres split) and gap
between hemispheres.
@section StereographicProjection_fromWgs84 Converting from WGS84 coordinates
Conversion is basically the same for both hemispheres. After removing
central meridian shift (so the hemispheres are split at 0° meridian), left
hemisphere is in range @f$ [ - \pi ; 0 ] @f$ and right in @f$ [ 0 ; \pi ] @f$.
The coordinates are converted to range @f$ [ - {\pi \over 2} ; {\pi \over 2} ] @f$
for both hemispheres. Then they need to be converted from latitude / longtitude
to coordinates of a point on a sphere @f$ (x, y, z) @f$. Longtitude will be on
@f$ x @f$ and latitude on @f$ y @f$:
@f[
    \begin{array}{lcl}
        x & = & \sin longtitude \cos latitude       \\
        y & = & - \sin latitude                     \\
        z & = & - \cos longtitude \cos latitude
    \end{array}
@f]
Conversion to plane coordinates (according to
<a href="http://en.wikipedia.org/wiki/Stereographic_projection">Wikipedia</a>):
@f[
    (X, Y) = \left( \frac{x}{1 - z}, \frac{y}{1 - z} \right)
@f]
Coordinates are in range @f$ [ -1 ; 1 ] @f$, so X coordinates are converted to
@f$ [ 0; 0.5 ] @f$ if the longtitude was in left and @f$ [ 0.5; 1 ] @f$ if it
was in right hemisphere, Y coordinates are converted to range @f$ [ 0 ; 1 ] @f$.
Last step is to apply shift, stretch and gap between hemispheres.
@section StereographicProjection_toWgs84 Converting to WGS84 coordinates
First step is to remove shift, stretch and gap between hemispheres, then
converting both Y coordinates from @f$ [ 0 ; 1 ] @f$ and X coordinates from
@f$ [ 0 ; 0.5 ] @f$ (for left hemisphere) or @f$ [ 0.5 ; 1 ] @f$ (for right
hemisphere) to @f$ [ -1 ; 1 ] @f$. Then they are converted to
coordinates of a point on a sphere (according to
<a href="http://en.wikipedia.org/wiki/Stereographic_projection">Wikipedia</a>):
@f[
    (x, y, z) = \left(
        \frac{2X}{1 + X^2 + Y^2},
        \frac{2Y}{1 + X^2 + Y^2},
        \frac{-1 + X^2 + Y^2}{1 + X^2 + Y^2}
    \right)
@f]
Because on the map there are only two circles with actual map data and all other
area is blank, invalid coordinates must be returned for these areas. When
z coordinate is greater than zero, it indicates that the point is from opposite
hemisphere than the displayed one, so the function returns invalid coordinates.

Conversion of @f$ (x, y, z) @f$ coordinates to latitude and longtitude:
@f[
    \begin{array}{lcl}
        latitude & = & \arcsin y              \\
        longtitude & = & \arcsin {x \over \sqrt {x^2 + z^2}}
    \end{array}
@f]
Latitude is in range @f$ [ - {\pi \over 2} ; {\pi \over 2} ] @f$, which is what is
needed, but longtitude needs to be converted to @f$ [ - \pi ; 0 ] @f$ for left
hemisphere or @f$ [ 0 ; \pi ] @f$ for right hemisphere. Last step is to apply
central meridian shift and get longtitude into limits, if needed.
 */
class StereographicProjection: public Core::AbstractProjection {
    public:
        /** @copydoc Core::AbstractProjection::AbstractProjection */
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
