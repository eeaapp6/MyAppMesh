/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKMeshParameterManager.h"
#include "FITKMeshParameterBase.h"
#include "FITKMeshParameterLine.h"
#include "FITKMeshParameterSurface.h"
#include "FITKMeshParameterVolume.h"

#include "FITK_Interface/FITKInterfaceGeometry/FITKVirtualTopoManager.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"

namespace Interface
{
    void FITKMeshParameterManager::clearAll()
    {
        QList<int> ids = _lineMesingParameter.keys();
        for (int id : ids) {
            FITKMeshParameterLine* para = _lineMesingParameter.take(id);
            if (para == nullptr) {
                continue;
            }
            delete para;
            para = nullptr;
        }
        _lineMesingParameter.clear();

        ids = _surfaceMesingParameter.keys();
        for (int id : ids) {
            FITKMeshParameterSurface* para = _surfaceMesingParameter.take(id);
            if (para == nullptr) {
                continue;
            }
            delete para;
            para = nullptr;
        }
        _surfaceMesingParameter.clear();

        ids = _volumeMesingParameter.keys();
        for (int id : ids) {
            FITKMeshParameterVolume* para = _volumeMesingParameter.take(id);
            if (para == nullptr) {
                continue;
            }
            delete para;
            para = nullptr;
        }
        _volumeMesingParameter.clear();
    }

    QList<int> FITKMeshParameterManager::getExistLineTopoIds()
    {
        return _lineMesingParameter.keys();
    }

    QList<int> FITKMeshParameterManager::getExistSurfaceTopoIds()
    {
        return _surfaceMesingParameter.keys();
    }

    QList<int> FITKMeshParameterManager::getExistVolumeTopoIds()
    {
        return _volumeMesingParameter.keys();
    }

    FITKMeshParameterLine* FITKMeshParameterManager::getLineMeshingParameters(int vTopoID, bool isCreate)
    {
        FITKMeshParameterLine* lineParam = _lineMesingParameter.value(vTopoID, nullptr);
        if (lineParam) return lineParam;

        if (isCreate == true)
        {
            lineParam = new FITKMeshParameterLine;
            lineParam->setModelID(vTopoID);
            _lineMesingParameter.insert(vTopoID, lineParam);
            return lineParam;
        }

        return nullptr;
    }

    void FITKMeshParameterManager::clearLineMeshingParameters(Interface::FITKAbsGeoCommand* cmd)
    {
        // 尝试获取几何命令。
        Interface::FITKShapeVirtualTopoManager* vEdgeMgr{ nullptr };
        if (cmd)
        {
            vEdgeMgr = cmd->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VSEdge);
        }

        if (vEdgeMgr)
        {
            // 检查不存在的线，移除参数。
            QList<int> edgeIds = _lineMesingParameter.keys();
            for (const int & edgeId : edgeIds)
            {
                if (!vEdgeMgr->getDataByID(edgeId))
                {
                    delete _lineMesingParameter.take(edgeId);
                }
            }
        }
        else
        {
            // 几何不存在则全部清除。
            for (FITKMeshParameterLine* param : _lineMesingParameter.values())
            {
                delete param;
            }

            _lineMesingParameter.clear();
        }
    }

    FITKMeshParameterSurface* FITKMeshParameterManager::getSurfaceMeshingParameters(int vTopoID, bool isCreate)
    {
        FITKMeshParameterSurface* surfParameter = _surfaceMesingParameter.value(vTopoID);
        if (surfParameter) return surfParameter;

        if (isCreate == true)
        {
            surfParameter = new FITKMeshParameterSurface;
            surfParameter->setModelID(vTopoID);
            _surfaceMesingParameter.insert(vTopoID, surfParameter);
            return surfParameter;
        }

        return nullptr;
    }

    void FITKMeshParameterManager::clearSurfaceMeshingParameters(Interface::FITKAbsGeoCommand* cmd)
    {
        // 尝试获取几何命令。
        Interface::FITKShapeVirtualTopoManager* vFaceMgr{ nullptr };
        if (cmd)
        {
            vFaceMgr = cmd->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VSFace);
        }

        if (vFaceMgr)
        {
            // 检查不存在的面，移除参数。
            QList<int> faceIds = _surfaceMesingParameter.keys();
            for (const int & faceId : faceIds)
            {
                if (!vFaceMgr->getDataByID(faceId))
                {
                    delete _surfaceMesingParameter.take(faceId);
                }
            }
        }
        else
        {
            // 几何不存在则全部清除。
            for (FITKMeshParameterSurface* sParameter : _surfaceMesingParameter.values())
            {
                delete sParameter;
                sParameter = nullptr;
            }

            _surfaceMesingParameter.clear();
        }
    }

    FITKMeshParameterVolume* FITKMeshParameterManager::getVolumeMeshingParameters(int vTopoID, bool isCreate)
    {
        FITKMeshParameterVolume* volumeParameter = _volumeMesingParameter.value(vTopoID);
        if (volumeParameter) return volumeParameter;
        if (isCreate == true)
        {
            volumeParameter = new FITKMeshParameterVolume;
            volumeParameter->setModelID(vTopoID);
            _volumeMesingParameter.insert(vTopoID, volumeParameter);
            return volumeParameter;
        }
        return nullptr;
    }

    void FITKMeshParameterManager::clearVolumeMeshingParameters(Interface::FITKAbsGeoCommand* cmd)
    {
        // 尝试获取几何命令。
        Interface::FITKShapeVirtualTopoManager* vSolidMgr{ nullptr };
        if (cmd)
        {
            vSolidMgr = cmd->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VSSolid);
        }

        if (vSolidMgr)
        {
            // 检查不存在的实体，移除参数。
            QList<int> solidIds = _volumeMesingParameter.keys();
            for (const int & solidId : solidIds)
            {
                if (!vSolidMgr->getDataByID(solidId))
                {
                    delete _volumeMesingParameter.take(solidId);
                }
            }
        }
        else
        {
            // 几何不存在则全部清除。
            for (FITKMeshParameterVolume* vParameter : _volumeMesingParameter.values())
            {
                delete vParameter;
                vParameter = nullptr;
            }

            _volumeMesingParameter.clear();
        }
    }
}