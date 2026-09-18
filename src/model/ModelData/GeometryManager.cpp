#include "GeometryManager.h"

#include <QReadLocker>
#include <QWriteLocker>

#include <exception>
#include <new>
#include <set>
#include <utility>

namespace AppMesh::Model
{
StagedGeometry::StagedGeometry(GeometryObject geometry)
    : m_geometry(std::move(geometry))
{
}

GeometryObject StagedGeometry::snapshot() const
{
    return m_geometry;
}

GeometryManager::GeometryManager(ApplicationRuntime& runtime,
                                 BeforePublishCheckpoint beforePublish)
    : m_runtime(runtime), m_beforePublish(std::move(beforePublish))
{
}

GeometryManager::~GeometryManager()
{
    std::function<void(const GeometryObject&)> releaser;
    {
        std::lock_guard<std::mutex> locker(m_externalResourceMutex);
        releaser = m_externalResourceReleaser;
    }
    if (!releaser)
        return;
    QVector<GeometryObject> resources;
    {
        QReadLocker locker(&m_lock);
        resources.reserve(static_cast<int>(m_payloads.size()));
        for (const auto& payload : m_payloads)
            resources.append(payload.second);
    }
    for (const auto& resource : resources)
        try { releaser(resource); } catch (...) {}
}

Common::Diagnostic GeometryManager::diagnostic(const QString& code,
                                               const QString& message,
                                               const QString& detail,
                                               ObjectId objectId,
                                               const QString& path) const
{
    return {QStringLiteral("model"),
            code,
            message,
            detail,
            false,
            path,
            QStringLiteral("geometry.manager"),
            objectId == InvalidObjectId ? QString() : QString::number(objectId)};
}

void GeometryManager::appendDiagnostics(Common::OperationResult& target,
                                        const Common::OperationResult& source) const
{
    for (const auto& item : source.diagnostics)
    {
        target.add(item);
    }
}

StageGeometryResult GeometryManager::stageGeometry(const GeometryObject& geometry) const
{
    StageGeometryResult result;
    appendDiagnostics(result, validateGeometryObject(geometry));
    if (result.succeeded())
    {
        result.staged = StagedGeometry(geometry);
    }
    return result;
}

CommitGeometryResult GeometryManager::commitGeometry(const CreateObjectRequest& common,
                                                     const StagedGeometry& staged)
{
    CommitGeometryResult result;
    appendDiagnostics(result, validateGeometryObject(staged.m_geometry));
    if (!result.succeeded())
    {
        return result;
    }

    CreateObjectRequest request = common;
    request.type = DataObjectType::Geometry;
    auto reserved = m_runtime.reserveDomainObject(request);
    appendDiagnostics(result, reserved);
    if (!reserved.succeeded() || !reserved.reservation.has_value())
    {
        return result;
    }

    auto& reservation = *reserved.reservation;
    const ObjectId id = reservation.id();
    try
    {
        QWriteLocker locker(&m_lock);
        const auto inserted = m_payloads.emplace(id, staged.m_geometry);
        if (!inserted.second)
        {
            result.add(diagnostic(QStringLiteral("GEO-PAYLOAD-ID-CONFLICT"),
                                  QStringLiteral("Geometry payload ID is already present."),
                                  QStringLiteral("The hidden common reservation will be cancelled."),
                                  id));
        }
    }
    catch (const std::bad_alloc&)
    {
        result.add(diagnostic(QStringLiteral("GEO-RESOURCE-EXHAUSTED"),
                              QStringLiteral("Geometry payload could not be committed."),
                              QStringLiteral("No geometry payload was retained."),
                              id));
    }

    if (!result.succeeded())
    {
        const auto cancelled = m_runtime.cancelDomainObject(reservation);
        appendDiagnostics(result, cancelled);
        return result;
    }

    if (m_beforePublish)
    {
        try
        {
            appendDiagnostics(result, m_beforePublish(id));
        }
        catch (const std::exception& exception)
        {
            result.add(diagnostic(QStringLiteral("GEO-COMMIT-CHECKPOINT-FAILED"),
                                  QStringLiteral("Geometry commit checkpoint failed."),
                                  QString::fromLocal8Bit(exception.what()),
                                  id));
        }
        catch (...)
        {
            result.add(diagnostic(QStringLiteral("GEO-COMMIT-CHECKPOINT-FAILED"),
                                  QStringLiteral("Geometry commit checkpoint failed."),
                                  QStringLiteral("The checkpoint raised an unknown exception."),
                                  id));
        }
        if (!result.succeeded())
        {
            {
                QWriteLocker locker(&m_lock);
                m_payloads.erase(id);
            }
            const auto cancelled = m_runtime.cancelDomainObject(reservation);
            appendDiagnostics(result, cancelled);
            return result;
        }
    }

    auto published = m_runtime.publishDomainObject(reservation);
    appendDiagnostics(result, published);
    if (!published.succeeded() || !published.object.has_value())
    {
        {
            QWriteLocker locker(&m_lock);
            m_payloads.erase(id);
        }
        if (reservation.isValid())
        {
            const auto cancelled = m_runtime.cancelDomainObject(reservation);
            appendDiagnostics(result, cancelled);
        }
        return result;
    }

    result.object = makeGeometrySnapshot(*published.object, staged.m_geometry);
    return result;
}

CommitGeometryResult GeometryManager::createGeometry(const CreateGeometryRequest& request)
{
    CommitGeometryResult result;
    const auto staged = stageGeometry(request.geometry);
    appendDiagnostics(result, staged);
    if (!staged.succeeded() || !staged.staged.has_value())
    {
        return result;
    }

    CreateObjectRequest common;
    common.name = request.name;
    common.type = DataObjectType::Geometry;
    common.parentId = request.parentId;
    common.visible = request.visible;
    common.selected = request.selected;
    common.metadata = request.commonMetadata;
    return commitGeometry(common, *staged.staged);
}

std::optional<GeometryObjectSnapshot> GeometryManager::findById(ObjectId id) const
{
    if (id == InvalidObjectId)
    {
        return std::nullopt;
    }

    const auto common = m_runtime.findById(id);
    if (!common.has_value() || common->type != DataObjectType::Geometry)
    {
        return std::nullopt;
    }

    QReadLocker locker(&m_lock);
    const auto payload = m_payloads.find(id);
    if (payload == m_payloads.end())
    {
        return std::nullopt;
    }
    return makeGeometrySnapshot(*common, payload->second);
}

QVector<GeometryObjectSnapshot> GeometryManager::snapshots() const
{
    const auto commonObjects = m_runtime.snapshots();
    QVector<GeometryObjectSnapshot> values;
    QReadLocker locker(&m_lock);
    for (const auto& common : commonObjects)
    {
        if (common.type != DataObjectType::Geometry)
        {
            continue;
        }
        const auto payload = m_payloads.find(common.id);
        if (payload != m_payloads.end())
        {
            values.append(makeGeometrySnapshot(common, payload->second));
        }
    }
    return values;
}

Common::OperationResult GeometryManager::validateManagedObject(ObjectId id) const
{
    Common::OperationResult result;
    if (id == InvalidObjectId)
    {
        result.add(diagnostic(QStringLiteral("GEO-OBJECT-ID-INVALID"),
                              QStringLiteral("Geometry object ID is invalid."),
                              QStringLiteral("Object ID zero is reserved."),
                              id,
                              QStringLiteral("object/id")));
        return result;
    }

    const auto common = m_runtime.findById(id);
    if (!common.has_value())
    {
        result.add(diagnostic(QStringLiteral("GEO-OBJECT-NOT-FOUND"),
                              QStringLiteral("Geometry object does not exist."),
                              QStringLiteral("Use a published geometry ObjectId."),
                              id));
        return result;
    }
    if (common->type != DataObjectType::Geometry)
    {
        result.add(diagnostic(QStringLiteral("GEO-OBJECT-TYPE-MISMATCH"),
                              QStringLiteral("Object is not a Geometry."),
                              QStringLiteral("The GeometryManager accepts only Geometry records."),
                              id));
        return result;
    }

    QReadLocker locker(&m_lock);
    if (m_payloads.find(id) == m_payloads.end())
    {
        result.add(diagnostic(QStringLiteral("GEO-PAYLOAD-NOT-FOUND"),
                              QStringLiteral("Geometry payload does not exist."),
                              QStringLiteral("The common record and geometry index are inconsistent."),
                              id));
    }
    return result;
}

Common::OperationResult GeometryManager::renameGeometry(ObjectId id,
                                                       const QString& requestedName)
{
    auto result = validateManagedObject(id);
    if (result.succeeded())
    {
        appendDiagnostics(result, m_runtime.renameObject(id, requestedName));
    }
    return result;
}

Common::OperationResult GeometryManager::setParent(ObjectId id, ObjectId parentId)
{
    auto result = validateManagedObject(id);
    if (result.succeeded())
    {
        appendDiagnostics(result, m_runtime.setParent(id, parentId));
    }
    return result;
}

Common::OperationResult GeometryManager::setVisible(ObjectId id, bool visible)
{
    auto result = validateManagedObject(id);
    if (result.succeeded())
    {
        appendDiagnostics(result, m_runtime.setVisible(id, visible));
    }
    return result;
}

Common::OperationResult GeometryManager::setSelected(ObjectId id, bool selected)
{
    auto result = validateManagedObject(id);
    if (result.succeeded())
    {
        appendDiagnostics(result, m_runtime.setSelected(id, selected));
    }
    return result;
}

Common::OperationResult GeometryManager::setDisplayMetadataValue(ObjectId id,
                                                                const QString& key,
                                                                const QVariant& value)
{
    auto result = validateManagedObject(id);
    const QString cleanKey = key.trimmed();
    if (cleanKey.isEmpty())
    {
        result.add(diagnostic(QStringLiteral("GEO-METADATA-KEY-EMPTY"),
                              QStringLiteral("Geometry display metadata key is empty."),
                              QStringLiteral("Provide a non-empty metadata key."),
                              id,
                              QStringLiteral("displayMetadata")));
    }
    if (!result.succeeded())
    {
        return result;
    }

    QWriteLocker locker(&m_lock);
    const auto payload = m_payloads.find(id);
    if (payload == m_payloads.end())
    {
        result.add(diagnostic(QStringLiteral("GEO-PAYLOAD-NOT-FOUND"),
                              QStringLiteral("Geometry payload does not exist."),
                              QStringLiteral("The object may have been removed concurrently."),
                              id));
        return result;
    }
    payload->second.displayMetadata.insert(cleanKey, value);
    return result;
}

Common::OperationResult GeometryManager::removeDisplayMetadataValue(ObjectId id,
                                                                   const QString& key)
{
    auto result = validateManagedObject(id);
    const QString cleanKey = key.trimmed();
    if (cleanKey.isEmpty())
    {
        result.add(diagnostic(QStringLiteral("GEO-METADATA-KEY-EMPTY"),
                              QStringLiteral("Geometry display metadata key is empty."),
                              QStringLiteral("Provide a non-empty metadata key."),
                              id,
                              QStringLiteral("displayMetadata")));
    }
    if (!result.succeeded())
    {
        return result;
    }

    QWriteLocker locker(&m_lock);
    const auto payload = m_payloads.find(id);
    if (payload == m_payloads.end())
    {
        result.add(diagnostic(QStringLiteral("GEO-PAYLOAD-NOT-FOUND"),
                              QStringLiteral("Geometry payload does not exist."),
                              QStringLiteral("The object may have been removed concurrently."),
                              id));
        return result;
    }
    payload->second.displayMetadata.remove(cleanKey);
    return result;
}

Common::OperationResult GeometryManager::removeGeometry(ObjectId id)
{
    auto result = validateManagedObject(id);
    if (!result.succeeded())
    {
        return result;
    }

    std::shared_ptr<GeometryRemovalConstraint> removalConstraint;
    {
        std::lock_guard<std::mutex> locker(m_removalConstraintMutex);
        removalConstraint = m_removalConstraint.lock();
    }
    if (removalConstraint)
    {
        appendDiagnostics(result, removalConstraint->beginGeometryRemoval(id));
        if (!result.succeeded())
        {
            return result;
        }
    }

    // The runtime call is deliberately made without holding m_lock. Once the
    // common record is gone, new joined queries cannot observe this payload.
    appendDiagnostics(result, m_runtime.removeTypedObject(id, DataObjectType::Geometry));
    if (!result.succeeded())
    {
        if (removalConstraint)
        {
            removalConstraint->cancelGeometryRemoval(id);
        }
        return result;
    }

    std::optional<GeometryObject> removedPayload;
    {
        QWriteLocker locker(&m_lock);
        const auto payload = m_payloads.find(id);
        if (payload == m_payloads.end())
        {
            result.add(diagnostic(QStringLiteral("GEO-PAYLOAD-NOT-FOUND"),
                                  QStringLiteral("Geometry payload disappeared during removal."),
                                  QStringLiteral("The common record was removed; validate geometry indexes."),
                                  id));
        }
        else
        {
            removedPayload = std::move(payload->second);
            m_payloads.erase(payload);
        }
    }
    if (removalConstraint)
    {
        removalConstraint->completeGeometryRemoval(id);
    }
    if (removedPayload)
    {
        std::function<void(const GeometryObject&)> releaser;
        {
            std::lock_guard<std::mutex> locker(m_externalResourceMutex);
            releaser = m_externalResourceReleaser;
        }
        if (releaser)
        {
            try
            {
                releaser(*removedPayload);
            }
            catch (const std::exception& exception)
            {
                result.add(diagnostic(QStringLiteral("GEO-EXTERNAL-RELEASE-FAILED"),
                                      QStringLiteral("The external geometry resource could not be released."),
                                      QString::fromLocal8Bit(exception.what()), id));
            }
            catch (...)
            {
                result.add(diagnostic(QStringLiteral("GEO-EXTERNAL-RELEASE-FAILED"),
                                      QStringLiteral("The external geometry resource could not be released."),
                                      QStringLiteral("Unknown release exception."), id));
            }
        }
    }
    return result;
}

void GeometryManager::setRemovalConstraint(
    const std::shared_ptr<GeometryRemovalConstraint>& constraint)
{
    std::lock_guard<std::mutex> locker(m_removalConstraintMutex);
    m_removalConstraint = constraint;
}

void GeometryManager::setExternalResourceReleaser(
    std::function<void(const GeometryObject&)> releaser)
{
    std::lock_guard<std::mutex> locker(m_externalResourceMutex);
    m_externalResourceReleaser = std::move(releaser);
}

int GeometryManager::objectCount() const
{
    return snapshots().size();
}

Common::OperationResult GeometryManager::validateIndexes() const
{
    Common::OperationResult result;
    appendDiagnostics(result, m_runtime.validateIndexes());

    const auto commonObjects = m_runtime.snapshots();
    const auto reservedIds = m_runtime.reservedObjectIds(DataObjectType::Geometry);
    std::set<ObjectId> published;
    std::set<ObjectId> reserved;
    for (const auto& common : commonObjects)
    {
        if (common.type == DataObjectType::Geometry)
        {
            published.insert(common.id);
        }
    }
    for (const ObjectId id : reservedIds)
    {
        reserved.insert(id);
    }

    QReadLocker locker(&m_lock);
    for (const auto& payload : m_payloads)
    {
        const ObjectId id = payload.first;
        if (id == InvalidObjectId ||
            (published.find(id) == published.end() && reserved.find(id) == reserved.end()))
        {
            result.add(diagnostic(QStringLiteral("GEO-INDEX-INCONSISTENT"),
                                  QStringLiteral("Geometry payload has no common record or reservation."),
                                  QStringLiteral("Remove the orphan geometry payload."),
                                  id));
        }

        auto validation = validateGeometryObject(payload.second);
        for (auto item : validation.diagnostics)
        {
            item.objectId = QString::number(id);
            item.stage = QStringLiteral("geometry.indexes");
            result.add(std::move(item));
        }
    }
    for (const ObjectId id : published)
    {
        if (m_payloads.find(id) == m_payloads.end())
        {
            result.add(diagnostic(QStringLiteral("GEO-INDEX-INCONSISTENT"),
                                  QStringLiteral("Geometry common record has no payload."),
                                  QStringLiteral("Published geometry records require a committed payload."),
                                  id));
        }
    }
    return result;
}
}
