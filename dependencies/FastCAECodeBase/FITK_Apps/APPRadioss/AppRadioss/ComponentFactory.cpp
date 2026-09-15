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
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKCommandLineHandler.h"
#include "FITK_Component/FITKCompMessageWidget/FITKConsoleComponent.h"
#include "FITK_Component/FITKRenderWindowVTK/FITKGraph3DWindowInterface.h"
#include "FITK_Component/FITKRadiossRadIO/FITKRadiossRadIoInterface.h"
#include "FITK_Component/FITKRadiossLSDynaTransfer/FITKRadiossLSDynaIOInterface.h"
#include "FITK_Component/FITKGeoCompOCC/FITKGeoCompOCCInterface.h"
#include "FITK_Component/FITKGmshExeDriver/FITKMeshGenGmshExecInterface.h"
#include "FITK_Component/FITKGmshMshIO/FITKGmshMshIOInterface.h"
#include "FITK_Component/FITKPlotWindow/FITKPlotWindowsInterface.h"
#include "FITK_Component/FITKRadiossDataIOHDF5/FITKRadiossDataHDF5Interface.h"
#include "FITK_Component/FITKGeoOCCIOHDF5/FITKOCCIOHDF5Interface.h"
#include "FITK_Component/FITKNastranBDFIO/FITKNastranBDFIOInterface.h"
#include "FITK_Component/FITKAbaqusRadiossDataMapper/FITKAbaqusRadiossDataMapperInterface.h"
#include "FITK_Component/FITKMeshIO/FITKMeshIOInterface.h"
#include "FITK_Component/FITKAIAssistant/FITKAIAssistantInterface.h"
#include "FITK_Component/FITKHttpPythonDriver/FITKHttpPythonDriverInterface.h"
#include "OperatorsModel/OperatorModelInterface.h"
#include "OperatorsGUI/OpersGUIInterface.h"
#include "PreWindowInitializer.h"
#include "FITK_Component/FITKAbaqusIOINP/FITKAbaqusIOINPInterface.h"

QList<AppFrame::FITKComponentInterface *> ComponentFactory::createComponents()
{
    // 自定义组件列表
    QList<AppFrame::FITKComponentInterface *> componentList;
    // 消息窗口组件
    componentList << new Comp::ConsoleComponent(FITKAPP->getGlobalData()->getMainWindow());
    // 3D图形窗口组件
    auto compVTKrender = new Comp::FITKGraph3DWindowInterface;
    // VTK窗口初始化器
    compVTKrender->addInitializer(1, new PreWindowInitializer);
    componentList << compVTKrender;
    // OCC组件
    componentList << new OCC::FITKGeoCompOCCInterface;
    // Radioss读写组件
    componentList <<  new Radioss::FITKRadiossRadIoInterface;
    // LS-Dyna读写组件
    componentList <<  new Radioss::FITKRadiossLSDynaIOInterface;
    // Gmsh网格生成组件
    componentList << new Gmsh::FITKMeshGenGmshExecInterface;
    // Gmsh网格读写组件
    componentList << new Gmsh::FITKGmshMshIOInterface;
    //图表窗口
    componentList << new Plot::FITKPlotWindowsInterface;
    //操作器
    componentList << new OperModel::OpersModelInterface;
    componentList << new GUIOper::OperGUIInterface;

    componentList << new IO::FITKRadiossDataHDF5Interface;
    componentList << new IO::FITKOCCIOHDF5Interface;
    componentList << new Nastran::FITKNastranBDFIOInterface;
    componentList << new AbaqusRadiossDataMapper::FITKAbaqusRadiossDataMapperInterface;
    componentList << new IO::FITKMeshIOInterface;
    componentList << new IO::FITKAbaqusINPIO;

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

    // 开启拓扑偏移。
    Comp::FITKGraph3DWindowInterface::SetResolveCoincidentType(Comp::ResolveCoincidentType::RCT_PolygonOffset);

    return componentList;
}
