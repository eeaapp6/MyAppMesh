#include "ConsoleWidget.h"

#include <QMetaObject>
#include <QPlainTextEdit>
#include <QPointer>
#include <QScrollBar>
#include <QStringList>
#include <QTextDocument>
#include <QThread>
#include <QVBoxLayout>

#include <algorithm>

namespace AppMesh::Gui
{
ConsoleWidget::ConsoleWidget(QWidget* parent)
    : QWidget(parent), m_output(new QPlainTextEdit(this))
{
    setObjectName(QStringLiteral("appmeshConsoleWidget"));
    m_output->setObjectName(QStringLiteral("appmeshConsoleOutput"));
    m_output->setReadOnly(true);
    m_output->setUndoRedoEnabled(false);
    m_output->document()->setMaximumBlockCount(m_maximumLineCount);

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_output);
}

ConsoleWidget::~ConsoleWidget() = default;

void ConsoleWidget::appendMessage(ConsoleMessageLevel level,
                                  const QString& message,
                                  const QString& context,
                                  const QDateTime& timestamp)
{
    if (QThread::currentThread() == thread())
    {
        appendOnGuiThread(level, message, context, timestamp);
        return;
    }

    const QPointer<ConsoleWidget> guard(this);
    QMetaObject::invokeMethod(
        this,
        [guard, level, message, context, timestamp]() {
            if (guard)
            {
                guard->appendOnGuiThread(level, message, context, timestamp);
            }
        },
        Qt::QueuedConnection);
}

void ConsoleWidget::appendNormal(const QString& message, const QString& context)
{
    appendMessage(ConsoleMessageLevel::Normal, message, context);
}

void ConsoleWidget::appendInfo(const QString& message, const QString& context)
{
    appendMessage(ConsoleMessageLevel::Info, message, context);
}

void ConsoleWidget::appendWarning(const QString& message, const QString& context)
{
    appendMessage(ConsoleMessageLevel::Warning, message, context);
}

void ConsoleWidget::appendError(const QString& message, const QString& context)
{
    appendMessage(ConsoleMessageLevel::Error, message, context);
}

void ConsoleWidget::appendDiagnostic(const Common::Diagnostic& diagnostic)
{
    const auto level = diagnostic.recoverable
        ? ConsoleMessageLevel::Warning
        : ConsoleMessageLevel::Error;
    QString message = diagnostic.message;
    if (!diagnostic.detail.trimmed().isEmpty())
    {
        message += QStringLiteral(" - ") + diagnostic.detail;
    }
    appendMessage(level, message, diagnosticContext(diagnostic));
}

void ConsoleWidget::clearMessages()
{
    if (QThread::currentThread() == thread())
    {
        clearOnGuiThread();
        return;
    }
    const QPointer<ConsoleWidget> guard(this);
    QMetaObject::invokeMethod(
        this,
        [guard]() {
            if (guard)
            {
                guard->clearOnGuiThread();
            }
        },
        Qt::QueuedConnection);
}

void ConsoleWidget::setMaximumLineCount(int lines)
{
    const int bounded = std::max(1, lines);
    if (QThread::currentThread() != thread())
    {
        const QPointer<ConsoleWidget> guard(this);
        QMetaObject::invokeMethod(
            this,
            [guard, bounded]() {
                if (guard)
                {
                    guard->setMaximumLineCount(bounded);
                }
            },
            Qt::QueuedConnection);
        return;
    }
    m_maximumLineCount = bounded;
    m_output->document()->setMaximumBlockCount(m_maximumLineCount);
}

int ConsoleWidget::maximumLineCount() const noexcept
{
    return m_maximumLineCount;
}

void ConsoleWidget::setAutoScroll(bool enabled) noexcept
{
    m_autoScroll = enabled;
}

bool ConsoleWidget::autoScroll() const noexcept
{
    return m_autoScroll;
}

QString ConsoleWidget::plainText() const
{
    return m_output->toPlainText();
}

int ConsoleWidget::lineCount() const
{
    return m_output->document()->blockCount();
}

QPlainTextEdit* ConsoleWidget::output() noexcept
{
    return m_output;
}

const QPlainTextEdit* ConsoleWidget::output() const noexcept
{
    return m_output;
}

bool ConsoleWidget::lastMutationOccurredOnGuiThread() const noexcept
{
    return m_lastMutationOnGuiThread;
}

void ConsoleWidget::appendOnGuiThread(ConsoleMessageLevel level,
                                      const QString& message,
                                      const QString& context,
                                      const QDateTime& timestamp)
{
    m_lastMutationOnGuiThread = QThread::currentThread() == thread();
    QString line = QStringLiteral("[%1] [%2]")
                       .arg(timestamp.toString(Qt::ISODateWithMs), levelLabel(level));
    if (!context.trimmed().isEmpty())
    {
        line += QStringLiteral(" [") + context.trimmed() + QStringLiteral("]");
    }
    line += QStringLiteral(" ") + message;
    m_output->appendPlainText(line);
    if (m_autoScroll)
    {
        auto* scrollBar = m_output->verticalScrollBar();
        scrollBar->setValue(scrollBar->maximum());
    }
}

void ConsoleWidget::clearOnGuiThread()
{
    m_lastMutationOnGuiThread = QThread::currentThread() == thread();
    m_output->clear();
}

QString ConsoleWidget::levelLabel(ConsoleMessageLevel level)
{
    switch (level)
    {
    case ConsoleMessageLevel::Normal:
        return QStringLiteral("NORMAL");
    case ConsoleMessageLevel::Info:
        return QStringLiteral("INFO");
    case ConsoleMessageLevel::Warning:
        return QStringLiteral("WARNING");
    case ConsoleMessageLevel::Error:
        return QStringLiteral("ERROR");
    }
    return QStringLiteral("NORMAL");
}

QString ConsoleWidget::diagnosticContext(const Common::Diagnostic& diagnostic)
{
    QStringList parts;
    if (!diagnostic.category.trimmed().isEmpty())
    {
        parts.append(diagnostic.category.trimmed());
    }
    if (!diagnostic.code.trimmed().isEmpty())
    {
        parts.append(diagnostic.code.trimmed());
    }
    if (!diagnostic.stage.trimmed().isEmpty())
    {
        parts.append(diagnostic.stage.trimmed());
    }
    if (!diagnostic.objectId.trimmed().isEmpty())
    {
        parts.append(QStringLiteral("object=") + diagnostic.objectId.trimmed());
    }
    if (!diagnostic.path.trimmed().isEmpty())
    {
        parts.append(QStringLiteral("path=") + diagnostic.path.trimmed());
    }
    return parts.join(QStringLiteral(" | "));
}
}
