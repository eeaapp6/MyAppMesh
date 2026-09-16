#include "TestSupport.h"

#include "model/ModelData/GeometryManager.h"
#include "model/ModelData/MeshData.h"
#include "model/ModelData/MeshManager.h"

#include <QCoreApplication>
#include <QStringList>

#include <cmath>
#include <limits>
#include <memory>

namespace
{
using namespace AppMesh::Model;

bool hasCode(const AppMesh::Common::OperationResult& result, const QString& code)
{
    for (const auto& item : result.diagnostics)
    {
        if (item.code == code)
        {
            return true;
        }
    }
    return false;
}

bool diagnosticsAreActionable(const AppMesh::Common::OperationResult& result)
{
    if (result.succeeded())
    {
        return false;
    }
    for (const auto& item : result.diagnostics)
    {
        if (item.code.isEmpty() || item.message.trimmed().isEmpty() ||
            (item.detail.trimmed().isEmpty() && item.path.trimmed().isEmpty()))
        {
            return false;
        }
    }
    return true;
}

QStringList diagnosticSequence(const AppMesh::Common::OperationResult& result)
{
    QStringList values;
    for (const auto& item : result.diagnostics)
    {
        values.append(item.code + QStringLiteral("|") + item.path);
    }
    return values;
}

MeshDimension validDimension(CellType type)
{
    const int dimension = cellTypeDimension(type);
    return dimension == 2 ? MeshDimension::D2
        : dimension == 3 ? MeshDimension::D3
                         : MeshDimension::Mixed;
}

QVector<Node> nodes(int count)
{
    QVector<Node> values;
    for (int index = 0; index < count; ++index)
    {
        values.append({static_cast<NodeId>(index + 1),
                       static_cast<double>(index),
                       static_cast<double>(index % 2),
                       static_cast<double>(index % 3)});
    }
    return values;
}

QVector<NodeId> nodeIds(int count)
{
    QVector<NodeId> values;
    for (int index = 0; index < count; ++index)
    {
        values.append(static_cast<NodeId>(index + 1));
    }
    return values;
}

MeshKernel kernel(CellType type,
                  MeshDimension dimension,
                  QVector<NodeId> references,
                  QVector<MeshSet> sets = {})
{
    return {dimension,
            nodes(std::max(8, references.size())),
            {{1, type, std::move(references)}},
            std::move(sets)};
}

GeometryObject geometry(GeometryType type, TopologyEntityType topologyType)
{
    GeometryObject value;
    value.type = type;
    value.adapter.providerKey = QStringLiteral("fitk.geometry");
    value.adapter.modelKey = QStringLiteral("matrix-model");
    value.topology.append({1, topologyType, QStringLiteral("entity-1"), {}});
    return value;
}

MeshKernel triangleKernel()
{
    return kernel(CellType::Triangle3, MeshDimension::D2, {1, 2, 3});
}

class MatrixMeshCreator final : public MeshDataCreator
{
public:
    MeshDataCreatorResult create(const MeshDataCreatorRequest& request) const override
    {
        MeshDataCreatorResult result;
        CreatedMeshData value;
        value.mesh = MeshData({QStringLiteral("matrix.creator"), QStringLiteral("1")},
                              request.geometryId,
                              {triangleKernel()});
        value.adapter.providerKey = QStringLiteral("fitk.mesh");
        value.adapter.meshKey = QStringLiteral("matrix-mesh");
        result.created = std::move(value);
        return result;
    }
};
}

int main(int argc, char* argv[])
{
    QCoreApplication application(argc, argv);
    TestSuite suite;
    ApplicationRuntime runtime;

    const QVector<CellType> cellTypes = {
        CellType::Line2,
        CellType::Triangle3,
        CellType::Quadrilateral4,
        CellType::Tetrahedron4,
        CellType::Hexahedron8};
    for (const CellType type : cellTypes)
    {
        const int arity = cellTypeArity(type);
        const auto valid = kernel(type, validDimension(type), nodeIds(arity)).validate(0);
        auto tooFew = nodeIds(arity - 1);
        auto tooMany = nodeIds(arity + 1);
        auto duplicate = nodeIds(arity);
        duplicate[duplicate.size() - 1] = duplicate.first();
        auto missing = nodeIds(arity);
        missing[missing.size() - 1] = 999;
        suite.expect(valid.succeeded() &&
                         hasCode(kernel(type, validDimension(type), tooFew).validate(0),
                                 QStringLiteral("MESH-CELL-ARITY-MISMATCH")) &&
                         hasCode(kernel(type, validDimension(type), tooMany).validate(0),
                                 QStringLiteral("MESH-CELL-ARITY-MISMATCH")) &&
                         hasCode(kernel(type, validDimension(type), duplicate).validate(0),
                                 QStringLiteral("MESH-ELEMENT-NODE-ID-DUPLICATE")) &&
                         hasCode(kernel(type, validDimension(type), missing).validate(0),
                                 QStringLiteral("MESH-ELEMENT-NODE-NOT-FOUND")),
                     QStringLiteral("cell %1 validates arity, duplicate and missing node references")
                         .arg(static_cast<int>(type)));
    }

    suite.expect(hasCode(kernel(CellType::Tetrahedron4,
                                MeshDimension::D2,
                                {1, 2, 3, 4}).validate(0),
                         QStringLiteral("MESH-CELL-DIMENSION-MISMATCH")) &&
                     hasCode(kernel(CellType::Triangle3,
                                    MeshDimension::D3,
                                    {1, 2, 3}).validate(0),
                             QStringLiteral("MESH-DIMENSION-CONTENT-MISMATCH")) &&
                     MeshKernel(MeshDimension::D3,
                                nodes(4),
                                {{1, CellType::Tetrahedron4, {1, 2, 3, 4}},
                                 {2, CellType::Line2, {1, 2}}}).validate(0).succeeded() &&
                     MeshKernel(MeshDimension::Mixed,
                                nodes(4),
                                {{1, CellType::Line2, {1, 2}},
                                 {2, CellType::Triangle3, {1, 2, 3}},
                                 {3, CellType::Tetrahedron4, {1, 2, 3, 4}}})
                         .validate(0).succeeded() &&
                     hasCode(MeshKernel(static_cast<MeshDimension>(99),
                                        nodes(2),
                                        {{1, CellType::Line2, {1, 2}}}).validate(0),
                             QStringLiteral("MESH-DIMENSION-INVALID")),
                 QStringLiteral("D2, D3, Mixed and invalid dimension rules match the frozen contract"));

    const QVector<double> nonFinite = {
        std::numeric_limits<double>::quiet_NaN(),
        std::numeric_limits<double>::infinity(),
        -std::numeric_limits<double>::infinity()};
    for (const double coordinate : nonFinite)
    {
        MeshKernel invalidCoordinate(
            MeshDimension::Mixed,
            {{1, coordinate, 0.0, 0.0}, {2, 1.0, 0.0, 0.0}},
            {{1, CellType::Line2, {1, 2}}});
        suite.expect(hasCode(invalidCoordinate.validate(0),
                             QStringLiteral("MESH-NODE-COORDINATE-NONFINITE")),
                     QStringLiteral("NaN and both infinities are rejected"));
    }

    MeshKernel invalidIds(
        MeshDimension::D2,
        {{0, 0.0, 0.0, 0.0}, {1, 0.0, 0.0, 0.0}, {1, 1.0, 0.0, 0.0}},
        {{0, CellType::Triangle3, {1, 1, 1}},
         {1, CellType::Triangle3, {1, 1, 1}},
         {1, CellType::Triangle3, {1, 1, 1}}},
        {{0, QString(), MeshEntityType::Node, {}},
         {1, QStringLiteral("nodes"), MeshEntityType::Node, {1, 1, 999}},
         {1, QStringLiteral("elements"), MeshEntityType::Element, {1, 999}}});
    const auto invalidIdResult = invalidIds.validate(2);
    suite.expect(hasCode(invalidIdResult, QStringLiteral("MESH-NODE-ID-INVALID")) &&
                     hasCode(invalidIdResult, QStringLiteral("MESH-NODE-ID-DUPLICATE")) &&
                     hasCode(invalidIdResult, QStringLiteral("MESH-ELEMENT-ID-INVALID")) &&
                     hasCode(invalidIdResult, QStringLiteral("MESH-ELEMENT-ID-DUPLICATE")) &&
                     hasCode(invalidIdResult, QStringLiteral("MESH-SET-ID-INVALID")) &&
                     hasCode(invalidIdResult, QStringLiteral("MESH-SET-ID-DUPLICATE")) &&
                     hasCode(invalidIdResult, QStringLiteral("MESH-SET-NAME-EMPTY")) &&
                     hasCode(invalidIdResult, QStringLiteral("MESH-SET-ENTITIES-EMPTY")) &&
                     hasCode(invalidIdResult, QStringLiteral("MESH-SET-ENTITY-ID-DUPLICATE")) &&
                     hasCode(invalidIdResult, QStringLiteral("MESH-SET-ENTITY-NOT-FOUND")) &&
                     diagnosticsAreActionable(invalidIdResult) &&
                     diagnosticSequence(invalidIdResult) ==
                         diagnosticSequence(invalidIds.validate(2)),
                 QStringLiteral("ID and MeshSet matrix is actionable and deterministically ordered"));

    MeshData twoKernels({QStringLiteral("matrix"), QStringLiteral("1")},
                        InvalidObjectId,
                        {triangleKernel(), triangleKernel()});
    suite.expect(twoKernels.validate(runtime).succeeded(),
                 QStringLiteral("node, element and set IDs are scoped independently per kernel"));

    for (const GeometryType type : {GeometryType::BRep, GeometryType::Step, GeometryType::Iges})
    {
        suite.expect(validateGeometryObject(geometry(type, TopologyEntityType::Solid)).succeeded(),
                     QStringLiteral("each frozen geometry type is accepted"));
    }
    for (const TopologyEntityType type : {TopologyEntityType::Vertex,
                                          TopologyEntityType::Edge,
                                          TopologyEntityType::Face,
                                          TopologyEntityType::Solid})
    {
        suite.expect(validateGeometryObject(geometry(GeometryType::BRep, type)).succeeded(),
                     QStringLiteral("each frozen topology type is accepted"));
    }
    auto invalidGeometry = geometry(GeometryType::Invalid, TopologyEntityType::Invalid);
    invalidGeometry.adapter.providerKey.clear();
    invalidGeometry.adapter.modelKey.clear();
    invalidGeometry.topology[0].id = InvalidGeometryEntityId;
    invalidGeometry.topology[0].adapterEntityKey.clear();
    const auto invalidGeometryResult = validateGeometryObject(invalidGeometry);
    suite.expect(hasCode(invalidGeometryResult, QStringLiteral("GEO-TYPE-INVALID")) &&
                     hasCode(invalidGeometryResult, QStringLiteral("GEO-ADAPTER-PROVIDER-MISSING")) &&
                     hasCode(invalidGeometryResult, QStringLiteral("GEO-ADAPTER-MODEL-MISSING")) &&
                     hasCode(invalidGeometryResult, QStringLiteral("GEO-ENTITY-ID-INVALID")) &&
                     hasCode(invalidGeometryResult, QStringLiteral("GEO-ENTITY-TYPE-INVALID")) &&
                     hasCode(invalidGeometryResult, QStringLiteral("GEO-ENTITY-ADAPTER-KEY-MISSING")) &&
                     diagnosticsAreActionable(invalidGeometryResult),
                 QStringLiteral("geometry validation locates every frozen invalid field"));

    GeometryManager geometryManager(runtime);
    CreateGeometryRequest geometryRequest;
    geometryRequest.name = QStringLiteral("Matrix geometry");
    geometryRequest.geometry = geometry(GeometryType::BRep, TopologyEntityType::Solid);
    const auto geometryResult = geometryManager.createGeometry(geometryRequest);
    CreateObjectRequest genericRequest;
    genericRequest.name = QStringLiteral("Matrix generic");
    genericRequest.type = DataObjectType::Generic;
    const auto generic = runtime.createObject(genericRequest);
    MeshManager meshManager(runtime, geometryManager);
    meshManager.registerCreator(QStringLiteral("matrix.creator"),
                                std::make_shared<MatrixMeshCreator>());
    CreateMeshRequest meshRequest;
    meshRequest.name = QStringLiteral("Matrix managed mesh");
    meshRequest.creatorKey = QStringLiteral("matrix.creator");
    const auto managedMesh = meshManager.createMesh(meshRequest);

    MeshData zeroGeometry({QStringLiteral("matrix"), QStringLiteral("1")},
                          InvalidObjectId,
                          {triangleKernel()},
                          {{QStringLiteral("size"), 1.0}},
                          {{QStringLiteral("algorithm"), QStringLiteral("stable")}},
                          {{QStringLiteral("color"), QStringLiteral("blue")}});
    MeshData missingGeometry({QStringLiteral("matrix"), QStringLiteral("1")},
                             999999,
                             {triangleKernel()});
    MeshData genericGeometry({QStringLiteral("matrix"), QStringLiteral("1")},
                             generic.object->id,
                             {triangleKernel()});
    MeshData meshGeometry({QStringLiteral("matrix"), QStringLiteral("1")},
                          managedMesh.object->common.id,
                          {triangleKernel()});
    MeshData validGeometryData({QStringLiteral("matrix"), QStringLiteral("1")},
                               geometryResult.object->common.id,
                               {triangleKernel()});
    MeshData emptyGenerator({QString(), QString()}, InvalidObjectId, {});
    suite.expect(zeroGeometry.validate(runtime).succeeded() &&
                     zeroGeometry.commonParameters().value(QStringLiteral("size")).toDouble() == 1.0 &&
                     zeroGeometry.generatorParameters().value(QStringLiteral("algorithm")).toString() ==
                         QStringLiteral("stable") &&
                     zeroGeometry.displayMetadata().value(QStringLiteral("color")).toString() ==
                         QStringLiteral("blue") &&
                     hasCode(missingGeometry.validate(runtime),
                             QStringLiteral("MESH-GEOMETRY-NOT-FOUND")) &&
                     hasCode(genericGeometry.validate(runtime),
                             QStringLiteral("MESH-GEOMETRY-TYPE-MISMATCH")) &&
                     hasCode(meshGeometry.validate(runtime),
                             QStringLiteral("MESH-GEOMETRY-TYPE-MISMATCH")) &&
                     validGeometryData.validate(runtime).succeeded() &&
                     hasCode(emptyGenerator.validate(runtime),
                             QStringLiteral("MESH-GENERATOR-KEY-EMPTY")) &&
                     hasCode(emptyGenerator.validate(runtime),
                             QStringLiteral("MESH-GENERATOR-VERSION-EMPTY")) &&
                     hasCode(emptyGenerator.validate(runtime),
                             QStringLiteral("MESH-KERNELS-EMPTY")),
                 QStringLiteral("MeshData matrix covers zero, missing, Generic, Mesh and Geometry references"));

    MeshAdapterReference adapter{QStringLiteral("fitk.mesh"),
                                 QStringLiteral("opaque-42"),
                                 {{QStringLiteral("owner"), QStringLiteral("plugin")}}};
    MeshAdapterReference adapterCopy = adapter;
    adapterCopy.meshKey = QStringLiteral("changed");
    suite.expect(adapter.isValid() && adapter.meshKey == QStringLiteral("opaque-42") &&
                     adapterCopy.meshKey == QStringLiteral("changed"),
                 QStringLiteral("provider and opaque mesh keys have detached value semantics"));

    return suite.result();
}
