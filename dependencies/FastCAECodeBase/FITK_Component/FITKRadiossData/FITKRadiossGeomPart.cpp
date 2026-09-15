/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossGeomPart.h"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoEnum.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoPart.h"

namespace  Radioss
{
    FITKRadiossGeomPart::FITKRadiossGeomPart()
    {
        //初始化部件
        this->initGeoPart();
    }

    FITKRadiossGeomPart::~FITKRadiossGeomPart()
    {
        //释放几何部件
        this->removeGeoPart();
    }

    Interface::FITKModelEnum::AbsModelType Radioss::FITKRadiossGeomPart::getAbsModelType()
    {
        return Interface::FITKModelEnum::AbsModelType::AMTGeometry;
    }

    Core::FITKAbstractNDataObject *FITKRadiossGeomPart::getAbstractMesh()
    {
        return nullptr;
    }

    Core::FITKAbstractNDataObject *FITKRadiossGeomPart::getAbstractGeom()
    {
        Interface::FITKAbsGeoCommand* cmdObj = FITKDATAREPO->
             getTDataByID<Interface::FITKAbsGeoCommand>(_geomCommandID);
        
        return  cmdObj; 
    }

    Interface::FITKAbsGeoPart * FITKRadiossGeomPart::getGeoPart()
    {
        return FITKDATAREPO->getTDataByID<Interface::FITKAbsGeoPart>(_geomCommandID);;
    }

    bool FITKRadiossGeomPart::isGeometryReferencedCmd()
    {
        Interface::FITKAbsGeoPart* part = this->getGeoPart();
        if (!part) return false;
        return part->getReferencedCmdCount() != 0;
    }

    void FITKRadiossGeomPart::setGeoPartID(int geoPartID)
    {
        removeGeoPart();
        _geomCommandID = geoPartID;
    }

    int FITKRadiossGeomPart::getGeoPartID()
    {
        return _geomCommandID;
    }

    void FITKRadiossGeomPart::initGeoPart()
    {
        //获取全局管理器
        if (FITKAPP == nullptr) return;
        AppFrame::FITKGlobalData* globalData = FITKAPP->getGlobalData();
        if (globalData == nullptr) return;
        //获取几何管理器
        Interface::FITKGeoCommandList* geoMgr = globalData->getGeometryData<Interface::FITKGeoCommandList>();
        if (geoMgr == nullptr) return;
        //初始化几何部件
        Interface::FITKAbsGeoPart* geoPart = dynamic_cast<Interface::FITKAbsGeoPart*>(Interface::FITKInterfaceGeometryFactory::getInstance()->createCommand(Interface::FITKGeoEnum::FGTPart));
        if (geoPart == nullptr) return;
        geoMgr->appendDataObj(geoPart);//添加到几何命令管理器
        geoPart->setAutoBool(false);//设置不自动合并
        _geomCommandID = geoPart->getDataObjectID();
    }

    void FITKRadiossGeomPart::removeGeoPart()
    {
        //获取全局管理器
        if (FITKAPP == nullptr) return;
        AppFrame::FITKGlobalData* globalData = FITKAPP->getGlobalData();
        if (globalData == nullptr) return;
        //获取几何管理器
        Interface::FITKGeoCommandList* geoMgr = globalData->getGeometryData<Interface::FITKGeoCommandList>();
        if (geoMgr == nullptr) return;
        geoMgr->removeDataByID(_geomCommandID);//移除几何
        _geomCommandID = -1;
    }

} // namespace  Radioss


