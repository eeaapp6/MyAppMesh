#include "io/GeometryIO/GeometryIO.h"
#include "operators/OperatorsModel/ImportGeometryOperator.h"
#include "TestSupport.h"

#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepTools.hxx>
#include <IGESControl_Controller.hxx>
#include <IGESControl_Writer.hxx>
#include <STEPControl_Writer.hxx>
#include <TopoDS_Shape.hxx>

#include <QCoreApplication>
#include <QFile>
#include <QTemporaryDir>
#include <QSet>

#include <future>
#include <vector>

namespace
{
using namespace AppMesh;

bool createFixture(const QString& path)
{
    const TopoDS_Shape shape = BRepPrimAPI_MakeBox(1.0, 2.0, 3.0).Shape();
    const QString suffix = QFileInfo(path).suffix().toCaseFolded();
    const QByteArray native = QFile::encodeName(path);
    if (suffix == QStringLiteral("brep"))
        return BRepTools::Write(shape, native.constData());
    if (suffix == QStringLiteral("step") || suffix == QStringLiteral("stp"))
    {
        STEPControl_Writer writer;
        return writer.Transfer(shape, STEPControl_AsIs) == IFSelect_RetDone &&
               writer.Write(native.constData()) == IFSelect_RetDone;
    }
    IGESControl_Controller::Init();
    IGESControl_Writer writer;
    writer.AddShape(shape);
    writer.ComputeModel();
    return writer.Write(native.constData());
}

Operators::OperatorInput input(const QString& path, const QString& work, const QString& key)
{
    Operators::OperatorInput value;
    value.operationKey = OperatorsModel::ImportGeometryOperator::operationKey();
    value.parameters.insert(OperatorsModel::ImportGeometryOperator::filePathParameter(), path);
    value.parameters.insert(OperatorsModel::ImportGeometryOperator::workDirectoryParameter(), work);
    value.parameters.insert(OperatorsModel::ImportGeometryOperator::formatKeyParameter(), key);
    value.parameters.insert(OperatorsModel::ImportGeometryOperator::requestedNameParameter(),
                            QStringLiteral("Imported"));
    return value;
}
}

int main(int argc, char* argv[])
{
    QCoreApplication application(argc, argv);
    TestSuite suite;
    QTemporaryDir temporary;
    suite.expect(temporary.isValid(), QStringLiteral("temporary geometry directory is available"));

    const auto capabilities = GeometryIO::firstReleaseGeometryCapabilities();
    suite.expect(capabilities.size() == 3 &&
                     GeometryIO::validateGeometryCapabilities(capabilities).succeeded(),
                 QStringLiteral("the frozen BRep, STEP/STP, and IGES/IGS table is conflict-free"));
    auto conflicting = capabilities;
    conflicting[2].aliases.append(QStringLiteral("stp"));
    suite.expect(!GeometryIO::validateGeometryCapabilities(conflicting).succeeded(),
                 QStringLiteral("duplicate format aliases are diagnosed"));

    auto repository = std::make_shared<GeometryIO::FITKGeometryRepository>();
    auto registry = std::make_shared<OperatorsModel::GeometryReaderRegistry>();
    const auto registered = GeometryIO::registerFirstReleaseGeometryReaders(*registry, repository);
    suite.expect(registered.succeeded() && registry->readerCount() == 3 &&
                     registry->find(QStringLiteral("STP")) && registry->find(QStringLiteral("igs")),
                 QStringLiteral("canonical formats and case-insensitive aliases register"));

    const struct { const char* extension; const char* key; Model::GeometryType type; } cases[] = {
        {"brep", "brep", Model::GeometryType::BRep},
        {"step", "step", Model::GeometryType::Step},
        {"stp", "stp", Model::GeometryType::Step},
        {"iges", "iges", Model::GeometryType::Iges},
        {"igs", "igs", Model::GeometryType::Iges}};
    for (const auto& item : cases)
    {
        const QString path = temporary.filePath(QStringLiteral("box.%1").arg(QString::fromLatin1(item.extension)));
        suite.expect(createFixture(path), QStringLiteral("real OCC fixture is generated: ") + path);
        OperatorsModel::GeometryReadRequest request;
        request.filePath = path;
        request.formatKey = QString::fromLatin1(item.key);
        const auto read = registry->find(request.formatKey)->read(request);
        QSet<Model::GeometryEntityId> topologyIds;
        if (read.geometry)
        {
            for (const auto& entity : read.geometry->topology)
                topologyIds.insert(entity.id);
        }
        suite.expect(read.succeeded() && read.geometry && read.transaction &&
                         read.geometry->type == item.type &&
                         read.geometry->adapter.providerKey == QStringLiteral("fitk.occ") &&
                         !read.geometry->adapter.modelKey.isEmpty() &&
                         !read.geometry->topology.isEmpty() &&
                         topologyIds.size() == read.geometry->topology.size() &&
                         !topologyIds.contains(Model::InvalidGeometryEntityId),
                     QStringLiteral("FITK/OCC reads real ") + QString::fromLatin1(item.extension));
        const QString modelKey = read.geometry ? read.geometry->adapter.modelKey : QString();
        const bool published = read.transaction && read.transaction->publish().succeeded();
        if (published)
            read.transaction->finalize();
        suite.expect(published && repository->contains(modelKey),
                     QStringLiteral("successful transaction preserves the FITK model"));
    }

    const int preserved = repository->modelCount();
    const QString emptyPath = temporary.filePath(QStringLiteral("empty.brep"));
    QFile(emptyPath).open(QIODevice::WriteOnly);
    OperatorsModel::GeometryReadRequest emptyRequest;
    emptyRequest.filePath = emptyPath;
    const auto empty = registry->find(QStringLiteral("brep"))->read(emptyRequest);
    suite.expect(!empty.succeeded() && repository->modelCount() == preserved,
                 QStringLiteral("empty files fail without FITK residue"));

    const QString corruptPath = temporary.filePath(QStringLiteral("corrupt.step"));
    QFile corrupt(corruptPath);
    corrupt.open(QIODevice::WriteOnly);
    corrupt.write("not a STEP file");
    corrupt.close();
    OperatorsModel::GeometryReadRequest corruptRequest;
    corruptRequest.filePath = corruptPath;
    const auto corruptRead = registry->find(QStringLiteral("step"))->read(corruptRequest);
    suite.expect(!corruptRead.succeeded() && repository->modelCount() == preserved,
                 QStringLiteral("corrupt content is diagnosed without FITK residue"));

    const QString mismatchPath = temporary.filePath(QStringLiteral("mismatch.step"));
    const QString sourceBrep = temporary.filePath(QStringLiteral("mismatch-source.brep"));
    createFixture(sourceBrep);
    QFile::copy(sourceBrep, mismatchPath);
    OperatorsModel::GeometryReadRequest mismatchRequest;
    mismatchRequest.filePath = mismatchPath;
    const auto mismatch = registry->find(QStringLiteral("step"))->read(mismatchRequest);
    suite.expect(!mismatch.succeeded() && repository->modelCount() == preserved,
                 QStringLiteral("extension/content mismatch is rejected without residue"));
    Model::ApplicationRuntime mismatchRuntime;
    Model::GeometryManager mismatchManager(mismatchRuntime);
    OperatorsModel::ImportGeometryOperator mismatchOperator(mismatchManager, registry);
    const auto formatMismatch = Operators::executeSafely(
        mismatchOperator, input(sourceBrep, temporary.path(), QStringLiteral("step")), 90);
    suite.expect(!formatMismatch.succeeded() && mismatchManager.objectCount() == 0,
                 QStringLiteral("format key and extension mismatch is rejected before FITK import"));

    const QString statePath = temporary.filePath(QStringLiteral("transaction-state.brep"));
    createFixture(statePath);
    auto readTransaction = [&registry, &statePath] {
        OperatorsModel::GeometryReadRequest request;
        request.filePath = statePath;
        request.formatKey = QStringLiteral("brep");
        return registry->find(request.formatKey)->read(request);
    };
    const int stateBaseline = repository->modelCount();
    auto staged = readTransaction();
    const QString stagedKey = staged.geometry ? staged.geometry->adapter.modelKey : QString();
    const auto stagedRollback = staged.transaction ? staged.transaction->rollback() : Common::OperationResult{};
    const auto stagedPublishAfterRollback = staged.transaction ? staged.transaction->publish() : Common::OperationResult{};
    suite.expect(staged.transaction && stagedRollback.succeeded() && !repository->contains(stagedKey) &&
                     !stagedPublishAfterRollback.succeeded() && repository->modelCount() == stateBaseline,
                 QStringLiteral("staged transaction rolls back idempotently and rejects publish after rollback"));

    auto published = readTransaction();
    const QString publishedKey = published.geometry ? published.geometry->adapter.modelKey : QString();
    const auto publishOnce = published.transaction ? published.transaction->publish() : Common::OperationResult{};
    const auto publishTwice = published.transaction ? published.transaction->publish() : Common::OperationResult{};
    const auto publishedRollback = published.transaction ? published.transaction->rollback() : Common::OperationResult{};
    const auto publishedRollbackTwice = published.transaction ? published.transaction->rollback() : Common::OperationResult{};
    suite.expect(published.transaction && publishOnce.succeeded() && !publishTwice.succeeded() &&
                     publishedRollback.succeeded() && publishedRollbackTwice.succeeded() &&
                     !repository->contains(publishedKey) && repository->modelCount() == stateBaseline,
                 QStringLiteral("published transaction retains unique rollback ownership until finalization"));

    auto finalized = readTransaction();
    const QString finalizedKey = finalized.geometry ? finalized.geometry->adapter.modelKey : QString();
    const bool finalizedPublished = finalized.transaction && finalized.transaction->publish().succeeded();
    if (finalizedPublished)
    {
        finalized.transaction->finalize();
        finalized.transaction->finalize();
    }
    const auto finalizedRollback = finalized.transaction ? finalized.transaction->rollback() : Common::OperationResult{};
    suite.expect(finalizedPublished && finalizedRollback.succeeded() && repository->contains(finalizedKey) &&
                     repository->modelCount() == stateBaseline + 1,
                 QStringLiteral("finalized transaction never rolls back its committed FITK model"));
    finalized.transaction.reset();
    suite.expect(repository->contains(finalizedKey),
                 QStringLiteral("finalized transaction destruction preserves the committed FITK model"));
    repository->release(finalizedKey);

    Model::ApplicationRuntime failedRuntime(
        1, [](Model::ObjectId, Model::DataObjectType) {
            Common::OperationResult result;
            result.add({QStringLiteral("test"), QStringLiteral("TEST-COMMIT-FAIL"),
                        QStringLiteral("Controlled APPMesh commit failure."), QString(), false,
                        QString(), QStringLiteral("test"), QString(), QString()});
            return result;
        });
    Model::GeometryManager failedManager(failedRuntime);
    OperatorsModel::ImportGeometryOperator operation(failedManager, registry);
    const QString rollbackPath = temporary.filePath(QStringLiteral("rollback.brep"));
    createFixture(rollbackPath);
    const auto failed = Operators::executeSafely(
        operation, input(rollbackPath, temporary.path(), QStringLiteral("brep")), 91);
    suite.expect(!failed.succeeded() && failedManager.objectCount() == 0 &&
                     repository->modelCount() == preserved,
                 QStringLiteral("APPMesh commit failure rolls back staged FITK ownership"));

    const QString concurrentPath = temporary.filePath(QStringLiteral("concurrent.brep"));
    createFixture(concurrentPath);
    std::vector<std::future<bool>> imports;
    for (int index = 0; index < 4; ++index)
    {
        imports.emplace_back(std::async(std::launch::async, [registry, repository, concurrentPath] {
            OperatorsModel::GeometryReadRequest request;
            request.filePath = concurrentPath;
            request.formatKey = QStringLiteral("brep");
            auto read = registry->find(request.formatKey)->read(request);
            return read.succeeded() && read.geometry && read.transaction &&
                   read.transaction->publish().succeeded() &&
                   (read.transaction->finalize(), true) &&
                   repository->contains(read.geometry->adapter.modelKey);
        }));
    }
    bool concurrentSuccess = true;
    for (auto& import : imports)
        concurrentSuccess = import.get() && concurrentSuccess;
    suite.expect(concurrentSuccess && repository->modelCount() == preserved + 4,
                 QStringLiteral("concurrent requests are serialized at the OCC boundary and commit uniquely"));

    Model::ApplicationRuntime removalRuntime;
    Model::GeometryManager removalManager(removalRuntime);
    removalManager.setExternalResourceReleaser(
        [repository](const Model::GeometryObject& object) {
            repository->release(object.adapter.modelKey);
        });
    OperatorsModel::ImportGeometryOperator removalOperator(removalManager, registry);
    const int beforeRemoval = repository->modelCount();
    const auto removable = Operators::executeSafely(
        removalOperator, input(concurrentPath, temporary.path(), QStringLiteral("brep")), 92);
    suite.expect(removable.succeeded() && removable.result() &&
                     repository->modelCount() == beforeRemoval + 1 &&
                     removalManager.removeGeometry(removable.result()->objectId).succeeded() &&
                     repository->modelCount() == beforeRemoval,
                 QStringLiteral("geometry deletion releases its committed FITK model"));

    const int beforeManagerDestruction = repository->modelCount();
    {
        Model::ApplicationRuntime destructionRuntime;
        Model::GeometryManager destructionManager(destructionRuntime);
        destructionManager.setExternalResourceReleaser(
            [repository](const Model::GeometryObject& object) {
                repository->release(object.adapter.modelKey);
            });
        OperatorsModel::ImportGeometryOperator destructionOperator(destructionManager, registry);
        const auto retained = Operators::executeSafely(
            destructionOperator, input(concurrentPath, temporary.path(), QStringLiteral("brep")), 93);
        suite.expect(retained.succeeded() && retained.result() &&
                         repository->modelCount() == beforeManagerDestruction + 1,
                     QStringLiteral("geometry manager owns a finalized FITK model before destruction"));
    }
    suite.expect(repository->modelCount() == beforeManagerDestruction,
                 QStringLiteral("geometry manager destruction releases finalized FITK models"));

    return suite.result();
}
