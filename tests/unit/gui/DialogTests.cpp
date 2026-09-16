#include "gui/GUIDialog/WorkDirectoryDialog.h"
#include "gui/GUIDialog/GeneratorDialog.h"
#include "gui/GUIDialog/ProjectDialog.h"

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QTemporaryDir>
#include <QTemporaryFile>
#include <QThread>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <cassert>
#include <thread>

using namespace AppMesh::Gui;

static bool hasCode(const AppMesh::Common::OperationResult& result, const QString& code)
{
    for (const auto& diagnostic : result.diagnostics)
        if (diagnostic.code == code) return true;
    return false;
}

int main(int argc, char** argv)
{
    qputenv("QT_QPA_PLATFORM", QByteArrayLiteral("offscreen"));
    QApplication app(argc, argv);
    QTemporaryDir temp;
    assert(temp.isValid());

    const QString existing = temp.path();
    WorkDirectoryDialog work(existing);
    assert(work.directory() == existing);
    assert(work.validate().succeeded());
    work.setDirectory(QString());
    assert(!work.validate().succeeded() && hasCode(work.validate(), QStringLiteral("GUI-DIALOG-WORKDIR-EMPTY")));
    work.setDirectory(QStringLiteral("relative/path"));
    assert(hasCode(work.validate(), QStringLiteral("GUI-DIALOG-WORKDIR-NOT-ABSOLUTE")));
    QTemporaryFile file(temp.path() + QStringLiteral("/file-XXXXXX"));
    assert(file.open());
    work.setDirectory(file.fileName());
    assert(hasCode(work.validate(), QStringLiteral("GUI-DIALOG-WORKDIR-NOT-DIRECTORY")));
    const QString missing = temp.path() + QStringLiteral("/new-work");
    work.setDirectory(missing);
    work.setCreateIfMissing(false);
    assert(hasCode(work.validate(), QStringLiteral("GUI-DIALOG-WORKDIR-NOT-FOUND")));
    work.setCreateIfMissing(true);
    assert(work.validate().succeeded());
    assert(!QFileInfo::exists(missing));
    assert(work.tryAccept());
    assert(work.accepted() && work.request().directory == QDir::cleanPath(missing));
    WorkDirectoryDialog cancelled(existing);
    static_cast<QDialog&>(cancelled).reject();
    assert(cancelled.cancelled() && !cancelled.accepted());

    GeneratorDialog generator;
    assert(hasCode(generator.validate(), QStringLiteral("GUI-DIALOG-GENERATOR-ID-EMPTY")));
    generator.setGeneratorId(QStringLiteral("box"));
    assert(generator.validate().succeeded());
    generator.setParameters({{QStringLiteral("size"), 2}});
    assert(generator.request().parameters.value(QStringLiteral("size")).toString() == QStringLiteral("2"));
    auto* table = generator.findChild<QTableWidget*>(QStringLiteral("generatorParametersTable"));
    assert(table);
    table->insertRow(table->rowCount());
    table->setItem(table->rowCount() - 1, 0, new QTableWidgetItem);
    table->setItem(table->rowCount() - 1, 1, new QTableWidgetItem(QStringLiteral("x")));
    assert(hasCode(generator.validate(), QStringLiteral("GUI-DIALOG-GENERATOR-PARAMETER-KEY-EMPTY")));
    table->removeRow(table->rowCount() - 1);
    table->insertRow(table->rowCount());
    table->setItem(table->rowCount() - 1, 0, new QTableWidgetItem(QStringLiteral("size")));
    table->setItem(table->rowCount() - 1, 1, new QTableWidgetItem(QStringLiteral("3")));
    assert(hasCode(generator.validate(), QStringLiteral("GUI-DIALOG-GENERATOR-PARAMETER-KEY-DUPLICATE")));
    generator.setParameters({});
    assert(generator.tryAccept());
    assert(generator.accepted());

    QTemporaryFile projectFile(temp.path() + QStringLiteral("/project-XXXXXX.dat"));
    assert(projectFile.open());
    ProjectDialog open(ProjectDialogMode::Open, projectFile.fileName());
    assert(open.validate().succeeded());
    ProjectDialog openDirectory(ProjectDialogMode::Open, temp.path());
    assert(hasCode(openDirectory.validate(), QStringLiteral("GUI-DIALOG-PROJECT-NOT-FILE")));
    ProjectDialog openMissing(ProjectDialogMode::Open, temp.path() + QStringLiteral("/missing.dat"));
    assert(hasCode(openMissing.validate(), QStringLiteral("GUI-DIALOG-PROJECT-NOT-FOUND")));
    ProjectDialog save(ProjectDialogMode::Save, temp.path() + QStringLiteral("/output.dat"));
    assert(save.validate().succeeded());
    assert(!QFileInfo::exists(temp.path() + QStringLiteral("/output.dat")));
    ProjectDialog saveMissingParent(ProjectDialogMode::Save, temp.path() + QStringLiteral("/missing/output.dat"));
    assert(hasCode(saveMissingParent.validate(), QStringLiteral("GUI-DIALOG-PROJECT-PARENT-NOT-FOUND")));
    ProjectDialog saveExisting(ProjectDialogMode::Save, projectFile.fileName());
    assert(hasCode(saveExisting.validate(), QStringLiteral("GUI-DIALOG-PROJECT-OVERWRITE-REQUIRED")));
    saveExisting.setOverwriteExisting(true);
    assert(saveExisting.validate().succeeded());
    assert(saveExisting.tryAccept());

    bool threadRejected = false;
    std::thread worker([&] { threadRejected = hasCode(work.validate(), QStringLiteral("GUI-DIALOG-THREAD-INVALID")); });
    worker.join();
    assert(threadRejected);
    return 0;
}
