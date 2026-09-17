#include "ImportGeometryOperator.h"

#include <QDir>
#include <QFileInfo>
#include <QSet>

#include <exception>

namespace AppMesh::OperatorsModel
{
namespace
{
Common::Diagnostic importDiagnostic(const QString& code,
                                    const QString& message,
                                    const QString& detail,
                                    const QString& path)
{
    return {QStringLiteral("operator"),
            code,
            message,
            detail,
            false,
            path,
            QStringLiteral("geometry.import"),
            QString(),
            QString()};
}

QString normalizedExtension(QString extension)
{
    extension = extension.trimmed().toCaseFolded();
    while (extension.startsWith(QLatin1Char('.')))
    {
        extension.remove(0, 1);
    }
    return extension;
}

bool isStringParameter(const QVariantMap& parameters, const QString& key)
{
    const auto found = parameters.find(key);
    return found == parameters.end() || found->type() == QVariant::String;
}
}

QString ImportGeometryOperator::operationKey()
{
    return QStringLiteral("geometry.import");
}

QString ImportGeometryOperator::filePathParameter()
{
    return QStringLiteral("filePath");
}

QString ImportGeometryOperator::workDirectoryParameter()
{
    return QStringLiteral("workDirectory");
}

QString ImportGeometryOperator::formatKeyParameter()
{
    return QStringLiteral("formatKey");
}

QString ImportGeometryOperator::requestedNameParameter()
{
    return QStringLiteral("requestedName");
}

QString ImportGeometryOperator::metadataParameter()
{
    return QStringLiteral("metadata");
}

ImportGeometryOperator::ImportGeometryOperator(
    Model::GeometryManager& geometryManager,
    std::shared_ptr<const GeometryReaderRegistry> readers)
    : m_geometryManager(geometryManager), m_readers(std::move(readers))
{
}

QString ImportGeometryOperator::key() const
{
    return operationKey();
}

GeometryReadRequest ImportGeometryOperator::requestFromInput(
    const Operators::OperatorInput& input) const
{
    GeometryReadRequest request;
    request.filePath = input.parameters.value(filePathParameter()).toString().trimmed();
    request.workDirectory = input.parameters.value(workDirectoryParameter()).toString().trimmed();
    if (!request.filePath.isEmpty())
    {
        request.filePath = QDir::cleanPath(request.filePath);
    }
    if (!request.workDirectory.isEmpty())
    {
        request.workDirectory = QDir::cleanPath(request.workDirectory);
    }
    request.formatKey = input.parameters.value(formatKeyParameter()).toString().trimmed();
    request.requestedName = input.parameters.value(requestedNameParameter()).toString().trimmed();
    request.metadata = input.parameters.value(metadataParameter()).toMap();
    if (request.requestedName.isEmpty())
    {
        request.requestedName = QFileInfo(request.filePath).completeBaseName().trimmed();
    }
    return request;
}

Common::OperationResult ImportGeometryOperator::validate(
    const Operators::OperatorInput& input) const
{
    Common::OperationResult result;
    if (input.operationKey.trimmed() != operationKey())
    {
        result.add(importDiagnostic(QStringLiteral("GEO-IMPORT-OPERATION-MISMATCH"),
                                    QStringLiteral("The operation key does not select geometry import."),
                                    input.operationKey,
                                    QStringLiteral("operationKey")));
    }
    const QStringList stringKeys{filePathParameter(),
                                 workDirectoryParameter(),
                                 formatKeyParameter(),
                                 requestedNameParameter()};
    for (const auto& key : stringKeys)
    {
        if (!isStringParameter(input.parameters, key))
        {
            result.add(importDiagnostic(QStringLiteral("GEO-IMPORT-PARAMETER-TYPE-INVALID"),
                                        QStringLiteral("A geometry import parameter has the wrong type."),
                                        QStringLiteral("The parameter must be a QString value."),
                                        QStringLiteral("parameters/") + key));
        }
    }
    const auto metadata = input.parameters.find(metadataParameter());
    if (metadata != input.parameters.end() && metadata->type() != QVariant::Map)
    {
        result.add(importDiagnostic(QStringLiteral("GEO-IMPORT-PARAMETER-TYPE-INVALID"),
                                    QStringLiteral("Geometry metadata has the wrong type."),
                                    QStringLiteral("The metadata parameter must be a QVariantMap."),
                                    QStringLiteral("parameters/metadata")));
    }
    if (!result.succeeded())
    {
        return result;
    }

    const auto request = requestFromInput(input);
    if (request.filePath.isEmpty())
    {
        result.add(importDiagnostic(QStringLiteral("GEO-IMPORT-PATH-EMPTY"),
                                    QStringLiteral("The geometry file path is empty."),
                                    QStringLiteral("Choose an existing readable geometry file."),
                                    QStringLiteral("filePath")));
    }
    if (request.workDirectory.isEmpty())
    {
        result.add(importDiagnostic(QStringLiteral("GEO-IMPORT-WORKDIR-EMPTY"),
                                    QStringLiteral("The work directory is empty."),
                                    QStringLiteral("Choose an existing working directory."),
                                    QStringLiteral("workDirectory")));
    }
    if (request.formatKey.isEmpty())
    {
        result.add(importDiagnostic(QStringLiteral("GEO-IMPORT-FORMAT-EMPTY"),
                                    QStringLiteral("The geometry format key is empty."),
                                    QStringLiteral("Select one registered geometry reader."),
                                    QStringLiteral("formatKey")));
    }
    if (!result.succeeded())
    {
        return result;
    }

    const QFileInfo file(request.filePath);
    if (!file.isAbsolute())
    {
        result.add(importDiagnostic(QStringLiteral("GEO-IMPORT-PATH-NOT-ABSOLUTE"),
                                    QStringLiteral("The geometry file path must be absolute."),
                                    QStringLiteral("Relative worker paths are not accepted."),
                                    request.filePath));
    }
    else if (!file.exists())
    {
        result.add(importDiagnostic(QStringLiteral("GEO-IMPORT-FILE-NOT-FOUND"),
                                    QStringLiteral("The geometry file does not exist."),
                                    QStringLiteral("Choose an existing file."),
                                    request.filePath));
    }
    else if (!file.isFile())
    {
        result.add(importDiagnostic(QStringLiteral("GEO-IMPORT-PATH-NOT-FILE"),
                                    QStringLiteral("The geometry path is not a regular file."),
                                    QStringLiteral("Directories cannot be imported."),
                                    request.filePath));
    }
    else if (!file.isReadable())
    {
        result.add(importDiagnostic(QStringLiteral("GEO-IMPORT-FILE-NOT-READABLE"),
                                    QStringLiteral("The geometry file is not readable."),
                                    QStringLiteral("Check file permissions."),
                                    request.filePath));
    }

    const QFileInfo workDirectory(request.workDirectory);
    if (!workDirectory.isAbsolute() || !workDirectory.exists() || !workDirectory.isDir())
    {
        result.add(importDiagnostic(QStringLiteral("GEO-IMPORT-WORKDIR-INVALID"),
                                    QStringLiteral("The work directory is invalid."),
                                    QStringLiteral("Use an existing absolute directory."),
                                    request.workDirectory));
    }
    if (request.requestedName.isEmpty())
    {
        result.add(importDiagnostic(QStringLiteral("GEO-IMPORT-NAME-EMPTY"),
                                    QStringLiteral("The imported geometry name is empty."),
                                    QStringLiteral("Provide a name or use a file with a non-empty base name."),
                                    QStringLiteral("requestedName")));
    }

    const auto reader = m_readers ? m_readers->find(request.formatKey) : nullptr;
    if (!reader)
    {
        result.add(importDiagnostic(QStringLiteral("GEO-IMPORT-READER-NOT-FOUND"),
                                    QStringLiteral("No geometry reader is registered for the format."),
                                    request.formatKey,
                                    QStringLiteral("formatKey")));
        return result;
    }
    QSet<QString> supported;
    for (const auto& extension : reader->supportedExtensions())
    {
        supported.insert(normalizedExtension(extension));
    }
    const QString suffix = normalizedExtension(file.suffix());
    if (suffix.isEmpty() || !supported.contains(suffix))
    {
        result.add(importDiagnostic(QStringLiteral("GEO-IMPORT-EXTENSION-UNSUPPORTED"),
                                    QStringLiteral("The geometry file extension is not supported by the reader."),
                                    suffix,
                                    request.filePath));
    }
    return result;
}

Operators::OperatorResult ImportGeometryOperator::failure(
    const Common::Diagnostic& diagnostic,
    QVector<Common::Diagnostic> diagnostics) const
{
    if (diagnostics.isEmpty())
    {
        diagnostics.append(diagnostic);
    }
    auto failed = Operators::OperatorResult::failure(
        Operators::errorInfoFromDiagnostic(diagnostic), std::move(diagnostics));
    return std::move(*failed);
}

Operators::OperatorResult ImportGeometryOperator::execute(
    const Operators::OperatorInput& input)
{
    const auto validation = validate(input);
    if (!validation.succeeded())
    {
        return failure(validation.diagnostics.front(), validation.diagnostics);
    }
    const auto request = requestFromInput(input);
    const auto reader = m_readers->find(request.formatKey);

    GeometryReadResult read;
    try
    {
        read = reader->read(request);
    }
    catch (const std::exception& exception)
    {
        const auto diagnostic = importDiagnostic(
            QStringLiteral("GEO-IMPORT-READER-EXCEPTION"),
            QStringLiteral("The geometry reader raised an exception."),
            QString::fromLocal8Bit(exception.what()),
            request.filePath);
        return failure(diagnostic);
    }
    catch (...)
    {
        const auto diagnostic = importDiagnostic(
            QStringLiteral("GEO-IMPORT-READER-UNKNOWN-EXCEPTION"),
            QStringLiteral("The geometry reader raised an unknown exception."),
            QStringLiteral("No exception detail is available."),
            request.filePath);
        return failure(diagnostic);
    }
    if (!read.succeeded())
    {
        return failure(read.diagnostics.front(), read.diagnostics);
    }
    if (!read.geometry)
    {
        const auto diagnostic = importDiagnostic(
            QStringLiteral("GEO-IMPORT-READER-EMPTY"),
            QStringLiteral("The geometry reader returned no geometry."),
            QStringLiteral("A successful reader result requires one staged value."),
            request.filePath);
        return failure(diagnostic);
    }

    Model::CreateGeometryRequest commit;
    commit.name = request.requestedName;
    commit.commonMetadata = request.metadata;
    commit.geometry = *read.geometry;
    const auto committed = m_geometryManager.createGeometry(commit);
    if (!committed.succeeded() || !committed.object)
    {
        if (!committed.diagnostics.isEmpty())
        {
            return failure(committed.diagnostics.front(), committed.diagnostics);
        }
        const auto diagnostic = importDiagnostic(
            QStringLiteral("GEO-IMPORT-COMMIT-FAILED"),
            QStringLiteral("The imported geometry could not be committed."),
            QStringLiteral("GeometryManager returned no object or diagnostic."),
            request.filePath);
        return failure(diagnostic);
    }

    Operators::ResultReference reference;
    reference.objectId = committed.object->common.id;
    reference.path = request.filePath;
    reference.value = QVariant::fromValue<qulonglong>(committed.object->common.id);
    return Operators::OperatorResult::success(reference, committed.diagnostics);
}
}
