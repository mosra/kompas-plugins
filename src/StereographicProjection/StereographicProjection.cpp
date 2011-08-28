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

PLUGIN_REGISTER(Kompas::Plugins::StereographicProjection,
                "cz.mosra.Kompas.Core.AbstractProjection/0.2")

namespace Kompas { namespace Plugins {

Coords<double> StereographicProjection::fromLatLon(const LatLonCoords& coords) const {
    double longitude = coords.longitude()-centralMeridian;

    /* Get shifted longitude into limits */
    if(longitude < -180) longitude += 360;
    else if (longitude > 180) longitude -= 360;

    /* Slightly different computation for both hemispheres */
    Coords<double> _coords;

    /* Left (western) hemisphere */
    if(longitude < 0) {
        /* Convert longitude from range -180° - 0° to -90° - 90° */
        longitude += 90;

        /* X coordinate in left hemisphere center */
        _coords.x = 0.25;

    /* Right (eastern) hemisphere */
    } else {
        /* Convert longitude from range 0° - 180° to -90° - 90° */
        longitude -= 90;

        /* X coordinate in right hemisphere center */
        _coords.x = 0.75;
    }

    /* Calculate point on a sphere from latitude and longitude */
    double x, y, z;
    LatLonCoords(coords.latitude(), longitude).toPointOnSphere(&x, &y, &z);

    /* Calculate point on a surface (y and z are flipped) */
    _coords.x += (x/(1 + z))/4;
    _coords.y = 0.5 - (y/(1 + z))/2;

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

LatLonCoords StereographicProjection::toLatLon(const Coords<double>& coords) const {
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
    _coords.y = 2*_coords.y-1;

    double longitude;

    /* Left (western) hemisphere */
    if(_coords.x < 0.5) {
        /* Convert X from range (0 - 0.5) to (-1 - 1) */
        _coords.x = 4*(_coords.x - 0.25);

        /* Longitude in left hemisphere center */
        longitude = -90;

    /* Right (eastern) hemisphere */
    } else {
        /* Convert X from range (0.5 - 1) to (-1 - 1) */
        _coords.x = 4*(_coords.x - 0.75);

        /* Longitude in right hemisphere center */
        longitude = 90;
    }

    /* Calculate x, y, z coordinates of point on a sphere */
    double divisor = 1+_coords.x*_coords.x+_coords.y*_coords.y;
    double x = 2*_coords.x/divisor;
    double y = 2*_coords.y/divisor;
    double z = 1 - 2/divisor;

    /* Coordinates from opposite hemisphere, no map available, return
        invalid coordinates */
    if(z > 0) return LatLonCoords();

    /* Calculate latitude and longitude (y and z is flipped), apply central
       meridian */
    LatLonCoords c = LatLonCoords::fromPointOnSphere(x, -y, -z);
    longitude += c.longitude()+centralMeridian;

    /* Get shifted longitude into limits */
    if(longitude > 180) longitude -= 360;
    else if(longitude < -180) longitude += 360;

    /* Convert from radians and return */
    return LatLonCoords(c.latitude(), longitude);
}

}}
