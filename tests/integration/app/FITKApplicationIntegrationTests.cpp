#include "FITKFastCAERegistrationAdapter.h"
#include "TestSupport.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"

#include <QStringList>

namespace
{
class Service final : public AppMesh::App::ManagedService
{
};

bool hasCode(const AppMesh::App::AppOperationResult& result, const QString& code)
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
    qputenv("QT_QPA_PLATFORM", QByteArrayLiteral("offscreen"));
    AppFrame::FITKApplication application(argc, argv);
    TestSuite suite;

    AppMesh::App::FITKFastCAERegistrationAdapter adapter(application);
    suite.expect(adapter.isFITKBacked(),
                 QStringLiteral("production adapter identifies real FITK backing"));
    suite.expect(adapter.initializeRuntime().succeeded(),
                 QStringLiteral("real FITK application runtime initializes"));
    suite.expect(application.getGlobalData() != nullptr &&
                     application.getComponents() != nullptr &&
                     application.getPluginsManager() != nullptr &&
                     application.getSignalTransfer() != nullptr,
                 QStringLiteral("FITKApplication exposes real base managers"));

    Service globalService;
    QString detail;
    suite.expect(adapter.registerGlobalData(QStringLiteral("fitk.global-data"),
                                            globalService,
                                            &detail),
                 QStringLiteral("global-data factory is registered through FITKApplication"));
    suite.expect(!adapter.registerGlobalData(QStringLiteral("fitk.global-data"),
                                             globalService,
                                             &detail) &&
                     detail.contains(QStringLiteral("already"), Qt::CaseInsensitive),
                 QStringLiteral("duplicate FITK global-data key is rejected diagnostically"));

    Service componentService;
    suite.expect(adapter.registerComponent(QStringLiteral("fitk.component-manager"),
                                           componentService,
                                           &detail),
                 QStringLiteral("component factory is registered through FITKApplication"));

    adapter.unregisterComponent(QStringLiteral("fitk.component-manager"), componentService);
    adapter.unregisterGlobalData(QStringLiteral("fitk.global-data"), globalService);
    const auto shutdown = adapter.shutdownRuntime();
    suite.expect(shutdown.succeeded(),
                 QStringLiteral("FITK runtime boundary shuts down without double ownership"));
    suite.expect(adapter.lifecycleEvents() ==
                     (QStringList() << QStringLiteral("fitk.application+")
                                    << QStringLiteral("global+fitk.global-data")
                                    << QStringLiteral("component+fitk.component-manager")
                                    << QStringLiteral("component-fitk.component-manager")
                                    << QStringLiteral("global-fitk.global-data")
                                    << QStringLiteral("fitk.application-")),
                 QStringLiteral("real FITK registrations unwind in strict reverse order"));

    const auto repeatedShutdown = adapter.shutdownRuntime();
    suite.expect(repeatedShutdown.succeeded(),
                 QStringLiteral("repeated FITK shutdown is idempotent"));
    const auto repeatedStart = adapter.initializeRuntime();
    suite.expect(!repeatedStart.succeeded() &&
                     hasCode(repeatedStart, QStringLiteral("FITK-RUNTIME-STATE-INVALID")),
                 QStringLiteral("restart after terminal FITK shutdown is rejected"));

    return suite.result();
}
