#include "GeometryObject.h"

#include <set>

namespace AppMesh::Model
{
namespace
{
Common::Diagnostic validationDiagnostic(const QString& code,
                                        const QString& message,
                                        const QString& detail,
                                        const QString& path)
{
    return {QStringLiteral("validation"),
            code,
            message,
            detail,
            false,
            path,
            QStringLiteral("geometry.validate"),
            QString()};
}

QString entityPath(const GeometryTopologyEntity& entity, int index)
{
    return entity.id == InvalidGeometryEntityId
        ? QStringLiteral("topology/entities/%1").arg(index)
        : QStringLiteral("topology/entities/%1").arg(entity.id);
}
}

bool isValidGeometryType(GeometryType type) noexcept
{
    return type == GeometryType::BRep || type == GeometryType::Step ||
        type == GeometryType::Iges;
}

bool isValidTopologyEntityType(TopologyEntityType type) noexcept
{
    return type == TopologyEntityType::Vertex || type == TopologyEntityType::Edge ||
        type == TopologyEntityType::Face || type == TopologyEntityType::Solid;
}

Common::OperationResult validateGeometryObject(const GeometryObject& geometry)
{
    Common::OperationResult result;
    if (!isValidGeometryType(geometry.type))
    {
        result.add(validationDiagnostic(QStringLiteral("GEO-TYPE-INVALID"),
                                        QStringLiteral("Geometry type is invalid."),
                                        QStringLiteral("Use BRep, Step, or Iges."),
                                        QStringLiteral("geometry/type")));
    }

    const QString providerKey = geometry.adapter.providerKey.trimmed();
    const QString modelKey = geometry.adapter.modelKey.trimmed();
    if (providerKey.isEmpty())
    {
        result.add(validationDiagnostic(QStringLiteral("GEO-ADAPTER-PROVIDER-MISSING"),
                                        QStringLiteral("Geometry adapter provider is missing."),
                                        QStringLiteral("Provide the stable FITK adapter provider key."),
                                        QStringLiteral("adapter/providerKey")));
    }
    if (modelKey.isEmpty())
    {
        result.add(validationDiagnostic(QStringLiteral("GEO-ADAPTER-MODEL-MISSING"),
                                        QStringLiteral("Geometry adapter model key is missing."),
                                        QStringLiteral("Provide the stable FITK-managed model key."),
                                        QStringLiteral("adapter/modelKey")));
    }
    if (geometry.topology.isEmpty())
    {
        result.add(validationDiagnostic(QStringLiteral("GEO-TOPOLOGY-EMPTY"),
                                        QStringLiteral("Geometry topology is empty."),
                                        QStringLiteral("Provide at least one validated topology entity."),
                                        QStringLiteral("topology/entities")));
    }

    std::set<GeometryEntityId> ids;
    for (int index = 0; index < geometry.topology.size(); ++index)
    {
        const auto& entity = geometry.topology[index];
        const QString path = entityPath(entity, index);
        if (entity.id == InvalidGeometryEntityId)
        {
            result.add(validationDiagnostic(QStringLiteral("GEO-ENTITY-ID-INVALID"),
                                            QStringLiteral("Geometry entity ID is invalid."),
                                            QStringLiteral("Geometry entity ID zero is reserved."),
                                            path + QStringLiteral("/id")));
        }
        else if (!ids.insert(entity.id).second)
        {
            result.add(validationDiagnostic(QStringLiteral("GEO-ENTITY-ID-DUPLICATE"),
                                            QStringLiteral("Geometry entity ID is duplicated."),
                                            QStringLiteral("Entity IDs must be unique within one geometry."),
                                            path + QStringLiteral("/id")));
        }
        if (!isValidTopologyEntityType(entity.type))
        {
            result.add(validationDiagnostic(QStringLiteral("GEO-ENTITY-TYPE-INVALID"),
                                            QStringLiteral("Topology entity type is invalid."),
                                            QStringLiteral("Use Vertex, Edge, Face, or Solid."),
                                            path + QStringLiteral("/type")));
        }
        if (entity.adapterEntityKey.trimmed().isEmpty())
        {
            result.add(validationDiagnostic(QStringLiteral("GEO-ENTITY-ADAPTER-KEY-MISSING"),
                                            QStringLiteral("Topology adapter key is missing."),
                                            QStringLiteral("Every entity must map to a stable FITK topology key."),
                                            path + QStringLiteral("/adapterEntityKey")));
        }
    }
    return result;
}

GeometryObjectSnapshot makeGeometrySnapshot(const DataObjectSnapshot& common,
                                            const GeometryObject& geometry)
{
    GeometryObjectSnapshot snapshot;
    snapshot.common = common;
    snapshot.type = geometry.type;
    snapshot.adapterProviderKey = geometry.adapter.providerKey;
    snapshot.metadata = geometry.metadata;
    snapshot.displayMetadata = geometry.displayMetadata;
    snapshot.topology.reserve(geometry.topology.size());
    for (const auto& entity : geometry.topology)
    {
        snapshot.topology.append({entity.id, entity.type, entity.metadata});
    }
    return snapshot;
}
}
