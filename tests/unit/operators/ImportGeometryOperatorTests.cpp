#include "operators/OperatorsModel/ImportGeometryOperator.h"
#include "TestSupport.h"

#include <QCoreApplication>
#include <QFile>
#include <QTemporaryDir>

#include <stdexcept>

namespace
{
using namespace AppMesh;
using namespace AppMesh::Operators;
using namespace AppMesh::OperatorsModel;

Model::GeometryObject validGeometry()
{
    Model::GeometryObject geometry;
    geometry.type = Model::GeometryType::BRep;
    geometry.adapter.providerKey = QStringLiteral("fake-reader");
    geometry.adapter.modelKey = QStringLiteral("staged-model");
    geometry.topology.append({1, Model::TopologyEntityType::Solid,
                              QStringLiteral("solid/1"), {}});
    return geometry;
}

class FakeReader final : public IGeometryReader
{
public:
    enum class Mode { Success, Failure, Empty, Invalid, ThrowStandard, ThrowUnknown };
    explicit FakeReader(Mode value = Mode::Success) : mode(value) {}
    QString key() const override { return QStringLiteral("fake"); }
    QStringList supportedExtensions() const override { return {QStringLiteral("fake")}; }
    GeometryReadResult read(const GeometryReadRequest&) const override
    {
        GeometryReadResult result;
        if (mode == Mode::Failure)
        {
            result.add({QStringLiteral("io"), QStringLiteral("GEO-FAKE-READ-FAILED"),
                        QStringLiteral("The fake reader failed."), QStringLiteral("Controlled failure."),
                        false, QStringLiteral("file"), QStringLiteral("fake.read"), {}, {}});
        }
        else if (mode == Mode::Success)
        {
            result.geometry = validGeometry();
        }
        else if (mode == Mode::Invalid)
        {
            result.geometry = Model::GeometryObject{};
        }
        else if (mode == Mode::ThrowStandard)
        {
            throw std::runtime_error("controlled reader exception");
        }
        else if (mode == Mode::ThrowUnknown)
        {
            throw 7;
        }
        return result;
    }
    Mode mode;
};

struct TransactionState
{
    bool publishFails = false;
    bool rollbackFails = false;
    bool publishThrows = false;
    int publishCalls = 0;
    int finalizeCalls = 0;
    int rollbackCalls = 0;
};

class FakeTransaction final : public GeometryReadResult::Transaction
{
public:
    explicit FakeTransaction(std::shared_ptr<TransactionState> state) : m_state(std::move(state)) {}

    Common::OperationResult publish() override
    {
        ++m_state->publishCalls;
        if (m_state->publishThrows)
            throw std::runtime_error("controlled publish exception");
        Common::OperationResult result;
        if (m_state->publishFails)
        {
            result.add({QStringLiteral("io"), QStringLiteral("TEST-FITK-PUBLISH-FAIL"),
                        QStringLiteral("Controlled FITK publish failure."), QStringLiteral("Before APPMesh."),
                        false, {}, QStringLiteral("test.transaction"), {}, {}});
        }
        return result;
    }

    void finalize() noexcept override { ++m_state->finalizeCalls; }

    Common::OperationResult rollback() override
    {
        ++m_state->rollbackCalls;
        Common::OperationResult result;
        if (m_state->rollbackFails)
        {
            result.add({QStringLiteral("io"), QStringLiteral("TEST-FITK-ROLLBACK-FAIL"),
                        QStringLiteral("Controlled FITK rollback failure."), QStringLiteral("Appended only."),
                        false, {}, QStringLiteral("test.transaction"), {}, {}});
        }
        return result;
    }

private:
    std::shared_ptr<TransactionState> m_state;
};

class TransactionReader final : public IGeometryReader
{
public:
    explicit TransactionReader(std::shared_ptr<TransactionState> state) : m_state(std::move(state)) {}
    QString key() const override { return QStringLiteral("fake"); }
    QStringList supportedExtensions() const override { return {QStringLiteral("fake")}; }
    GeometryReadResult read(const GeometryReadRequest&) const override
    {
        GeometryReadResult result;
        result.geometry = validGeometry();
        result.transaction = std::make_shared<FakeTransaction>(m_state);
        return result;
    }

private:
    std::shared_ptr<TransactionState> m_state;
};

class RejectingRemovalConstraint final : public Model::GeometryRemovalConstraint
{
public:
    Common::OperationResult beginGeometryRemoval(Model::ObjectId) override
    {
        ++beginCalls;
        Common::OperationResult result;
        result.add({QStringLiteral("test"), QStringLiteral("TEST-REMOVAL-REJECTED"),
                    QStringLiteral("Geometry deletion is deliberately rejected."), QString(), false,
                    {}, QStringLiteral("test.constraint"), {}, {}});
        return result;
    }
    void cancelGeometryRemoval(Model::ObjectId) noexcept override {}
    void completeGeometryRemoval(Model::ObjectId) noexcept override {}
    int beginCalls = 0;
};

OperatorInput makeInput(const QString& filePath,
                        const QString& workDirectory,
                        const QString& requestedName = QStringLiteral("Imported"))
{
    OperatorInput input;
    input.operationKey = ImportGeometryOperator::operationKey();
    input.parameters.insert(ImportGeometryOperator::filePathParameter(), filePath);
    input.parameters.insert(ImportGeometryOperator::workDirectoryParameter(), workDirectory);
    input.parameters.insert(ImportGeometryOperator::formatKeyParameter(), QStringLiteral("fake"));
    input.parameters.insert(ImportGeometryOperator::requestedNameParameter(), requestedName);
    input.parameters.insert(ImportGeometryOperator::metadataParameter(),
                            QVariantMap{{QStringLiteral("source"), QStringLiteral("test")}});
    return input;
}

std::shared_ptr<GeometryReaderRegistry> registryWith(FakeReader::Mode mode)
{
    auto registry = std::make_shared<GeometryReaderRegistry>();
    registry->registerReader(std::make_shared<FakeReader>(mode));
    return registry;
}

std::shared_ptr<GeometryReaderRegistry> registryWithTransaction(
    const std::shared_ptr<TransactionState>& state)
{
    auto registry = std::make_shared<GeometryReaderRegistry>();
    registry->registerReader(std::make_shared<TransactionReader>(state));
    return registry;
}
}

int main(int argc, char* argv[])
{
    QCoreApplication application(argc, argv);
    TestSuite suite;
    QTemporaryDir temporary;
    suite.expect(temporary.isValid(), QStringLiteral("temporary import directory is available"));
    const QString filePath = temporary.filePath(QStringLiteral("part.fake"));
    QFile file(filePath);
    file.open(QIODevice::WriteOnly);
    file.write("fake");
    file.close();

    Model::ApplicationRuntime runtime;
    Model::GeometryManager manager(runtime);
    auto readers = registryWith(FakeReader::Mode::Success);
    ImportGeometryOperator operation(manager, readers);
    const auto success = executeSafely(operation, makeInput(filePath, temporary.path()), 1);
    suite.expect(success.succeeded() && success.result() &&
                     success.result()->objectId != Model::InvalidObjectId &&
                     manager.objectCount() == 1 && runtime.objectCount() == 1 &&
                     manager.validateIndexes().succeeded(),
                 QStringLiteral("valid fake reader output commits atomically through GeometryManager"));

    const auto duplicateName = executeSafely(operation,
                                              makeInput(filePath, temporary.path()), 2);
    const auto allGeometry = manager.snapshots();
    suite.expect(duplicateName.succeeded() && allGeometry.size() == 2 &&
                     allGeometry[0].common.name == QStringLiteral("Imported") &&
                     allGeometry[1].common.name == QStringLiteral("Imported (2)"),
                 QStringLiteral("name conflicts use the Runtime unique-name protocol"));

    const int preservedCount = manager.objectCount();
    auto failureReaders = registryWith(FakeReader::Mode::Failure);
    ImportGeometryOperator failedRead(manager, failureReaders);
    const auto readFailure = executeSafely(failedRead,
                                           makeInput(filePath, temporary.path(), QStringLiteral("Failed")),
                                           3);
    suite.expect(!readFailure.succeeded() && readFailure.error()->taskId == 3 &&
                     manager.objectCount() == preservedCount &&
                     manager.validateIndexes().succeeded(),
                 QStringLiteral("reader failure preserves all existing geometry and indexes"));

    for (const auto mode : {FakeReader::Mode::Empty,
                            FakeReader::Mode::Invalid,
                            FakeReader::Mode::ThrowStandard,
                            FakeReader::Mode::ThrowUnknown})
    {
        ImportGeometryOperator failing(manager, registryWith(mode));
        const auto result = executeSafely(
            failing, makeInput(filePath, temporary.path(), QStringLiteral("Rejected")), 4);
        suite.expect(!result.succeeded() && manager.objectCount() == preservedCount &&
                         manager.validateIndexes().succeeded(),
                     QStringLiteral("empty, invalid, and exceptional reader outputs leave no residue"));
    }

    OperatorInput emptyPath = makeInput(filePath, temporary.path());
    emptyPath.parameters[ImportGeometryOperator::filePathParameter()] = QString();
    OperatorInput missingPath = makeInput(temporary.filePath(QStringLiteral("missing.fake")),
                                          temporary.path());
    OperatorInput directoryPath = makeInput(temporary.path(), temporary.path());
    OperatorInput unsupported = makeInput(filePath, temporary.path());
    unsupported.parameters[ImportGeometryOperator::formatKeyParameter()] = QStringLiteral("missing");
    OperatorInput wrongType = makeInput(filePath, temporary.path());
    wrongType.parameters[ImportGeometryOperator::filePathParameter()] = 42;
    suite.expect(!operation.validate(emptyPath).succeeded() &&
                     !operation.validate(missingPath).succeeded() &&
                     !operation.validate(directoryPath).succeeded() &&
                     !operation.validate(unsupported).succeeded() &&
                     !operation.validate(wrongType).succeeded(),
                 QStringLiteral("path, reader, extension, and parameter-type validation rejects bad input"));

    auto duplicateRegistry = std::make_shared<GeometryReaderRegistry>();
    const auto firstRegistration = duplicateRegistry->registerReader(
        std::make_shared<FakeReader>());
    const auto duplicateRegistration = duplicateRegistry->registerReader(
        std::make_shared<FakeReader>());
    suite.expect(firstRegistration.succeeded() && !duplicateRegistration.succeeded() &&
                     duplicateRegistry->readerCount() == 1,
                 QStringLiteral("reader keys are unique and never silently overwritten"));

    Model::ApplicationRuntime publishFailureRuntime(
        1,
        [](Model::ObjectId, Model::DataObjectType) {
            Common::OperationResult result;
            result.add({QStringLiteral("test"), QStringLiteral("TEST-PUBLISH-FAIL"),
                        QStringLiteral("Controlled publish failure"), QStringLiteral("Rollback expected."),
                        false, {}, QStringLiteral("test.publish"), {}, {}});
            return result;
        });
    Model::GeometryManager publishFailureManager(publishFailureRuntime);
    ImportGeometryOperator publishFailure(publishFailureManager,
                                          registryWith(FakeReader::Mode::Success));
    const auto rolledBack = executeSafely(
        publishFailure, makeInput(filePath, temporary.path(), QStringLiteral("Rollback")), 5);
    suite.expect(!rolledBack.succeeded() && publishFailureManager.objectCount() == 0 &&
                     publishFailureRuntime.objectCount() == 0 &&
                     publishFailureManager.validateIndexes().succeeded(),
                 QStringLiteral("publish failure cancels reservations, payloads, and name claims"));

    auto transactionPublishFailure = std::make_shared<TransactionState>();
    transactionPublishFailure->publishFails = true;
    Model::ApplicationRuntime transactionRuntime;
    Model::GeometryManager transactionManager(transactionRuntime);
    auto rejectingConstraint = std::make_shared<RejectingRemovalConstraint>();
    transactionManager.setRemovalConstraint(rejectingConstraint);
    ImportGeometryOperator transactionFailure(transactionManager,
                                              registryWithTransaction(transactionPublishFailure));
    const auto publishFirst = executeSafely(
        transactionFailure, makeInput(filePath, temporary.path(), QStringLiteral("PublishFirst")), 6);
    suite.expect(!publishFirst.succeeded() && transactionManager.objectCount() == 0 &&
                     transactionRuntime.objectCount() == 0 &&
                     transactionPublishFailure->publishCalls == 1 &&
                     transactionPublishFailure->rollbackCalls == 1 &&
                     rejectingConstraint->beginCalls == 0 && publishFirst.error() &&
                     publishFirst.error()->code == QStringLiteral("TEST-FITK-PUBLISH-FAIL"),
                 QStringLiteral("FITK publish failure occurs before APPMesh creation and never invokes deletion compensation"));

    auto transactionPublishException = std::make_shared<TransactionState>();
    transactionPublishException->publishThrows = true;
    Model::ApplicationRuntime transactionExceptionRuntime;
    Model::GeometryManager transactionExceptionManager(transactionExceptionRuntime);
    ImportGeometryOperator transactionException(
        transactionExceptionManager, registryWithTransaction(transactionPublishException));
    const auto publishException = executeSafely(
        transactionException, makeInput(filePath, temporary.path(), QStringLiteral("PublishException")), 61);
    suite.expect(!publishException.succeeded() && transactionExceptionManager.objectCount() == 0 &&
                     transactionExceptionRuntime.objectCount() == 0 &&
                     transactionPublishException->rollbackCalls == 1 && publishException.error() &&
                     publishException.error()->code == QStringLiteral("GEO-IMPORT-TRANSACTION-PUBLISH-EXCEPTION"),
                 QStringLiteral("transaction publish exceptions are isolated before APPMesh creation"));

    auto transactionCommitFailure = std::make_shared<TransactionState>();
    transactionCommitFailure->rollbackFails = true;
    Model::ApplicationRuntime transactionCommitRuntime(
        1, [](Model::ObjectId, Model::DataObjectType) {
            Common::OperationResult result;
            result.add({QStringLiteral("test"), QStringLiteral("TEST-APPMESH-COMMIT-FAIL"),
                        QStringLiteral("Controlled APPMesh commit failure."), QString(), false,
                        {}, QStringLiteral("test.publish"), {}, {}});
            return result;
        });
    Model::GeometryManager transactionCommitManager(transactionCommitRuntime);
    ImportGeometryOperator transactionCommitFailureOperation(
        transactionCommitManager, registryWithTransaction(transactionCommitFailure));
    const auto transactionCommitFailureResult = executeSafely(
        transactionCommitFailureOperation,
        makeInput(filePath, temporary.path(), QStringLiteral("CommitFirst")), 7);
    suite.expect(!transactionCommitFailureResult.succeeded() &&
                     transactionCommitManager.objectCount() == 0 &&
                     transactionCommitRuntime.objectCount() == 0 &&
                     transactionCommitFailure->publishCalls == 1 &&
                     transactionCommitFailure->rollbackCalls == 1 &&
                     transactionCommitFailure->finalizeCalls == 0 &&
                     transactionCommitFailureResult.error() &&
                     transactionCommitFailureResult.error()->code == QStringLiteral("TEST-APPMESH-COMMIT-FAIL") &&
                     transactionCommitFailureResult.diagnostics().size() >= 2 &&
                     transactionCommitFailureResult.diagnostics()[1].code == QStringLiteral("TEST-FITK-ROLLBACK-FAIL"),
                 QStringLiteral("APPMesh commit failure remains primary while FITK rollback diagnostics append"));

    auto transactionSuccess = std::make_shared<TransactionState>();
    Model::ApplicationRuntime transactionSuccessRuntime;
    Model::GeometryManager transactionSuccessManager(transactionSuccessRuntime);
    ImportGeometryOperator transactionSuccessOperation(
        transactionSuccessManager, registryWithTransaction(transactionSuccess));
    const auto transactionSuccessResult = executeSafely(
        transactionSuccessOperation,
        makeInput(filePath, temporary.path(), QStringLiteral("Finalized")), 8);
    suite.expect(transactionSuccessResult.succeeded() && transactionSuccess->publishCalls == 1 &&
                     transactionSuccess->finalizeCalls == 1 && transactionSuccess->rollbackCalls == 0,
                 QStringLiteral("successful import finalizes transaction only after APPMesh publication"));

    return suite.result();
}
