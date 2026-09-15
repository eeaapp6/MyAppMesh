#include "FITKHttpCommons.h"
#include "FITKHttpPythonDriverInterface.h"
#include "HttpClient.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"

#include <workflow/HttpMessage.h>
#include <workflow/PlatformSocket.h>
#include <workflow/WFFacilities.h>

#include <QJsonDocument>
#include <QJsonObject>

namespace HttpPython
{
    bool FITKHttpCommons::_isStandaloneMode = false;
    QString FITKHttpCommons::_clientName = "";
    QString FITKHttpCommons::_serverIP = "127.0.0.1";
    QString FITKHttpCommons::_serverPort = "8890";
    QString FITKHttpCommons::_curTaskId = "";

    bool FITKHttpCommons::sendHttpRequest(const HttpRequest& request, QString& responseText, int& statusCode, QString& errorInfo)
    {
        if (_isStandaloneMode == true)
        {
            return false;
        }

        responseText.clear();
        errorInfo.clear();
        statusCode = 0;

        QString normalizedMethod = request._httpReqMethod.trimmed().toUpper();
        if (normalizedMethod.isEmpty())
        {
            normalizedMethod = "POST";
        }

        if (normalizedMethod != "GET" && normalizedMethod != "POST")
        {
            errorInfo = tr("Unsupported HTTP method: %1").arg(normalizedMethod);
            return false;
        }

        QString normalizedUri = request._httpReqUri.trimmed();
        if (normalizedUri.isEmpty())
        {
            normalizedUri = "/";
        }
        if (!normalizedUri.startsWith('/'))
        {
            normalizedUri.prepend('/');
        }

        const std::string url = QString("http://%1:%2%3").arg(request._httpReqIP).arg(request._httpReqPort).arg(normalizedUri).toStdString();

        WFFacilities::WaitGroup waitGroup(1);
        bool ok = false;
        QString localResponse;
        QString localError;
        int localStatusCode = 0;

        WFHttpTask* task = WFTaskFactory::create_http_task(url, 2, 3,[&waitGroup, &ok, &localResponse, &localError, &localStatusCode](WFHttpTask* httpTask)
            {
                if (httpTask == nullptr)
                {
                    localError = QObject::tr("HTTP task is null.");
                    waitGroup.done();
                    return;
                }

                if (httpTask->get_state() != WFT_STATE_SUCCESS)
                {
                    localError = QObject::tr("HTTP request failed, state=%1, error=%2").arg(httpTask->get_state()).arg(httpTask->get_error());
                    waitGroup.done();
                    return;
                }

                protocol::HttpResponse* response = httpTask->get_resp();
                if (response == nullptr)
                {
                    localError = QObject::tr("HTTP response is null.");
                    waitGroup.done();
                    return;
                }

                std::string codeText;
                response->get_status_code(codeText);
                localStatusCode = QString::fromStdString(codeText).toInt();

                const void* bodyData = nullptr;
                size_t bodySize = 0;
                if (response->get_parsed_body(&bodyData, &bodySize) && bodyData && bodySize > 0)
                {
                    localResponse = QString::fromUtf8(static_cast<const char*>(bodyData), static_cast<int>(bodySize));
                }

                ok = true;
                waitGroup.done();
            });

        if (task == nullptr)
        {
            errorInfo = tr("Failed to create HTTP task.");
            return false;
        }

        protocol::HttpRequest* httpRequest = task->get_req();
        httpRequest->set_method(normalizedMethod.toUtf8().constData());
        if (normalizedMethod == "POST")
        {
            const QString contentType = request._httpReqContentType.trimmed().isEmpty() ? QString("application/json; charset=utf-8") : request._httpReqContentType.trimmed();
            httpRequest->add_header_pair("Content-Type", contentType.toUtf8().constData());

            if (!request._httpReqAcceptType.trimmed().isEmpty())
            {
                httpRequest->add_header_pair("Accept", request._httpReqAcceptType.trimmed().toUtf8().constData());
            }

            const QByteArray payload = request._httpReqMessageText.toUtf8();
            httpRequest->append_output_body(payload.constData(), static_cast<size_t>(payload.size()));
        }

        task->start();
        waitGroup.wait();

        if (!ok)
        {
            errorInfo = localError;
            return false;
        }

        statusCode = localStatusCode;
        responseText = localResponse;
        return true;
    }

    QString FITKHttpCommons::getJsonTypeValue(const QString& jsonText, const QString& type)
    {
        if (jsonText.trimmed().isEmpty() || type.trimmed().isEmpty())
        {
            return QString();
        }

        QJsonParseError parseError;
        const QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonText.toUtf8(), &parseError);
        if (parseError.error != QJsonParseError::NoError || !jsonDocument.isObject())
        {
            return QString();
        }

        const QJsonObject jsonObject = jsonDocument.object();
        if (!jsonObject.contains(type))
        {
            return QString();
        }

        const QJsonValue jsonValue = jsonObject.value(type);
        if (jsonValue.isString())
        {
            return jsonValue.toString();
        }

        if (jsonValue.isDouble())
        {
            return QString::number(jsonValue.toDouble(), 'g', 16);
        }

        if (jsonValue.isBool())
        {
            return jsonValue.toBool() ? QString("true") : QString("false");
        }

        if (jsonValue.isNull() || jsonValue.isUndefined())
        {
            return QString();
        }

        return QString::fromUtf8(QJsonDocument(jsonValue.toObject()).toJson(QJsonDocument::Compact));
    }

    void FITKHttpCommons::sendMessageToServer(const QString& message)
    {
        FITKHttpPythonDriverInterface* httpPythonDriver = FITKAPP->getComponents()->getComponentTByName<FITKHttpPythonDriverInterface>("FITKHttpPythonDriver");
        if (httpPythonDriver == nullptr)
        {
            return;
        }
        HttpClient* client = httpPythonDriver->getHttpClient();
        if (client == nullptr)
        {
            return;
        }
        client->sendMessage(message);
    }
}