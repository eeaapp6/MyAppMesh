#include "TestSupport.h"

#include "model/ModelData/GeometryManager.h"
#include "model/ModelData/MeshData.h"

#include <QCoreApplication>
#include <QStringList>

#include <limits>

namespace
{
using namespace AppMesh::Model;

bool hasCode(const AppMesh::Common::OperationResult& result, const QString& code)
{
    for (const auto& diagnostic : result.diagnostics)
    {
        if (diagnostic.code == code)
        {
            return true;
        }
    }
    return false;
}

QStringList diagnosticSequence(const AppMesh::Common::OperationResult& result)
{
    QStringList sequence;
    for (const auto& diagnostic : result.diagnostics)
    {
        sequence.append(diagnostic.code + QStringLiteral("|") + diagnostic.path);
    }
    return sequence;
}

GeometryObject validGeometry()
{
    GeometryObject geometry;
    geometry.type = GeometryType::BRep;
    geometry.adapter.providerKey = QStringLiteral("fitk.geometry");
    geometry.adapter.modelKey = QStringLiteral("mesh-source");
    geometry.topology.append(
        {1, TopologyEntityType::Solid, QStringLiteral("solid-1"), {}});
    return geometry;
}

MeshKernel validD2Kernel()
{
    return {MeshDimension::D2,
            {{1, 0.0, 0.0, 0.0},
             {2, 1.0, 0.0, 0.0},
             {3, 0.0, 1.0, 0.0}},
            {{1, CellType::Triangle3, {1, 2, 3}},
             {2, CellType::Line2, {1, 2}}},
            {{1, QStringLiteral("nodes"), MeshEntityType::Node, {1, 2}},
             {2, QStringLiteral("elements"), MeshEntityType::Element, {1, 2}}}};
}
}

int main(int argc, char* argv[])
{
    QCoreApplication application(argc, argv);
    TestSuite suite;
    ApplicationRuntime runtime;
    GeometryManager geometryManager(runtime);

    CreateGeometryRequest geometryRequest;
    geometryRequest.name = QStringLiteral("Source geometry");
    geometryRequest.geometry = validGeometry();
    const auto geometry = geometryManager.createGeometry(geometryRequest);
    suite.expect(geometry.succeeded() && geometry.object.has_value(),
                 QStringLiteral("mesh validation fixture has a published Geometry source"));

    MeshData valid({QStringLiteral("gmsh"), QStringLiteral("4.5.4")},
                   geometry.object->common.id,
                   {validD2Kernel()},
                   {{QStringLiteral("size"), 1.0}},
                   {{QStringLiteral("algorithm"), QStringLiteral("default")}},
                   {{QStringLiteral("visible"), true}});
    suite.expect(valid.validate(runtime).succeeded(),
                 QStringLiteral("a valid D2 mesh and Geometry association pass validation"));

    auto detachedKernels = valid.kernels();
    detachedKernels.clear();
    suite.expect(valid.kernels().size() == 1,
                 QStringLiteral("MeshData accessors return detached value containers"));

    suite.expect(cellTypeArity(CellType::Line2) == 2 &&
                     cellTypeArity(CellType::Triangle3) == 3 &&
                     cellTypeArity(CellType::Quadrilateral4) == 4 &&
                     cellTypeArity(CellType::Tetrahedron4) == 4 &&
                     cellTypeArity(CellType::Hexahedron8) == 8 &&
                     cellTypeDimension(CellType::Line2) == 1 &&
                     cellTypeDimension(CellType::Triangle3) == 2 &&
                     cellTypeDimension(CellType::Tetrahedron4) == 3,
                 QStringLiteral("the frozen CellType arity and dimension table is exact"));

    CreateObjectRequest genericRequest;
    genericRequest.name = QStringLiteral("Not geometry");
    genericRequest.type = DataObjectType::Generic;
    const auto generic = runtime.createObject(genericRequest);
    MeshData wrongType({QStringLiteral("gmsh"), QStringLiteral("4.5.4")},
                       generic.object->id,
                       {validD2Kernel()});
    const auto wrongTypeResult = wrongType.validate(runtime);
    MeshData missingGeometry({QStringLiteral("gmsh"), QStringLiteral("4.5.4")},
                             geometry.object->common.id + 1000,
                             {validD2Kernel()});
    suite.expect(hasCode(wrongTypeResult, QStringLiteral("MESH-GEOMETRY-TYPE-MISMATCH")) &&
                     wrongTypeResult.diagnostics.first().path == QStringLiteral("geometryId") &&
                     hasCode(missingGeometry.validate(runtime),
                             QStringLiteral("MESH-GEOMETRY-NOT-FOUND")),
                 QStringLiteral("nonzero geometryId is resolved and type-checked through Runtime only"));

    MeshKernel invalidKernel(
        MeshDimension::D2,
        {{0, std::numeric_limits<double>::quiet_NaN(), 0.0, 0.0},
         {1, 0.0, 0.0, 0.0},
         {1, 1.0, 0.0, 0.0}},
        {{0, CellType::Triangle3, {1, 99}},
         {1, CellType::Tetrahedron4, {1, 1, 1, 1}},
         {1, CellType::Triangle3, {1, 1, 1}},
         {2, CellType::Triangle3, {1, 1, 1}},
         {3, static_cast<CellType>(99), {1}}},
        {{0, QString(), static_cast<MeshEntityType>(99), {}},
         {2, QStringLiteral("missing nodes"), MeshEntityType::Node, {99, 99, 0}},
         {2, QStringLiteral("wrong type"), MeshEntityType::Node, {2}}});
    MeshData invalid({QString(), QString()}, InvalidObjectId, {invalidKernel});
    const auto invalidResult = invalid.validate(runtime);
    suite.expect(hasCode(invalidResult, QStringLiteral("MESH-GENERATOR-KEY-EMPTY")) &&
                     hasCode(invalidResult, QStringLiteral("MESH-GENERATOR-VERSION-EMPTY")) &&
                     hasCode(invalidResult, QStringLiteral("MESH-NODE-ID-INVALID")) &&
                     hasCode(invalidResult, QStringLiteral("MESH-NODE-ID-DUPLICATE")) &&
                     hasCode(invalidResult, QStringLiteral("MESH-NODE-COORDINATE-NONFINITE")) &&
                     hasCode(invalidResult, QStringLiteral("MESH-ELEMENT-ID-INVALID")) &&
                     hasCode(invalidResult, QStringLiteral("MESH-ELEMENT-ID-DUPLICATE")) &&
                     hasCode(invalidResult, QStringLiteral("MESH-CELL-TYPE-INVALID")) &&
                     hasCode(invalidResult, QStringLiteral("MESH-CELL-ARITY-MISMATCH")) &&
                     hasCode(invalidResult, QStringLiteral("MESH-CELL-DIMENSION-MISMATCH")) &&
                     hasCode(invalidResult, QStringLiteral("MESH-ELEMENT-NODE-NOT-FOUND")) &&
                     hasCode(invalidResult, QStringLiteral("MESH-SET-ID-INVALID")) &&
                     hasCode(invalidResult, QStringLiteral("MESH-SET-ID-DUPLICATE")) &&
                     hasCode(invalidResult, QStringLiteral("MESH-SET-ENTITY-TYPE-INVALID")) &&
                     hasCode(invalidResult, QStringLiteral("MESH-SET-ENTITY-TYPE-MISMATCH")) &&
                     hasCode(invalidResult, QStringLiteral("MESH-SET-ENTITY-NOT-FOUND")),
                 QStringLiteral("mesh integrity failures identify every affected entity class"));
    suite.expect(diagnosticSequence(invalidResult) ==
                     diagnosticSequence(invalid.validate(runtime)),
                 QStringLiteral("identical mesh input produces an identical diagnostic order"));

    MeshKernel d3WithoutVolume(
        MeshDimension::D3,
        {{1, 0.0, 0.0, 0.0}, {2, 1.0, 0.0, 0.0}, {3, 0.0, 1.0, 0.0}},
        {{1, CellType::Triangle3, {1, 2, 3}}});
    suite.expect(hasCode(d3WithoutVolume.validate(0),
                         QStringLiteral("MESH-DIMENSION-CONTENT-MISMATCH")),
                 QStringLiteral("D3 requires a three-dimensional primary cell"));

    MeshData empty({QStringLiteral("gmsh"), QStringLiteral("4.5.4")},
                   InvalidObjectId,
                   {});
    suite.expect(hasCode(empty.validate(runtime), QStringLiteral("MESH-KERNELS-EMPTY")),
                 QStringLiteral("the frozen empty MeshData structure is rejected"));

    suite.expect(geometryManager.removeGeometry(geometry.object->common.id).succeeded() &&
                     runtime.removeObject(generic.object->id).succeeded(),
                 QStringLiteral("validation fixtures leave no runtime records behind"));
    return suite.result();
}
