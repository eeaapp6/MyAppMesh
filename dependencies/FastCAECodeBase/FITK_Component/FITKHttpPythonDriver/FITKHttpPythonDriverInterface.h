/**
 * @file FITKHttpPythonDriverInterface.h
 * @brief FITKHttpPythonDriver 组件接口声明
 * @author libaojunAI
 * @date 2026-05-26
 */
#ifndef __FITK_HTTP_PYTHON_DRIVER_INTERFACE_H__
#define __FITK_HTTP_PYTHON_DRIVER_INTERFACE_H__

#include "FITK_Component/FITKHttpPythonDriver/FITKHttpPythonDriverAPI.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponentInterface.h"

namespace HttpPython
{
    class HttpClient;
    class HttpClientListenLoop;
    class HttpClientSseLoop;
}

namespace HttpPython
{
    /**
        * @brief   HTTP Python Driver 组件对外接口
        * @author  libaojunAI
        * @date    2026-05-26
     */
    class FITKHTTPPYTHONDRIVERAPI FITKHttpPythonDriverInterface : public AppFrame::FITKComponentInterface
    {
    public:
        /**
         * @brief    客户端名称构造函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-06-08
         */
        explicit FITKHttpPythonDriverInterface();

        /**
         * @brief    析构函数
         * @author   libaojunAI
         * @date     2026-05-26
         */
        virtual ~FITKHttpPythonDriverInterface();

        /**
         * @brief    初始化组件并启动 HTTP 接收端
         * @author   libaojunAI
         * @date     2026-05-26
         */
        virtual void init() override;

        /**
         * @brief    返回组件名称 FITKHttpPythonDriver
         * @return   QString        组件唯一名称
         * @author   libaojunAI
         * @date     2026-05-26
         */
        virtual QString getComponentName() override;

        /**
         * @brief    返回组件窗口
         * @param[i] indexPort      端口索引
         * @return   QWidget*       组件窗口
         * @author   libaojunAI
         * @date     2026-05-26
         */
        virtual QWidget* getWidget(const int indexPort) override; 
        /**
         * @brief    获取HttpClient
         * @return   HttpClient*                HttpClient
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-06-03
         */
        HttpClient* getHttpClient();

    private:
        /** @brief    HTTP 客户端对象 */
        HttpClient* _httpClient{};

        /** @brief    HTTP 轮询监听对象 */
        HttpClientListenLoop* _listenLoop{};

        /** @brief    SSE 长连接监听对象 */
        HttpClientSseLoop* _sseLoop{};
    };
}

#endif