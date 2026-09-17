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

    return suite.result();
}
