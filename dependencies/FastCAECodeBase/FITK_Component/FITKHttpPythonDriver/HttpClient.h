/**
 * @file HttpClient.h
 * @brief FITKHttpPythonDriver HTTP 客户端声明
 * @author libaojunAI
 * @date 2026-05-26
 */
#ifndef __FITK_HTTP_PYTHON_DRIVER_HTTP_CLIENT_H__
#define __FITK_HTTP_PYTHON_DRIVER_HTTP_CLIENT_H__

#include "FITKHttpPythonDriverAPI.h"
#include "FITKHttpCommons.h"

#include <QObject>
#include <QString>

namespace HttpPython
{
    class RunTimeMessage;
    class FITKHttpPythonExecutor;

    /**
    * @brief 负责加载配置并向服务端发送 HTTP 报文
     * @author libaojunAI
     * @date 2026-05-26
     */
    class FITKHTTPPYTHONDRIVERAPI HttpClient : public QObject
    {
        Q_OBJECT
    public:
        /**
         * @brief 构造函数
         * @param parent 父对象
         * @author libaojunAI
         * @date 2026-05-26
         */
        explicit HttpClient(QObject* parent = nullptr);   

        /**
         * @brief 析构函数
         * @author libaojunAI
         * @date 2026-05-26
         */
        virtual ~HttpClient();

        /**
         * @brief 启动 HTTP 客户端配置
         * @return 是否启动成功
         * @author libaojunAI
         * @date 2026-05-26
         */
        bool start();

        /**
         * @brief 停止 HTTP 客户端
         * @author libaojunAI
         * @date 2026-05-26
         */
        void stop();
        /**
         * @brief    发送信息
         * @param[i] message                    信息
         * @return   bool                       是否成功
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-06-10
         */
        bool sendMessage(const QString& message);
        /**
         * @brief    发送注册客户端消息
         * @param[i] responseText               服务端返回文本
         * @param[i] statusCode                 HTTP 状态码
         * @param[i] errorInfo                  失败时错误信息
         * @return   bool                       是否成功
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-06-03
         */
        bool sendRegisterMessage(QString& responseText, int& statusCode, QString& errorInfo);
        /**
         * @brief    Send software information to the server.
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-06-03
         */
        void sendSoftwareInfo();
        /**
         * @brief 是否已启动
         * @return true 表示已启动
         * @author libaojunAI
         * @date 2026-05-26
         */
        bool isRunning() const;

        /**
         * @brief 是否为单机模式
         * @return true 表示当前按单机模式运行
         * @author libaojunAI
         * @date 2026-06-03
         */
        bool isStandaloneMode() const;

        /**
         * @brief 获取配置文件路径
         * @return 配置文件路径
         * @author libaojunAI
         * @date 2026-05-26
         */
        QString configFilePath() const;
        /**
         * @brief 获取运行时消息对象
         * @return 运行时消息对象指针
         * @author libaojunAI
         * @date 2026-05-26
         */
        RunTimeMessage* getRunTimeMessage() const;

        /**
         * @brief 获取 Python 命令执行器对象
         * @return Python 命令执行器对象指针
         * @author libaojunAI
         * @date 2026-05-26
         */
        FITKHttpPythonExecutor* getPythonExecutor() const;
    private:
        /**
         * @brief 确保配置文件存在
         * @return true 表示可用
         * @author libaojunAI
         * @date 2026-05-26
         */
        bool ensureConfigFile();

        /**
         * @brief 从配置文件加载客户端地址和端口
         * @return true 表示加载成功
         * @author libaojunAI
         * @date 2026-05-26
         */
        bool loadConfig();

        /**
         * @brief 默认配置模板文本
         * @return 模板内容
         * @author libaojunAI
         * @date 2026-05-26
         */
        static QByteArray defaultConfigTemplate();
    private:
        QString _configFilePath{};    
        bool _isRunning{ false };
        FITKHttpPythonExecutor* _pythonExecutor{ nullptr };
        RunTimeMessage* _runTimeMessage{ nullptr };
    };
}

#endif