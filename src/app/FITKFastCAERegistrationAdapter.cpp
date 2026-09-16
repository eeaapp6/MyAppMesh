#include "FITKFastCAERegistrationAdapter.h"

#include "FITK_Kernel/FITKAppFramework/FITKAbstractGlobalDataFactory.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponentFactory.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Kernel/FITKAppFramework/FITKPluginManager.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKThreadPool.h"

#include <QApplication>

#include <new>

namespace AppMesh::App
{
namespace
{
class GlobalDataFactoryBridge final : public AppFrame::FITKAbstractGlobalDataFactory
{
public:
    void materialize(AppFrame::FITKGlobalData* globalData)
    {
        createData(globalData);
        m_materialized = true;
    }

    bool materialized() const noexcept
    {
        return m_materialized;
    }

private:
    bool m_materialized = false;
};

class ComponentFactoryBridge final : public AppFrame::FITKComponentFactory
{
public:
    void materialize()
    {
        create();
        m_materialized = true;
    }

    bool materialized() const noexcept
    {
        return m_materialized;
    }

private:
    QList<AppFrame::FITKComponentInterface*> createComponents() override
    {
        return {};
    }

    bool m_materialized = false;
};

AppDiagnostic runtimeDiagnostic(const QString& code,
                                const QString& message,
                                const QString& detail)
{
    return {QStringLiteral("component"), code, message, detail, false, QString(),
            QStringLiteral("fitk.application")};
}
}

FITKFastCAERegistrationAdapter::FITKFastCAERegistrationAdapter(
    AppFrame::FITKApplication& application)
    : m_application(application)
{
}

FITKFastCAERegistrationAdapter::~FITKFastCAERegistrationAdapter()
{
    releaseRegistrations();
}

AppOperationResult FITKFastCAERegistrationAdapter::initializeRuntime()
{
    AppOperationResult result;
    if (m_state != State::Created)
    {
        result.add(runtimeDiagnostic(QStringLiteral("FITK-RUNTIME-STATE-INVALID"),
                                     QStringLiteral("FITK runtime initialization was repeated."),
                                     QStringLiteral("A FITK application boundary is initialized once.")));
        return result;
    }
    if (qApp != &m_application)
    {
        result.add(runtimeDiagnostic(QStringLiteral("FITK-APPLICATION-NOT-ACTIVE"),
                                     QStringLiteral("The supplied FITKApplication is not qApp."),
                                     QStringLiteral("Construct FITKApplication as the process application object.")));
        return result;
    }
    if (!m_application.getGlobalData() || !m_application.getComponents() ||
        !m_application.getPluginsManager() || !m_application.getSignalTransfer() ||
        !Core::FITKThreadPool::getInstance() || !Core::FITKOperatorRepo::getInstance())
    {
        result.add(runtimeDiagnostic(QStringLiteral("FITK-BASE-MANAGER-MISSING"),
                                     QStringLiteral("FITKApplication did not create all required base managers."),
                                     QStringLiteral("Global data, components, plugins, signals, thread pool and operator repository are required.")));
        return result;
    }

    m_state = State::Running;
    m_events.append(QStringLiteral("fitk.application+"));
    return result;
}

AppOperationResult FITKFastCAERegistrationAdapter::shutdownRuntime() noexcept
{
    AppOperationResult result;
    if (m_state == State::Shutdown || m_state == State::Created)
    {
        return result;
    }
    if (!m_globalData.empty() || !m_components.empty())
    {
        result.add(runtimeDiagnostic(QStringLiteral("FITK-RUNTIME-REGISTRATIONS-ACTIVE"),
                                     QStringLiteral("FITK runtime cannot shut down with active registrations."),
                                     QStringLiteral("Release components and global data before the FITK runtime boundary.")));
        return result;
    }

    releaseRegistrations();
    Core::FITKThreadPool::getInstance()->wait();
    Core::FITKOperatorRepo::getInstance()->clear();
    m_state = State::Shutdown;
    m_events.append(QStringLiteral("fitk.application-"));
    return result;
}

bool FITKFastCAERegistrationAdapter::isFITKBacked() const noexcept
{
    return true;
}

QString FITKFastCAERegistrationAdapter::adapterName() const
{
    return QStringLiteral("FITKFastCAERegistrationAdapter");
}

bool FITKFastCAERegistrationAdapter::validateRegistration(const QString& key,
                                                          QString* errorDetail) const
{
    if (m_state != State::Running)
    {
        if (errorDetail)
        {
            *errorDetail = QStringLiteral("FITKApplication runtime is not initialized.");
        }
        return false;
    }
    if (key.trimmed().isEmpty())
    {
        if (errorDetail)
        {
            *errorDetail = QStringLiteral("FITK registration keys must not be empty.");
        }
        return false;
    }
    if (m_globalData.find(key) != m_globalData.end() ||
        m_components.find(key) != m_components.end())
    {
        if (errorDetail)
        {
            *errorDetail = QStringLiteral("The FITK registration key is already active: %1").arg(key);
        }
        return false;
    }
    return true;
}

bool FITKFastCAERegistrationAdapter::registerGlobalData(const QString& key,
                                                        ManagedService& service,
                                                        QString* errorDetail)
{
    if (!validateRegistration(key, errorDetail) || !m_application.getGlobalData())
    {
        if (errorDetail && errorDetail->isEmpty())
        {
            *errorDetail = QStringLiteral("FITKGlobalData is unavailable.");
        }
        return false;
    }

    if (!m_globalFactory)
    {
        auto* bridge = new (std::nothrow) GlobalDataFactoryBridge;
        if (!bridge)
        {
            if (errorDetail)
            {
                *errorDetail = QStringLiteral("Unable to allocate the FITK global-data factory bridge.");
            }
            return false;
        }
        m_application.regGlobalDataFactory(bridge);
        bridge->materialize(m_application.getGlobalData());
        if (!bridge->materialized())
        {
            m_application.regGlobalDataFactory(nullptr);
            delete bridge;
            if (errorDetail)
            {
                *errorDetail = QStringLiteral("FITK global-data factory did not materialize through the public API.");
            }
            return false;
        }
        m_globalFactory = bridge;
    }

    m_globalData.emplace(key, &service);
    m_events.append(QStringLiteral("global+") + key);
    return true;
}

void FITKFastCAERegistrationAdapter::unregisterGlobalData(const QString& key,
                                                          ManagedService& service) noexcept
{
    const auto iterator = m_globalData.find(key);
    if (iterator == m_globalData.end() || iterator->second != &service)
    {
        return;
    }
    m_globalData.erase(iterator);
    m_events.append(QStringLiteral("global-") + key);
    if (m_globalData.empty())
    {
        releaseGlobalFactory();
    }
}

bool FITKFastCAERegistrationAdapter::registerComponent(const QString& key,
                                                       ManagedService& service,
                                                       QString* errorDetail)
{
    if (!validateRegistration(key, errorDetail) || !m_application.getComponents())
    {
        if (errorDetail && errorDetail->isEmpty())
        {
            *errorDetail = QStringLiteral("FITK component manager is unavailable.");
        }
        return false;
    }

    if (!m_componentFactory)
    {
        auto* bridge = new (std::nothrow) ComponentFactoryBridge;
        if (!bridge)
        {
            if (errorDetail)
            {
                *errorDetail = QStringLiteral("Unable to allocate the FITK component factory bridge.");
            }
            return false;
        }
        m_application.regComponentsFactory(bridge);
        bridge->materialize();
        if (!bridge->materialized())
        {
            m_application.regComponentsFactory(nullptr);
            delete bridge;
            if (errorDetail)
            {
                *errorDetail = QStringLiteral("FITK component factory did not materialize through the public API.");
            }
            return false;
        }
        m_componentFactory = bridge;
    }

    m_components.emplace(key, &service);
    m_events.append(QStringLiteral("component+") + key);
    return true;
}

void FITKFastCAERegistrationAdapter::unregisterComponent(const QString& key,
                                                         ManagedService& service) noexcept
{
    const auto iterator = m_components.find(key);
    if (iterator == m_components.end() || iterator->second != &service)
    {
        return;
    }
    m_components.erase(iterator);
    m_events.append(QStringLiteral("component-") + key);
    if (m_components.empty())
    {
        releaseComponentFactory();
    }
}

QStringList FITKFastCAERegistrationAdapter::lifecycleEvents() const
{
    return m_events;
}

void FITKFastCAERegistrationAdapter::releaseGlobalFactory() noexcept
{
    if (!m_globalFactory)
    {
        return;
    }
    m_application.regGlobalDataFactory(nullptr);
    delete m_globalFactory;
    m_globalFactory = nullptr;
}

void FITKFastCAERegistrationAdapter::releaseComponentFactory() noexcept
{
    if (!m_componentFactory)
    {
        return;
    }
    m_application.regComponentsFactory(nullptr);
    delete m_componentFactory;
    m_componentFactory = nullptr;
}

void FITKFastCAERegistrationAdapter::releaseRegistrations() noexcept
{
    m_components.clear();
    m_globalData.clear();
    releaseComponentFactory();
    releaseGlobalFactory();
}
}
