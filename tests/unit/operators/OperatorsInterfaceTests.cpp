#include "operators/OperatorsInterface/IOperator.h"
#include "operators/OperatorsInterface/Task.h"
#include "TestSupport.h"

#include <QCoreApplication>

#include <limits>
#include <stdexcept>

namespace
{
using namespace AppMesh;
using namespace AppMesh::Operators;

ErrorInfo validError(TaskId taskId = InvalidTaskId)
{
    return {QStringLiteral("validation"),
            QStringLiteral("TASK-TEST-ERROR"),
            QStringLiteral("Controlled failure"),
            QStringLiteral("The test requested a failure."),
            true,
            taskId,
            42,
            QStringLiteral("parameters/value")};
}

OperatorInput validInput()
{
    OperatorInput input;
    input.operationKey = QStringLiteral("test.operation");
    input.parameters.insert(QStringLiteral("value"), 7);
    input.sourceObjectIds = {42};
    input.description = QStringLiteral("contract test");
    return input;
}

class TestOperator final : public IOperator
{
public:
    QString key() const override
    {
        return QStringLiteral("test.operation");
    }

    Common::OperationResult validate(const OperatorInput& input) const override
    {
        Common::OperationResult result;
        if (!input.parameters.contains(QStringLiteral("value")))
        {
            result.add({QStringLiteral("validation"),
                        QStringLiteral("TASK-TEST-VALUE-MISSING"),
                        QStringLiteral("The value parameter is required."),
                        QStringLiteral("Provide parameters/value."),
                        true,
                        QStringLiteral("parameters/value"),
                        QStringLiteral("operator.validate"),
                        QString(),
                        QString()});
        }
        return result;
    }

    OperatorResult execute(const OperatorInput& input) override
    {
        if (input.parameters.value(QStringLiteral("value")).toInt() < 0)
        {
            return *OperatorResult::failure(validError());
        }
        ResultReference reference;
        reference.objectId = 42;
        reference.value = input.parameters.value(QStringLiteral("value"));
        return OperatorResult::success(reference);
    }
};

class ThrowingOperator final : public IOperator
{
public:
    QString key() const override { return QStringLiteral("test.throw"); }
    Common::OperationResult validate(const OperatorInput&) const override { return {}; }
    OperatorResult execute(const OperatorInput&) override
    {
        throw std::runtime_error("controlled operator exception");
    }
};
}

int main(int argc, char* argv[])
{
    QCoreApplication application(argc, argv);
    TestSuite suite;

    Common::Diagnostic diagnostic{QStringLiteral("input"),
                                  QStringLiteral("TASK-INPUT-INVALID"),
                                  QStringLiteral("Invalid input"),
                                  QStringLiteral("The selected input is unavailable."),
                                  true,
                                  QStringLiteral("source/42"),
                                  QStringLiteral("operator.validate"),
                                  QStringLiteral("42"),
                                  QStringLiteral("9")};
    const ErrorInfo converted = errorInfoFromDiagnostic(diagnostic);
    const Common::Diagnostic roundTrip = diagnosticFromErrorInfo(converted,
                                                                 diagnostic.stage);
    suite.expect(converted.taskId == 9 && converted.objectId == 42 &&
                     roundTrip.category == diagnostic.category &&
                     roundTrip.code == diagnostic.code &&
                     roundTrip.message == diagnostic.message &&
                     roundTrip.detail == diagnostic.detail &&
                     roundTrip.recoverable == diagnostic.recoverable &&
                     roundTrip.path == diagnostic.path &&
                     roundTrip.taskId == diagnostic.taskId &&
                     roundTrip.objectId == diagnostic.objectId,
                 QStringLiteral("Diagnostic and ErrorInfo round-trip every shared field"));
    ErrorInfo copiedError = converted;
    copiedError.detail = QStringLiteral("changed");
    suite.expect(converted.detail != copiedError.detail &&
                     !validateErrorInfo(ErrorInfo{}).succeeded(),
                 QStringLiteral("ErrorInfo is an independent validated value"));

    TaskIdAllocator allocator;
    const auto firstId = allocator.allocate();
    const auto secondId = allocator.allocate();
    TaskIdAllocator exhaustedAllocator(std::numeric_limits<TaskId>::max());
    const auto maximumId = exhaustedAllocator.allocate();
    const auto exhaustedId = exhaustedAllocator.allocate();
    suite.expect(firstId.id == 1 && secondId.id == 2 &&
                     maximumId.id == std::numeric_limits<TaskId>::max() &&
                     !exhaustedId.succeeded() && !exhaustedId.id.has_value(),
                 QStringLiteral("TaskId allocation is nonzero, monotonic and non-wrapping"));

    OperatorInput originalInput = validInput();
    auto created = Task::create(7, originalInput);
    originalInput.parameters[QStringLiteral("value")] = 99;
    originalInput.sourceObjectIds.clear();
    suite.expect(created.succeeded() && created.task &&
                     created.task->snapshot().input.parameters.value(
                         QStringLiteral("value")) == 7 &&
                     created.task->snapshot().input.sourceObjectIds == QVector<Model::ObjectId>{42},
                 QStringLiteral("Task stores an immutable value snapshot of operator input"));
    TaskSnapshot detachedSnapshot = created.task->snapshot();
    detachedSnapshot.input.parameters[QStringLiteral("value")] = 123;
    detachedSnapshot.diagnostics.append(diagnostic);
    suite.expect(created.task->snapshot().input.parameters.value(
                     QStringLiteral("value")) == 7 &&
                     created.task->snapshot().diagnostics.isEmpty(),
                 QStringLiteral("TaskSnapshot mutation cannot change Task state"));
    suite.expect(!Task::create(InvalidTaskId, validInput()).succeeded() &&
                     !Task::create(8, OperatorInput{}).succeeded(),
                 QStringLiteral("Task rejects invalid identity and empty operation keys"));

    Task& successfulTask = *created.task;
    const TaskSnapshot createdSnapshot = successfulTask.snapshot();
    const auto prematureSuccess = Task::create(10, validInput());
    const auto started = successfulTask.start();
    const auto duplicateStart = successfulTask.start();
    const auto invalidLowProgress = successfulTask.updateProgress(-1);
    const auto invalidHighProgress = successfulTask.updateProgress(101);
    const auto progress = successfulTask.updateProgress(35, QStringLiteral("working"));
    Common::Diagnostic warning{QStringLiteral("operator"),
                               QStringLiteral("TASK-TEST-WARNING"),
                               QStringLiteral("Controlled warning"),
                               QStringLiteral("Execution continues."),
                               true,
                               QStringLiteral("parameters/value"),
                               QStringLiteral("operator.execute"),
                               QString(),
                               QString()};
    const auto reported = successfulTask.reportDiagnostic(warning);
    ResultReference reference;
    reference.objectId = 42;
    reference.path = QStringLiteral("objects/42");
    const auto succeeded = successfulTask.succeed(reference);
    const TaskSnapshot successSnapshot = successfulTask.snapshot();
    const auto failAfterSuccess = successfulTask.fail(validError(7));
    const auto progressAfterSuccess = successfulTask.updateProgress(50);
    suite.expect(createdSnapshot.createdAt.isValid() &&
                     !createdSnapshot.startedAt.isValid() &&
                     !createdSnapshot.completedAt.isValid() &&
                     started.succeeded() && started.event->type == TaskEventType::Started &&
                     started.event->sequence == 1 && !duplicateStart.succeeded() &&
                     !invalidLowProgress.succeeded() && !invalidHighProgress.succeeded() &&
                     progress.succeeded() && progress.event->sequence == 2 &&
                     progress.event->type == TaskEventType::ProgressChanged &&
                     reported.succeeded() && reported.event->sequence == 3 &&
                     reported.event->type == TaskEventType::DiagnosticReported &&
                     succeeded.succeeded() && succeeded.event->sequence == 4 &&
                     succeeded.event->type == TaskEventType::Succeeded,
                 QStringLiteral("Task accepts only ordered Created-Executing-Succeeded updates"));
    suite.expect(successSnapshot.status == TaskStatus::Succeeded &&
                     successSnapshot.startedAt.isValid() &&
                     successSnapshot.completedAt.isValid() &&
                     successSnapshot.progress == 35 &&
                     successSnapshot.phase == QStringLiteral("working") &&
                     successSnapshot.result.has_value() && !successSnapshot.error.has_value() &&
                     !failAfterSuccess.succeeded() && !progressAfterSuccess.succeeded(),
                 QStringLiteral("Succeeded is a single immutable terminal state"));
    suite.expect(prematureSuccess.task &&
                     !prematureSuccess.task->succeed().succeeded() &&
                     !prematureSuccess.task->fail(validError()).succeeded() &&
                     prematureSuccess.task->snapshot().status == TaskStatus::Created,
                 QStringLiteral("Created cannot transition directly to a terminal state"));

    auto failedCreation = Task::create(11, validInput());
    Task& failedTask = *failedCreation.task;
    const auto failedStarted = failedTask.start();
    ErrorInfo invalidError;
    const auto rejectedFailure = failedTask.fail(invalidError);
    const auto failed = failedTask.fail(validError());
    const auto successAfterFailure = failedTask.succeed();
    const TaskSnapshot failedSnapshot = failedTask.snapshot();
    suite.expect(failedStarted.succeeded() && !rejectedFailure.succeeded() &&
                     rejectedFailure.event == std::nullopt && failed.succeeded() &&
                     failed.event->type == TaskEventType::Failed &&
                     failed.event->sequence == 2 && !successAfterFailure.succeeded() &&
                     failedSnapshot.status == TaskStatus::Failed &&
                     failedSnapshot.error.has_value() &&
                     failedSnapshot.error->taskId == failedSnapshot.id &&
                     !failedSnapshot.result.has_value() &&
                     failedSnapshot.diagnostics.size() == 1,
                 QStringLiteral("Failed requires ErrorInfo and is the only terminal state"));

    const auto successfulResult = OperatorResult::success(reference);
    const auto failedResult = OperatorResult::failure(validError());
    const auto missingErrorResult = OperatorResult::failure(std::nullopt);
    suite.expect(successfulResult.succeeded() && successfulResult.result().has_value() &&
                     !successfulResult.error().has_value() && failedResult.has_value() &&
                     !failedResult->succeeded() && failedResult->error().has_value() &&
                     !failedResult->result().has_value() && !missingErrorResult.has_value(),
                 QStringLiteral("OperatorResult enforces mutually exclusive success and failure"));

    TestOperator operation;
    OperatorInput operatorInput = validInput();
    const auto operatorSuccess = executeSafely(operation, operatorInput, 20);
    OperatorInput missingInput = validInput();
    missingInput.parameters.clear();
    const auto validationFailure = executeSafely(operation, missingInput, 21);
    OperatorInput failureInput = validInput();
    failureInput.parameters[QStringLiteral("value")] = -1;
    const auto operatorFailure = executeSafely(operation, failureInput, 22);
    ThrowingOperator throwingOperation;
    const auto exceptionFailure = executeSafely(throwingOperation, validInput(), 23);
    suite.expect(operatorSuccess.succeeded() &&
                     operatorInput.parameters.value(QStringLiteral("value")) == 7 &&
                     !validationFailure.succeeded() &&
                     validationFailure.error()->taskId == 21 &&
                     !operatorFailure.succeeded() && operatorFailure.error()->taskId == 22 &&
                     !exceptionFailure.succeeded() &&
                     exceptionFailure.error()->taskId == 23 &&
                     exceptionFailure.error()->code ==
                         QStringLiteral("TASK-OPERATOR-EXCEPTION"),
                 QStringLiteral("IOperator validates immutable input and converts exceptions"));

    return suite.result();
}
