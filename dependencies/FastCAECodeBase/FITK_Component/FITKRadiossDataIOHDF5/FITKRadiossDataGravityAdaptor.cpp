/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossDataGravityAdaptor.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
//RadiossData
#include "FITK_Component/FITKRadiossData/FITKGravity.h"

namespace Interface
{
    bool FITKRadiossDataGravityAdaptor::adaptR()
    {
        _gravityData = dynamic_cast<Radioss::FITKGravity*>(_dataObj);
        if (_reader == nullptr || _gravityData == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!readRadiossData(_gravityData, *_h5Group)) return false;
        if (!readDataObject(_gravityData, *_h5Group)) return false;
        //// 读取时间函数ID
        //int timeFunctionID = 0;
        //if (!h5tools->readGroupint(*_h5Group, "TimeFunctionID", &timeFunctionID)) return false;
        //_gravityData->setTimeFunctionID(timeFunctionID);
        //// 读取坐标系ID
        //int systemID = 0;
        //if (!h5tools->readGroupint(*_h5Group, "SystemID", &systemID)) return false;
        //_gravityData->setSystemID(systemID);
        //// 读取传感器ID
        //int sensorID = 0;
        //if (!h5tools->readGroupint(*_h5Group, "SensorID", &sensorID)) return false;
        //_gravityData->setSensorID(sensorID);
        // 读取节点组ID
        int nodeGroupID = 0;
        if (!h5tools->readGroupint(*_h5Group, "NodeGroupID", &nodeGroupID)) return false;
        _gravityData->setNodeGroupID(nodeGroupID);
        //// 读取X方向缩放因子
        //double aScaleX = 1.0;
        //if (!h5tools->readGroupDouble(*_h5Group, "AScaleX", &aScaleX)) return false;
        //_gravityData->setAScaleX(aScaleX);
        // 读取Y方向缩放因子
        double fScaleY = 1.0;
        if (!h5tools->readGroupDouble(*_h5Group, "Value", &fScaleY)) return false;
        _gravityData->setFScaleY(fScaleY);
        // 读取重力方向枚举值
        std::string directionStr{};
        if (!h5tools->readGroupStr(*_h5Group, "GravityDirection", directionStr)) return false;

        Core::FITKEnumTransfer<Radioss::FITKGravity::GravityDirection> directionTransfer;
        bool isDirectionValid = false;
        Radioss::FITKGravity::GravityDirection direction
            = directionTransfer.fromString(QString::fromStdString(directionStr), isDirectionValid);
        if (!isDirectionValid) return false;
        _gravityData->setGravityDirection(direction);
        return true;
    }

    bool FITKRadiossDataGravityAdaptor::adaptW()
    {
        _gravityData = dynamic_cast<Radioss::FITKGravity*>(_dataObj);
        if (_writer == nullptr || _gravityData == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!writeRadiossData(_gravityData, *_h5Group))return false;
        if (!writeDataObject(_gravityData, *_h5Group))return false;
        // 获取数据并写入HDF5
        //h5tools->writeGroupAttrInt(*_h5Group, "TimeFunctionID", _gravityData->getTimeFunctionID());
        //h5tools->writeGroupAttrInt(*_h5Group, "SystemID", _gravityData->getSystemID());
        //h5tools->writeGroupAttrInt(*_h5Group, "SensorID", _gravityData->getSensorID());
        h5tools->writeGroupAttrInt(*_h5Group, "NodeGroupID", _gravityData->getNodeGroupID());
        //h5tools->writeGroupAttrDouble(*_h5Group, "AScaleX", _gravityData->getAScaleX());
        h5tools->writeGroupAttrDouble(*_h5Group, "Value", _gravityData->getFScaleY());
        // 处理重力方向枚举值
        Core::FITKEnumTransfer<Radioss::FITKGravity::GravityDirection> directionTransfer;
        bool isDirectionValid = false;
        QString directionStr = directionTransfer.toString(_gravityData->getGravityDirection(), isDirectionValid);
        if (!isDirectionValid) return false;
        h5tools->writeGroupAttrStr(*_h5Group, "GravityDirection", directionStr.toStdString());
        // 写入关键字
        QString keyWord = Radioss::FITKGravity::GetFITKGravityRadiossKeyWord();
        h5tools->writeGroupAttrStr(*_h5Group, "GravityKeyWord", keyWord.toStdString());
        return true;
    }
}
