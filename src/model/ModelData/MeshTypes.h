#pragma once

#include <QString>
#include <QVector>

#include <QtGlobal>

namespace AppMesh::Model
{
using NodeId = quint64;
using ElementId = quint64;
using SetId = quint64;

constexpr NodeId InvalidNodeId = 0;
constexpr ElementId InvalidElementId = 0;
constexpr SetId InvalidSetId = 0;

enum class MeshDimension : quint8
{
    D2 = 1,
    D3,
    Mixed
};

enum class CellType : quint8
{
    Line2 = 1,
    Triangle3,
    Quadrilateral4,
    Tetrahedron4,
    Hexahedron8
};

enum class MeshEntityType : quint8
{
    Node = 1,
    Element
};

struct Node
{
    NodeId id = InvalidNodeId;
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
};

struct Element
{
    ElementId id = InvalidElementId;
    CellType cellType = CellType::Line2;
    QVector<NodeId> nodeIds;
};

struct MeshSet
{
    SetId id = InvalidSetId;
    QString name;
    MeshEntityType entityType = MeshEntityType::Node;
    QVector<quint64> entityIds;
};

struct GeneratorProvenance
{
    QString key;
    QString version;
};

bool isValidMeshDimension(MeshDimension dimension) noexcept;
bool isValidCellType(CellType cellType) noexcept;
bool isValidMeshEntityType(MeshEntityType entityType) noexcept;
int cellTypeArity(CellType cellType) noexcept;
int cellTypeDimension(CellType cellType) noexcept;
}
