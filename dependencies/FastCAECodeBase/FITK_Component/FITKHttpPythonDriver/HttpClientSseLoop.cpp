/**
 * @file    HttpClientSseLoop.cpp
 * @brief   SSE 客户端监听循环实现
 * @author  BaGuijun (baguijun@163.com)
 * @date    2026-06-29
 */
#include "HttpClientSseLoop.h"
#include "HttpClient.h"
#include "FITKHttpCommons.h"
#include "FITKHttpPythonExecutor.h"

#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"

#include "workflow/HttpMessage.h"
#include "workflow/WFTask.h"
#include "workflow/WFTaskFactory.h"

#include <cerrno>
#include <QByteArray>
#include <QCoreApplication>
#include <QDebug>
#include <QList>
#include <QMetaObject>
#include <QPointer>
#include <QTimer>

namespace HttpPython
{
    void SseWorkflowResponse::setPayloadHandler(const std::function<void(const QByteArray&)>& handler)
    {
        _payloadHandler = handler;
    }

    int SseWorkflowResponse::append(const void* buf, size_t* size)
    {
        if (buf == nullptr || size == nullptr)
        {
            errno = EINVAL;
            return -1;
        }

        const size_t appendSize = *size;
        if (appendSize > 0)
        {
            _buffer.append(static_cast<const char*>(buf), static_cast<int>(appendSize));
        }

        *size = appendSize;
        if (parseStreamBuffer() == false)
        {
            errno = EBADMSG;
            return -1;
        }

        return _streamFinished == true ? 1 : 0;
    }

    int SseWorkflowResponse::encode(struct iovec vectors[], int max)
    {
        Q_UNUSED(vectors);
        Q_UNUSED(max);
        errno = ENOSYS;
        return -1;
    }

    bool SseWorkflowResponse::parseStreamBuffer()
    {
        if (_headerParsed == false)
        {
            const int headerEndIndex = _buffer.indexOf("\r\n\r\n");
            if (headerEndIndex < 0)
            {
                return true;
            }

            const QByteArray responseHeader = _buffer.left(headerEndIndex + 4);
            _buffer.remove(0, headerEndIndex + 4);
            _headerParsed = true;
            _chunkedMode = responseHeader.toLower().contains("transfer-encoding: chunked");
        }

        if (_chunkedMode == true)
        {
            return parseChunkedPayload();
        }

        return dispatchRawPayload();
    }

    bool SseWorkflowResponse::parseChunkedPayload()
    {
        while (_buffer.isEmpty() == false)
        {
            const int lineEndIndex = _buffer.indexOf("\r\n");
            if (lineEndIndex < 0)
            {
                return true;
            }

            QByteArray chunkSizeText = _buffer.left(lineEndIndex).trimmed();
            const int extensionIndex = chunkSizeText.indexOf(';');
            if (extensionIndex >= 0)
            {
                chunkSizeText = chunkSizeText.left(extensionIndex);
            }

            bool ok = false;
            const qulonglong chunkSize = chunkSizeText.toULongLong(&ok, 16);
            if (ok == false)
            {
                if (looksLikeRawPayload(_buffer) == true)
                {
                    _chunkedMode = false;
                    return dispatchRawPayload();
                }

                return false;
            }

            const int chunkHeaderSize = lineEndIndex + 2;
            const int totalChunkSize = chunkHeaderSize + static_cast<int>(chunkSize) + 2;
            if (_buffer.size() < totalChunkSize)
            {
                return true;
            }

            _buffer.remove(0, chunkHeaderSize);
            const QByteArray payload = _buffer.left(static_cast<int>(chunkSize));
            _buffer.remove(0, static_cast<int>(chunkSize));
            if (_buffer.startsWith("\r\n") == false)
            {
                return false;
            }
            _buffer.remove(0, 2);

            if (chunkSize == 0)
            {
                _streamFinished = true;
                return true;
            }

            dispatchPayload(payload);
        }

        return true;
    }

    bool SseWorkflowResponse::dispatchRawPayload()
    {
        if (_buffer.isEmpty() == true)
        {
            return true;
        }

        const QByteArray payload = _buffer;
        _buffer.clear();
        dispatchPayload(payload);
        return true;
    }

    bool SseWorkflowResponse::looksLikeRawPayload(const QByteArray& payload) const
    {
        const QByteArray trimmedPayload = payload.trimmed();
        return trimmedPayload.startsWith('{')
            || trimmedPayload.startsWith('[')
            || trimmedPayload.startsWith(':')
            || trimmedPayload.startsWith("data:")
            || trimmedPayload.startsWith("event:")
            || trimmedPayload.startsWith("retry:");
    }

    void SseWorkflowResponse::dispatchPayload(const QByteArray& payload)
    {
        if (_payloadHandler)
        {
            _payloadHandler(payload);
        }
    }

    typedef WFNetworkTask<protocol::HttpRequest, SseWorkflowResponse> SseWorkflowTask;

    HttpClientSseLoop::HttpClientSseLoop(HttpClient* client)
        : QObject(nullptr)
    {
        _client = client;
        _reconnectTimer = new QTimer(this);
        _pendingBuffer = new QByteArray();

        if (_reconnectTimer != nullptr)
        {
            _reconnectTimer->setSingleShot(true);
            connect(_reconnectTimer, &QTimer::timeout, this, [this]()
                {
                    start();
                });
        }
    }

    HttpClientSseLoop::~HttpClientSseLoop()
    {
        stop();
        if (_pendingBuffer != nullptr)
        {
            delete _pendingBuffer;
            _pendingBuffer = nullptr;
        }
    }

    bool HttpClientSseLoop::start()
    {
        if (_client == nullptr)
        {
            qWarning() << "HttpClientSseLoop start aborted: client is null.";
            return false;
        }

        if (_client->isRunning() == false)
        {
            qWarning() << "HttpClientSseLoop start aborted: HttpClient is not running.";
            return false;
        }

        if (_client->isStandaloneMode() == true)
        {
            qWarning() << "HttpClientSseLoop start aborted: standalone mode is enabled.";
            return false;
        }

        if (_hasActiveTask == true)
        {
            _isRunning = true;
            return true;
        }

        if (_pendingBuffer != nullptr)
        {
            _pendingBuffer->clear();
        }

        const qulonglong processId = static_cast<qulonglong>(QCoreApplication::applicationPid());
        const QString requestUri = QString("/api/actions/software/launch-events?processId=%1").arg(QString::number(processId));
        const QString requestHost = FITKHttpCommons::_serverIP.trimmed();
        const unsigned short requestPort = static_cast<unsigned short>(FITKHttpCommons::_serverPort.toUShort());
        const QString requestUrl = QString("http://%1:%2%3").arg(requestHost).arg(requestPort).arg(requestUri);

        QPointer<HttpClientSseLoop> loopPointer(this);
        SseWorkflowTask* sseTask = WFNetworkTaskFactory<protocol::HttpRequest, SseWorkflowResponse>::create_client_task(TT_TCP, requestHost.toStdString(), requestPort, 0, [loopPointer](SseWorkflowTask* task)
            {
                if (loopPointer.isNull() == true)
                {
                    return;
                }

                const int taskState = task->get_state();
                const int taskError = task->get_error();
                const int timeoutReason = task->get_timeout_reason();
                QMetaObject::invokeMethod(loopPointer.data(),
                    [loopPointer, taskState, taskError, timeoutReason]()
                    {
                        if (loopPointer.isNull() == true)
                        {
                            return;
                        }

                        loopPointer->handleTaskFinished(taskState, taskError, timeoutReason);
                    },
                    Qt::QueuedConnection);
            });
        if (sseTask == nullptr)
        {
            qWarning() << "HttpClientSseLoop failed to create workflow SSE task:" << requestUrl;
            return false;
        }

        protocol::HttpRequest* request = sseTask->get_req();
        if (request == nullptr)
        {
            qWarning() << "HttpClientSseLoop request is null:" << requestUrl;
            return false;
        }

        request->set_http_version("HTTP/1.1");
        request->set_method("GET");
        request->set_request_uri(requestUri.toUtf8().constData());
        request->add_header_pair("Host", QString("%1:%2").arg(requestHost).arg(requestPort).toUtf8().constData());
        request->add_header_pair("Accept", "text/event-stream");
        request->add_header_pair("Cache-Control", "no-cache");
        request->add_header_pair("Connection", "keep-alive");

        qDebug() << "HttpClientSseLoop start SSE request:" << requestUrl;

        sseTask->set_receive_timeout(30000);
        sseTask->set_keep_alive(60000);
        sseTask->get_resp()->setPayloadHandler([loopPointer](const QByteArray& payload)
            {
                if (loopPointer.isNull() == true)
                {
                    return;
                }

                const QByteArray payloadCopy = payload;
                QMetaObject::invokeMethod(loopPointer.data(),
                    [loopPointer, payloadCopy]()
                    {
                        if (loopPointer.isNull() == true)
                        {
                            return;
                        }

                        loopPointer->handleIncomingPayload(payloadCopy);
                    },
                    Qt::QueuedConnection);
            });

        _isRunning = true;
        _isConnected = false;
        _hasActiveTask = true;

        sseTask->start();

        return true;
    }

    void HttpClientSseLoop::stop()
    {
        _isRunning = false;
        _isConnected = false;

        if (_reconnectTimer != nullptr)
        {
            _reconnectTimer->stop();
        }
        _hasActiveTask = false;

        if (_pendingBuffer != nullptr)
        {
            _pendingBuffer->clear();
        }
        _receivedPayloadBytes = 0;
    }

    bool HttpClientSseLoop::isRunning() const
    {
        return _isRunning;
    }

    void HttpClientSseLoop::scheduleReconnect()
    {
        if (_isRunning == false || _client == nullptr || _client->isStandaloneMode() == true || _reconnectTimer == nullptr || _hasActiveTask == true)
        {
            return;
        }

        if (_reconnectTimer->isActive())
        {
            return;
        }

        _reconnectTimer->start(3000);
    }

    void HttpClientSseLoop::handleReadyRead()
    {
    }

    void HttpClientSseLoop::handleReplyFinished()
    {
        handleTaskFinished(WFT_STATE_SUCCESS, 0, 0);
    }

    void HttpClientSseLoop::handleReplyError()
    {
        handleTaskFinished(WFT_STATE_SYS_ERROR, 0, 0);
    }

    void HttpClientSseLoop::handleIncomingPayload(const QByteArray& payload)
    {
        if (_isRunning == false || _pendingBuffer == nullptr)
        {
            return;
        }

        _isConnected = true;
        _receivedPayloadBytes += payload.size();
        _pendingBuffer->append(payload);
        _pendingBuffer->replace("\r\n", "\n");
        _pendingBuffer->replace("\r", "\n");
        processPayload();
    }

    void HttpClientSseLoop::handleTaskFinished(int state, int error, int timeoutReason)
    {
        qWarning() << "HttpClientSseLoop task finished, state =" << state
            << ", error =" << error
            << ", timeoutReason =" << timeoutReason
            << ", receivedPayloadBytes =" << _receivedPayloadBytes
            << ", isConnected =" << _isConnected;

        _hasActiveTask = false;
        _isConnected = false;
        if (_isRunning == false)
        {
            return;
        }

        scheduleReconnect();
    }

    void HttpClientSseLoop::processPayload()
    {
        if (_pendingBuffer == nullptr)
        {
            return;
        }

        while (_pendingBuffer->isEmpty() == false)
        {
            while (_pendingBuffer->isEmpty() == false
                && ((*_pendingBuffer)[0] == '\n' || (*_pendingBuffer)[0] == '\r'))
            {
                _pendingBuffer->remove(0, 1);
            }

            if (_pendingBuffer->isEmpty() == true)
            {
                return;
            }

            const QByteArray& pendingBuffer = *_pendingBuffer;
            if (pendingBuffer.startsWith("data:")
                || pendingBuffer.startsWith(":")
                || pendingBuffer.startsWith("event:")
                || pendingBuffer.startsWith("retry:"))
            {
                const int separatorIndex = pendingBuffer.indexOf("\n\n");
                if (separatorIndex < 0)
                {
                    return;
                }

                const QByteArray eventBlock = pendingBuffer.left(separatorIndex);
                _pendingBuffer->remove(0, separatorIndex + 2);
                processEventBlock(eventBlock);
                continue;
            }

            if (pendingBuffer.startsWith('{') || pendingBuffer.startsWith('['))
            {
                const int jsonMessageLength = findCompleteJsonMessageLength(pendingBuffer);
                if (jsonMessageLength <= 0)
                {
                    return;
                }

                const QByteArray jsonMessage = pendingBuffer.left(jsonMessageLength);
                _pendingBuffer->remove(0, jsonMessageLength);
                processJsonMessage(QString::fromUtf8(jsonMessage));
                continue;
            }

            const int lineEndIndex = pendingBuffer.indexOf('\n');
            if (lineEndIndex < 0)
            {
                return;
            }

            const QByteArray plainLine = pendingBuffer.left(lineEndIndex).trimmed();
            _pendingBuffer->remove(0, lineEndIndex + 1);
            processEventBlock(plainLine);
        }
    }

    void HttpClientSseLoop::processEventBlock(const QByteArray& eventBlock)
    {
        const QByteArray trimmedBlock = eventBlock.trimmed();
        if (trimmedBlock.isEmpty() == true)
        {
            return;
        }

        if (trimmedBlock.startsWith(':') || trimmedBlock.startsWith("retry:"))
        {
            return;
        }

        if (trimmedBlock.startsWith('{') || trimmedBlock.startsWith('['))
        {
            processJsonMessage(QString::fromUtf8(trimmedBlock));
            return;
        }

        QList<QByteArray> lines = trimmedBlock.split('\n');
        QByteArray dataPayload = "";
        for (int i = 0; i < lines.size(); ++i)
        {
            const QByteArray currentLine = lines[i].trimmed();
            if (currentLine.startsWith("data:"))
            {
                if (dataPayload.isEmpty() == false)
                {
                    dataPayload += "\n";
                }

                dataPayload += currentLine.mid(5).trimmed();
            }
        }

        if (dataPayload.isEmpty() == false)
        {
            processJsonMessage(QString::fromUtf8(dataPayload));
        }
    }

    void HttpClientSseLoop::processJsonMessage(const QString& jsonText)
    {
        const QString trimmedText = jsonText.trimmed();
        if (trimmedText.isEmpty() == true)
        {
            return;
        }

        const qulonglong processId = static_cast<qulonglong>(QCoreApplication::applicationPid());
        const QString responseProcessId = FITKHttpCommons::getJsonTypeValue(trimmedText, "processId");
        if (responseProcessId.isEmpty() == false && responseProcessId != QString::number(processId))
        {
            return;
        }

        QString commandText = FITKHttpCommons::getJsonTypeValue(trimmedText, "command");
        if (commandText.trimmed().isEmpty() == true)
        {
            commandText = trimmedText;
        }

        FITKHttpCommons::_curTaskId = FITKHttpCommons::getJsonTypeValue(trimmedText, "taskId");
        processCommand(commandText);
    }

    void HttpClientSseLoop::processCommand(const QString& commandText)
    {
        if (_client == nullptr || commandText.trimmed().isEmpty() == true)
        {
            return;
        }

        FITKHttpPythonExecutor* executor = _client->getPythonExecutor();
        if (executor == nullptr)
        {
            return;
        }

        QString errorInfo = "";
        const bool result = executor->executeCommand(commandText, errorInfo);
        if (result == false)
        {
            _client->sendMessage("message");
            return;
        }

        AppFrame::FITKMessageInfo(QObject::tr("HttpClient SSE received message: %1").arg(commandText));
    }

    int HttpClientSseLoop::findCompleteJsonMessageLength(const QByteArray& payload) const
    {
        if (payload.isEmpty() == true)
        {
            return 0;
        }

        bool inString = false;
        bool escaped = false;
        int objectDepth = 0;
        int arrayDepth = 0;
        for (int i = 0; i < payload.size(); ++i)
        {
            const char currentChar = payload.at(i);
            if (escaped)
            {
                escaped = false;
                continue;
            }

            if (currentChar == '\\')
            {
                escaped = inString;
                continue;
            }

            if (currentChar == '"')
            {
                inString = !inString;
                continue;
            }

            if (inString)
            {
                continue;
            }

            if (currentChar == '{')
            {
                ++objectDepth;
                continue;
            }

            if (currentChar == '}')
            {
                --objectDepth;
            }
            else if (currentChar == '[')
            {
                ++arrayDepth;
                continue;
            }
            else if (currentChar == ']')
            {
                --arrayDepth;
            }

            if (objectDepth == 0 && arrayDepth == 0)
            {
                return i + 1;
            }
        }

        return 0;
    }
}