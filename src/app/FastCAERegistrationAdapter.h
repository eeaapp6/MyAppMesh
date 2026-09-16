#pragma once

#include "GlobalDataFactory.h"

#include <map>

namespace AppMesh::App
{
// Test-only registration bookkeeping. Production composition uses
// FITKFastCAERegistrationAdapter and the real FITKApplication public API.
class LocalFastCAERegistrationAdapter final : public IFastCAERegistrationAdapter
{
public:
    bool registerGlobalData(const QString& key,
                            ManagedService& service,
                            QString* errorDetail) override;
    void unregisterGlobalData(const QString& key,
                              ManagedService& service) noexcept override;
    bool registerComponent(const QString& key,
                           ManagedService& service,
                           QString* errorDetail) override;
    void unregisterComponent(const QString& key,
                             ManagedService& service) noexcept override;

    bool hasGlobalData(const QString& key) const;
    bool hasComponent(const QString& key) const;

private:
    std::map<QString, ManagedService*> m_globalData;
    std::map<QString, ManagedService*> m_components;
};
}
