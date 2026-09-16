#include "ApplicationRuntime.h"

#include <QReadLocker>
#include <QWriteLocker>

#include <algorithm>
#include <limits>
#include <mutex>
#include <new>
#include <utility>
#include <vector>

namespace AppMesh::Model
{
struct ModelChangeState
{
    mutable std::mutex mutex;
    std::recursive_mutex deliveryMutex;
    std::map<quint64, ApplicationRuntime::ModelChangeCallback> callbacks;
    quint64 nextSubscriptionId = 1;
    quint64 nextSequence = 1;
    bool sourceAlive = true;
};

ModelChangeSubscription::ModelChangeSubscription(std::weak_ptr<ModelChangeState> state,
                                                 quint64 id) noexcept
    : m_state(std::move(state)), m_id(id)
{
}

ModelChangeSubscription::ModelChangeSubscription(ModelChangeSubscription&& other) noexcept
    : m_state(std::move(other.m_state)), m_id(other.m_id)
{
    other.m_id = 0;
}

ModelChangeSubscription& ModelChangeSubscription::operator=(
    ModelChangeSubscription&& other) noexcept
{
    if (this != &other)
    {
        reset();
        m_state = std::move(other.m_state);
        m_id = other.m_id;
        other.m_id = 0;
    }
    return *this;
}

ModelChangeSubscription::~ModelChangeSubscription()
{
    reset();
}

void ModelChangeSubscription::reset() noexcept
{
    if (m_id != 0)
    {
        if (const auto state = m_state.lock())
        {
            std::lock_guard<std::mutex> guard(state->mutex);
            state->callbacks.erase(m_id);
        }
    }
    m_state.reset();
    m_id = 0;
}

bool ModelChangeSubscription::isActive() const noexcept
{
    if (m_id == 0)
    {
        return false;
    }
    const auto state = m_state.lock();
    if (!state)
    {
        return false;
    }
    std::lock_guard<std::mutex> guard(state->mutex);
    return state->sourceAlive && state->callbacks.find(m_id) != state->callbacks.end();
}

bool ModelChangeSubscription::isSourceAlive() const noexcept
{
    const auto state = m_state.lock();
    if (!state)
    {
        return false;
    }
    std::lock_guard<std::mutex> guard(state->mutex);
    return state->sourceAlive;
}

DomainObjectReservation::DomainObjectReservation(ApplicationRuntime* owner,
                                                 ObjectId id,
                                                 DataObjectType type) noexcept
    : m_owner(owner), m_id(id), m_type(type)
{
}

DomainObjectReservation::DomainObjectReservation(DomainObjectReservation&& other) noexcept
    : m_owner(other.m_owner), m_id(other.m_id), m_type(other.m_type)
{
    other.reset();
}

DomainObjectReservation& DomainObjectReservation::operator=(
    DomainObjectReservation&& other) noexcept
{
    if (this != &other)
    {
        m_owner = other.m_owner;
        m_id = other.m_id;
        m_type = other.m_type;
        other.reset();
    }
    return *this;
}

bool DomainObjectReservation::isValid() const noexcept
{
    return m_owner != nullptr && m_id != InvalidObjectId &&
        m_type != DataObjectType::Invalid;
}

ObjectId DomainObjectReservation::id() const noexcept
{
    return m_id;
}

DataObjectType DomainObjectReservation::type() const noexcept
{
    return m_type;
}

void DomainObjectReservation::reset() noexcept
{
    m_owner = nullptr;
    m_id = InvalidObjectId;
    m_type = DataObjectType::Invalid;
}

ApplicationRuntime::ApplicationRuntime(
    ObjectId firstObjectId,
    BeforeDomainPublishCheckpoint beforeDomainPublish)
    : m_nextObjectId(firstObjectId == InvalidObjectId ? 1 : firstObjectId),
      m_beforeDomainPublish(std::move(beforeDomainPublish)),
      m_changeState(std::make_shared<ModelChangeState>())
{
}

ApplicationRuntime::~ApplicationRuntime()
{
    closeChangeSource();
}

ModelChangeSubscription ApplicationRuntime::subscribe(ModelChangeCallback callback)
{
    if (!callback || !m_changeState)
    {
        return {};
    }
    std::lock_guard<std::mutex> guard(m_changeState->mutex);
    if (!m_changeState->sourceAlive || m_changeState->nextSubscriptionId == 0)
    {
        return {};
    }
    const quint64 id = m_changeState->nextSubscriptionId++;
    m_changeState->callbacks.emplace(id, std::move(callback));
    return ModelChangeSubscription(m_changeState, id);
}

void ApplicationRuntime::emitChange(ModelChangeType type,
                                    ObjectId objectId,
                                    std::optional<DataObjectSnapshot> snapshot)
{
    std::lock_guard<std::recursive_mutex> deliveryGuard(m_changeState->deliveryMutex);
    std::vector<ModelChangeCallback> callbacks;
    ModelChangeEvent event;
    {
        std::lock_guard<std::mutex> guard(m_changeState->mutex);
        if (!m_changeState->sourceAlive)
        {
            return;
        }
        event = {type, objectId, std::move(snapshot), m_changeState->nextSequence++};
        callbacks.reserve(m_changeState->callbacks.size());
        for (const auto& entry : m_changeState->callbacks)
        {
            callbacks.push_back(entry.second);
        }
    }
    for (const auto& callback : callbacks)
    {
        try
        {
            callback(event);
        }
        catch (...)
        {
            // Domain commits cannot be rolled back by an observer failure.
        }
    }
}

void ApplicationRuntime::closeChangeSource() noexcept
{
    if (!m_changeState)
    {
        return;
    }
    std::lock_guard<std::recursive_mutex> deliveryGuard(m_changeState->deliveryMutex);
    std::vector<ModelChangeCallback> callbacks;
    ModelChangeEvent event;
    {
        std::lock_guard<std::mutex> guard(m_changeState->mutex);
        if (!m_changeState->sourceAlive)
        {
            return;
        }
        m_changeState->sourceAlive = false;
        event = {ModelChangeType::Reset,
                 InvalidObjectId,
                 {},
                 m_changeState->nextSequence++};
        callbacks.reserve(m_changeState->callbacks.size());
        for (const auto& entry : m_changeState->callbacks)
        {
            callbacks.push_back(entry.second);
        }
        m_changeState->callbacks.clear();
    }
    for (const auto& callback : callbacks)
    {
        try
        {
            callback(event);
        }
        catch (...)
        {
        }
    }
}

QString ApplicationRuntime::normalizedName(const QString& name) const
{
    return name.trimmed().toCaseFolded();
}

QString ApplicationRuntime::allocateUniqueNameLocked(const QString& requestedName,
                                                      ObjectId ignoredId,
                                                      QString* baseKey,
                                                      quint64* nextSuffix) const
{
    const QString displayBase = requestedName.trimmed();
    *baseKey = normalizedName(displayBase);
    *nextSuffix = 2;

    const auto existing = m_nameIndex.find(*baseKey);
    const auto reserved = m_reservedNameIndex.find(*baseKey);
    if ((existing == m_nameIndex.end() || existing->second == ignoredId) &&
        (reserved == m_reservedNameIndex.end() || reserved->second == ignoredId))
    {
        return displayBase;
    }

    const auto suffixIterator = m_nextNameSuffix.find(*baseKey);
    quint64 suffix = suffixIterator == m_nextNameSuffix.end()
        ? 2
        : std::max<quint64>(2, suffixIterator->second);
    while (true)
    {
        const QString candidate = QStringLiteral("%1 (%2)").arg(displayBase).arg(suffix);
        const auto candidateIterator = m_nameIndex.find(normalizedName(candidate));
        const auto reservedIterator = m_reservedNameIndex.find(normalizedName(candidate));
        if ((candidateIterator == m_nameIndex.end() || candidateIterator->second == ignoredId) &&
            (reservedIterator == m_reservedNameIndex.end() ||
             reservedIterator->second == ignoredId))
        {
            *nextSuffix = suffix == std::numeric_limits<quint64>::max() ? suffix : suffix + 1;
            return candidate;
        }
        if (suffix == std::numeric_limits<quint64>::max())
        {
            return {};
        }
        ++suffix;
    }
}

void ApplicationRuntime::consumeNextObjectIdLocked(ObjectId allocatedId) noexcept
{
    if (allocatedId == std::numeric_limits<ObjectId>::max())
    {
        m_idExhausted = true;
    }
    else
    {
        m_nextObjectId = allocatedId + 1;
    }
}

Common::Diagnostic ApplicationRuntime::diagnostic(const QString& code,
                                                  const QString& message,
                                                  const QString& detail,
                                                  ObjectId objectId) const
{
    return {QStringLiteral("model"),
            code,
            message,
            detail,
            false,
            QString(),
            QStringLiteral("model.runtime"),
            objectId == InvalidObjectId ? QString() : QString::number(objectId)};
}

CreateObjectResult ApplicationRuntime::createObject(const CreateObjectRequest& request)
{
    CreateObjectResult result;
    const QString requestedName = request.name.trimmed();
    if (requestedName.isEmpty())
    {
        result.add(diagnostic(QStringLiteral("MODEL-NAME-EMPTY"),
                              QStringLiteral("Object name is empty."),
                              QStringLiteral("Provide a non-empty object name.")));
        return result;
    }
    if (request.type == DataObjectType::Invalid)
    {
        result.add(diagnostic(QStringLiteral("MODEL-TYPE-INVALID"),
                              QStringLiteral("Object type is invalid."),
                              QStringLiteral("Choose a registered non-invalid object type.")));
        return result;
    }
    if (request.type == DataObjectType::Geometry || request.type == DataObjectType::Mesh)
    {
        result.add(diagnostic(QStringLiteral("MODEL-DOMAIN-MANAGER-REQUIRED"),
                              QStringLiteral("Domain objects require their owning manager."),
                              QStringLiteral("Create Geometry and Mesh objects through their domain manager.")));
        return result;
    }

    QWriteLocker locker(&m_lock);
    if (request.parentId != InvalidObjectId &&
        m_objects.find(request.parentId) == m_objects.end())
    {
        result.add(diagnostic(QStringLiteral("MODEL-PARENT-NOT-FOUND"),
                              QStringLiteral("Parent object does not exist."),
                              QStringLiteral("The parent must be registered before its child."),
                              request.parentId));
        return result;
    }
    if (m_idExhausted || m_nextObjectId == InvalidObjectId)
    {
        result.add(diagnostic(QStringLiteral("MODEL-ID-EXHAUSTED"),
                              QStringLiteral("No object IDs remain."),
                              QStringLiteral("The monotonic object ID range is exhausted.")));
        return result;
    }

    QString baseKey;
    quint64 nextSuffix = 2;
    const QString uniqueName = allocateUniqueNameLocked(
        requestedName, InvalidObjectId, &baseKey, &nextSuffix);
    if (uniqueName.isEmpty())
    {
        result.add(diagnostic(QStringLiteral("MODEL-NAME-EXHAUSTED"),
                              QStringLiteral("No unique object name remains."),
                              QStringLiteral("The deterministic name suffix range is exhausted.")));
        return result;
    }

    const ObjectId id = m_nextObjectId;
    const QString nameKey = normalizedName(uniqueName);
    std::unique_ptr<DataObject> object;
    try
    {
        object.reset(new DataObject(id,
                                    uniqueName,
                                    request.type,
                                    request.parentId,
                                    request.visible,
                                    request.selected,
                                    request.metadata));
        m_objects.emplace(id, std::move(object));
        m_nameIndex.emplace(nameKey, id);
        if (request.parentId != InvalidObjectId)
        {
            m_children[request.parentId].insert(id);
        }
        auto& storedSuffix = m_nextNameSuffix[baseKey];
        storedSuffix = std::max(storedSuffix, nextSuffix);
    }
    catch (const std::bad_alloc&)
    {
        if (request.parentId != InvalidObjectId)
        {
            const auto childrenIterator = m_children.find(request.parentId);
            if (childrenIterator != m_children.end())
            {
                childrenIterator->second.erase(id);
                if (childrenIterator->second.empty())
                {
                    m_children.erase(childrenIterator);
                }
            }
        }
        m_nameIndex.erase(nameKey);
        m_objects.erase(id);
        result.add(diagnostic(QStringLiteral("MODEL-RESOURCE-EXHAUSTED"),
                              QStringLiteral("Object creation could not allocate memory."),
                              QStringLiteral("No object or index entry was retained.")));
        return result;
    }

    const auto created = m_objects.find(id);
    result.object = created->second->snapshot();
    consumeNextObjectIdLocked(id);
    locker.unlock();
    emitChange(ModelChangeType::Added, id, result.object);
    return result;
}

ReserveObjectResult ApplicationRuntime::reserveDomainObject(const CreateObjectRequest& request)
{
    ReserveObjectResult result;
    const QString requestedName = request.name.trimmed();
    if (requestedName.isEmpty())
    {
        result.add(diagnostic(QStringLiteral("MODEL-NAME-EMPTY"),
                              QStringLiteral("Object name is empty."),
                              QStringLiteral("Provide a non-empty object name.")));
        return result;
    }
    if (request.type != DataObjectType::Geometry && request.type != DataObjectType::Mesh)
    {
        result.add(diagnostic(QStringLiteral("MODEL-DOMAIN-TYPE-INVALID"),
                              QStringLiteral("The reserved object type is invalid."),
                              QStringLiteral("Only Geometry or Mesh records use domain registration.")));
        return result;
    }

    QWriteLocker locker(&m_lock);
    if (request.parentId != InvalidObjectId &&
        m_objects.find(request.parentId) == m_objects.end())
    {
        result.add(diagnostic(QStringLiteral("MODEL-PARENT-NOT-FOUND"),
                              QStringLiteral("Parent object does not exist."),
                              QStringLiteral("The parent must be published before its child."),
                              request.parentId));
        return result;
    }
    if (m_idExhausted || m_nextObjectId == InvalidObjectId)
    {
        result.add(diagnostic(QStringLiteral("MODEL-ID-EXHAUSTED"),
                              QStringLiteral("No object IDs remain."),
                              QStringLiteral("The monotonic object ID range is exhausted.")));
        return result;
    }

    QString baseKey;
    quint64 nextSuffix = 2;
    const QString uniqueName = allocateUniqueNameLocked(
        requestedName, InvalidObjectId, &baseKey, &nextSuffix);
    if (uniqueName.isEmpty())
    {
        result.add(diagnostic(QStringLiteral("MODEL-NAME-EXHAUSTED"),
                              QStringLiteral("No unique object name remains."),
                              QStringLiteral("The deterministic name suffix range is exhausted.")));
        return result;
    }

    const ObjectId id = m_nextObjectId;
    const QString nameKey = normalizedName(uniqueName);
    consumeNextObjectIdLocked(id);
    try
    {
        ReservedObject reserved;
        reserved.object.reset(new DataObject(id,
                                             uniqueName,
                                             request.type,
                                             request.parentId,
                                             request.visible,
                                             request.selected,
                                             request.metadata));
        const auto objectInsert = m_reservedObjects.emplace(id, std::move(reserved));
        const auto nameInsert = m_reservedNameIndex.emplace(nameKey, id);
        if (!objectInsert.second || !nameInsert.second)
        {
            m_reservedNameIndex.erase(nameKey);
            m_reservedObjects.erase(id);
            result.add(diagnostic(QStringLiteral("MODEL-RESERVATION-CONFLICT"),
                                  QStringLiteral("The domain object could not be reserved."),
                                  QStringLiteral("An internal ID or name claim already exists."),
                                  id));
            return result;
        }
        auto& storedSuffix = m_nextNameSuffix[baseKey];
        storedSuffix = std::max(storedSuffix, nextSuffix);
    }
    catch (const std::bad_alloc&)
    {
        m_reservedNameIndex.erase(nameKey);
        m_reservedObjects.erase(id);
        result.add(diagnostic(QStringLiteral("MODEL-RESOURCE-EXHAUSTED"),
                              QStringLiteral("Object reservation could not allocate memory."),
                              QStringLiteral("No reservation or index entry was retained."),
                              id));
        return result;
    }

    result.reservation = DomainObjectReservation(this, id, request.type);
    return result;
}

PublishObjectResult ApplicationRuntime::publishDomainObject(
    DomainObjectReservation& reservation)
{
    PublishObjectResult result;
    if (!reservation.isValid() || reservation.m_owner != this)
    {
        result.add(diagnostic(QStringLiteral("MODEL-RESERVATION-INVALID"),
                              QStringLiteral("The domain reservation is invalid."),
                              QStringLiteral("Use the active reservation returned by this runtime.")));
        return result;
    }

    if (m_beforeDomainPublish)
    {
        try
        {
            const auto checkpoint =
                m_beforeDomainPublish(reservation.m_id, reservation.m_type);
            for (const auto& item : checkpoint.diagnostics)
            {
                result.add(item);
            }
        }
        catch (...)
        {
            result.add(diagnostic(QStringLiteral("MODEL-PUBLISH-CHECKPOINT-FAILED"),
                                  QStringLiteral("Domain publication checkpoint failed."),
                                  QStringLiteral("The checkpoint raised an exception."),
                                  reservation.m_id));
        }
        if (!result.succeeded())
        {
            return result;
        }
    }

    QWriteLocker locker(&m_lock);
    const auto reservedIterator = m_reservedObjects.find(reservation.m_id);
    if (reservedIterator == m_reservedObjects.end() ||
        !reservedIterator->second.object ||
        reservedIterator->second.object->m_type != reservation.m_type)
    {
        result.add(diagnostic(QStringLiteral("MODEL-RESERVATION-NOT-FOUND"),
                              QStringLiteral("The domain reservation no longer exists."),
                              QStringLiteral("It may already have been published or cancelled."),
                              reservation.m_id));
        return result;
    }

    DataObject& reservedObject = *reservedIterator->second.object;
    const ObjectId parentId = reservedObject.m_parentId;
    if (parentId != InvalidObjectId && m_objects.find(parentId) == m_objects.end())
    {
        result.add(diagnostic(QStringLiteral("MODEL-PARENT-NOT-FOUND"),
                              QStringLiteral("Parent object no longer exists."),
                              QStringLiteral("Cancel the reservation or choose a published parent."),
                              parentId));
        return result;
    }

    const QString nameKey = normalizedName(reservedObject.m_name);
    const auto nameClaim = m_reservedNameIndex.find(nameKey);
    if (nameClaim == m_reservedNameIndex.end() || nameClaim->second != reservation.m_id ||
        m_nameIndex.find(nameKey) != m_nameIndex.end() ||
        m_objects.find(reservation.m_id) != m_objects.end())
    {
        result.add(diagnostic(QStringLiteral("MODEL-RESERVATION-INCONSISTENT"),
                              QStringLiteral("The reserved name claim is inconsistent."),
                              QStringLiteral("The record remains hidden and can be cancelled."),
                              reservation.m_id));
        return result;
    }

    bool nameInserted = false;
    bool childIndexTouched = false;
    bool childInserted = false;
    try
    {
        nameInserted = m_nameIndex.emplace(nameKey, reservation.m_id).second;
        if (!nameInserted)
        {
            result.add(diagnostic(QStringLiteral("MODEL-RESERVATION-CONFLICT"),
                                  QStringLiteral("The reserved name is no longer available."),
                                  QStringLiteral("The record remains hidden and can be cancelled."),
                                  reservation.m_id));
            return result;
        }

        if (parentId != InvalidObjectId)
        {
            auto& children = m_children[parentId];
            childIndexTouched = true;
            childInserted = children.insert(reservation.m_id).second;
            if (!childInserted)
            {
                m_nameIndex.erase(nameKey);
                result.add(diagnostic(QStringLiteral("MODEL-RESERVATION-CONFLICT"),
                                      QStringLiteral("The parent already contains the reserved child ID."),
                                      QStringLiteral("The record remains hidden and can be cancelled."),
                                      reservation.m_id));
                return result;
            }
        }

        const auto objectInsert = m_objects.emplace(
            reservation.m_id, std::move(reservedIterator->second.object));
        if (!objectInsert.second)
        {
            if (childInserted)
            {
                auto children = m_children.find(parentId);
                if (children != m_children.end())
                {
                    children->second.erase(reservation.m_id);
                    if (children->second.empty())
                    {
                        m_children.erase(children);
                    }
                }
            }
            m_nameIndex.erase(nameKey);
            result.add(diagnostic(QStringLiteral("MODEL-RESERVATION-CONFLICT"),
                                  QStringLiteral("The reserved ID is no longer available."),
                                  QStringLiteral("The record remains hidden and can be cancelled."),
                                  reservation.m_id));
            return result;
        }
    }
    catch (const std::bad_alloc&)
    {
        if (childIndexTouched)
        {
            const auto children = m_children.find(parentId);
            if (children != m_children.end())
            {
                children->second.erase(reservation.m_id);
                if (children->second.empty())
                {
                    m_children.erase(children);
                }
            }
        }
        if (nameInserted)
        {
            m_nameIndex.erase(nameKey);
        }
        result.add(diagnostic(QStringLiteral("MODEL-RESOURCE-EXHAUSTED"),
                              QStringLiteral("The reserved object could not be published."),
                              QStringLiteral("The record remains hidden and can be cancelled."),
                              reservation.m_id));
        return result;
    }

    m_reservedNameIndex.erase(nameKey);
    m_reservedObjects.erase(reservedIterator);
    const auto published = m_objects.find(reservation.m_id);
    result.object = published->second->snapshot();
    const ObjectId publishedId = reservation.m_id;
    reservation.reset();
    locker.unlock();
    emitChange(ModelChangeType::Added, publishedId, result.object);
    return result;
}

Common::OperationResult ApplicationRuntime::cancelDomainObject(
    DomainObjectReservation& reservation)
{
    Common::OperationResult result;
    if (!reservation.isValid() || reservation.m_owner != this)
    {
        result.add(diagnostic(QStringLiteral("MODEL-RESERVATION-INVALID"),
                              QStringLiteral("The domain reservation is invalid."),
                              QStringLiteral("Use the active reservation returned by this runtime.")));
        return result;
    }

    QWriteLocker locker(&m_lock);
    const auto reservedIterator = m_reservedObjects.find(reservation.m_id);
    if (reservedIterator == m_reservedObjects.end() ||
        !reservedIterator->second.object ||
        reservedIterator->second.object->m_type != reservation.m_type)
    {
        result.add(diagnostic(QStringLiteral("MODEL-RESERVATION-NOT-FOUND"),
                              QStringLiteral("The domain reservation no longer exists."),
                              QStringLiteral("It may already have been published or cancelled."),
                              reservation.m_id));
        return result;
    }

    m_reservedNameIndex.erase(normalizedName(reservedIterator->second.object->m_name));
    m_reservedObjects.erase(reservedIterator);
    reservation.reset();
    return result;
}

QVector<ObjectId> ApplicationRuntime::reservedObjectIds(DataObjectType type) const
{
    QReadLocker locker(&m_lock);
    QVector<ObjectId> ids;
    for (const auto& entry : m_reservedObjects)
    {
        if (entry.second.object && entry.second.object->m_type == type)
        {
            ids.append(entry.first);
        }
    }
    return ids;
}

std::optional<DataObjectSnapshot> ApplicationRuntime::findById(ObjectId id) const
{
    QReadLocker locker(&m_lock);
    const auto iterator = m_objects.find(id);
    if (iterator == m_objects.end())
    {
        return std::nullopt;
    }
    return iterator->second->snapshot();
}

std::optional<DataObjectSnapshot> ApplicationRuntime::findByName(const QString& name) const
{
    const QString key = normalizedName(name);
    if (key.isEmpty())
    {
        return std::nullopt;
    }

    QReadLocker locker(&m_lock);
    const auto nameIterator = m_nameIndex.find(key);
    if (nameIterator == m_nameIndex.end())
    {
        return std::nullopt;
    }
    const auto objectIterator = m_objects.find(nameIterator->second);
    return objectIterator == m_objects.end()
        ? std::optional<DataObjectSnapshot>{}
        : std::optional<DataObjectSnapshot>{objectIterator->second->snapshot()};
}

QVector<DataObjectSnapshot> ApplicationRuntime::snapshots() const
{
    QReadLocker locker(&m_lock);
    QVector<DataObjectSnapshot> values;
    values.reserve(static_cast<int>(m_objects.size()));
    for (const auto& entry : m_objects)
    {
        values.append(entry.second->snapshot());
    }
    return values;
}

QVector<ObjectId> ApplicationRuntime::childIds(ObjectId parentId) const
{
    QReadLocker locker(&m_lock);
    QVector<ObjectId> values;
    const auto iterator = m_children.find(parentId);
    if (iterator == m_children.end())
    {
        return values;
    }
    values.reserve(static_cast<int>(iterator->second.size()));
    for (const ObjectId id : iterator->second)
    {
        values.append(id);
    }
    return values;
}

Common::OperationResult ApplicationRuntime::renameObject(ObjectId id,
                                                         const QString& requestedName)
{
    Common::OperationResult result;
    if (requestedName.trimmed().isEmpty())
    {
        result.add(diagnostic(QStringLiteral("MODEL-NAME-EMPTY"),
                              QStringLiteral("Object name is empty."),
                              QStringLiteral("Provide a non-empty object name."),
                              id));
        return result;
    }

    QWriteLocker locker(&m_lock);
    const auto objectIterator = m_objects.find(id);
    if (objectIterator == m_objects.end())
    {
        result.add(diagnostic(QStringLiteral("MODEL-OBJECT-NOT-FOUND"),
                              QStringLiteral("Object does not exist."),
                              QStringLiteral("The requested object cannot be renamed."),
                              id));
        return result;
    }

    QString baseKey;
    quint64 nextSuffix = 2;
    const QString uniqueName = allocateUniqueNameLocked(
        requestedName, id, &baseKey, &nextSuffix);
    if (uniqueName.isEmpty())
    {
        result.add(diagnostic(QStringLiteral("MODEL-NAME-EXHAUSTED"),
                              QStringLiteral("No unique object name remains."),
                              QStringLiteral("The deterministic name suffix range is exhausted."),
                              id));
        return result;
    }

    DataObject& object = *objectIterator->second;
    const QString oldKey = normalizedName(object.m_name);
    const QString newKey = normalizedName(uniqueName);
    if (oldKey != newKey)
    {
        m_nameIndex.emplace(newKey, id);
        m_nameIndex.erase(oldKey);
    }
    object.m_name = uniqueName;
    auto& storedSuffix = m_nextNameSuffix[baseKey];
    storedSuffix = std::max(storedSuffix, nextSuffix);
    const auto snapshot = object.snapshot();
    locker.unlock();
    emitChange(ModelChangeType::Updated, id, snapshot);
    return result;
}

bool ApplicationRuntime::wouldCreateCycleLocked(ObjectId id, ObjectId parentId) const
{
    ObjectId current = parentId;
    while (current != InvalidObjectId)
    {
        if (current == id)
        {
            return true;
        }
        const auto iterator = m_objects.find(current);
        if (iterator == m_objects.end())
        {
            return false;
        }
        current = iterator->second->m_parentId;
    }
    return false;
}

Common::OperationResult ApplicationRuntime::setParent(ObjectId id, ObjectId parentId)
{
    Common::OperationResult result;
    QWriteLocker locker(&m_lock);
    const auto objectIterator = m_objects.find(id);
    if (objectIterator == m_objects.end())
    {
        result.add(diagnostic(QStringLiteral("MODEL-OBJECT-NOT-FOUND"),
                              QStringLiteral("Object does not exist."),
                              QStringLiteral("The requested object cannot be reparented."),
                              id));
        return result;
    }
    if (parentId == id)
    {
        result.add(diagnostic(QStringLiteral("MODEL-PARENT-SELF"),
                              QStringLiteral("An object cannot be its own parent."),
                              QStringLiteral("Choose another parent or make the object a root."),
                              id));
        return result;
    }
    if (parentId != InvalidObjectId && m_objects.find(parentId) == m_objects.end())
    {
        result.add(diagnostic(QStringLiteral("MODEL-PARENT-NOT-FOUND"),
                              QStringLiteral("Parent object does not exist."),
                              QStringLiteral("Choose an existing object as the parent."),
                              parentId));
        return result;
    }
    if (wouldCreateCycleLocked(id, parentId))
    {
        result.add(diagnostic(QStringLiteral("MODEL-PARENT-CYCLE"),
                              QStringLiteral("Parent change would create a cycle."),
                              QStringLiteral("The object hierarchy must remain acyclic."),
                              id));
        return result;
    }

    DataObject& object = *objectIterator->second;
    if (object.m_parentId == parentId)
    {
        return result;
    }
    if (parentId != InvalidObjectId)
    {
        m_children[parentId].insert(id);
    }
    if (object.m_parentId != InvalidObjectId)
    {
        const auto oldChildren = m_children.find(object.m_parentId);
        if (oldChildren != m_children.end())
        {
            oldChildren->second.erase(id);
            if (oldChildren->second.empty())
            {
                m_children.erase(oldChildren);
            }
        }
    }
    object.m_parentId = parentId;
    const auto snapshot = object.snapshot();
    locker.unlock();
    emitChange(ModelChangeType::Updated, id, snapshot);
    return result;
}

Common::OperationResult ApplicationRuntime::setVisible(ObjectId id, bool visible)
{
    Common::OperationResult result;
    QWriteLocker locker(&m_lock);
    const auto iterator = m_objects.find(id);
    if (iterator == m_objects.end())
    {
        result.add(diagnostic(QStringLiteral("MODEL-OBJECT-NOT-FOUND"),
                              QStringLiteral("Object does not exist."),
                              QStringLiteral("Visibility cannot be updated."),
                              id));
        return result;
    }
    iterator->second->m_visible = visible;
    const auto snapshot = iterator->second->snapshot();
    locker.unlock();
    emitChange(ModelChangeType::Updated, id, snapshot);
    return result;
}

Common::OperationResult ApplicationRuntime::setSelected(ObjectId id, bool selected)
{
    Common::OperationResult result;
    QWriteLocker locker(&m_lock);
    const auto iterator = m_objects.find(id);
    if (iterator == m_objects.end())
    {
        result.add(diagnostic(QStringLiteral("MODEL-OBJECT-NOT-FOUND"),
                              QStringLiteral("Object does not exist."),
                              QStringLiteral("Selection cannot be updated."),
                              id));
        return result;
    }
    iterator->second->m_selected = selected;
    const auto snapshot = iterator->second->snapshot();
    locker.unlock();
    emitChange(ModelChangeType::Updated, id, snapshot);
    return result;
}

Common::OperationResult ApplicationRuntime::setMetadataValue(ObjectId id,
                                                              const QString& key,
                                                              const QVariant& value)
{
    Common::OperationResult result;
    const QString cleanKey = key.trimmed();
    if (cleanKey.isEmpty())
    {
        result.add(diagnostic(QStringLiteral("MODEL-METADATA-KEY-EMPTY"),
                              QStringLiteral("Metadata key is empty."),
                              QStringLiteral("Provide a non-empty metadata key."),
                              id));
        return result;
    }

    QWriteLocker locker(&m_lock);
    const auto iterator = m_objects.find(id);
    if (iterator == m_objects.end())
    {
        result.add(diagnostic(QStringLiteral("MODEL-OBJECT-NOT-FOUND"),
                              QStringLiteral("Object does not exist."),
                              QStringLiteral("Metadata cannot be updated."),
                              id));
        return result;
    }
    iterator->second->m_metadata.insert(cleanKey, value);
    const auto snapshot = iterator->second->snapshot();
    locker.unlock();
    emitChange(ModelChangeType::Updated, id, snapshot);
    return result;
}

Common::OperationResult ApplicationRuntime::removeMetadataValue(ObjectId id, const QString& key)
{
    Common::OperationResult result;
    const QString cleanKey = key.trimmed();
    if (cleanKey.isEmpty())
    {
        result.add(diagnostic(QStringLiteral("MODEL-METADATA-KEY-EMPTY"),
                              QStringLiteral("Metadata key is empty."),
                              QStringLiteral("Provide a non-empty metadata key."),
                              id));
        return result;
    }

    QWriteLocker locker(&m_lock);
    const auto iterator = m_objects.find(id);
    if (iterator == m_objects.end())
    {
        result.add(diagnostic(QStringLiteral("MODEL-OBJECT-NOT-FOUND"),
                              QStringLiteral("Object does not exist."),
                              QStringLiteral("Metadata cannot be removed."),
                              id));
        return result;
    }
    iterator->second->m_metadata.remove(cleanKey);
    const auto snapshot = iterator->second->snapshot();
    locker.unlock();
    emitChange(ModelChangeType::Updated, id, snapshot);
    return result;
}

bool ApplicationRuntime::hasReservedChildLocked(ObjectId parentId) const
{
    for (const auto& entry : m_reservedObjects)
    {
        if (entry.second.object && entry.second.object->m_parentId == parentId)
        {
            return true;
        }
    }
    return false;
}

Common::OperationResult ApplicationRuntime::removeObjectLocked(
    ObjectId id,
    std::optional<DataObjectType> expectedType)
{
    Common::OperationResult result;
    const auto objectIterator = m_objects.find(id);
    if (objectIterator == m_objects.end())
    {
        result.add(diagnostic(QStringLiteral("MODEL-OBJECT-NOT-FOUND"),
                              QStringLiteral("Object does not exist."),
                              QStringLiteral("The requested object cannot be removed."),
                              id));
        return result;
    }
    if (expectedType.has_value() && objectIterator->second->m_type != *expectedType)
    {
        result.add(diagnostic(QStringLiteral("MODEL-OBJECT-TYPE-MISMATCH"),
                              QStringLiteral("Object type does not match the requested operation."),
                              QStringLiteral("Typed removal cannot delete a different domain type."),
                              id));
        return result;
    }
    if (!expectedType.has_value() &&
        (objectIterator->second->m_type == DataObjectType::Geometry ||
         objectIterator->second->m_type == DataObjectType::Mesh))
    {
        result.add(diagnostic(QStringLiteral("MODEL-DOMAIN-MANAGER-REQUIRED"),
                              QStringLiteral("Domain objects require their owning manager."),
                              QStringLiteral("Delete Geometry and Mesh objects through their domain manager."),
                              id));
        return result;
    }
    const auto childrenIterator = m_children.find(id);
    if ((childrenIterator != m_children.end() && !childrenIterator->second.empty()) ||
        hasReservedChildLocked(id))
    {
        result.add(diagnostic(QStringLiteral("MODEL-PARENT-HAS-CHILDREN"),
                              QStringLiteral("Object still has children."),
                              QStringLiteral("Remove, cancel, or reparent child objects first."),
                              id));
        return result;
    }

    const ObjectId parentId = objectIterator->second->m_parentId;
    if (parentId != InvalidObjectId)
    {
        const auto parentChildren = m_children.find(parentId);
        if (parentChildren != m_children.end())
        {
            parentChildren->second.erase(id);
            if (parentChildren->second.empty())
            {
                m_children.erase(parentChildren);
            }
        }
    }
    m_nameIndex.erase(normalizedName(objectIterator->second->m_name));
    m_children.erase(id);
    m_objects.erase(objectIterator);
    return result;
}

Common::OperationResult ApplicationRuntime::removeObject(ObjectId id)
{
    QWriteLocker locker(&m_lock);
    const auto iterator = m_objects.find(id);
    const auto snapshot = iterator == m_objects.end()
        ? std::optional<DataObjectSnapshot>{}
        : std::optional<DataObjectSnapshot>{iterator->second->snapshot()};
    auto result = removeObjectLocked(id, std::nullopt);
    locker.unlock();
    if (result.succeeded() && snapshot.has_value())
    {
        emitChange(ModelChangeType::Removed, id, snapshot);
    }
    return result;
}

Common::OperationResult ApplicationRuntime::removeTypedObject(ObjectId id,
                                                              DataObjectType expectedType)
{
    Common::OperationResult result;
    if (expectedType != DataObjectType::Geometry && expectedType != DataObjectType::Mesh)
    {
        result.add(diagnostic(QStringLiteral("MODEL-DOMAIN-TYPE-INVALID"),
                              QStringLiteral("Typed removal requires a domain object type."),
                              QStringLiteral("Only Geometry or Mesh records use typed removal."),
                              id));
        return result;
    }

    QWriteLocker locker(&m_lock);
    const auto iterator = m_objects.find(id);
    const auto snapshot = iterator == m_objects.end()
        ? std::optional<DataObjectSnapshot>{}
        : std::optional<DataObjectSnapshot>{iterator->second->snapshot()};
    result = removeObjectLocked(id, expectedType);
    locker.unlock();
    if (result.succeeded() && snapshot.has_value())
    {
        emitChange(ModelChangeType::Removed, id, snapshot);
    }
    return result;
}

int ApplicationRuntime::objectCount() const
{
    QReadLocker locker(&m_lock);
    return static_cast<int>(m_objects.size());
}

Common::OperationResult ApplicationRuntime::validateIndexes() const
{
    Common::OperationResult result;
    QReadLocker locker(&m_lock);

    if (m_nameIndex.size() != m_objects.size())
    {
        result.add(diagnostic(QStringLiteral("MODEL-INDEX-INCONSISTENT"),
                              QStringLiteral("Name and object index sizes differ."),
                              QStringLiteral("Every object must have exactly one name entry.")));
    }

    for (const auto& entry : m_objects)
    {
        const ObjectId id = entry.first;
        const DataObject& object = *entry.second;
        if (object.m_id != id)
        {
            result.add(diagnostic(QStringLiteral("MODEL-INDEX-INCONSISTENT"),
                                  QStringLiteral("Object ID does not match its index key."),
                                  QStringLiteral("The ID index is corrupt."),
                                  id));
        }
        const auto nameIterator = m_nameIndex.find(normalizedName(object.m_name));
        if (nameIterator == m_nameIndex.end() || nameIterator->second != id)
        {
            result.add(diagnostic(QStringLiteral("MODEL-INDEX-INCONSISTENT"),
                                  QStringLiteral("Object name is not indexed correctly."),
                                  QStringLiteral("The name index is missing or points to another object."),
                                  id));
        }
        if (object.m_parentId != InvalidObjectId)
        {
            const auto parentIterator = m_objects.find(object.m_parentId);
            const auto childrenIterator = m_children.find(object.m_parentId);
            if (parentIterator == m_objects.end() ||
                childrenIterator == m_children.end() ||
                childrenIterator->second.find(id) == childrenIterator->second.end())
            {
                result.add(diagnostic(QStringLiteral("MODEL-INDEX-INCONSISTENT"),
                                      QStringLiteral("Object parent index is inconsistent."),
                                      QStringLiteral("The parent or reverse child entry is missing."),
                                      id));
            }
        }
        if (wouldCreateCycleLocked(id, object.m_parentId))
        {
            result.add(diagnostic(QStringLiteral("MODEL-INDEX-INCONSISTENT"),
                                  QStringLiteral("Object hierarchy contains a cycle."),
                                  QStringLiteral("Parent chains must terminate at a root."),
                                  id));
        }
    }

    for (const auto& nameEntry : m_nameIndex)
    {
        const auto objectIterator = m_objects.find(nameEntry.second);
        if (objectIterator == m_objects.end() ||
            normalizedName(objectIterator->second->m_name) != nameEntry.first)
        {
            result.add(diagnostic(QStringLiteral("MODEL-INDEX-INCONSISTENT"),
                                  QStringLiteral("Name index points to an invalid object."),
                                  QStringLiteral("The reverse name mapping is corrupt."),
                                  nameEntry.second));
        }
    }

    for (const auto& childrenEntry : m_children)
    {
        if (m_objects.find(childrenEntry.first) == m_objects.end())
        {
            result.add(diagnostic(QStringLiteral("MODEL-INDEX-INCONSISTENT"),
                                  QStringLiteral("Child index has a missing parent."),
                                  QStringLiteral("Remove the orphan reverse index."),
                                  childrenEntry.first));
        }
        for (const ObjectId childId : childrenEntry.second)
        {
            const auto childIterator = m_objects.find(childId);
            if (childIterator == m_objects.end() ||
                childIterator->second->m_parentId != childrenEntry.first)
            {
                result.add(diagnostic(QStringLiteral("MODEL-INDEX-INCONSISTENT"),
                                      QStringLiteral("Child index points to an invalid child."),
                                      QStringLiteral("The child's forward parent does not match."),
                                      childId));
            }
        }
    }

    if (m_reservedNameIndex.size() != m_reservedObjects.size())
    {
        result.add(diagnostic(QStringLiteral("MODEL-RESERVATION-INDEX-INCONSISTENT"),
                              QStringLiteral("Reserved name and object index sizes differ."),
                              QStringLiteral("Every reservation must have exactly one hidden name claim.")));
    }
    for (const auto& entry : m_reservedObjects)
    {
        const ObjectId id = entry.first;
        const DataObject* object = entry.second.object.get();
        if (!object || object->m_id != id ||
            (object->m_type != DataObjectType::Geometry &&
             object->m_type != DataObjectType::Mesh))
        {
            result.add(diagnostic(QStringLiteral("MODEL-RESERVATION-INDEX-INCONSISTENT"),
                                  QStringLiteral("Reserved object identity is inconsistent."),
                                  QStringLiteral("Reservations require a matching domain object."),
                                  id));
            continue;
        }
        const QString nameKey = normalizedName(object->m_name);
        const auto nameIterator = m_reservedNameIndex.find(nameKey);
        if (nameIterator == m_reservedNameIndex.end() || nameIterator->second != id ||
            m_nameIndex.find(nameKey) != m_nameIndex.end())
        {
            result.add(diagnostic(QStringLiteral("MODEL-RESERVATION-INDEX-INCONSISTENT"),
                                  QStringLiteral("Reserved object name is not claimed correctly."),
                                  QStringLiteral("Hidden and published names must remain unique."),
                                  id));
        }
        if (object->m_parentId != InvalidObjectId &&
            m_objects.find(object->m_parentId) == m_objects.end())
        {
            result.add(diagnostic(QStringLiteral("MODEL-RESERVATION-INDEX-INCONSISTENT"),
                                  QStringLiteral("Reserved object parent does not exist."),
                                  QStringLiteral("A reservation may reference only a published parent."),
                                  id));
        }
    }
    for (const auto& nameEntry : m_reservedNameIndex)
    {
        const auto objectIterator = m_reservedObjects.find(nameEntry.second);
        if (objectIterator == m_reservedObjects.end() ||
            !objectIterator->second.object ||
            normalizedName(objectIterator->second.object->m_name) != nameEntry.first)
        {
            result.add(diagnostic(QStringLiteral("MODEL-RESERVATION-INDEX-INCONSISTENT"),
                                  QStringLiteral("Reserved name points to an invalid reservation."),
                                  QStringLiteral("Remove the orphan hidden name claim."),
                                  nameEntry.second));
        }
    }
    return result;
}
}
