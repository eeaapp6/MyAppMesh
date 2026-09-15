/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKFlowOFHDF5AdaptorGeometryMeshSize.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKGeometryMeshSize.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"

#include "FITKFlowOFHDF5Reader.h"
#include "FITKFlowOFHDF5Writer.h"


#include <QDebug>
namespace IO
{
    QString FITKFlowOFHDF5AdaptorGeometryMeshSize::getAdaptorClass()
    {
        return "FITKFlowOFHDF5AdaptorGeometryMeshSize";
    }

    bool FITKFlowOFHDF5AdaptorGeometryMeshSize::adaptR()
    {
        _geoMeshSizeData = dynamic_cast<Interface::FITKGeometryMeshSize*>(_dataObj);
        if (!_reader || !_geoMeshSizeData) return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        //读取数据
        if (!readNDataObject(_geoMeshSizeData, *_h5Group))return false;

        int componentId = 0;
        int layerNum = 0;
        double expansionRatio = 0.0;
        double layerThickness = 0.0;

        tool->readGroupint(*_h5Group, "geoComponentId", &componentId);
        tool->readGroupint(*_h5Group, "layerNumber", &layerNum);
        tool->readGroupDouble(*_h5Group, "expansionRatio", &expansionRatio);
        tool->readGroupDouble(*_h5Group, "layerThickness", &layerThickness);

        _geoMeshSizeData->setGeoGroupComponentId(componentId);
        _geoMeshSizeData->setLayerNumber(layerNum);
        _geoMeshSizeData->setExpansionRatio(expansionRatio);
        _geoMeshSizeData->setLayerThickness(layerThickness);

        return true;
    }

    bool FITKFlowOFHDF5AdaptorGeometryMeshSize::adaptW()
    {
        _geoMeshSizeData = dynamic_cast<Interface::FITKGeometryMeshSize*>(_dataObj);
        if (!_writer || !_geoMeshSizeData) return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        if (!tool) return false;
        //写出数据
        if (!writeNDataObject(_geoMeshSizeData, *_h5Group))return false;

        int componentId = _geoMeshSizeData->getGeoGroupComponentId();
        int layerNum = _geoMeshSizeData->getLayerNumber();
        double expansionRatio = _geoMeshSizeData->getExpansionRatio();
        double layerThickness = _geoMeshSizeData->getLayerThickness();

        tool->writeGroupAttrInt(*_h5Group, "geoComponentId", componentId);
        tool->writeGroupAttrInt(*_h5Group, "layerNumber", layerNum);
        tool->writeGroupAttrDouble(*_h5Group, "expansionRatio", expansionRatio);
        tool->writeGroupAttrDouble(*_h5Group, "layerThickness", layerThickness);
        return true;
    }

}
