#include "gui/GUIWidget/ConsoleWidget.h"
#include "gui/GUIWidget/ModelTree.h"
#include "model/ModelData/GeometryManager.h"
#include "model/ModelData/MeshManager.h"
#include "TestSupport.h"

#include <QApplication>
#include <QDateTime>
#include <QEventLoop>
#include <QThread>

#include <memory>
#include <thread>

namespace
{
using namespace AppMesh::Model;

GeometryObject validGeometry()
{
    GeometryObject geometry;
    geometry.type = GeometryType::BRep;
    geometry.adapter.providerKey = QStringLiteral("fitk.geometry");
    geometry.adapter.modelKey = QStringLiteral("gui-test-geometry");
    geometry.topology.append(
        {1, TopologyEntityType::Solid, QStringLiteral("solid-1"), {}});
    return geometry;
}

MeshKernel validKernel()
{
    return {MeshDimension::D2,
            {{1, 0.0, 0.0, 0.0},
             {2, 1.0, 0.0, 0.0},
             {3, 0.0, 1.0, 0.0}},
            {{1, CellType::Triangle3, {1, 2, 3}}}};
}

class TestMeshCreator final : public MeshDataCreator
{
public:
    MeshDataCreatorResult create(const MeshDataCreatorRequest& request) const override
    {
        MeshDataCreatorResult result;
        CreatedMeshData created;
        created.mesh = MeshData({QStringLiteral("gui.test"), QStringLiteral("1")},
                                request.geometryId,
                                {validKernel()});
        created.adapter.providerKey = QStringLiteral("fitk.mesh");
        created.adapter.meshKey = QStringLiteral("gui-test-mesh");
        result.created = std::move(created);
        return result;
    }
};
}

int main(int argc, char* argv[])
{
    QApplication application(argc, argv);
    TestSuite suite;
    ApplicationRuntime runtime;
    GeometryManager geometryManager(runtime);
    MeshManager meshManager(runtime, geometryManager);
    meshManager.registerCreator(QStringLiteral("gui.test"),
                                std::make_shared<TestMeshCreator>());

    AppMesh::Gui::ModelTree tree;
    suite.expect(tree.bind(&runtime, &geometryManager, &meshManager).succeeded() &&
                     tree.objectModel()->rowCount() == 0,
                 QStringLiteral("empty Runtime produces an empty model"));

    CreateObjectRequest groupRequest;
    groupRequest.name = QStringLiteral("Objects");
    groupRequest.type = DataObjectType::Generic;
    const auto group = runtime.createObject(groupRequest);

    CreateGeometryRequest geometryRequest;
    geometryRequest.name = QStringLiteral("Geometry");
    geometryRequest.parentId = group.object->id;
    geometryRequest.geometry = validGeometry();
    const auto geometry = geometryManager.createGeometry(geometryRequest);

    CreateMeshRequest meshRequest;
    meshRequest.name = QStringLiteral("Mesh");
    meshRequest.parentId = geometry.object->common.id;
    meshRequest.creatorKey = QStringLiteral("gui.test");
    meshRequest.creatorRequest.geometryId = geometry.object->common.id;
    const auto mesh = meshManager.createMesh(meshRequest);

    const ObjectId groupId = group.object->id;
    const ObjectId geometryId = geometry.object->common.id;
    const ObjectId meshId = mesh.object->common.id;
    suite.expect(tree.refreshFromRuntime().succeeded(),
                 QStringLiteral("Runtime snapshots refresh the tree"));

    const auto geometryIndex = tree.objectModel()->indexForObject(geometryId);
    const auto meshIndex = tree.objectModel()->indexForObject(meshId);
    suite.expect(geometryIndex.isValid() && meshIndex.isValid() &&
                     geometryIndex.data(AppMesh::Gui::ModelTreeModel::ObjectIdRole).toULongLong() ==
                         geometryId &&
                     geometryIndex.data(AppMesh::Gui::ModelTreeModel::ObjectTypeRole).toInt() ==
                         static_cast<int>(DataObjectType::Geometry) &&
                     meshIndex.data(AppMesh::Gui::ModelTreeModel::ObjectTypeRole).toInt() ==
                         static_cast<int>(DataObjectType::Mesh) &&
                     tree.objectModel()->parent(meshIndex) == geometryIndex,
                 QStringLiteral("Geometry and Mesh nodes expose stable roles and hierarchy"));

    suite.expect(tree.requestRename(groupId, QStringLiteral("Project Objects")).succeeded() &&
                     tree.requestRename(geometryId, QStringLiteral("Body")).succeeded() &&
                     tree.requestRename(meshId, QStringLiteral("Surface Mesh")).succeeded() &&
                     tree.requestVisible(geometryId, false).succeeded() &&
                     tree.requestSelected(meshId, true).succeeded() &&
                     tree.requestParent(meshId, groupId).succeeded(),
                 QStringLiteral("user requests route through Runtime and typed managers"));
    const auto changedGeometry = geometryManager.findById(geometryId);
    const auto changedMesh = meshManager.findById(meshId);
    suite.expect(changedGeometry.has_value() && changedMesh.has_value() &&
                     changedGeometry->common.name == QStringLiteral("Body") &&
                     !changedGeometry->common.visible &&
                     changedMesh->common.name == QStringLiteral("Surface Mesh") &&
                     changedMesh->common.selected &&
                     changedMesh->common.parentId == groupId,
                 QStringLiteral("manager-owned authoritative state reflects tree requests"));

    QVector<DataObjectSnapshot> invalidSnapshots = runtime.snapshots();
    invalidSnapshots.front().parentId = 999999;
    const int previousRows = tree.objectModel()->rowCount();
    suite.expect(!tree.objectModel()->replaceSnapshots(invalidSnapshots).succeeded() &&
                     tree.objectModel()->rowCount() == previousRows,
                 QStringLiteral("invalid refresh preserves the last valid model"));

    suite.expect(meshManager.removeMesh(meshId).succeeded() &&
                     geometryManager.removeGeometry(geometryId).succeeded() &&
                     tree.refreshFromRuntime().succeeded() &&
                     !tree.objectModel()->indexForObject(meshId).isValid() &&
                     !tree.objectModel()->indexForObject(geometryId).isValid(),
                 QStringLiteral("deleted objects leave no stale tree nodes"));

    AppMesh::Gui::ConsoleWidget console;
    console.setMaximumLineCount(3);
    const QDateTime timestamp = QDateTime::fromMSecsSinceEpoch(0, Qt::UTC);
    console.appendMessage(AppMesh::Gui::ConsoleMessageLevel::Normal,
                          QStringLiteral("normal"), QStringLiteral("test"), timestamp);
    console.appendMessage(AppMesh::Gui::ConsoleMessageLevel::Info,
                          QStringLiteral("info"), QStringLiteral("test"), timestamp);
    console.appendMessage(AppMesh::Gui::ConsoleMessageLevel::Warning,
                          QStringLiteral("warning"), QStringLiteral("test"), timestamp);
    console.appendMessage(AppMesh::Gui::ConsoleMessageLevel::Error,
                          QStringLiteral("error"), QStringLiteral("test"), timestamp);
    suite.expect(console.plainText().contains(QStringLiteral("[INFO]")) &&
                     console.plainText().contains(QStringLiteral("[WARNING]")) &&
                     console.plainText().contains(QStringLiteral("[ERROR]")) &&
                     console.lineCount() <= 3,
                 QStringLiteral("console formats levels and enforces its line limit"));

    std::thread worker([&console] {
        console.appendInfo(QStringLiteral("worker-message"), QStringLiteral("worker"));
    });
    worker.join();
    for (int attempt = 0;
         attempt < 20 && !console.plainText().contains(QStringLiteral("worker-message"));
         ++attempt)
    {
        application.processEvents(QEventLoop::AllEvents, 50);
        QThread::msleep(5);
    }
    suite.expect(console.plainText().contains(QStringLiteral("worker-message")) &&
                     console.lastMutationOccurredOnGuiThread(),
                 QStringLiteral("worker messages mutate the control only on the GUI thread"));
    console.clearMessages();
    suite.expect(console.plainText().isEmpty(),
                 QStringLiteral("console messages can be cleared"));

    tree.unbind();
    suite.expect(!tree.isBound() && tree.objectModel()->rowCount() == 0,
                 QStringLiteral("unbind clears non-owning service references"));
    return suite.result();
}
