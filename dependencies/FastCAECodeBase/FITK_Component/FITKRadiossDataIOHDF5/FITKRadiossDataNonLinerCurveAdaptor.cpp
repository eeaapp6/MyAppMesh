/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossDataNonLinerCurveAdaptor.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
//RadiossData
#include "FITK_Component/FITKRadiossData/FITKNonLinerCurve.h"

namespace Interface
{
    bool FITKRadiossDataNonLinerCurveAdaptor::adaptR()
    {
        _curveData = dynamic_cast<Radioss::FITKNonLinerCurve*>(_dataObj);
        if (_reader == nullptr || _curveData == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!readRadiossData(_curveData, *_h5Group)) return false;
        if (!readDataObject(_curveData, *_h5Group)) return false;
        // 读取点的数量
        int pointCount = 0;
        if (!h5tools->readGroupint(*_h5Group, "PointCount", &pointCount)) return false;
        if (pointCount <= 0) return true; // 没有点数据
        // 读取x坐标数组
        std::vector<double> xCoords(pointCount);
        if (!h5tools->readGroupAttrDoubleArray(*_h5Group, "XCoordinates", xCoords.data(), 1, pointCount))return false;
        // 读取y坐标数组
        std::vector<double> yCoords(pointCount);
        if (!h5tools->readGroupAttrDoubleArray(*_h5Group, "YCoordinates", yCoords.data(), 1, pointCount))return false;
        // 清空现有点并添加新读取的点
        _curveData->removeAllPoints();
        for (int i = 0; i < pointCount; ++i)
        {
            Radioss::FITKNonLinerCurve::FITKNonLinerCurvePoint point;
            point.x = xCoords[i];
            point.y = yCoords[i];
            _curveData->addPoint(point);
        }
        return true;
    }

    bool FITKRadiossDataNonLinerCurveAdaptor::adaptW()
    {
        _curveData = dynamic_cast<Radioss::FITKNonLinerCurve*>(_dataObj);
        if (_writer == nullptr || _curveData == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!writeRadiossData(_curveData, *_h5Group))return false;
        if (!writeDataObject(_curveData, *_h5Group))return false;
        // 获取曲线点数据
        int pointCount = _curveData->getPointCount();
        h5tools->writeGroupAttrInt(*_h5Group, "PointCount", pointCount);
        if (pointCount <= 0) return true; // 没有点数据需要写入
        // 准备x和y坐标数组
        std::vector<double> xCoords(pointCount);
        std::vector<double> yCoords(pointCount);
        for (int i = 0; i < pointCount; ++i)
        {
            Radioss::FITKNonLinerCurve::FITKNonLinerCurvePoint point = _curveData->getPoint(i);
            xCoords[i] = point.x;
            yCoords[i] = point.y;
        }
        // 写入坐标数据到HDF5
        h5tools->writeGroupAttrDoubleArray(*_h5Group, "XCoordinates", xCoords.data(), 1, pointCount);
        h5tools->writeGroupAttrDoubleArray(*_h5Group, "YCoordinates", yCoords.data(), 1, pointCount);
        return true;
    }
}
