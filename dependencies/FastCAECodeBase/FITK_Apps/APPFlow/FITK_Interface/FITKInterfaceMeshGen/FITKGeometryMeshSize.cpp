/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKGeometryMeshSize.h"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractGeoModel.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoShapeAgent.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"

namespace Interface
{
    void FITKGeometryMeshSize::setGeoGroupComponentId(int geoComponentId)
    {
        //设置组件
        _geoGroupComponentId = geoComponentId;
    }

    int FITKGeometryMeshSize::getGeoGroupComponentId()
    {
        //获取组件Id
        return _geoGroupComponentId;
    }

    Interface::FITKGeoComponent* FITKGeometryMeshSize::getGeoComponent()
    {
        //获取组件数据对象
        return FITKDATAREPO->getTDataByID<FITKGeoComponent>(_geoGroupComponentId);
    }

    Interface::FITKAbsGeoCommand* FITKGeometryMeshSize::getGeoModel()
    {
        //获取组件对应的模型
        Interface::FITKGeoComponent* componentGeo = this->getGeoComponent();
        if (!componentGeo)return nullptr;
        Interface::FITKAbsGeoShapeAgent* shape = dynamic_cast<Interface::FITKAbsGeoShapeAgent*>(componentGeo->getAbstractModel());
        if (!shape)return nullptr;
        return shape->getGeoCommand();
    }

    void FITKGeometryMeshSize::setLayerNumber(int number)
    {
        _layerNum = number;
    }

    int FITKGeometryMeshSize::getLayerNumber()
    {
        return _layerNum;
    }

    void FITKGeometryMeshSize::setExpansionRatio(double expansionRatio)
    {
        _expansionRatio = expansionRatio;
    }

    double FITKGeometryMeshSize::getExpansionRatio()
    {
        return _expansionRatio;
    }

    void FITKGeometryMeshSize::setLayerThickness(double layerThickness)
    {
        _layerThickness = layerThickness;
    }

    double FITKGeometryMeshSize::getLayerThickness()
    {
        return _layerThickness;
    }
}
