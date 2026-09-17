#include "GeometryReader.h"

#include <QReadLocker>
#include <QWriteLocker>

namespace AppMesh::OperatorsModel
{
namespace
{
Common::Diagnostic readerDiagnostic(const QString& code,
                                    const QString& message,
                                    const QString& detail)
{
    return {QStringLiteral("operator"),
            code,
            message,
            detail,
            false,
            QStringLiteral("reader"),
            QStringLiteral("geometry.reader.registry"),
            QString(),
            QString()};
}
}

QString GeometryReaderRegistry::normalizedKey(const QString& key)
{
    return key.trimmed().toCaseFolded();
}

Common::OperationResult GeometryReaderRegistry::registerReader(
    std::shared_ptr<const IGeometryReader> reader)
{
    Common::OperationResult result;
    if (!reader)
    {
        result.add(readerDiagnostic(QStringLiteral("GEO-READER-MISSING"),
                                    QStringLiteral("The geometry reader is missing."),
                                    QStringLiteral("Register a shared reader instance.")));
        return result;
    }
    const QString key = normalizedKey(reader->key());
    if (key.isEmpty())
    {
        result.add(readerDiagnostic(QStringLiteral("GEO-READER-KEY-EMPTY"),
                                    QStringLiteral("The geometry reader key is empty."),
                                    QStringLiteral("Reader keys must be non-empty and unique.")));
        return result;
    }
    if (reader->supportedExtensions().isEmpty())
    {
        result.add(readerDiagnostic(QStringLiteral("GEO-READER-EXTENSIONS-EMPTY"),
                                    QStringLiteral("The geometry reader has no file extensions."),
                                    QStringLiteral("Declare at least one supported extension.")));
        return result;
    }
    QWriteLocker guard(&m_lock);
    if (m_readers.find(key) != m_readers.end())
    {
        result.add(readerDiagnostic(QStringLiteral("GEO-READER-KEY-DUPLICATE"),
                                    QStringLiteral("The geometry reader key is already registered."),
                                    reader->key().trimmed()));
        return result;
    }
    m_readers.emplace(key, std::move(reader));
    return result;
}

std::shared_ptr<const IGeometryReader> GeometryReaderRegistry::find(
    const QString& key) const
{
    QReadLocker guard(&m_lock);
    const auto found = m_readers.find(normalizedKey(key));
    return found == m_readers.end() ? nullptr : found->second;
}

int GeometryReaderRegistry::readerCount() const
{
    QReadLocker guard(&m_lock);
    return static_cast<int>(m_readers.size());
}
}
