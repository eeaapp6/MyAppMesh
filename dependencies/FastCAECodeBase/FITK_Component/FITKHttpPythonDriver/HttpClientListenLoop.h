/**
 * @file HttpClientListenLoop.h
 * @brief HTTP 客户端监听循环声明
 * @author libaojunAI
 * @date 2026-06-01
 */
#ifndef __FITK_HTTP_PYTHON_DRIVER_HTTP_CLIENT_LISTEN_LOOP_H__
#define __FITK_HTTP_PYTHON_DRIVER_HTTP_CLIENT_LISTEN_LOOP_H__

#include "FITKHttpPythonDriverAPI.h"

#include <QObject>
#include <QString>

class QThread;

namespace HttpPython
{
    class HttpClient;
    /**
     * @brief 使用后台线程轮询客户端消息并在主线程处理响应
     * @author libaojunAI
     * @date 2026-06-01
     */
    class FITKHTTPPYTHONDRIVERAPI HttpClientListenLoop : public QObject
    {
        Q_OBJECT
    public:
        /**
         * @brief 构造函数
         * @param client HTTP 客户端对象
         */
        explicit HttpClientListenLoop(HttpClient* client);

        /**
         * @brief 析构函数
         */
        virtual ~HttpClientListenLoop();

        /**
         * @brief 启动监听循环
         * @return 成功返回 true
         */
        bool start();

        /**
         * @brief 停止监听循环
         */
        void stop();

        /**
         * @brief 是否正在运行
         * @return true 表示运行中
         */
        bool isRunning() const;

    private:
        /**
         * @brief    单词轮询获取服务端消息
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-06-09
         */
        void pollOnce();
        /**
         * @brief 在主线程处理服务端响应消息
         * @param responseText 响应文本
         */
        void processResponseMessage(const QString& responseText);

    private:
        QThread* _workerThread{};
        HttpClient* _client{};
        bool _isRunning{ false };
    };
}

#endif