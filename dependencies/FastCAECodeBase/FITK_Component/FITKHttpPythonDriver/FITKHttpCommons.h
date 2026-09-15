/**********************************************************************
 * @file   FITKHttpCommons.h
 * @brief  FITKHttpPython共有类
 * @author BaGuijun (baguijun@163.com)
 * @date   2026-06-09
 *********************************************************************/
#ifndef _FITKHttpCommons_H_
#define _FITKHttpCommons_H_

#include "FITKHttpPythonDriverInterface.h"

#include <QObject>

 //定义通信协议是否是SSE
#define ProtocolIsSSE true

namespace HttpPython
{
    /**
     * @brief  HTTP请求描述
     * @author BaGuijun (baguijun@163.com)
     * @date   2026-06-03
     */
    struct HttpRequest
    {
        //请求方法
        QString _httpReqMethod = "POST";

        //ip
        QString _httpReqIP = "";

        //Post
        QString _httpReqPort = "";

        //请求路径
        QString _httpReqUri = "/";

        //请求文本
        QString _httpReqMessageText = "";

        //请求内容类型
        QString _httpReqContentType = "application/json; charset=utf-8";

        //请求接受类型
        QString _httpReqAcceptType = "application/json";
    };

    /**
     * @brief  FITKHttpPython共有类
     * @author BaGuijun (baguijun@163.com)
     * @date   2026-06-09
     */
    class FITKHTTPPYTHONDRIVERAPI FITKHttpCommons : public QObject
    {
    public:
        /**
         * @brief    构造函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-06-09
         */
        explicit FITKHttpCommons() = default;
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-06-09
         */
        ~FITKHttpCommons() = default;
        /**
         * @brief    发送 HTTP 请求
         * @param[i] request                    HTTP 请求描述
         * @param[0] responseText               服务端返回文本
         * @param[o] statusCode                 HTTP 状态码
         * @param[0] errorInfo                  失败时错误信息
         * @return   bool                       是否成功
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-06-03
         */
        static bool sendHttpRequest(const HttpRequest& request, QString& responseText, int& statusCode, QString& errorInfo);
        /**
         * @brief    获取 JSON 文本中指定字段的值
         * @param[i] jsonText                   JSON 文本
         * @param[i] type                       字段名
         * @return   QString                    字段值，不存在时返回空字符串
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-06-09
         */
        static QString getJsonTypeValue(const QString& jsonText, const QString& type);
        /**
         * @brief    发送消息到服务器
         * @param[i] message                     消息内容
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-06-09
         */
        static void sendMessageToServer(const QString& message);
    public:
        /**
         * @brief  标记是否是单机模式
         * @author BaGuijun (baguijun@163.com)
         * @date   2026-06-03
         */
        static bool _isStandaloneMode;
        /**
         * @brief  客户端名称
         * @author BaGuijun (baguijun@163.com)
         * @date   2026-06-08
         */
        static QString _clientName;
        /**
         * @brief  IP
         * @author BaGuijun (baguijun@163.com)
         * @date   2026-06-09
         */
        static QString _serverIP;
        /**
         * @brief  端口
         * @author BaGuijun (baguijun@163.com)
         * @date   2026-06-09
         */
        static QString _serverPort;
        /**
         * @brief  当任务id
         * @author BaGuijun (baguijun@163.com)
         * @date   2026-06-15
         */
        static QString _curTaskId;
    };
}

#endif
