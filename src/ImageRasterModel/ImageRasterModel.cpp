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

#include "ImageRasterModel.h"

#include <QtGui/QImage>
#include <QtGui/QPainter>

using namespace std;
using namespace Kompas::Core;

PLUGIN_REGISTER(Kompas::Plugins::ImageRasterModel,
                "cz.mosra.Kompas.Core.AbstractRasterModel/0.2")

namespace Kompas { namespace Plugins {

#define __base      "base"

ImageRasterModel::ImageRasterModel(PluginManager::AbstractPluginManager* manager, const string& plugin): AbstractRasterModel(manager, plugin), _tileSize(0, 0), output(0) {
    _fileExtensions.push_back("*.png");
    _fileExtensions.push_back("*.jpg");
    _fileExtensions.push_back("*.gif");
    _zoomLevels.insert(0);
    _layers.push_back("base");
}

ImageRasterModel::Output::Output(const QString& filename, const TileSize& tileSize, const TileArea& area): filename(filename), tileSize(tileSize), area(area), image(tileSize.x*area.w, tileSize.y*area.h, QImage::Format_RGB32) {
    image.fill(QColor(Qt::black).rgb());
}

bool ImageRasterModel::initializePackage(const string& filename, const TileSize& tileSize, const vector<Zoom>& zoomLevels, const TileArea& area, const vector<string>& layers, const vector<string>& overlays) {
    /* If the image is already initialized, exit. Also only one zoom level,
       layer and no overlay can be saved to image */
    if(output || zoomLevels.size() != 1 || layers.size() != 1 || !overlays.empty()) return false;

    output = new Output(QString::fromStdString(filename), tileSize, area);
    if(!output->painter.begin(&output->image)) {
        delete output;
        output = 0;
        return false;
    }

    return true;
}

bool ImageRasterModel::tileToPackage(const string& layer, Zoom z, const TileCoords& coords, const string& data) {
    if(!output) return false;

    output->painter.drawImage(QRect(output->tileSize.x*(coords.x-output->area.x), output->tileSize.y*(coords.y-output->area.y), output->tileSize.x, output->tileSize.y), QImage::fromData(QByteArray(data.c_str(), data.size())));

    return true;
}

bool ImageRasterModel::finalizePackage() {
    if(!output) return false;

    bool ret = output->painter.end() && output->image.save(output->filename);
    delete output;
    output = 0;
    return ret;
}

}}
