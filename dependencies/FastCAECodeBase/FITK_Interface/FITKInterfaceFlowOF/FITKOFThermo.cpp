/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFThermo.h"
#include "FITKOFThermoPhysicalEquationOfState.h"
#include "FITKOFThermoPhysicalTransport.h"
#include "FITKOFThermoPhysicalThermodynamics.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredFluidMeshVTK.h"

namespace Interface
{
    FITKOFThermoPhysicalProp::FITKOFThermoPhysicalProp()
    {
        //初始化数据
        _speciePara = new Core::FITKParameter;
    }

    FITKOFThermoPhysicalProp::~FITKOFThermoPhysicalProp()
    {
        //销毁数据
        if (_speciePara) delete _speciePara;
        if (_equationOfStatePara) delete _equationOfStatePara;
        if (_thermodynamicsPara) delete _thermodynamicsPara;
        if (_transportPara) delete _transportPara;
    }

    FITKOFSolverTypeEnum::FITKOFRegionMeshType FITKOFThermoPhysicalProp::getRegionType()
    {
        //获取区域类型
        return _regionType;
    }

    void FITKOFThermoPhysicalProp::setRegionMeshType(FITKOFSolverTypeEnum::FITKOFRegionMeshType type)
    {
        //设置区域网格类型
        _regionType = type;
    }

    Core::FITKParameter * FITKOFThermoPhysicalProp::getSpeciePara()
    {
        //获取Specie参数
        return _speciePara;
    }

    FITKOFAbsThermoPhysicalEquationOfState * FITKOFThermoPhysicalProp::getEquationOfStatePara()
    {
        //状态方程参数
        return _equationOfStatePara;
    }

    FITKOFAbsThermoPhysicalThermodynamics * FITKOFThermoPhysicalProp::getThermodynamicsPara()
    {
        //热力学属性参数
        return _thermodynamicsPara;
    }

    FITKOFAbsThermoPhysicalTransport * FITKOFThermoPhysicalProp::getTransportPara()
    {
        //材料属性参数
        return _transportPara;
    }

    void FITKOFThermoPhysicalProp::setEquationOfStatePara(FITKOFAbsThermoPhysicalEquationOfState * equationOfStatePara)
    {
        //设置状态方程参数
        if (_equationOfStatePara != equationOfStatePara && _equationOfStatePara) delete _equationOfStatePara;
        _equationOfStatePara = equationOfStatePara;
    }

    void FITKOFThermoPhysicalProp::setThermodynamicsPara(FITKOFAbsThermoPhysicalThermodynamics * thermodynamicsPara)
    {
        //设置热力学属性参数
        if (_thermodynamicsPara != thermodynamicsPara && _thermodynamicsPara) delete _thermodynamicsPara;
        _thermodynamicsPara = thermodynamicsPara;
    }

    void FITKOFThermoPhysicalProp::setTransportPara(FITKOFAbsThermoPhysicalTransport * transportPara)
    {
        //设置材料属性参数
        if (_transportPara != transportPara && _transportPara) delete _transportPara;
        _transportPara = transportPara;
    }

    void FITKOFThermoPhysicalProp::setRegionMeshID(int regionID)
    {
        //设置区域网格ID
        _regionMeshID = regionID;
    }

    int FITKOFThermoPhysicalProp::getRegionMeshID()
    {
        //获取区域网格ID
        return _regionMeshID;
    }

    FITKFluidRegionsMesh * FITKOFThermoPhysicalProp::getRegionMeshObj()
    {
        //获取区域网格数据
        AppFrame::FITKGlobalData* globalData = FITKAPP->getGlobalData();
        if (!globalData)return nullptr;
        Interface::FITKUnstructuredFluidMeshVTK* meshData = globalData->getMeshData< Interface::FITKUnstructuredFluidMeshVTK>();
        if (meshData == nullptr)return nullptr;
        return meshData->getDataByID(_regionMeshID);
    }

    FITKOFThermo::FITKOFThermo()
    {

    }

    FITKOFThermo::~FITKOFThermo()
    {

    }

    int FITKOFThermo::getThermoPropRegionCount()
    {
        //获取热物理区域属性个数
        return _regionThermoProp.size();
    }

    FITKOFThermoPhysicalProp * FITKOFThermo::getThermoPropRegionByIndex(int index)
    {
        //通过index获取热物理区域属性
        if (index < 0 || index >= _regionThermoProp.size()) return nullptr;
        return _regionThermoProp.at(index);
    }

    QList<FITKOFThermoPhysicalProp*> FITKOFThermo::getThermoPropRegionByType(FITKOFSolverTypeEnum::FITKOFRegionMeshType type)
    {
        //通过区域网格ID获取热物理区域属性
        QList<FITKOFThermoPhysicalProp*> data;
        //通过区域类型获取数据
        for (QList<FITKOFThermoPhysicalProp*>::iterator iter = _regionThermoProp.begin(); iter != _regionThermoProp.end(); ++iter)
        {
            if (!*iter) continue;
            if ((*iter)->getRegionType() == type)
                data.append(*iter);
        }
        return data;
    }

    FITKOFThermoPhysicalProp * FITKOFThermo::getThermoPropRegionByRegionMeshID(int regionMeshID)
    {
        //通过区域网格ID获取热物理区域属性
        for (QList<FITKOFThermoPhysicalProp*>::iterator iter = _regionThermoProp.begin(); iter != _regionThermoProp.end(); ++iter)
        {
            if (!*iter) continue;
            if ((*iter)->getRegionMeshID() == regionMeshID)
                return *iter;
        }
        return nullptr;
    }

    void FITKOFThermo::addThermoPropRegion(FITKOFThermoPhysicalProp * thermoPhysicalProp)
    {
        //添加初始化区域属性
        _regionThermoProp.append(thermoPhysicalProp);
    }

    void FITKOFThermo::removeThermoPropRegion(int regionMeshID)
    {
        //移除区域初始化参数
        FITKOFThermoPhysicalProp * thermoPhysicalProp = getThermoPropRegionByRegionMeshID(regionMeshID);
        if (thermoPhysicalProp) {
            _regionThermoProp.removeAll(thermoPhysicalProp);
            delete thermoPhysicalProp;
        }
    }

    void FITKOFThermo::clearAllThermoPropRegion()
    {
        //清理全部数据
        for (FITKOFThermoPhysicalProp* p : _regionThermoProp)
            delete p;
        _regionThermoProp.clear();
    }

}
