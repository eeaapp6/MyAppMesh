#include "MeshData.h"

#include "ApplicationRuntime.h"

#include <utility>

namespace AppMesh::Model
{
namespace
{
Common::Diagnostic meshDiagnostic(const QString& code,
                                  const QString& message,
                                  const QString& detail,
                                  const QString& path,
                                  ObjectId objectId = InvalidObjectId)
{
    return {QStringLiteral("model"),
            code,
            message,
            detail,
            true,
            path,
            QStringLiteral("mesh.validate"),
            objectId == InvalidObjectId ? QString() : QString::number(objectId)};
}
}

MeshData::MeshData(GeneratorProvenance generator,
                   ObjectId geometryId,
                   QVector<MeshKernel> kernels,
                   QVariantMap commonParameters,
                   QVariantMap generatorParameters,
                   QVariantMap displayMetadata)
    : m_generator(std::move(generator)),
      m_geometryId(geometryId),
      m_kernels(std::move(kernels)),
      m_commonParameters(std::move(commonParameters)),
      m_generatorParameters(std::move(generatorParameters)),
      m_displayMetadata(std::move(displayMetadata))
{
}

GeneratorProvenance MeshData::generator() const
{
    return m_generator;
}

ObjectId MeshData::geometryId() const noexcept
{
    return m_geometryId;
}

QVector<MeshKernel> MeshData::kernels() const
{
    return m_kernels;
}

QVariantMap MeshData::commonParameters() const
{
    return m_commonParameters;
}

QVariantMap MeshData::generatorParameters() const
{
    return m_generatorParameters;
}

QVariantMap MeshData::displayMetadata() const
{
    return m_displayMetadata;
}

Common::OperationResult MeshData::validate(const ApplicationRuntime& runtime) const
{
    Common::OperationResult result;

    if (m_generator.key.trimmed().isEmpty())
    {
        result.add(meshDiagnostic(QStringLiteral("MESH-GENERATOR-KEY-EMPTY"),
                                  QStringLiteral("Mesh generator key is empty."),
                                  QStringLiteral("Provide the stable plugin registry key."),
                                  QStringLiteral("generator/key")));
    }
    if (m_generator.version.trimmed().isEmpty())
    {
        result.add(meshDiagnostic(QStringLiteral("MESH-GENERATOR-VERSION-EMPTY"),
                                  QStringLiteral("Mesh generator version is empty."),
                                  QStringLiteral("Provide the opaque generator version value."),
                                  QStringLiteral("generator/version")));
    }

    if (m_geometryId != InvalidObjectId)
    {
        const std::optional<DataObjectSnapshot> geometry = runtime.findById(m_geometryId);
        if (!geometry.has_value())
        {
            result.add(meshDiagnostic(QStringLiteral("MESH-GEOMETRY-NOT-FOUND"),
                                      QStringLiteral("Source geometry does not exist."),
                                      QStringLiteral("The geometryId must identify a published runtime record."),
                                      QStringLiteral("geometryId"),
                                      m_geometryId));
        }
        else if (geometry->type != DataObjectType::Geometry)
        {
            result.add(meshDiagnostic(QStringLiteral("MESH-GEOMETRY-TYPE-MISMATCH"),
                                      QStringLiteral("Source object is not Geometry."),
                                      QStringLiteral("The geometryId must identify DataObjectType::Geometry."),
                                      QStringLiteral("geometryId"),
                                      m_geometryId));
        }
    }

    if (m_kernels.isEmpty())
    {
        result.add(meshDiagnostic(QStringLiteral("MESH-KERNELS-EMPTY"),
                                  QStringLiteral("Mesh data has no kernels."),
                                  QStringLiteral("A valid mesh candidate must contain at least one kernel."),
                                  QStringLiteral("kernels")));
    }

    for (int kernelIndex = 0; kernelIndex < m_kernels.size(); ++kernelIndex)
    {
        const Common::OperationResult kernelResult = m_kernels.at(kernelIndex).validate(kernelIndex);
        for (const Common::Diagnostic& diagnostic : kernelResult.diagnostics)
        {
            result.add(diagnostic);
        }
    }
    return result;
}
}
