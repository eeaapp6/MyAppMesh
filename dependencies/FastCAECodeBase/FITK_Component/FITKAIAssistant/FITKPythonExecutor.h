/**
 * @file   FITKPythonExecutor.h
 * @brief  宿主 Python 命令执行器声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date   2026-05-14
 */
#ifndef __FITK_PYTHON_EXECUTOR_H__
#define __FITK_PYTHON_EXECUTOR_H__

#include "FITK_Component/FITKAIAssistant/FITKAIAssistantAPI.h"

#include <QObject>

class QString;

namespace AIAssistant
{
    /**
     * @brief 通过 FITKPythonInterface 执行宿主 Python 命令
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2026-05-14
     */
    class FITKAIASSISTANTAPI FITKPythonExecutor : public QObject
    {
        Q_OBJECT
    public:
        /**
         * @brief 构造函数
         * @param parent 父对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2026-05-14
         */
        explicit FITKPythonExecutor(QObject* parent = nullptr);

        /**
         * @brief 执行一条 Python 命令
         * @param script Python 命令文本
         * @param result 执行结果文本
         * @param error 执行错误文本
         * @return 执行成功返回 true
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2026-05-14
         */
        bool executeScript(const QString& script, QString& result, QString& error);
    };
}

#endif
