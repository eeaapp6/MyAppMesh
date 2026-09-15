#include "GlobalDataFactory.h"

#include <exception>

namespace AppMesh::App
{
namespace
{
AppDiagnostic lifecycleDiagnostic(const QString& code,
                                  const QString& message,
                                  const QString& detail,
                                  const QString& key)
{
    return {QStringLiteral("component"), code, message, detail, false, key};
}
}

LifecycleResult GlobalDataFactory::add(GlobalDataDefinition definition)
{
    LifecycleResult result;
    if (m_state != State::Configuring && m_state != State::Failed)
    {
        result.add(lifecycleDiagnostic(QStringLiteral("APP-GLOBAL-DATA-REGISTRATION-CLOSED"),
                                       QStringLiteral("Global data registration is closed."),
                                       QStringLiteral("Definitions must be added before initialization."),
                                       definition.key));
        return result;
    }
    if (definition.key.trimmed().isEmpty() || !definition.create)
    {
        result.add(lifecycleDiagnostic(QStringLiteral("APP-GLOBAL-DATA-DEFINITION-INVALID"),
                                       QStringLiteral("Global data definition is invalid."),
                                       QStringLiteral("A non-empty key and creator are required."),
                                       definition.key));
        return result;
    }
    for (const auto& existing : m_definitions)
    {
        if (existing.key == definition.key)
        {
            result.add(lifecycleDiagnostic(QStringLiteral("APP-GLOBAL-DATA-DUPLICATE"),
                                           QStringLiteral("Global data key is already registered."),
                                           QStringLiteral("Duplicate definitions are not allowed."),
                                           definition.key));
            return result;
        }
    }
    m_definitions.push_back(std::move(definition));
    return result;
}

LifecycleResult GlobalDataFactory::initialize(IFastCAERegistrationAdapter& adapter)
{
    LifecycleResult result;
    if (m_state == State::Initialized || m_state == State::Initializing)
    {
        result.add(lifecycleDiagnostic(QStringLiteral("APP-GLOBAL-DATA-ALREADY-INITIALIZED"),
                                       QStringLiteral("Global data is already initialized."),
                                       QStringLiteral("Repeated initialization is rejected."),
                                       QString()));
        return result;
    }

    rollback(adapter);
    m_state = State::Initializing;
    for (const auto& definition : m_definitions)
    {
        std::unique_ptr<ManagedService> service;
        try
        {
            service = definition.create();
        }
        catch (const std::exception& exception)
        {
            result.add(lifecycleDiagnostic(QStringLiteral("APP-GLOBAL-DATA-CREATE-FAILED"),
                                           QStringLiteral("Global data creation failed."),
                                           QString::fromLocal8Bit(exception.what()),
                                           definition.key));
        }
        catch (...)
        {
            result.add(lifecycleDiagnostic(QStringLiteral("APP-GLOBAL-DATA-CREATE-FAILED"),
                                           QStringLiteral("Global data creation failed."),
                                           QStringLiteral("The creator raised an unknown exception."),
                                           definition.key));
        }

        if (!result.succeeded() || !service)
        {
            if (result.succeeded())
            {
                result.add(lifecycleDiagnostic(QStringLiteral("APP-GLOBAL-DATA-CREATE-FAILED"),
                                               QStringLiteral("Global data creation returned no service."),
                                               QStringLiteral("Creators must return an owned service instance."),
                                               definition.key));
            }
            rollback(adapter);
            m_state = State::Failed;
            return result;
        }

        QString detail;
        if (!adapter.registerGlobalData(definition.key, *service, &detail))
        {
            result.add(lifecycleDiagnostic(QStringLiteral("APP-GLOBAL-DATA-REGISTER-FAILED"),
                                           QStringLiteral("FastCAE global data registration failed."),
                                           detail,
                                           definition.key));
            rollback(adapter);
            m_state = State::Failed;
            return result;
        }

        m_instances.emplace(definition.key, std::move(service));
        m_initializedKeys.append(definition.key);
    }

    m_state = State::Initialized;
    return result;
}

void GlobalDataFactory::rollback(IFastCAERegistrationAdapter& adapter) noexcept
{
    for (auto iterator = m_initializedKeys.crbegin(); iterator != m_initializedKeys.crend(); ++iterator)
    {
        const auto instanceIterator = m_instances.find(*iterator);
        if (instanceIterator != m_instances.end())
        {
            adapter.unregisterGlobalData(*iterator, *instanceIterator->second);
        }
    }
    m_initializedKeys.clear();
    m_instances.clear();
}

void GlobalDataFactory::shutdown(IFastCAERegistrationAdapter& adapter) noexcept
{
    rollback(adapter);
    m_state = State::Configuring;
}

bool GlobalDataFactory::isInitialized() const noexcept
{
    return m_state == State::Initialized;
}

bool GlobalDataFactory::contains(const QString& key) const
{
    return m_instances.find(key) != m_instances.end();
}

ManagedService* GlobalDataFactory::instance(const QString& key) const
{
    const auto iterator = m_instances.find(key);
    return iterator == m_instances.end() ? nullptr : iterator->second.get();
}

QStringList GlobalDataFactory::initializedKeys() const
{
    return m_initializedKeys;
}
}
