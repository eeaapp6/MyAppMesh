/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GraphProviderManager.h"

// Graph widget
#include "FITK_Kernel/FITKCore/FITKAbstractGraphWidget.h"
#include "FITK_Component/FITKRenderWindowOCC/FITKGraph3DWindowOCC.h"

// Provider
#include "GraphModelProvider.h"
#include "GraphMarkerProvider.h"

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
    }

    GraphModelProvider* GraphProviderManager::getModelProvider(Render::FITKGraph3DWindowOCC* graphWidget)
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

    GraphMarkerProvider* GraphProviderManager::getMarkerProvider(Render::FITKGraph3DWindowOCC* graphWidget)
    {
        // 不存在则创建新管理器。
        if (!m_markProvider)
        {
            if (!graphWidget)
            {
                return nullptr;
            }

            m_markProvider = new GraphMarkerProvider(graphWidget);
        }

        return m_markProvider;
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
}   // namespace GraphData