/**
 * @file RunTimeMessage.h
 * @brief 运行时消息缓存器声明
 * @author libaojunAI
 * @date 2026-05-26
 */
#ifndef __FITK_HTTP_PYTHON_DRIVER_RUNTIME_MESSAGE_H__
#define __FITK_HTTP_PYTHON_DRIVER_RUNTIME_MESSAGE_H__

 

#include <QObject>
#include <QStringList>

namespace AIAssistant
{
    /**
     * @brief 缓存运行时错误消息
     * @author libaojunAI
     * @date 2026-05-26
     */
    class  RunTimeMessage : public QObject
    {
        Q_OBJECT
    public:
        /**
         * @brief 构造函数
         * @param parent 父对象
         * @author libaojunAI
         * @date 2026-05-26
         */
        explicit RunTimeMessage(QObject* parent = nullptr);

        /**
         * @brief 获取当前缓存的全部错误消息
         * @return 错误消息列表
         * @author libaojunAI
         * @date 2026-05-26
         */
        QStringList getErrorMessages() const;

        /**
         * @brief 获取拼接后的错误消息文本
         * @return 错误消息文本
         * @author libaojunAI
         * @date 2026-05-26
         */
        QString getErrorMessageText() const;

        /**
         * @brief 清空缓存的错误消息
         * @author libaojunAI
         * @date 2026-05-26
         */
        void clearErrorMessages();

    public slots:
        /**
         * @brief 接收框架输出消息并缓存错误项
         * @param type 消息类型
         * @param message 消息内容
         * @author libaojunAI
         * @date 2026-05-26
         */
        void appendMessage(int type, const QString& message);

    private:
        QStringList _errorMessages{};
    };
}

#endif