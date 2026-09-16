#pragma once

#include <QDateTime>
#include <QString>
#include <QVariantMap>

#include <QtGlobal>

namespace AppMesh::Model
{
using ObjectId = quint64;
constexpr ObjectId InvalidObjectId = 0;

enum class DataObjectType : quint8
{
    Invalid = 0,
    Generic,
    Geometry,
    Mesh
};

struct DataObjectSnapshot
{
    ObjectId id = InvalidObjectId;
    QString name;
    DataObjectType type = DataObjectType::Invalid;
    bool visible = true;
    bool selected = false;
    ObjectId parentId = InvalidObjectId;
    QDateTime createdAt;
    QVariantMap metadata;
};

class ApplicationRuntime;

class DataObject final
{
public:
    DataObject(const DataObject&) = delete;
    DataObject& operator=(const DataObject&) = delete;
    DataObject(DataObject&&) = delete;
    DataObject& operator=(DataObject&&) = delete;

    ObjectId id() const noexcept;
    const QString& name() const noexcept;
    DataObjectType type() const noexcept;
    bool isVisible() const noexcept;
    bool isSelected() const noexcept;
    ObjectId parentId() const noexcept;
    const QDateTime& createdAt() const noexcept;
    QVariantMap metadata() const;
    DataObjectSnapshot snapshot() const;

private:
    friend class ApplicationRuntime;

    DataObject(ObjectId id,
               QString name,
               DataObjectType type,
               ObjectId parentId,
               bool visible,
               bool selected,
               QVariantMap metadata);

    ObjectId m_id = InvalidObjectId;
    QString m_name;
    DataObjectType m_type = DataObjectType::Invalid;
    bool m_visible = true;
    bool m_selected = false;
    ObjectId m_parentId = InvalidObjectId;
    QDateTime m_createdAt;
    QVariantMap m_metadata;
};
}
