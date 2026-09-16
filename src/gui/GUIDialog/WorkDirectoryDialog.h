#pragma once

#include "DialogTypes.h"

#include <QDialog>

class QCheckBox;
class QLineEdit;

namespace AppMesh::Gui
{
class WorkDirectoryDialog final : public QDialog
{
public:
    explicit WorkDirectoryDialog(const QString& initialDirectory,
                                 QWidget* parent = nullptr);
    ~WorkDirectoryDialog() override = default;
    WorkDirectoryDialog(const WorkDirectoryDialog&) = delete;
    WorkDirectoryDialog& operator=(const WorkDirectoryDialog&) = delete;
    WorkDirectoryDialog(WorkDirectoryDialog&&) = delete;
    WorkDirectoryDialog& operator=(WorkDirectoryDialog&&) = delete;

    QString directory() const;
    void setDirectory(const QString& directory);
    bool createIfMissing() const;
    void setCreateIfMissing(bool enabled);
    WorkDirectoryRequest request() const;
    Common::OperationResult validate() const;
    bool tryAccept();
    bool accepted() const noexcept;
    bool cancelled() const noexcept;

private:
    void reject() override;
    void chooseDirectory();
    void showDiagnostics(const Common::OperationResult& result);
    QLineEdit* m_directoryEdit = nullptr;
    QCheckBox* m_createIfMissing = nullptr;
    bool m_accepted = false;
    bool m_cancelled = false;
};
}
