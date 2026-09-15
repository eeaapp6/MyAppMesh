/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFBoundary.h"
#include "FITKOFBoundaryType.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredFluidMeshVTK.h"

namespace Interface
{

    FITKOFBoundary::FITKOFBoundary(FITKOFSolverTypeEnum::FITKOFBoundaryType bType)
        :_boundaryType(bType)
    {
        //构造
    }

    FITKOFBoundary::~FITKOFBoundary()
    {
        //销毁
        for (QList<QPair<QString, FITKOFAbsBoundaryType*>>::iterator iter = _flow.begin(); iter != _flow.end(); ++iter)
        {
            if (iter->second) delete iter->second;
        }
        for (QList<QPair<QString, FITKOFAbsBoundaryType*>>::iterator iter = _turbulence.begin(); iter != _turbulence.end(); ++iter)
        {
            if (iter->second) delete iter->second;
        }
        for (QList<QPair<QString, FITKOFAbsBoundaryType*>>::iterator iter = _phases.begin(); iter != _phases.end(); ++iter)
        {
            if (iter->second) delete iter->second;
        }
        for (QList<QPair<QString, FITKOFAbsBoundaryType*>>::iterator iter = _thermal.begin(); iter != _thermal.end(); ++iter)
        {
            if (iter->second) delete iter->second;
        }
    }

    FITKOFSolverTypeEnum::FITKOFBoundaryType FITKOFBoundary::getBoundaryType()
    {
        //获取边界类型
        return _boundaryType;
    }

    void FITKOFBoundary::setMeshBoundaryID(int regionID, int meshId)
    {
        //设置网格边界ID
        _meshBoundaryID.first = regionID;
        _meshBoundaryID.second = meshId;
        auto mesh = this->getMeshBoundary();
        if (mesh)
            this->setUserData(Core::FITKUserData::FITKUserRole, mesh->getDataObjectName());
    }

    int FITKOFBoundary::getMeshBoundaryID()
    {
        //获取网格边界id
        return _meshBoundaryID.second;
    }

    int FITKOFBoundary::getMeshRegionID()
    {
        //获取区域网格ID
        return _meshBoundaryID.first;
    }

    FITKBoundaryMeshVTK* FITKOFBoundary::getMeshBoundary()
    {
        //获取网格边界数据
        AppFrame::FITKGlobalData* globalData = FITKAPP->getGlobalData();
        if (!globalData)return nullptr;
        Interface::FITKUnstructuredFluidMeshVTK* meshData = globalData->getMeshData< Interface::FITKUnstructuredFluidMeshVTK>();
        if (meshData == nullptr)return nullptr;
        Interface::FITKFluidRegionsMesh* region = meshData->getDataByID(_meshBoundaryID.first);
        if (region == nullptr)return nullptr;
        Interface::FITKBoundaryMeshVTKManager* boundMeshManager = region->getBoundaryMeshManager();
        if (boundMeshManager == nullptr) return nullptr;
        return boundMeshManager->getDataByID(_meshBoundaryID.second);
    }

    int FITKOFBoundary::getFlowCount()
    {
        //获取流体参数类型数量
        return _flow.size();
    }

    QString FITKOFBoundary::getFlowVariableName(int index)
    {
        //通过索引获取流体参数类型名称
        if (index < 0 || index >= _flow.size())return QString();
        return _flow[index].first;
    }

    FITKOFAbsBoundaryType* FITKOFBoundary::getFlowVBType(int index)
    {
        //通过索引获取流体参数类型对象
        if (index < 0 || index >= _flow.size())return nullptr;
        return _flow[index].second;
    }

    int FITKOFBoundary::getTurbulenceCount()
    {
        //获取湍流参数类型数量
        return _turbulence.size();
    }

    QString FITKOFBoundary::getTurbulenceVariableName(int index)
    {
        //通过索引获取湍流参数类型名称
        if (index < 0 || index >= _turbulence.size())return QString();
        return _turbulence[index].first;
    }

    FITKOFAbsBoundaryType* FITKOFBoundary::getTurbulenceVBType(int index)
    {
        //通过索引获取湍流参数类型对象
        if (index < 0 || index >= _turbulence.size())return nullptr;
        return _turbulence[index].second;
    }

    int FITKOFBoundary::getPhasesCount()
    {
        //获取Phases参数数量
        return _phases.size();
    }

    QString FITKOFBoundary::getPhasesVariableName(int index)
    {
        //通过index获取Phases的变量
        if (index < 0 || index >= _phases.size())return QString();
        return _phases[index].first;
    }

    FITKOFAbsBoundaryType* FITKOFBoundary::getPhasesVBType(int index)
    {
        //通过index获取Phases的边界数据
        if (index < 0 || index >= _phases.size())return nullptr;
        return _phases[index].second;
    }

    int FITKOFBoundary::getThermalCount()
    {
        //获取热力学参数数量
        return _thermal.size();
    }

    QString FITKOFBoundary::getThermalVariableName(int index)
    {
        //通过index获取热力学的变量
        if (index < 0 || index >= _thermal.size())return QString();
        return _thermal[index].first;
    }

    FITKOFAbsBoundaryType * FITKOFBoundary::getThermalVBType(int index)
    {
        //通过index获取热力学的变量的边界数据
        if (index < 0 || index >= _thermal.size())return nullptr;
        return _thermal[index].second;
    }

    void FITKOFBoundary::setFlowBoundary(QString variableName, FITKOFAbsBoundaryType* boundaryType)
    {
        //设置流体边界参数类型对象（数据存在替换、数据不存在添加）
        if (!boundaryType)return;
        for (QList<QPair<QString, FITKOFAbsBoundaryType*>>::iterator iter = _flow.begin(); iter != _flow.end(); ++iter)
        {
            if (iter->first != variableName)continue;
            if (iter->second)
                delete iter->second;
            iter->second = boundaryType;
            return;
        }
        _flow.append(QPair<QString, FITKOFAbsBoundaryType*>(variableName, boundaryType));
    }

    void FITKOFBoundary::setTurbulenceBoundary(QString variableName, FITKOFAbsBoundaryType* boundaryType)
    {
        //设置湍流边界参数类型对象（数据存在替换、数据不存在添加）
        if (!boundaryType)return;
        for (QList<QPair<QString, FITKOFAbsBoundaryType*>>::iterator iter = _turbulence.begin(); iter != _turbulence.end(); ++iter)
        {
            if (iter->first != variableName)continue;
            if (iter->second)
                delete iter->second;
            iter->second = boundaryType;
            return;
        }
        _turbulence.append(QPair<QString, FITKOFAbsBoundaryType*>(variableName, boundaryType));
    }

    void FITKOFBoundary::setPhasesBoundary(QString variableName, FITKOFAbsBoundaryType* boundaryType)
    {
        //设置材料边界参数类型对象
        if (!boundaryType)return;
        for (QList<QPair<QString, FITKOFAbsBoundaryType*>>::iterator iter = _phases.begin(); iter != _phases.end(); ++iter)
        {
            if (iter->first != variableName)continue;
            if (iter->second)
                delete iter->second;
            iter->second = boundaryType;
            return;
        }
        _phases.append(QPair<QString, FITKOFAbsBoundaryType*>(variableName, boundaryType));
    }

    void FITKOFBoundary::setThermalBoundary(QString variableName, FITKOFAbsBoundaryType * boundaryType)
    {
        //设置热力学边界参数类型对象（数据存在替换、数据不存在添加）
        if (!boundaryType)return;
        for (QList<QPair<QString, FITKOFAbsBoundaryType*>>::iterator iter = _thermal.begin(); iter != _thermal.end(); ++iter)
        {
            if (iter->first != variableName)continue;
            if (iter->second)
                delete iter->second;
            iter->second = boundaryType;
            return;
        }
        _thermal.append(QPair<QString, FITKOFAbsBoundaryType*>(variableName, boundaryType));
    }

    void FITKOFBoundary::setBoundary(QString variableName, FITKOFAbsBoundaryType* boundaryType)
    {
        //设置边界参数类型对象（数据存在替换、数据不存在添加）
        if (!boundaryType)return;
        //Flow
        for (QList<QPair<QString, FITKOFAbsBoundaryType*>>::iterator iter = _flow.begin(); iter != _flow.end(); ++iter)
        {
            if (iter->first != variableName)continue;
            if (iter->second)
                delete iter->second;
            iter->second = boundaryType;
            return;
        }
        //Turbulence
        for (QList<QPair<QString, FITKOFAbsBoundaryType*>>::iterator iter = _turbulence.begin(); iter != _turbulence.end(); ++iter)
        {
            if (iter->first != variableName)continue;
            if (iter->second)
                delete iter->second;
            iter->second = boundaryType;
            return;
        }
        //Phases
        for (QList<QPair<QString, FITKOFAbsBoundaryType*>>::iterator iter = _phases.begin(); iter != _phases.end(); ++iter)
        {
            if (iter->first != variableName)continue;
            if (iter->second)
                delete iter->second;
            iter->second = boundaryType;
            return;
        }
    }

    void FITKOFBoundary::clearTurbulenceBoundary()
    {
        //清理湍流边界变量数据
        for (QList<QPair<QString, FITKOFAbsBoundaryType*>>::iterator iter = _turbulence.begin(); iter != _turbulence.end(); ++iter)
        {
            if (iter->second) delete iter->second;
        }
        _turbulence.clear();
    }

    void FITKOFBoundary::clearFlowBoundary()
    {
        //清理流体边界变量数据
        for (QList<QPair<QString, FITKOFAbsBoundaryType*>>::iterator iter = _flow.begin(); iter != _flow.end(); ++iter)
        {
            if (iter->second) delete iter->second;
        }
        _flow.clear();
    }

    FITKOFAbsBoundaryType* FITKOFBoundary::getBoundary(QString variableName)
    {
        //Flow
        for (QList<QPair<QString, FITKOFAbsBoundaryType*>>::iterator iter = _flow.begin(); iter != _flow.end(); ++iter)
        {
            if (iter->first != variableName)continue;
            return iter->second;
        }
        //Turbulence
        for (QList<QPair<QString, FITKOFAbsBoundaryType*>>::iterator iter = _turbulence.begin(); iter != _turbulence.end(); ++iter)
        {
            if (iter->first != variableName)continue;
            return iter->second;
        }
        //Phases
        for (QList<QPair<QString, FITKOFAbsBoundaryType*>>::iterator iter = _phases.begin(); iter != _phases.end(); ++iter)
        {
            if (iter->first != variableName)continue;
            return iter->second;
        }
        //Thermal
        for (QList<QPair<QString, FITKOFAbsBoundaryType*>>::iterator iter = _thermal.begin(); iter != _thermal.end(); ++iter)
        {
            if (iter->first != variableName)continue;
            return iter->second;
        }
        return nullptr;
    }

    QList<FITKOFBoundary*> FITKOFBoundaryManager::getBoundarys(FITKOFSolverTypeEnum::FITKOFBoundaryType t)
    {
        QMutexLocker locker(&_mutex);
        QList<FITKOFBoundary*> bds{};
        const int n = this->getDataCount();
        for (int i = 0; i < n; i++)
        {
            FITKOFBoundary* bd = this->getDataByIndex(i);
            if(bd == nullptr) continue;
            if (bd->getBoundaryType() == t)
                bds.append(bd);
        }
        return bds;
    }

    FITKOFBoundary * FITKOFBoundaryManager::getBoundary(int MeshBoundaryID)
    {
        const int n = this->getDataCount();
        for (int i = 0; i < n; i++)
        {
            FITKOFBoundary* bd = this->getDataByIndex(i);
            if (bd == nullptr) continue;
            if (bd->getMeshBoundaryID() == MeshBoundaryID)return bd;
        }
        return nullptr;
    }

    void FITKOFBoundaryManager::setMappedWallBoundary(int b1, int b2)
    {
        _mappedWallBoundary.insert(b1, b2);
    }

    QHash<int, int> FITKOFBoundaryManager::getMappedWallBoundary()
    {
        return _mappedWallBoundary;
    }

    FITKOFBoundary * FITKOFBoundaryManager::getMappendWallBoundary(int boundaryID)
    {
        if (_mappedWallBoundary.contains(boundaryID))
            return getDataByID(_mappedWallBoundary[boundaryID]);
        else if (_mappedWallBoundary.values().contains(boundaryID))
            return getDataByID(_mappedWallBoundary.key(boundaryID));
        return nullptr;
    }

}

