/**
 * @file FITKAIPythonAgentLauncher.cpp
 * @brief 实现 Python 桥接进程的启动、通信与输出转发。
 */
#include "FITKAIPythonAgentLauncher.h"
#include "FITKPythonExecutor.h"
#include "FITKAIAssistantWindow.h"
#include "RunTimeMessage.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QProcessEnvironment>
#include <QStringList>

namespace
{
    // 桥接输出中用于触发宿主执行的协议前缀。
    constexpr const char* kHostPyPrefix = "HOSTPY\t";

    QString sanitizeHostPythonCommand(const QString& rawCommand)
    {
        QString sanitized = rawCommand;
        sanitized.replace("\r\n", "\n");
        sanitized.replace('\r', '\n');

        const QStringList lines = sanitized.split('\n', Qt::SkipEmptyParts);
        QStringList kept;
        for (QString line : lines)
        {
            line = line.trimmed();
            if (line.isEmpty()) continue;
            if (line.startsWith("[BRIDGE]")) continue;
            if (line.startsWith("HOSTPY_RESULT\t")) continue;

            const int bridgePos = line.indexOf("[BRIDGE]");
            if (bridgePos >= 0)
            {
                line = line.left(bridgePos).trimmed();
            }
            if (!line.isEmpty())
            {
                kept << line;
            }
        }

        return kept.join("\n");
    }

    bool couldBeHostPyPrefix(const QString& fragment)
    {
        if (fragment.isEmpty()) return false;

        const QString hostPrefix = QString::fromLatin1(kHostPyPrefix);
        if (fragment.size() >= hostPrefix.size())
        {
            return fragment.startsWith(hostPrefix);
        }

        return hostPrefix.startsWith(fragment);
    }

}

namespace AIAssistant
{
    /** @brief 初始化桥接进程与信号槽连接。 */
    FITKAIPythonAgentLauncher::FITKAIPythonAgentLauncher(FITKAIAssistantWindow* parent)
        : QObject(parent), _assistantWindow(parent)
    {
        _workDir = QCoreApplication::applicationDirPath();
        _process = new QProcess(this);
        _pythonExecutor = new FITKPythonExecutor(this);
        _process->setWorkingDirectory(_workDir);
        connect(_process, SIGNAL(readyReadStandardOutput()), this, SLOT(slotReadyReadStandardOutput()));
        connect(_process, SIGNAL(readyReadStandardError()), this, SLOT(slotReadyReadStandardError()));
        connect(_process, SIGNAL(started()), this, SIGNAL(sigStarted()));
        connect(_process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(slotProcessFinished(int, QProcess::ExitStatus)));
    }

    /** @brief 析构时安全停止桥接进程。 */
    FITKAIPythonAgentLauncher::~FITKAIPythonAgentLauncher()
    {
        stop();
    }

    /** @brief 读取并分发标准输出内容。 */
    void FITKAIPythonAgentLauncher::slotReadyReadStandardOutput()
    {
        if (_process == nullptr) return;

        const QString out = QString::fromUtf8(_process->readAllStandardOutput());
        if (out.isEmpty()) return;

        _stdoutBuffer += out;

        while (true)
        {
            const int lineEnd = _stdoutBuffer.indexOf('\n');
            if (lineEnd < 0)
            {
                // 行未结束时仅在“可能是 HOSTPY 前缀”场景保留，否则立即透传，避免流式拖尾。
                if (!couldBeHostPyPrefix(_stdoutBuffer))
                {
                    _allOutput += _stdoutBuffer;
                    emit sigOutput(_stdoutBuffer);
                    _stdoutBuffer.clear();
                }
                break;
            }

            QString rawLine = _stdoutBuffer.left(lineEnd);
            _stdoutBuffer.remove(0, lineEnd + 1);

            if (rawLine.endsWith('\r'))
            {
                rawLine.chop(1);
            }

            // 仅处理整行 HOSTPY 命令，普通输出直接透传给 UI。
            if (rawLine.startsWith(QString::fromLatin1(kHostPyPrefix)))
            {
                handleHostPythonCommand(rawLine.mid(QString::fromLatin1(kHostPyPrefix).size()));
                continue;
            }

            const QString normalLine = rawLine + "\n";
            _allOutput += normalLine;
            emit sigOutput(normalLine);
        }
    }

    /** @brief 读取并分发标准错误内容。 */
    void FITKAIPythonAgentLauncher::slotReadyReadStandardError()
    {
        if (_process == nullptr) return;

        const QString err = QString::fromUtf8(_process->readAllStandardError());
        if (err.isEmpty()) return;

        _allOutput += err;
        emit sigError(err);
    }

    void FITKAIPythonAgentLauncher::slotProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
    {
        if (!_stdoutBuffer.isEmpty())
        {
            // 进程退出时若残留的是完整 HOSTPY 前缀，按命令处理；否则作为普通文本透传。
            if (_stdoutBuffer.startsWith(QString::fromLatin1(kHostPyPrefix)))
            {
                handleHostPythonCommand(_stdoutBuffer.mid(QString::fromLatin1(kHostPyPrefix).size()));
            }
            else
            {
                _allOutput += _stdoutBuffer;
                emit sigOutput(_stdoutBuffer);
            }
            _stdoutBuffer.clear();
        }

        emit sigFinished(exitCode, exitStatus);
    }

    /** @brief 根据当前平台解析 Python 可执行文件路径。 */
    QString FITKAIPythonAgentLauncher::getPythonExecutablePath() const
    {
        const QString appDir = qApp->applicationDirPath();
#ifdef Q_OS_WIN
        const QString pythonExecutable = appDir + QDir::separator() + "python.exe";
#else
        const QString pythonExecutable = appDir + QDir::separator() + "python";
#endif
        return pythonExecutable;
    }

    /** @brief 解析 FastCAEAssistant.py 绝对路径。 */
    QString FITKAIPythonAgentLauncher::getAssistantScriptPath() const
    {
        return QCoreApplication::applicationDirPath() + QDir::separator()
            + "FastCAEAssistantPy" + QDir::separator() + "FastCAEAssistant.py";
    }

    void FITKAIPythonAgentLauncher::handleHostPythonCommand(const QString& commandLine)
    {
        // 清洗桥接噪声后直接执行 HOSTPY 命令。
        const QString scriptToRun = sanitizeHostPythonCommand(commandLine);
        if (scriptToRun.isEmpty())
        {
            emit sigError(tr("[HOSTPY][ERROR] Empty python command after bridge marker filtering."));
            return;
        }

        if (_pythonExecutor == nullptr)
        {
            const QString error = tr("FITKPythonExecutor is not initialized.");
            emit sigError(tr("[HOSTPY][ERROR] %1").arg(error));
            if (_process && _process->state() == QProcess::Running)
            {
                _process->write(QString("HOSTPY_RESULT\tERROR: %1\n").arg(escapeForBridge(error)).toUtf8());
                _process->waitForBytesWritten(1000);
            }
            return;
        }
        // 执行脚本并捕获结果或错误，优先保证桥接链路的稳定反馈。
        RunTimeMessage* msgBuffer = _assistantWindow ? _assistantWindow->getRunTimeMessage() : nullptr;

        if (msgBuffer)
            msgBuffer->clearErrorMessages();
        

        QString result;
        QString error;
        if (!_pythonExecutor->executeScript(scriptToRun, result, error))
        {
            if (msgBuffer)
            {
                QStringList errorMessages = msgBuffer->getErrorMessages();
                if (!errorMessages.isEmpty())
                {
                    error += "\n" + errorMessages.join("\n");
                }
            }

            emit sigError(tr("[HOSTPY][ERROR] %1").arg(error));
            if (_process && _process->state() == QProcess::Running)
            {
                _process->write(QString("HOSTPY_RESULT\tERROR: %1\n").arg(escapeForBridge(error)).toUtf8());
                _process->waitForBytesWritten(1000);
            }

            if(msgBuffer)
                msgBuffer->clearErrorMessages();
                
            return;
        }

        const QString feedback = result.isEmpty()
            ? tr("[HOSTPY] Script executed.")
            : tr("[HOSTPY] %1").arg(result);
        emit sigOutput(feedback + "\n");

        if (_process && _process->state() == QProcess::Running)
        {
            _process->write(QString("HOSTPY_RESULT\t%1\n").arg(escapeForBridge(result)).toUtf8());
            _process->waitForBytesWritten(1000);
        }
    }

    QString FITKAIPythonAgentLauncher::escapeForBridge(const QString& text) const
    {
        QString escaped = text;
        escaped.replace("\\", "\\\\");
        escaped.replace("\r", "");
        escaped.replace("\n", "\\n");
        escaped.replace("\t", "\\t");
        return escaped;
    }

    /** @brief 启动 Python 桥接脚本。 */
    void FITKAIPythonAgentLauncher::start()
    {
        if (_process == nullptr) return;
        if (_process->state() != QProcess::NotRunning) return;

        const QString pythonProgram = getPythonExecutablePath();
        const QString assistantScript = getAssistantScriptPath();

        if (!QFileInfo::exists(pythonProgram))
        {
            emit sigError(tr("[System] Python executable not found at: %1").arg(pythonProgram));
            return;
        }
        if (!QFileInfo::exists(assistantScript))
        {
            emit sigError(tr("[System] Assistant script not found at: %1").arg(assistantScript));
            return;
        }

        _allOutput.clear();

        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        env.insert("PYTHONIOENCODING", "utf-8");
        env.insert("PYTHONUTF8", "1");
        env.insert("PYTHONUNBUFFERED", "1");
        _process->setProcessEnvironment(env);

        _process->start(pythonProgram, QStringList() << "-u" << assistantScript << "--bridge-stdio");

        if (!_process->waitForStarted(3000))
        {
            emit sigError(tr("[System] Failed to start Python process: %1").arg(_process->errorString()));
        }
    }

    /** @brief 向 Python 子进程写入一行命令。 */
    void FITKAIPythonAgentLauncher::writeToProcess(const QString &input)
    {
        if (_process == nullptr || input.isEmpty()) return;

        // HOSTPY\t<python_script> 由宿主 FITKPython 执行器处理，不参与 agent 启动链路。
        if (input.startsWith("HOSTPY\t"))
        {
            if (_pythonExecutor == nullptr)
            {
                emit sigError(tr("[System] FITKPythonExecutor is not initialized."));
                return;
            }

            const QString payload = sanitizeHostPythonCommand(input.mid(QString("HOSTPY\t").size()));
            if (payload.isEmpty())
            {
                emit sigError(tr("[HOSTPY][ERROR] Empty python command after bridge marker filtering."));
                return;
            }

            QString result;
            QString error;
            if (!_pythonExecutor->executeScript(payload, result, error))
            {
                emit sigError(tr("[HOSTPY][ERROR] %1").arg(error));
                return;
            }

            const QString feedback = result.isEmpty()
                ? tr("[HOSTPY] Script executed.")
                : tr("[HOSTPY] %1").arg(result);
            emit sigOutput(feedback + "\n");

            if (_process->state() == QProcess::Running)
            {
                QString escaped = result;
                escaped.replace("\\", "\\\\");
                escaped.replace("\r", "");
                escaped.replace("\n", "\\n");
                _process->write(QString("HOSTPY_RESULT\t%1\n").arg(escaped).toUtf8());
                _process->waitForBytesWritten(1000);
            }
            return;
        }

        if (_process->state() != QProcess::Running)
        {
            emit sigError(tr("[System] Python process is not running."));
            return;
        }

        QByteArray data = input.toUtf8();
        if (!data.endsWith('\n'))
        {
            data.append('\n');
        }

        _process->write(data);
        _process->waitForBytesWritten(1000);
    }

    /** @brief 优雅停止进程，超时后强制结束。 */
    void FITKAIPythonAgentLauncher::stop()
    {
        if (_process == nullptr)
        {
            return;
        }
        if (_process->state() == QProcess::NotRunning)
        {
            return;
        }

        _process->write("STOP\n");
        _process->waitForBytesWritten(1000);
        _process->closeWriteChannel();

        if (!_process->waitForFinished(3000))
        {
            _process->kill();
            _process->waitForFinished();
        }

    }

    /** @brief 返回桥接进程当前运行状态。 */
    bool FITKAIPythonAgentLauncher::isRunning() const
    {
        if (_process == nullptr) return false;
        return _process->state() == QProcess::Running;
    }
}
