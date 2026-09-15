/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKAppFramework.h
 * @brief  应用程序核心声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-02-22
 *
 */
#ifndef __FITKAPPFRAMEWORK_H___
#define __FITKAPPFRAMEWORK_H___

#include <QApplication>
#include <QString>
#include "FITKAppFrameworkAPI.h"

namespace AppFrame
{
    class FITKGlobalData;
    class FITKCmponents;
    class FITKAbstractMainwindowGenerator;
    class FITKAbstractGlobalDataFactory;
    class FITKAbstractAutoSaver;
    class FITKComponentFactory;
    class FITKSignalTransfer;
    class FITKGlobalEventFilter;
    class FITKAbstractEventFilter;
    class FITKAppSettings;
    class FITKCommandLineHandler;
    class FITKWorkBenchHandler;
    class FITKProgramTaskManeger;
    class FITKAbstractPythonRegister;
    class FITKAbstractSysChecker;
    class FITKAbstractAppInitializer;
    class FITKAppScriptHelper;
    class FITKPluginsManager;
    class FITKAbstractLicenseChecker;

    /**
     * @brief AppFrame类，应用程序的核心
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-02-22
     */
    class FITKAppFrameworkAPI FITKApplication : public QApplication
    {
        Q_OBJECT
    public:
        /**
         * @brief Construct a new FITKApplication object
         * @param[i]  argc           程序启动命令行参数
         * @param[i]  argv           程序启动命令行参数
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        explicit FITKApplication(int &argc, char **argv);
        /**
         * @brief Destroy the FITKApplication object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        virtual ~FITKApplication();
        
        /**
         * @brief 检查系统是否符合要求
         * @param[i] checker 系统检查器
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-09-18
         */
        void checkSystem(FITKAbstractSysChecker* checker);
        
        /**
         * @brief 完成程序初始化，启动程序进入事件循环
         * @return int
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        int exec();
        /**
         * @brief 注册主界面生成器
         * @param[i]  generator      主界面生成器
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        void regMainWindowGenerator(FITKAbstractMainwindowGenerator* generator);

        /**
         * @brief 注册全局数据生成器
         * @param[i]  fac         全局数据生成
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-29
         */
        void regGlobalDataFactory(FITKAbstractGlobalDataFactory* fac);

        /**
         * @brief 注册命令行处理器
         * @param[i]  handler            命令行处理器
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-05
         */
        void regCommandLineHandler(FITKCommandLineHandler* handler);
        /**
         * @brief 获取命令行处理器
         * @return FITKCommandLineHandler*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-05
         */
        FITKCommandLineHandler* getCommandLineHandler();

        /**
         * @brief 注册设置存储
         * @param[i]  settings            存储器
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-05
         */
        void regAppSettings(FITKAppSettings* settings);

        /**
         * @brief 获取设置存储
         * @return FITKAppSettings*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-05
         */
        FITKAppSettings* getAppSettings();

        /**
         * @brief 获取全局数据管理器
         * @return FITKGlobalData*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-26
         */
        FITKGlobalData* getGlobalData();
        /**
         * @brief 注册组件生产工厂
         * @param[i]  fac            组件生产工厂
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-05
         */
        void regComponentsFactory(FITKComponentFactory* fac);
        /**
         * @brief 注册Python接口
         * @param[i]  pyreg            接口注册器
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-18
         */
        void regPythonRegister(FITKAbstractPythonRegister* pyreg);

        /**
         * @brief 获取组件管理器
         * @return FITKCmponents*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-05
         */
        FITKCmponents* getComponents();
        /**
         * @brief 获取信号转接器
         * @return FITKSignalTransfer*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-05
         */
        FITKSignalTransfer* getSignalTransfer();
        /**
        * @brief 获取启动的程序管理器，可执行程序形式执行
        * @return FITKProgramTaskManeger*
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-06-09
        */
        FITKProgramTaskManeger* getProgramTaskManager();
        /**
         * @brief 追加信号理器，会根据名称自动关联signalTransfer的信号
         * @param[i]  obj           处理器
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-08
         */
        void addGolbalSignalProcesser(QObject* obj);
        /**
         * @brief 追加自定义 事件处理器
         * @param[i]  filter           处理器
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-30
         */
        void addEventFilter(FITKAbstractEventFilter* filter);
       /**
        * @brief 注册自动保存器
        * @param[i]  saver          自动保存器
        * @author libaojun (libaojunqd@foxmail.com)
        * @date 2024-09-13
        */
        void regAutoSaver(FITKAbstractAutoSaver* saver);
        /**
         * @brief 获取自动保存器
         * @return FITKAbstractAutoSaver*
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-09-13
         */
        FITKAbstractAutoSaver* getAutoSaver();
        /**
        * @brief 注册初始化器
        * @param[i]  saver          自动保存器
        * @author libaojun (libaojunqd@foxmail.com)
        * @date 2024-10-23
        */
        void regAppInitalizer(FITKAbstractAppInitializer* ini);
        /**
         * @brief 获取初始化器
         * @return FITKAbstractAppInitializer*
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-10-23
         */
        FITKAbstractAppInitializer* getAppInitalizer();

        /**
         * @brief 注册许可证检查器
         * @param[i] checker 许可证检查器
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-04-02
         */
        void regLicenseChecker(FITKAbstractLicenseChecker* checker);

        /**
         * @brief 获取许可证检查器
         * @return FITKAbstractLicenseChecker*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-04-02
         */
        FITKAbstractLicenseChecker* getLicenseChecker();

        /**
         * @brief 程序初始化，加载动态库，生成主界面等操作，由exec调用
         * @return true  初始化成功
         * @return false 初始化失败
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        virtual bool init();

        /**
         * @brief 获取全局事件处理
         * @return FITKGlobalEventFilter*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-30
         */
        FITKGlobalEventFilter* getGlobalEventFilter();
        /**
         * @brief 显示版权信息对话框
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-30
         */
        virtual void showCopyrightDlg();
        /**
         * @brief 重置软件中的数据对象，globaldata的中的对象不会被清除
         * @param savedIDs 需要额外保存的数据ID
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-30
         */
        void resetDataObjects(const QList<int>& savedIDs = QList<int>());
        /**
         * @brief 是否在workbench中运行
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-19
         */
        bool workingInWorkBench();
        /**
         * @brief 注册在workbench中处理的事件
         * @param[i]  handler        处理器
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-19
         */
        void regWorkBenchHandler(FITKWorkBenchHandler* handler );
        /**
         * @brief 获取workbench处理器
         * @return FITKWorkBenchHandler* 
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-19
         */
        FITKWorkBenchHandler* getWorkBenchHandler();
        /**
         * @brief 获取脚本辅助操作器
         * @return FITKAppScriptHelper*
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-19
         */
        FITKAppScriptHelper* getScriptHelper();

        /**
         * @brief 获取插件管理器
         * @return FITKPluginsManager*
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-19
         */
        FITKPluginsManager* getPluginsManager();
        /**
         * @brief 设置插件识别关键字
         * @param[i] k 插件识别关键字 
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-19
         */
        void setPluginKey(const QString& k);
        /**
         * @brief 将字符串设置到系统剪贴板
         * @param[i]  text           字符串内容
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-03-07
         */
        void setClipboardText(const QString & text) const;
        /**
         * @brief 获取系统剪贴板内容
         * @return QString
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-03-07
         */
        QString getClipboardText() const;
        /**
         * @brief 获取临时目录,可以指定子目录,若子目录不存在则创建
         * @return QString
         * @param[i]  subDir        子目录 
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-05-02
         */
        QString getTempDir(const bool & clearDir = false , const QString& subDir = QString()) const;
        /**
         * @brief 设置是否在MPI环境中运行
         * @param[i]  run            是否在MPI环境中运行
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-09-11
         */
        void setRunInMPI(bool run);
        /**
         * @brief 获取是否在MPI环境中运行
         * @return true
         * @return false
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-09-11
         */
        bool getRunInMPI() const;

    private:
         /**
          * @brief 执行命行参数
          * @author LiBaojun (libaojunqd@foxmail.com)
          * @date 2024-04-13
          */
        void execCommand();

    private:
        /**
       * @brief 主界面生成器
       * @author LiBaojun (libaojunqd@foxmail.com)
       * @date 2024-09-18
       */
        bool _passSystemCheck{ true };
        /**
         * @brief 主界面生成器
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        FITKAbstractMainwindowGenerator* _mwGenerator{};
        /**
         * @brief 全局数据生成器
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-29
         */
        FITKAbstractGlobalDataFactory* _gDataFactory{};

        /**
         * @brief 全局数据总线
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        FITKGlobalData* _golbalData{};
        /**
         * @brief 组件生产工厂
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-05
         */
        FITKComponentFactory* _componentsFactory{};
        /**
         * @brief 组件管理器
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-05
         */
        FITKCmponents* _components{};
        /**
         * @brief 信号转接器
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-05
         */
        FITKSignalTransfer* _signalTransfer{};
        /**
         * @brief 全局事件对象
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-03-19
         */
        FITKGlobalEventFilter* _globalEvent = nullptr;
        /**
        * @brief 命令行处理器
        * @author libaojun (libaojunqd@foxmail.com)
        * @date 2024-04-06
        */
        FITKCommandLineHandler* _commandLineHandler{};
        /**
        * @brief 软件设置
        * @author libaojun (libaojunqd@foxmail.com)
        * @date 2024-04-06
        */
        FITKAppSettings* _settings{};
        /**
        * @brief 可执行程序任务管理器
        * @author libaojun (libaojunqd@foxmail.com)
        * @date 2024-06-09
        */
        FITKProgramTaskManeger* _programTaskManager{};
        /**
        * @brief Python接口注册器
        * @author libaojun (libaojunqd@foxmail.com)
        * @date 2024-06-18
        */
        FITKAbstractPythonRegister* _pyReg{};
        /**
        * @brief 工作空间处理器
        * @author libaojun (libaojunqd@foxmail.com)
        * @date 2024-08-19
        */
        FITKWorkBenchHandler* _workbenchHandler{};
        /**
        * @brief 自动保存器
        * @author libaojun (libaojunqd@foxmail.com)
        * @date 2024-09-12
        */
        FITKAbstractAutoSaver* _autoSaver{};
        /**
        * @brief 初始化器
        * @author libaojun (libaojunqd@foxmail.com)
        * @date 2024-10-23
        */
        FITKAbstractAppInitializer* _initializer{};
        /**
        * @brief 脚本辅助操作器
        * @author libaojun (libaojunqd@foxmail.com)
        * @date 2024-10-23
        */
        FITKAppScriptHelper* _scriptHelper{};
        /**
        * @brief 插件管理器
        * @author libaojun (libaojunqd@foxmail.com)
        * @date 2024-12-04
        */
        FITKPluginsManager* _pluginManager{};
        /**
        * @brief 许可证检查器
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2025-04-02
        */
        FITKAbstractLicenseChecker* _licenseChecker{};
        /**
         * @brief 是否在MPI环境中运行
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-09-11
         */
        bool _runInMPI{ false };
    };
}
/**
 * @brief 定义快速访问宏
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-02-29
 */
#define FITKAPP (dynamic_cast<AppFrame::FITKApplication*>(qApp))

#define FITKGLODATA  (FITKAPP->getGlobalData())

#endif