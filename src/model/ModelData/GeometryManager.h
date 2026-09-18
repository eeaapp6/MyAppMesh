#pragma once

#include "ApplicationRuntime.h"
#include "GeometryObject.h"

#include <QReadWriteLock>

#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <optional>

namespace AppMesh::Model
{
class GeometryRemovalConstraint
{
public:
    virtual ~GeometryRemovalConstraint() = default;
    virtual Common::OperationResult beginGeometryRemoval(ObjectId geometryId) = 0;
    virtual void cancelGeometryRemoval(ObjectId geometryId) noexcept = 0;
    virtual void completeGeometryRemoval(ObjectId geometryId) noexcept = 0;
};

struct CreateGeometryRequest
{
    QString name;
    ObjectId parentId = InvalidObjectId;
    bool visible = true;
    bool selected = false;
    QVariantMap commonMetadata;
    GeometryObject geometry;
};

class StagedGeometry final
{
public:
    StagedGeometry(const StagedGeometry&) = default;
    StagedGeometry& operator=(const StagedGeometry&) = default;
    StagedGeometry(StagedGeometry&&) noexcept = default;
    StagedGeometry& operator=(StagedGeometry&&) noexcept = default;

    GeometryObject snapshot() const;

private:
    friend class GeometryManager;
    explicit StagedGeometry(GeometryObject geometry);

    GeometryObject m_geometry;
};

struct StageGeometryResult : Common::OperationResult
{
    std::optional<StagedGeometry> staged;
};

struct CommitGeometryResult : Common::OperationResult
{
    std::optional<GeometryObjectSnapshot> object;
};

// ApplicationRuntime must outlive GeometryManager. The manager owns only
// geometry payload/adapter values; the runtime owns every common record.
class GeometryManager final
{
public:
    // The optional checkpoint is a narrow test seam. It runs after payload
    // installation and before publication, with neither manager lock held.
    using BeforePublishCheckpoint =
        std::function<Common::OperationResult(ObjectId reservedId)>;

    explicit GeometryManager(ApplicationRuntime& runtime,
                             BeforePublishCheckpoint beforePublish = {});
    ~GeometryManager();

    GeometryManager(const GeometryManager&) = delete;
    GeometryManager& operator=(const GeometryManager&) = delete;
    GeometryManager(GeometryManager&&) = delete;
    GeometryManager& operator=(GeometryManager&&) = delete;

    StageGeometryResult stageGeometry(const GeometryObject& geometry) const;
    CommitGeometryResult commitGeometry(const CreateObjectRequest& common,
                                        const StagedGeometry& staged);
    CommitGeometryResult createGeometry(const CreateGeometryRequest& request);

    std::optional<GeometryObjectSnapshot> findById(ObjectId id) const;
    QVector<GeometryObjectSnapshot> snapshots() const;

    Common::OperationResult renameGeometry(ObjectId id, const QString& requestedName);
    Common::OperationResult setParent(ObjectId id, ObjectId parentId);
    Common::OperationResult setVisible(ObjectId id, bool visible);
    Common::OperationResult setSelected(ObjectId id, bool selected);
    Common::OperationResult setDisplayMetadataValue(ObjectId id,
                                                    const QString& key,
                                                    const QVariant& value);
    Common::OperationResult removeDisplayMetadataValue(ObjectId id, const QString& key);
    Common::OperationResult removeGeometry(ObjectId id);

    // The constraint is weakly held so GeometryManager never owns MeshManager
    // or its association state. An expired constraint cannot dangle.
    void setRemovalConstraint(
        const std::shared_ptr<GeometryRemovalConstraint>& constraint);
    void setExternalResourceReleaser(
        std::function<void(const GeometryObject&)> releaser);

    int objectCount() const;
    Common::OperationResult validateIndexes() const;

private:
    Common::Diagnostic diagnostic(const QString& code,
                                  const QString& message,
                                  const QString& detail,
                                  ObjectId objectId = InvalidObjectId,
                                  const QString& path = QString()) const;
    Common::OperationResult validateManagedObject(ObjectId id) const;
    void appendDiagnostics(Common::OperationResult& target,
                           const Common::OperationResult& source) const;

    ApplicationRuntime& m_runtime;
    BeforePublishCheckpoint m_beforePublish;
    mutable std::mutex m_removalConstraintMutex;
    std::weak_ptr<GeometryRemovalConstraint> m_removalConstraint;
    std::mutex m_externalResourceMutex;
    std::function<void(const GeometryObject&)> m_externalResourceReleaser;
    mutable QReadWriteLock m_lock;
    std::map<ObjectId, GeometryObject> m_payloads;
};
}
