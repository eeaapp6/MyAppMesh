/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file PythonAgent.h
 * @brief PythonQt代理类 
 * @author BaGuijun (baguijun@163.com)
 * @date 2024-02-26
 * 
 */
#ifndef _PYTHONAGENT_H_
#define _PYTHONAGENT_H_

#include <PythonQtObjectPtr.h>
#include <QStringList>
#include <QVariantList>
#include <QMutex>

class QObject;
class QWidget;
class PythonQtScriptingConsole;

namespace Python
{
    class FITKPythonInterface;
    /**
     * @brief PythonQt代理类
     * @author BaGuijun (baguijun@163.com)
     * @date 2024-02-26
     */
    class PythonAgent
    {
    public:
        /**
         * @brief Construct a new Python Agent object
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-02-26
         */
        PythonAgent( FITKPythonInterface* interface);
        /**
         * @brief Destroy the Python Agent object
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-02-26
         */
        ~PythonAgent();
        /**
         * @brief 向PythonQt中添加装饰器
         * @param[i]  obj            装饰器对象
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-02-26
         */
        void addPyQtDecorator(QObject* obj);
        /**
         * @brief 向PythonQt中注册c++类
         * @param[i]  typeName       注册类名称
         * @param[i]  parentTypeName 父类名称
         * @param[i]  package        包体名称(包体为空时为private)
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-02-26
         */
        void registerPyQtCPPClass(const char* typeName, const char* parentTypeName = nullptr, const char* package = nullptr);
        /**
         * @brief 执行脚本文件
         * @param[i]  ScriptFile     脚本文件
         * @param[i]  commands       命令参数（仅支持：Bool、Int、Double、String）
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-02-26
         */
        void submitScriptFile(const QString &ScriptFile, QVariantList commands = {});
        /**
         * @brief 执行脚本
         * @param[i]  Script         脚本
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-02-26
         */
        void submitScript(const QString &Script);
        /**
         * @brief    执行脚本（包含返回值）
         * @param[i] script         脚本
         * @param[o] result         返回值
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-11-07
         */
        void submitScript(const QString & script, QVariant & result);
        /**
         * @brief 执行脚本组
         * @param[i]  Scripts        脚本组
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-02-26
         */
        void submitScript(const QStringList &Scripts);
        /**
         * @brief 历史脚本保存
         * @param[i]  filePath       文件路径
         * @return true 
         * @return false 
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-02-27
         */
        bool saveScriptHistory(const QString &filePath);
        /**
         * @brief 获取python可视化界面
         * @return QWidget* 
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-02-27
         */
        QWidget* getPyWidget();
        /**
         * @brief    获取历史命令
         * @return   QStringList                     历史命令
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-02-27
         */
        QStringList getHistory();
        /**
         * @brief 判断注册的类名是否存在
         * @param[i]  typeName      注册类名称
         * @return true             注册类存在
         * @return false            注册类不存在
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-02-26
         */
        bool classIsExist(const char* typeName);

        /**
         * @brief 互斥锁对象
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-10-18
         */
        bool hadError();
        /**
         * @brief 获取最近一次运行错误信息
         * @return QString
         * @author libaojun
         * @date 2026-05-18
         */
        QString getErrorInfo() const;
        /**
         * @brief 清理python环境
         * @return void
         * @author libaojun
         * @date 2025-03-26
         */
        void clearEnvoirment();
    protected:
        /**
         * @brief    脚本参数设置
         * @param[i] ScriptFile    脚本路劲 
         * @param[i] commands      参数列表 
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-11-07
         */
        void commandsSetting(const QString &ScriptFile, const QVariantList& commands);
    protected:
        /**
         * @brief pythonqt接口对象
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-05-06
         */
        Python::FITKPythonInterface* m_pyInterface{};

        /**
         * @brief 互斥锁对象
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-02-27
         */
        static QMutex m_mutex;
        /**
         * @brief 注册的类名称合集
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-02-26
         */
        QStringList m_regClassNames = {};
        /**
         * @brief python控制台
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-02-27
         */
        PythonQtScriptingConsole* m_Console = nullptr;
        /**
         * @brief 最近一次脚本执行错误信息
         * @author libaojun
         * @date 2026-05-18
         */
        QString m_errorInfo{};
    };
}
#endif