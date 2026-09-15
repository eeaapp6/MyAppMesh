#include "ComponentFactory.h"

#include <QSet>

#include <exception>

namespace AppMesh::App
{
namespace
{
AppDiagnostic componentDiagnostic(const QString& code,
                                  const QString& message,
                                  const QString& detail,
                                  const QString& key)
{
    return {QStringLiteral("component"), code, message, detail, false, key};
}
}

LifecycleResult ComponentFactory::add(ComponentDefinition definitionValue)
{
    LifecycleResult result;
    if (m_state != State::Configuring && m_state != State::Failed)
    {
        result.add(componentDiagnostic(QStringLiteral("APP-COMPONENT-REGISTRATION-CLOSED"),
                                       QStringLiteral("Component registration is closed."),
                                       QStringLiteral("Definitions must be added before initialization."),
                                       definitionValue.key));
        return result;
    }
    if (definitionValue.key.trimmed().isEmpty() || !definitionValue.create)
    {
        result.add(componentDiagnostic(QStringLiteral("APP-COMPONENT-DEFINITION-INVALID"),
                                       QStringLiteral("Component definition is invalid."),
                                       QStringLiteral("A non-empty key and creator are required."),
                                       definitionValue.key));
        return result;
    }
    if (definition(definitionValue.key))
    {
        result.add(componentDiagnostic(QStringLiteral("APP-COMPONENT-DUPLICATE"),
                                       QStringLiteral("Component key is already registered."),
                                       QStringLiteral("Duplicate definitions are not allowed."),
                                       definitionValue.key));
        return result;
    }
    m_definitions.push_back(std::move(definitionValue));
    return result;
}

const ComponentDefinition* ComponentFactory::definition(const QString& key) const
{
    for (const auto& candidate : m_definitions)
    {
        if (candidate.key == key)
        {
            return &candidate;
        }
    }
    return nullptr;
}

LifecycleResult ComponentFactory::initialize(const GlobalDataFactory& globalData,
                                             IFastCAERegistrationAdapter& adapter)
{
    LifecycleResult result;
    if (!globalData.isInitialized())
    {
        result.add(componentDiagnostic(QStringLiteral("APP-GLOBAL-DATA-NOT-READY"),
                                       QStringLiteral("Global data must be initialized before components."),
                                       QStringLiteral("Initialize GlobalDataFactory and verify its result first."),
                                       QString()));
        return result;
    }
    if (m_state == State::Initialized || m_state == State::Initializing)
    {
        result.add(componentDiagnostic(QStringLiteral("APP-COMPONENTS-ALREADY-INITIALIZED"),
                                       QStringLiteral("Components are already initialized."),
                                       QStringLiteral("Repeated initialization is rejected."),
                                       QString()));
        return result;
    }

    rollback(adapter);
    for (const auto& candidate : m_definitions)
    {
        for (const auto& globalKey : candidate.requiredGlobalData)
        {
            if (!globalData.contains(globalKey))
            {
                result.add(componentDiagnostic(QStringLiteral("APP-GLOBAL-DATA-DEPENDENCY-MISSING"),
                                               QStringLiteral("A component global-data dependency is missing."),
                                               QStringLiteral("Required global data: %1").arg(globalKey),
                                               candidate.key));
            }
        }
        for (const auto& dependencyKey : candidate.dependencies)
        {
            if (!definition(dependencyKey))
            {
                result.add(componentDiagnostic(QStringLiteral("APP-COMPONENT-DEPENDENCY-MISSING"),
                                               QStringLiteral("A component dependency is not registered."),
                                               QStringLiteral("Required component: %1").arg(dependencyKey),
                                               candidate.key));
            }
        }
    }
    if (!result.succeeded())
    {
        m_state = State::Failed;
        return result;
    }

    m_state = State::Initializing;
    QSet<QString> completed;
    while (completed.size() < static_cast<int>(m_definitions.size()))
    {
        bool progressed = false;
        for (const auto& candidate : m_definitions)
        {
            if (completed.contains(candidate.key))
            {
                continue;
            }

            bool dependenciesReady = true;
            for (const auto& dependencyKey : candidate.dependencies)
            {
                dependenciesReady = dependenciesReady && completed.contains(dependencyKey);
            }
            if (!dependenciesReady)
            {
                continue;
            }

            std::unique_ptr<ManagedService> service;
            try
            {
                service = candidate.create();
            }
            catch (const std::exception& exception)
            {
                result.add(componentDiagnostic(QStringLiteral("APP-COMPONENT-CREATE-FAILED"),
                                               QStringLiteral("Component creation failed."),
                                               QString::fromLocal8Bit(exception.what()),
                                               candidate.key));
            }
            catch (...)
            {
                result.add(componentDiagnostic(QStringLiteral("APP-COMPONENT-CREATE-FAILED"),
                                               QStringLiteral("Component creation failed."),
                                               QStringLiteral("The creator raised an unknown exception."),
                                               candidate.key));
            }
            if (!result.succeeded() || !service)
            {
                if (result.succeeded())
                {
                    result.add(componentDiagnostic(QStringLiteral("APP-COMPONENT-CREATE-FAILED"),
                                                   QStringLiteral("Component creation returned no service."),
                                                   QStringLiteral("Creators must return an owned service instance."),
                                                   candidate.key));
                }
                rollback(adapter);
                m_state = State::Failed;
                return result;
            }

            QString detail;
            if (!adapter.registerComponent(candidate.key, *service, &detail))
            {
                result.add(componentDiagnostic(QStringLiteral("APP-COMPONENT-REGISTER-FAILED"),
                                               QStringLiteral("FastCAE component registration failed."),
                                               detail,
                                               candidate.key));
                rollback(adapter);
                m_state = State::Failed;
                return result;
            }

            m_instances.emplace(candidate.key, std::move(service));
            m_initializedKeys.append(candidate.key);
            completed.insert(candidate.key);
            progressed = true;
        }

        if (!progressed)
        {
            result.add(componentDiagnostic(QStringLiteral("APP-COMPONENT-DEPENDENCY-CYCLE"),
                                           QStringLiteral("Component dependency cycle detected."),
                                           QStringLiteral("No remaining component has all dependencies satisfied."),
                                           QString()));
            rollback(adapter);
            m_state = State::Failed;
            return result;
        }
    }

    m_state = State::Initialized;
    return result;
}

void ComponentFactory::rollback(IFastCAERegistrationAdapter& adapter) noexcept
{
    for (auto iterator = m_initializedKeys.crbegin(); iterator != m_initializedKeys.crend(); ++iterator)
    {
        const auto instanceIterator = m_instances.find(*iterator);
        if (instanceIterator != m_instances.end())
        {
            adapter.unregisterComponent(*iterator, *instanceIterator->second);
        }
    }
    m_initializedKeys.clear();
    m_instances.clear();
}

void ComponentFactory::shutdown(IFastCAERegistrationAdapter& adapter) noexcept
{
    rollback(adapter);
    m_state = State::Configuring;
}

bool ComponentFactory::isInitialized() const noexcept
{
    return m_state == State::Initialized;
}

ManagedService* ComponentFactory::instance(const QString& key) const
{
    const auto iterator = m_instances.find(key);
    return iterator == m_instances.end() ? nullptr : iterator->second.get();
}

QStringList ComponentFactory::initializedKeys() const
{
    return m_initializedKeys;
}
}
