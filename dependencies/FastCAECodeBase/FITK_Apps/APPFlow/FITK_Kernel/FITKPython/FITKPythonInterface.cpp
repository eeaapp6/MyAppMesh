/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPythonInterface.h"
#include "PythonAgent.h"
#include "FITKAppFramework/FITKAppFramework.h"
#include "FITKAppFramework/FITKAppScriptHelper.h"
#include "FITKCore/FITKDirFileTools.h"
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QTextStream>
#include <QMutex>
#include <QMutexLocker>
#include "FITKCore/FITKLibInfo.h"
 
FITKLIBINFOREGISTER(FITKPython, FITKPythonVersion);

namespace Python
{
    QMutex FITKPythonInterface::m_mutex;
    FITKPythonInterface* FITKPythonInterface::m_interface = nullptr;

    FITKPythonInterface* FITKPythonInterface::getInstance()
    {
        QMutexLocker muLocker(&m_mutex);
        if (m_interface == nullptr)
        {
            m_interface = new FITKPythonInterface();
            if (FITKAPP)
            {
                //注册执行函数
                AppFrame::FITKAppScriptHelper* helper = FITKAPP->getScriptHelper();
                if (helper) helper->regExecScriptFun(ExecScript);
            }
        }
        return m_interface;
    }

    void FITKPythonInterface::addDecorator(QObject * obj)
    {
        //pythonQt中添加装饰器
        if (obj == nullptr)return;
        m_pyAgent->addPyQtDecorator(obj);
    }

    void FITKPythonInterface::registerCPPClass(const char * typeName, const char * parentTypeName, const char * package)
    {
        //注册c++类
        m_pyAgent->registerPyQtCPPClass(typeName, parentTypeName, package);

        //python中导入注册的模块
        m_pyAgent->submitScript(QString("from PythonQt.%1 import %2").arg(package).arg(typeName));
        //        importPython(package);
    }

    bool FITKPythonInterface::submit(const QString &script)
    {
        //单行脚本请求
        setExecutionStatus(true);
        m_pyAgent->submitScript(script);
        return getExecutionStatus();
    }

    bool FITKPythonInterface::submit(const QString & script, QVariant & result)
    {
        //单行脚本请求
        setExecutionStatus(true);
        m_pyAgent->submitScript(script, result);
        return getExecutionStatus();
    }

    void FITKPythonInterface::submit(const QStringList &scripts)
    {
        //多行脚本请求
        m_pyAgent->submitScript(scripts);
    }

    void FITKPythonInterface::submitFile(const QString &filePath, QVariantList commands)
    {
        //脚本文件请求
        m_pyAgent->submitScriptFile(filePath, commands);
    }

    void FITKPythonInterface::claerPythonEnvoirment()
    {   
        //清理python环境
         // 获取当前可执行程序所在路径
        QString exePath = QCoreApplication::applicationDirPath();
        QString tempDirPath = QDir(exePath).filePath("temp");

        // 创建temp文件夹（如果不存在）
        QDir tempDir(tempDirPath);
        if (!tempDir.exists())
            tempDir.mkpath(".");

        // 资源文件路径
        QString resourceFilePath = ":/PythonCode/clear.py";
        // 目标文件路径
        QString targetFilePath = tempDir.filePath("clear.py");

        // 复制资源文件到目标路径
        QFile resourceFile(resourceFilePath);
        if (resourceFile.exists())
        {
            if (!resourceFile.copy(targetFilePath))
            {
                qDebug() << "Failed to copy clear.py to temp folder";
            }
        }
        else
        {
            qDebug() << "Resource file clear.py does not exist";
        }

        if (!QFile::exists(targetFilePath)) return;
        //执行脚本
        m_pyAgent->submitScriptFile(targetFilePath);
        m_pyAgent->clearEnvoirment();
        m_pyAgent->submitScript("import sys");
        
    }

    bool FITKPythonInterface::saveScriptHistory(const QString &filePath)
    {
        return m_pyAgent->saveScriptHistory(filePath);
    }

    void FITKPythonInterface::importPython(const QString package)
    {
        //导入python模块
        m_pyAgent->submitScript(QString("from PythonQt import %1").arg(package));
    }

    QWidget * FITKPythonInterface::getPyWidget()
    {
        return m_pyAgent->getPyWidget();
    }

    QStringList FITKPythonInterface::getHistory()
    {
        return m_pyAgent->getHistory();
    }

    QString FITKPythonInterface::getFITKPythonPath()
    {
        QString exePath = QCoreApplication::applicationDirPath();
        QString tempDirPath = QDir(exePath).filePath("FITKPython");

        if(Core::CreateDir(tempDirPath))
            return tempDirPath;
        return QString();
    }

    FITKPythonInterface::FITKPythonInterface()
    {
#ifdef Q_OS_LINUX
        qputenv("PYTHONHOME", FITKAPP->applicationDirPath().toUtf8() + "/../Python37");

        QStringList pythonPaths;
        pythonPaths << FITKAPP->applicationDirPath() + "/../Python37/bin";
        pythonPaths << FITKAPP->applicationDirPath() + "/../Python37/lib";
        pythonPaths << FITKAPP->applicationDirPath() + "/../Python37/lib/python3.7";
        pythonPaths << FITKAPP->applicationDirPath() + "/../Python37/lib/python3.7/site-packages";
        pythonPaths << FITKAPP->applicationDirPath() + "/../Python37/lib/python3.7/lib-dynload";
        qputenv("PYTHONPATH", pythonPaths.join(":").toUtf8());
#endif
        //PythonQt代理类对象创建
        m_pyAgent = new PythonAgent(this);
    }

    FITKPythonInterface::~FITKPythonInterface()
    {
        if (m_pyAgent != nullptr)
        {
            delete m_pyAgent;
            m_pyAgent = nullptr;
        }
    }

    void FITKPythonInterface::setExecutionStatus(bool status)
    {
        m_executionStatus = status;
    }

    bool FITKPythonInterface::getExecutionStatus()
    {
        return m_executionStatus && !m_pyAgent->hadError();
    }

    void FITKPythonInterface::setErrorInfo(QString errorInfo)
    {
        m_callErrorInfo = errorInfo;
    }

    QString FITKPythonInterface::getErrorInfo()
    {
        return m_callErrorInfo;
    }
}

void ExecScript(const QString& file)
{
    Python::FITKPythonInterface::getInstance()->submitFile(file);
}

