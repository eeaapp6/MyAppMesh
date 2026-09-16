#include "SignalProcessor.h"

namespace AppMesh::App
{
SignalProcessor::SignalProcessor(bool enabled,
                                 StageAction connectAction,
                                 StageAction disconnectAction,
                                 StageAction stopAcceptingAction,
                                 StageAction finishCurrentAction)
    : m_connection(QStringLiteral("Signal processing"),
                   enabled,
                   std::move(connectAction),
                   std::move(disconnectAction)),
      m_enabled(enabled),
      m_stopAcceptingAction(std::move(stopAcceptingAction)),
      m_finishCurrentAction(std::move(finishCurrentAction))
{
}

AppOperationResult SignalProcessor::connectSignals()
{
    return m_connection.start();
}

AppOperationResult SignalProcessor::stopAccepting()
{
    return m_enabled && m_stopAcceptingAction ? m_stopAcceptingAction() : AppOperationResult{};
}

AppOperationResult SignalProcessor::finishCurrent()
{
    return m_enabled && m_finishCurrentAction ? m_finishCurrentAction() : AppOperationResult{};
}

AppOperationResult SignalProcessor::disconnectSignals()
{
    return m_connection.stop();
}

bool SignalProcessor::isConnected() const noexcept
{
    return m_connection.isActive();
}
}
