/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "PythonAgent.h"
#include "FITKPythonInterface.h"
#include "FITKPythonQtScriptingConsole.h"

#include "FITKCore/FITKDirFileTools.h"

#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QMutex>
#include <QMutexLocker>
#include <QWidget>
#include <QVBoxLayout>
#include <QTextStream>
#include <QObject>
#include <PythonQt.h>
#include <PythonQt_QtAll.h>
#include <PythonQtScriptingConsole.h>
#include <PythonQtConversion.h>
#include <QApplication>
#include <QDir>

namespace Python
{
    QMutex PythonAgent::m_mutex;

    PythonAgent::PythonAgent(Python::FITKPythonInterface* interface)
        : m_pyInterface(interface)
    {
        //PythonQt初始化
        PythonQt::init(PythonQt::IgnoreSiteModule | PythonQt::RedirectStdOut);
        PythonQt_QtAll::init();

        //添加Mian模块至可视化界面中
        m_Console = new FITKPythonQtScriptingConsole(nullptr, PythonQt::self()->getMainModule());

        // 捕获Python stderr，保存最近一次错误信息并同步到接口层。
        QObject::connect(PythonQt::self(), &PythonQt::pythonStdErr, [this](const QString& message) {
            const QString trimmed = message.trimmed();
            if (trimmed.isEmpty()) return;
            if (!m_errorInfo.isEmpty()) m_errorInfo += "\n";
            m_errorInfo += trimmed;
            if (m_pyInterface) m_pyInterface->setErrorInfo(m_errorInfo);
        });

        submitScript("import sys");
        //第三方库
        const QString pkge = qApp->applicationDirPath()+"/Lib/site-packages";
        QDir dir(pkge);
        if (!dir.exists()) return;
        submitScript(QString("sys.path.append('%1')").arg(dir.absolutePath()));

        //添加python环境变量
        if(m_pyInterface != nullptr)
        {
            //添加python环境变量
            QString pyPath = m_pyInterface->getFITKPythonPath();
            QDir pyDir(pyPath);
            if (!pyDir.exists()) return;
            submitScript(QString("sys.path.append('%1')").arg(pyPath));
        }
    }

    PythonAgent::~PythonAgent()
    {
        if (m_Console != nullptr){
            delete m_Console;
            m_Console = nullptr;
        }
    }

    void PythonAgent::addPyQtDecorator(QObject * obj)
    {
        if (obj == nullptr)return;
        PythonQt::self()->addDecorators(obj);
    }

    void PythonAgent::registerPyQtCPPClass(const char * typeName, const char * parentTypeName, const char * package)
    {
        //添加互斥锁
        QMutexLocker mutexLoc(&m_mutex);

        if (classIsExist(typeName)){
            qDebug() << "Failed to register " << typeName << " to PythonQt. The typename already exists!";
            return;
        }
        PythonQt::self()->registerCPPClass(typeName, parentTypeName, package);
        m_regClassNames << typeName;
    }

    void PythonAgent::submitScriptFile(const QString &ScriptFile, QVariantList commands)
    {
        //添加互斥锁
        QMutexLocker mutexLoc(&m_mutex);

        m_errorInfo.clear();
        if (m_pyInterface) m_pyInterface->setErrorInfo(QString());
        if (m_Console) m_Console->clearError();
        if (PythonQt::self()) PythonQt::self()->clearError();

        QFile file(ScriptFile);
        if (!file.exists()) {
            m_errorInfo = QString("Script file does not exist: %1").arg(ScriptFile);
            if (m_pyInterface) m_pyInterface->setErrorInfo(m_errorInfo);
            return;
        }
        if (!file.open(QIODevice::ReadOnly)) {
            m_errorInfo = QString("Failed to open script file: %1").arg(ScriptFile);
            if (m_pyInterface) m_pyInterface->setErrorInfo(m_errorInfo);
            return;
        }
        QByteArray data = file.readAll();
        QFileInfo fileInfo(file);
        QString filePath = fileInfo.absolutePath();
        file.close();

        //添加脚本环境(当执行的脚本依赖多个py文件时，需要添加环境可以找到其他py文件)
        PythonQt::self()->getMainModule().evalScript(QString("sys.path.append(\"%1\")").arg(filePath));

        //编译脚本
        PyObject *code;
#ifdef  PY3K
        PyObject* filename = PythonQtConv::QStringToPyObject(ScriptFile);
        code = Py_CompileStringObject(data.data(), filename,Py_file_input, nullptr, -1);
        Py_DECREF(filename);
#else
        code = Py_CompileString(data.data(), QStringToPythonConstCharPointer(ScriptFile), Py_file_input);
#endif
        //创建脚本执行对象
        PythonQtObjectPtr codeObjPtr;
        codeObjPtr.setNewRef(code);

        //脚本参数设置
        commandsSetting(ScriptFile, commands);

        //执行脚本
        PythonQt::self()->getMainModule().evalCode(codeObjPtr);

        if (commands.size() > 0) {
            //清除命令参数，防止参数误用
            QString comStr = "del sys.argv";
            PythonQt::self()->getMainModule().evalScript(comStr);
        }

        //通知控制台执行结束
        m_Console->executeLine(false);
    }

    void PythonAgent::submitScript(const QString &Script)
    {
        //添加互斥锁
        QMutexLocker mutexLoc(&m_mutex);
        m_errorInfo.clear();
        if (m_pyInterface) m_pyInterface->setErrorInfo(QString());
        if (m_Console) m_Console->clearError();
        if (PythonQt::self()) PythonQt::self()->clearError();

        m_Console->insertCompletion(Script);
        m_Console->executeLine(false);
    }

    void PythonAgent::submitScript(const QString & script, QVariant & result)
    {
        //添加互斥锁
        QMutexLocker mutexLoc(&m_mutex);

        m_errorInfo.clear();
        if (m_pyInterface) m_pyInterface->setErrorInfo(QString());
        if (m_Console) m_Console->clearError();
        if (PythonQt::self()) PythonQt::self()->clearError();

        result = PythonQt::self()->getMainModule().evalScript(script, Py_eval_input);
    }

    void PythonAgent::submitScript(const QStringList &Scripts)
    {
        //添加互斥锁
        QMutexLocker mutexLoc(&m_mutex);
        m_errorInfo.clear();
        if (m_pyInterface) m_pyInterface->setErrorInfo(QString());
        if (m_Console) m_Console->clearError();
        if (PythonQt::self()) PythonQt::self()->clearError();

        for (auto scr : Scripts){
            m_Console->insertCompletion(scr);
            m_Console->executeLine(false);
        }
    }

    bool PythonAgent::saveScriptHistory(const QString &filePath)
    {
        //针对目标文件及其目录不存在逻辑处理
        QFile file(filePath);
        if (!file.exists()){
            QFileInfo fileinfo(file);
            QDir dir;
            if (!dir.exists(fileinfo.absolutePath())) Core::CreateDir(fileinfo.absolutePath());
        }

        if (!file.open(QIODevice::WriteOnly))return false;

        //书写历史脚本至目标文件
        QTextStream fstream(&file);
        for (auto script : m_Console->history()){
            fstream << script << endl;
        }
        file.close();
        return true;
    }

    QWidget * PythonAgent::getPyWidget()
    {
        return m_Console;
    }

    QStringList PythonAgent::getHistory()
    {
        return m_Console->history();
    }

    bool PythonAgent::classIsExist(const char * typeName)
    {
        if (m_regClassNames.contains(typeName))return true;
        return false;
    }

    bool PythonAgent::hadError()
    {
        if (m_Console && m_Console->hadError()) return true;
        if (PythonQt::self()) return PythonQt::self()->hadError();
        return false;
    }

    QString PythonAgent::getErrorInfo() const
    {
        return m_errorInfo;
    }

    void PythonAgent::clearEnvoirment()
    {
        //添加互斥锁
        QMutexLocker mutexLoc(&m_mutex);
        //清理python环境
        m_regClassNames.clear();
        m_Console->clear();
        m_Console->setHistory(QStringList());
        m_errorInfo.clear();
        if (m_pyInterface) m_pyInterface->setErrorInfo(QString());
        if (PythonQt::self()) PythonQt::self()->clearError();
    }

    void PythonAgent::commandsSetting(const QString &ScriptFile, const QVariantList& commands)
    {
        //判断是否有命令参数，有命令参数环境中设置命令参数
        if (commands.size() > 0) {
            //因PythonQt自身的sys中无argv参数，需手动添加参数
            QString comStr = QString("sys.argv=[\"%1\",").arg(ScriptFile);
            for (int i = 0; i < commands.size(); i++) {
                QVariant::Type type = commands.at(i).type();
                switch (type) {
                case QVariant::Invalid:continue;
                case QVariant::Bool: {
                    bool value = commands.at(i).toBool();
                    if (value == true)comStr = comStr + "True";
                    else comStr = comStr + "False";
                    break;
                }
                case QVariant::Int: {
                    int value = commands.at(i).toInt();
                    comStr = comStr + QString::number(value);
                    break;
                }
                case QVariant::Double: {
                    double value = commands.at(i).toDouble();
                    comStr = comStr + QString::number(value);
                    break;
                }
                case QVariant::String: {
                    QString value = commands.at(i).toString();
                    comStr = comStr + QString("\"%1\"").arg(value);
                    break;
                }
                default:continue;
                }

                if ((i + 1) == commands.size()) {
                    comStr = comStr + "]";
                }
                else {
                    comStr = comStr + ",";
                }
            }
            //设置参数
            PythonQt::self()->getMainModule().evalScript(comStr);
        }
    }
}