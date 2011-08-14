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

#include "FilesystemCacheTest.h"

#include <QtCore/QDir>
#include <QtTest/QTest>

#include "Utility/Directory.h"

#include "FilesystemCacheTestConfigure.h"

QTEST_APPLESS_MAIN(Kompas::Plugins::Test::FilesystemCacheTest)

using namespace Kompas::Utility;

namespace Kompas { namespace Plugins { namespace Test {

void FilesystemCacheTest::save() {
    QFile index(QString::fromStdString(Directory::join(FILESYSTEMCACHE_WRITE_TEST_DIR, "index.kps")));
    index.remove();

    Cache cache;
    cache.setBlockSize(5);
    cache.setCacheSize(15);
    cache.initializeCache(FILESYSTEMCACHE_WRITE_TEST_DIR);
    cache.set("a", "data1");
    cache.set("b", "data02");
    cache.set("c", "data1");
    QVERIFY(cache.get("b") == "data02");
    cache.finalizeCache();

    /* Index */
    QFile indexExpected(QString::fromStdString(Directory::join(FILESYSTEMCACHE_TEST_DIR, "index.kps")));
    indexExpected.open(QFile::ReadOnly);
    QVERIFY(index.open(QFile::ReadOnly));
    QCOMPARE(index.readAll(), indexExpected.readAll());
    indexExpected.close();
    index.close();

    QFile a(QString::fromStdString(Directory::join(FILESYSTEMCACHE_WRITE_TEST_DIR, "cb/cc2ff6a0894e6e7f9a1a6a6a36b68fb36aa151")));
    QVERIFY(a.open(QFile::ReadOnly));
    QByteArray dataA = a.readAll();
    QVERIFY(dataA == "data1");
    a.close();

    QFile b(QString::fromStdString(Directory::join(FILESYSTEMCACHE_WRITE_TEST_DIR, "92/ad4a1c5b3c423a4c3b48cf74cadf93225be4ef")));
    QVERIFY(b.open(QFile::ReadOnly));
    QByteArray dataB = b.readAll();
    QVERIFY(dataB == "data02");
    b.close();
}

void FilesystemCacheTest::loadSave() {
    Cache cache;
    cache.initializeCache(FILESYSTEMCACHE_WRITE_TEST_DIR);
    QVERIFY(cache.blockSize() == 5);
    QVERIFY(cache.cacheSize() == 15);
    QVERIFY(cache.usedSize() == 15);
    cache.finalizeCache();

    QFile index(QString::fromStdString(Directory::join(FILESYSTEMCACHE_WRITE_TEST_DIR, "index.kps")));
    QFile indexExpected(QString::fromStdString(Directory::join(FILESYSTEMCACHE_TEST_DIR, "index.kps")));
    indexExpected.open(QFile::ReadOnly);
    QVERIFY(index.open(QFile::ReadOnly));
    QCOMPARE(index.readAll(), indexExpected.readAll());
    indexExpected.close();
    index.close();
}

void FilesystemCacheTest::optimize() {
    QFile a(QString::fromStdString(Directory::join(FILESYSTEMCACHE_WRITE_TEST_DIR, "92/ad4a1c5b3c423a4c3b48cf74cadf93225be4ef")));
    a.rename(QString::fromStdString(Directory::join(FILESYSTEMCACHE_WRITE_TEST_DIR, "92/bd4a1c5b3c423a4c3b48cf74cadf93225be4ef")));

    QDir dir;
    QVERIFY(dir.exists(QString::fromStdString(Directory::join(FILESYSTEMCACHE_WRITE_TEST_DIR, "92"))));

    Cache cache;
    cache.initializeCache(FILESYSTEMCACHE_WRITE_TEST_DIR);
    QCOMPARE(cache.usedSize(), 15ul);
    cache.optimize();
    QCOMPARE(cache.usedSize(), 5ul);

    QVERIFY(!dir.exists(QString::fromStdString(Directory::join(FILESYSTEMCACHE_WRITE_TEST_DIR, "92"))));
}

void FilesystemCacheTest::purge() {
    QDir dir;
    QVERIFY(dir.exists(QString::fromStdString(Directory::join(FILESYSTEMCACHE_WRITE_TEST_DIR, "cb"))));

    Cache cache;
    cache.initializeCache(FILESYSTEMCACHE_WRITE_TEST_DIR);
    cache.purge();

    QVERIFY(!dir.exists(QString::fromStdString(Directory::join(FILESYSTEMCACHE_WRITE_TEST_DIR, "cb"))));
}

}}}
