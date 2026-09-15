/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperGraphTool.h"

// APP
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppSettings.h"

// Global data
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"

// Graph widget and object
#include "FITK_Component/FITKRenderWindowOCC/FITKGraph3DWindowOCC.h"
#include "FITK_Component/FITKOCCGraphAdaptor/FITKOCCGraphObject3D.h"

// Graph data manager
#include "GraphDataProvider/GraphProviderManager.h"
#include "GraphDataProvider/GraphModelProvider.h"
#include "GraphDataProvider/GraphMarkerProvider.h"

// GUI
#include "GUIFrame/MainWindow.h"
#include "GUIFrame/RenderWidget.h"
#include "FITK_Component/FITKWidget/FITKMdiArea.h"

namespace GUIOper
{
    void OperGraphTool::measure(QList<int> items, EventOper::MeasureToolType type, QColor color)
    {
        // 获取可视化窗口。
        Render::FITKGraph3DWindowOCC* graphWidget = getGraphWidget();
        if (!graphWidget)
        {
            return;
        }

        // 获取工具标识可视化对象管理器。
        GraphData::GraphMarkerProvider* markProvider = GRAPH_PROVIDER_MGR->getMarkerProvider(graphWidget);
        if (!markProvider)
        {
            return;
        }

        GraphData::MeasureToolType mtt;
        switch (type)
        {
        case EventOper::MTT_Position:
            mtt = GraphData::MTT_Position;
            break;
        case EventOper::MTT_Distance:
            mtt = GraphData::MTT_Distance;
            break;
        case EventOper::MTT_Angle:
            mtt = GraphData::MTT_Angle;
            break;
        case EventOper::MTT_Area:
            mtt = GraphData::MTT_Area;
            break;
        case EventOper::MTT_Volume:
            mtt = GraphData::MTT_Volume;
            break;
        default:
            return;
        }

        Exchange::FITKOCCGraphObject3D* obj = markProvider->getMeasureToolGraphObject(items, mtt);
        if (!obj)
        {
            return;
        }

        // 添加到三维窗口。
        addGraphObjectToWidget(obj, graphWidget, false);

        // 修改可视化属性。
        obj->setColor(color);
        obj->update();
    }

    void OperGraphTool::clearMeasure()
    {
        // 获取可视化窗口。
        Render::FITKGraph3DWindowOCC* graphWidget = getGraphWidget();
        if (!graphWidget)
        {
            return;
        }

        // 获取工具标识可视化对象管理器。
        GraphData::GraphMarkerProvider* markProvider = GRAPH_PROVIDER_MGR->getMarkerProvider(graphWidget);
        if (!markProvider)
        {
            return;
        }

        markProvider->clearMeasureObject();
    }

    void OperGraphTool::initPickTool(bool calculateCenter, EventOper::PickToolType type, QColor color)
    {
        // 获取可视化窗口。
        Render::FITKGraph3DWindowOCC* graphWidget = getGraphWidget();
        if (!graphWidget)
        {
            return;
        }

        // 获取工具标识可视化对象管理器。
        GraphData::GraphMarkerProvider* markProvider = GRAPH_PROVIDER_MGR->getMarkerProvider(graphWidget);
        if (!markProvider)
        {
            return;
        }

        GraphData::PickToolType ptt;
        switch (type)
        {
        case EventOper::PTT_Proj_Pos:
            ptt = GraphData::PTT_Proj_Pos;
            break;
        case EventOper::PTT_Direction:
            ptt = GraphData::PTT_Direction;
            break;
        default:
            return;
        }

        Exchange::FITKOCCGraphObject3D* obj = markProvider->getPickToolGraphObject(ptt);
        if (!obj)
        {
            return;
        }

        // Modified 2024/10/08 修改为初始化窗口时自动将对象添加至三维窗口。
        // 添加到三维窗口。
        // addGraphObjectToWidget(obj, graphWidget, false);

        // 修改可视化属性。
        obj->setVisible(false);
        obj->setColor(color);
        obj->clearCache();

        // 设置计算中心属性。
        obj->setUserData(ROLE_INPUT_PICK_DIRECTION_CENTER, QVariant::fromValue<bool>(calculateCenter));

        obj->update();
    }

    void OperGraphTool::clearPickTool()
    {
        // 获取可视化窗口。
        Render::FITKGraph3DWindowOCC* graphWidget = getGraphWidget();
        if (!graphWidget)
        {
            return;
        }

        // 获取工具标识可视化对象管理器。
        GraphData::GraphMarkerProvider* markProvider = GRAPH_PROVIDER_MGR->getMarkerProvider(graphWidget);
        if (!markProvider)
        {
            return;
        }

        markProvider->clearPickToolObject();
    }

    Exchange::FITKOCCGraphObject3D* OperGraphTool::getPickToolObject(EventOper::PickToolType type)
    {
        // 获取可视化窗口。
        Render::FITKGraph3DWindowOCC* graphWidget = getGraphWidget();
        if (!graphWidget)
        {
            return nullptr;
        }

        // 获取工具标识可视化对象管理器。
        GraphData::GraphMarkerProvider* markProvider = GRAPH_PROVIDER_MGR->getMarkerProvider(graphWidget);
        if (!markProvider)
        {
            return nullptr;
        }

        GraphData::PickToolType ptt;
        switch (type)
        {
        case EventOper::PTT_Proj_Pos:
            ptt = GraphData::PTT_Proj_Pos;
            break;
        case EventOper::PTT_Direction:
            ptt = GraphData::PTT_Direction;
            break;
        default:
            return nullptr;
        }

        return markProvider->getPickToolGraphObject(ptt);
    }

    void OperGraphTool::setPickToolObjectVisible(EventOper::PickToolType type, bool visible)
    {
        // 获取可视化窗口。
        Render::FITKGraph3DWindowOCC* graphWidget = getGraphWidget();
        if (!graphWidget)
        {
            return;
        }

        // 获取工具对象。
        Exchange::FITKOCCGraphObject3D* obj = getPickToolObject(type);
        if (!obj)
        {
            return;
        }

        // 添加到三维窗口。
        addGraphObjectToWidget(obj, graphWidget, false);

        obj->setVisible(visible);
        obj->update();
    }

    void OperGraphTool::setPickDirectionAnchorTopoId(int mainId, int subId)
    {
        // 获取拾取方向工具可视化对象。
        Exchange::FITKOCCGraphObject3D* obj = getPickToolObject(EventOper::PTT_Direction);
        if (!obj)
        {
            return;
        }

        obj->setUserData(ROLE_INPUT_PICK_DIRECTION_ANCHOR_MAIN_ID, mainId);
        obj->setUserData(ROLE_INPUT_PICK_DIRECTION_ANCHOR_SUB_ID, subId);
        obj->update();
    }

    void OperGraphTool::setPickDirectionUserDirection(QList<double> dir)
    {
        // 获取拾取方向工具可视化对象。
        Exchange::FITKOCCGraphObject3D* obj = getPickToolObject(EventOper::PTT_Direction);
        if (!obj)
        {
            return;
        }

        obj->setUserData(ROLE_INPUT_PICK_DIRECTION_USER_DIRECTION, QVariant::fromValue<QList<double>>(dir));
        obj->update();
    }

    bool OperGraphTool::getPickedDirection(QList<double> & dir)
    {
        // 获取拾取方向工具可视化对象。
        Exchange::FITKOCCGraphObject3D* obj = getPickToolObject(EventOper::PTT_Direction);
        if (!obj)
        {
            return false;
        }

        QList<double> outDir = obj->getUserDataAs<QList<double>>(ROLE_OUTPUT_PICK_DIRECTION_DIRECTION);
        if (outDir.count() != 3)
        {
            return false;
        }

        dir = outDir;
        return true;
    }

    bool OperGraphTool::getPickedProjPosition(QList<double> & pos)
    {
        // 获取拾取边投影坐标工具可视化对象。
        Exchange::FITKOCCGraphObject3D* obj = getPickToolObject(EventOper::PTT_Proj_Pos);
        if (!obj)
        {
            return false;
        }

        QList<double> outPos = obj->getUserDataAs<QList<double>>(ROLE_OUTPUT_PICK_PROJ_POSITION);
        if (outPos.count() != 3)
        {
            return false;
        }

        pos = outPos;
        return true;
    }

    Render::FITKGraph3DWindowOCC* OperGraphTool::getGraphWidget()
    {
        // 获取主窗口。
        GUI::MainWindow* mainWindow = FITKGLODATA->getMainWindowT<GUI::MainWindow>();
        if (!mainWindow)
        {
            return nullptr;
        }

        // 获取可视化区。
        GUI::RenderWidget* renderWidget = mainWindow->getRenderWidget();
        if (!renderWidget)
        {
            return nullptr;
        }
        // 获取当前窗口，尝试转换为三维窗口。
        auto w = renderWidget->getGraph3DWidget();
        Render::FITKGraph3DWindowOCC* graphWidget = dynamic_cast<Render::FITKGraph3DWindowOCC*>(w);

        return graphWidget;
    }

    void OperGraphTool::addGraphObjectToWidget(Exchange::FITKOCCGraphObject3D* obj, Render::FITKGraph3DWindowOCC* graphWidget, bool fitView)
    {
        if (!obj || !graphWidget)
        {
            return;
        }

        // 添加可视化对象。
        //@{
        // obj->removeFromGraphWidget();
        obj->setGraphWidget(graphWidget);
        graphWidget->addObject(obj, false);
        //}

        // 刷新窗口。
        //@{
        if (fitView)
        {
            graphWidget->fitView();
        }
        //@}
    }

    void OperGraphTool::setPickProjPosTargetId(int targetCmdId, int targetTopoId)
    {
        // 获取拾取形状坐标工具可视化对象。
        Exchange::FITKOCCGraphObject3D* obj = getPickToolObject(EventOper::PTT_Proj_Pos);
        if (!obj)
        {
            return;
        }

        obj->setUserData(ROLE_INPUT_PICK_PROJ_POSITION_TARGET_CMD_ID, targetCmdId);
        obj->setUserData(ROLE_INPUT_PICK_PROJ_POSITION_TARGET_TOPO_ID, targetTopoId);
        // obj->update();
    }
}  // namespace GUIOper
