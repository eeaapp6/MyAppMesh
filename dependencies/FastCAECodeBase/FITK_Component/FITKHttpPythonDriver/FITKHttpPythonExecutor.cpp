/**
 * @file FITKHttpPythonExecutor.cpp
 * @brief FITKHttpPythonDriver Python 命令执行器实现
 * @author libaojunAI
 * @date 2026-05-26
 */
#include "FITKHttpPythonExecutor.h"
#include "HttpClient.h"
#include "RunTimeMessage.h"
#include "FITK_Kernel/FITKPython/FITKPythonInterface.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include <QRegularExpression>
#include <QVariant>

namespace HttpPython
{
    FITKHttpPythonExecutor::FITKHttpPythonExecutor(HttpClient* httpClient)
        : _httpClient(httpClient)
    {
    }

    bool FITKHttpPythonExecutor::executeCommandFromText(const QString &commandText, QString &errorInfo)
    {
        if(_httpClient == nullptr)
        {
            errorInfo = QObject::tr("HttpClient is null.");
            return false;
        }   
        errorInfo.clear();
        // 当前实现将请求体整体作为一条 Python 语句执行。

        bool ok = executeCommand(commandText, errorInfo);

        if(!ok)
        {
            // 叠加运行时缓存中的错误，便于一次性返回完整上下文。
            RunTimeMessage* runTimeMessage = _httpClient->getRunTimeMessage();
            if (runTimeMessage != nullptr)
            {
                errorInfo.append("\n"+ runTimeMessage->getErrorMessageText());
                runTimeMessage->clearErrorMessages();
            }
        }
        return ok;
    }

    bool FITKHttpPythonExecutor::executeCommand(const QString &command, QString &errorInfo)
    {
        errorInfo.clear();

        QString statement = command.trimmed();
        if (statement.isEmpty())
        {
            // 空请求视为成功，避免无意义报错。
            return true;
        }

        // 兼容赋值语句：支持 "a = b" 转为可求值表达式，同时兼容 "a := b" 写法。
        static const QRegularExpression assignmentPattern(
            "^([A-Za-z_][A-Za-z0-9_]*)\\s*=\\s*(.+)$");
        const QRegularExpressionMatch assignmentMatch = assignmentPattern.match(statement);
        if (assignmentMatch.hasMatch())
        {
            statement = QString("(%1 := %2)")
                .arg(assignmentMatch.captured(1))
                .arg(assignmentMatch.captured(2).trimmed());
        }
        else if (statement.contains(":=")
            && !(statement.startsWith('(') && statement.endsWith(')')))
        {
            statement = QString("(%1)").arg(statement);
        }

        Python::FITKPythonInterface* pyInterface = Python::FITKPythonInterface::getInstance();
        if (pyInterface == nullptr)
        {
            errorInfo = QObject::tr("FITKPythonInterface is null.");
            return false;
        }

        QVariant pyResult;
        if (!pyInterface->submit(statement, pyResult))
        {
            // 优先返回 Python 引擎给出的错误信息。
            errorInfo = pyInterface->getErrorInfo();
            if (errorInfo.isEmpty())
            {
                QString summarized = statement;
                if (summarized.size() > 200)
                {
                    summarized = summarized.left(200) + "...";
                } 
                errorInfo = QObject::tr("Python execution failed: %1").arg(summarized);
            }
            return false;
        }

        return true;
    }
}
