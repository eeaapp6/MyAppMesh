/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @brief Python脚本接口类声明
 * @author BaGuijun (baguijun@163.com)
 * @date 2024-02-26
 */
#ifndef _FITKPYTHONINTERFACE_H_
#define _FITKPYTHONINTERFACE_H_

#include "FITKPythonAPI.h"
#include <QString>
#include <QStringList>
#include <QVariantList>

class QObject;
class QWidget;
class QStringList;
class QMutex;
class QVariant;

namespace Python
{
    class PythonAgent;
    /**
     * @brief Python脚本接口声明类，该类为单例类
     * @author BaGuijun (baguijun@163.com)
     * @date 2024-02-26
     */
    class FITKPythonAPI FITKPythonInterface
    {
    public:
        /**
         * @brief 获取单例对象
         * @return FITKPythonInterface* 
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-02-26
         */
        static FITKPythonInterface* getInstance();
        /**
         * @brief 添加PythonQt装饰器
         * @param[i]  obj            装饰器对象
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-02-26
         */
        void addDecorator(QObject* obj);
        /**
         * @brief 向PythonQt中注册c++类
         * @param[i]  typeName       注册类名称
         * @param[i]  parentTypeName 父类名称
         * @param[i]  package        包体名称(包体为空时为private)
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-02-26
         */
        void registerCPPClass(const char* typeName, const char* parentTypeName = nullptr, const char* package = nullptr);
        /**
         * @brief python脚本请求
         * @param[i]  script         python脚本
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-02-27
         */
        bool submit(const QString &script);
        /**
         * @brief    Python脚本请求，并返回结果
         * @param[i] script        脚本
         * @param[o] result        执行结果
         * @return   bool          是否执行成功
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-11-07
         */
        bool submit(const QString &script, QVariant& result);
        /**
         * @brief python脚本请求
         * @param[i]  scripts        Python脚本组
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-02-27
         */
        void submit(const QStringList &scripts);
        /**
         * @brief python脚本文件请求
         * @param[i]  filePath           脚本文件
         * @param[i]  commands           命令参数（仅支持：Bool、Int、Double、String）
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-02-26
         */
        void submitFile(const QString &filePath, QVariantList commands = {});
        /**
         * @brief 清理python环境
         * @return void
         * @author libaojun(libaojunqd@foxmail.com)
         * @date 2025-03-26
         */
        void claerPythonEnvoirment();
        /**
         * @brief 保存历史脚本
         * @param[i]  filePath        文件路径
         * @return true              保存成功
         * @return false             保存失败
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-02-26
         */
        bool saveScriptHistory(const QString &filePath);
        /**
         * @brief 导入python包体，默认全部导入
         * @param[i]  package        python包体
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-03-01
         */
        void importPython(const QString package = "*");
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
         * @brief   设置执行状态
         * @param   status 执行状态
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-10-18
         */
        void setExecutionStatus(bool status);

        /**
         * @brief   获取执行状态
         * @return  true  执行成功
         * @return  false 执行失败
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-10-18
         */
        bool getExecutionStatus();

        /**
         * @brief   设置错误信息
         * @param   errorInfo 错误信息
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-10-18
         */
        void setErrorInfo(QString errorInfo);

        /**
         * @brief   获取错误信息
         * @return  true 错误信息
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-10-18
         */
        QString getErrorInfo();

        /**
         * @brief   获取FITKPython路径
         * @return  QString
         * @author  libaojun (libaojunqd@foxmail.com)
         * @date    2025-05-06
         */
        QString getFITKPythonPath();
    private:
        /**
         * @brief Construct a new FITKPythonInterface object
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-02-26
         */
        FITKPythonInterface();
        /**
         * @brief Destroy the FITKPythonInterface object
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-02-26
         */
        ~FITKPythonInterface();
    private:
        /**
         * @brief 互斥锁对象
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-02-29
         */
        static QMutex m_mutex;

        /**
         * @brief 执行状态
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-10-18
         */
        bool m_executionStatus = false;

        /**
         * @brief py执行错误信息
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-10-18
         */
        QString m_callErrorInfo{};
        /**
         * @brief 单例对象
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-02-26
         */
        static FITKPythonInterface* m_interface;
        /**
         * @brief PythonQt代理类对象
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-02-26
         */
        PythonAgent* m_pyAgent = nullptr;
    };
}
extern "C"
{
    void FITKPythonAPI ExecScript(const QString& file);
}



#endif