#pragma once

#include "GlobalDataFactory.h"

#include <map>
#include <memory>
#include <vector>

namespace AppMesh::App
{
struct ComponentDefinition
{
    QString key;
    QStringList requiredGlobalData;
    QStringList dependencies;
    ServiceCreator create;
};

class ComponentFactory
{
public:
    LifecycleResult add(ComponentDefinition definition);
    LifecycleResult initialize(const GlobalDataFactory& globalData,
                               IFastCAERegistrationAdapter& adapter);
    void shutdown(IFastCAERegistrationAdapter& adapter) noexcept;

    bool isInitialized() const noexcept;
    ManagedService* instance(const QString& key) const;
    QStringList initializedKeys() const;

private:
    enum class State
    {
        Configuring,
        Initializing,
        Initialized,
        Failed
    };

    void rollback(IFastCAERegistrationAdapter& adapter) noexcept;
    const ComponentDefinition* definition(const QString& key) const;

    State m_state = State::Configuring;
    std::vector<ComponentDefinition> m_definitions;
    std::map<QString, std::unique_ptr<ManagedService>> m_instances;
    QStringList m_initializedKeys;
};
}
