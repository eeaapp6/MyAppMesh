#include "HttpClient.h"
#include "RunTimeMessage.h"
#include "FITKHttpCommons.h"
#include "FITKHttpPythonExecutor.h"

#include "workflow/HttpMessage.h"
#include "workflow/WFTaskFactory.h"
#include "workflow/WFFacilities.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKRunTimeSetting.h"
#include "FITK_Kernel/FITKAppFramework/FITKSignalTransfer.h"

#include <QApplication>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QSettings>
#include <QStringList>
#include <QWidget>

#include <string>

namespace
{
    QString jsonEscape(const QString& text)
    {
        QString escaped = text;
        escaped.replace("\\", "\\\\");
        escaped.replace("\"", "\\\"");
        escaped.replace("\r", "\\r");
        escaped.replace("\n", "\\n");
        return escaped;
    }
}

namespace HttpPython
{
    HttpClient::HttpClient(QObject* parent)
        : QObject(parent)
    {
        _configFilePath = QDir(QCoreApplication::applicationDirPath()).filePath("FITKHttpPythonDriver.ini");
        _pythonExecutor = new FITKHttpPythonExecutor(this);
        _runTimeMessage = new RunTimeMessage(this);
        AppFrame::FITKSignalTransfer* signalTransfer = FITKAPP->getSignalTransfer();
        if(signalTransfer != nullptr && _runTimeMessage != nullptr)
        {
            connect(signalTransfer, SIGNAL(outputMessageSig(int, QString)), 
                 _runTimeMessage, SLOT(appendMessage(int, QString)));
        }
    }

    HttpClient::~HttpClient()
    {
        if (_pythonExecutor)
        {
            delete _pythonExecutor;
            _pythonExecutor = nullptr;
        }
        if (_runTimeMessage)
        {
            delete _runTimeMessage;
            _runTimeMessage = nullptr;
        }
        this->stop();
    }

    bool HttpClient::start()
    {
        if (_isRunning)
        {
            return true;
        }

        FITKHttpCommons::_isStandaloneMode = false;

        //判断客户端名称是否有效
        if (FITKHttpCommons::_clientName.isEmpty())
        {
            return false;
        }

        const bool hasConfigFile = QFile::exists(_configFilePath);

        if (!this->ensureConfigFile() || !this->loadConfig())
        {
            AppFrame::FITKMessageError(tr("FITKHttpPythonDriver 配置加载失败"));
            return false;
        }

        _isRunning = true;

        if (!hasConfigFile)
        {
            FITKHttpCommons::_isStandaloneMode = true;
            AppFrame::FITKMessageInfo(tr("FITKHttpPythonDriver 未找到配置文件"));
            return true;
        }

        QString responseText;
        int statusCode = 0;
        QString errorInfo;
        if (!sendRegisterMessage(responseText, statusCode, errorInfo))
        {
            FITKHttpCommons::_isStandaloneMode = true;
            AppFrame::FITKMessageWarning(tr("FITKHttpPythonDriver 注册客户端失败: %1").arg(errorInfo));
            return true;
        }

        AppFrame::FITKMessageInfo(tr("FITKHttpPythonDriver 客户端已就绪: %1@%2:%3")
            .arg(FITKHttpCommons::_clientName).arg(FITKHttpCommons::_serverIP).arg(FITKHttpCommons::_serverPort));

        return true;
    }

    void HttpClient::stop()
    {
        _isRunning = false;
    }

    bool HttpClient::isRunning() const
    {
        return _isRunning;
    }

    bool HttpClient::isStandaloneMode() const
    {
        return FITKHttpCommons::_isStandaloneMode;
    }

    QString HttpClient::configFilePath() const
    {
        return _configFilePath;
    }

    RunTimeMessage* HttpClient::getRunTimeMessage() const
    {
        return _runTimeMessage;
    }

    FITKHttpPythonExecutor* HttpClient::getPythonExecutor() const
    {
        return _pythonExecutor;
    }

    bool HttpClient::sendMessage(const QString& message)
    {
        const qulonglong processId = static_cast<qulonglong>(QCoreApplication::applicationPid());
        QString messageTemp = QString("APP : %1, ProcessId : %2, Message : %3").arg(FITKHttpCommons::_clientName).arg(QString::number(processId)).arg(message);

        HttpRequest request;
        request._httpReqMethod = "POST";
        request._httpReqUri = "/api/actions/platform/launch-message";
        request._httpReqAcceptType = "application/json";
        request._httpReqMessageText = QString("{\"target\":\"%1\",\"message\":\"%2\",\"taskId\":\"%3\"}")
            .arg("PythonAssistant")
            .arg(messageTemp)
            .arg(FITKHttpCommons::_curTaskId);
        request._httpReqIP = FITKHttpCommons::_serverIP;
        request._httpReqPort = FITKHttpCommons::_serverPort;

        //任务id置空
        FITKHttpCommons::_curTaskId = "";

        QString responseText = "";
        int statusCode = 0;
        QString errorInfo = "";
        bool result = FITKHttpCommons::sendHttpRequest(request, responseText, statusCode, errorInfo);
        if (result == false)
        {
            return false;
        }

        if (statusCode == 200)
        {
            return true;
        }

        return false;
    }

    bool HttpClient::sendRegisterMessage(QString& responseText, int& statusCode, QString& errorInfo)
    {
        const qulonglong processId = static_cast<qulonglong>(QCoreApplication::applicationPid());

        HttpRequest request;
        request._httpReqMethod = "POST";
        request._httpReqUri = "/api/actions/platform/launch-register";
        request._httpReqAcceptType = "application/json";
        request._httpReqMessageText = QString("{\"actionType\":\"%1\",\"processId\":\"%2\"}").arg(jsonEscape(FITKHttpCommons::_clientName)).arg(QString::number(processId));
        request._httpReqIP = FITKHttpCommons::_serverIP;
        request._httpReqPort = FITKHttpCommons::_serverPort;

        return FITKHttpCommons::sendHttpRequest(request, responseText, statusCode, errorInfo);
    }

    bool HttpClient::ensureConfigFile()
    {
        if (QFile::exists(_configFilePath))
        {
            return true;
        }

        const QString templatePath = QDir(QCoreApplication::applicationDirPath())
            .filePath("FITKHttpPythonDriver.ini.template");

        if (QFile::exists(templatePath) && QFile::copy(templatePath, _configFilePath))
        {
            return true;
        }

        QFile configFile(_configFilePath);
        if (!configFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            return false;
        }

        configFile.write(HttpClient::defaultConfigTemplate());
        configFile.close();
        return true;
    }

    bool HttpClient::loadConfig()
    {
        QSettings settings(_configFilePath, QSettings::IniFormat);
        if (settings.status() != QSettings::NoError)
        {
            return false;
        }

        const QString ipValue = settings.value("client/ip",settings.value("client/host", FITKHttpCommons::_serverIP)).toString().trimmed();
        FITKHttpCommons::_serverIP = ipValue.isEmpty() ? QString("127.0.0.1") : ipValue;

        const uint portValue = settings.value("client/port", FITKHttpCommons::_serverPort).toUInt();
        FITKHttpCommons::_serverPort = (portValue > 0 && portValue <= 65535) ? QString::number(portValue) : "8890";

        return true;
    }

    QByteArray HttpClient::defaultConfigTemplate()
    {
        return QByteArray(
            "; FITKHttpPythonDriver 配置模板\n"
            "; 使用方式：复制为可执行程序目录下的 FITKHttpPythonDriver.ini，并按需调整参数。\n\n"
            "[client]\n"
            //"; 客户端名称\n"
            //"clientName=FITKHttpPythonDriver\n\n"
            "; 服务端地址\n"
            "ip=127.0.0.1\n\n"
            "; 服务端端口\n"
            "port=8890\n\n");
    }

    void HttpClient::sendSoftwareInfo()
    {
        QString responseText;
        int statusCode = 0;
        QString errorInfo;
        QString windowIdText = "0";
        QWidget* targetWindow = FITKAPP->getGlobalData()->getMainWindow();
        AppFrame::FITKRunTimeSetting* runTimeSetting = FITKAPP->getGlobalData()->getRunTimeSetting();
        if (targetWindow == nullptr || runTimeSetting == nullptr)
        {
            return;
        }

        if (targetWindow != nullptr)
        {
            const qulonglong windowId = static_cast<qulonglong>(targetWindow->winId());
            windowIdText = QString::number(windowId);
        }

        const qulonglong processId = static_cast<qulonglong>(QCoreApplication::applicationPid());

        if (FITKHttpCommons::_clientName.isEmpty())
        {
            return;
        }

        HttpRequest request;
        request._httpReqMethod = "POST";
        request._httpReqUri = "/api/actions/platform/launch-appinfo";
        request._httpReqMessageText = QString("{\"actionType\":\"%1\",\"processId\":\"%2\",\"winId\":\"%3\"}").arg(FITKHttpCommons::_clientName).arg(QString::number(processId)).arg(windowIdText);
        request._httpReqIP = FITKHttpCommons::_serverIP;
        request._httpReqPort = FITKHttpCommons::_serverPort;

        if (FITKHttpCommons::sendHttpRequest(request, responseText, statusCode, errorInfo) == false)
        {
            AppFrame::FITKMessageWarning(tr("FITKHttpPythonDriver failed to send application information: %1").arg(errorInfo));
            return;
        }

        if (statusCode != 200)
        {
            AppFrame::FITKMessageWarning(tr("FITKHttpPythonDriver application information upload returned unexpected status code: %1, response: %2").arg(statusCode).arg(responseText));
            return;
        }

        AppFrame::FITKMessageInfo(tr("FITKHttpPythonDriver application information uploaded: processId=%1, winId=%2").arg(QString::number(processId)).arg(windowIdText));
    }
}
