#include "GeometryIO.h"

#include "FITK_Component/FITKGeoCompOCC/FITKAbstractOCCModel.h"
#include "FITK_Component/FITKGeoCompOCC/FITKOCCModelImport.h"
#include "FITK_Interface/FITKInterfaceModel/FITKModelEnum.h"

#include <QFileInfo>
#include <QMutex>
#include <QMutexLocker>
#include <QReadLocker>
#include <QSet>
#include <QWriteLocker>

#include <map>
#include <mutex>
#include <exception>

namespace AppMesh::GeometryIO
{
namespace
{
QMutex s_occImportMutex;

Common::Diagnostic diagnostic(const QString& code,
                              const QString& message,
                              const QString& detail,
                              const QString& path = {})
{
    return {QStringLiteral("io"), code, message, detail, false, path,
            QStringLiteral("geometry.io.fitk-occ"), QString(), QString()};
}

QString normalized(QString value)
{
    value = value.trimmed().toCaseFolded();
    while (value.startsWith(QLatin1Char('.')))
        value.remove(0, 1);
    return value;
}

struct ImportedModel
{
    std::unique_ptr<OCC::FITKOCCModelImport> command;
};
}

class FITKGeometryRepository::Private
{
public:
    mutable QReadWriteLock lock;
    std::map<QString, ImportedModel> models;
};

class FITKGeometryReadTransaction final
    : public OperatorsModel::GeometryReadResult::Transaction
{
public:
    FITKGeometryReadTransaction(std::shared_ptr<FITKGeometryRepository> repository,
                                QString modelKey,
                                std::unique_ptr<OCC::FITKOCCModelImport> command)
        : m_repository(std::move(repository)), m_modelKey(std::move(modelKey)),
          m_command(std::move(command)) {}

    ~FITKGeometryReadTransaction() override { rollback(); }

    Common::OperationResult publish() override
    {
        Common::OperationResult result;
        if (m_state == State::Published || m_state == State::Finalized)
        {
            result.add(diagnostic(QStringLiteral("GEO-IO-TRANSACTION-PUBLISH-REPEATED"),
                                  QStringLiteral("The FITK geometry transaction is already published."),
                                  m_modelKey));
            return result;
        }
        if (m_state == State::RolledBack)
        {
            result.add(diagnostic(QStringLiteral("GEO-IO-TRANSACTION-PUBLISH-AFTER-ROLLBACK"),
                                  QStringLiteral("A rolled back FITK geometry cannot be published."),
                                  m_modelKey));
            return result;
        }
        if (m_state != State::Staged)
            return result;
        if (!m_repository || !m_command)
        {
            result.add(diagnostic(QStringLiteral("GEO-IO-TRANSACTION-INVALID"),
                                  QStringLiteral("The staged FITK geometry is unavailable."),
                                  m_modelKey));
            return result;
        }
        QWriteLocker locker(&m_repository->d->lock);
        if (m_repository->d->models.find(m_modelKey) != m_repository->d->models.end())
        {
            result.add(diagnostic(QStringLiteral("GEO-IO-MODEL-KEY-DUPLICATE"),
                                  QStringLiteral("The FITK model key is already committed."),
                                  m_modelKey));
            return result;
        }
        try
        {
            m_repository->d->models.emplace(m_modelKey, ImportedModel{std::move(m_command)});
        }
        catch (const std::exception& exception)
        {
            result.add(diagnostic(QStringLiteral("GEO-IO-COMMIT-FAILED"),
                                  QStringLiteral("The staged FITK geometry could not be committed."),
                                  QString::fromLocal8Bit(exception.what())));
            return result;
        }
        m_state = State::Published;
        return result;
    }

    void finalize() noexcept override
    {
        if (m_state == State::Published)
            m_state = State::Finalized;
    }

    Common::OperationResult rollback() override
    {
        Common::OperationResult result;
        if (m_state == State::RolledBack || m_state == State::Finalized)
            return result;
        if (m_state == State::Published && m_repository)
        {
            // This transaction owns the unpublished-to-APPMesh repository entry.
            m_repository->release(m_modelKey);
        }
        m_command.reset();
        m_state = State::RolledBack;
        return result;
    }

private:
    enum class State { Staged, Published, Finalized, RolledBack };
    std::shared_ptr<FITKGeometryRepository> m_repository;
    QString m_modelKey;
    std::unique_ptr<OCC::FITKOCCModelImport> m_command;
    State m_state = State::Staged;
};

QVector<GeometryFormatCapability> firstReleaseGeometryCapabilities()
{
    return {{QStringLiteral("brep"), {}, {QStringLiteral("brep")}, Model::GeometryType::BRep, true, QStringLiteral("fitk.occ")},
            {QStringLiteral("step"), {QStringLiteral("stp")}, {QStringLiteral("step"), QStringLiteral("stp")}, Model::GeometryType::Step, true, QStringLiteral("fitk.occ")},
            {QStringLiteral("iges"), {QStringLiteral("igs")}, {QStringLiteral("iges"), QStringLiteral("igs")}, Model::GeometryType::Iges, true, QStringLiteral("fitk.occ")}};
}

Common::OperationResult validateGeometryCapabilities(
    const QVector<GeometryFormatCapability>& capabilities)
{
    Common::OperationResult result;
    QSet<QString> keys;
    QSet<QString> extensions;
    for (const auto& capability : capabilities)
    {
        if (!capability.supportsRead || capability.providerKey.trimmed().isEmpty() ||
            !Model::isValidGeometryType(capability.geometryType))
        {
            result.add(diagnostic(QStringLiteral("GEO-IO-CAPABILITY-INVALID"),
                                  QStringLiteral("A geometry format capability is incomplete or unreadable."),
                                  capability.key));
        }
        QStringList aliases = capability.aliases;
        aliases.append(capability.key);
        for (const auto& raw : aliases)
        {
            const QString key = normalized(raw);
            if (key.isEmpty() || keys.contains(key))
                result.add(diagnostic(QStringLiteral("GEO-IO-CAPABILITY-KEY-CONFLICT"),
                                      QStringLiteral("A geometry format key or alias is empty or duplicated."), raw));
            else
                keys.insert(key);
        }
        for (const auto& raw : capability.extensions)
        {
            const QString extension = normalized(raw);
            if (extension.isEmpty() || extensions.contains(extension))
                result.add(diagnostic(QStringLiteral("GEO-IO-CAPABILITY-EXTENSION-CONFLICT"),
                                      QStringLiteral("A geometry extension is empty or duplicated."), raw));
            else
                extensions.insert(extension);
        }
    }
    return result;
}

FITKGeometryRepository::FITKGeometryRepository() : d(new Private) {}
FITKGeometryRepository::~FITKGeometryRepository() = default;

bool FITKGeometryRepository::contains(const QString& modelKey) const
{
    QReadLocker locker(&d->lock);
    return d->models.find(modelKey) != d->models.end();
}

int FITKGeometryRepository::modelCount() const
{
    QReadLocker locker(&d->lock);
    return static_cast<int>(d->models.size());
}

void FITKGeometryRepository::release(const QString& modelKey) noexcept
{
    QWriteLocker locker(&d->lock);
    d->models.erase(modelKey);
}

FITKOCCGeometryReader::FITKOCCGeometryReader(
    GeometryFormatCapability capability,
    std::shared_ptr<FITKGeometryRepository> repository)
    : m_capability(std::move(capability)), m_repository(std::move(repository)) {}

QString FITKOCCGeometryReader::key() const { return m_capability.key; }
QStringList FITKOCCGeometryReader::aliases() const { return m_capability.aliases; }
QStringList FITKOCCGeometryReader::supportedExtensions() const { return m_capability.extensions; }

OperatorsModel::GeometryReadResult FITKOCCGeometryReader::read(
    const OperatorsModel::GeometryReadRequest& request) const
{
    OperatorsModel::GeometryReadResult result;
    const QFileInfo file(request.filePath);
    if (!file.exists() || !file.isFile())
    {
        result.add(diagnostic(QStringLiteral("GEO-IO-FILE-INVALID"),
                              QStringLiteral("The geometry file is missing or not a regular file."),
                              QStringLiteral("FITK/OCC import was not started."), request.filePath));
        return result;
    }
    if (file.size() == 0)
    {
        result.add(diagnostic(QStringLiteral("GEO-IO-FILE-EMPTY"),
                              QStringLiteral("The geometry file is empty."),
                              QStringLiteral("FITK/OCC import was not started."), request.filePath));
        return result;
    }
    if (!m_repository)
    {
        result.add(diagnostic(QStringLiteral("GEO-IO-REPOSITORY-MISSING"),
                              QStringLiteral("The FITK geometry repository is unavailable."),
                              QStringLiteral("Register the production geometry lifetime boundary.")));
        return result;
    }

    auto command = std::make_unique<OCC::FITKOCCModelImport>();
    command->setFileName(file.absoluteFilePath());
    bool imported = false;
    {
        QMutexLocker locker(&s_occImportMutex);
        imported = command->update();
    }
    if (!imported)
    {
        result.add(diagnostic(QStringLiteral("GEO-IO-OCC-READ-FAILED"),
                              QStringLiteral("FITK/OCC could not read the geometry file."),
                              QStringLiteral("The content may be corrupt, empty, or incompatible with its extension."),
                              request.filePath));
        return result;
    }
    auto* shape = command->getTShapeAgent<OCC::FITKAbstractOCCModel>();
    if (!shape || !shape->getShape())
    {
        result.add(diagnostic(QStringLiteral("GEO-IO-OCC-SHAPE-EMPTY"),
                              QStringLiteral("FITK/OCC returned no geometry shape."),
                              request.filePath, request.filePath));
        return result;
    }

    Model::GeometryObject geometry;
    geometry.type = m_capability.geometryType;
    geometry.adapter.providerKey = m_capability.providerKey;
    geometry.adapter.modelKey = QStringLiteral("fitk-occ:%1").arg(command->getDataObjectID());
    geometry.adapter.metadata.insert(QStringLiteral("format"), m_capability.key);
    const struct { Interface::FITKModelEnum::FITKModelSetType fitk; Model::TopologyEntityType app; const char* key; } types[] = {
        {Interface::FITKModelEnum::FMSPoint, Model::TopologyEntityType::Vertex, "vertex"},
        {Interface::FITKModelEnum::FMSEdge, Model::TopologyEntityType::Edge, "edge"},
        {Interface::FITKModelEnum::FMSSurface, Model::TopologyEntityType::Face, "face"},
        {Interface::FITKModelEnum::FMSSolid, Model::TopologyEntityType::Solid, "solid"}};
    Model::GeometryEntityId nextId = 1;
    for (const auto& type : types)
    {
        const int count = shape->getShapeCount(type.fitk);
        for (int index = 0; index < count; ++index)
        {
            Model::GeometryTopologyEntity entity;
            entity.id = nextId++;
            entity.type = type.app;
            entity.adapterEntityKey = QStringLiteral("%1/%2:%3")
                .arg(geometry.adapter.modelKey, QString::fromLatin1(type.key))
                .arg(index);
            geometry.topology.append(std::move(entity));
        }
    }
    if (geometry.topology.isEmpty())
    {
        result.add(diagnostic(QStringLiteral("GEO-IO-OCC-TOPOLOGY-EMPTY"),
                              QStringLiteral("FITK/OCC returned no supported topology."),
                              request.filePath, request.filePath));
        return result;
    }
    const auto geometryValidation = Model::validateGeometryObject(geometry);
    if (!geometryValidation.succeeded())
    {
        for (auto item : geometryValidation.diagnostics)
        {
            item.stage = QStringLiteral("geometry.io.fitk-occ.topology");
            item.path = request.filePath;
            result.add(std::move(item));
        }
        return result;
    }
    result.geometry = geometry;
    result.transaction = std::make_shared<FITKGeometryReadTransaction>(
        m_repository, geometry.adapter.modelKey, std::move(command));
    return result;
}

Common::OperationResult registerFirstReleaseGeometryReaders(
    OperatorsModel::GeometryReaderRegistry& registry,
    const std::shared_ptr<FITKGeometryRepository>& repository)
{
    auto validation = validateGeometryCapabilities(firstReleaseGeometryCapabilities());
    if (!validation.succeeded())
        return validation;
    for (const auto& capability : firstReleaseGeometryCapabilities())
    {
        auto registration = registry.registerReader(
            std::make_shared<FITKOCCGeometryReader>(capability, repository));
        for (auto item : registration.diagnostics)
            validation.add(std::move(item));
    }
    return validation;
}
}
