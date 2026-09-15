/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUIPickInfo.h"

#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKAppFramework/FITKRunTimeSetting.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"

// GUI
#include "GUIFrame/MainWindow.h"
#include "GUIFrame/RenderWidget.h"
#include "FITK_Component/FITKWidget/FITKMdiArea.h"

// Render
#include "FITK_Component/FITKRenderWindowOCC/FITKGraph3DWindowOCC.h"
#include "FITK_Component/FITKRenderWindowOCC/FITKGraph3DWindowOCCInterface.h"

// Operator
#include "OperatorsInterface/GraphInteractionOperator.h"
#include "OperatorsInterface/GraphEventOperator.h"

#define  PickObjTypeStr      "PickObjType"
#define  PickMethodStr       "PickMethod"
#define  EnableModelPickStr  "EnableModelPick"
#define  EnableDatumPickStr  "EnableDatumPick"
#define  EnableSketchPickStr "EnableSketchPick"
#define  EnablePickCenterStr "EnablePickCenter"

namespace GUI
{
    /**
  * @brief 清除选择信息
  * 该函数用于清除运行时设置中的与选择信息相关的变量。
  */
    void GUIPickInfo::ClearPickInfo()
    {
        auto RTsetting = FITKGLODATA->getRunTimeSetting();
        if (RTsetting == nullptr) return;

        // 移除运行时设置中的变量
        RTsetting->removeVarient(PickObjTypeStr);
        RTsetting->removeVarient(PickMethodStr);
        RTsetting->removeVarient(EnableModelPickStr);
        RTsetting->removeVarient(EnableDatumPickStr);
        RTsetting->removeVarient(EnableSketchPickStr);
        RTsetting->removeVarient(EnablePickCenterStr);

        // 获取主窗口。
        GUI::MainWindow* mainWindow = FITKGLODATA->getMainWindowT<GUI::MainWindow>();
        if (!mainWindow) return;

        // 获取可视化区。
        GUI::RenderWidget* renderWidget = mainWindow->getRenderWidget();
        if (!renderWidget) return;

        // 获取当前窗口，尝试转换为三维窗口。
        auto* w = renderWidget->getGraph3DWidget();
        auto graphWidget = dynamic_cast<Render::FITKGraph3DWindowOCC*>(w);
        // graphWidget->setSelectMode(SelectMode::S_None, false);

        // 清除高亮。
        EventOper::GraphEventOperator* operPre = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (operPre)
        {
            operPre->clearHighlight();
        }

        // 设置到交互器。
        EventOper::GraphInteractionOperator* operPick = FITKOPERREPO->getOperatorT<EventOper::GraphInteractionOperator>("GraphPick");
        if (operPick)
        {
            operPick->setGraphStateByPickInfo();
        }
    }

    /**
     * @brief 设置选择信息
     * @param info 包含选择信息的结构体
     * 该函数用于设置运行时设置中的与选择信息相关的变量。
     */
    void GUIPickInfo::SetPickInfo(GUIPickInfoStru info, int dataObjId)
    {
        auto RTsetting = FITKGLODATA->getRunTimeSetting();
        if (RTsetting == nullptr) return;

        // 设置运行时设置中的变量值
        RTsetting->setValue(PickObjTypeStr, (int)info._pickObjType);
        RTsetting->setValue(PickMethodStr, (int)info._pickMethod);
        RTsetting->setValue(EnableModelPickStr, (bool)info._enableModelPick);
        RTsetting->setValue(EnableDatumPickStr, (bool)info._enableDatumPick);
        RTsetting->setValue(EnableSketchPickStr, (bool)info._enableSketchPick);
        RTsetting->setValue(EnablePickCenterStr, (bool)info._enablePickCenter);

        // 清除高亮。
        EventOper::GraphEventOperator* operPre = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (operPre)
        {
            operPre->clearHighlight();
        }

        // 设置到交互器。
        EventOper::GraphInteractionOperator* operPick = FITKOPERREPO->getOperatorT<EventOper::GraphInteractionOperator>("GraphPick");
        if (operPick)
        {
            operPick->setGraphStateByPickInfo(dataObjId);
        }
    }

    /**
     * @brief 获取选择信息
     * @return GUIPickInfoStru 返回选择信息的结构体
     * 该函数用于从运行时设置中获取与选择信息相关的变量值，并封装到一个结构体中返回。
     */
    GUIPickInfoStru GUIPickInfo::GetPickInfo()
    {
        GUIPickInfoStru pickInfo;

        auto RTsetting = FITKGLODATA->getRunTimeSetting();
        if (RTsetting == nullptr) return pickInfo;
        // 从运行时设置中获取变量值
        pickInfo._pickObjType = (GUIPickInfo::PickObjType)RTsetting->getValueT<int>(PickObjTypeStr);
        pickInfo._pickMethod = (GUIPickInfo::PickMethod)RTsetting->getValueT<int>(PickMethodStr);
        pickInfo._enableModelPick = RTsetting->getValueT<bool>(EnableModelPickStr);
        pickInfo._enableDatumPick = RTsetting->getValueT<bool>(EnableDatumPickStr);
        pickInfo._enableSketchPick = RTsetting->getValueT<bool>(EnableSketchPickStr);
        pickInfo._enablePickCenter = RTsetting->getValueT<bool>(EnablePickCenterStr);

        return pickInfo;
    }
}