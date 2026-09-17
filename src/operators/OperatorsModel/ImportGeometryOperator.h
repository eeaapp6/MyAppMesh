#pragma once

#include "GeometryReader.h"
#include "model/ModelData/GeometryManager.h"
#include "operators/OperatorsInterface/IOperator.h"

#include <memory>

namespace AppMesh::OperatorsModel
{
class ImportGeometryOperator final : public Operators::IOperator
{
public:
    static QString operationKey();
    static QString filePathParameter();
    static QString workDirectoryParameter();
    static QString formatKeyParameter();
    static QString requestedNameParameter();
    static QString metadataParameter();

    ImportGeometryOperator(Model::GeometryManager& geometryManager,
                           std::shared_ptr<const GeometryReaderRegistry> readers);

    QString key() const override;
    Common::OperationResult validate(const Operators::OperatorInput& input) const override;
    Operators::OperatorResult execute(const Operators::OperatorInput& input) override;

private:
    GeometryReadRequest requestFromInput(const Operators::OperatorInput& input) const;
    Operators::OperatorResult failure(const Common::Diagnostic& diagnostic,
                                      QVector<Common::Diagnostic> diagnostics = {}) const;

    Model::GeometryManager& m_geometryManager;
    std::shared_ptr<const GeometryReaderRegistry> m_readers;
};
}
