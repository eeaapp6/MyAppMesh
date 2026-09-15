/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKFlowOFHDF5AdaptorMaterialPoint.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKZonePoints.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"

#include "FITKFlowOFHDF5Reader.h"
#include "FITKFlowOFHDF5Writer.h"


#include <QDebug>
namespace IO
{
    QString FITKFlowOFHDF5AdaptorMaterialPoint::getAdaptorClass()
    {
        return "FITKFlowOFHDF5AdaptorMaterialPoint";
    }

    bool FITKFlowOFHDF5AdaptorMaterialPoint::adaptR()
    {
        _pointData = dynamic_cast<Interface::FITKZonePoint*>(_dataObj);
        if (!_reader || !_pointData) return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        //获取数据
        if (!readDataObject(_pointData, *_h5Group))return false;
        //坐标
        double coor[3] = { 0 };
        tool->readGroupAttrDoubleArray(*_h5Group, "coor", coor, 1, 3);
        _pointData->setCoor(coor[0], coor[1], coor[2]);
        return true;
    }

    bool FITKFlowOFHDF5AdaptorMaterialPoint::adaptW()
    {
        _pointData = dynamic_cast<Interface::FITKZonePoint*>(_dataObj);
        if (!_writer || !_pointData) return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        if (!tool) return false;
        //写出数据
        if (!writeDataObject(_pointData, *_h5Group))return false;
        //坐标
        double coor[3]{ 0 };
        _pointData->getCoor(coor);
        tool->writeGroupAttrDoubleArray(*_h5Group, "coor", coor, 1, 3);
        return true;
    }

}
