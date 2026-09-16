#include "MainWindowGenerator.h"

#include <QLabel>
#include <QWidget>

namespace AppMesh::App
{
namespace
{
std::unique_ptr<QWidget> createDefaultWindow()
{
    auto window = std::unique_ptr<QWidget>(new QWidget);
    window->setWindowTitle(QStringLiteral("APPMesh"));
    window->resize(960, 640);
    auto* label = new QLabel(QStringLiteral("APPMesh startup composition root"), window.get());
    label->setAlignment(Qt::AlignCenter);
    label->setGeometry(window->rect());
    return window;
}
}

MainWindowGenerator::MainWindowGenerator(WindowFactory factory, bool showWindow)
    : m_factory(factory ? std::move(factory) : WindowFactory(createDefaultWindow)),
      m_showWindow(showWindow)
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
    m_window = m_factory();
    if (!m_window)
    {
        result.add({QStringLiteral("application"),
                    QStringLiteral("APP-MAIN-WINDOW-CREATE-FAILED"),
                    QStringLiteral("The minimal main window could not be created."),
                    QStringLiteral("The registered window factory returned no window."),
                    false,
                    QString(),
                    QString()});
        return result;
    }
    if (m_showWindow)
    {
        m_window->show();
    }
    return result;
}

AppOperationResult MainWindowGenerator::destroy()
{
    m_window.reset();
    return {};
}

QWidget* MainWindowGenerator::window() const noexcept
{
    return m_window.get();
}
}
