#pragma once

#include "MeshData.h"

#include <optional>

namespace AppMesh::Model
{
struct MeshAdapterReference
{
    QString providerKey;
    QString meshKey;
    QVariantMap metadata;

    bool isValid() const noexcept;
};

struct MeshDataCreatorRequest
{
    ObjectId geometryId = InvalidObjectId;
    QVariantMap commonParameters;
    QVariantMap generatorParameters;
    QVariantMap displayMetadata;
};

struct CreatedMeshData
{
    MeshData mesh;
    MeshAdapterReference adapter;
};

struct MeshDataCreatorResult : Common::OperationResult
{
    std::optional<CreatedMeshData> created;
};

// Implementations are registered through shared ownership. MeshManager invokes
// create() without holding its data lock and never retains a borrowed pointer.
class MeshDataCreator
{
public:
    virtual ~MeshDataCreator() = default;
    virtual MeshDataCreatorResult create(const MeshDataCreatorRequest& request) const = 0;
};
}
