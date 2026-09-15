/**
 * @file FITKAIAssistantWindow.cpp
 * @brief 实现 AI Assistant 对话窗口与桥接输出展示逻辑。
 */
#include "FITKAIAssistantWindow.h"
#include "FITKAIAssistantInputHistory.h"
#include "FITKAIPythonAgentLauncher.h"
#include "RunTimeMessage.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKSignalTransfer.h"

#include <QColor>
#include <QDateTime>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QProcess>
#include <QStringList>
#include <QTextBlockFormat>
#include <QTextCharFormat>
#include <QTextCursor>
#include <QTextDocument>
#include <QTextDocumentFragment>
#include <QTextEdit>
#include <QVBoxLayout>

namespace
{
    /** @brief 过滤无需展示到界面的调试型 stderr 行。 */
    bool isIgnorableStderrLine(const QString& line)
    {
        const QString trimmed = line.trimmed();
        if (trimmed.isEmpty()) return true;

        if (trimmed.startsWith("Processing request of type ")) return true;
        if (trimmed.startsWith("Received notification of type ")) return true;

        return false;
    }

    /** @brief 规范化输出文本，去除多余空行，仅对 markdown 语法内容加换行。 */
    QString normalizeMarkdownLineBreaks(const QString& text)
    {
        QString normalized = text;
        normalized.replace("\r\n", "\n");
        normalized.replace('\r', '\n');

        // 去除多余空行：最多保留一个空行。
        const QStringList rawLines = normalized.split('\n', Qt::KeepEmptyParts);
        QStringList compactLines;
        int emptyCount = 0;
        for (const QString& line : rawLines)
        {
            if (line.trimmed().isEmpty())
            {
                ++emptyCount;
                if (emptyCount > 1)
                {
                    continue;
                }
            }
            else
            {
                emptyCount = 0;
            }
            compactLines << line;
        }
        normalized = compactLines.join("\n");

        // 只对 markdown 语法内容加 2空格换行，其余直接用\n
        // 这里简单处理：如果包含 markdown 标记（如 **、#、- ），才加 2空格换行
        if (normalized.contains("**") || normalized.contains("#") || normalized.contains("- ")) {
            QStringList lines = normalized.split('\n');
            for (int i = 0; i < lines.size() - 1; ++i) {
                if (!lines[i].trimmed().isEmpty() && !lines[i+1].trimmed().isEmpty())
                    lines[i] += "  ";
            }
            normalized = lines.join("\n");
        }
        return normalized;
    }

    bool isBridgeStatusLine(const QString& line)
    {
        const QString trimmed = line.trimmed();
        if (trimmed == "[BRIDGE] READY") return true;
        if (trimmed == "[BRIDGE] END") return true;
        if (trimmed == "[BRIDGE] CLEARED") return true;
        if (trimmed == "[BRIDGE] BYE") return true;
        return false;
    }

    /** @brief 将可能触发 HTML 解析的字符转义，避免 markdown 渲染被破坏。 */
    QString escapeHtmlSensitiveForMarkdown(const QString& text)
    {
        QString escaped = text;
        escaped.replace("&", "&amp;");
        escaped.replace("<", "&lt;");
        escaped.replace(">", "&gt;");
        return escaped;
    }
}

namespace AIAssistant
{
    /** @brief 构造窗口并完成布局、信号槽和启动器初始化。 */
    FITKAIAssistantWindow::FITKAIAssistantWindow(QWidget* parent)
        : QWidget(parent)
    {
        _output = new QTextEdit(this);
        _output->setReadOnly(true);
        _output->setPlaceholderText(tr("AI output will be displayed here..."));

        _input = new QLineEdit(this);
        _input->setPlaceholderText(tr("Input query... (/clear to clear, /stop to stop)"));

        _submit = new QPushButton(tr("Submit"), this);
        _launcher = new FITKAIPythonAgentLauncher(this);
        _runTimeMessage = new RunTimeMessage(this);
        _inputHistory = new FITKAIAssistantInputHistory(this);
        _inputHistory->bindInput(_input);

        QHBoxLayout* inputLayout = new QHBoxLayout();
        inputLayout->addWidget(_input);
        inputLayout->addWidget(_submit);

        QVBoxLayout* mainLayout = new QVBoxLayout(this);
        mainLayout->setContentsMargins(8, 8, 8, 8);
        mainLayout->setSpacing(6);
        mainLayout->addWidget(_output, 1);
        mainLayout->addLayout(inputLayout);

        connect(_submit, SIGNAL(clicked()), this, SLOT(slotSubmitCommand()));
        connect(_input, SIGNAL(returnPressed()), this, SLOT(slotSubmitCommand()));
        connect(_launcher, SIGNAL(sigOutput(const QString&)), this, SLOT(slotAppendOutput(const QString&)));
        connect(_launcher, SIGNAL(sigError(const QString&)), this, SLOT(slotAppendError(const QString&)));
        connect(_launcher, SIGNAL(sigStarted()), this, SLOT(slotScriptStarted()));
        connect(_launcher, SIGNAL(sigFinished(int, QProcess::ExitStatus)), this, SLOT(slotScriptFinished(int, QProcess::ExitStatus)));

        AppFrame::FITKSignalTransfer* signalTransfer = FITKAPP->getSignalTransfer();
        if(signalTransfer != nullptr && _runTimeMessage != nullptr)
        {
            connect(signalTransfer, SIGNAL(outputMessageSig(int, QString)), 
                 _runTimeMessage, SLOT(appendMessage(int, QString)));
        }



        setCommandRunningState(false);
        _launcher->start();
    
    }

    RunTimeMessage* FITKAIAssistantWindow::getRunTimeMessage() const
    {
        return _runTimeMessage;
    }

    /** @brief 将新增文本并入 markdown 缓冲并刷新输出控件。 */
    void FITKAIAssistantWindow::appendMarkdownText(const QString& text)
    {
        if (_output == nullptr || text.isEmpty()) return;

        // 规范化文本并转义敏感字符
        QString mdText = normalizeMarkdownLineBreaks(text);
        mdText = escapeHtmlSensitiveForMarkdown(mdText);

        // 将 Markdown 转换为带格式的文档片段
        QTextDocument tempDoc;
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
        tempDoc.setMarkdown(mdText);
#else
        tempDoc.setPlainText(mdText);
#endif
        QTextDocumentFragment fragment(&tempDoc);

        // 获取输出控件的文本游标并移动到末尾
        QTextCursor cursor(_output->document());
        cursor.movePosition(QTextCursor::End);

        // 添加分隔块
        QTextBlockFormat separatorFormat;
        separatorFormat.setTopMargin(0);
        separatorFormat.setBottomMargin(0);
        cursor.insertBlock(separatorFormat);

        // 插入带格式的片段，保留 Markdown 样式
        cursor.insertFragment(fragment);

        // 滚动到底部
        _output->setTextCursor(cursor);
        _output->ensureCursorVisible();

        // 更新缓冲区（用于可能的导出功能）
        _markdownBuffer += mdText + "\n";
    }

    /** @brief 追加用户消息文本（分隔线 + 时间戳 + 右对齐浅灰色气泡样式）。 */
    void FITKAIAssistantWindow::appendUserText(const QString& text)
    {
        if (_output == nullptr || text.isEmpty()) return;

        // 转义敏感字符
        QString escapedText = escapeHtmlSensitiveForMarkdown(text);

        // 获取输出控件的文本游标并移动到末尾
        QTextCursor cursor(_output->document());
        cursor.movePosition(QTextCursor::End);

        // 1. 使用 HTML <hr> 插入分隔线，自动铺满窗口宽度
        QTextBlockFormat separatorBlockFormat;
        separatorBlockFormat.setTopMargin(0);
        separatorBlockFormat.setBottomMargin(0);
        cursor.insertBlock(separatorBlockFormat);
        cursor.insertHtml("<hr style=\"border: none; border-top: 1px solid #C8C8C8; margin: 0;\">");

        // 2. 添加居中显示的时间戳
        QTextBlockFormat timestampBlockFormat;
        timestampBlockFormat.setAlignment(Qt::AlignCenter);
        timestampBlockFormat.setTopMargin(0);
        timestampBlockFormat.setBottomMargin(0);
        cursor.insertBlock(timestampBlockFormat);

        QTextCharFormat timestampCharFormat;
        timestampCharFormat.setForeground(QColor(150, 150, 150));
        timestampCharFormat.setFontPointSize(10);
        QDateTime now = QDateTime::currentDateTime();
        cursor.insertText(now.toString("yyyy-MM-dd hh:mm:ss"), timestampCharFormat);

        // 3. 设置块格式：右对齐，带边距
        QTextBlockFormat blockFormat;
        blockFormat.setAlignment(Qt::AlignRight);
        blockFormat.setTopMargin(0);
        blockFormat.setBottomMargin(0);
        blockFormat.setLeftMargin(50);   // 左边距留空，形成气泡效果

        // 设置字符格式：浅灰色背景，黑色文字
        QTextCharFormat charFormat;
        charFormat.setBackground(QColor(240, 240, 240));
        charFormat.setForeground(Qt::black);
        charFormat.setFontFamily(_output->font().family());

        // 插入块和文本
        cursor.insertBlock(blockFormat, charFormat);
        cursor.insertText(escapedText, charFormat);

        // 滚动到底部
        _output->setTextCursor(cursor);
        _output->ensureCursorVisible();
    }

    void FITKAIAssistantWindow::setCommandRunningState(bool running)
    {
        _isCommandRunning = running;
        if (_submit != nullptr)
        {
            _submit->setText(running ? tr("Terminate") : tr("Submit"));
            _submit->setEnabled(true);
        }
    }

    /** @brief 读取输入命令并发送到 Python 桥接进程。 */
    void FITKAIAssistantWindow::slotSubmitCommand()
    {
        if (_input == nullptr || _output == nullptr || _launcher == nullptr) return;

        if (_isCommandRunning)
        {
            if (_launcher->isRunning())
            {
                _launcher->writeToProcess("STOP");
                appendMarkdownText(tr("\n\n_System:_ Termination requested, waiting for command finish.\n"));
                _submit->setEnabled(false);
            }
            else
            {
                setCommandRunningState(false);
            }
            return;
        }

        const QString cmd = _input->text().trimmed();
        if (cmd.isEmpty()) return;

        if (_inputHistory != nullptr)
        {
            _inputHistory->recordSubmission(cmd);
        }

        if (!_launcher->isRunning())
        {
            _launcher->start();
        }

        // 使用用户气泡样式显示输入
        appendUserText(cmd);

        if (cmd.toLower() == "/clear" || cmd.toLower() == "clear()")
        {
            _launcher->writeToProcess("CLEAR");
            _markdownBuffer.clear();
            _output->clear();
        }
        else if (cmd.toLower() == "/stop" || cmd.toLower() == "stop()")
        {
            _launcher->writeToProcess("STOP");
        }
        else
        {
            _launcher->writeToProcess(QString("ASK\t%1").arg(cmd));
            setCommandRunningState(true);
        }
        
        _input->clear();
    }

    /** @brief 处理桥接标准输出并追加展示。 */
    void FITKAIAssistantWindow::slotAppendOutput(const QString& text)
    {
        if (text.isEmpty()) return;

        QString normalized = text;
        normalized.replace("\r\n", "\n");
        normalized.replace('\r', '\n');

        const QStringList lines = normalized.split('\n', Qt::KeepEmptyParts);
        QString visibleText;
        for (int i = 0; i < lines.size(); ++i)
        {
            const QString& line = lines.at(i);
            const QString trimmed = line.trimmed();
            if (trimmed == "[BRIDGE] END" || trimmed == "[BRIDGE] CLEARED" || trimmed == "[BRIDGE] BYE" 
                || trimmed == "[BRIDGE] STOPPED")
            {
                setCommandRunningState(false);
            }

            if (isBridgeStatusLine(line)) continue;

            visibleText += line;
            if (i != lines.size() - 1)
            {
                visibleText += "\n";
            }
        }

        appendMarkdownText(visibleText);
    }

    /** @brief 处理桥接标准错误并追加展示。 */
    void FITKAIAssistantWindow::slotAppendError(const QString& text)
    {
        if (_output == nullptr || text.isEmpty()) return;

        const QStringList lines = text.split('\n', Qt::SkipEmptyParts);
        for (const QString& line : lines)
        {
            if (isIgnorableStderrLine(line)) continue;

            // stderr 后直接输出文本，不使用额外换行或代码块。
            QString safeLine = line;
            appendMarkdownText(tr("\n\n**stderr:** %1").arg(safeLine));
        }
    }

    /** @brief 脚本启动时输出状态信息。 */
    void FITKAIAssistantWindow::slotScriptStarted()
    {
        if (_output == nullptr) return;
        appendMarkdownText(tr("\n\n_System:_ Python script started.\n"));
    }

    /** @brief 脚本结束时输出退出码与退出状态。 */
    void FITKAIAssistantWindow::slotScriptFinished(int exitCode, QProcess::ExitStatus exitStatus)
    {
        if (_output == nullptr) return;
        appendMarkdownText(tr("\n\n_System:_ Python script finished. exitCode=%1, status=%2\n")
            .arg(exitCode)
            .arg(exitStatus == QProcess::NormalExit ? tr("NormalExit") : tr("CrashExit")));
        setCommandRunningState(false);
    }
}
