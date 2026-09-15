/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAppFramework.h"
#include "FITKGlobalData.h"
#include "FITKComponents.h"
#include "FITKAbstractMainWinGenerator.h"
#include "FITKAbstractGlobalDataFactory.h"
#include "FITKAbstractAutoSaver.h"
#include "FITKComponentFactory.h"
#include "FITKSignalTransfer.h"
#include "FITKGlobalEventFilter.h"
#include "FITKAppSettings.h"
#include "FITKCommandLineHandler.h"
#include "FITKAbstractPythonRegister.h"
#include "FITKMessage.h"
#include "FITKCopyrightDialog.h"
#include "FITKProgramTaskManager.h"
#include "FITKWorkBenchHandler.h"
#include "FITKAbstractSysChecker.h"
#include "FITKAbstractAppInitializer.h"
#include "FITKAppScriptHelper.h"
#include "FITKPluginManager.h"
#include "FITKAppHistoryFiles.h"
#include "FITK_Kernel/FITKCore/FITKThreadPool.h"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
#include "FITK_Kernel/FITKCore/FITKLibInfo.h"
#include "FITK_Kernel/FITKCore/FITKDirFileTools.h"
#include "FITKAbstractLicenseChecker.h"
#include <QWidget>
#include <QMessageBox>
#include <QClipboard>
#include <QDir>
#include "FITKExceptionFilter.h"

namespace AppFrame
{
    FITKLIBINFOREGISTER(FITKAppFramework, FITKAppFrameworkVersion);

    FITKApplication::FITKApplication(int &argc, char **argv)
        :QApplication(argc, argv)
    {
        //注册异常处理器，通过构造函数注册
        FITKExceptionFilter exW;

        //创建golbalData
        _golbalData = new FITKGlobalData;
        _components = new FITKCmponents;
        _signalTransfer = new FITKSignalTransfer(this);
        _programTaskManager = new FITKProgramTaskManeger;
        _scriptHelper = new FITKAppScriptHelper;
        _pluginManager = new FITKPluginsManager;

        Core::FITKThreadPool::getInstance()->init(_signalTransfer);
        //全局事件捕获添加
        _globalEvent = new FITKGlobalEventFilter();
        this->installEventFilter(_globalEvent);
    }

    FITKApplication::~FITKApplication()
    {
        Core::FITKThreadPool::getInstance()->wait();
        

        if (_programTaskManager) delete _programTaskManager;
        if (_signalTransfer) delete _signalTransfer;
        if (_pluginManager) delete _pluginManager;

        if (_golbalData) delete _golbalData;
        if (_components) delete _components;
        if (_gDataFactory) delete _gDataFactory;
        if (_componentsFactory) delete _componentsFactory;
        if (_mwGenerator) delete _mwGenerator;
        if (_globalEvent)delete _globalEvent;
        if (_commandLineHandler) delete _commandLineHandler;
        if (_workbenchHandler) delete _workbenchHandler;
        if (_settings) delete _settings;
        if (_pyReg) delete _pyReg;
        if (_autoSaver) delete _autoSaver;
        if (_initializer) delete _initializer;
        if (_scriptHelper) delete _scriptHelper;
        if (_licenseChecker) delete _licenseChecker;
    }

    void FITKApplication::checkSystem(FITKAbstractSysChecker* checker)
    {
        if (checker == nullptr) return;
        QStringList error = checker->check();
        _passSystemCheck = error.isEmpty();
        delete checker;
        //弹出对话框
        if(!_passSystemCheck)
            QMessageBox::critical(nullptr, "Error", error.join("\n"));
    }

    int FITKApplication::exec()
    {
        if (_licenseChecker)
        {
            QString errorMessage;
            if (!_licenseChecker->checkLicense(errorMessage))
            {
                QMessageBox::critical(nullptr, "Error", errorMessage);
                return -100;
            }
        }
        if (!_passSystemCheck) return -2;

        //开始事件循环
        if (!init())
            return -1;
        

        //加载插件
        if (_pluginManager)
            _pluginManager->loadFromSetting();

        int exitcode = 0;
        //无界面
        if (_commandLineHandler && _commandLineHandler->isNoGUI())
        {
            exitcode = -1;
        }
        //有界面
        else
        {
 //          this->showCopyrightDlg();

            //启动自动保存
            if (_autoSaver) _autoSaver->start();
            
            if(exitcode >= 0)
                exitcode = QApplication::exec();
        }
        //停止自动保存
        if (_autoSaver) 
        {
            _autoSaver->stop();
            _autoSaver->finalize();
        }
        //插件管理器写出到配置文件
        if (_pluginManager)
        {
            _pluginManager->write2Setting();
        }

        //写出历史文件信息
        if (_golbalData != nullptr && _golbalData->getHistoryFiles() != nullptr)
            _golbalData->getHistoryFiles()->write2Setting();

        //写出配置文件，读取在init函数实现
        if (_settings)
            _settings->write();

        //workbench保存文件
        if (_workbenchHandler && _workbenchHandler->workingInWorkBench())
            _workbenchHandler->execOutput();

        //尝试释放主界面
        if (_golbalData && _golbalData->getMainWindow())
        {
            QWidget* mw = _golbalData->getMainWindow();
            delete mw;
        }
        
        return exitcode;
    }

    void FITKApplication::regMainWindowGenerator(FITKAbstractMainwindowGenerator* generator)
    {
        //注册主界面生成器
        _mwGenerator = generator;
    }

    void FITKApplication::regGlobalDataFactory(FITKAbstractGlobalDataFactory* fac)
    {
        _gDataFactory = fac;
    }

    void FITKApplication::regCommandLineHandler(FITKCommandLineHandler* handler)
    {
        _commandLineHandler = handler;
    }

    AppFrame::FITKCommandLineHandler* FITKApplication::getCommandLineHandler()
    {
        return _commandLineHandler;
    }

    void FITKApplication::regAppSettings(FITKAppSettings* settings)
    {
        _settings = settings;
    }

    FITKAppSettings* FITKApplication::getAppSettings()
    {
        return _settings;
    }

    FITKGlobalData* FITKApplication::getGlobalData()
    {
        return _golbalData;
    }

    void FITKApplication::regComponentsFactory(FITKComponentFactory* fac)
    {
        _componentsFactory = fac;
    }

    void FITKApplication::regPythonRegister(FITKAbstractPythonRegister* pyreg)
    {
        if (pyreg == nullptr) return;
        //已经存在则先删除
        if (_pyReg != nullptr) delete _pyReg;
        _pyReg = pyreg;
    }

    FITKCmponents* FITKApplication::getComponents()
    {
        return _components;
    }

    FITKSignalTransfer* FITKApplication::getSignalTransfer()
    {
        return _signalTransfer;
    }

    FITKProgramTaskManeger* FITKApplication::getProgramTaskManager()
    {
        return _programTaskManager;
    }

    void FITKApplication::addGolbalSignalProcesser(QObject* obj)
    {
        if (_signalTransfer == nullptr) return;
        _signalTransfer->addSignalProcesser(obj);
    }

    void FITKApplication::addEventFilter(FITKAbstractEventFilter* filter)
    {
        if (_globalEvent)
            _globalEvent->addEventFilter(filter);
    }

    void FITKApplication::regAutoSaver(FITKAbstractAutoSaver* saver)
    {
        _autoSaver = saver;
    }

    AppFrame::FITKAbstractAutoSaver* FITKApplication::getAutoSaver()
    {
        return _autoSaver;
    }

    void FITKApplication::regAppInitalizer(FITKAbstractAppInitializer* ini)
    {
        _initializer = ini;
    }


    AppFrame::FITKAbstractAppInitializer* FITKApplication::getAppInitalizer()
    {
        return _initializer;
    }

    bool FITKApplication::init()
    {
        if (!_passSystemCheck) return false;
        //创建命令行处理器
        if(_commandLineHandler == nullptr)
           _commandLineHandler = new FITKCommandLineHandler;
        if (_commandLineHandler->isHelp())
        {
            _commandLineHandler->printHelp();
            return false;
        }

        static bool Inited = false;
        if (Inited) return true;

        //读取配置文件
        if (_settings)
            _settings->read();

        //错误判断
        if (!_golbalData || !_gDataFactory ||!_componentsFactory)
            return false;

        //读取历史文件信息
        if (_golbalData->getHistoryFiles() != nullptr)
            _golbalData->getHistoryFiles()->loadFromSetting();

        //生成全局数据
        _gDataFactory->createData(_golbalData);

        //加载qss
        if (_mwGenerator)
        {
            _mwGenerator->loadQssStyle();
        }

        //加载全部组件
        _componentsFactory->create();

        //无界面
        if (_commandLineHandler && _commandLineHandler->isNoGUI())
        {
            this->execCommand();
            return true;
        }
            
        //有界面显示
        if (!_mwGenerator) return false;

        //执行命令行处理
        this->execCommand();

        //加载qm
        if (_mwGenerator)
        {
            _mwGenerator->loadTranslatorQmFiles();
        }

         //生成主界面
        QWidget* w = _mwGenerator->genMainWindow();
        if (!w) return false;
        //存储主界面
        _golbalData->setMainWindow(w);
    
        _mwGenerator->showWelcomePage();
        if(!_mwGenerator->showPreSettingPage()) return false;
        
        //最大化窗口
        if (_mwGenerator->showMaximize())
            w->showMaximized();
        else
            w->showNormal();

        //注册Python接口
        if (_pyReg) _pyReg->registWapper();

        //程序初始化
        if (_initializer)
        {
            if (!_initializer->init())
                return false;
        }

        Inited = true;
        return true;
    }

    AppFrame::FITKGlobalEventFilter* FITKApplication::getGlobalEventFilter()
    {
        return _globalEvent;
    }

    void FITKApplication::showCopyrightDlg()
    {
        FITKCopyRightDialog* dlg = new FITKCopyRightDialog(_golbalData->getMainWindow());
        dlg->show();
    }

    void FITKApplication::resetDataObjects(const QList<int>& savedIDs)
    {
        if (_golbalData == nullptr) return;
        QList<int> ids = _golbalData->getGlobalDataIDs();
        ids.append(savedIDs);
        if (_components != nullptr)
            ids.append(_components->getUnResetDataObj());
        FITKDATAREPO->resetRepo(ids);
    }

    bool FITKApplication::workingInWorkBench()
    {
        return _workbenchHandler == nullptr? false: _workbenchHandler->workingInWorkBench();
    }

    void FITKApplication::regWorkBenchHandler(FITKWorkBenchHandler* handler)
    {
        _workbenchHandler = handler;
    }

    AppFrame::FITKWorkBenchHandler* FITKApplication::getWorkBenchHandler()
    {
        return _workbenchHandler;
    }

    FITKAppScriptHelper* FITKApplication::getScriptHelper()
    {
        return _scriptHelper;
    }

    AppFrame::FITKPluginsManager* FITKApplication::getPluginsManager()
    {
        return _pluginManager;
    }

    void FITKApplication::setPluginKey(const QString& k)
    {
        AppFrame::FITKPluginsManager* mgr = this->getPluginsManager();
        if (mgr) mgr->setPluginKey(k);
    }

    void FITKApplication::setClipboardText(const QString & text) const
    {
        //获取系统剪贴板
        QClipboard* c = this->clipboard();
        if (c != nullptr)  c->setText(text);
    }

    QString FITKApplication::getClipboardText() const
    {
        //获取系统剪贴板
        QClipboard* c = this->clipboard();
        if( c != nullptr) return  c->text();
        return "Invalid clipboard";
    }

    QString FITKApplication::getTempDir( const bool & clearDir , const QString &subDir) const
    {
        //获取系统临时目录
        QString tempdir = QApplication::applicationDirPath() + "/../FITK_Temp";
        QDir dir(tempdir);
        QString absDir = dir.absolutePath();
        Core::CreateDir(absDir);
                
        if (subDir.isEmpty())
        {
            //清空临时目录
            if (clearDir)
                Core::ClearDir(absDir);
            return absDir;

        } 
        else
        {
            //创建子目录
            QString subdir = absDir + "/" + subDir;
            if (Core::CreateDir(subdir))
            {
                //清空临时目录
                if (clearDir)
                    Core::ClearDir(subdir);
                return subdir;
            }
        }
        return QString();
    }

    void FITKApplication::setRunInMPI(bool run)
    {
        _runInMPI = run;
    }

    bool FITKApplication::getRunInMPI() const
    {
        return _runInMPI;
    }

    void FITKApplication::execCommand()
    {
        //输出提示
        QString appname = QCoreApplication::applicationName();
        AppFrame::FITKMessageNormal(appname + " initialized !");
        //输出动态库版本
//     #ifdef QT_DEBUG
//         FITKLIBINFO->printAllLibraryInfo();
// 
//      #endif
        //执行命令行命令
        if(_commandLineHandler)
        {
            _commandLineHandler->exec();
        }
        if (_workbenchHandler && _workbenchHandler->workingInWorkBench())
            _workbenchHandler->execHandler();
    }

    void FITKApplication::regLicenseChecker(FITKAbstractLicenseChecker* checker)
    {
        _licenseChecker = checker;
    }

    FITKAbstractLicenseChecker* FITKApplication::getLicenseChecker()
    {
        return _licenseChecker;
    }

}

