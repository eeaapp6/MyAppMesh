#include "MainWindowGenerator.h"

#include "../gui/GUIFrame/MainWindow.h"

#include <QApplication>
#include <QCoreApplication>
#include <QThread>
#include <QWidget>

#include <exception>

namespace AppMesh::App
{
namespace
{
std::unique_ptr<QWidget> createDefaultWindow()
{
    return std::unique_ptr<QWidget>(new GUIFrame::MainWindow);
}

AppDiagnostic windowDiagnostic(const QString& code,
                               const QString& message,
                               const QString& detail)
{
    return {QStringLiteral("application"),
            code,
            message,
            detail,
            false,
            QString(),
            QStringLiteral("main-window.create")};
}
}

MainWindowGenerator::MainWindowGenerator(WindowFactory factory,
                                         bool showWindow,
                                         WindowDestroyedAction destroyedAction)
    : m_factory(factory ? std::move(factory) : WindowFactory(createDefaultWindow)),
      m_showWindow(showWindow),
      m_destroyedAction(std::move(destroyedAction))
{
}

MainWindowGenerator::~MainWindowGenerator() = default;

AppOperationResult MainWindowGenerator::create()
{
    AppOperationResult result;
    if (m_window)
    {
        return result;
    }
    auto* application = qobject_cast<QApplication*>(QCoreApplication::instance());
    if (!application)
    {
        result.add(windowDiagnostic(QStringLiteral("APP-MAIN-WINDOW-APPLICATION-MISSING"),
                                    QStringLiteral("The main window requires QApplication."),
                                    QStringLiteral("Create QApplication before the window stage.")));
        return result;
    }
    if (QThread::currentThread() != application->thread())
    {
        result.add(windowDiagnostic(QStringLiteral("APP-MAIN-WINDOW-THREAD-INVALID"),
                                    QStringLiteral("The main window must be created on the GUI thread."),
                                    QStringLiteral("Dispatch window creation to the QApplication thread.")));
        return result;
    }

    std::unique_ptr<QWidget> candidate;
    try
    {
        candidate = m_factory();
    }
    catch (const std::exception& exception)
    {
        result.add(windowDiagnostic(QStringLiteral("APP-MAIN-WINDOW-CREATE-FAILED"),
                                    QStringLiteral("The main window could not be created."),
                                    QString::fromLocal8Bit(exception.what())));
        return result;
    }
    catch (...)
    {
        result.add(windowDiagnostic(QStringLiteral("APP-MAIN-WINDOW-CREATE-FAILED"),
                                    QStringLiteral("The main window could not be created."),
                                    QStringLiteral("The window factory raised an unknown exception.")));
        return result;
    }

    if (!candidate)
    {
        result.add(windowDiagnostic(QStringLiteral("APP-MAIN-WINDOW-CREATE-FAILED"),
                                    QStringLiteral("The main window could not be created."),
                                    QStringLiteral("The registered window factory returned no window.")));
        return result;
    }
    if (m_showWindow)
    {
        candidate->show();
    }
    m_window = std::move(candidate);
    return result;
}

AppOperationResult MainWindowGenerator::destroy()
{
    m_window.reset();
    return m_destroyedAction ? m_destroyedAction() : AppOperationResult{};
}

QWidget* MainWindowGenerator::window() const noexcept
{
    return m_window.get();
}
}
