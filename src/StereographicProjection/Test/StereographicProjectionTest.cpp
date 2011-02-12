/*
    Copyright © 2007, 2008, 2009, 2010, 2011 Vladimír Vondruš <mosra@centrum.cz>
    Copyright © 2010 Jan Dupal <dupal.j@seznam.cz>

    This file is part of Kompas.

    Kompas is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License version 3
    only, as published by the Free Software Foundation.

    Kompas is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License version 3 for more details.
*/

#include "StereographicProjectionTest.h"

#include <QtTest/QTest>

#include "LatLonCoords.h"
#include "constants.h"

Q_DECLARE_METATYPE(Kompas::Core::LatLonCoords)
QTEST_APPLESS_MAIN(Kompas::Plugins::Test::StereographicProjectionTest)

using namespace Kompas::Core;

namespace Kompas { namespace Plugins { namespace Test {

StereographicProjectionTest::StereographicProjectionTest(QObject* parent): QObject(parent) {
     projection.setCentralMeridian(-10*PI/180);
     projection.setGap(0.13f);
     projection.setShift(Coords<double>(0.10f, 0.05f));
     projection.setStretch(Coords<double>(0.70f, 0.85f));
}

void StereographicProjectionTest::coords_data() {
    QTest::addColumn<LatLonCoords>("coords");

    QTest::newRow("Greenwich")
        << LatLonCoords(0.0, 0.0);
    QTest::newRow("Prague")
        << LatLonCoords(50.08333, 14.46667);
    QTest::newRow("New York")
        << LatLonCoords(40.7142691, -74.0059729);
    QTest::newRow("Sydney")
        << LatLonCoords(-33.88333, 151.2167);
    QTest::newRow("East")
        << LatLonCoords(0, 179.90);
    QTest::newRow("West")
        << LatLonCoords(0, -179.90);
}

void StereographicProjectionTest::coords() {
    QFETCH(LatLonCoords, coords);

    QVERIFY(projection.toLatLon(projection.fromLatLon(coords)) == coords);
}

}}}
