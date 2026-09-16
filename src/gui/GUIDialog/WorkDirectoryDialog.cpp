#include "WorkDirectoryDialog.h"

#include <QCheckBox>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QDir>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QThread>
#include <QRegularExpression>
#include <QApplication>

namespace AppMesh::Gui
{
namespace
{
Common::Diagnostic diag(const QString& code, const QString& message,
                        const QString& detail, const QString& path = {})
{
    return {QStringLiteral("validation"), code, message, detail, false,
            path, QStringLiteral("gui.dialog.work-directory"), QString()};
}
}

WorkDirectoryDialog::WorkDirectoryDialog(const QString& initialDirectory, QWidget* parent)
    : QDialog(parent)
{
    Q_ASSERT(!qApp || QThread::currentThread() == qApp->thread());
    setObjectName(QStringLiteral("workDirectoryDialog"));
    setWindowTitle(tr("Work Directory"));
    auto* form = new QFormLayout(this);
    m_directoryEdit = new QLineEdit(this);
    m_directoryEdit->setObjectName(QStringLiteral("workDirectoryEdit"));
    m_directoryEdit->setText(initialDirectory);
    auto* browse = new QPushButton(tr("Browse..."), this);
    browse->setObjectName(QStringLiteral("browseDirectoryButton"));
    auto* row = new QWidget(this);
    auto* rowLayout = new QHBoxLayout(row);
    rowLayout->addWidget(m_directoryEdit);
    rowLayout->addWidget(browse);
    form->addRow(tr("Directory"), row);
    m_createIfMissing = new QCheckBox(tr("Allow creation by the calling service"), this);
    m_createIfMissing->setObjectName(QStringLiteral("createDirectoryCheckBox"));
    form->addRow(QString(), m_createIfMissing);
    auto* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttons->setObjectName(QStringLiteral("dialogButtonBox"));
    form->addRow(buttons);
    connect(browse, &QPushButton::clicked, this, &WorkDirectoryDialog::chooseDirectory);
    connect(buttons, &QDialogButtonBox::accepted, this, &WorkDirectoryDialog::tryAccept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

QString WorkDirectoryDialog::directory() const { return m_directoryEdit->text(); }
void WorkDirectoryDialog::setDirectory(const QString& directory) { m_directoryEdit->setText(directory); }
bool WorkDirectoryDialog::createIfMissing() const { return m_createIfMissing->isChecked(); }
void WorkDirectoryDialog::setCreateIfMissing(bool enabled) { m_createIfMissing->setChecked(enabled); }
WorkDirectoryRequest WorkDirectoryDialog::request() const
{
    return {QDir::cleanPath(QFileInfo(directory().trimmed()).absoluteFilePath()), createIfMissing()};
}

Common::OperationResult WorkDirectoryDialog::validate() const
{
    Common::OperationResult result;
    if (!qApp || QThread::currentThread() != qApp->thread())
    {
        result.add(diag(QStringLiteral("GUI-DIALOG-THREAD-INVALID"),
                        QStringLiteral("Dialog validation must run on the GUI thread."),
                        QStringLiteral("Invoke validation from the thread that owns the dialog.")));
        return result;
    }
    const QString value = directory().trimmed();
    if (value.isEmpty())
    {
        result.add(diag(QStringLiteral("GUI-DIALOG-WORKDIR-EMPTY"),
                        QStringLiteral("The work directory is empty."),
                        QStringLiteral("Choose an absolute directory path."), value));
        return result;
    }
    const QFileInfo info(value);
    if (!info.isAbsolute())
    {
        result.add(diag(QStringLiteral("GUI-DIALOG-WORKDIR-NOT-ABSOLUTE"),
                        QStringLiteral("The work directory must be absolute."),
                        QStringLiteral("Relative paths are not accepted at the dialog boundary."), value));
        return result;
    }
    if (value.contains(QRegularExpression(QStringLiteral("[<>\\\"|?*]"))))
    {
        result.add(diag(QStringLiteral("GUI-DIALOG-WORKDIR-INVALID"),
                        QStringLiteral("The work directory contains invalid characters."),
                        QStringLiteral("Remove Windows filename metacharacters from the path."), value));
        return result;
    }
    if (info.exists())
    {
        if (!info.isDir())
        {
            result.add(diag(QStringLiteral("GUI-DIALOG-WORKDIR-NOT-DIRECTORY"),
                            QStringLiteral("The selected path is not a directory."),
                            QStringLiteral("Select a directory rather than a regular file."), value));
        }
        else if (!info.isWritable())
        {
            result.add(diag(QStringLiteral("GUI-DIALOG-WORKDIR-NOT-WRITABLE"),
                            QStringLiteral("The work directory is not writable."),
                            QStringLiteral("Choose a directory with write permission."), value));
        }
    }
    else if (!createIfMissing())
    {
        result.add(diag(QStringLiteral("GUI-DIALOG-WORKDIR-NOT-FOUND"),
                        QStringLiteral("The work directory does not exist."),
                        QStringLiteral("Enable creation permission for the later application service."), value));
    }
    return result;
}

bool WorkDirectoryDialog::tryAccept()
{
    const auto result = validate();
    if (!result.succeeded()) { showDiagnostics(result); return false; }
    m_accepted = true; m_cancelled = false; QDialog::accept(); return true;
}
bool WorkDirectoryDialog::accepted() const noexcept { return m_accepted; }
bool WorkDirectoryDialog::cancelled() const noexcept { return m_cancelled; }
void WorkDirectoryDialog::reject() { m_cancelled = true; m_accepted = false; QDialog::reject(); }
void WorkDirectoryDialog::chooseDirectory()
{
    const QString chosen = QFileDialog::getExistingDirectory(this, tr("Choose Work Directory"), directory());
    if (!chosen.isEmpty()) setDirectory(chosen);
}
void WorkDirectoryDialog::showDiagnostics(const Common::OperationResult& result)
{
    if (!result.diagnostics.isEmpty()) QMessageBox::warning(this, tr("Invalid input"), result.diagnostics.front().message);
}
}
