/**
 * @file HttpClientListenLoop.cpp
 * @brief HTTP 客户端监听循环实现
 * @author libaojunAI
 * @date 2026-06-01
 */
#include "HttpClientListenLoop.h"
#include "HttpClient.h"
#include "FITKHttpPythonExecutor.h"

#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "workflow/HttpMessage.h"
#include "workflow/WFTaskFactory.h"
#include "workflow/WFFacilities.h"

#include <atomic>
#include <functional>
#include <string>
#include <QMetaObject>
#include <QThread>
#include <QCoreApplication>
#include <qDebug>

namespace
{
    class HttpClientListenWorkerThread : public QThread
    {
    public:
        explicit HttpClientListenWorkerThread(std::function<void()> pollCallback, QObject* parent = nullptr)
            : QThread(parent)
            , _pollCallback(std::move(pollCallback))
        {
        }

        void requestStop()
        {
            _running = false;
        }

    protected:
        void run() override
        {
            while (_running)
            {
                if (_pollCallback)
                {
                    _pollCallback();
                }

                QThread::msleep(500);
            }
        }

    private:
        std::function<void()> _pollCallback;
        std::atomic_bool _running{ true };
    };
}

namespace HttpPython
{
    HttpClientListenLoop::HttpClientListenLoop(HttpClient* client)
        : QObject(nullptr)
        , _client(client)
    {
    }

    HttpClientListenLoop::~HttpClientListenLoop()
    {
        stop();
    }

    bool HttpClientListenLoop::start()
    {
        if (_isRunning)
        {
            return true;
        }

        if (_client != nullptr && _client->isStandaloneMode())
        {
            return true;
        }

        _isRunning = true;

        if (_workerThread == nullptr)
        {
            _workerThread = new HttpClientListenWorkerThread(
                [this]()
                {
                    pollOnce();
                },
                this);
        }

        if (!_workerThread->isRunning())
        {
            _workerThread->start();
        }

        return true;
    }

    void HttpClientListenLoop::stop()
    {
        _isRunning = false;

        if (_workerThread != nullptr && _workerThread->isRunning())
        {
            auto* loopThread = static_cast<HttpClientListenWorkerThread*>(_workerThread);
            if (loopThread != nullptr)
            {
                loopThread->requestStop();
            }

            _workerThread->wait();
        }

        if (_workerThread != nullptr)
        {
            delete _workerThread;
            _workerThread = nullptr;
        }
    }

    bool HttpClientListenLoop::isRunning() const
    {
        return _isRunning;
    }

    void HttpClientListenLoop::pollOnce()
    {
        if (_isRunning == false || _client == nullptr || _client->isStandaloneMode())
        {
            return;
        }

        const qulonglong processId = static_cast<qulonglong>(QCoreApplication::applicationPid());

        HttpRequest request;
        request._httpReqMethod = "POST";
        request._httpReqUri = "/api/actions/software/launch-commands";
        request._httpReqAcceptType = "application/json";
        request._httpReqMessageText = QString("{\"processId\":\"%1\"}").arg(QString::number(processId));
        request._httpReqIP = FITKHttpCommons::_serverIP;
        request._httpReqPort = FITKHttpCommons::_serverPort;

        QString responseText;
        int statusCode = 0;
        QString errorInfo;
        QString commond = "";
        if (FITKHttpCommons::sendHttpRequest(request, responseText, statusCode, errorInfo) == false)
        {
            return;
        }

        if (statusCode == 200 && responseText.isEmpty() == false)
        {
            const QString responseProcessId = FITKHttpCommons::getJsonTypeValue(responseText, "processId");
            if (responseProcessId != QString::number(processId))
            {
                return;
            }

            commond = FITKHttpCommons::getJsonTypeValue(responseText, "command");
            if (commond.trimmed().isEmpty())
            {
                return;
            }

            FITKHttpCommons::_curTaskId = FITKHttpCommons::getJsonTypeValue(responseText, "taskId");
        }
        else
        {
            return;
        }

        QMetaObject::invokeMethod(this,[this, commond]()
            {
                processResponseMessage(commond);
            },Qt::QueuedConnection);
    }

    void HttpClientListenLoop::processResponseMessage(const QString& responseText)
    {
        if (responseText.trimmed().isEmpty())
        {
            return;
        }

        QString errorInfo;
        bool result = _client->getPythonExecutor()->executeCommand(responseText, errorInfo);
        if (result == false)
        {
            QString message = QString("%1 : Task failed").arg(responseText);
            _client->sendMessage("message");
            return;
        }

        AppFrame::FITKMessageInfo(QObject::tr("HttpClientListenLoop received message: %1").arg(responseText));
    }
}