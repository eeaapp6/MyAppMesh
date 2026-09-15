#include "MeshAPPSettings.h"
#include "TestSupport.h"

#include <QCoreApplication>
#include <QFile>
#include <QSettings>
#include <QTemporaryDir>

namespace
{
bool hasCode(const AppMesh::App::AppOperationResult& result, const QString& code)
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
}

int main(int argc, char* argv[])
{
    QCoreApplication application(argc, argv);
    TestSuite suite;
    QTemporaryDir temporaryDirectory;

    const QString recentFilePath = temporaryDirectory.filePath(QStringLiteral("model.step"));
    const QString secondRecentFilePath = temporaryDirectory.filePath(QStringLiteral("mesh.inp"));
    const QString excessRecentFilePath = temporaryDirectory.filePath(QStringLiteral("excess.cgns"));
    const QStringList fixturePaths =
        QStringList() << recentFilePath << secondRecentFilePath << excessRecentFilePath;
    for (const auto& fixturePath : fixturePaths)
    {
        QFile fixture(fixturePath);
        suite.expect(fixture.open(QIODevice::WriteOnly),
                     QStringLiteral("created recent-file fixture: %1").arg(fixturePath));
        fixture.write("fixture");
        fixture.close();
    }

    const QString staleRecentFilePath =
        temporaryDirectory.filePath(QStringLiteral("deleted-model.step"));
    const QString settingsPath = temporaryDirectory.filePath(QStringLiteral("config/save-stale.ini"));
    AppMesh::App::MeshAPPSettings writer(settingsPath, 5);
    AppMesh::App::AppSettings expected;
    expected.workingDirectory = temporaryDirectory.path();
    expected.recentFiles = QStringList() << recentFilePath << staleRecentFilePath;

    const auto saveResult = writer.save(expected);
    suite.expect(saveResult.succeeded(),
                 QStringLiteral("stale recent file does not prevent settings persistence"));

    QSettings persisted(settingsPath, QSettings::IniFormat);
    suite.expect(persisted.value(QStringLiteral("paths/workingDirectory")).toString() ==
                     temporaryDirectory.path(),
                 QStringLiteral("working directory is persisted with stale history"));
    suite.expect(persisted.value(QStringLiteral("history/recentFiles")).toStringList() ==
                     (QStringList() << recentFilePath),
                 QStringLiteral("stale recent file is omitted from persisted settings"));

    AppMesh::App::MeshAPPSettings reader(settingsPath, 5);
    const auto loadResult = reader.load();
    suite.expect(loadResult.succeeded(), QStringLiteral("valid settings restored"));
    suite.expect(loadResult.settings.schemaVersion == AppMesh::App::AppSettings::CurrentSchemaVersion,
                 QStringLiteral("settings schema version restored"));
    suite.expect(loadResult.settings.workingDirectory == temporaryDirectory.path(),
                 QStringLiteral("working directory restored"));
    suite.expect(loadResult.settings.recentFiles == (QStringList() << recentFilePath),
                 QStringLiteral("persisted recent files contain only valid entries"));

    const QString deletedAfterSavePath =
        temporaryDirectory.filePath(QStringLiteral("deleted-after-save.step"));
    QFile deletedAfterSaveFile(deletedAfterSavePath);
    suite.expect(deletedAfterSaveFile.open(QIODevice::WriteOnly),
                 QStringLiteral("created soon-to-be-deleted recent-file fixture"));
    deletedAfterSaveFile.write("fixture");
    deletedAfterSaveFile.close();

    const QString loadSettingsPath =
        temporaryDirectory.filePath(QStringLiteral("config/load-stale.ini"));
    {
        QSettings stored(loadSettingsPath, QSettings::IniFormat);
        stored.setValue(QStringLiteral("meta/schemaVersion"),
                        AppMesh::App::AppSettings::CurrentSchemaVersion);
        stored.setValue(QStringLiteral("paths/workingDirectory"), temporaryDirectory.path());
        stored.setValue(QStringLiteral("history/recentFiles"),
                        QStringList() << recentFilePath << deletedAfterSavePath);
        stored.sync();
        suite.expect(stored.status() == QSettings::NoError,
                     QStringLiteral("created settings fixture containing a recent file"));
    }
    suite.expect(QFile::remove(deletedAfterSavePath),
                 QStringLiteral("deleted recent-file fixture before loading settings"));

    AppMesh::App::MeshAPPSettings staleReader(loadSettingsPath, 5);
    const auto staleLoadResult = staleReader.load();
    suite.expect(staleLoadResult.succeeded(),
                 QStringLiteral("deleted recent file does not prevent settings recovery"));
    suite.expect(staleLoadResult.settings.workingDirectory == temporaryDirectory.path(),
                 QStringLiteral("working directory is recovered with stale history"));
    suite.expect(staleLoadResult.settings.recentFiles == (QStringList() << recentFilePath),
                 QStringLiteral("deleted recent file is filtered during recovery"));

    AppMesh::App::MeshAPPSettings limitedWriter(
        temporaryDirectory.filePath(QStringLiteral("config/mixed.ini")), 2);
    AppMesh::App::AppSettings mixed;
    mixed.workingDirectory = temporaryDirectory.path();
    mixed.recentFiles = QStringList() << recentFilePath << staleRecentFilePath
                                      << recentFilePath.toUpper() << secondRecentFilePath
                                      << excessRecentFilePath;
    const auto mixedSaveResult = limitedWriter.save(mixed);
    suite.expect(mixedSaveResult.succeeded(),
                 QStringLiteral("mixed recent-file history is normalized without failure"));
    const auto mixedLoadResult = limitedWriter.load();
    suite.expect(mixedLoadResult.succeeded(),
                 QStringLiteral("normalized mixed recent-file history is restored"));
    suite.expect(mixedLoadResult.settings.recentFiles ==
                     (QStringList() << recentFilePath << secondRecentFilePath),
                 QStringLiteral("valid entries remain, case-insensitive duplicates and stale entries are removed, and the limit applies"));

    AppMesh::App::AppSettings invalid = expected;
    invalid.workingDirectory = temporaryDirectory.filePath(QStringLiteral("does-not-exist"));
    const auto invalidResult = writer.save(invalid);
    suite.expect(!invalidResult.succeeded(), QStringLiteral("invalid work directory is not persisted"));
    suite.expect(hasCode(invalidResult, QStringLiteral("CFG-WORKDIR-INVALID")),
                 QStringLiteral("invalid work directory has a structured diagnostic"));

    {
        QSettings future(settingsPath, QSettings::IniFormat);
        future.setValue(QStringLiteral("meta/schemaVersion"),
                        AppMesh::App::AppSettings::CurrentSchemaVersion + 1);
        future.sync();
    }
    const auto futureResult = reader.load();
    suite.expect(!futureResult.succeeded(), QStringLiteral("future settings schema is rejected"));
    suite.expect(hasCode(futureResult, QStringLiteral("CFG-VERSION-UNSUPPORTED")),
                 QStringLiteral("future schema reports compatibility diagnostic"));

    return suite.result();
}
