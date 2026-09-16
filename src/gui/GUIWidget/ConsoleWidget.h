#pragma once

#include "common/AppDiagnostic.h"

#include <QDateTime>
#include <QWidget>

class QPlainTextEdit;
class QThread;

namespace AppMesh::Gui
{
enum class ConsoleMessageLevel
{
    Normal,
    Info,
    Warning,
    Error
};

class ConsoleWidget final : public QWidget
{
public:
    explicit ConsoleWidget(QWidget* parent = nullptr);
    ~ConsoleWidget() override;

    void appendMessage(ConsoleMessageLevel level,
                       const QString& message,
                       const QString& context = QString(),
                       const QDateTime& timestamp = QDateTime::currentDateTime());
    void appendNormal(const QString& message, const QString& context = QString());
    void appendInfo(const QString& message, const QString& context = QString());
    void appendWarning(const QString& message, const QString& context = QString());
    void appendError(const QString& message, const QString& context = QString());
    void appendDiagnostic(const Common::Diagnostic& diagnostic);
    void clearMessages();

    void setMaximumLineCount(int lines);
    int maximumLineCount() const noexcept;
    void setAutoScroll(bool enabled) noexcept;
    bool autoScroll() const noexcept;

    QString plainText() const;
    int lineCount() const;
    QPlainTextEdit* output() noexcept;
    const QPlainTextEdit* output() const noexcept;
    bool lastMutationOccurredOnGuiThread() const noexcept;

private:
    void appendOnGuiThread(ConsoleMessageLevel level,
                           const QString& message,
                           const QString& context,
                           const QDateTime& timestamp);
    void clearOnGuiThread();
    static QString levelLabel(ConsoleMessageLevel level);
    static QString diagnosticContext(const Common::Diagnostic& diagnostic);

    QPlainTextEdit* m_output = nullptr;
    int m_maximumLineCount = 2000;
    bool m_autoScroll = true;
    bool m_lastMutationOnGuiThread = true;
};
}
