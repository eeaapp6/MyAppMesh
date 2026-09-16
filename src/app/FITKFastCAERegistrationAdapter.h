#pragma once

#include "GlobalDataFactory.h"

#include <QStringList>

#include <map>

namespace AppFrame
{
class FITKApplication;
class FITKAbstractGlobalDataFactory;
class FITKComponentFactory;
}

namespace AppMesh::App
{
class FITKFastCAERegistrationAdapter final : public IFastCAERegistrationAdapter
{
public:
    explicit FITKFastCAERegistrationAdapter(AppFrame::FITKApplication& application);
    ~FITKFastCAERegistrationAdapter() override;

    AppOperationResult initializeRuntime() override;
    AppOperationResult shutdownRuntime() noexcept override;
    bool isFITKBacked() const noexcept override;
    QString adapterName() const override;

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

    QStringList lifecycleEvents() const;

private:
    enum class State
    {
        Created,
        Running,
        Shutdown
    };

    bool validateRegistration(const QString& key, QString* errorDetail) const;
    void releaseGlobalFactory() noexcept;
    void releaseComponentFactory() noexcept;
    void releaseRegistrations() noexcept;

    AppFrame::FITKApplication& m_application;
    State m_state = State::Created;
    std::map<QString, ManagedService*> m_globalData;
    std::map<QString, ManagedService*> m_components;
    AppFrame::FITKAbstractGlobalDataFactory* m_globalFactory = nullptr;
    AppFrame::FITKComponentFactory* m_componentFactory = nullptr;
    QStringList m_events;
};
}
