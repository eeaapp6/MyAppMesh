/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GraphProviderManager.h"
#include "GraphModelProvider.h"
#include "GraphMarkProvider.h"
#include "GraphPostProvider.h"
#include "PickDataProvider/GUIPickInfo.h"

#include "FITK_Kernel/FITKCore/FITKAbstractGraphWidget.h"
#include "FITK_Component/FITKRenderWindowVTK/FITKGraph3DWindowVTK.h"

namespace GraphData
{
    // 静态变量初始化
    GraphProviderManager* GraphProviderManager::s_instance = nullptr;
    QMutex GraphProviderManager::m_mutex;

    GraphProviderManager* GraphProviderManager::getInstance()
    {
        // 获取单例实例。
        if (!s_instance)
        {
            s_instance = new GraphProviderManager;
        }

        return s_instance;
    }

    void GraphProviderManager::Delete()
    {
        // 手动销毁实例。
        if (s_instance)
        {
            delete s_instance;
        }

        s_instance = nullptr;
    }

    GraphProviderManager::GraphProviderManager()
    {

    }

    GraphProviderManager::~GraphProviderManager()
    {
        // 清除数据管理器。
        deleteProvider(m_modelProvider);
        deleteProvider(m_markProvider); 
        m_postProvider.clear();
    }

    GraphModelProvider* GraphProviderManager::getModelProvider(Comp::FITKGraph3DWindowVTK* graphWidget)
    {
        // 不存在则创建新管理器。
        if (!m_modelProvider)
        {
            if (!graphWidget)
            {
                return nullptr;
            }

            m_modelProvider = new GraphModelProvider(graphWidget);
        }
      
        return m_modelProvider;
    }

    GraphMarkProvider* GraphProviderManager::getMarkProvider(Comp::FITKGraph3DWindowVTK* graphWidget)
    {
        if (!m_markProvider)
        {
            if (!graphWidget)
            {
                return nullptr;
            }

            m_markProvider = new GraphMarkProvider(graphWidget);
        }

        return m_markProvider;
    }

    GraphPostProvider * GraphProviderManager::getPostProvider(Comp::FITKGraph3DWindowVTK * graphWidget, bool isIntt)
    {
        if (!graphWidget)
        {
            return nullptr;
        }
        GraphPostProvider* provider = m_postProvider.value(graphWidget->getGUIObjectID());
        if (provider)
        {
            return provider;
        }
        if(isIntt == false)
        {
            return nullptr;
        }
        provider = new GraphPostProvider(graphWidget);
        m_postProvider.insert(graphWidget->getGUIObjectID(), provider);
        return provider;
        
    }

    void GraphProviderManager::updateObjectById(int dataId)
    {
        if (dataId == -1 || !m_modelProvider)
        {
            return;
        }

        bool flag = m_modelProvider->updateObjById(dataId);
        Q_UNUSED(flag);
    }

    void GraphProviderManager::setActorStateByPickInfo(int pickObjType, int pickMethod, int dataObjId)
    {
        Q_UNUSED(pickMethod);

        if (!m_modelProvider) {
            return;
        }

        switch (pickObjType) {
        case GUI::GUIPickInfo::POBGeoVert:
            m_modelProvider->setGeoVertPickable(dataObjId);
            break;
        case GUI::GUIPickInfo::POBGeoEdge:
            m_modelProvider->setGeoEdgePickable(dataObjId);
            break;
        case GUI::GUIPickInfo::POBGeoFace:
            m_modelProvider->setGeoFacePickable(dataObjId);
            break;
        case GUI::GUIPickInfo::POBGeoSolid:
            m_modelProvider->setGeoSolidPickable(dataObjId);
            break;
        case GUI::GUIPickInfo::POBMeshVert:
            m_modelProvider->setMeshVerPickable(dataObjId);
            break;
        case GUI::GUIPickInfo::POBMeshVertSurf:
            m_modelProvider->setMeshVerSurfPickable(dataObjId);
            break;
        case GUI::GUIPickInfo::POBMeshCell:
            m_modelProvider->setMeshCellPickable(dataObjId);
            break;
        case GUI::GUIPickInfo::POBMeshCellSurf:
            m_modelProvider->setMeshCellSurfPickable(dataObjId);
            break;
        default:
            //取消拾取
            m_modelProvider->setNonePickable(dataObjId);
            break;
        }
    }
}   // namespace GraphData