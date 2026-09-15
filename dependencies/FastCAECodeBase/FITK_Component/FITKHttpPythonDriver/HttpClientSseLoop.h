/**
 * @brief  SSE客户端监听循环类
 * @author BaGuijun (baguijun@163.com)
 * @date   2026-06-29
 */
#ifndef __FITK_HTTP_PYTHON_DRIVER_HTTP_CLIENT_SSE_LOOP_H__
#define __FITK_HTTP_PYTHON_DRIVER_HTTP_CLIENT_SSE_LOOP_H__

#include "FITKHttpPythonDriverAPI.h"

#include "workflow/ProtocolMessage.h"

#include <functional>
#include <QObject>

class QByteArray;
class QString;
class QTimer;

namespace HttpPython
{
    class HttpClient;

    /**
     * @brief  SSE响应数据解析类
     * @author BaGuijun (baguijun@163.com)
     * @date   2026-06-29
     */
    class SseWorkflowResponse : public protocol::ProtocolMessage
    {
    public:
        /**
         * @brief    设置SSE数据回调函数
         * @param[i] handler       SSE数据回调函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-06-29
         */
        void setPayloadHandler(const std::function<void(const QByteArray&)>& handler);

    protected:
        /**
         * @brief    追加协议数据
         * @param[i] buf           数据缓冲区
         * @param[i] size          数据长度
         * @return   int           追加结果
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-06-29
         */
        virtual int append(const void* buf, size_t* size) override;

        /**
         * @brief    编码协议数据
         * @param[i] vectors       编码缓存
         * @param[i] max           最大编码数量
         * @return   int           编码结果
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-06-29
         */
        virtual int encode(struct iovec vectors[], int max) override;

    private:
        /**
         * @brief    解析SSE流缓存
         * @return   bool          是否解析成功
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-06-29
         */
        bool parseStreamBuffer();

        /**
         * @brief    解析chunked负载
         * @return   bool          是否解析成功
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-06-29
         */
        bool parseChunkedPayload();

        /**
         * @brief    分发原始负载
         * @return   bool          是否分发成功
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-06-29
         */
        bool dispatchRawPayload();

        /**
         * @brief    判断是否为原始SSE负载
         * @param[i] payload       待判断负载
         * @return   bool          是否为原始SSE负载
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-06-29
         */
        bool looksLikeRawPayload(const QByteArray& payload) const;

        /**
         * @brief    分发SSE负载数据
         * @param[i] payload       SSE负载数据
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-06-29
         */
        void dispatchPayload(const QByteArray& payload);

    private:
        /**
         * @brief  SSE响应缓存
         * @author BaGuijun (baguijun@163.com)
         * @date   2026-06-29
         */
        QByteArray _buffer{};

        /**
         * @brief  SSE数据回调函数
         * @author BaGuijun (baguijun@163.com)
         * @date   2026-06-29
         */
        std::function<void(const QByteArray&)> _payloadHandler{};

        /**
         * @brief  响应头解析状态
         * @author BaGuijun (baguijun@163.com)
         * @date   2026-06-29
         */
        bool _headerParsed{ false };

        /**
         * @brief  chunked传输状态
         * @author BaGuijun (baguijun@163.com)
         * @date   2026-06-29
         */
        bool _chunkedMode{ false };

        /**
         * @brief  SSE流结束状态
         * @author BaGuijun (baguijun@163.com)
         * @date   2026-06-29
         */
        bool _streamFinished{ false };
    };

    /**
     * @brief  通过SSE长连接接收服务端推送消息并执行命令
     * @author BaGuijun (baguijun@163.com)
     * @date   2026-06-29
     */
    class FITKHTTPPYTHONDRIVERAPI HttpClientSseLoop : public QObject
    {
        Q_OBJECT
    public:
        /**
         * @brief    构造函数
         * @param[i] client        HTTP 客户端对象
         * @param[i] parent        父对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-06-29
         */
        explicit HttpClientSseLoop(HttpClient* client);

        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-06-29
         */
        virtual ~HttpClientSseLoop();

        /**
         * @brief    启动 SSE 监听
         * @return   bool             是否启动成功
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-06-29
         */
        bool start();

        /**
         * @brief    停止 SSE 监听
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-06-29
         */
        void stop();

        /**
         * @brief    是否处于运行状态
         * @return   bool             是否处于运行状态
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-06-29
         */
        bool isRunning() const;

    private:
        /**
         * @brief    安排重连定时器
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-06-29
         */
        void scheduleReconnect();

        /**
         * @brief    处理 SSE 数据可读事件
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-06-29
         */
        void handleReadyRead();

        /**
         * @brief    处理 SSE 响应结束事件
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-06-29
         */
        void handleReplyFinished();

        /**
         * @brief    处理 SSE 响应错误事件
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-06-29
         */
        void handleReplyError();

        /**
         * @brief    处理 workflow 接收到的 SSE 数据
         * @param[i] payload        SSE数据
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-06-29
         */
        void handleIncomingPayload(const QByteArray& payload);

        /**
         * @brief    处理 workflow SSE 任务结束事件
         * @param[i] state          任务状态
         * @param[i] error          错误码
         * @param[i] timeoutReason  超时原因
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-06-29
         */
        void handleTaskFinished(int state, int error, int timeoutReason);

        /**
         * @brief    处理缓存中的待解析数据
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-06-29
         */
        void processPayload();

        /**
         * @brief    处理单个 SSE 事件块
         * @param[i] eventBlock     事件块内容
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-06-29
         */
        void processEventBlock(const QByteArray& eventBlock);

        /**
         * @brief    处理 JSON 消息
         * @param[i] jsonText       JSON 文本
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-06-29
         */
        void processJsonMessage(const QString& jsonText);

        /**
         * @brief    执行命令文本
         * @param[i] commandText    命令文本
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-06-29
         */
        void processCommand(const QString& commandText);

        /**
         * @brief    查找完整 JSON 消息长度
         * @param[i] payload        待解析内容
         * @return   int            完整 JSON 消息长度
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-06-29
         */
        int findCompleteJsonMessageLength(const QByteArray& payload) const;

    private:
        /**
         * @brief  HTTP客户端对象
         * @author BaGuijun (baguijun@163.com)
         * @date   2026-06-29
         */
        HttpClient* _client{};

        /**
         * @brief  SSE任务运行状态
         * @author BaGuijun (baguijun@163.com)
         * @date   2026-06-29
         */
        bool _hasActiveTask{ false };

        /**
         * @brief  SSE重连定时器
         * @author BaGuijun (baguijun@163.com)
         * @date   2026-06-29
         */
        QTimer* _reconnectTimer{};

        /**
         * @brief  待处理的SSE数据缓存
         * @author BaGuijun (baguijun@163.com)
         * @date   2026-06-29
         */
        QByteArray* _pendingBuffer{};

        /**
         * @brief  SSE监听运行状态
         * @author BaGuijun (baguijun@163.com)
         * @date   2026-06-29
         */
        bool _isRunning{ false };

        /**
         * @brief  SSE长连接建立状态
         * @author BaGuijun (baguijun@163.com)
         * @date   2026-06-29
         */
        bool _isConnected{ false };

        /**
         * @brief  当前SSE任务接收的总字节数
         * @author BaGuijun (baguijun@163.com)
         * @date   2026-07-02
         */
        qint64 _receivedPayloadBytes{ 0 };
    };
}

#endif