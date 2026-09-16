#pragma once

#include "MeshTypes.h"
#include "common/AppDiagnostic.h"

#include <QVariantMap>

namespace AppMesh::Model
{
class MeshKernel final
{
public:
    MeshKernel() = default;
    MeshKernel(MeshDimension dimension,
               QVector<Node> nodes,
               QVector<Element> elements,
               QVector<MeshSet> sets = {},
               QVariantMap additionalData = {});

    MeshDimension dimension() const noexcept;
    QVector<Node> nodes() const;
    QVector<Element> elements() const;
    QVector<MeshSet> sets() const;
    QVariantMap additionalData() const;

    Common::OperationResult validate(int kernelIndex = 0) const;

private:
    MeshDimension m_dimension = MeshDimension::D2;
    QVector<Node> m_nodes;
    QVector<Element> m_elements;
    QVector<MeshSet> m_sets;
    QVariantMap m_additionalData;
};
}
