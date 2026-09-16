#include "MeshDataCreator.h"

namespace AppMesh::Model
{
bool MeshAdapterReference::isValid() const noexcept
{
    return !providerKey.trimmed().isEmpty() && !meshKey.trimmed().isEmpty();
}
}
