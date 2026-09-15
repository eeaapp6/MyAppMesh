/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalEventFilter.h"
#include "FITK_Kernel/FITKAppFramework/FITKCommandLineHandler.h"
#include "MainWindowGenerator.h"
#include "GlobalDataFactory.h"
#include "ComponentFactory.h"
#include "RadiossAppSettings.h"
#include "CommandLineHandler.h"
#include "SignalProcessor.h"
// #include "AcousticsAppSettings.h"
// #include "WorkBenchHandler.h"
// #include "SystemChecker.h"
#include "AppInitializer.h"
 #include "PythonRegister.h"
// #include "UndoRedoKeyEventFilter.h"
#ifdef Q_OS_WIN
#include <Windows.h>
#endif // Q_WIN
/**
 * @brief 程序的主入口函数
 * @param argc 命令行参数个数
 * @param argv 命令行参数值数组
 * @return int 程序执行的返回状态
 * @author libaojun(libaojunqd@foxmail.com)
 * @date 2024-04-19
 */
int main(int argc, char *argv[])
{
    AppFrame::FITKAbstractMainwindowGenerator::EnableHighDPIScaling();
    // 初始化应用框架
    AppFrame::FITKApplication app(argc, argv);
    app.setStyle("Fusion");
    app.setApplicationName("FastCAE-OpenRadioss");
//    app.checkSystem(new SystemChecker);
   // 注册程序的主要组件和设置
    app.regMainWindowGenerator(new MainWindowGenerator); // 注册主窗口生成器
    app.regGlobalDataFactory(new GlobalDataFactory);     // 注册全局数据工厂
    app.regComponentsFactory(new ComponentFactory);      // 注册组件工厂
    app.regAppSettings(new RadiossAPPSettings);        // 注册应用设置
    app.regCommandLineHandler(new CommandLineHandler);   // 注册命令行处理
    app.addGolbalSignalProcesser(new SignalProcessor);   // 信号处理器
//     app.regWorkBenchHandler(new FlowAppWorkBenchHandler); //workbench处理
    app.regAppInitalizer(new AppInitializer);             //注册初始化器
    app.regPythonRegister(new PyRegister);
//     app.getGlobalEventFilter()->addEventFilter(new UndoRedoKeyEventFilter);
    app.setPluginKey("RadiossAppPlugin");
    app.setApplicationName("APPRadioss");
    // 运行应用程序的消息循环
    return app.exec();
}

