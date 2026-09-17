#pragma once

#include "model/ModelData/GeometryObject.h"

#include <QReadWriteLock>
#include <QStringList>

#include <map>
#include <memory>
#include <optional>

namespace AppMesh::OperatorsModel
{
struct GeometryReadRequest
{
    QString filePath;
    QString workDirectory;
    QString formatKey;
    QString requestedName;
    QVariantMap metadata;
};

struct GeometryReadResult : Common::OperationResult
{
    std::optional<Model::GeometryObject> geometry;
};

class IGeometryReader
{
public:
    virtual ~IGeometryReader() = default;
    virtual QString key() const = 0;
    virtual QStringList supportedExtensions() const = 0;
    virtual GeometryReadResult read(const GeometryReadRequest& request) const = 0;
};

class GeometryReaderRegistry final
{
public:
    Common::OperationResult registerReader(std::shared_ptr<const IGeometryReader> reader);
    std::shared_ptr<const IGeometryReader> find(const QString& key) const;
    int readerCount() const;

private:
    static QString normalizedKey(const QString& key);
    mutable QReadWriteLock m_lock;
    std::map<QString, std::shared_ptr<const IGeometryReader>> m_readers;
};
}
