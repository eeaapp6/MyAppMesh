/**
 * @file FITKHttpPythonDriverInterface.cpp
 * @brief FITKHttpPythonDriver 组件接口实现
 * @author libaojunAI
 * @date 2026-05-26
 */
#include "FITKHttpPythonDriverInterface.h"
#include "HttpClient.h"
#include "HttpClientListenLoop.h"
#include "HttpClientSseLoop.h"
#include "FITKHttpCommons.h"
#include "FITK_Kernel/FITKCore/FITKLibInfo.h"
#include <QApplication>
#include <QDebug>

FITKLIBINFOREGISTER(FITKHttpPythonDriver, FITKHTTPPYTHONDRIVER_VERSION);

namespace HttpPython
{
    FITKHttpPythonDriverInterface::FITKHttpPythonDriverInterface()
        : _httpClient(new HttpClient)
    {
        const QString appName = qApp ? qApp->applicationName() : QString();
        qDebug() << "Initializing FITKHttpPythonDriverInterface for client in application:" << appName;
        if (appName.isEmpty() == true)
        {
            qWarning() << "Client name is empty. Using default client name.";
            return;
        }
        FITKHttpCommons::_clientName = appName;

        _listenLoop = new HttpClientListenLoop(_httpClient);
        _sseLoop = new HttpClientSseLoop(_httpClient);

        // 组件创建时预置 HttpClient，确保 init() 可直接进入启动流程。
        _httpClient->start();

        //判断是否使用SSE协议，只启动一种接收模式。
        if (ProtocolIsSSE)
        {
            if (_listenLoop != nullptr)
            {
                _listenLoop->stop();
            }

            if (_sseLoop != nullptr)
            {
                _sseLoop->start();
            }
        }
        else
        {
            if (_sseLoop != nullptr)
            {
                _sseLoop->stop();
            }

            if (_listenLoop != nullptr)
            {
                _listenLoop->start();
            }
        }
    }

    FITKHttpPythonDriverInterface::~FITKHttpPythonDriverInterface()
    {
        if (_httpClient)
        {
            _httpClient->stop();
            delete _httpClient;
            _httpClient = nullptr;
        }

        if (_listenLoop)
        {
            _listenLoop->stop();
            delete _listenLoop;
            _listenLoop = nullptr;
        }

        if (_sseLoop)
        {
            _sseLoop->stop();
            delete _sseLoop;
            _sseLoop = nullptr;
        }
    }

    void FITKHttpPythonDriverInterface::init()
    {
        if (_httpClient == nullptr)
        {
            _httpClient = new HttpClient;
        }

        if (_listenLoop == nullptr)
        {
            _listenLoop = new HttpClientListenLoop(_httpClient);
        }

        if (_sseLoop == nullptr)
        {
            _sseLoop = new HttpClientSseLoop(_httpClient);
        }

        // 组件初始化阶段同时启动原有轮询与 SSE 接收服务。
        if (!_httpClient->start())
        {
            qWarning() << "FITKHttpPythonDriver failed to start http client from config"
                       << _httpClient->configFilePath();
        }

        if (ProtocolIsSSE)
        {
            if (_listenLoop != nullptr)
            {
                _listenLoop->stop();
            }

            if (_sseLoop != nullptr)
            {
                _sseLoop->start();
            }
        }
        else
        {
            if (_sseLoop != nullptr)
            {
                _sseLoop->stop();
            }

            if (_listenLoop != nullptr)
            {
                _listenLoop->start();
            }
        }
    }

    QString FITKHttpPythonDriverInterface::getComponentName()
    {
        return "FITKHttpPythonDriver";
    }

    QWidget* FITKHttpPythonDriverInterface::getWidget(const int indexPort)
    {
        Q_UNUSED(indexPort);
        // 当前组件以后台服务形态运行，不提供可视化页面。
        return nullptr;
    }

    HttpClient* FITKHttpPythonDriverInterface::getHttpClient()
    {
        return _httpClient;
    }
}