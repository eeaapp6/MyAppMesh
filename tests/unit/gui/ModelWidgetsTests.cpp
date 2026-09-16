#include "gui/GUIWidget/ConsoleWidget.h"
#include "gui/GUIWidget/ModelTree.h"
#include "model/ModelData/GeometryManager.h"
#include "model/ModelData/MeshManager.h"
#include "TestSupport.h"

#include <QApplication>
#include <QDateTime>
#include <QElapsedTimer>
#include <QEventLoop>
#include <QPointer>
#include <QThread>

#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

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

bool waitUntil(QApplication& application,
               const std::function<bool()>& condition,
               int timeoutMilliseconds = 3000)
{
    QElapsedTimer timer;
    timer.start();
    while (!condition() && timer.elapsed() < timeoutMilliseconds)
    {
        application.processEvents(QEventLoop::AllEvents, 25);
    }
    return condition();
}
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
    suite.expect(waitUntil(application, [&] {
                     return tree.objectModel()->indexForObject(groupId).isValid() &&
                         tree.objectModel()->indexForObject(geometryId).isValid() &&
                         tree.objectModel()->indexForObject(meshId).isValid();
                 }),
                 QStringLiteral("Generic, Geometry and Mesh additions synchronize automatically"));

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

    suite.expect(waitUntil(application, [&] {
                     const auto geometryNode = tree.objectModel()->snapshotForObject(geometryId);
                     const auto meshNode = tree.objectModel()->snapshotForObject(meshId);
                     return geometryNode.has_value() && meshNode.has_value() &&
                         geometryNode->name == QStringLiteral("Body") &&
                         !geometryNode->visible &&
                         meshNode->name == QStringLiteral("Surface Mesh") &&
                         meshNode->selected && meshNode->parentId == groupId;
                 }),
                 QStringLiteral("rename, visibility, selection and parent changes synchronize automatically"));

    std::vector<ModelChangeEvent> observedEvents;
    std::mutex observedMutex;
    auto eventSubscription = runtime.subscribe([&](const ModelChangeEvent& event) {
        std::lock_guard<std::mutex> guard(observedMutex);
        observedEvents.push_back(event);
    });
    std::atomic<ObjectId> workerObjectId{InvalidObjectId};
    std::thread modelWorker([&] {
        CreateObjectRequest request;
        request.name = QStringLiteral("Worker Object");
        request.type = DataObjectType::Generic;
        const auto created = runtime.createObject(request);
        if (created.object)
        {
            workerObjectId.store(created.object->id);
            runtime.renameObject(created.object->id, QStringLiteral("Worker Renamed"));
            runtime.setVisible(created.object->id, false);
            runtime.setSelected(created.object->id, true);
        }
    });
    modelWorker.join();
    suite.expect(waitUntil(application, [&] {
                     const auto node = tree.objectModel()->snapshotForObject(workerObjectId.load());
                     return node.has_value() && node->name == QStringLiteral("Worker Renamed") &&
                         !node->visible && node->selected;
                 }) && tree.lastAutomaticRefreshOccurredOnGuiThread(),
                 QStringLiteral("worker-thread events are queued and applied on the GUI thread"));
    {
        std::lock_guard<std::mutex> guard(observedMutex);
        bool ordered = observedEvents.size() >= 4;
        for (std::size_t i = 1; ordered && i < observedEvents.size(); ++i)
        {
            ordered = observedEvents[i - 1].sequence < observedEvents[i].sequence;
        }
        suite.expect(ordered && observedEvents.front().type == ModelChangeType::Added,
                     QStringLiteral("model change events preserve a stable success order"));
    }

    const int beforeFailedMutation = tree.objectModel()->rowCount();
    std::size_t eventsBeforeFailure = 0;
    {
        std::lock_guard<std::mutex> guard(observedMutex);
        eventsBeforeFailure = observedEvents.size();
    }
    suite.expect(!runtime.renameObject(999999, QStringLiteral("Missing")).succeeded(),
                 QStringLiteral("failed model mutation is rejected"));
    application.processEvents(QEventLoop::AllEvents, 25);
    {
        std::lock_guard<std::mutex> guard(observedMutex);
        suite.expect(observedEvents.size() == eventsBeforeFailure &&
                         tree.objectModel()->rowCount() == beforeFailedMutation,
                     QStringLiteral("failed operations emit no success event or GUI mutation"));
    }

    QVector<DataObjectSnapshot> invalidSnapshots = runtime.snapshots();
    invalidSnapshots.front().parentId = 999999;
    const int previousRows = tree.objectModel()->rowCount();
    suite.expect(!tree.objectModel()->replaceSnapshots(invalidSnapshots).succeeded() &&
                     tree.objectModel()->rowCount() == previousRows,
                 QStringLiteral("invalid refresh preserves the last valid model"));

    suite.expect(meshManager.removeMesh(meshId).succeeded() &&
                     geometryManager.removeGeometry(geometryId).succeeded() &&
                     waitUntil(application, [&] {
                         return !tree.objectModel()->indexForObject(meshId).isValid() &&
                             !tree.objectModel()->indexForObject(geometryId).isValid();
                     }),
                 QStringLiteral("deleted objects automatically leave no stale tree nodes"));

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
    suite.expect(waitUntil(application, [&] {
                     return console.plainText().contains(QStringLiteral("worker-message"));
                 }) &&
                     console.lastMutationOccurredOnGuiThread(),
                 QStringLiteral("worker messages mutate the control only on the GUI thread"));
    console.clearMessages();
    suite.expect(console.plainText().isEmpty(),
                 QStringLiteral("console messages can be cleared"));

    tree.unbind();
    suite.expect(!tree.isBound() && tree.objectModel()->rowCount() == 0,
                 QStringLiteral("unbind clears non-owning service references"));
    CreateObjectRequest afterUnbind;
    afterUnbind.name = QStringLiteral("After Unbind");
    afterUnbind.type = DataObjectType::Generic;
    runtime.createObject(afterUnbind);
    application.processEvents(QEventLoop::AllEvents, 25);
    suite.expect(tree.objectModel()->rowCount() == 0,
                 QStringLiteral("unbind cancels future model notifications"));

    {
        auto ownedRuntime = std::make_unique<ApplicationRuntime>();
        auto ownedTree = std::make_unique<AppMesh::Gui::ModelTree>();
        suite.expect(ownedTree->bind(ownedRuntime.get(), nullptr, nullptr).succeeded(),
                     QStringLiteral("tree can bind to an independently owned Runtime"));
        ownedRuntime.reset();
        suite.expect(waitUntil(application, [&] { return !ownedTree->isBound(); }),
                     QStringLiteral("Runtime shutdown invalidates the binding without use-after-free"));
    }

    {
        ApplicationRuntime survivingRuntime;
        QPointer<AppMesh::Gui::ModelTree> destroyedTree = new AppMesh::Gui::ModelTree;
        destroyedTree->bind(&survivingRuntime, nullptr, nullptr);
        delete destroyedTree.data();
        survivingRuntime.createObject(afterUnbind);
        application.processEvents(QEventLoop::AllEvents, 25);
        suite.expect(destroyedTree.isNull(),
                     QStringLiteral("destroyed ModelTree safely unsubscribes while Runtime survives"));
    }
    return suite.result();
}
