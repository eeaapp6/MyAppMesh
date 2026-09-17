#include "operators/OperatorsGUI/GeometryImportController.h"
#include "operators/OperatorsModel/FITKTaskExecutor.h"
#include "gui/GUIWidget/ConsoleWidget.h"
#include "gui/GUIWidget/ModelTree.h"
#include "model/ModelData/MeshManager.h"
#include "TestSupport.h"

#include <QApplication>
#include <QEventLoop>
#include <QFile>
#include <QTemporaryDir>
#include <QTimer>

#include <condition_variable>
#include <mutex>

namespace
{
using namespace AppMesh;
using namespace AppMesh::OperatorsGUI;
using namespace AppMesh::OperatorsModel;

class BlockingReader final : public IGeometryReader
{
public:
    QString key() const override { return QStringLiteral("fake"); }
    QStringList supportedExtensions() const override { return {QStringLiteral("fake")}; }
    GeometryReadResult read(const GeometryReadRequest&) const override
    {
        {
            std::lock_guard<std::mutex> guard(m_mutex);
            m_started = true;
            m_condition.notify_all();
        }
        std::unique_lock<std::mutex> lock(m_mutex);
        m_condition.wait(lock, [this] { return m_released; });
        GeometryReadResult result;
        Model::GeometryObject geometry;
        geometry.type = Model::GeometryType::BRep;
        geometry.adapter.providerKey = QStringLiteral("controller-test");
        geometry.adapter.modelKey = QStringLiteral("model");
        geometry.topology.append({1, Model::TopologyEntityType::Solid,
                                  QStringLiteral("solid/1"), {}});
        result.geometry = geometry;
        return result;
    }

    bool waitUntilStarted(int timeoutMs) const
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        return m_condition.wait_for(lock, std::chrono::milliseconds(timeoutMs),
                                    [this] { return m_started; });
    }

    void release() const
    {
        std::lock_guard<std::mutex> guard(m_mutex);
        m_released = true;
        m_condition.notify_all();
    }

private:
    mutable std::mutex m_mutex;
    mutable std::condition_variable m_condition;
    mutable bool m_started = false;
    mutable bool m_released = false;
};

GeometryImportRequest requestFor(const QString& filePath,
                                 const QString& workDirectory,
                                 quint64 token)
{
    GeometryImportRequest request;
    request.requestToken = token;
    request.filePath = filePath;
    request.workDirectory = workDirectory;
    request.formatKey = QStringLiteral("fake");
    request.requestedName = QStringLiteral("GUI Geometry");
    request.metadata.insert(QStringLiteral("route"), QStringLiteral("gui"));
    return request;
}
}

int main(int argc, char* argv[])
{
    QApplication application(argc, argv);
    TestSuite suite;
    QTemporaryDir temporary;
    const QString filePath = temporary.filePath(QStringLiteral("gui.fake"));
    QFile file(filePath);
    file.open(QIODevice::WriteOnly);
    file.write("fake");
    file.close();

    Model::ApplicationRuntime runtime;
    Model::GeometryManager geometryManager(runtime);
    Model::MeshManager meshManager(runtime, geometryManager);
    Gui::ModelTree tree;
    Gui::ConsoleWidget console;
    suite.expect(tree.bind(&runtime, &geometryManager, &meshManager).succeeded(),
                 QStringLiteral("model tree binds before geometry import routing"));

    auto reader = std::make_shared<BlockingReader>();
    auto registry = std::make_shared<GeometryReaderRegistry>();
    registry->registerReader(reader);
    auto importOperator = std::make_shared<ImportGeometryOperator>(geometryManager, registry);
    auto executor = std::make_shared<FITKTaskExecutor>();
    TaskService service(executor);
    GeometryImportController controller(service, importOperator, &console, &tree);

    const auto cancelled = controller.routeDialogResult(
        requestFor(filePath, temporary.path(), 1), false);
    suite.expect(cancelled.succeeded() && !cancelled.taskId &&
                     controller.submittedRequestCount() == 0,
                 QStringLiteral("cancelled dialog submits no task"));

    const auto submitted = controller.routeDialogResult(
        requestFor(filePath, temporary.path(), 2), true);
    const auto duplicate = controller.routeDialogResult(
        requestFor(filePath, temporary.path(), 2), true);
    suite.expect(submitted.taskId.has_value() && !duplicate.succeeded() &&
                     controller.submittedRequestCount() == 1 &&
                     reader->waitUntilStarted(2000),
                 QStringLiteral("accepted dialog submits once and reaches a FITK worker"));
    const auto invalid = controller.routeDialogResult(
        requestFor(temporary.filePath(QStringLiteral("missing.fake")),
                   temporary.path(), 4),
        true);
    suite.expect(!invalid.succeeded() && !invalid.taskId &&
                     !console.plainText().isEmpty(),
                 QStringLiteral("preflight diagnostics are routed to ConsoleWidget without a task"));

    int heartbeats = 0;
    QTimer heartbeat;
    heartbeat.setInterval(10);
    QObject::connect(&heartbeat, &QTimer::timeout, [&heartbeats] { ++heartbeats; });
    heartbeat.start();
    QEventLoop loop;
    QTimer::singleShot(80, [&] {
        reader->release();
        QTimer::singleShot(80, &loop, &QEventLoop::quit);
    });
    loop.exec();
    heartbeat.stop();
    const auto drained = service.stop(5000);
    QCoreApplication::processEvents();

    const auto snapshot = service.findById(*submitted.taskId);
    suite.expect(drained.succeeded() && snapshot &&
                     snapshot->status == Operators::TaskStatus::Succeeded &&
                     heartbeats >= 5,
                 QStringLiteral("worker import completes while the GUI event loop keeps processing timers"));
    suite.expect(runtime.objectCount() == 1 &&
                     tree.objectModel()->indexForObject(snapshot->result->objectId).isValid(),
                 QStringLiteral("successful commit reaches ModelTree through ModelData events"));
    suite.expect(console.plainText().contains(QStringLiteral("Geometry import succeeded")) &&
                     console.lastMutationOccurredOnGuiThread(),
                 QStringLiteral("task events update ConsoleWidget only on its GUI thread"));

    auto secondExecutor = std::make_shared<FITKTaskExecutor>();
    TaskService secondService(secondExecutor);
    auto secondReader = std::make_shared<BlockingReader>();
    auto secondRegistry = std::make_shared<GeometryReaderRegistry>();
    secondRegistry->registerReader(secondReader);
    auto secondOperator = std::make_shared<ImportGeometryOperator>(geometryManager,
                                                                   secondRegistry);
    auto* transient = new GeometryImportController(secondService,
                                                   secondOperator,
                                                   &console,
                                                   &tree);
    const auto transientTask = transient->routeDialogResult(
        requestFor(filePath, temporary.path(), 3), true);
    suite.expect(transientTask.taskId.has_value() && secondReader->waitUntilStarted(2000),
                 QStringLiteral("transient controller submits background work"));
    delete transient;
    secondReader->release();
    suite.expect(secondService.stop(5000).succeeded(),
                 QStringLiteral("controller destruction leaves no dangling observer callback"));
    QCoreApplication::processEvents();

    tree.unbind();
    return suite.result();
}
