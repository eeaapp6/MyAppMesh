/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include <QApplication>

//FITK框架
#include "MainWindowGenerator.h"
#include "GlobalDataFactory.h"
#include "ComponentFactory.h"
#include "CommandLineHandler.h"
#include "WorkBenchHandler.h"
#include "CFDPostAPPSettings.h"
#include "AppInitializer.h"
#include "PythonRegister.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"

int main(int argc, char* argv[])
{
    //初始化应用框架
    AppFrame::FITKApplication postApp(argc, argv);
    // 注册程序的主要组件和设置
    postApp.regMainWindowGenerator(new MainWindowGenerator); // 注册主窗口生成器
    postApp.regGlobalDataFactory(new GlobalDataFactory);     // 注册全局数据工厂
    postApp.regComponentsFactory(new ComponentFactory);      // 注册组件工厂
    postApp.regCommandLineHandler(new CommandLineHandler);   // 注册命令处理器
    postApp.regAppInitalizer(new AppInitializer);             //注册初始化器
    postApp.regWorkBenchHandler(new CFDPostWorkBenchHandler);// 工作台处理器
    postApp.regAppSettings(new CFDPostAPPSettings);          // 注册软件设置

    //注册python接口
    postApp.regPythonRegister(new PyRegister);
    postApp.setApplicationName("APPCFDPost");

    return postApp.exec();
}
