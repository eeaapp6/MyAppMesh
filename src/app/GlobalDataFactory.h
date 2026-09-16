#pragma once

#include "AppDiagnostic.h"
#include "../common/ManagedService.h"

#include <QStringList>

#include <functional>
#include <map>
#include <memory>
#include <vector>

namespace AppMesh::App
{
using ManagedService = Common::ManagedService;

// Narrow ABI boundary for the public FITK registration contracts. A concrete
// T004 adapter maps these calls to FITKApplication::regGlobalDataFactory and
// FITKApplication::regComponentsFactory without exposing FITK internals here.
class IFastCAERegistrationAdapter
{
public:
    virtual ~IFastCAERegistrationAdapter() = default;
    virtual AppOperationResult initializeRuntime()
    {
        return {};
    }
    virtual AppOperationResult shutdownRuntime() noexcept
    {
        return {};
    }
    virtual bool isFITKBacked() const noexcept
    {
        return false;
    }
    virtual QString adapterName() const
    {
        return QStringLiteral("test-registration-adapter");
    }
    virtual bool registerGlobalData(const QString& key,
                                    ManagedService& service,
                                    QString* errorDetail) = 0;
    virtual void unregisterGlobalData(const QString& key,
                                      ManagedService& service) noexcept = 0;
    virtual bool registerComponent(const QString& key,
                                   ManagedService& service,
                                   QString* errorDetail) = 0;
    virtual void unregisterComponent(const QString& key,
                                     ManagedService& service) noexcept = 0;
};

using ServiceCreator = std::function<std::unique_ptr<ManagedService>()>;
using LifecycleResult = AppOperationResult;

struct GlobalDataDefinition
{
    QString key;
    ServiceCreator create;
};

class GlobalDataFactory
{
public:
    LifecycleResult add(GlobalDataDefinition definition);
    LifecycleResult initialize(IFastCAERegistrationAdapter& adapter);
    void shutdown(IFastCAERegistrationAdapter& adapter) noexcept;

    bool isInitialized() const noexcept;
    bool contains(const QString& key) const;
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

    State m_state = State::Configuring;
    std::vector<GlobalDataDefinition> m_definitions;
    std::map<QString, std::unique_ptr<ManagedService>> m_instances;
    QStringList m_initializedKeys;
};
}
