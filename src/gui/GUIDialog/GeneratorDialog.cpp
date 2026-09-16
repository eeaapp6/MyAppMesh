#include "GeneratorDialog.h"
#include <QApplication>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QHeaderView>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidget>
#include <QThread>
#include <QSet>
#include <QTableWidgetItem>

namespace AppMesh::Gui
{
namespace { Common::Diagnostic diag(const QString& code, const QString& msg, const QString& detail, const QString& path = {}) { return {QStringLiteral("validation"), code, msg, detail, false, path, QStringLiteral("gui.dialog.generator"), QString()}; } }
GeneratorDialog::GeneratorDialog(QWidget* parent) : QDialog(parent)
{
    Q_ASSERT(!qApp || QThread::currentThread() == qApp->thread());
    setObjectName(QStringLiteral("generatorDialog")); setWindowTitle(tr("Mesh Generator"));
    auto* form = new QFormLayout(this);
    m_generatorEdit = new QLineEdit(this); m_generatorEdit->setObjectName(QStringLiteral("generatorIdEdit")); form->addRow(tr("Generator"), m_generatorEdit);
    m_parametersTable = new QTableWidget(0, 2, this); m_parametersTable->setObjectName(QStringLiteral("generatorParametersTable")); m_parametersTable->setHorizontalHeaderLabels({tr("Key"), tr("Value")}); m_parametersTable->horizontalHeader()->setStretchLastSection(true); form->addRow(tr("Parameters"), m_parametersTable);
    auto* add = new QPushButton(tr("Add Parameter"), this); add->setObjectName(QStringLiteral("addParameterButton")); form->addRow(QString(), add);
    auto* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this); form->addRow(buttons);
    connect(add, &QPushButton::clicked, this, &GeneratorDialog::addParameterRow); connect(buttons, &QDialogButtonBox::accepted, this, &GeneratorDialog::tryAccept); connect(buttons, &QDialogButtonBox::rejected, this, &GeneratorDialog::reject);
}
QString GeneratorDialog::generatorId() const { return m_generatorEdit->text(); }
void GeneratorDialog::setGeneratorId(const QString& id) { m_generatorEdit->setText(id); }
QVariantMap GeneratorDialog::parameters() const { QVariantMap values; for (int row=0; row<m_parametersTable->rowCount(); ++row) { auto* key=m_parametersTable->item(row,0); auto* value=m_parametersTable->item(row,1); if (key) values.insert(key->text(), value ? QVariant(value->text()) : QVariant()); } return values; }
void GeneratorDialog::setParameters(const QVariantMap& parameters) { m_parametersTable->setRowCount(0); for (auto it=parameters.cbegin(); it!=parameters.cend(); ++it) { const int row=m_parametersTable->rowCount(); m_parametersTable->insertRow(row); m_parametersTable->setItem(row,0,new QTableWidgetItem(it.key())); m_parametersTable->setItem(row,1,new QTableWidgetItem(it.value().toString())); } }
GeneratorRequest GeneratorDialog::request() const { return {generatorId().trimmed(), parameters()}; }
Common::OperationResult GeneratorDialog::validate() const
{
    Common::OperationResult result;
    if (!qApp || QThread::currentThread()!=qApp->thread()) { result.add(diag(QStringLiteral("GUI-DIALOG-THREAD-INVALID"), QStringLiteral("Dialog validation must run on the GUI thread."), QStringLiteral("Invoke validation from the owning GUI thread."))); return result; }
    if (generatorId().trimmed().isEmpty()) result.add(diag(QStringLiteral("GUI-DIALOG-GENERATOR-ID-EMPTY"), QStringLiteral("Generator ID is empty."), QStringLiteral("Choose a concrete generator identifier."), QStringLiteral("generatorId")));
    QSet<QString> keys;
    for (int row = 0; row < m_parametersTable->rowCount(); ++row)
    {
        const auto* keyItem = m_parametersTable->item(row, 0);
        const QString key = keyItem ? keyItem->text().trimmed() : QString();
        if (key.isEmpty())
            result.add(diag(QStringLiteral("GUI-DIALOG-GENERATOR-PARAMETER-KEY-EMPTY"), QStringLiteral("A generator parameter key is empty."), QStringLiteral("Every parameter requires a non-empty key."), QStringLiteral("parameters/%1").arg(row)));
        else if (keys.contains(key))
            result.add(diag(QStringLiteral("GUI-DIALOG-GENERATOR-PARAMETER-KEY-DUPLICATE"), QStringLiteral("A generator parameter key is duplicated."), QStringLiteral("Parameter keys must be unique."), QStringLiteral("parameters/%1").arg(key)));
        else
            keys.insert(key);
        const auto* value = m_parametersTable->item(row, 1);
        if (value && value->data(Qt::UserRole).isValid())
            result.add(diag(QStringLiteral("GUI-DIALOG-GENERATOR-PARAMETER-VALUE-INVALID"), QStringLiteral("A generator parameter value is not a value type."), QStringLiteral("QObject and QWidget pointers are not accepted."), QStringLiteral("parameters/%1").arg(key)));
    }
    return result;
}
bool GeneratorDialog::tryAccept() { const auto result=validate(); if(!result.succeeded()){showDiagnostics(result); return false;} m_accepted=true; m_cancelled=false; QDialog::accept(); return true; }
bool GeneratorDialog::accepted() const noexcept { return m_accepted; }
bool GeneratorDialog::cancelled() const noexcept { return m_cancelled; }
void GeneratorDialog::reject() { m_cancelled=true; m_accepted=false; QDialog::reject(); }
void GeneratorDialog::addParameterRow() { const int row=m_parametersTable->rowCount(); m_parametersTable->insertRow(row); m_parametersTable->setItem(row,0,new QTableWidgetItem); m_parametersTable->setItem(row,1,new QTableWidgetItem); }
void GeneratorDialog::showDiagnostics(const Common::OperationResult& result) { if(!result.diagnostics.isEmpty()) QMessageBox::warning(this,tr("Invalid input"),result.diagnostics.front().message); }
}
