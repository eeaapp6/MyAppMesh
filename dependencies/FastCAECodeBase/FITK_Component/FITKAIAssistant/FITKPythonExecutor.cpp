/**
 * @file   FITKPythonExecutor.cpp
 * @brief  宿主 Python 命令执行器实现
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date   2026-05-14
 */
#include "FITKPythonExecutor.h"

#include "FITK_Kernel/FITKPython/FITKPythonInterface.h"

#include <QVariant>
#include <QStringList>

namespace AIAssistant
{
    FITKPythonExecutor::FITKPythonExecutor(QObject* parent)
        : QObject(parent)
    {
    }

    bool FITKPythonExecutor::executeScript(const QString& script, QString& result, QString& error)
    {
        result.clear();
        error.clear();

        QString normalized = script;
        normalized.replace("\r\n", "\n");
        normalized.replace('\r', '\n');

        const QStringList rawStatements = normalized.split(';');
        QStringList statements;
        statements.reserve(rawStatements.size());
        for (const QString& raw : rawStatements)
        {
            const QString statement = raw.trimmed();
            if (!statement.isEmpty())
            {
                statements.append(statement);
            }
        }

        if (statements.isEmpty())
        {   
            return true;
        }

        Python::FITKPythonInterface* pyInterface = Python::FITKPythonInterface::getInstance();
        if (pyInterface == nullptr)
        {
            error = tr("FITKPythonInterface is null.");
            return false;
        }

        for (int i = 0; i < statements.size(); ++i)
        {
            QString command = statements.at(i).trimmed();
            command.replace("=", ":=");
            if(command.contains(":="))
            {
                // 对于赋值语句，尝试转换为表达式形式以获取结果。
                command = QString("(%1)").arg(command);
            }

            QVariant pyResult;
            if (!pyInterface->submit(command, pyResult))
            {
                error = pyInterface->getErrorInfo();

                // 保证失败时始终回传可见错误，避免上层误判为成功。
                if (error.isEmpty())
                {
                    QString summarized = command;
                    if (summarized.size() > 200)
                    {
                        summarized = summarized.left(200) + "...";
                    }
                    error = tr("Python execution failed at statement %1: %2")
                        .arg(i + 1)
                        .arg(summarized);
                }
                return false;
            }

            if (pyResult.isValid() && !pyResult.isNull())
            {
                result = pyResult.toString();
            }
        }


        // 某些语句型脚本不会产生可读返回值，统一回传可消费的成功文本。
        if (result.isEmpty())
        {
            if (statements.size() == 1)
            {
                QString summarized = statements.first();
                if (summarized.size() > 200)
                {
                    summarized = summarized.left(200) + "...";
                }
                result = tr("OK: Executed %1").arg(summarized);
            }
            else
            {
                result = tr("OK: Executed %1 statements").arg(statements.size());
            }
        }

        return true;
    }
}
