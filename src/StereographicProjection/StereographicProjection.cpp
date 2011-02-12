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

#include "StereographicProjection.h"

#include <cmath>
#include <iostream>

#include "constants.h"

using namespace std;
using namespace Kompas::Core;

namespace Kompas { namespace Plugins {

Coords<double> StereographicProjection::fromWgs84(const Wgs84Coords& coords) const {
    /* Convert coordinates to radians */
    double latitude = coords.latitude()*PI/180;
    double longitude = coords.longitude()*PI/180-centralMeridian;

    /* Get shifted longitude into limits */
    if(longitude < -PI) longitude += 2*PI;
    else if (longitude > PI) longitude -= 2*PI;

    /* Slightly different computation for both hemispheres */
    Coords<double> _coords;

    /* Left (western) hemisphere */
    if(longitude < 0) {
        /* Convert longitude from range -180° - 0° to -90° - 90° */
        longitude += PI/2;

        /* X coordinate in left hemisphere center */
        _coords.x = 0.25;

    /* Right (eastern) hemisphere */
    } else {
        /* Convert longitude from range 0° - 180° to -90° - 90° */
        longitude -= PI/2;

        /* X coordinate in right hemisphere center */
        _coords.x = 0.75;
    }

    /* Calculate point on a sphere from latitude and longitude */
    double x = sin(longitude)*cos(latitude);
    double y = -sin(latitude);
    double z = -cos(longitude)*cos(latitude);

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

    double longitude;

    /* Left (western) hemisphere */
    if(_coords.x < 0.5) {
        /* Convert X from range (0 - 0.5) to (-1 - 1) */
        _coords.x = 4*(_coords.x - 0.25);

        /* Longtitude in left hemisphere center */
        longitude = -PI/2;

    /* Right (eastern) hemisphere */
    } else {
        /* Convert X from range (0.5 - 1) to (-1 - 1) */
        _coords.x = 4*(_coords.x - 0.75);

        /* Longtitude in right hemisphere center */
        longitude = PI/2;
    }

    /* Calculate x, y, z coordinates of point on a sphere */
    double divisor = 1+_coords.x*_coords.x+_coords.y*_coords.y;
    double x = 2*_coords.x/divisor;
    double y = 2*_coords.y/divisor;
    double z = 1 - 2/divisor;

    /* Coordinates from opposite hemisphere, no map available, return
        invalid coordinates */
    if(z > 0) return Wgs84Coords();

    /* Calculate latitude and longitude, apply central meridian */
    double latitude = asin(y);
    longitude += asin(x/sqrt(x*x+z*z))+centralMeridian;

    /* Get shifted longitude into limits */
    if(longitude > PI) longitude -= 2*PI;
    else if(longitude < -PI) longitude += 2*PI;

    /* Convert from radians and return */
    return Wgs84Coords(latitude*180/PI, longitude*180/PI);
}

}}

PLUGIN_REGISTER(Kompas::Plugins::StereographicProjection,
                "cz.mosra.Kompas.Core.AbstractProjection/0.1")
