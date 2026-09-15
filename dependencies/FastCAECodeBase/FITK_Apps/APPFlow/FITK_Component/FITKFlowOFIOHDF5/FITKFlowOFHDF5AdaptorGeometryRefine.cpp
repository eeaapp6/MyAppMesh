/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKFlowOFHDF5AdaptorGeometryRefine.h"
#include "FITKFlowOFHDF5Reader.h"
#include "FITKFlowOFHDF5Writer.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKRegionGeometryRefine.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
#include "FITKFlowOFHDF5Reader.h"
#include "FITKFlowOFHDF5Writer.h"


#include <QDebug>
namespace IO
{
    QString FITKFlowOFHDF5AdaptorGeometryRefine::getAdaptorClass()
    {
        return "FITKFlowOFHDF5AdaptorGeometry";
    }

    bool FITKFlowOFHDF5AdaptorGeometryRefine::adaptR()
    {
        _geoRefineData = dynamic_cast<Interface::FITKRegionGeometryRefine*>(_dataObj);
        if (!_reader || !_geoRefineData) return false;
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;

        readDataObject(_geoRefineData, *_h5Group);

        int geoId = 0;
        bool limitRefineMent = false;
        int refineModel = 0;
        double distance = 0.0;
        int level = 0;
        int gapRefineModel = 0;
        int cellsInGapNum = 0;
        int maxGapLevel = 0;

        tool->readGroupint(*_h5Group, "geometryId", &geoId);
        tool->readGroupBool(*_h5Group, "limitRefineMent", &limitRefineMent);
        tool->readGroupint(*_h5Group, "refineModel", &refineModel);
        tool->readGroupDouble(*_h5Group, "distance", &distance);
        tool->readGroupint(*_h5Group, "level", &level);
        tool->readGroupint(*_h5Group, "gapRefineModel", &gapRefineModel);
        tool->readGroupint(*_h5Group, "cellsInGapNum", &cellsInGapNum);
        tool->readGroupint(*_h5Group, "maxGapLevel", &maxGapLevel);

        _geoRefineData->setGeometryId(geoId);
        _geoRefineData->setLimitRefinement(limitRefineMent);
        _geoRefineData->setRefineModel(Interface::FITKRegionGeometryRefine::RefineModelType(refineModel));
        _geoRefineData->setDistance(distance);
        _geoRefineData->setLevel(level);
        _geoRefineData->setGapRefineModel(Interface::FITKRegionGeometryRefine::GapModelType(gapRefineModel));
        _geoRefineData->setCellsInGapNum(cellsInGapNum);
        _geoRefineData->setMaxGapLevel(maxGapLevel);
        return true;
    }

    bool FITKFlowOFHDF5AdaptorGeometryRefine::adaptW()
    {
        _geoRefineData = dynamic_cast<Interface::FITKRegionGeometryRefine*>(_dataObj);
        if (!_writer || !_geoRefineData) return false;
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        if (!tool) return false;
        if (!_geoRefineData->getGeometryModel())return false;

        this->writeDataObject(_dataObj, *_h5Group);

        int geoId = _geoRefineData->getGeometryId();
        bool limitRefineMent = _geoRefineData->getLimitRefineMent();
        int refineModel = _geoRefineData->getRefineModel();
        double distance = _geoRefineData->getDistance();
        int level = _geoRefineData->getLevel();
        int gapRefineModel = _geoRefineData->getGapRefineModel();
        int cellsInGapNum = _geoRefineData->getCellsInGapNum();
        int maxGapLevel = _geoRefineData->getMaxGapLevel();

        tool->writeGroupAttrInt(*_h5Group, "geometryId", geoId);
        tool->writeGroupAttrBool(*_h5Group, "limitRefineMent", limitRefineMent);
        tool->writeGroupAttrInt(*_h5Group, "refineModel", refineModel);
        tool->writeGroupAttrDouble(*_h5Group, "distance", distance);
        tool->writeGroupAttrInt(*_h5Group, "level", level);
        tool->writeGroupAttrInt(*_h5Group, "gapRefineModel", gapRefineModel);
        tool->writeGroupAttrInt(*_h5Group, "cellsInGapNum", cellsInGapNum);
        tool->writeGroupAttrInt(*_h5Group, "maxGapLevel", maxGapLevel);

        return true;
    }

}
