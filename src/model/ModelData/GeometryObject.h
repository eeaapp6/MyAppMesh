#pragma once

#include "DataObject.h"
#include "common/AppDiagnostic.h"

#include <QString>
#include <QVariantMap>
#include <QVector>

namespace AppMesh::Model
{
using GeometryEntityId = quint64;
constexpr GeometryEntityId InvalidGeometryEntityId = 0;

enum class GeometryType : quint8
{
    Invalid = 0,
    BRep,
    Step,
    Iges
};

enum class TopologyEntityType : quint8
{
    Invalid = 0,
    Vertex,
    Edge,
    Face,
    Solid
};

struct GeometryAdapterReference
{
    QString providerKey;
    QString modelKey;
    QVariantMap metadata;
};

struct GeometryTopologyEntity
{
    GeometryEntityId id = InvalidGeometryEntityId;
    TopologyEntityType type = TopologyEntityType::Invalid;
    QString adapterEntityKey;
    QVariantMap metadata;
};

struct GeometryTopologyEntitySnapshot
{
    GeometryEntityId id = InvalidGeometryEntityId;
    TopologyEntityType type = TopologyEntityType::Invalid;
    QVariantMap metadata;
};

// A detached staging payload. It identifies FITK-managed data with stable keys;
// it never owns or exposes a FITK/OCC pointer.
struct GeometryObject
{
    GeometryType type = GeometryType::Invalid;
    GeometryAdapterReference adapter;
    QVector<GeometryTopologyEntity> topology;
    QVariantMap metadata;
    QVariantMap displayMetadata;
};

struct GeometryObjectSnapshot
{
    DataObjectSnapshot common;
    GeometryType type = GeometryType::Invalid;
    QString adapterProviderKey;
    QVector<GeometryTopologyEntitySnapshot> topology;
    QVariantMap metadata;
    QVariantMap displayMetadata;
};

bool isValidGeometryType(GeometryType type) noexcept;
bool isValidTopologyEntityType(TopologyEntityType type) noexcept;
Common::OperationResult validateGeometryObject(const GeometryObject& geometry);
GeometryObjectSnapshot makeGeometrySnapshot(const DataObjectSnapshot& common,
                                            const GeometryObject& geometry);
}
