#pragma once

#include "ApplicationBoundaries.h"

namespace AppMesh::App
{
class SignalProcessor
{
public:
    SignalProcessor(bool enabled = false,
                    StageAction connectAction = {},
                    StageAction disconnectAction = {},
                    StageAction stopAcceptingAction = {},
                    StageAction finishCurrentAction = {});

    AppOperationResult connectSignals();
    AppOperationResult stopAccepting();
    AppOperationResult finishCurrent();
    AppOperationResult disconnectSignals();
    bool isConnected() const noexcept;

private:
    OptionalLifecycleStage m_connection;
    bool m_enabled = false;
    StageAction m_stopAcceptingAction;
    StageAction m_finishCurrentAction;
};
}
