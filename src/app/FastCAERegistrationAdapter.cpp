#include "FastCAERegistrationAdapter.h"

namespace AppMesh::App
{
namespace
{
bool registerService(std::map<QString, ManagedService*>& services,
                     const QString& key,
                     ManagedService& service,
                     QString* errorDetail)
{
    if (key.trimmed().isEmpty())
    {
        if (errorDetail)
        {
            *errorDetail = QStringLiteral("Registration keys must not be empty.");
        }
        return false;
    }
    if (services.find(key) != services.end())
    {
        if (errorDetail)
        {
            *errorDetail = QStringLiteral("The registration key is already active: %1").arg(key);
        }
        return false;
    }
    services.emplace(key, &service);
    return true;
}

void unregisterService(std::map<QString, ManagedService*>& services,
                       const QString& key,
                       ManagedService& service) noexcept
{
    const auto iterator = services.find(key);
    if (iterator != services.end() && iterator->second == &service)
    {
        services.erase(iterator);
    }
}
}

bool LocalFastCAERegistrationAdapter::registerGlobalData(const QString& key,
                                                         ManagedService& service,
                                                         QString* errorDetail)
{
    return registerService(m_globalData, key, service, errorDetail);
}

void LocalFastCAERegistrationAdapter::unregisterGlobalData(const QString& key,
                                                           ManagedService& service) noexcept
{
    unregisterService(m_globalData, key, service);
}

bool LocalFastCAERegistrationAdapter::registerComponent(const QString& key,
                                                        ManagedService& service,
                                                        QString* errorDetail)
{
    return registerService(m_components, key, service, errorDetail);
}

void LocalFastCAERegistrationAdapter::unregisterComponent(const QString& key,
                                                          ManagedService& service) noexcept
{
    unregisterService(m_components, key, service);
}

bool LocalFastCAERegistrationAdapter::hasGlobalData(const QString& key) const
{
    return m_globalData.find(key) != m_globalData.end();
}

bool LocalFastCAERegistrationAdapter::hasComponent(const QString& key) const
{
    return m_components.find(key) != m_components.end();
}
}
