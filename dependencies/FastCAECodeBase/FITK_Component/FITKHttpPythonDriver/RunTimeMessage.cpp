/**
 * @file RunTimeMessage.cpp
 * @brief 运行时消息缓存器实现
 * @author libaojunAI
 * @date 2026-05-26
 */
#include "RunTimeMessage.h"

namespace HttpPython
{
    RunTimeMessage::RunTimeMessage(QObject* parent)
        : QObject(parent)
    {
    }

    QStringList RunTimeMessage::getErrorMessages() const
    {
        // 返回当前缓存的原始错误条目列表。
        return _errorMessages;
    }

    QString RunTimeMessage::getErrorMessageText() const
    {
        // 用换行拼接为单文本，便于直接回传给 HTTP 客户端。
        return _errorMessages.join("\n");
    }

    void RunTimeMessage::clearErrorMessages()
    {
        // 清理历史错误，避免旧错误影响新请求诊断。
        _errorMessages.clear();
    }

    void RunTimeMessage::appendMessage(int type, const QString& message)
    {
        // 仅记录错误级别(type==3)的消息。
        if (type != 3 || message.isEmpty())
        {
            return;
        }

        _errorMessages.append(message);
    }
}