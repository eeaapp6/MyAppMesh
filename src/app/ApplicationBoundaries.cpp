#include "ApplicationBoundaries.h"

#include <QCoreApplication>

namespace AppMesh::App
{
namespace
{
AppDiagnostic boundaryDiagnostic(const QString& code,
                                 const QString& message,
                                 const QString& detail)
{
    return {QStringLiteral("application"), code, message, detail, false, QString(), QString()};
}
}

OptionalLifecycleStage::OptionalLifecycleStage(QString name,
                                               bool enabled,
                                               StageAction startAction,
                                               StageAction stopAction)
    : m_name(std::move(name)),
      m_enabled(enabled),
      m_startAction(std::move(startAction)),
      m_stopAction(std::move(stopAction))
{
}

AppOperationResult OptionalLifecycleStage::start()
{
    AppOperationResult result;
    if (m_active)
    {
        return result;
    }
    if (m_enabled && !m_startAction)
    {
        result.add(boundaryDiagnostic(QStringLiteral("APP-LIFECYCLE-ADAPTER-MISSING"),
                                      QStringLiteral("An enabled lifecycle capability has no adapter."),
                                      m_name));
        return result;
    }
    if (m_enabled)
    {
        result = m_startAction();
    }
    if (result.succeeded())
    {
        m_active = true;
    }
    return result;
}

AppOperationResult OptionalLifecycleStage::stop()
{
    AppOperationResult result;
    if (!m_active)
    {
        return result;
    }
    if (m_enabled && m_stopAction)
    {
        result = m_stopAction();
    }
    m_active = false;
    return result;
}

bool OptionalLifecycleStage::isActive() const noexcept
{
    return m_active;
}

bool OptionalLifecycleStage::isEnabled() const noexcept
{
    return m_enabled;
}

AppOperationResult BasicCommandLineBoundary::parse(const QStringList& arguments,
                                                   ParsedCommandLine* parsed)
{
    AppOperationResult result;
    if (!parsed)
    {
        result.add(boundaryDiagnostic(QStringLiteral("APP-COMMAND-LINE-TARGET-INVALID"),
                                      QStringLiteral("Command-line output is unavailable."),
                                      QStringLiteral("A parsed command-line target is required.")));
        return result;
    }

    *parsed = {};
    for (int index = 1; index < arguments.size(); ++index)
    {
        const QString argument = arguments.at(index);
        if (argument == QStringLiteral("--smoke-test"))
        {
            parsed->smokeTest = true;
        }
        else if (argument == QStringLiteral("--workbench"))
        {
            parsed->workbench = true;
        }
        else if (argument.startsWith(QStringLiteral("--working-directory=")))
        {
            parsed->workingDirectory = argument.mid(QStringLiteral("--working-directory=").size());
            if (parsed->workingDirectory.trimmed().isEmpty())
            {
                result.add(boundaryDiagnostic(QStringLiteral("APP-WORKDIR-ARGUMENT-INVALID"),
                                              QStringLiteral("The working-directory argument is empty."),
                                              argument));
                return result;
            }
        }
        else
        {
            parsed->inputs.append(argument);
        }
    }
    return result;
}

AppOperationResult BasicCommandLineBoundary::process(const ParsedCommandLine& parsed)
{
    AppOperationResult result;
    if (!parsed.inputs.isEmpty())
    {
        result.add(boundaryDiagnostic(QStringLiteral("APP-INPUT-NOT-ENABLED"),
                                      QStringLiteral("Command-line input handling is not enabled yet."),
                                      parsed.inputs.join(QStringLiteral(", "))));
    }
    return result;
}

DisabledLifecycleBoundary::DisabledLifecycleBoundary(QString capability)
    : m_capability(std::move(capability))
{
}

AppOperationResult DisabledLifecycleBoundary::initialize()
{
    m_initialized = true;
    return {};
}

AppOperationResult DisabledLifecycleBoundary::shutdown()
{
    m_initialized = false;
    return {};
}

bool DisabledLifecycleBoundary::isInitialized() const noexcept
{
    return m_initialized;
}

AppOperationResult DisabledOperatorBoundary::initialize()
{
    m_initialized = true;
    return {};
}

AppOperationResult DisabledOperatorBoundary::stopAccepting()
{
    return {};
}

AppOperationResult DisabledOperatorBoundary::finishCurrent()
{
    return {};
}

AppOperationResult DisabledOperatorBoundary::shutdown()
{
    m_initialized = false;
    return {};
}

QtEventLoop::QtEventLoop(QCoreApplication& application, bool bypass)
    : m_application(application), m_bypass(bypass)
{
}

int QtEventLoop::exec()
{
    return m_bypass ? 0 : m_application.exec();
}
}
