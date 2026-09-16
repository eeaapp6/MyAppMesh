#pragma once

#include "AppDiagnostic.h"

#include <functional>
#include <memory>

class QWidget;

namespace AppMesh::App
{
using WindowFactory = std::function<std::unique_ptr<QWidget>()>;

class MainWindowGenerator
{
public:
    explicit MainWindowGenerator(WindowFactory factory = {}, bool showWindow = true);
    ~MainWindowGenerator();

    AppOperationResult create();
    AppOperationResult destroy();
    QWidget* window() const noexcept;

private:
    WindowFactory m_factory;
    bool m_showWindow = true;
    std::unique_ptr<QWidget> m_window;
};
}
