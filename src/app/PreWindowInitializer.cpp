#include "PreWindowInitializer.h"

#include "SignalProcessor.h"

namespace AppMesh::App
{
PreWindowInitializer::PreWindowInitializer(bool enabled,
                                           StageAction initializeAction,
                                           StageAction shutdownAction)
    : m_stage(QStringLiteral("Pre-window initialization"),
              enabled,
              std::move(initializeAction),
              std::move(shutdownAction))
{
}

AppOperationResult PreWindowInitializer::initialize(SignalProcessor& signals)
{
    AppOperationResult result = signals.connectSignals();
    if (!result.succeeded())
    {
        return result;
    }
    result = m_stage.start();
    if (!result.succeeded())
    {
        const auto disconnectResult = signals.disconnectSignals();
        for (const auto& diagnostic : disconnectResult.diagnostics)
        {
            result.add(diagnostic);
        }
    }
    return result;
}

AppOperationResult PreWindowInitializer::shutdown()
{
    return m_stage.stop();
}

bool PreWindowInitializer::isInitialized() const noexcept
{
    return m_stage.isActive();
}
}
