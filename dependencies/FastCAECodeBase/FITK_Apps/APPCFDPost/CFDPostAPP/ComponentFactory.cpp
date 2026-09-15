/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file ComponentFactory.cpp
 * @brief 组件工厂
 * @author YanZhiHui (chanyuantiandao@126.com)
 * @date 2024-04-19
 */
#include "ComponentFactory.h"
#include "PreWindowInitializer.h"

#include "OperatorsModel/OpersModelInterface.h"
#include "OperatorsGUI/OperatorsGUIInterface.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKCommandLineHandler.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Component/FITKRenderWindowVTK/FITKGraph3DWindowInterface.h"
#include "FITK_Component/FITKPlotWindow/FITKPlotWindowsInterface.h"
#include "FITK_Component/FITKHttpPythonDriver/FITKHttpPythonDriverInterface.h"
#include "FITK_Component/FITKAIAssistant/FITKAIAssistantInterface.h"

#include <QDebug>

QList<AppFrame::FITKComponentInterface *> ComponentFactory::createComponents()
{
    // 自定义组件列表
    QList<AppFrame::FITKComponentInterface *> componentList;
    // 3D图形窗口组件
    auto compVTKrender = new Comp::FITKGraph3DWindowInterface;
    compVTKrender->addInitializer(1, new PreWindowInitializer);
    componentList << compVTKrender;
    // 模型数据控制器组件
    componentList << new OperModel::OpersModelInterface;
    componentList << new OperGUI::OperatorsGUIInterface;
    componentList << new Plot::FITKPlotWindowsInterface;

    if (FITKAPP->getCommandLineHandler()->containsValue("-httpserver"))
    {
        //Http组件
        componentList << new HttpPython::FITKHttpPythonDriverInterface();
    }
    else
    {
        // AI助手组件
        componentList << new AIAssistant::FITKAIAssistantInterface;
    }

    return componentList;
}
