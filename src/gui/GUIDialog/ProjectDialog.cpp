#include "ProjectDialog.h"
#include <QApplication>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QThread>
#include <QDir>

namespace AppMesh::Gui
{
namespace { Common::Diagnostic diag(const QString& code, const QString& msg, const QString& detail, const QString& path = {}) { return {QStringLiteral("validation"), code, msg, detail, false, path, QStringLiteral("gui.dialog.project"), QString()}; } }
ProjectDialog::ProjectDialog(ProjectDialogMode mode, const QString& initialPath, QWidget* parent) : QDialog(parent), m_mode(mode)
{
    Q_ASSERT(!qApp || QThread::currentThread()==qApp->thread()); setObjectName(QStringLiteral("projectDialog")); setWindowTitle(mode==ProjectDialogMode::Open ? tr("Open Project") : tr("Save Project"));
    auto* form=new QFormLayout(this); m_pathEdit=new QLineEdit(this); m_pathEdit->setObjectName(QStringLiteral("projectPathEdit")); m_pathEdit->setText(initialPath); auto* browse=new QPushButton(tr("Browse..."),this); browse->setObjectName(QStringLiteral("browseProjectButton")); auto* row=new QWidget(this); auto* rowLayout=new QHBoxLayout(row); rowLayout->addWidget(m_pathEdit); rowLayout->addWidget(browse); form->addRow(tr("Project path"),row);
    m_overwrite=new QCheckBox(tr("Allow overwrite"),this); m_overwrite->setObjectName(QStringLiteral("overwriteCheckBox")); m_overwrite->setVisible(mode==ProjectDialogMode::Save); form->addRow(QString(),m_overwrite); auto* buttons=new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel,this); form->addRow(buttons); connect(browse,&QPushButton::clicked,this,&ProjectDialog::chooseFile); connect(buttons,&QDialogButtonBox::accepted,this,&ProjectDialog::tryAccept); connect(buttons,&QDialogButtonBox::rejected,this,&ProjectDialog::reject);
}
ProjectDialogMode ProjectDialog::mode() const noexcept { return m_mode; }
QString ProjectDialog::projectPath() const { return m_pathEdit->text(); }
void ProjectDialog::setProjectPath(const QString& path) { m_pathEdit->setText(path); }
bool ProjectDialog::overwriteExisting() const { return m_overwrite->isChecked(); }
void ProjectDialog::setOverwriteExisting(bool enabled) { m_overwrite->setChecked(enabled); }
ProjectRequest ProjectDialog::request() const { return {m_mode,QDir::cleanPath(QFileInfo(projectPath().trimmed()).absoluteFilePath()),overwriteExisting()}; }
Common::OperationResult ProjectDialog::validate() const
{
    Common::OperationResult result; if(!qApp||QThread::currentThread()!=qApp->thread()){result.add(diag(QStringLiteral("GUI-DIALOG-THREAD-INVALID"),QStringLiteral("Dialog validation must run on the GUI thread."),QStringLiteral("Invoke validation from the owning GUI thread.")));return result;}
    const QString value=projectPath().trimmed(); if(value.isEmpty()){result.add(diag(QStringLiteral("GUI-DIALOG-PROJECT-PATH-EMPTY"),QStringLiteral("Project path is empty."),QStringLiteral("Choose a project file path."),value));return result;} const QFileInfo info(value);
    if(!info.isAbsolute()){result.add(diag(QStringLiteral("GUI-DIALOG-PROJECT-PATH-NOT-ABSOLUTE"),QStringLiteral("Project path must be absolute."),QStringLiteral("Relative project paths are not accepted."),value));return result;}
    if(m_mode==ProjectDialogMode::Open){if(!info.exists()) result.add(diag(QStringLiteral("GUI-DIALOG-PROJECT-NOT-FOUND"),QStringLiteral("Project file does not exist."),QStringLiteral("Choose an existing project file."),value)); else if(!info.isFile()) result.add(diag(QStringLiteral("GUI-DIALOG-PROJECT-NOT-FILE"),QStringLiteral("Project path is not a regular file."),QStringLiteral("Choose a file rather than a directory."),value));}
    else {if(!info.dir().exists()) result.add(diag(QStringLiteral("GUI-DIALOG-PROJECT-PARENT-NOT-FOUND"),QStringLiteral("Project parent directory does not exist."),QStringLiteral("The later IO service may not create directories in this increment."),info.dir().absolutePath())); else if(info.exists()&&!overwriteExisting()) result.add(diag(QStringLiteral("GUI-DIALOG-PROJECT-OVERWRITE-REQUIRED"),QStringLiteral("The project file already exists."),QStringLiteral("Enable overwrite explicitly before saving."),value));}
    return result;
}
bool ProjectDialog::tryAccept(){const auto result=validate();if(!result.succeeded()){showDiagnostics(result);return false;}m_accepted=true;m_cancelled=false;QDialog::accept();return true;}
bool ProjectDialog::accepted() const noexcept{return m_accepted;} bool ProjectDialog::cancelled() const noexcept{return m_cancelled;} void ProjectDialog::reject(){m_cancelled=true;m_accepted=false;QDialog::reject();}
void ProjectDialog::chooseFile(){const QString chosen=m_mode==ProjectDialogMode::Open?QFileDialog::getOpenFileName(this,tr("Open Project"),projectPath()):QFileDialog::getSaveFileName(this,tr("Save Project"),projectPath());if(!chosen.isEmpty())setProjectPath(chosen);}
void ProjectDialog::showDiagnostics(const Common::OperationResult& result){if(!result.diagnostics.isEmpty())QMessageBox::warning(this,tr("Invalid input"),result.diagnostics.front().message);}
}
