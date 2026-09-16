#pragma once
#include "DialogTypes.h"
#include <QDialog>
class QLineEdit;
class QTableWidget;
namespace AppMesh::Gui
{
class GeneratorDialog final : public QDialog
{
public:
    explicit GeneratorDialog(QWidget* parent = nullptr);
    ~GeneratorDialog() override = default;
    GeneratorDialog(const GeneratorDialog&) = delete;
    GeneratorDialog& operator=(const GeneratorDialog&) = delete;
    GeneratorDialog(GeneratorDialog&&) = delete;
    GeneratorDialog& operator=(GeneratorDialog&&) = delete;
    QString generatorId() const;
    void setGeneratorId(const QString& id);
    QVariantMap parameters() const;
    void setParameters(const QVariantMap& parameters);
    GeneratorRequest request() const;
    Common::OperationResult validate() const;
    bool tryAccept();
    bool accepted() const noexcept;
    bool cancelled() const noexcept;
private:
    void reject() override;
    void addParameterRow();
    void showDiagnostics(const Common::OperationResult& result);
    QLineEdit* m_generatorEdit = nullptr;
    QTableWidget* m_parametersTable = nullptr;
    bool m_accepted = false;
    bool m_cancelled = false;
};
}
