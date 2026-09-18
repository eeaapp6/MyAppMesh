#include "GeometryReader.h"

#include <QReadLocker>
#include <QWriteLocker>

#include <set>

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
    QStringList keys = reader->aliases();
    keys.append(reader->key());
    for (auto& alias : keys)
    {
        alias = normalizedKey(alias);
    }
    keys.removeAll(QString());
    keys.removeDuplicates();
    QWriteLocker guard(&m_lock);
    for (const auto& alias : keys)
    {
        if (m_readers.find(alias) != m_readers.end())
        {
            result.add(readerDiagnostic(QStringLiteral("GEO-READER-KEY-DUPLICATE"),
                                        QStringLiteral("The geometry reader key or alias is already registered."),
                                        alias));
            return result;
        }
    }
    for (const auto& alias : keys)
    {
        m_readers.emplace(alias, reader);
    }
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
    std::set<const IGeometryReader*> unique;
    for (const auto& entry : m_readers)
    {
        unique.insert(entry.second.get());
    }
    return static_cast<int>(unique.size());
}
}
