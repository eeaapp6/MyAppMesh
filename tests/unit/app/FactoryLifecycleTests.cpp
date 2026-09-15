#include "ComponentFactory.h"
#include "GlobalDataFactory.h"
#include "TestSupport.h"

#include <QCoreApplication>
#include <QStringList>

#include <memory>

namespace
{
class Service final : public AppMesh::App::ManagedService
{
};

class RecordingAdapter final : public AppMesh::App::IFastCAERegistrationAdapter
{
public:
    bool registerGlobalData(const QString& key,
                            AppMesh::App::ManagedService&,
                            QString* errorDetail) override
    {
        events.append(QStringLiteral("global+") + key);
        if (key == failGlobalKey)
        {
            *errorDetail = QStringLiteral("injected global registration failure");
            return false;
        }
        return true;
    }

    void unregisterGlobalData(const QString& key,
                              AppMesh::App::ManagedService&) noexcept override
    {
        events.append(QStringLiteral("global-") + key);
    }

    bool registerComponent(const QString& key,
                           AppMesh::App::ManagedService&,
                           QString* errorDetail) override
    {
        events.append(QStringLiteral("component+") + key);
        if (key == failComponentKey)
        {
            *errorDetail = QStringLiteral("injected component registration failure");
            return false;
        }
        return true;
    }

    void unregisterComponent(const QString& key,
                             AppMesh::App::ManagedService&) noexcept override
    {
        events.append(QStringLiteral("component-") + key);
    }

    QString failGlobalKey;
    QString failComponentKey;
    QStringList events;
};

AppMesh::App::ServiceCreator serviceCreator()
{
    return [] { return std::unique_ptr<AppMesh::App::ManagedService>(new Service); };
}

bool hasCode(const AppMesh::App::LifecycleResult& result, const QString& code)
{
    for (const auto& diagnostic : result.diagnostics)
    {
        if (diagnostic.code == code)
        {
            return true;
        }
    }
    return false;
}
}

int main(int argc, char* argv[])
{
    QCoreApplication application(argc, argv);
    TestSuite suite;

    AppMesh::App::GlobalDataFactory globals;
    suite.expect(globals.add({QStringLiteral("runtime"), serviceCreator()}).succeeded(),
                 QStringLiteral("global data definition registered"));
    suite.expect(!globals.add({QStringLiteral("runtime"), serviceCreator()}).succeeded(),
                 QStringLiteral("duplicate global data definition rejected"));

    AppMesh::App::ComponentFactory prematureComponents;
    prematureComponents.add({QStringLiteral("console"), {}, {}, serviceCreator()});
    RecordingAdapter prematureAdapter;
    const auto prematureResult = prematureComponents.initialize(globals, prematureAdapter);
    suite.expect(hasCode(prematureResult, QStringLiteral("APP-GLOBAL-DATA-NOT-READY")),
                 QStringLiteral("components cannot initialize before global data"));
    suite.expect(prematureAdapter.events.isEmpty(),
                 QStringLiteral("premature component initialization has no side effects"));

    RecordingAdapter adapter;
    suite.expect(globals.initialize(adapter).succeeded(), QStringLiteral("global data initialized"));

    AppMesh::App::ComponentFactory components;
    suite.expect(components.add({QStringLiteral("view"),
                                 {QStringLiteral("runtime")},
                                 {QStringLiteral("messages")},
                                 serviceCreator()}).succeeded(),
                 QStringLiteral("dependent component definition registered"));
    suite.expect(components.add({QStringLiteral("messages"),
                                 {QStringLiteral("runtime")},
                                 {},
                                 serviceCreator()}).succeeded(),
                 QStringLiteral("component dependency registered"));
    suite.expect(!components.add({QStringLiteral("messages"), {}, {}, serviceCreator()}).succeeded(),
                 QStringLiteral("duplicate component definition rejected"));
    suite.expect(components.initialize(globals, adapter).succeeded(),
                 QStringLiteral("components initialized in dependency order"));
    suite.expect(adapter.events == QStringList{QStringLiteral("global+runtime"),
                                               QStringLiteral("component+messages"),
                                               QStringLiteral("component+view")},
                 QStringLiteral("global data precedes topologically ordered components"));

    components.shutdown(adapter);
    globals.shutdown(adapter);
    suite.expect(adapter.events.mid(3) == QStringList{QStringLiteral("component-view"),
                                                      QStringLiteral("component-messages"),
                                                      QStringLiteral("global-runtime")},
                 QStringLiteral("shutdown releases registrations in reverse order"));

    AppMesh::App::GlobalDataFactory failureGlobals;
    failureGlobals.add({QStringLiteral("runtime"), serviceCreator()});
    RecordingAdapter failureAdapter;
    suite.expect(failureGlobals.initialize(failureAdapter).succeeded(),
                 QStringLiteral("failure fixture globals initialized"));

    AppMesh::App::ComponentFactory failureComponents;
    failureComponents.add({QStringLiteral("messages"),
                           {QStringLiteral("runtime")}, {}, serviceCreator()});
    failureComponents.add({QStringLiteral("view"),
                           {QStringLiteral("runtime")},
                           {QStringLiteral("messages")},
                           serviceCreator()});
    failureAdapter.failComponentKey = QStringLiteral("view");
    const auto failureResult = failureComponents.initialize(failureGlobals, failureAdapter);
    suite.expect(!failureResult.succeeded(), QStringLiteral("component registration failure is reported"));
    suite.expect(hasCode(failureResult, QStringLiteral("APP-COMPONENT-REGISTER-FAILED")),
                 QStringLiteral("component failure has structured diagnostic"));
    suite.expect(!failureComponents.isInitialized() && failureComponents.initializedKeys().isEmpty(),
                 QStringLiteral("failed component initialization leaves no partial state"));
    suite.expect(failureGlobals.isInitialized(),
                 QStringLiteral("component failure preserves valid global data"));
    suite.expect(failureAdapter.events.contains(QStringLiteral("component-messages")),
                 QStringLiteral("component failure rolls back prior registrations"));

    AppMesh::App::GlobalDataFactory globalRegistrationFailure;
    globalRegistrationFailure.add({QStringLiteral("runtime"), serviceCreator()});
    globalRegistrationFailure.add({QStringLiteral("model"), serviceCreator()});
    RecordingAdapter globalFailureAdapter;
    globalFailureAdapter.failGlobalKey = QStringLiteral("model");
    const auto globalFailureResult = globalRegistrationFailure.initialize(globalFailureAdapter);
    suite.expect(hasCode(globalFailureResult, QStringLiteral("APP-GLOBAL-DATA-REGISTER-FAILED")),
                 QStringLiteral("global registration failure has structured diagnostic"));
    suite.expect(!globalRegistrationFailure.isInitialized() &&
                     globalRegistrationFailure.initializedKeys().isEmpty(),
                 QStringLiteral("global registration failure leaves no partial state"));
    suite.expect(globalFailureAdapter.events.contains(QStringLiteral("global-runtime")),
                 QStringLiteral("global registration failure rolls back prior registrations"));

    AppMesh::App::GlobalDataFactory cycleGlobals;
    cycleGlobals.add({QStringLiteral("runtime"), serviceCreator()});
    RecordingAdapter cycleAdapter;
    suite.expect(cycleGlobals.initialize(cycleAdapter).succeeded(),
                 QStringLiteral("cycle fixture globals initialized"));
    AppMesh::App::ComponentFactory cyclicComponents;
    cyclicComponents.add({QStringLiteral("first"),
                          {QStringLiteral("runtime")},
                          {QStringLiteral("second")},
                          serviceCreator()});
    cyclicComponents.add({QStringLiteral("second"),
                          {QStringLiteral("runtime")},
                          {QStringLiteral("first")},
                          serviceCreator()});
    const auto cycleResult = cyclicComponents.initialize(cycleGlobals, cycleAdapter);
    suite.expect(hasCode(cycleResult, QStringLiteral("APP-COMPONENT-DEPENDENCY-CYCLE")),
                 QStringLiteral("component dependency cycle is diagnosed"));
    suite.expect(cyclicComponents.initializedKeys().isEmpty(),
                 QStringLiteral("dependency cycle creates no components"));

    return suite.result();
}
