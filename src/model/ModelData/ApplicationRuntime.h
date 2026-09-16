#pragma once

#include "DataObject.h"
#include "common/AppDiagnostic.h"
#include "common/ManagedService.h"

#include <QReadWriteLock>
#include <QVector>

#include <map>
#include <memory>
#include <optional>
#include <set>
#include <functional>

namespace AppMesh::Model
{
class GeometryManager;
class MeshManager;

struct CreateObjectRequest
{
    QString name;
    DataObjectType type = DataObjectType::Invalid;
    ObjectId parentId = InvalidObjectId;
    bool visible = true;
    bool selected = false;
    QVariantMap metadata;
};

struct CreateObjectResult : Common::OperationResult
{
    std::optional<DataObjectSnapshot> object;
};

class DomainObjectReservation final
{
public:
    DomainObjectReservation(const DomainObjectReservation&) = delete;
    DomainObjectReservation& operator=(const DomainObjectReservation&) = delete;
    DomainObjectReservation(DomainObjectReservation&& other) noexcept;
    DomainObjectReservation& operator=(DomainObjectReservation&& other) noexcept;
    ~DomainObjectReservation() = default;

    bool isValid() const noexcept;
    ObjectId id() const noexcept;
    DataObjectType type() const noexcept;

private:
    friend class ApplicationRuntime;

    DomainObjectReservation(ApplicationRuntime* owner,
                            ObjectId id,
                            DataObjectType type) noexcept;
    void reset() noexcept;

    ApplicationRuntime* m_owner = nullptr;
    ObjectId m_id = InvalidObjectId;
    DataObjectType m_type = DataObjectType::Invalid;
};

struct ReserveObjectResult : Common::OperationResult
{
    std::optional<DomainObjectReservation> reservation;
};

struct PublishObjectResult : Common::OperationResult
{
    std::optional<DataObjectSnapshot> object;
};

class ApplicationRuntime final : public Common::ManagedService
{
public:
    using BeforeDomainPublishCheckpoint =
        std::function<Common::OperationResult(ObjectId, DataObjectType)>;

    explicit ApplicationRuntime(
        ObjectId firstObjectId = 1,
        BeforeDomainPublishCheckpoint beforeDomainPublish = {});
    ~ApplicationRuntime() override;

    ApplicationRuntime(const ApplicationRuntime&) = delete;
    ApplicationRuntime& operator=(const ApplicationRuntime&) = delete;
    ApplicationRuntime(ApplicationRuntime&&) = delete;
    ApplicationRuntime& operator=(ApplicationRuntime&&) = delete;

    CreateObjectResult createObject(const CreateObjectRequest& request);

    std::optional<DataObjectSnapshot> findById(ObjectId id) const;
    std::optional<DataObjectSnapshot> findByName(const QString& name) const;
    QVector<DataObjectSnapshot> snapshots() const;
    QVector<ObjectId> childIds(ObjectId parentId) const;

    Common::OperationResult renameObject(ObjectId id, const QString& requestedName);
    Common::OperationResult setParent(ObjectId id, ObjectId parentId);
    Common::OperationResult setVisible(ObjectId id, bool visible);
    Common::OperationResult setSelected(ObjectId id, bool selected);
    Common::OperationResult setMetadataValue(ObjectId id,
                                             const QString& key,
                                             const QVariant& value);
    Common::OperationResult removeMetadataValue(ObjectId id, const QString& key);
    Common::OperationResult removeObject(ObjectId id);

    int objectCount() const;
    Common::OperationResult validateIndexes() const;

private:
    friend class GeometryManager;
    friend class MeshManager;

    struct ReservedObject
    {
        std::unique_ptr<DataObject> object;
    };

    ReserveObjectResult reserveDomainObject(const CreateObjectRequest& request);
    PublishObjectResult publishDomainObject(DomainObjectReservation& reservation);
    Common::OperationResult cancelDomainObject(DomainObjectReservation& reservation);
    Common::OperationResult removeTypedObject(ObjectId id, DataObjectType expectedType);
    QVector<ObjectId> reservedObjectIds(DataObjectType type) const;

    QString normalizedName(const QString& name) const;
    QString allocateUniqueNameLocked(const QString& requestedName,
                                     ObjectId ignoredId,
                                     QString* baseKey,
                                     quint64* nextSuffix) const;
    Common::Diagnostic diagnostic(const QString& code,
                                  const QString& message,
                                  const QString& detail,
                                  ObjectId objectId = InvalidObjectId) const;
    bool wouldCreateCycleLocked(ObjectId id, ObjectId parentId) const;
    bool hasReservedChildLocked(ObjectId parentId) const;
    Common::OperationResult removeObjectLocked(ObjectId id,
                                               std::optional<DataObjectType> expectedType);
    void consumeNextObjectIdLocked(ObjectId allocatedId) noexcept;

    mutable QReadWriteLock m_lock;
    std::map<ObjectId, std::unique_ptr<DataObject>> m_objects;
    std::map<ObjectId, ReservedObject> m_reservedObjects;
    std::map<QString, ObjectId> m_nameIndex;
    std::map<QString, ObjectId> m_reservedNameIndex;
    std::map<ObjectId, std::set<ObjectId>> m_children;
    std::map<QString, quint64> m_nextNameSuffix;
    ObjectId m_nextObjectId = 1;
    bool m_idExhausted = false;
    BeforeDomainPublishCheckpoint m_beforeDomainPublish;
};
}
