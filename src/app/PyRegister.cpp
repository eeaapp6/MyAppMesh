#include "PyRegister.h"

namespace AppMesh::App
{
PyRegister::PyRegister(bool enabled,
                       StageAction registerAction,
                       StageAction unregisterAction)
    : m_stage(QStringLiteral("Python registration"),
              enabled,
              std::move(registerAction),
              std::move(unregisterAction))
{
}

AppOperationResult PyRegister::registerBindings()
{
    return m_stage.start();
}

AppOperationResult PyRegister::unregisterBindings()
{
    return m_stage.stop();
}

bool PyRegister::isRegistered() const noexcept
{
    return m_stage.isActive();
}
}
