#include "TestSupport.h"

#include "FastCAERegistrationAdapter.h"
#include "GlobalDataFactory.h"
#include "model/ModelData/ApplicationRuntime.h"

#include <QCoreApplication>

#include <memory>

namespace
{
class RejectingRegistrationAdapter final : public AppMesh::App::IFastCAERegistrationAdapter
{
public:
    bool registerGlobalData(const QString&,
                            AppMesh::App::ManagedService&,
                            QString* errorDetail) override
    {
        if (errorDetail)
        {
            *errorDetail = QStringLiteral("injected registration failure");
        }
        return false;
    }

    void unregisterGlobalData(const QString&,
                              AppMesh::App::ManagedService&) noexcept override
    {
    }

    bool registerComponent(const QString&,
                           AppMesh::App::ManagedService&,
                           QString*) override
    {
        return false;
    }

    void unregisterComponent(const QString&,
                             AppMesh::App::ManagedService&) noexcept override
    {
    }
};
}

int main(int argc, char* argv[])
{
    QCoreApplication application(argc, argv);
    TestSuite suite;

    AppMesh::App::GlobalDataFactory factory;
    AppMesh::App::LocalFastCAERegistrationAdapter adapter;
    auto addResult = factory.add(
        {QStringLiteral("model.runtime"),
         [] {
             return std::unique_ptr<AppMesh::App::ManagedService>(
                 new AppMesh::Model::ApplicationRuntime);
         }});
    suite.expect(addResult.succeeded(),
                 QStringLiteral("ApplicationRuntime is accepted as managed global data"));
    suite.expect(factory.initialize(adapter).succeeded() &&
                     adapter.hasGlobalData(QStringLiteral("model.runtime")),
                 QStringLiteral("GlobalDataFactory registers the real ApplicationRuntime"));

    auto* runtime = dynamic_cast<AppMesh::Model::ApplicationRuntime*>(
        factory.instance(QStringLiteral("model.runtime")));
    AppMesh::Model::CreateObjectRequest request;
    request.name = QStringLiteral("Integrated object");
    request.type = AppMesh::Model::DataObjectType::Generic;
    suite.expect(runtime && runtime->createObject(request).succeeded(),
                 QStringLiteral("the registered runtime is usable through the composition root"));

    factory.shutdown(adapter);
    suite.expect(!adapter.hasGlobalData(QStringLiteral("model.runtime")) &&
                     factory.instance(QStringLiteral("model.runtime")) == nullptr,
                 QStringLiteral("shutdown unregisters and destroys the runtime"));

    AppMesh::App::GlobalDataFactory failingFactory;
    RejectingRegistrationAdapter rejectingAdapter;
    failingFactory.add(
        {QStringLiteral("model.runtime"),
         [] {
             return std::unique_ptr<AppMesh::App::ManagedService>(
                 new AppMesh::Model::ApplicationRuntime);
         }});
    suite.expect(!failingFactory.initialize(rejectingAdapter).succeeded() &&
                     failingFactory.instance(QStringLiteral("model.runtime")) == nullptr &&
                     !failingFactory.isInitialized(),
                 QStringLiteral("registration failure leaves no partial runtime"));

    return suite.result();
}
