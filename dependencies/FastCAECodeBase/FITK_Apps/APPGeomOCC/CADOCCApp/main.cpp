/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "ApplicationCAD.h"
#include "MainWindowGenerator.h"
#include "GlobalDataFactory.h"
#include "ComponentFactory.h"
#include "CommandLineHandler.h"
#include "AppInitializer.h"
#include "SignalProcessor.h"
#include "WorkBenchHandler.h"
#include "PythonRegister.h"
#include "CADOCCSettings.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKAbstractGlobalDataFactory.h"
#include "FITK_Kernel/FITKAppFramework/FITKRunTimeSetting.h"

/**
 * @brief 程序的主入口函数
 * @param argc 命令行参数个数
 * @param argv 命令行参数值数组
 * @return int 程序执行的返回状态
 * @author YanZhiHui(chanyuantiandao@126.com)
 * @date 2024-04-19
 */
int main(int argc, char *argv[])
{
    //控制图片缩放质量
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    // 初始化应用框架
    ApplicationCAD app(argc, argv);

#ifdef Q_OS_LINUX
    qputenv("PYTHONHOME", app.applicationDirPath().toUtf8() + "/../Python37");

    QStringList pythonPaths;
    pythonPaths << app.applicationDirPath() + "/../Python37/bin";
    pythonPaths << app.applicationDirPath() + "/../Python37/lib";
    pythonPaths << app.applicationDirPath() + "/../Python37/lib/python3.7";
    pythonPaths << app.applicationDirPath() + "/../Python37/lib/python3.7/site-packages";
    pythonPaths << app.applicationDirPath() + "/../Python37/lib/python3.7/lib-dynload";
    qputenv("PYTHONPATH", pythonPaths.join(":").toUtf8());
#endif

   // 注册程序的主要组件和设置
    app.regMainWindowGenerator(new MainWindowGenerator); // 注册主窗口生成器
    app.regGlobalDataFactory(new GlobalDataFactory);     // 注册全局数据工厂
    app.regComponentsFactory(new ComponentFactory);      // 注册组件工厂
    app.regCommandLineHandler(new CommandLineHandler);   // 注册命令行处理
    app.regAppInitalizer(new AppInitializer);             //注册初始化器
    app.addGolbalSignalProcesser(new SignalProcessor);
    app.regWorkBenchHandler(new OCCAppWorkBenchHandler);
    app.regAppSettings(new CADOCCSettings);          // 注册应用设置

    /*
    // 注册全局信号处理器
    app.addGolbalSignalProcesser(new ThreadTaskProcessHandler);
    //注册自定义事件处理
    app.addEventFilter(new MouseKeyEventFilter);*/
    // 运行应用程序的消息循环

    //注册python接口
    app.regPythonRegister(new PyRegister);
    app.setApplicationName("APPCADOCC");
    // 开发测试属性。
    //@{
    app.getGlobalData()->getRunTimeSetting()->setValue("DEV", app.arguments().contains("-dev"));
    //@}

    return app.exec();
}

