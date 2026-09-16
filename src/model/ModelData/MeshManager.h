#pragma once

#include "ApplicationRuntime.h"
#include "MeshDataCreator.h"

#include <functional>
#include <memory>
#include <optional>

#include <QStringList>

namespace AppMesh::Model
{
class GeometryManager;

struct CreateMeshRequest
{
    QString name;
    ObjectId parentId = InvalidObjectId;
    bool visible = true;
    bool selected = false;
    QVariantMap commonMetadata;
    QString creatorKey;
    MeshDataCreatorRequest creatorRequest;
};

struct MeshObjectSnapshot
{
    DataObjectSnapshot common;
    MeshData mesh;
    QString creatorKey;
    QString adapterProviderKey;
    QString adapterMeshKey;
    QVariantMap adapterMetadata;
};

struct CreateMeshResult : Common::OperationResult
{
    std::optional<MeshObjectSnapshot> object;
};

// ApplicationRuntime and GeometryManager must outlive MeshManager. Runtime owns
// common records; MeshManager owns creator registrations, mesh payloads and both
// Geometry-Mesh association indexes.
class MeshManager final
{
public:
    using BeforePublishCheckpoint =
        std::function<Common::OperationResult(ObjectId reservedId)>;
    using BeforeTypedRemoveCheckpoint =
        std::function<Common::OperationResult(ObjectId meshId, ObjectId geometryId)>;

    MeshManager(ApplicationRuntime& runtime,
                GeometryManager& geometryManager,
                BeforePublishCheckpoint beforePublish = {},
                BeforeTypedRemoveCheckpoint beforeTypedRemove = {});
    ~MeshManager();

    MeshManager(const MeshManager&) = delete;
    MeshManager& operator=(const MeshManager&) = delete;
    MeshManager(MeshManager&&) = delete;
    MeshManager& operator=(MeshManager&&) = delete;

    Common::OperationResult registerCreator(
        const QString& key,
        std::shared_ptr<const MeshDataCreator> creator);
    Common::OperationResult unregisterCreator(const QString& key);
    QStringList creatorKeys() const;

    CreateMeshResult createMesh(const CreateMeshRequest& request);
    Common::OperationResult removeMesh(ObjectId id);

    Common::OperationResult renameMesh(ObjectId id, const QString& requestedName);
    Common::OperationResult setParent(ObjectId id, ObjectId parentId);
    Common::OperationResult setVisible(ObjectId id, bool visible);
    Common::OperationResult setSelected(ObjectId id, bool selected);

    std::optional<MeshObjectSnapshot> findById(ObjectId id) const;
    QVector<MeshObjectSnapshot> snapshots() const;
    QVector<ObjectId> meshIdsForGeometry(ObjectId geometryId) const;
    std::optional<ObjectId> geometryIdForMesh(ObjectId meshId) const;

    int objectCount() const;
    Common::OperationResult validateIndexes() const;

private:
    class State;

    Common::Diagnostic diagnostic(const QString& code,
                                  const QString& message,
                                  const QString& detail,
                                  ObjectId objectId = InvalidObjectId,
                                  const QString& path = QString(),
                                  const QString& stage = QStringLiteral("mesh.manager")) const;
    void appendDiagnostics(Common::OperationResult& target,
                           const Common::OperationResult& source) const;
    Common::OperationResult validateManagedObject(ObjectId id) const;
    void finishCreatorOperation(const QString& creatorKey) noexcept;

    ApplicationRuntime& m_runtime;
    std::shared_ptr<State> m_state;
    BeforePublishCheckpoint m_beforePublish;
    BeforeTypedRemoveCheckpoint m_beforeTypedRemove;
};
}
