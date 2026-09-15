/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GraphMarkerProvider.h"

// Graph widget
#include "FITK_Component/FITKRenderWindowOCC/FITKGraph3DWindowOCC.h"

// Graph
#include "FITK_Component/FITKOCCGraphAdaptor/FITKOCCGraphObject3D.h"

// Adaptor
#include "FITK_Component/FITKOCCGraphAdaptor/FITKOCCViewAdaptorBase.h"

// Global data
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"

// Data
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsVirtualTopo.h"

namespace GraphData
{
    GraphMarkerProvider::GraphMarkerProvider(Render::FITKGraph3DWindowOCC* graphWidget)
        : GraphProviderBase(graphWidget)
    {

    }

    GraphMarkerProvider::~GraphMarkerProvider()
    {
        // 析构三维可视化对象。
        for (Exchange::FITKOCCGraphObject3D* obj : m_measureObjHash.values())
        {
            delete obj;
        }

        m_measureObjHash.clear();

        for (Exchange::FITKOCCGraphObject3D* obj : m_pickToolObjHash.values())
        {
            delete obj;
        }

        m_pickToolObjHash.clear();     
    }

    QString GraphMarkerProvider::getClassName()
    {
        return "GraphMarkerProvider";
    }

    Exchange::FITKOCCGraphObject3D* GraphMarkerProvider::getMeasureToolGraphObject(QList<int> items, MeasureToolType type)
    {
        // 测量工具对象。
        Exchange::FITKOCCGraphObject3D* obj{ nullptr };

        QString typeName;
        switch (type)
        {
        case GraphData::MTT_Position:
        {
            typeName = "POSITION";
            break;
        }
        case GraphData::MTT_Distance:
        {
            typeName = "DISTANCE";
            break;
        }
        case GraphData::MTT_Angle:
        {
            typeName = "ANGLE";
            break;
        }
        case GraphData::MTT_Area:
        {
            typeName = "AREA";
            break;
        }
        case GraphData::MTT_Volume:
        {
            typeName = "VOLUME";
            break;
        }
        default:
            return obj;
        }

        // 获取已实例化的测距对象。
        if (m_measureObjHash.contains(type))
        {
            obj = m_measureObjHash[type];
        }

        // 未实例化过则创建新对象并保存。
        if (!obj)
        {
            // 创建工具。
            obj = getGraphObject("MeasureToolOCC", "DimensionTools", typeName);
            if (obj)
            {
                m_measureObjHash.insert(type, obj);
            }
        }

        if (!obj)
        {
            return obj;
        }

        // 设置计算数据。
        obj->setUserData(ROLE_INPUT_MEASURE_IDS, QVariant::fromValue<QList<int>>(items));

        return obj;
    }

    Exchange::FITKOCCGraphObject3D* GraphMarkerProvider::getPickToolGraphObject(PickToolType type)
    {
        // 拾取工具对象。
        Exchange::FITKOCCGraphObject3D* obj{ nullptr };

        QString typeName;
        switch (type)
        {
        case GraphData::PTT_Proj_Pos:
        {
            typeName = "EDGE_POS";
            break;
        }
        case GraphData::PTT_Direction:
        {
            typeName = "DIRECTION";
            break;
        }
        default:
            return obj;
        }

        // 获取已实例化的拾取工具对象。
        if (m_pickToolObjHash.contains(type))
        {
            obj = m_pickToolObjHash[type];
        }

        // 未实例化过则创建新对象并保存。
        if (!obj)
        {
            // 创建工具。
            obj = getGraphObject("PickToolOCC", "PickTools", typeName);
            if (obj)
            {
                m_pickToolObjHash.insert(type, obj);
            }
        }

        return obj;
    }

    QList<Exchange::FITKOCCGraphObject3D*> GraphMarkerProvider::getAllPickToolGraphObjects()
    {
        // 工具对象列表。
        QList<Exchange::FITKOCCGraphObject3D*> objs;

        // 循环枚举值创建工具对象。
        for (int i = GraphData::PTT_Begin; i < GraphData::PTT_End; i++)
        {
            Exchange::FITKOCCGraphObject3D* obj = getPickToolGraphObject(PickToolType(i));
            if (obj)
            {
                objs.push_back(obj);
            }
        }

        return objs;
    }

    void GraphMarkerProvider::clearMeasureObject()
    {
        // 从窗口移除全部对象。
        for (Exchange::FITKOCCGraphObject3D* obj : m_measureObjHash.values())
        {
            if (obj)
            {
                obj->removeFromGraphWidget();
            }
        }
    }

    void GraphMarkerProvider::clearPickToolObject()
    {
        // 从窗口移除全部对象。
        for (Exchange::FITKOCCGraphObject3D* obj : m_pickToolObjHash.values())
        {
            if (obj)
            {
                obj->setVisible(false);
                // obj->removeFromGraphWidget();
                obj->clearCache();
            }
        }
    }

    QList<Exchange::FITKOCCGraphObject3D*> GraphMarkerProvider::getCurrentGraphObjs()
    {
        // 当前所有可视化对象数据。
        QList<Exchange::FITKOCCGraphObject3D*> objs;

        // 测量工具可视化对象。
        objs << m_measureObjHash.values();

        // 拾取工具可视化对象。
        objs << m_pickToolObjHash.values();

        return objs;
    }
}   // namespace GraphData