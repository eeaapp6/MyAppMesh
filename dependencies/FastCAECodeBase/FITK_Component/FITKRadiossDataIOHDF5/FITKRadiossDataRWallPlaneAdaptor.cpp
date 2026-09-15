/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossDataRWallPlaneAdaptor.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
//RadiossData
#include "FITK_Component/FITKRadiossData/FITKRWall.h"

namespace Interface
{
    bool FITKRadiossDataRWallPlaneAdaptor::adaptR()
    {
        _wallPlaneData = dynamic_cast<Radioss::FITKRWallPlane*>(_dataObj);
        if (_reader == nullptr || _wallPlaneData == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!readRadiossData(_wallPlaneData, *_h5Group)) return false;
        if (!readDataObject(_wallPlaneData, *_h5Group)) return false;
        // 参考节点ID
        int nodeId = 0;
        if (!h5tools->readGroupint(*_h5Group, "NodeId", &nodeId)) return false;
        _wallPlaneData->setNodeId(nodeId);
        // 滑移类型
        std::string slidingTypeStr{};
        if (!h5tools->readGroupStr(*_h5Group, "SlidingType", slidingTypeStr)) return false;
        Core::FITKEnumTransfer<Radioss::FITKAbstractRWall::RWallSliding> slidingTransfer;
        bool isSlidingValid = false;
        auto slidingType = slidingTransfer.fromString(
            QString::fromStdString(slidingTypeStr), isSlidingValid );
        if (!isSlidingValid) return false;
        _wallPlaneData->setSlidingType(slidingType);
        // 第一节点集合ID
        int nodeGroupId1 = 0;
        if (!h5tools->readGroupint(*_h5Group, "NodeGroupId1", &nodeGroupId1)) return false;
        _wallPlaneData->setNodeGroupId1(nodeGroupId1);
        // 第二节点集合ID
        int nodeGroupId2 = 0;
        if (!h5tools->readGroupint(*_h5Group, "NodeGroupId2", &nodeGroupId2)) return false;
        _wallPlaneData->setNodeGroupId2(nodeGroupId2);
        // 搜索距离
        double dSearch = 0.0;
        if (!h5tools->readGroupDouble(*_h5Group, "DSearch", &dSearch)) return false;
        _wallPlaneData->setDSearch(dSearch);
        // 摩擦系数
        double frictionCoef = 0.0;
        if (!h5tools->readGroupDouble(*_h5Group, "FrictionCoef", &frictionCoef)) return false;
        _wallPlaneData->setFrictionCoef(frictionCoef);
        // 特征直径
        double diameter = 0.0;
        if (!h5tools->readGroupDouble(*_h5Group, "Diameter", &diameter)) return false;
        _wallPlaneData->setDiameter(diameter);
        // 滤波因子
        double filteringFactor = 0.0;
        if (!h5tools->readGroupDouble(*_h5Group, "FilteringFactor", &filteringFactor)) return false;
        _wallPlaneData->setFilteringFactor(filteringFactor);
        // 滤波标志
        int filteringFlag = 0;
        if (!h5tools->readGroupint(*_h5Group, "FilteringFlag", &filteringFlag)) return false;
        _wallPlaneData->setFilteringFlag(filteringFlag);
        // 平面基点坐标
        double basicX = 0.0, basicY = 0.0, basicZ = 0.0;
        if (!h5tools->readGroupDouble(*_h5Group, "BasicPointX", &basicX) ||
            !h5tools->readGroupDouble(*_h5Group, "BasicPointY", &basicY) ||
            !h5tools->readGroupDouble(*_h5Group, "BasicPointZ", &basicZ))
            return false;
        _wallPlaneData->setBasicPoint(basicX, basicY, basicZ);
        // 平面法向量
        double normalX = 0.0, normalY = 0.0, normalZ = 0.0;
        if (!h5tools->readGroupDouble(*_h5Group, "NormalVectorX", &normalX) ||
            !h5tools->readGroupDouble(*_h5Group, "NormalVectorY", &normalY) ||
            !h5tools->readGroupDouble(*_h5Group, "NormalVectorZ", &normalZ))
            return false;
        _wallPlaneData->setNormalVector(normalX, normalY, normalZ);
        return true;
    }

    bool FITKRadiossDataRWallPlaneAdaptor::adaptW()
    {
        _wallPlaneData = dynamic_cast<Radioss::FITKRWallPlane*>(_dataObj);
        if (_writer == nullptr || _wallPlaneData == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!writeRadiossData(_wallPlaneData, *_h5Group))return false;
        if (!writeDataObject(_wallPlaneData, *_h5Group))return false;
        //获取数据
        // 参考节点ID
        h5tools->writeGroupAttrInt(*_h5Group, "NodeId", _wallPlaneData->getNodeId());
        // 滑移类型
        Core::FITKEnumTransfer<Radioss::FITKAbstractRWall::RWallSliding> slidingTransfer;
        bool isSlidingValid = false;
        QString slidingTypeStr = slidingTransfer.toString(
            _wallPlaneData->getSlidingType(), isSlidingValid);
        if (!isSlidingValid) return false;
        h5tools->writeGroupAttrStr(*_h5Group, "SlidingType", slidingTypeStr.toStdString());
        // 第一节点集合ID
        h5tools->writeGroupAttrInt(*_h5Group, "NodeGroupId1", _wallPlaneData->getNodeGroupId1());
        // 第二节点集合ID
        h5tools->writeGroupAttrInt(*_h5Group, "NodeGroupId2", _wallPlaneData->getNodeGroupId2());
        // 搜索距离
        h5tools->writeGroupAttrDouble(*_h5Group, "DSearch", _wallPlaneData->getDSearch());
        // 摩擦系数
        h5tools->writeGroupAttrDouble(*_h5Group, "FrictionCoef", _wallPlaneData->getFrictionCoef());
        // 特征直径
        h5tools->writeGroupAttrDouble(*_h5Group, "Diameter", _wallPlaneData->getDiameter());
        // 滤波因子
        h5tools->writeGroupAttrDouble(*_h5Group, "FilteringFactor", _wallPlaneData->getFilteringFactor());
        // 滤波标志
        h5tools->writeGroupAttrInt(*_h5Group, "FilteringFlag", _wallPlaneData->getFilteringFlag());
        // 平面基点坐标
        double basicX = 0.0, basicY = 0.0, basicZ = 0.0;
        _wallPlaneData->getBasicPoint(basicX, basicY, basicZ);
        h5tools->writeGroupAttrDouble(*_h5Group, "BasicPointX", basicX);
        h5tools->writeGroupAttrDouble(*_h5Group, "BasicPointY", basicY);
        h5tools->writeGroupAttrDouble(*_h5Group, "BasicPointZ", basicZ);
        // 平面法向量
        double normalX = 0.0, normalY = 0.0, normalZ = 0.0;
        _wallPlaneData->getNormalVector(normalX, normalY, normalZ);
        h5tools->writeGroupAttrDouble(*_h5Group, "NormalVectorX", normalX);
        h5tools->writeGroupAttrDouble(*_h5Group, "NormalVectorY", normalY);
        h5tools->writeGroupAttrDouble(*_h5Group, "NormalVectorZ", normalZ);
        // 写入关键字
        QString rwallKeyWord = Radioss::FITKRWallPlane::GetFITKRWallPlaneRadiossKeyWord();
        h5tools->writeGroupAttrStr(*_h5Group, "RWallKeyWord", rwallKeyWord.toStdString());
        return true;
    }
}
