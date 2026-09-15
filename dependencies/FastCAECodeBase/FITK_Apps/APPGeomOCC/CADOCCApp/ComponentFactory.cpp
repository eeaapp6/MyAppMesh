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
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKCommandLineHandler.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Component/FITKCompMessageWidget/FITKConsoleComponent.h"
#include "FITK_Component/FITKRenderWindowOCC/FITKGraph3DWindowOCCInterface.h"
#include "FITK_Component/FITKGeoCompOCC/FITKGeoCompOCCInterface.h"
#include "FITK_Component/FITKGeoOCCIOHDF5/FITKOCCIOHDF5Interface.h"
#include "FITK_Component/FITKAIAssistant/FITKAIAssistantInterface.h"
#include "FITK_Component/FITKHttpPythonDriver/FITKHttpPythonDriverInterface.h"
#include "OperatorsModel/OpersModelInterface.h"
#include "OperatorsGUI/OpersGUIInterface.h"

QList<AppFrame::FITKComponentInterface *> ComponentFactory::createComponents()
{
    // 自定义组件列表
    QList<AppFrame::FITKComponentInterface *> componentList;
    // 消息窗口组件
    componentList << new Comp::ConsoleComponent(FITKGLODATA->getMainWindow());
    // 模型数据控制器组件
    componentList << new OperModel::OpersModelInterface;
    // 界面控制器组件
    componentList << new GUIOper::OperatorsGUIInterface;
    //读写HDF5
    componentList << new IO::FITKOCCIOHDF5Interface;
    // 3D图形窗口组件
    auto compOCCRender = new Render::FITKGraph3DWindowOCCInterface;
    componentList << compOCCRender;
    //occ 建模
    componentList << new OCC::FITKGeoCompOCCInterface;

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
