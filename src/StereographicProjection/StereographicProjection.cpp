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

#include "StereographicProjection.h"

#include <cmath>
#include <iostream>

#include "basictypes.h"

using namespace std;
using namespace Map2X::Core;

namespace Map2X { namespace Plugins {

Coords<double> StereographicProjection::fromWgs84(const Wgs84Coords& coords) const {
    /* Convert coordinates to radians */
    double latitude = coords.latitude()*PI/180;
    double longtitude = coords.longtitude()*PI/180;

    /* Remove central meridian shift */
    longtitude -= centralMeridian;

    /* Slightly different computation for both hemispheres */
    Coords<double> _coords;

    /* Left (western) hemisphere */
    if(longtitude < 0) {
        /* Convert longtitude from range -180° - 0° to -90° - 90° */
        longtitude += PI/2;

        /* X coordinate in left hemisphere center */
        _coords.x = 0.25;

    /* Right (eastern) hemisphere */
    } else {
        /* Convert longtitude from range 0° - 180° to -90° - 90° */
        longtitude -= PI/2;

        /* X coordinate in right hemisphere center */
        _coords.x = 0.75;
    }

    /* Calculate point on a sphere from latitude and longtitude */
    double x = sin(longtitude)*cos(latitude);
    double y = -sin(latitude);
    double z = -cos(longtitude)*cos(latitude);

    /* Calculate point on a surface */
    _coords.x += (x/(1 - z))/4;
    _coords.y = 0.5 + (y/(1 - z))/2;

    /* Apply stretch */
    _coords.x *= stretch.x;
    _coords.y *= stretch.y;

    /* Apply gap between hemispheres */
    if(_coords.x > stretch.x/2) _coords.x += gap;

    /* Add shift */
    _coords.x += shift.x;
    _coords.y += shift.y;

    return _coords;
}

Wgs84Coords StereographicProjection::toWgs84(const Coords<double>& coords) const {
    Coords<double> _coords = coords;

    /* Remove shift */
    _coords.x -= shift.x;
    _coords.y -= shift.y;

    /* Remove gap between hemispheres */
    if(_coords.x > stretch.x/2) _coords.x -= gap;

    /* Remove stretch */
    _coords.x /= stretch.x;
    _coords.y /= stretch.y;

    /* Convert Y from range (0 - 1) to (-1 - 1) */
    _coords.y = 2*(0.5 - _coords.y);

    double longtitude;

    /* Left (western) hemisphere */
    if(_coords.x < 0.5) {
        /* Convert X from range (0 - 0.5) to (-1 - 1) */
        _coords.x = 4*(_coords.x - 0.25);

        /* Longtitude in left hemisphere center */
        longtitude = -PI/2;

    /* Right (eastern) hemisphere */
    } else {
        /* Convert X from range (0.5 - 1) to (-1 - 1) */
        _coords.x = 4*(_coords.x - 0.75);

        /* Longtitude in right hemisphere center */
        longtitude = PI/2;
    }

    /* Calculate x, y, z coordinates of point on a sphere */
    double divisor = 1+_coords.x*_coords.x+_coords.y*_coords.y;
    double x = 2*_coords.x/divisor;
    double y = 2*_coords.y/divisor;
    double z = 1 - 2/divisor;

    /* Coordinates from opposite hemisphere, no map available, return
        invalid coordinates */
    if(z > 0) return Wgs84Coords();

    /* Calculate latitude and longtitude, apply central meridian */
    double latitude = asin(y);
    longtitude += asin(x/sqrt(x*x+z*z))+centralMeridian;

    /* Get it into limits */
    if(abs(longtitude) > PI) longtitude = -longtitude-PI;

    /* Convert from radians and return */
    return Wgs84Coords(latitude*180/PI, longtitude*180/PI);
}

}}

PLUGIN_REGISTER(Map2X::Plugins::StereographicProjection,
                "cz.mosra.Map2X.Core.AbstractProjection/0.1")
