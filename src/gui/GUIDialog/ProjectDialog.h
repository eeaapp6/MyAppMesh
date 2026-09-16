#pragma once
#include "DialogTypes.h"
#include <QDialog>
class QCheckBox;
class QLineEdit;
namespace AppMesh::Gui
{
class ProjectDialog final : public QDialog
{
public:
    explicit ProjectDialog(ProjectDialogMode mode, const QString& initialPath = {}, QWidget* parent = nullptr);
    ~ProjectDialog() override = default;
    ProjectDialog(const ProjectDialog&) = delete;
    ProjectDialog& operator=(const ProjectDialog&) = delete;
    ProjectDialog(ProjectDialog&&) = delete;
    ProjectDialog& operator=(ProjectDialog&&) = delete;
    ProjectDialogMode mode() const noexcept;
    QString projectPath() const;
    void setProjectPath(const QString& path);
    bool overwriteExisting() const;
    void setOverwriteExisting(bool enabled);
    ProjectRequest request() const;
    Common::OperationResult validate() const;
    bool tryAccept();
    bool accepted() const noexcept;
    bool cancelled() const noexcept;
private:
    void reject() override;
    void chooseFile();
    void showDiagnostics(const Common::OperationResult& result);
    ProjectDialogMode m_mode;
    QLineEdit* m_pathEdit = nullptr;
    QCheckBox* m_overwrite = nullptr;
    bool m_accepted = false;
    bool m_cancelled = false;
};
}
