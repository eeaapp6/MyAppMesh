/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFInitialConditions.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredFluidMeshVTK.h"

namespace Interface
{

    FITKOFInitPropRegions::FITKOFInitPropRegions()
    {
        _regionPara = new Core::FITKParameter;
    }

    FITKOFInitPropRegions::~FITKOFInitPropRegions()
    {
        if (_regionPara) delete _regionPara;
    }

    FITKOFSolverTypeEnum::FITKOFRegionMeshType FITKOFInitPropRegions::getRegionMeshType()
    {
        //获取区域网格类型
        return _regionType;
    }

    void FITKOFInitPropRegions::setRegionMeshType(FITKOFSolverTypeEnum::FITKOFRegionMeshType type)
    {
        _regionType = type;
    }

    Core::FITKParameter * FITKOFInitPropRegions::getRegionPara()
    {
        //获取区域参数
        return _regionPara;
    }

    void FITKOFInitPropRegions::setRegionMeshID(int regionID)
    {
        //设置区域网格ID
        _regionMeshID = regionID;
    }

    int FITKOFInitPropRegions::getRegionMeshID()
    {
        //获取区域网格ID
        return _regionMeshID;
    }

    FITKFluidRegionsMesh * FITKOFInitPropRegions::getRegionMeshObj()
    {
        //获取区域网格数据
        AppFrame::FITKGlobalData* globalData = FITKAPP->getGlobalData();
        if (!globalData)return nullptr;
        Interface::FITKUnstructuredFluidMeshVTK* meshData = globalData->getMeshData< Interface::FITKUnstructuredFluidMeshVTK>();
        if (meshData == nullptr)return nullptr;
        return meshData->getDataByID(_regionMeshID);
    }

    FITKOFGeometryPatch::FITKOFGeometryPatch(int geoId, FITKOFSolverInitialEnum::FITKPatchApplyObjectType type)
    {
        _fieldPara = new Core::FITKParameter;
        //设置组件
        _geometryId = geoId;
        _applyType = type;
    }

    FITKOFGeometryPatch::~FITKOFGeometryPatch()
    {
        if (_fieldPara) delete _fieldPara;
    }

    int FITKOFGeometryPatch::getGeometryId()
    {
        //获取组件Id
        return _geometryId;
    }

    Interface::FITKAbsGeoCommand* FITKOFGeometryPatch::getGeometryModel()
    {
        //获取几何数据对象
        return FITKDATAREPO->getTDataByID<FITKAbsGeoCommand>(_geometryId);
    }

    Core::FITKParameter* FITKOFGeometryPatch::getFieldPara()
    {
        return _fieldPara;
    }

    FITKOFSolverInitialEnum::FITKPatchApplyObjectType FITKOFGeometryPatch::getAppltTo()
    {
        return _applyType;
    }

    FITKOFInitialConditions::FITKOFInitialConditions()
    {
        _basic = new Core::FITKParameter;
    }

    FITKOFInitialConditions::~FITKOFInitialConditions()
    {
        if (_basic) delete _basic;
        for (FITKOFGeometryPatch* pat : _patchs)
        {
            if (pat) delete pat;
        }
    }

    bool FITKOFInitialConditions::getInitPropRegions()
    {
        //获取是否初始化区域属性
        return _isInitPropRegions;
    }

    void FITKOFInitialConditions::setInitPropRegions(bool isPropReg)
    {
        //设置是否初始化区域属性
        _isInitPropRegions = isPropReg;
    }

    int FITKOFInitialConditions::getInitPropRegionCount()
    {
        ////获取区域属性个数
        return _regions.size();
    }

    FITKOFInitPropRegions * FITKOFInitialConditions::getInitPropRegionByIndex(int index)
    {
        //通过index获取区域属性
        if (index < 0 || index >= _regions.size()) return nullptr;
        return _regions.at(index);
    }

    QList<FITKOFInitPropRegions*> FITKOFInitialConditions::getInitPropRegionByType(FITKOFSolverTypeEnum::FITKOFRegionMeshType type)
    {
        QList<FITKOFInitPropRegions*> data;
        //通过区域类型获取数据
        for (QList<FITKOFInitPropRegions*>::iterator iter = _regions.begin(); iter != _regions.end(); ++iter)
        {
            if (!*iter) continue;
            if ((*iter)->getRegionMeshType() == type)
                data.append(*iter);
        }
        return data;
    }

    FITKOFInitPropRegions * FITKOFInitialConditions::getInitPropRegionByRegionMeshID(int regionMeshID)
    {
        //通过区域网格ID获取区域属性
        for (QList<FITKOFInitPropRegions*>::iterator iter = _regions.begin(); iter != _regions.end(); ++iter)
        {
            if (!*iter) continue;
            if ((*iter)->getRegionMeshID() == regionMeshID)
                return *iter;
        }
        return nullptr;
    }

    Core::FITKParameter* FITKOFInitialConditions::getBasicData()
    {
        //获取基础初始化
        return _basic;
    }

    int FITKOFInitialConditions::getPatchCount()
    {
        return _patchs.size();
    }

    FITKOFGeometryPatch* FITKOFInitialConditions::getPatch(int index)
    {
        if (index < 0 || index >= _patchs.size())return nullptr;
        return _patchs.at(index);
    }

    void FITKOFInitialConditions::appendPatch(FITKOFGeometryPatch* patch)
    {
        if (!patch)return;
        _patchs.append(patch);
    }

    void FITKOFInitialConditions::addInitPropRegion(FITKOFInitPropRegions * regionProp)
    {
        //添加初始化区域属性
        _regions.append(regionProp);
    }

    void FITKOFInitialConditions::removeInitPropRegion(int regionMeshID)
    {
        //移除区域初始化参数
        FITKOFInitPropRegions * initprop = getInitPropRegionByRegionMeshID(regionMeshID);
        if (initprop) {
            _regions.removeAll(initprop);
            delete initprop;
        }
    }

    void FITKOFInitialConditions::clearAllInitPropRegion()
    {
        for (FITKOFInitPropRegions * initprop : _regions)
            delete initprop;
        _regions.clear();
    }

    void FITKOFInitialConditions::removePatch(int index)
    {
        FITKOFGeometryPatch* patch = this->getPatch(index);
        if (patch)
        {
            _patchs.removeAt(index);
            delete patch;
        }
    }
}
