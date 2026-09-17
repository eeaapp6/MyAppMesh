#include "gui/GUIFrame/MainWindow.h"
#include "gui/GUIWidget/ConsoleWidget.h"
#include "gui/GUIWidget/ModelTree.h"
#include "TestSupport.h"

#include <QApplication>
#include <QEventLoop>
#include <QMetaObject>
#include <QObject>
#include <QTextStream>
#include <QTimer>

#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>

namespace
{
enum class TaskState
{
    Running,
    Succeeded,
    Failed
};

QString stateName(TaskState state)
{
    switch (state)
    {
    case TaskState::Running:
        return QStringLiteral("running");
    case TaskState::Succeeded:
        return QStringLiteral("succeeded");
    case TaskState::Failed:
        return QStringLiteral("failed");
    }
    return QStringLiteral("unknown");
}

struct FixtureResult
{
    std::vector<qint64> samples;
    std::vector<TaskState> states;
    bool workerExited = false;
    qint64 workerDurationMilliseconds = 0;
    bool timedOut = false;
    bool guiAssembled = false;
    bool businessEventAfterTerminal = false;
    bool heartbeatAfterTerminal = false;
    bool consoleUpdatedOnGuiThread = false;
    QString consoleText;
};

FixtureResult runFixture(QApplication& application, TaskState terminalState)
{
    using Clock = std::chrono::steady_clock;
    constexpr auto WorkerDuration = std::chrono::milliseconds(5600);

    FixtureResult result;
    QObject dispatcher;
    AppMesh::GUIFrame::MainWindow window;
    auto* tree = new AppMesh::Gui::ModelTree;
    auto* console = new AppMesh::Gui::ConsoleWidget;
    result.guiAssembled = window.setModelTreeWidget(tree) &&
        window.setConsoleWidget(console);
    window.show();
    application.processEvents();
    QEventLoop loop;
    QTimer heartbeat;
    heartbeat.setInterval(100);
    heartbeat.setTimerType(Qt::PreciseTimer);
    QTimer watchdog;
    watchdog.setSingleShot(true);

    const auto sampleEpoch = Clock::now();
    QObject::connect(&heartbeat, &QTimer::timeout, &dispatcher, [&] {
        result.samples.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(
                                     Clock::now() - sampleEpoch)
                                     .count());
    });
    QObject::connect(&watchdog, &QTimer::timeout, &dispatcher, [&] {
        result.timedOut = true;
        loop.quit();
    });

    std::mutex gateMutex;
    std::condition_variable gateChanged;
    bool startWorker = false;
    std::thread worker([&] {
        {
            std::unique_lock<std::mutex> lock(gateMutex);
            gateChanged.wait(lock, [&] { return startWorker; });
        }

        const auto workerStarted = Clock::now();
        QMetaObject::invokeMethod(&dispatcher, [&] {
            result.states.push_back(TaskState::Running);
        }, Qt::QueuedConnection);
        console->appendInfo(QStringLiteral("controlled worker running"),
                            QStringLiteral("t013.heartbeat"));

        const auto deadline = Clock::now() + WorkerDuration;
        std::mutex durationMutex;
        std::condition_variable durationChanged;
        std::unique_lock<std::mutex> durationLock(durationMutex);
        durationChanged.wait_until(durationLock, deadline, [] { return false; });

        console->appendMessage(
            terminalState == TaskState::Succeeded
                ? AppMesh::Gui::ConsoleMessageLevel::Info
                : AppMesh::Gui::ConsoleMessageLevel::Error,
            QStringLiteral("controlled worker %1").arg(stateName(terminalState)),
            QStringLiteral("t013.heartbeat"));
        QMetaObject::invokeMethod(&dispatcher, [&, terminalState] {
            result.states.push_back(terminalState);
            heartbeat.stop();
            loop.quit();
        }, Qt::QueuedConnection);
        result.workerDurationMilliseconds =
            std::chrono::duration_cast<std::chrono::milliseconds>(
                Clock::now() - workerStarted)
                .count();
        result.workerExited = true;
    });

    heartbeat.start();
    watchdog.start(15000);
    {
        std::lock_guard<std::mutex> lock(gateMutex);
        startWorker = true;
    }
    gateChanged.notify_one();
    loop.exec();
    worker.join();
    watchdog.stop();

    const std::size_t statesAtTerminal = result.states.size();
    const std::size_t samplesAtTerminal = result.samples.size();
    QEventLoop drainLoop;
    QTimer::singleShot(250, &drainLoop, &QEventLoop::quit);
    drainLoop.exec();
    result.businessEventAfterTerminal = result.states.size() != statesAtTerminal;
    result.heartbeatAfterTerminal = result.samples.size() != samplesAtTerminal;

    result.consoleUpdatedOnGuiThread = console->lastMutationOccurredOnGuiThread();
    result.consoleText = console->plainText();
    window.close();
    application.processEvents();
    return result;
}

void verifyFixture(TestSuite& suite,
                   const FixtureResult& result,
                   TaskState expectedTerminal)
{
    qint64 minimumInterval = 0;
    qint64 maximumInterval = 0;
    if (result.samples.size() > 1)
    {
        minimumInterval = result.samples[1] - result.samples[0];
        maximumInterval = minimumInterval;
        for (std::size_t index = 2; index < result.samples.size(); ++index)
        {
            const qint64 interval = result.samples[index] - result.samples[index - 1];
            minimumInterval = std::min(minimumInterval, interval);
            maximumInterval = std::max(maximumInterval, interval);
        }
    }

    const int runningCount = static_cast<int>(std::count(
        result.states.begin(), result.states.end(), TaskState::Running));
    const int successCount = static_cast<int>(std::count(
        result.states.begin(), result.states.end(), TaskState::Succeeded));
    const int failureCount = static_cast<int>(std::count(
        result.states.begin(), result.states.end(), TaskState::Failed));
    const int terminalCount = successCount + failureCount;

    QStringList stateSequence;
    for (const TaskState state : result.states)
    {
        stateSequence.append(stateName(state));
    }
    QTextStream(stderr)
        << QStringLiteral("T013 heartbeat terminal=%1 samples=%2 minIntervalMs=%3 "
                          "maxIntervalMs=%4 states=%5 terminalCount=%6 "
                          "workerDurationMs=%7 workerExited=%8\n")
               .arg(stateName(expectedTerminal))
               .arg(result.samples.size())
               .arg(minimumInterval)
               .arg(maximumInterval)
               .arg(stateSequence.join(QStringLiteral("->")))
               .arg(terminalCount)
               .arg(result.workerDurationMilliseconds)
               .arg(result.workerExited);

    suite.expect(!result.timedOut && result.workerExited,
                 QStringLiteral("controlled worker exits and joins before fixture teardown"));
    suite.expect(result.guiAssembled,
                 QStringLiteral("MainWindow remains assembled while the worker runs"));
    suite.expect(result.workerDurationMilliseconds >= 5000,
                 QStringLiteral("controlled worker remains active for at least five seconds"));
    suite.expect(result.samples.size() >= 50,
                 QStringLiteral("100 ms UI heartbeat records at least 50 consecutive samples"));
    suite.expect(result.samples.size() > 1 && maximumInterval <= 500,
                 QStringLiteral("maximum adjacent UI heartbeat interval is at most 500 ms"));
    suite.expect(result.states.size() == 2 && result.states.front() == TaskState::Running &&
                     result.states.back() == expectedTerminal && runningCount == 1 &&
                     terminalCount == 1,
                 QStringLiteral("running is observed before exactly one requested terminal state"));
    suite.expect((expectedTerminal == TaskState::Succeeded && successCount == 1 &&
                  failureCount == 0) ||
                     (expectedTerminal == TaskState::Failed && failureCount == 1 &&
                      successCount == 0),
                 QStringLiteral("success and failure terminal states are mutually exclusive"));
    suite.expect(!result.businessEventAfterTerminal && !result.heartbeatAfterTerminal,
                 QStringLiteral("terminal delivery stops task events and the UI heartbeat"));
    suite.expect(result.consoleUpdatedOnGuiThread &&
                     result.consoleText.contains(QStringLiteral("controlled worker running")) &&
                     result.consoleText.contains(stateName(expectedTerminal)),
                 QStringLiteral("worker diagnostics are queued to ConsoleWidget on the GUI thread"));
}
}

int main(int argc, char* argv[])
{
    QApplication application(argc, argv);
    TestSuite suite;
    verifyFixture(suite, runFixture(application, TaskState::Succeeded), TaskState::Succeeded);
    verifyFixture(suite, runFixture(application, TaskState::Failed), TaskState::Failed);
    return suite.result();
}
