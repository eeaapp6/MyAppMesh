/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperGraphPick.h"

// App
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

// Render
#include "FITK_Component/FITKRenderWindowOCC/FITKGraph3DWindowOCC.h"

// Graph data manager
#include "GraphDataProvider/GraphProviderManager.h"
#include "GraphDataProvider/GraphModelProvider.h"
#include "GraphDataProvider/GraphMarkerProvider.h"

// GUI
#include "GUIFrame/MainWindow.h"
#include "GUIFrame/RenderWidget.h"
#include "FITK_Component/FITKWidget/FITKMdiArea.h"
#include "PickDataProvider/GUIPickInfo.h"

// Operator
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "OperatorsInterface/GraphEventOperator.h"
#include "OperatorsInterface/GraphToolOperator.h"

namespace GUIOper
{
    void OperGraphPick::setGraphStateByPickInfo(int dataObjId)
    {
        // 获取当前拾取方式信息。
        GUI::GUIPickInfoStru pickInfo = GUI::GUIPickInfo::GetPickInfo();

        // 获取可视化窗口。
        Render::FITKGraph3DWindowOCC* graphWidget = getGraphWidget();
        if (!graphWidget)
        {
            return;
        }

        // 获取可视化对象管理器。
        GraphData::GraphModelProvider* modelProvider = GRAPH_PROVIDER_MGR->getModelProvider(graphWidget);
        GraphData::GraphMarkerProvider* markProvider = GRAPH_PROVIDER_MGR->getMarkerProvider(graphWidget);
        if (!modelProvider || !markProvider)
        {
            return;
        }

        // 判断是否为特殊附加拾取功能。
        bool isAddinPickMode = pickInfo._pickObjType == GUI::GUIPickInfo::PickObjType::POBJDirecEdge ||
            pickInfo._pickObjType == GUI::GUIPickInfo::PickObjType::POBJDirecFace ||
            pickInfo._pickObjType == GUI::GUIPickInfo::PickObjType::POBJEdgeProjPos ||
            pickInfo._pickObjType == GUI::GUIPickInfo::PickObjType::POBJFaceProjPos;

        // 开启或关闭Shift与Alt框选拾取状态。
        bool isMultiSelect = (GUI::GUIPickInfo::PickMethod::PMIndividually == pickInfo._pickMethod && !isAddinPickMode);
        graphWidget->enableShiftSelect(isMultiSelect);
        graphWidget->enableAreaSelect(isMultiSelect);
        graphWidget->enableSelect(true);
        // graphWidget->enableSelect(GUI::GUIPickInfo::PickObjType::POBJNone != pickInfo._pickObjType);
        graphWidget->clearSelection();

        // 获取可视化对象操作器。
        EventOper::GraphToolOperator* operTool = FITKOPERREPO->getOperatorT<EventOper::GraphToolOperator>("GraphTool");

        // 转换拾取方式。
        CommonOCC::SelectMode sMode;
        switch (pickInfo._pickObjType)
        {
        case GUI::GUIPickInfo::PickObjType::POBJCustom: sMode = CommonOCC::SelectMode::S_ShapeShape; break;
        case GUI::GUIPickInfo::PickObjType::POBJVert: sMode = CommonOCC::SelectMode::S_ShapeVertex; break;
        case GUI::GUIPickInfo::PickObjType::POBJEdge: sMode = CommonOCC::SelectMode::S_ShapeEdge; break;
        case GUI::GUIPickInfo::PickObjType::POBJWire: sMode = CommonOCC::SelectMode::S_ShapeWire; break;
        case GUI::GUIPickInfo::PickObjType::POBJFace: sMode = CommonOCC::SelectMode::S_ShapeFace; break;
        case GUI::GUIPickInfo::PickObjType::POBJShell: sMode = CommonOCC::SelectMode::S_ShapeShell; break;
        case GUI::GUIPickInfo::PickObjType::POBJSolid: sMode = CommonOCC::SelectMode::S_ShapeSolid; break;
        case GUI::GUIPickInfo::PickObjType::POBJAssembly: sMode = CommonOCC::SelectMode::S_ShapeCompound; break;
        case GUI::GUIPickInfo::PickObjType::POBJShape: sMode = CommonOCC::SelectMode::S_ShapeShape; break;
        case GUI::GUIPickInfo::PickObjType::POBJDirecEdge:
        {
            // 开启拾取方向模式。
            if (operTool)
            {
                operTool->initPickTool(pickInfo._enablePickCenter, EventOper::PickToolType::PTT_Direction);
            }

            sMode = CommonOCC::SelectMode::S_ShapeEdge;
            break;
        }
        case GUI::GUIPickInfo::PickObjType::POBJDirecFace:
        {
            // 开启拾取方向模式。
            if (operTool)
            {
                operTool->initPickTool(pickInfo._enablePickCenter, EventOper::PickToolType::PTT_Direction);
            }

            sMode = CommonOCC::SelectMode::S_ShapeFace;
            break;
        }
        case GUI::GUIPickInfo::PickObjType::POBJEdgeProjPos:
        {
            // 开启拾取边投影模式。
            if (operTool)
            {
                operTool->initPickTool(pickInfo._enablePickCenter, EventOper::PickToolType::PTT_Proj_Pos);
            }

            sMode = CommonOCC::SelectMode::S_ShapeEdge;
            break;
        }
        case GUI::GUIPickInfo::PickObjType::POBJFaceProjPos:
        {
            // 开启拾取面投影模式。
            if (operTool)
            {
                operTool->initPickTool(pickInfo._enablePickCenter, EventOper::PickToolType::PTT_Proj_Pos);
            }

            sMode = CommonOCC::SelectMode::S_ShapeFace;
            break;
        }
        default: sMode = CommonOCC::SelectMode::S_None;
        };

        // 清除拾取工具。
        //if (!isAddinPickMode && operGraph)
        //{
        //    operGraph->clearPickTool();
        //}

        // 开启模型拾取。
        modelProvider->setModelSelectMode(pickInfo._enableModelPick ? sMode : CommonOCC::SelectMode::S_None, dataObjId);

        // 拾取工具模式下如果草图被禁用拾取，则开启草图内部拾取功能。
        if (pickInfo._pickObjType == GUI::GUIPickInfo::PickObjType::POBJEdge &&
            !pickInfo._enableSketchPick)
        {
            modelProvider->setSketchSelectEdgeMode(dataObjId);
        }
        else
        {
            // 开启草图拾取。
            bool enableSketch = (pickInfo._pickObjType == GUI::GUIPickInfo::PickObjType::POBJCustom ||
                pickInfo._pickObjType == GUI::GUIPickInfo::PickObjType::POBJDirecFace ||
                pickInfo._pickObjType == GUI::GUIPickInfo::PickObjType::POBJDirecEdge ||
                pickInfo._pickObjType == GUI::GUIPickInfo::PickObjType::POBJVert ||
                pickInfo._pickObjType == GUI::GUIPickInfo::PickObjType::POBJEdge ||
                pickInfo._pickObjType == GUI::GUIPickInfo::PickObjType::POBJFace)
                && pickInfo._enableSketchPick;

            modelProvider->setSketchSelectMode(enableSketch ? sMode : CommonOCC::SelectMode::S_None, dataObjId);
        }

        // 开启基准元素拾取。
        modelProvider->setDatumSelectMode((pickInfo._enableDatumPick && !isAddinPickMode) ? sMode : CommonOCC::SelectMode::S_None, dataObjId);
    }

    void OperGraphPick::setEnableGraphPickable(bool model, bool sketch, bool datum)
    {
        // 获取当前拾取方式信息,保存拾取状态。
        GUI::GUIPickInfoStru pickInfo = GUI::GUIPickInfo::GetPickInfo();
        pickInfo._enableModelPick = model;
        pickInfo._enableSketchPick = sketch;
        pickInfo._enableDatumPick = datum;
        // GUI::GUIPickInfo::SetPickInfo(pickInfo);

        // 获取可视化窗口。
        Render::FITKGraph3DWindowOCC* graphWidget = getGraphWidget();
        if (!graphWidget)
        {
            return;
        }

        // 获取可视化对象管理器。
        GraphData::GraphModelProvider* modelProvider = GRAPH_PROVIDER_MGR->getModelProvider(graphWidget);
        GraphData::GraphMarkerProvider* markProvider = GRAPH_PROVIDER_MGR->getMarkerProvider(graphWidget);
        if (!modelProvider || !markProvider)
        {
            return;
        }

        // 转换拾取方式。
        CommonOCC::SelectMode sMode;
        switch (pickInfo._pickObjType)
        {
        case GUI::GUIPickInfo::PickObjType::POBJCustom: sMode = CommonOCC::SelectMode::S_ShapeShape; break;
        case GUI::GUIPickInfo::PickObjType::POBJVert: sMode = CommonOCC::SelectMode::S_ShapeVertex; break;
        case GUI::GUIPickInfo::PickObjType::POBJEdge: sMode = CommonOCC::SelectMode::S_ShapeEdge; break;
        case GUI::GUIPickInfo::PickObjType::POBJWire: sMode = CommonOCC::SelectMode::S_ShapeWire; break;
        case GUI::GUIPickInfo::PickObjType::POBJFace: sMode = CommonOCC::SelectMode::S_ShapeFace; break;
        case GUI::GUIPickInfo::PickObjType::POBJShell: sMode = CommonOCC::SelectMode::S_ShapeShell; break;
        case GUI::GUIPickInfo::PickObjType::POBJSolid: sMode = CommonOCC::SelectMode::S_ShapeSolid; break;
        case GUI::GUIPickInfo::PickObjType::POBJAssembly: sMode = CommonOCC::SelectMode::S_ShapeCompound; break;
        case GUI::GUIPickInfo::PickObjType::POBJShape: sMode = CommonOCC::SelectMode::S_ShapeShape; break;
        default: sMode = CommonOCC::SelectMode::S_None;
        };

        // 开启模型拾取。
        modelProvider->setModelSelectMode(pickInfo._enableModelPick ? sMode : CommonOCC::SelectMode::S_None);

        // 开启草图拾取。
        modelProvider->setSketchSelectMode(pickInfo._enableSketchPick ? sMode : CommonOCC::SelectMode::S_None);

        // 开启基准元素拾取。
        modelProvider->setDatumSelectMode(pickInfo._enableDatumPick ? sMode : CommonOCC::SelectMode::S_None);
    }

    Render::FITKGraph3DWindowOCC* OperGraphPick::getGraphWidget()
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

        // 获取当前可视化窗口，尝试转换为三维窗口。
        Render::FITKGraph3DWindowOCC* graphWidget = dynamic_cast<Render::FITKGraph3DWindowOCC*>(renderWidget->getGraph3DWidget());

        return graphWidget;
    }

    void OperGraphPick::setCustomPickMode()
    {
        // 重置拾取模式。
        GUI::GUIPickInfoStru pickInfo;
        pickInfo._pickMethod = GUI::GUIPickInfo::PickMethod::PMIndividually;
        pickInfo._pickObjType = GUI::GUIPickInfo::PickObjType::POBJCustom;
        pickInfo._enableModelPick = true;
        pickInfo._enableDatumPick = true;
        pickInfo._enableSketchPick = true;
        GUI::GUIPickInfo::SetPickInfo(pickInfo);
    }

    void OperGraphPick::clearPickMode()
    {
        // 重置拾取模式。
        GUI::GUIPickInfoStru pickInfo;
        pickInfo._pickMethod = GUI::GUIPickInfo::PickMethod::PMIndividually;
        pickInfo._pickObjType = GUI::GUIPickInfo::PickObjType::POBJNone;
        pickInfo._enableModelPick = false;
        pickInfo._enableDatumPick = false;
        pickInfo._enableSketchPick = false;
        GUI::GUIPickInfo::SetPickInfo(pickInfo);
    }
}  // namespace GUIOper