#pragma once

#include "operators/OperatorsModel/GeometryReader.h"

#include <QReadWriteLock>

#include <memory>

namespace AppMesh::GeometryIO
{
struct GeometryFormatCapability
{
    QString key;
    QStringList aliases;
    QStringList extensions;
    Model::GeometryType geometryType = Model::GeometryType::Invalid;
    bool supportsRead = true;
    QString providerKey = QStringLiteral("fitk.occ");
};

QVector<GeometryFormatCapability> firstReleaseGeometryCapabilities();
Common::OperationResult validateGeometryCapabilities(
    const QVector<GeometryFormatCapability>& capabilities);

class FITKGeometryRepository final
{
public:
    FITKGeometryRepository();
    ~FITKGeometryRepository();
    FITKGeometryRepository(const FITKGeometryRepository&) = delete;
    FITKGeometryRepository& operator=(const FITKGeometryRepository&) = delete;

    bool contains(const QString& modelKey) const;
    int modelCount() const;
    void release(const QString& modelKey) noexcept;

private:
    class Private;
    std::unique_ptr<Private> d;
    friend class FITKGeometryReadTransaction;
};

class FITKOCCGeometryReader final : public OperatorsModel::IGeometryReader
{
public:
    FITKOCCGeometryReader(GeometryFormatCapability capability,
                          std::shared_ptr<FITKGeometryRepository> repository);
    QString key() const override;
    QStringList aliases() const override;
    QStringList supportedExtensions() const override;
    OperatorsModel::GeometryReadResult read(
        const OperatorsModel::GeometryReadRequest& request) const override;

private:
    GeometryFormatCapability m_capability;
    std::shared_ptr<FITKGeometryRepository> m_repository;
};

Common::OperationResult registerFirstReleaseGeometryReaders(
    OperatorsModel::GeometryReaderRegistry& registry,
    const std::shared_ptr<FITKGeometryRepository>& repository);
}
