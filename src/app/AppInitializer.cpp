#include "AppInitializer.h"

namespace AppMesh::App
{
AppInitializer::AppInitializer(bool enabled,
                               StageAction initializeAction,
                               StageAction shutdownAction)
    : m_stage(QStringLiteral("Application initialization"),
              enabled,
              std::move(initializeAction),
              std::move(shutdownAction))
{
}

AppOperationResult AppInitializer::initialize()
{
    return m_stage.start();
}

AppOperationResult AppInitializer::shutdown()
{
    return m_stage.stop();
}

bool AppInitializer::isInitialized() const noexcept
{
    return m_stage.isActive();
}
}
