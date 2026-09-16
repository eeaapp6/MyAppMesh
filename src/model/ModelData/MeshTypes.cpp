#include "MeshTypes.h"

namespace AppMesh::Model
{
bool isValidMeshDimension(MeshDimension dimension) noexcept
{
    switch (dimension)
    {
    case MeshDimension::D2:
    case MeshDimension::D3:
    case MeshDimension::Mixed:
        return true;
    }
    return false;
}

bool isValidCellType(CellType cellType) noexcept
{
    return cellTypeArity(cellType) != 0;
}

bool isValidMeshEntityType(MeshEntityType entityType) noexcept
{
    switch (entityType)
    {
    case MeshEntityType::Node:
    case MeshEntityType::Element:
        return true;
    }
    return false;
}

int cellTypeArity(CellType cellType) noexcept
{
    switch (cellType)
    {
    case CellType::Line2:
        return 2;
    case CellType::Triangle3:
        return 3;
    case CellType::Quadrilateral4:
    case CellType::Tetrahedron4:
        return 4;
    case CellType::Hexahedron8:
        return 8;
    }
    return 0;
}

int cellTypeDimension(CellType cellType) noexcept
{
    switch (cellType)
    {
    case CellType::Line2:
        return 1;
    case CellType::Triangle3:
    case CellType::Quadrilateral4:
        return 2;
    case CellType::Tetrahedron4:
    case CellType::Hexahedron8:
        return 3;
    }
    return 0;
}
}
