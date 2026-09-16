#pragma once

#include "DataObject.h"
#include "MeshKernel.h"

#include <QVariantMap>

namespace AppMesh::Model
{
class ApplicationRuntime;

class MeshData final
{
public:
    MeshData() = default;
    MeshData(GeneratorProvenance generator,
             ObjectId geometryId,
             QVector<MeshKernel> kernels,
             QVariantMap commonParameters = {},
             QVariantMap generatorParameters = {},
             QVariantMap displayMetadata = {});

    GeneratorProvenance generator() const;
    ObjectId geometryId() const noexcept;
    QVector<MeshKernel> kernels() const;
    QVariantMap commonParameters() const;
    QVariantMap generatorParameters() const;
    QVariantMap displayMetadata() const;

    Common::OperationResult validate(const ApplicationRuntime& runtime) const;

private:
    GeneratorProvenance m_generator;
    ObjectId m_geometryId = InvalidObjectId;
    QVector<MeshKernel> m_kernels;
    QVariantMap m_commonParameters;
    QVariantMap m_generatorParameters;
    QVariantMap m_displayMetadata;
};
}
