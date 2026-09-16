#include "DataObject.h"

#include <utility>

namespace AppMesh::Model
{
DataObject::DataObject(ObjectId id,
                       QString name,
                       DataObjectType type,
                       ObjectId parentId,
                       bool visible,
                       bool selected,
                       QVariantMap metadata)
    : m_id(id),
      m_name(std::move(name)),
      m_type(type),
      m_visible(visible),
      m_selected(selected),
      m_parentId(parentId),
      m_createdAt(QDateTime::currentDateTimeUtc()),
      m_metadata(std::move(metadata))
{
}

ObjectId DataObject::id() const noexcept
{
    return m_id;
}

const QString& DataObject::name() const noexcept
{
    return m_name;
}

DataObjectType DataObject::type() const noexcept
{
    return m_type;
}

bool DataObject::isVisible() const noexcept
{
    return m_visible;
}

bool DataObject::isSelected() const noexcept
{
    return m_selected;
}

ObjectId DataObject::parentId() const noexcept
{
    return m_parentId;
}

const QDateTime& DataObject::createdAt() const noexcept
{
    return m_createdAt;
}

QVariantMap DataObject::metadata() const
{
    return m_metadata;
}

DataObjectSnapshot DataObject::snapshot() const
{
    DataObjectSnapshot value;
    value.id = m_id;
    value.name = m_name;
    value.type = m_type;
    value.visible = m_visible;
    value.selected = m_selected;
    value.parentId = m_parentId;
    value.createdAt = m_createdAt;
    value.metadata = m_metadata;
    return value;
}
}
