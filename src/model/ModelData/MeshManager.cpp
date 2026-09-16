#include "MeshManager.h"

#include "GeometryManager.h"

#include <QReadLocker>
#include <QWriteLocker>

#include <algorithm>
#include <exception>
#include <map>
#include <new>
#include <set>
#include <utility>

namespace AppMesh::Model
{
namespace
{
struct ManagedMeshPayload
{
    MeshData mesh;
    QString creatorKey;
    MeshAdapterReference adapter;
};

Common::Diagnostic associationDiagnostic(const QString& code,
                                         const QString& message,
                                         const QString& detail,
                                         ObjectId geometryId)
{
    return {QStringLiteral("model"),
            code,
            message,
            detail,
            true,
            QStringLiteral("geometryId"),
            QStringLiteral("mesh.associations"),
            geometryId == InvalidObjectId ? QString() : QString::number(geometryId)};
}
}

class MeshManager::State final : public GeometryRemovalConstraint
{
public:
    Common::OperationResult beginGeometryRemoval(ObjectId geometryId) override
    {
        Common::OperationResult result;
        QWriteLocker locker(&lock);
        const auto referenced = geometryToMeshes.find(geometryId);
        const auto deleting = inFlightMeshRemovals.find(geometryId);
        if ((referenced != geometryToMeshes.end() && !referenced->second.empty()) ||
            (deleting != inFlightMeshRemovals.end() && deleting->second > 0))
        {
            result.add(associationDiagnostic(
                QStringLiteral("MESH-GEOMETRY-IN-USE"),
                QStringLiteral("Geometry is referenced by managed meshes."),
                QStringLiteral("Remove the referencing meshes before deleting geometry %1.")
                    .arg(geometryId),
                geometryId));
            return result;
        }
        unavailableGeometryIds.insert(geometryId);
        return result;
    }

    void cancelGeometryRemoval(ObjectId geometryId) noexcept override
    {
        QWriteLocker locker(&lock);
        unavailableGeometryIds.erase(geometryId);
    }

    void completeGeometryRemoval(ObjectId geometryId) noexcept override
    {
        QWriteLocker locker(&lock);
        unavailableGeometryIds.insert(geometryId);
    }

    mutable QReadWriteLock lock;
    std::map<QString, std::shared_ptr<const MeshDataCreator>> creators;
    std::map<QString, int> activeCreatorOperations;
    std::map<ObjectId, ManagedMeshPayload> payloads;
    std::map<ObjectId, std::set<ObjectId>> geometryToMeshes;
    std::map<ObjectId, ObjectId> meshToGeometry;
    std::map<ObjectId, int> inFlightMeshRemovals;
    std::set<ObjectId> unavailableGeometryIds;
};

MeshManager::MeshManager(ApplicationRuntime& runtime,
                         GeometryManager& geometryManager,
                         BeforePublishCheckpoint beforePublish,
                         BeforeTypedRemoveCheckpoint beforeTypedRemove)
    : m_runtime(runtime),
      m_state(std::make_shared<State>()),
      m_beforePublish(std::move(beforePublish)),
      m_beforeTypedRemove(std::move(beforeTypedRemove))
{
    geometryManager.setRemovalConstraint(m_state);
}

MeshManager::~MeshManager() = default;

Common::Diagnostic MeshManager::diagnostic(const QString& code,
                                           const QString& message,
                                           const QString& detail,
                                           ObjectId objectId,
                                           const QString& path,
                                           const QString& stage) const
{
    return {QStringLiteral("model"),
            code,
            message,
            detail,
            true,
            path,
            stage,
            objectId == InvalidObjectId ? QString() : QString::number(objectId)};
}

void MeshManager::appendDiagnostics(Common::OperationResult& target,
                                    const Common::OperationResult& source) const
{
    for (const auto& item : source.diagnostics)
    {
        target.add(item);
    }
}

Common::OperationResult MeshManager::registerCreator(
    const QString& key,
    std::shared_ptr<const MeshDataCreator> creator)
{
    Common::OperationResult result;
    const QString cleanKey = key.trimmed();
    if (cleanKey.isEmpty())
    {
        result.add(diagnostic(QStringLiteral("MESH-CREATOR-KEY-EMPTY"),
                              QStringLiteral("Mesh creator key is empty."),
                              QStringLiteral("Provide a non-empty stable registry key."),
                              InvalidObjectId,
                              QStringLiteral("creator/key"),
                              QStringLiteral("mesh.creator.register")));
        return result;
    }
    if (!creator)
    {
        result.add(diagnostic(QStringLiteral("MESH-CREATOR-NULL"),
                              QStringLiteral("Mesh creator is null."),
                              QStringLiteral("Register an owned MeshDataCreator instance."),
                              InvalidObjectId,
                              QStringLiteral("creator"),
                              QStringLiteral("mesh.creator.register")));
        return result;
    }

    QWriteLocker locker(&m_state->lock);
    if (!m_state->creators.emplace(cleanKey, std::move(creator)).second)
    {
        result.add(diagnostic(QStringLiteral("MESH-CREATOR-DUPLICATE"),
                              QStringLiteral("Mesh creator key is already registered."),
                              QStringLiteral("Creator key '%1' was not overwritten.").arg(cleanKey),
                              InvalidObjectId,
                              QStringLiteral("creator/key"),
                              QStringLiteral("mesh.creator.register")));
    }
    return result;
}

Common::OperationResult MeshManager::unregisterCreator(const QString& key)
{
    Common::OperationResult result;
    const QString cleanKey = key.trimmed();
    QWriteLocker locker(&m_state->lock);
    const auto creator = m_state->creators.find(cleanKey);
    if (creator == m_state->creators.end())
    {
        result.add(diagnostic(QStringLiteral("MESH-CREATOR-NOT-FOUND"),
                              QStringLiteral("Mesh creator is not registered."),
                              QStringLiteral("No creator exists for key '%1'.").arg(cleanKey),
                              InvalidObjectId,
                              QStringLiteral("creator/key"),
                              QStringLiteral("mesh.creator.unregister")));
        return result;
    }
    const auto active = m_state->activeCreatorOperations.find(cleanKey);
    if (active != m_state->activeCreatorOperations.end() && active->second > 0)
    {
        result.add(diagnostic(QStringLiteral("MESH-CREATOR-ACTIVE"),
                              QStringLiteral("Mesh creator has an active operation."),
                              QStringLiteral("Wait for the creator operation to finish before unregistering."),
                              InvalidObjectId,
                              QStringLiteral("creator/key"),
                              QStringLiteral("mesh.creator.unregister")));
        return result;
    }
    const auto used = std::find_if(
        m_state->payloads.cbegin(),
        m_state->payloads.cend(),
        [&](const auto& entry) { return entry.second.creatorKey == cleanKey; });
    if (used != m_state->payloads.cend())
    {
        result.add(diagnostic(QStringLiteral("MESH-CREATOR-IN-USE"),
                              QStringLiteral("Mesh creator owns managed mesh data."),
                              QStringLiteral("Remove mesh %1 before unregistering '%2'.")
                                  .arg(used->first)
                                  .arg(cleanKey),
                              used->first,
                              QStringLiteral("creator/key"),
                              QStringLiteral("mesh.creator.unregister")));
        return result;
    }
    m_state->activeCreatorOperations.erase(cleanKey);
    m_state->creators.erase(creator);
    return result;
}

QStringList MeshManager::creatorKeys() const
{
    QStringList keys;
    QReadLocker locker(&m_state->lock);
    for (const auto& creator : m_state->creators)
    {
        keys.append(creator.first);
    }
    return keys;
}

void MeshManager::finishCreatorOperation(const QString& creatorKey) noexcept
{
    QWriteLocker locker(&m_state->lock);
    const auto active = m_state->activeCreatorOperations.find(creatorKey);
    if (active != m_state->activeCreatorOperations.end() && active->second > 0)
    {
        --active->second;
    }
}

CreateMeshResult MeshManager::createMesh(const CreateMeshRequest& request)
{
    CreateMeshResult result;
    const QString creatorKey = request.creatorKey.trimmed();
    std::shared_ptr<const MeshDataCreator> creator;
    {
        QWriteLocker locker(&m_state->lock);
        const auto found = m_state->creators.find(creatorKey);
        if (found == m_state->creators.end())
        {
            result.add(diagnostic(QStringLiteral("MESH-CREATOR-NOT-FOUND"),
                                  QStringLiteral("Mesh creator is not registered."),
                                  QStringLiteral("Register creator '%1' before creating mesh data.")
                                      .arg(creatorKey),
                                  InvalidObjectId,
                                  QStringLiteral("creator/key"),
                                  QStringLiteral("mesh.create")));
            return result;
        }
        creator = found->second;
        ++m_state->activeCreatorOperations[creatorKey];
    }

    MeshDataCreatorResult created;
    try
    {
        created = creator->create(request.creatorRequest);
    }
    catch (const std::exception& exception)
    {
        created.add(diagnostic(QStringLiteral("MESH-CREATOR-EXCEPTION"),
                               QStringLiteral("Mesh creator raised an exception."),
                               QString::fromLocal8Bit(exception.what()),
                               InvalidObjectId,
                               QStringLiteral("creator"),
                               QStringLiteral("mesh.create.creator")));
    }
    catch (...)
    {
        created.add(diagnostic(QStringLiteral("MESH-CREATOR-EXCEPTION"),
                               QStringLiteral("Mesh creator raised an exception."),
                               QStringLiteral("The creator raised an unknown exception."),
                               InvalidObjectId,
                               QStringLiteral("creator"),
                               QStringLiteral("mesh.create.creator")));
    }
    appendDiagnostics(result, created);
    if (!created.succeeded() || !created.created.has_value())
    {
        if (created.succeeded())
        {
            result.add(diagnostic(QStringLiteral("MESH-CREATOR-RESULT-MISSING"),
                                  QStringLiteral("Mesh creator returned no data."),
                                  QStringLiteral("A successful creator must return mesh and adapter values."),
                                  InvalidObjectId,
                                  QStringLiteral("creator/result"),
                                  QStringLiteral("mesh.create.creator")));
        }
        finishCreatorOperation(creatorKey);
        return result;
    }

    const CreatedMeshData& candidate = *created.created;
    if (candidate.mesh.generator().key.trimmed() != creatorKey)
    {
        result.add(diagnostic(QStringLiteral("MESH-CREATOR-KEY-MISMATCH"),
                              QStringLiteral("Mesh provenance does not match its creator."),
                              QStringLiteral("Expected generator key '%1'.").arg(creatorKey),
                              InvalidObjectId,
                              QStringLiteral("generator/key"),
                              QStringLiteral("mesh.create.validate")));
    }
    if (!candidate.adapter.isValid())
    {
        result.add(diagnostic(QStringLiteral("MESH-ADAPTER-REFERENCE-INVALID"),
                              QStringLiteral("Mesh adapter reference is invalid."),
                              QStringLiteral("Provide non-empty provider and opaque mesh keys; raw pointers are not accepted."),
                              InvalidObjectId,
                              QStringLiteral("adapter"),
                              QStringLiteral("mesh.create.validate")));
    }
    appendDiagnostics(result, candidate.mesh.validate(m_runtime));
    if (!result.succeeded())
    {
        finishCreatorOperation(creatorKey);
        return result;
    }

    CreateObjectRequest common;
    common.name = request.name;
    common.type = DataObjectType::Mesh;
    common.parentId = request.parentId;
    common.visible = request.visible;
    common.selected = request.selected;
    common.metadata = request.commonMetadata;
    auto reserved = m_runtime.reserveDomainObject(common);
    appendDiagnostics(result, reserved);
    if (!reserved.succeeded() || !reserved.reservation.has_value())
    {
        finishCreatorOperation(creatorKey);
        return result;
    }

    auto& reservation = *reserved.reservation;
    const ObjectId meshId = reservation.id();
    const ObjectId geometryId = candidate.mesh.geometryId();
    bool installed = false;
    try
    {
        QWriteLocker locker(&m_state->lock);
        if (geometryId != InvalidObjectId &&
            m_state->unavailableGeometryIds.find(geometryId) !=
                m_state->unavailableGeometryIds.end())
        {
            result.add(diagnostic(QStringLiteral("MESH-GEOMETRY-REMOVING"),
                                  QStringLiteral("Source geometry is being removed or is unavailable."),
                                  QStringLiteral("Retry with a published Geometry object."),
                                  geometryId,
                                  QStringLiteral("geometryId"),
                                  QStringLiteral("mesh.create.associate")));
        }
        else
        {
            const auto payload = m_state->payloads.emplace(
                meshId, ManagedMeshPayload{candidate.mesh, creatorKey, candidate.adapter});
            const auto reverse = m_state->meshToGeometry.emplace(meshId, geometryId);
            if (!payload.second || !reverse.second)
            {
                m_state->meshToGeometry.erase(meshId);
                m_state->payloads.erase(meshId);
                result.add(diagnostic(QStringLiteral("MESH-PAYLOAD-ID-CONFLICT"),
                                      QStringLiteral("Mesh payload ID is already present."),
                                      QStringLiteral("The hidden common reservation will be cancelled."),
                                      meshId,
                                      QStringLiteral("object/id"),
                                      QStringLiteral("mesh.create.install")));
            }
            else
            {
                if (geometryId != InvalidObjectId)
                {
                    m_state->geometryToMeshes[geometryId].insert(meshId);
                }
                installed = true;
            }
        }
    }
    catch (const std::bad_alloc&)
    {
        QWriteLocker locker(&m_state->lock);
        m_state->payloads.erase(meshId);
        m_state->meshToGeometry.erase(meshId);
        if (geometryId != InvalidObjectId)
        {
            const auto forward = m_state->geometryToMeshes.find(geometryId);
            if (forward != m_state->geometryToMeshes.end())
            {
                forward->second.erase(meshId);
                if (forward->second.empty())
                {
                    m_state->geometryToMeshes.erase(forward);
                }
            }
        }
        result.add(diagnostic(QStringLiteral("MESH-RESOURCE-EXHAUSTED"),
                              QStringLiteral("Mesh payload could not be installed."),
                              QStringLiteral("No payload or association index was retained."),
                              meshId,
                              QStringLiteral("payload"),
                              QStringLiteral("mesh.create.install")));
    }

    if (!installed || !result.succeeded())
    {
        appendDiagnostics(result, m_runtime.cancelDomainObject(reservation));
        finishCreatorOperation(creatorKey);
        return result;
    }

    if (m_beforePublish)
    {
        try
        {
            appendDiagnostics(result, m_beforePublish(meshId));
        }
        catch (...)
        {
            result.add(diagnostic(QStringLiteral("MESH-COMMIT-CHECKPOINT-FAILED"),
                                  QStringLiteral("Mesh commit checkpoint failed."),
                                  QStringLiteral("The checkpoint raised an exception."),
                                  meshId,
                                  QStringLiteral("object"),
                                  QStringLiteral("mesh.create.publish")));
        }
    }

    PublishObjectResult published;
    if (result.succeeded())
    {
        published = m_runtime.publishDomainObject(reservation);
        appendDiagnostics(result, published);
    }
    if (!result.succeeded() || !published.object.has_value())
    {
        {
            QWriteLocker locker(&m_state->lock);
            m_state->payloads.erase(meshId);
            m_state->meshToGeometry.erase(meshId);
            if (geometryId != InvalidObjectId)
            {
                const auto forward = m_state->geometryToMeshes.find(geometryId);
                if (forward != m_state->geometryToMeshes.end())
                {
                    forward->second.erase(meshId);
                    if (forward->second.empty())
                    {
                        m_state->geometryToMeshes.erase(forward);
                    }
                }
            }
        }
        if (reservation.isValid())
        {
            appendDiagnostics(result, m_runtime.cancelDomainObject(reservation));
        }
        finishCreatorOperation(creatorKey);
        return result;
    }

    MeshObjectSnapshot snapshot;
    snapshot.common = *published.object;
    snapshot.mesh = candidate.mesh;
    snapshot.creatorKey = creatorKey;
    snapshot.adapterProviderKey = candidate.adapter.providerKey;
    snapshot.adapterMeshKey = candidate.adapter.meshKey;
    snapshot.adapterMetadata = candidate.adapter.metadata;
    result.object = std::move(snapshot);
    finishCreatorOperation(creatorKey);
    return result;
}

Common::OperationResult MeshManager::removeMesh(ObjectId id)
{
    Common::OperationResult result;
    const auto common = m_runtime.findById(id);
    if (!common.has_value())
    {
        result.add(diagnostic(QStringLiteral("MESH-OBJECT-NOT-FOUND"),
                              QStringLiteral("Mesh object does not exist."),
                              QStringLiteral("Use a published Mesh ObjectId."),
                              id));
        return result;
    }
    if (common->type != DataObjectType::Mesh)
    {
        result.add(diagnostic(QStringLiteral("MESH-OBJECT-TYPE-MISMATCH"),
                              QStringLiteral("Object is not a Mesh."),
                              QStringLiteral("MeshManager removes only Mesh records."),
                              id));
        return result;
    }

    ObjectId geometryId = InvalidObjectId;
    {
        QWriteLocker locker(&m_state->lock);
        if (m_state->payloads.find(id) == m_state->payloads.end())
        {
            result.add(diagnostic(QStringLiteral("MESH-PAYLOAD-NOT-FOUND"),
                                  QStringLiteral("Mesh payload does not exist."),
                                  QStringLiteral("The common record and mesh index are inconsistent."),
                                  id));
            return result;
        }
        const auto reverse = m_state->meshToGeometry.find(id);
        if (reverse == m_state->meshToGeometry.end())
        {
            result.add(diagnostic(QStringLiteral("MESH-ASSOCIATION-NOT-FOUND"),
                                  QStringLiteral("Mesh reverse association is missing."),
                                  QStringLiteral("Validate MeshManager indexes before removal."),
                                  id,
                                  QStringLiteral("associations/reverse"),
                                  QStringLiteral("mesh.remove.index")));
            return result;
        }
        geometryId = reverse->second;
        if (geometryId != InvalidObjectId)
        {
            ++m_state->inFlightMeshRemovals[geometryId];
        }
        m_state->meshToGeometry.erase(reverse);
        if (geometryId != InvalidObjectId)
        {
            const auto forward = m_state->geometryToMeshes.find(geometryId);
            if (forward != m_state->geometryToMeshes.end())
            {
                forward->second.erase(id);
                if (forward->second.empty())
                {
                    m_state->geometryToMeshes.erase(forward);
                }
            }
        }
    }

    if (m_beforeTypedRemove)
    {
        try
        {
            appendDiagnostics(result, m_beforeTypedRemove(id, geometryId));
        }
        catch (...)
        {
            result.add(diagnostic(QStringLiteral("MESH-REMOVE-CHECKPOINT-FAILED"),
                                  QStringLiteral("Mesh remove checkpoint failed."),
                                  QStringLiteral("The checkpoint raised an exception."),
                                  id,
                                  QStringLiteral("object"),
                                  QStringLiteral("mesh.remove.checkpoint")));
        }
    }
    if (result.succeeded())
    {
        appendDiagnostics(result, m_runtime.removeTypedObject(id, DataObjectType::Mesh));
    }
    if (!result.succeeded())
    {
        QWriteLocker locker(&m_state->lock);
        m_state->meshToGeometry[id] = geometryId;
        if (geometryId != InvalidObjectId)
        {
            m_state->geometryToMeshes[geometryId].insert(id);
            const auto deleting = m_state->inFlightMeshRemovals.find(geometryId);
            if (deleting != m_state->inFlightMeshRemovals.end() && --deleting->second == 0)
            {
                m_state->inFlightMeshRemovals.erase(deleting);
            }
        }
        return result;
    }

    QWriteLocker locker(&m_state->lock);
    if (m_state->payloads.erase(id) != 1)
    {
        result.add(diagnostic(QStringLiteral("MESH-PAYLOAD-NOT-FOUND"),
                              QStringLiteral("Mesh payload disappeared during removal."),
                              QStringLiteral("The common record was removed; validate mesh indexes."),
                              id,
                              QStringLiteral("payload"),
                              QStringLiteral("mesh.remove.payload")));
    }
    if (geometryId != InvalidObjectId)
    {
        const auto deleting = m_state->inFlightMeshRemovals.find(geometryId);
        if (deleting != m_state->inFlightMeshRemovals.end() && --deleting->second == 0)
        {
            m_state->inFlightMeshRemovals.erase(deleting);
        }
    }
    return result;
}

std::optional<MeshObjectSnapshot> MeshManager::findById(ObjectId id) const
{
    const auto common = m_runtime.findById(id);
    if (!common.has_value() || common->type != DataObjectType::Mesh)
    {
        return std::nullopt;
    }
    QReadLocker locker(&m_state->lock);
    const auto payload = m_state->payloads.find(id);
    if (payload == m_state->payloads.end())
    {
        return std::nullopt;
    }
    MeshObjectSnapshot snapshot;
    snapshot.common = *common;
    snapshot.mesh = payload->second.mesh;
    snapshot.creatorKey = payload->second.creatorKey;
    snapshot.adapterProviderKey = payload->second.adapter.providerKey;
    snapshot.adapterMeshKey = payload->second.adapter.meshKey;
    snapshot.adapterMetadata = payload->second.adapter.metadata;
    return snapshot;
}

Common::OperationResult MeshManager::validateManagedObject(ObjectId id) const
{
    Common::OperationResult result;
    if (id == InvalidObjectId)
    {
        result.add(diagnostic(QStringLiteral("MESH-OBJECT-ID-INVALID"),
                              QStringLiteral("Mesh object ID is invalid."),
                              QStringLiteral("Object ID zero is reserved."),
                              id,
                              QStringLiteral("object/id")));
        return result;
    }

    const auto common = m_runtime.findById(id);
    if (!common.has_value())
    {
        result.add(diagnostic(QStringLiteral("MESH-OBJECT-NOT-FOUND"),
                              QStringLiteral("Mesh object does not exist."),
                              QStringLiteral("Use a published mesh ObjectId."),
                              id));
        return result;
    }
    if (common->type != DataObjectType::Mesh)
    {
        result.add(diagnostic(QStringLiteral("MESH-OBJECT-TYPE-MISMATCH"),
                              QStringLiteral("Object is not a Mesh."),
                              QStringLiteral("The MeshManager accepts only Mesh records."),
                              id));
        return result;
    }

    QReadLocker locker(&m_state->lock);
    if (m_state->payloads.find(id) == m_state->payloads.end())
    {
        result.add(diagnostic(QStringLiteral("MESH-PAYLOAD-NOT-FOUND"),
                              QStringLiteral("Mesh payload does not exist."),
                              QStringLiteral("The common record and mesh index are inconsistent."),
                              id,
                              QStringLiteral("payload")));
    }
    return result;
}

Common::OperationResult MeshManager::renameMesh(ObjectId id,
                                                const QString& requestedName)
{
    auto result = validateManagedObject(id);
    if (result.succeeded())
    {
        appendDiagnostics(result, m_runtime.renameObject(id, requestedName));
    }
    return result;
}

Common::OperationResult MeshManager::setParent(ObjectId id, ObjectId parentId)
{
    auto result = validateManagedObject(id);
    if (result.succeeded())
    {
        appendDiagnostics(result, m_runtime.setParent(id, parentId));
    }
    return result;
}

Common::OperationResult MeshManager::setVisible(ObjectId id, bool visible)
{
    auto result = validateManagedObject(id);
    if (result.succeeded())
    {
        appendDiagnostics(result, m_runtime.setVisible(id, visible));
    }
    return result;
}

Common::OperationResult MeshManager::setSelected(ObjectId id, bool selected)
{
    auto result = validateManagedObject(id);
    if (result.succeeded())
    {
        appendDiagnostics(result, m_runtime.setSelected(id, selected));
    }
    return result;
}

QVector<MeshObjectSnapshot> MeshManager::snapshots() const
{
    const auto commonObjects = m_runtime.snapshots();
    QVector<MeshObjectSnapshot> values;
    QReadLocker locker(&m_state->lock);
    for (const auto& common : commonObjects)
    {
        if (common.type != DataObjectType::Mesh)
        {
            continue;
        }
        const auto payload = m_state->payloads.find(common.id);
        if (payload == m_state->payloads.end())
        {
            continue;
        }
        MeshObjectSnapshot snapshot;
        snapshot.common = common;
        snapshot.mesh = payload->second.mesh;
        snapshot.creatorKey = payload->second.creatorKey;
        snapshot.adapterProviderKey = payload->second.adapter.providerKey;
        snapshot.adapterMeshKey = payload->second.adapter.meshKey;
        snapshot.adapterMetadata = payload->second.adapter.metadata;
        values.append(std::move(snapshot));
    }
    return values;
}

QVector<ObjectId> MeshManager::meshIdsForGeometry(ObjectId geometryId) const
{
    QVector<ObjectId> candidates;
    {
        QReadLocker locker(&m_state->lock);
        for (const auto& payload : m_state->payloads)
        {
            if (payload.second.mesh.geometryId() == geometryId)
            {
                candidates.append(payload.first);
            }
        }
    }
    QVector<ObjectId> ids;
    for (const ObjectId id : candidates)
    {
        const auto common = m_runtime.findById(id);
        if (common.has_value() && common->type == DataObjectType::Mesh)
        {
            ids.append(id);
        }
    }
    return ids;
}

std::optional<ObjectId> MeshManager::geometryIdForMesh(ObjectId meshId) const
{
    const auto common = m_runtime.findById(meshId);
    if (!common.has_value() || common->type != DataObjectType::Mesh)
    {
        return std::nullopt;
    }
    QReadLocker locker(&m_state->lock);
    const auto found = m_state->payloads.find(meshId);
    return found == m_state->payloads.end()
        ? std::optional<ObjectId>{}
        : std::optional<ObjectId>{found->second.mesh.geometryId()};
}

int MeshManager::objectCount() const
{
    return snapshots().size();
}

Common::OperationResult MeshManager::validateIndexes() const
{
    Common::OperationResult result;
    appendDiagnostics(result, m_runtime.validateIndexes());
    const auto commonObjects = m_runtime.snapshots();
    const auto reservedIds = m_runtime.reservedObjectIds(DataObjectType::Mesh);

    std::map<ObjectId, ManagedMeshPayload> payloads;
    std::map<ObjectId, std::set<ObjectId>> forward;
    std::map<ObjectId, ObjectId> reverse;
    std::set<QString> creators;
    {
        QReadLocker locker(&m_state->lock);
        payloads = m_state->payloads;
        forward = m_state->geometryToMeshes;
        reverse = m_state->meshToGeometry;
        for (const auto& creator : m_state->creators)
        {
            creators.insert(creator.first);
        }
    }

    std::set<ObjectId> published;
    std::set<ObjectId> reserved(reservedIds.cbegin(), reservedIds.cend());
    for (const auto& common : commonObjects)
    {
        if (common.type == DataObjectType::Mesh)
        {
            published.insert(common.id);
        }
    }
    for (const auto& payload : payloads)
    {
        const ObjectId id = payload.first;
        if (published.find(id) == published.end() && reserved.find(id) == reserved.end())
        {
            result.add(diagnostic(QStringLiteral("MESH-INDEX-INCONSISTENT"),
                                  QStringLiteral("Mesh payload has no common record or reservation."),
                                  QStringLiteral("Remove the orphan mesh payload."),
                                  id,
                                  QStringLiteral("payload"),
                                  QStringLiteral("mesh.indexes")));
        }
        if (creators.find(payload.second.creatorKey) == creators.end())
        {
            result.add(diagnostic(QStringLiteral("MESH-CREATOR-INDEX-INCONSISTENT"),
                                  QStringLiteral("Mesh payload references an unregistered creator."),
                                  QStringLiteral("Keep creators registered while their meshes exist."),
                                  id,
                                  QStringLiteral("creator/key"),
                                  QStringLiteral("mesh.indexes")));
        }
        auto validation = payload.second.mesh.validate(m_runtime);
        for (auto item : validation.diagnostics)
        {
            item.objectId = QString::number(id);
            item.stage = QStringLiteral("mesh.indexes");
            result.add(std::move(item));
        }
        const auto reverseEntry = reverse.find(id);
        if (reverseEntry == reverse.end() ||
            reverseEntry->second != payload.second.mesh.geometryId())
        {
            result.add(diagnostic(QStringLiteral("MESH-REVERSE-INDEX-INCONSISTENT"),
                                  QStringLiteral("Mesh reverse association is inconsistent."),
                                  QStringLiteral("Rebuild the mesh-to-geometry index."),
                                  id,
                                  QStringLiteral("associations/reverse"),
                                  QStringLiteral("mesh.indexes")));
        }
    }
    for (const ObjectId id : published)
    {
        if (payloads.find(id) == payloads.end())
        {
            result.add(diagnostic(QStringLiteral("MESH-INDEX-INCONSISTENT"),
                                  QStringLiteral("Mesh common record has no payload."),
                                  QStringLiteral("Published Mesh records require a committed payload."),
                                  id,
                                  QStringLiteral("payload"),
                                  QStringLiteral("mesh.indexes")));
        }
    }
    for (const auto& entry : reverse)
    {
        if (entry.second == InvalidObjectId)
        {
            continue;
        }
        const auto found = forward.find(entry.second);
        if (found == forward.end() || found->second.find(entry.first) == found->second.end())
        {
            result.add(diagnostic(QStringLiteral("MESH-FORWARD-INDEX-INCONSISTENT"),
                                  QStringLiteral("Geometry forward association is inconsistent."),
                                  QStringLiteral("Rebuild the geometry-to-mesh index."),
                                  entry.first,
                                  QStringLiteral("associations/forward"),
                                  QStringLiteral("mesh.indexes")));
        }
    }
    for (const auto& entry : forward)
    {
        for (const ObjectId meshId : entry.second)
        {
            const auto found = reverse.find(meshId);
            if (found == reverse.end() || found->second != entry.first)
            {
                result.add(diagnostic(QStringLiteral("MESH-REVERSE-INDEX-INCONSISTENT"),
                                      QStringLiteral("Geometry forward association has no reverse entry."),
                                      QStringLiteral("Rebuild the mesh-to-geometry index."),
                                      meshId,
                                      QStringLiteral("associations/reverse"),
                                      QStringLiteral("mesh.indexes")));
            }
        }
    }
    return result;
}
}
