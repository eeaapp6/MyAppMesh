/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossDataRWallCylinderAdaptor.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
//RadiossData
#include "FITK_Component/FITKRadiossData/FITKRWall.h"

namespace Interface
{
    bool FITKRadiossDataRWallCylinderAdaptor::adaptR()
    {
        _wallCylinderData = dynamic_cast<Radioss::FITKRWallCylinder*>(_dataObj);
        if (_reader == nullptr || _wallCylinderData == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!readRadiossData(_wallCylinderData, *_h5Group)) return false;
        if (!readDataObject(_wallCylinderData, *_h5Group)) return false;
        // 参考节点ID
        int nodeId = 0;
        if (!h5tools->readGroupint(*_h5Group, "NodeId", &nodeId)) return false;
        _wallCylinderData->setNodeId(nodeId);
        // 滑移类型
        std::string slidingTypeStr{};
        if (!h5tools->readGroupStr(*_h5Group, "SlidingType", slidingTypeStr)) return false;
        Core::FITKEnumTransfer<Radioss::FITKAbstractRWall::RWallSliding> slidingTransfer;
        bool isSlidingValid = false;
        auto slidingType = slidingTransfer.fromString(
            QString::fromStdString(slidingTypeStr), isSlidingValid);
        if (!isSlidingValid) return false;
        _wallCylinderData->setSlidingType(slidingType);
        // 第一节点集合ID
        int nodeGroupId1 = 0;
        if (!h5tools->readGroupint(*_h5Group, "NodeGroupId1", &nodeGroupId1)) return false;
        _wallCylinderData->setNodeGroupId1(nodeGroupId1);
        // 第二节点集合ID
        int nodeGroupId2 = 0;
        if (!h5tools->readGroupint(*_h5Group, "NodeGroupId2", &nodeGroupId2)) return false;
        _wallCylinderData->setNodeGroupId2(nodeGroupId2);
        // 搜索距离
        double dSearch = 0.0;
        if (!h5tools->readGroupDouble(*_h5Group, "DSearch", &dSearch)) return false;
        _wallCylinderData->setDSearch(dSearch);
        // 摩擦系数
        double frictionCoef = 0.0;
        if (!h5tools->readGroupDouble(*_h5Group, "FrictionCoef", &frictionCoef)) return false;
        _wallCylinderData->setFrictionCoef(frictionCoef);
        // 特征直径
        double diameter = 0.0;
        if (!h5tools->readGroupDouble(*_h5Group, "Diameter", &diameter)) return false;
        _wallCylinderData->setDiameter(diameter);
        // 滤波因子
        double filteringFactor = 0.0;
        if (!h5tools->readGroupDouble(*_h5Group, "FilteringFactor", &filteringFactor)) return false;
        _wallCylinderData->setFilteringFactor(filteringFactor);
        // 滤波标志
        int filteringFlag = 0;
        if (!h5tools->readGroupint(*_h5Group, "FilteringFlag", &filteringFlag)) return false;
        _wallCylinderData->setFilteringFlag(filteringFlag);
        // 
        double basicX = 0.0, basicY = 0.0, basicZ = 0.0;
        if (!h5tools->readGroupDouble(*_h5Group, "BasicPointX", &basicX) ||
            !h5tools->readGroupDouble(*_h5Group, "BasicPointY", &basicY) ||
            !h5tools->readGroupDouble(*_h5Group, "BasicPointZ", &basicZ))
            return false;
        _wallCylinderData->setBasicPoint(basicX, basicY, basicZ);
        // 
        double AxisX = 0.0, AxisY = 0.0, AxisZ = 0.0;
        if (!h5tools->readGroupDouble(*_h5Group, "AxisVectorX", &AxisX) ||
            !h5tools->readGroupDouble(*_h5Group, "AxisVectorY", &AxisY) ||
            !h5tools->readGroupDouble(*_h5Group, "AxisVectorZ", &AxisZ))
            return false;
        _wallCylinderData->setAxisVector(AxisX, AxisY, AxisZ);
        return true;
    }

    bool FITKRadiossDataRWallCylinderAdaptor::adaptW()
    {
        _wallCylinderData = dynamic_cast<Radioss::FITKRWallCylinder*>(_dataObj);
        if (_writer == nullptr || _wallCylinderData == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!writeRadiossData(_wallCylinderData, *_h5Group))return false;
        if (!writeDataObject(_wallCylinderData, *_h5Group))return false;
        //获取数据
        h5tools->writeGroupAttrInt(*_h5Group, "NodeId", _wallCylinderData->getNodeId());
        // 滑移类型
        Core::FITKEnumTransfer<Radioss::FITKAbstractRWall::RWallSliding> slidingTransfer;
        bool isSlidingValid = false;
        QString slidingTypeStr = slidingTransfer.toString(
            _wallCylinderData->getSlidingType(), isSlidingValid);
        if (!isSlidingValid) return false;
        h5tools->writeGroupAttrStr(*_h5Group, "SlidingType", slidingTypeStr.toStdString());
        // 第一节点集合ID
        h5tools->writeGroupAttrInt(*_h5Group, "NodeGroupId1", _wallCylinderData->getNodeGroupId1());
        // 第二节点集合ID
        h5tools->writeGroupAttrInt(*_h5Group, "NodeGroupId2", _wallCylinderData->getNodeGroupId2());
        // 搜索距离
        h5tools->writeGroupAttrDouble(*_h5Group, "DSearch", _wallCylinderData->getDSearch());
        // 摩擦系数
        h5tools->writeGroupAttrDouble(*_h5Group, "FrictionCoef", _wallCylinderData->getFrictionCoef());
        // 特征直径
        h5tools->writeGroupAttrDouble(*_h5Group, "Diameter", _wallCylinderData->getDiameter());
        // 滤波因子
        h5tools->writeGroupAttrDouble(*_h5Group, "FilteringFactor", _wallCylinderData->getFilteringFactor());
        // 滤波标志
        h5tools->writeGroupAttrInt(*_h5Group, "FilteringFlag", _wallCylinderData->getFilteringFlag());
        // 
        double basicX = 0.0, basicY = 0.0, basicZ = 0.0;
        _wallCylinderData->getBasicPoint(basicX, basicY, basicZ);
        h5tools->writeGroupAttrDouble(*_h5Group, "BasicPointX", basicX);
        h5tools->writeGroupAttrDouble(*_h5Group, "BasicPointY", basicY);
        h5tools->writeGroupAttrDouble(*_h5Group, "BasicPointZ", basicZ);
        // 
        double AxisX = 0.0, AxisY = 0.0, AxisZ = 0.0;
        _wallCylinderData->getAxisVector(AxisX, AxisY, AxisZ);
        h5tools->writeGroupAttrDouble(*_h5Group, "AxisVectorX", AxisX);
        h5tools->writeGroupAttrDouble(*_h5Group, "AxisVectorY", AxisY);
        h5tools->writeGroupAttrDouble(*_h5Group, "AxisVectorZ", AxisZ);
        // 写入关键字
        QString rwallKeyWord = Radioss::FITKRWallCylinder::GetFITKRWallCylinderRadiossKeyWord();
        h5tools->writeGroupAttrStr(*_h5Group, "RWallKeyWord", rwallKeyWord.toStdString());
        return true;
    }
}
