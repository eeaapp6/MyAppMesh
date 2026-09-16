#include "MeshKernel.h"

#include <cmath>
#include <set>
#include <utility>

namespace AppMesh::Model
{
namespace
{
Common::Diagnostic meshDiagnostic(const QString& code,
                                  const QString& message,
                                  const QString& detail,
                                  const QString& path)
{
    return {QStringLiteral("model"),
            code,
            message,
            detail,
            true,
            path,
            QStringLiteral("mesh.validate"),
            QString()};
}

QString kernelPath(int kernelIndex)
{
    return QStringLiteral("kernels/%1").arg(kernelIndex);
}

QString entityPath(int kernelIndex,
                   const QString& collection,
                   quint64 entityId)
{
    return QStringLiteral("%1/%2/%3")
        .arg(kernelPath(kernelIndex), collection, QString::number(entityId));
}

bool dimensionAllowsCell(MeshDimension dimension, int cellDimension)
{
    switch (dimension)
    {
    case MeshDimension::D2:
        return cellDimension == 1 || cellDimension == 2;
    case MeshDimension::D3:
        return cellDimension >= 1 && cellDimension <= 3;
    case MeshDimension::Mixed:
        return cellDimension >= 1 && cellDimension <= 3;
    }
    return false;
}
}

MeshKernel::MeshKernel(MeshDimension dimension,
                       QVector<Node> nodes,
                       QVector<Element> elements,
                       QVector<MeshSet> sets,
                       QVariantMap additionalData)
    : m_dimension(dimension),
      m_nodes(std::move(nodes)),
      m_elements(std::move(elements)),
      m_sets(std::move(sets)),
      m_additionalData(std::move(additionalData))
{
}

MeshDimension MeshKernel::dimension() const noexcept
{
    return m_dimension;
}

QVector<Node> MeshKernel::nodes() const
{
    return m_nodes;
}

QVector<Element> MeshKernel::elements() const
{
    return m_elements;
}

QVector<MeshSet> MeshKernel::sets() const
{
    return m_sets;
}

QVariantMap MeshKernel::additionalData() const
{
    return m_additionalData;
}

Common::OperationResult MeshKernel::validate(int kernelIndex) const
{
    Common::OperationResult result;
    const QString rootPath = kernelPath(kernelIndex);

    if (!isValidMeshDimension(m_dimension))
    {
        result.add(meshDiagnostic(QStringLiteral("MESH-DIMENSION-INVALID"),
                                  QStringLiteral("Mesh dimension is invalid."),
                                  QStringLiteral("Use D2, D3, or Mixed."),
                                  rootPath + QStringLiteral("/dimension")));
    }
    if (m_nodes.isEmpty())
    {
        result.add(meshDiagnostic(QStringLiteral("MESH-NODES-EMPTY"),
                                  QStringLiteral("Mesh kernel has no nodes."),
                                  QStringLiteral("A valid kernel must contain at least one node."),
                                  rootPath + QStringLiteral("/nodes")));
    }
    if (m_elements.isEmpty())
    {
        result.add(meshDiagnostic(QStringLiteral("MESH-ELEMENTS-EMPTY"),
                                  QStringLiteral("Mesh kernel has no elements."),
                                  QStringLiteral("A valid kernel must contain at least one element."),
                                  rootPath + QStringLiteral("/elements")));
    }

    std::set<NodeId> nodeIds;
    for (int nodeIndex = 0; nodeIndex < m_nodes.size(); ++nodeIndex)
    {
        const Node& node = m_nodes.at(nodeIndex);
        const QString path = entityPath(kernelIndex, QStringLiteral("nodes"), node.id);
        if (node.id == InvalidNodeId)
        {
            result.add(meshDiagnostic(QStringLiteral("MESH-NODE-ID-INVALID"),
                                      QStringLiteral("Node ID is invalid."),
                                      QStringLiteral("Node at ordinal %1 must have a nonzero ID.")
                                          .arg(nodeIndex),
                                      path + QStringLiteral("/id")));
        }
        else if (!nodeIds.insert(node.id).second)
        {
            result.add(meshDiagnostic(QStringLiteral("MESH-NODE-ID-DUPLICATE"),
                                      QStringLiteral("Node ID is duplicated."),
                                      QStringLiteral("Node ID %1 appears again at ordinal %2.")
                                          .arg(node.id)
                                          .arg(nodeIndex),
                                      path + QStringLiteral("/id")));
        }

        if (!std::isfinite(node.x) || !std::isfinite(node.y) || !std::isfinite(node.z))
        {
            result.add(meshDiagnostic(QStringLiteral("MESH-NODE-COORDINATE-NONFINITE"),
                                      QStringLiteral("Node coordinates are not finite."),
                                      QStringLiteral("Coordinates x, y, and z must be finite values."),
                                      path + QStringLiteral("/coordinates")));
        }
    }

    std::set<ElementId> elementIds;
    bool containsTopDimension = false;
    for (int elementIndex = 0; elementIndex < m_elements.size(); ++elementIndex)
    {
        const Element& element = m_elements.at(elementIndex);
        const QString path = entityPath(kernelIndex, QStringLiteral("elements"), element.id);
        if (element.id == InvalidElementId)
        {
            result.add(meshDiagnostic(QStringLiteral("MESH-ELEMENT-ID-INVALID"),
                                      QStringLiteral("Element ID is invalid."),
                                      QStringLiteral("Element at ordinal %1 must have a nonzero ID.")
                                          .arg(elementIndex),
                                      path + QStringLiteral("/id")));
        }
        else if (!elementIds.insert(element.id).second)
        {
            result.add(meshDiagnostic(QStringLiteral("MESH-ELEMENT-ID-DUPLICATE"),
                                      QStringLiteral("Element ID is duplicated."),
                                      QStringLiteral("Element ID %1 appears again at ordinal %2.")
                                          .arg(element.id)
                                          .arg(elementIndex),
                                      path + QStringLiteral("/id")));
        }

        const int arity = cellTypeArity(element.cellType);
        const int elementDimension = cellTypeDimension(element.cellType);
        if (!isValidCellType(element.cellType))
        {
            result.add(meshDiagnostic(QStringLiteral("MESH-CELL-TYPE-INVALID"),
                                      QStringLiteral("Cell type is invalid."),
                                      QStringLiteral("Use one of the frozen fixed-arity cell types."),
                                      path + QStringLiteral("/cellType")));
        }
        else
        {
            if (element.nodeIds.size() != arity)
            {
                result.add(meshDiagnostic(QStringLiteral("MESH-CELL-ARITY-MISMATCH"),
                                          QStringLiteral("Cell node count does not match its type."),
                                          QStringLiteral("Expected %1 node IDs but received %2.")
                                              .arg(arity)
                                              .arg(element.nodeIds.size()),
                                          path + QStringLiteral("/nodeIds")));
            }
            if (!dimensionAllowsCell(m_dimension, elementDimension))
            {
                result.add(meshDiagnostic(QStringLiteral("MESH-CELL-DIMENSION-MISMATCH"),
                                          QStringLiteral("Cell dimension is incompatible with the kernel."),
                                          QStringLiteral("Cell dimension %1 is not allowed by this kernel dimension.")
                                              .arg(elementDimension),
                                          path + QStringLiteral("/cellType")));
            }
            if ((m_dimension == MeshDimension::D2 && elementDimension == 2) ||
                (m_dimension == MeshDimension::D3 && elementDimension == 3))
            {
                containsTopDimension = true;
            }
        }

        std::set<NodeId> referencedNodeIds;
        for (int nodeIndex = 0; nodeIndex < element.nodeIds.size(); ++nodeIndex)
        {
            const NodeId nodeId = element.nodeIds.at(nodeIndex);
            const QString nodePath = path + QStringLiteral("/nodeIds/%1").arg(nodeIndex);
            if (nodeId == InvalidNodeId)
            {
                result.add(meshDiagnostic(QStringLiteral("MESH-ELEMENT-NODE-ID-INVALID"),
                                          QStringLiteral("Element references an invalid node ID."),
                                          QStringLiteral("Referenced node IDs must be nonzero."),
                                          nodePath));
            }
            else if (!referencedNodeIds.insert(nodeId).second)
            {
                result.add(meshDiagnostic(QStringLiteral("MESH-ELEMENT-NODE-ID-DUPLICATE"),
                                          QStringLiteral("Element repeats a node ID."),
                                          QStringLiteral("Node ID %1 appears more than once in this element.")
                                              .arg(nodeId),
                                          nodePath));
            }
            else if (nodeIds.find(nodeId) == nodeIds.end())
            {
                result.add(meshDiagnostic(QStringLiteral("MESH-ELEMENT-NODE-NOT-FOUND"),
                                          QStringLiteral("Element references a missing node."),
                                          QStringLiteral("Node ID %1 is not present in this kernel.")
                                              .arg(nodeId),
                                          nodePath));
            }
        }
    }

    if (!m_elements.isEmpty() &&
        ((m_dimension == MeshDimension::D2 || m_dimension == MeshDimension::D3) &&
         !containsTopDimension))
    {
        result.add(meshDiagnostic(QStringLiteral("MESH-DIMENSION-CONTENT-MISMATCH"),
                                  QStringLiteral("Kernel contents do not establish its declared dimension."),
                                  m_dimension == MeshDimension::D2
                                      ? QStringLiteral("A D2 kernel must contain at least one 2D cell.")
                                      : QStringLiteral("A D3 kernel must contain at least one 3D cell."),
                                  rootPath + QStringLiteral("/dimension")));
    }

    std::set<SetId> setIds;
    for (int setIndex = 0; setIndex < m_sets.size(); ++setIndex)
    {
        const MeshSet& meshSet = m_sets.at(setIndex);
        const QString path = entityPath(kernelIndex, QStringLiteral("sets"), meshSet.id);
        if (meshSet.id == InvalidSetId)
        {
            result.add(meshDiagnostic(QStringLiteral("MESH-SET-ID-INVALID"),
                                      QStringLiteral("Set ID is invalid."),
                                      QStringLiteral("Set at ordinal %1 must have a nonzero ID.")
                                          .arg(setIndex),
                                      path + QStringLiteral("/id")));
        }
        else if (!setIds.insert(meshSet.id).second)
        {
            result.add(meshDiagnostic(QStringLiteral("MESH-SET-ID-DUPLICATE"),
                                      QStringLiteral("Set ID is duplicated."),
                                      QStringLiteral("Set ID %1 appears again at ordinal %2.")
                                          .arg(meshSet.id)
                                          .arg(setIndex),
                                      path + QStringLiteral("/id")));
        }
        if (meshSet.name.trimmed().isEmpty())
        {
            result.add(meshDiagnostic(QStringLiteral("MESH-SET-NAME-EMPTY"),
                                      QStringLiteral("Mesh set name is empty."),
                                      QStringLiteral("Provide a non-empty mesh set name."),
                                      path + QStringLiteral("/name")));
        }

        const bool validEntityType = isValidMeshEntityType(meshSet.entityType);
        if (!validEntityType)
        {
            result.add(meshDiagnostic(QStringLiteral("MESH-SET-ENTITY-TYPE-INVALID"),
                                      QStringLiteral("Mesh set entity type is invalid."),
                                      QStringLiteral("A mesh set must reference nodes or elements."),
                                      path + QStringLiteral("/entityType")));
        }
        if (meshSet.entityIds.isEmpty())
        {
            result.add(meshDiagnostic(QStringLiteral("MESH-SET-ENTITIES-EMPTY"),
                                      QStringLiteral("Mesh set has no members."),
                                      QStringLiteral("A declared mesh set must reference at least one entity."),
                                      path + QStringLiteral("/entityIds")));
        }

        std::set<quint64> memberIds;
        for (int memberIndex = 0; memberIndex < meshSet.entityIds.size(); ++memberIndex)
        {
            const quint64 memberId = meshSet.entityIds.at(memberIndex);
            const QString memberPath = path + QStringLiteral("/entityIds/%1").arg(memberIndex);
            if (memberId == 0)
            {
                result.add(meshDiagnostic(QStringLiteral("MESH-SET-ENTITY-ID-INVALID"),
                                          QStringLiteral("Mesh set references an invalid entity ID."),
                                          QStringLiteral("Referenced entity IDs must be nonzero."),
                                          memberPath));
                continue;
            }
            if (!memberIds.insert(memberId).second)
            {
                result.add(meshDiagnostic(QStringLiteral("MESH-SET-ENTITY-ID-DUPLICATE"),
                                          QStringLiteral("Mesh set contains a duplicate entity ID."),
                                          QStringLiteral("Entity ID %1 is repeated in this set.")
                                              .arg(memberId),
                                          memberPath));
            }
            if (!validEntityType)
            {
                continue;
            }

            const bool referencedExists = meshSet.entityType == MeshEntityType::Node
                ? nodeIds.find(static_cast<NodeId>(memberId)) != nodeIds.end()
                : elementIds.find(static_cast<ElementId>(memberId)) != elementIds.end();
            if (!referencedExists)
            {
                const bool existsAsOtherType = meshSet.entityType == MeshEntityType::Node
                    ? elementIds.find(static_cast<ElementId>(memberId)) != elementIds.end()
                    : nodeIds.find(static_cast<NodeId>(memberId)) != nodeIds.end();
                result.add(meshDiagnostic(
                    existsAsOtherType
                        ? QStringLiteral("MESH-SET-ENTITY-TYPE-MISMATCH")
                        : QStringLiteral("MESH-SET-ENTITY-NOT-FOUND"),
                    existsAsOtherType
                        ? QStringLiteral("Mesh set member has the wrong entity type.")
                        : QStringLiteral("Mesh set references a missing entity."),
                    QStringLiteral("Entity ID %1 does not resolve as the set's declared type.")
                        .arg(memberId),
                    memberPath));
            }
        }
    }

    return result;
}
}
