#pragma once

#include "ApplicationBoundaries.h"

namespace AppMesh::App
{
class PyRegister
{
public:
    explicit PyRegister(bool enabled = false,
                        StageAction registerAction = {},
                        StageAction unregisterAction = {});
    AppOperationResult registerBindings();
    AppOperationResult unregisterBindings();
    bool isRegistered() const noexcept;

private:
    OptionalLifecycleStage m_stage;
};
}
