#pragma once

#include "ApplicationBoundaries.h"

namespace AppMesh::App
{
class AppInitializer
{
public:
    explicit AppInitializer(bool enabled = false,
                            StageAction initializeAction = {},
                            StageAction shutdownAction = {});
    AppOperationResult initialize();
    AppOperationResult shutdown();
    bool isInitialized() const noexcept;

private:
    OptionalLifecycleStage m_stage;
};
}
