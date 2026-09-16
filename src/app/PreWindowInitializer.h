#pragma once

#include "ApplicationBoundaries.h"

namespace AppMesh::App
{
class SignalProcessor;

class PreWindowInitializer
{
public:
    explicit PreWindowInitializer(bool enabled = false,
                                  StageAction initializeAction = {},
                                  StageAction shutdownAction = {});
    AppOperationResult initialize(SignalProcessor& signals);
    AppOperationResult shutdown();
    bool isInitialized() const noexcept;

private:
    OptionalLifecycleStage m_stage;
};
}
