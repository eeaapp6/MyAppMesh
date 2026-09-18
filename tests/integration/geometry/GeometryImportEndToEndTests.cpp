#include "io/GeometryIO/GeometryIO.h"
#include "operators/OperatorsGUI/GeometryImportController.h"
#include "operators/OperatorsModel/FITKTaskExecutor.h"
#include "gui/GUIWidget/ConsoleWidget.h"
#include "gui/GUIWidget/ModelTree.h"
#include "model/ModelData/MeshManager.h"
#include "TestSupport.h"

#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepTools.hxx>
#include <IGESControl_Controller.hxx>
#include <IGESControl_Writer.hxx>
#include <STEPControl_Writer.hxx>
#include <TopoDS_Shape.hxx>

#include <QApplication>
#include <QEventLoop>
#include <QFile>
#include <QElapsedTimer>
#include <QPointer>
#include <QSet>
#include <QTemporaryDir>
#include <QThread>
#include <QTimer>

#include <chrono>
#include <algorithm>
#include <condition_variable>
#include <future>
#include <map>
#include <mutex>

namespace
{
using namespace AppMesh;
using namespace AppMesh::Operators;
using namespace AppMesh::OperatorsGUI;
using namespace AppMesh::OperatorsModel;

bool createFixture(const QString& path)
{
    const TopoDS_Shape shape = BRepPrimAPI_MakeBox(1.0, 2.0, 3.0).Shape();
    const QByteArray native = QFile::encodeName(path);
    const auto suffix = QFileInfo(path).suffix().toCaseFolded();
    if (suffix == QStringLiteral("brep")) return BRepTools::Write(shape, native.constData());
    if (suffix == QStringLiteral("step") || suffix == QStringLiteral("stp")) { STEPControl_Writer w; return w.Transfer(shape, STEPControl_AsIs) == IFSelect_RetDone && w.Write(native.constData()) == IFSelect_RetDone; }
    IGESControl_Controller::Init(); IGESControl_Writer w; w.AddShape(shape); w.ComputeModel(); return w.Write(native.constData());
}

GeometryImportRequest request(const QString& path, const QString& work, quint64 token, const QString& key = QStringLiteral("brep"))
{
    GeometryImportRequest value;
    value.requestToken = token;
    value.filePath = path;
    value.workDirectory = work;
    value.formatKey = key;
    value.requestedName = QStringLiteral("T017 Geometry %1").arg(token);
    return value;
}

bool waitForTerminal(QApplication& application, TaskService& service, TaskId id, int timeoutMs = 10000)
{
    QEventLoop loop;
    QTimer poll;
    QTimer timeout;
    bool terminal = false;
    poll.setInterval(10);
    timeout.setSingleShot(true);
    QObject::connect(&poll, &QTimer::timeout, [&] {
        const auto snapshot = service.findById(id);
        if (snapshot && (snapshot->status == TaskStatus::Succeeded || snapshot->status == TaskStatus::Failed))
        {
            terminal = true;
            loop.quit();
        }
    });
    QObject::connect(&timeout, &QTimer::timeout, &loop, &QEventLoop::quit);
    poll.start();
    timeout.start(timeoutMs);
    loop.exec();
    application.processEvents();
    return terminal;
}

class EventLog final
{
public:
    void append(const TaskEvent& event)
    {
        std::lock_guard<std::mutex> guard(m_mutex);
        m_events.append(event);
    }

    int count(TaskId id, TaskEventType type) const
    {
        std::lock_guard<std::mutex> guard(m_mutex);
        int result = 0;
        for (const auto& event : m_events)
            if (event.taskId == id && event.type == type) ++result;
        return result;
    }

    int size() const
    {
        std::lock_guard<std::mutex> guard(m_mutex);
        return m_events.size();
    }

    QString sequence(TaskId id) const
    {
        std::lock_guard<std::mutex> guard(m_mutex);
        QStringList names;
        for (const auto& event : m_events)
        {
            if (event.taskId != id) continue;
            switch (event.type)
            {
            case TaskEventType::Started: names.append(QStringLiteral("executing")); break;
            case TaskEventType::Succeeded: names.append(QStringLiteral("succeeded")); break;
            case TaskEventType::Failed: names.append(QStringLiteral("failed")); break;
            default: break;
            }
        }
        return names.join(QStringLiteral("->"));
    }

private:
    mutable std::mutex m_mutex;
    QVector<TaskEvent> m_events;
};

bool validTopology(const Model::GeometryObjectSnapshot& geometry)
{
    QSet<Model::GeometryEntityId> ids;
    for (const auto& item : geometry.topology)
    {
        if (item.id == Model::InvalidGeometryEntityId || ids.contains(item.id)) return false;
        ids.insert(item.id);
    }
    return !geometry.topology.isEmpty() && ids.size() == geometry.topology.size();
}

bool validModelKey(const QString& key)
{
    bool numeric = false;
    const qlonglong value = key.mid(QStringLiteral("fitk-occ:").size()).toLongLong(&numeric);
    return key.startsWith(QStringLiteral("fitk-occ:")) && numeric && value > 0;
}

bool sameTopology(const Model::GeometryObjectSnapshot& left,
                  const Model::GeometryObjectSnapshot& right)
{
    if (left.topology.size() != right.topology.size()) return false;
    for (int index = 0; index < left.topology.size(); ++index)
    {
        if (left.topology.at(index).id != right.topology.at(index).id ||
            left.topology.at(index).type != right.topology.at(index).type)
            return false;
    }
    return true;
}

bool validFailedTask(const std::optional<TaskSnapshot>& snapshot)
{
    return snapshot && snapshot->status == TaskStatus::Failed && snapshot->error &&
           !snapshot->error->code.trimmed().isEmpty() &&
           !snapshot->error->message.trimmed().isEmpty() &&
           !snapshot->diagnostics.isEmpty() &&
           !snapshot->diagnostics.front().stage.trimmed().isEmpty();
}

class RecordingReader final : public IGeometryReader
{
public:
    explicit RecordingReader(std::shared_ptr<const IGeometryReader> reader)
        : m_reader(std::move(reader)) {}
    QString key() const override { return m_reader->key(); }
    QStringList aliases() const override { return m_reader->aliases(); }
    QStringList supportedExtensions() const override { return m_reader->supportedExtensions(); }
    GeometryReadResult read(const GeometryReadRequest& value) const override
    {
        auto result = m_reader->read(value);
        if (result.geometry)
        {
            std::lock_guard<std::mutex> guard(m_mutex);
            m_modelKeys[value.filePath] = result.geometry->adapter.modelKey;
        }
        return result;
    }
    QString modelKey(const QString& path) const
    {
        std::lock_guard<std::mutex> guard(m_mutex);
        const auto found = m_modelKeys.find(path);
        return found == m_modelKeys.end() ? QString() : found->second;
    }
private:
    std::shared_ptr<const IGeometryReader> m_reader;
    mutable std::mutex m_mutex;
    mutable std::map<QString, QString> m_modelKeys;
};

class GatedRealReader final : public IGeometryReader
{
public:
    explicit GatedRealReader(std::shared_ptr<const IGeometryReader> reader) : m_reader(std::move(reader)) {}
    QString key() const override { return m_reader->key(); }
    QStringList aliases() const override { return m_reader->aliases(); }
    QStringList supportedExtensions() const override { return m_reader->supportedExtensions(); }
    GeometryReadResult read(const GeometryReadRequest& value) const override
    {
        auto result = m_reader->read(value); // Real FITK/OCC read and transaction staging happen first.
        if (!result.succeeded()) return result;
        std::unique_lock<std::mutex> locker(m_mutex);
        if (!m_armed) return result;
        m_started = true;
        m_changed.notify_all();
        m_changed.wait(locker, [this] { return m_released; });
        return result;
    }
    void arm() const
    {
        std::lock_guard<std::mutex> locker(m_mutex);
        m_armed = true;
        m_started = false;
        m_released = false;
    }
    bool waitUntilStarted(int timeoutMs) const
    {
        std::unique_lock<std::mutex> locker(m_mutex);
        return m_changed.wait_for(locker, std::chrono::milliseconds(timeoutMs),
                                  [this] { return m_started; });
    }
    void release() const
    {
        std::lock_guard<std::mutex> locker(m_mutex);
        m_released = true;
        m_changed.notify_all();
    }
private:
    std::shared_ptr<const IGeometryReader> m_reader;
    mutable std::mutex m_mutex;
    mutable std::condition_variable m_changed;
    mutable bool m_armed = false;
    mutable bool m_started = false;
    mutable bool m_released = false;
};
}

int main(int argc, char* argv[])
{
    QApplication application(argc, argv);
    TestSuite suite;
    QTemporaryDir temporary;
    suite.expect(temporary.isValid(), QStringLiteral("T017 temporary fixture directory is available"));
    const QString validPath = temporary.filePath(QStringLiteral("box.brep"));
    suite.expect(createFixture(validPath), QStringLiteral("T017 creates a deterministic real BRep fixture"));

    Model::ApplicationRuntime runtime;
    Model::GeometryManager geometryManager(runtime);
    Model::MeshManager meshManager(runtime, geometryManager);
    auto repository = std::make_shared<GeometryIO::FITKGeometryRepository>();
    auto productionReaders = std::make_shared<GeometryReaderRegistry>();
    suite.expect(GeometryIO::registerFirstReleaseGeometryReaders(*productionReaders, repository).succeeded(),
                 QStringLiteral("T017 registers the production FITK/OCC readers"));
    auto brepReader = std::make_shared<RecordingReader>(productionReaders->find(QStringLiteral("brep")));
    auto stepReader = std::make_shared<RecordingReader>(productionReaders->find(QStringLiteral("step")));
    auto igesReader = std::make_shared<RecordingReader>(productionReaders->find(QStringLiteral("iges")));
    auto registry = std::make_shared<GeometryReaderRegistry>();
    suite.expect(registry->registerReader(brepReader).succeeded() &&
                     registry->registerReader(stepReader).succeeded() &&
                     registry->registerReader(igesReader).succeeded(),
                 QStringLiteral("T017 wraps production readers with value-only model-key observation"));
    auto executor = std::make_shared<FITKTaskExecutor>();
    TaskService service(executor);
    Gui::ModelTree tree;
    Gui::ConsoleWidget console;
    suite.expect(tree.bind(&runtime, &geometryManager, &meshManager).succeeded(),
                 QStringLiteral("T017 binds the production ModelTree"));
    QVector<Model::ObjectId> refreshed;
    auto operation = std::make_shared<ImportGeometryOperator>(geometryManager, registry);
    GeometryImportController controller(service, operation, &console, &tree,
                                        [&refreshed](Model::ObjectId id) { refreshed.append(id); });

    EventLog events;
    const auto subscription = service.subscribe([&](const TaskEvent& event) {
        events.append(event);
    });
    const auto submitted = controller.routeDialogResult(request(validPath, temporary.path(), 1), true);
    suite.expect(submitted.taskId && waitForTerminal(application, service, *submitted.taskId),
                 QStringLiteral("real GUI route submits and completes a real FITK/OCC import task"));
    const auto completed = submitted.taskId ? service.findById(*submitted.taskId) : std::nullopt;
    const Model::ObjectId importedId = completed && completed->result ? completed->result->objectId : Model::InvalidObjectId;
    suite.expect(completed && completed->status == TaskStatus::Succeeded && importedId != Model::InvalidObjectId &&
                     runtime.objectCount() == 1 && tree.objectModel()->indexForObject(importedId).isValid() &&
                     refreshed.size() == 1 && refreshed.front() == importedId &&
                     tree.lastAutomaticRefreshOccurredOnGuiThread() && console.lastMutationOccurredOnGuiThread(),
                 QStringLiteral("success publishes one GeometryObject, refreshes ModelTree and notifies the placeholder view boundary on the GUI thread"));
    suite.expect(submitted.taskId && events.count(*submitted.taskId, TaskEventType::Started) == 1 &&
                     events.count(*submitted.taskId, TaskEventType::Succeeded) == 1 &&
                     events.count(*submitted.taskId, TaskEventType::Failed) == 0,
                 QStringLiteral("successful import emits one Executing and one terminal event"));
    const auto geometry = geometryManager.findById(importedId);
    const QString brepModelKey = brepReader->modelKey(validPath);
    suite.expect(geometry && geometry->adapterProviderKey == QStringLiteral("fitk.occ") &&
                     validModelKey(brepModelKey) && repository->contains(brepModelKey) &&
                     validTopology(*geometry) && repository->modelCount() == 1,
                 QStringLiteral("success preserves stable FITK adapter and topology values"));

    const struct { const char* extension; const char* key; } formats[] = {
        {"step", "step"}, {"stp", "stp"}, {"iges", "iges"}, {"igs", "igs"}};
    quint64 nextToken = 10;
    for (const auto& format : formats)
    {
        const QString path = temporary.filePath(QStringLiteral("box.%1").arg(QString::fromLatin1(format.extension)));
        const bool fixture = createFixture(path);
        const auto routed = controller.routeDialogResult(
            request(path, temporary.path(), nextToken++, QString::fromLatin1(format.key)), true);
        const bool terminal = routed.taskId && waitForTerminal(application, service, *routed.taskId);
        const auto snapshot = routed.taskId ? service.findById(*routed.taskId) : std::nullopt;
        const Model::ObjectId id = snapshot && snapshot->result ? snapshot->result->objectId : Model::InvalidObjectId;
        const auto imported = id == Model::InvalidObjectId ? std::optional<Model::GeometryObjectSnapshot>{} : geometryManager.findById(id);
        const auto recording = QString::fromLatin1(format.key) == QStringLiteral("stp")
            ? stepReader : (QString::fromLatin1(format.key) == QStringLiteral("igs") ? igesReader :
                            (QString::fromLatin1(format.key) == QStringLiteral("step") ? stepReader : igesReader));
        const QString modelKey = recording->modelKey(path);
        const int refreshCount = static_cast<int>(std::count(refreshed.cbegin(), refreshed.cend(), id));
        suite.expect(fixture && routed.taskId && terminal && snapshot && snapshot->status == TaskStatus::Succeeded &&
                         events.count(*routed.taskId, TaskEventType::Started) == 1 &&
                         events.count(*routed.taskId, TaskEventType::Succeeded) == 1 &&
                         events.count(*routed.taskId, TaskEventType::Failed) == 0 &&
                         id != Model::InvalidObjectId && snapshot->result && snapshot->result->objectId == id &&
                         imported && imported->adapterProviderKey == QStringLiteral("fitk.occ") &&
                         validModelKey(modelKey) && repository->contains(modelKey) && validTopology(*imported) &&
                         tree.objectModel()->indexForObject(id).isValid() && refreshCount == 1 &&
                         tree.lastAutomaticRefreshOccurredOnGuiThread() && console.lastMutationOccurredOnGuiThread(),
                     QStringLiteral("real GUI route imports ") + QString::fromLatin1(format.extension));
    }

    const int beforeFailure = runtime.objectCount();
    const int modelsBeforeFailure = repository->modelCount();
    const int treeRowsBeforeFailure = tree.objectModel()->rowCount();
    const int refreshesBeforeFailures = refreshed.size();
    const auto existingDataIntact = [&] {
        const auto current = geometryManager.findById(importedId);
        return current && geometry &&
               current->common.name == geometry->common.name &&
               current->adapterProviderKey == geometry->adapterProviderKey &&
               sameTopology(*current, *geometry) && repository->contains(brepModelKey) &&
               runtime.objectCount() == beforeFailure &&
               geometryManager.objectCount() == beforeFailure &&
               repository->modelCount() == modelsBeforeFailure &&
               tree.objectModel()->rowCount() == treeRowsBeforeFailure &&
               tree.objectModel()->indexForObject(importedId).isValid() &&
               refreshed.size() == refreshesBeforeFailures;
    };
    const QString emptyPath = temporary.filePath(QStringLiteral("empty.brep"));
    QFile(emptyPath).open(QIODevice::WriteOnly);
    const auto failed = controller.routeDialogResult(request(emptyPath, temporary.path(), 2), true);
    suite.expect(failed.taskId && waitForTerminal(application, service, *failed.taskId),
                 QStringLiteral("empty real geometry is reported through a task terminal state"));
    const auto failedSnapshot = failed.taskId ? service.findById(*failed.taskId) : std::nullopt;
    suite.expect(validFailedTask(failedSnapshot) && failedSnapshot->error &&
                     failedSnapshot->error->code == QStringLiteral("GEO-IO-FILE-EMPTY") &&
                     existingDataIntact() &&
                     console.plainText().contains(QStringLiteral("GEO-IO-FILE-EMPTY")),
                 QStringLiteral("failed import retains existing data, tree, FITK models and uses queued GUI diagnostics"));
    suite.expect(failed.taskId && events.count(*failed.taskId, TaskEventType::Started) == 1 &&
                     events.count(*failed.taskId, TaskEventType::Failed) == 1 &&
                     events.count(*failed.taskId, TaskEventType::Succeeded) == 0,
                 QStringLiteral("empty-file import emits one Executing and one Failed terminal event"));

    const QString corruptPath = temporary.filePath(QStringLiteral("corrupt.step"));
    QFile corrupt(corruptPath); corrupt.open(QIODevice::WriteOnly); corrupt.write("not a STEP file"); corrupt.close();
    const auto corruptTask = controller.routeDialogResult(request(corruptPath, temporary.path(), 3, QStringLiteral("step")), true);
    const bool corruptTerminal = corruptTask.taskId && waitForTerminal(application, service, *corruptTask.taskId);
    const auto corruptSnapshot = corruptTask.taskId ? service.findById(*corruptTask.taskId) : std::nullopt;
    suite.expect(corruptTerminal && validFailedTask(corruptSnapshot) && corruptSnapshot->error &&
                     corruptSnapshot->error->code == QStringLiteral("GEO-IO-OCC-READ-FAILED") &&
                     existingDataIntact(),
                 QStringLiteral("corrupt real STEP content fails asynchronously without replacing existing geometry"));
    suite.expect(corruptTask.taskId && events.count(*corruptTask.taskId, TaskEventType::Started) == 1 &&
                     events.count(*corruptTask.taskId, TaskEventType::Failed) == 1 &&
                     events.count(*corruptTask.taskId, TaskEventType::Succeeded) == 0,
                 QStringLiteral("corrupt import emits one Executing and one Failed terminal event"));

    const QString mismatchPath = temporary.filePath(QStringLiteral("mismatch.step"));
    QFile::copy(validPath, mismatchPath);
    const auto mismatchTask = controller.routeDialogResult(request(mismatchPath, temporary.path(), 4, QStringLiteral("step")), true);
    const bool mismatchTerminal = mismatchTask.taskId && waitForTerminal(application, service, *mismatchTask.taskId);
    const auto mismatchSnapshot = mismatchTask.taskId ? service.findById(*mismatchTask.taskId) : std::nullopt;
    suite.expect(mismatchTerminal && validFailedTask(mismatchSnapshot) &&
                     existingDataIntact(),
                 QStringLiteral("extension-content mismatch fails asynchronously without residue"));
    suite.expect(mismatchTask.taskId && events.count(*mismatchTask.taskId, TaskEventType::Started) == 1 &&
                     events.count(*mismatchTask.taskId, TaskEventType::Failed) == 1 &&
                     events.count(*mismatchTask.taskId, TaskEventType::Succeeded) == 0,
                 QStringLiteral("mismatch import emits one Executing and one Failed terminal event"));

    const int eventsBeforeUnsupported = events.size();
    const auto unsupported = controller.routeDialogResult(request(validPath, temporary.path(), 5, QStringLiteral("unknown")), true);
    application.processEvents();
    suite.expect(!unsupported.taskId && !unsupported.succeeded() &&
                     events.size() == eventsBeforeUnsupported && existingDataIntact(),
                 QStringLiteral("unsupported format is rejected at the GUI-to-operator boundary without side effects"));

    Model::ApplicationRuntime rejectedRuntime(1, [](Model::ObjectId id, Model::DataObjectType) {
        Common::OperationResult result;
        if (id == 1) return result;
        result.add({QStringLiteral("test"), QStringLiteral("T017-APPMESH-PUBLISH-FAILED"),
                    QStringLiteral("Controlled APPMesh publication failure."), QString(), false,
                    QString(), QStringLiteral("t017"), QString(), QString()});
        return result;
    });
    Model::GeometryManager rejectedManager(rejectedRuntime);
    Model::MeshManager rejectedMesh(rejectedRuntime, rejectedManager);
    Gui::ModelTree rejectedTree;
    Gui::ConsoleWidget rejectedConsole;
    rejectedTree.bind(&rejectedRuntime, &rejectedManager, &rejectedMesh);
    auto rejectedRepository = std::make_shared<GeometryIO::FITKGeometryRepository>();
    rejectedManager.setExternalResourceReleaser(
        [rejectedRepository](const Model::GeometryObject& object) {
            rejectedRepository->release(object.adapter.modelKey);
        });
    auto rejectedProductionReaders = std::make_shared<GeometryReaderRegistry>();
    GeometryIO::registerFirstReleaseGeometryReaders(*rejectedProductionReaders, rejectedRepository);
    auto rejectedRecordingReader = std::make_shared<RecordingReader>(
        rejectedProductionReaders->find(QStringLiteral("brep")));
    auto rejectedRegistry = std::make_shared<GeometryReaderRegistry>();
    rejectedRegistry->registerReader(rejectedRecordingReader);
    TaskService rejectedService(std::make_shared<FITKTaskExecutor>());
    EventLog rejectedEvents;
    auto rejectedSubscription = rejectedService.subscribe([&](const TaskEvent& event) {
        rejectedEvents.append(event);
    });
    QVector<Model::ObjectId> rejectedRefreshes;
    GeometryImportController rejectedController(rejectedService,
        std::make_shared<ImportGeometryOperator>(rejectedManager, rejectedRegistry),
        &rejectedConsole, &rejectedTree,
        [&rejectedRefreshes](Model::ObjectId id) { rejectedRefreshes.append(id); });
    const auto rejectedBaselineTask = rejectedController.routeDialogResult(
        request(validPath, temporary.path(), 60), true);
    suite.expect(rejectedBaselineTask.taskId &&
                     waitForTerminal(application, rejectedService, *rejectedBaselineTask.taskId),
                 QStringLiteral("APPMesh failure fixture first publishes a valid real geometry"));
    const auto rejectedBaselineSnapshot = rejectedBaselineTask.taskId
        ? rejectedService.findById(*rejectedBaselineTask.taskId) : std::nullopt;
    const Model::ObjectId rejectedBaselineId = rejectedBaselineSnapshot && rejectedBaselineSnapshot->result
        ? rejectedBaselineSnapshot->result->objectId : Model::InvalidObjectId;
    const auto rejectedBaselineGeometry = rejectedManager.findById(rejectedBaselineId);
    const QString rejectedBaselineModelKey = rejectedRecordingReader->modelKey(validPath);
    const int rejectedRefreshCount = rejectedRefreshes.size();
    suite.expect(rejectedBaselineSnapshot && rejectedBaselineSnapshot->status == TaskStatus::Succeeded &&
                     rejectedBaselineGeometry && validModelKey(rejectedBaselineModelKey) &&
                     rejectedRepository->contains(rejectedBaselineModelKey) &&
                     rejectedTree.objectModel()->indexForObject(rejectedBaselineId).isValid() &&
                     rejectedRefreshCount == 1,
                 QStringLiteral("APPMesh failure fixture records its existing model, tree and view state"));

    const QString rejectedPath = temporary.filePath(QStringLiteral("publish-failure.brep"));
    suite.expect(createFixture(rejectedPath),
                 QStringLiteral("APPMesh failure fixture uses a distinct real OCC file"));
    const auto rejectedTask = rejectedController.routeDialogResult(
        request(rejectedPath, temporary.path(), 61), true);
    const bool rejectedTerminal = rejectedTask.taskId && waitForTerminal(application, rejectedService, *rejectedTask.taskId);
    const auto rejectedSnapshot = rejectedTask.taskId ? rejectedService.findById(*rejectedTask.taskId) : std::nullopt;
    const auto rejectedPreservedGeometry = rejectedManager.findById(rejectedBaselineId);
    suite.expect(rejectedTerminal && validFailedTask(rejectedSnapshot) &&
                     rejectedSnapshot->error && rejectedSnapshot->error->code == QStringLiteral("T017-APPMESH-PUBLISH-FAILED") &&
                     rejectedSnapshot->diagnostics.front().code == QStringLiteral("T017-APPMESH-PUBLISH-FAILED") &&
                     rejectedRuntime.objectCount() == 1 && rejectedManager.objectCount() == 1 &&
                     rejectedRepository->modelCount() == 1 &&
                     rejectedRepository->contains(rejectedBaselineModelKey) &&
                     rejectedPreservedGeometry && rejectedBaselineGeometry &&
                     rejectedPreservedGeometry->common.name == rejectedBaselineGeometry->common.name &&
                     rejectedPreservedGeometry->adapterProviderKey == rejectedBaselineGeometry->adapterProviderKey &&
                     sameTopology(*rejectedPreservedGeometry, *rejectedBaselineGeometry) &&
                     rejectedTree.objectModel()->rowCount() == 1 &&
                     rejectedTree.objectModel()->indexForObject(rejectedBaselineId).isValid() &&
                     rejectedRefreshes.size() == rejectedRefreshCount,
                 QStringLiteral("APPMesh publication failure remains primary and rolls back the real FITK model"));
    suite.expect(rejectedTask.taskId &&
                     rejectedEvents.count(*rejectedTask.taskId, TaskEventType::Started) == 1 &&
                     rejectedEvents.count(*rejectedTask.taskId, TaskEventType::Failed) == 1 &&
                     rejectedEvents.count(*rejectedTask.taskId, TaskEventType::Succeeded) == 0,
                 QStringLiteral("APPMesh publication failure emits one Executing and one Failed terminal event"));
    rejectedService.stopAccepting();
    rejectedService.drain();
    rejectedService.stopAccepting();
    rejectedService.drain();
    rejectedSubscription.reset();
    rejectedTree.unbind();

    auto lifecycleRepository = std::make_shared<GeometryIO::FITKGeometryRepository>();
    EventLog lifecycleEvents;
    QVector<Model::ObjectId> lifecycleRefreshes;
    QPointer<Gui::ModelTree> lifecycleTreeGuard;
    QPointer<Gui::ConsoleWidget> lifecycleConsoleGuard;
    std::optional<TaskId> lifecycleSlowTaskId;
    int lifecycleEventsAfterDrain = 0;
    int lifecycleRefreshesAfterDrain = 0;
    {
        auto lifecycleRuntime = std::make_unique<Model::ApplicationRuntime>();
        auto lifecycleGeometry = std::make_unique<Model::GeometryManager>(*lifecycleRuntime);
        lifecycleGeometry->setExternalResourceReleaser(
            [lifecycleRepository](const Model::GeometryObject& object) {
                lifecycleRepository->release(object.adapter.modelKey);
            });
        auto lifecycleMesh = std::make_unique<Model::MeshManager>(*lifecycleRuntime, *lifecycleGeometry);
        auto lifecycleTree = std::make_unique<Gui::ModelTree>();
        auto lifecycleConsole = std::make_unique<Gui::ConsoleWidget>();
        lifecycleTreeGuard = lifecycleTree.get();
        lifecycleConsoleGuard = lifecycleConsole.get();
        suite.expect(lifecycleTree->bind(lifecycleRuntime.get(), lifecycleGeometry.get(), lifecycleMesh.get()).succeeded(),
                     QStringLiteral("controlled lifecycle binds its own GUI observers"));

        auto lifecycleProductionReaders = std::make_shared<GeometryReaderRegistry>();
        suite.expect(GeometryIO::registerFirstReleaseGeometryReaders(
                         *lifecycleProductionReaders, lifecycleRepository).succeeded(),
                     QStringLiteral("controlled lifecycle registers the real FITK/OCC reader"));
        auto lifecycleRecordingReader = std::make_shared<RecordingReader>(
            lifecycleProductionReaders->find(QStringLiteral("brep")));
        auto lifecycleGatedReader = std::make_shared<GatedRealReader>(lifecycleRecordingReader);
        auto lifecycleRegistry = std::make_shared<GeometryReaderRegistry>();
        suite.expect(lifecycleRegistry->registerReader(lifecycleGatedReader).succeeded(),
                     QStringLiteral("SC-002 uses a gate after the real FITK/OCC BRep read"));
        auto lifecycleService = std::make_unique<TaskService>(std::make_shared<FITKTaskExecutor>());
        auto lifecycleOperation = std::make_shared<ImportGeometryOperator>(
            *lifecycleGeometry, lifecycleRegistry);
        auto lifecycleController = std::make_unique<GeometryImportController>(
            *lifecycleService, lifecycleOperation, lifecycleConsole.get(), lifecycleTree.get(),
            [&lifecycleRefreshes](Model::ObjectId id) { lifecycleRefreshes.append(id); });
        auto lifecycleSubscription = lifecycleService->subscribe([&](const TaskEvent& event) {
            lifecycleEvents.append(event);
        });

        const auto preservedRoute = lifecycleController->routeDialogResult(
            request(validPath, temporary.path(), 40), true);
        suite.expect(preservedRoute.taskId &&
                         waitForTerminal(application, *lifecycleService, *preservedRoute.taskId),
                     QStringLiteral("controlled lifecycle first imports a valid object"));
        const auto preservedTask = preservedRoute.taskId
            ? lifecycleService->findById(*preservedRoute.taskId) : std::nullopt;
        const Model::ObjectId preservedId = preservedTask && preservedTask->result
            ? preservedTask->result->objectId : Model::InvalidObjectId;
        const auto preservedGeometry = lifecycleGeometry->findById(preservedId);
        const QString preservedModelKey = lifecycleRecordingReader->modelKey(validPath);
        const int preservedRuntimeCount = lifecycleRuntime->objectCount();
        const int preservedGeometryCount = lifecycleGeometry->objectCount();
        const int preservedRepositoryCount = lifecycleRepository->modelCount();
        const int preservedRefreshCount = static_cast<int>(std::count(
            lifecycleRefreshes.cbegin(), lifecycleRefreshes.cend(), preservedId));
        suite.expect(preservedTask && preservedTask->status == TaskStatus::Succeeded && preservedGeometry &&
                         validModelKey(preservedModelKey) && lifecycleRepository->contains(preservedModelKey) &&
                         lifecycleTree->objectModel()->indexForObject(preservedId).isValid() &&
                         preservedRuntimeCount == 1 && preservedGeometryCount == 1 &&
                         preservedRepositoryCount == 1 && preservedRefreshCount == 1,
                     QStringLiteral("controlled lifecycle records the valid object, tree, view and FITK model before shutdown"));

        lifecycleGatedReader->arm();
        const QString slowPath = temporary.filePath(QStringLiteral("controlled-slow.brep"));
        suite.expect(createFixture(slowPath),
                     QStringLiteral("controlled lifecycle creates a distinct real OCC fixture"));
        const auto slow = lifecycleController->routeDialogResult(
            request(slowPath, temporary.path(), 41), true);
        lifecycleSlowTaskId = slow.taskId;
        suite.expect(slow.taskId && lifecycleGatedReader->waitUntilStarted(10000),
                     QStringLiteral("controlled lifecycle reaches the post-OCC gate while Executing"));

        QVector<qint64> samples;
        QElapsedTimer clock;
        clock.start();
        QTimer heartbeat;
        heartbeat.setInterval(100);
        heartbeat.setTimerType(Qt::PreciseTimer);
        QObject::connect(&heartbeat, &QTimer::timeout, [&] { samples.append(clock.elapsed()); });
        heartbeat.start();
        QTimer::singleShot(5200, [lifecycleGatedReader] { lifecycleGatedReader->release(); });

        lifecycleService->stopAccepting();
        lifecycleService->stopAccepting();
        auto draining = std::async(std::launch::async, [&lifecycleService] {
            lifecycleService->drain();
        });
        const bool drainBlocked =
            draining.wait_for(std::chrono::milliseconds(50)) == std::future_status::timeout;
        const int eventsBeforeRejectedSubmit = lifecycleEvents.size();
        const auto rejectedDuringDrain = lifecycleController->routeDialogResult(
            request(validPath, temporary.path(), 42), true);
        application.processEvents();
        const bool rejectedCreatedNoEvents =
            lifecycleEvents.size() == eventsBeforeRejectedSubmit;
        const bool slowTerminal = slow.taskId &&
            waitForTerminal(application, *lifecycleService, *slow.taskId, 15000);
        const bool drainCompleted =
            draining.wait_for(std::chrono::seconds(2)) == std::future_status::ready;
        heartbeat.stop();

        qint64 maximumInterval = 0;
        for (int index = 1; index < samples.size(); ++index)
            maximumInterval = qMax(maximumInterval, samples.at(index) - samples.at(index - 1));
        const auto slowSnapshot = slow.taskId
            ? lifecycleService->findById(*slow.taskId) : std::nullopt;
        const auto preservedAfterDrain = lifecycleGeometry->findById(preservedId);
        const QString states = slow.taskId ? lifecycleEvents.sequence(*slow.taskId) : QString();
        QTextStream(stderr) << "T017 heartbeat samples=" << samples.size()
                            << " maxIntervalMs=" << maximumInterval
                            << " states=" << states << "\n";

        suite.expect(drainBlocked && !rejectedDuringDrain.taskId && rejectedCreatedNoEvents &&
                         slowTerminal && drainCompleted && slowSnapshot &&
                         slowSnapshot->status == TaskStatus::Succeeded && slow.taskId &&
                         lifecycleEvents.count(*slow.taskId, TaskEventType::Started) == 1 &&
                         lifecycleEvents.count(*slow.taskId, TaskEventType::Succeeded) == 1 &&
                         lifecycleEvents.count(*slow.taskId, TaskEventType::Failed) == 0 &&
                         samples.size() >= 50 && maximumInterval <= 500,
                     QStringLiteral("SC-002 keeps the GUI responsive while drain blocks, rejects new work, and observes one terminal state"));
        suite.expect(preservedAfterDrain && preservedGeometry &&
                         preservedAfterDrain->common.name == preservedGeometry->common.name &&
                         preservedAfterDrain->adapterProviderKey == preservedGeometry->adapterProviderKey &&
                         sameTopology(*preservedAfterDrain, *preservedGeometry) &&
                         lifecycleRecordingReader->modelKey(validPath) == preservedModelKey &&
                         lifecycleRepository->contains(preservedModelKey) &&
                         lifecycleTree->objectModel()->indexForObject(preservedId).isValid() &&
                         static_cast<int>(std::count(lifecycleRefreshes.cbegin(), lifecycleRefreshes.cend(), preservedId)) == 1 &&
                         lifecycleRuntime->objectCount() == preservedRuntimeCount + 1 &&
                         lifecycleGeometry->objectCount() == preservedGeometryCount + 1 &&
                         lifecycleRepository->modelCount() == preservedRepositoryCount + 1,
                     QStringLiteral("drain preserves the previous object, model key, topology, tree, view and FITK model"));

        lifecycleService->stopAccepting();
        lifecycleService->drain();
        lifecycleEventsAfterDrain = lifecycleEvents.size();
        lifecycleRefreshesAfterDrain = lifecycleRefreshes.size();
        lifecycleSubscription.reset();
        lifecycleController.reset();
        lifecycleTree->unbind();
        lifecycleTree.reset();
        lifecycleConsole.reset();
        application.processEvents();
        lifecycleService->stopAccepting();
        lifecycleService->drain();
        lifecycleService.reset();
        lifecycleOperation.reset();
        lifecycleMesh.reset();
        lifecycleGeometry.reset();
        suite.expect(lifecycleRepository->modelCount() == 0,
                     QStringLiteral("GeometryManager destruction releases every finalized FITK model"));
        lifecycleRuntime.reset();
    }
    QEventLoop lateCallbackWindow;
    QTimer::singleShot(100, &lateCallbackWindow, &QEventLoop::quit);
    lateCallbackWindow.exec();
    suite.expect(lifecycleTreeGuard.isNull() && lifecycleConsoleGuard.isNull() &&
                     lifecycleRepository->modelCount() == 0 &&
                     lifecycleEvents.size() == lifecycleEventsAfterDrain &&
                     lifecycleRefreshes.size() == lifecycleRefreshesAfterDrain &&
                     lifecycleSlowTaskId &&
                     lifecycleEvents.count(*lifecycleSlowTaskId, TaskEventType::Succeeded) == 1 &&
                     lifecycleEvents.count(*lifecycleSlowTaskId, TaskEventType::Failed) == 0,
                 QStringLiteral("ordered destruction leaves no repository models, late GUI callbacks or duplicate terminal events"));

    service.stopAccepting();
    suite.expect(!controller.routeDialogResult(request(validPath, temporary.path(), 3), true).taskId,
                 QStringLiteral("controlled shutdown rejects new GUI-routed imports"));
    service.drain();
    tree.unbind();
    return suite.result();
}
