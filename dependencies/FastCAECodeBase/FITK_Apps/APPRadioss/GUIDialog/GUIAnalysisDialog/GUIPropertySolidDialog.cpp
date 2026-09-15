/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUIPropertySolidDialog.h"
#include "ui_GUIPropertyDialog.h"
#include "FITK_Interface/FITKInterfacePhysics/FITKAbstractMaterial.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Component/FITKRadiossData/FITKPropSolid.h"
#include "FITK_Interface/FITKInterfacePhysics/FITKAbstractSection.h"
#include <QTableWidget>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QRegularExpression>
#include "FITK_Component/FITKWidget/FITKSciNotationLineEdit.h"

namespace GUI
{
    GUIPropertySolidDialog::GUIPropertySolidDialog(Core::FITKActionOperator* oper, Radioss::FITKPropAbstract* property, QWidget* parent)
        : _oper(oper), _ui(new Ui::GUIPropertyDialog), Core::FITKDialog(parent), _editProperty(property)
    {
        _ui->setupUi(this);
        //去掉问号
        setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
        // 设置对话框关闭时自动删除
        setAttribute(Qt::WA_DeleteOnClose);
        // 设置对话框标题
        setWindowTitle(tr("Analysis Property"));
        if (_editProperty)  _isEditMode = true;
        // 初始化GUI
        initGUI();
    }

    GUIPropertySolidDialog::~GUIPropertySolidDialog()
    {
        // 释放UI
        if (_ui) delete _ui;
        _ui = nullptr;
    }

    void GUIPropertySolidDialog::on_pushButtonOK_clicked()
    {
        if (_isEditMode && _editProperty != nullptr)
        {
            _oldNameForScript = _editProperty->getDataObjectName();
        }
        // 运行结果
        _oper->setArgs("AnalysisPropertyResult", QVariant::fromValue(true));
        // 属性名称
        _oper->setArgs("AnalysisPropertyName", QVariant::fromValue(_ui->lineEditName->text()));
        // 表单数据
        _oper->setArgs("AnalysisPropertyTableWidget", qVariantFromValue(_ui->tableWidgetValue));
        // 处理业务逻辑
        _oper->execProfession();
        // 关闭对话框

        writePythonScript();
        accept();
    }

    void GUIPropertySolidDialog::initGUI()
    {
        if (!_oper)return;
        // 获取算例数据
        Radioss::FITKRadiossCase* dataCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (dataCase == nullptr) return;
        // 获取材料管理器
        Interface::FITKMaterialManager* managerMaterial = dataCase->getMaterialManager();
        if (managerMaterial == nullptr) return;

        // 获取材料名称列表
        QStringList listMaterial;
        for (int i = 0; i < managerMaterial->getDataCount(); i++) listMaterial.append(managerMaterial->getDataByIndex(i)->getDataObjectName());
        listMaterial.append("None");

        // Name - 根据模式设置不同的名称
        QString name;
        if (_isEditMode && _editProperty != nullptr)
        {
            // 编辑模式：使用现有属性的名称
            name = _editProperty->getDataObjectName();
            _ui->lineEditName->setText(name);
        }
        else
        {
            // 创建模式：使用操作器提供的名称或默认名称
            _oper->argValue("AnalysisPropertyName", name);
            _ui->lineEditName->setText(name);
            _ui->lineEditName->setEnabled(false);
        }
        _ui->lineEditPropertyID->setReadOnly(true);
        _ui->lineEditPropertyID->setText("/PROP/TYPE14");


        // 连接信号槽
        connect(_ui->pushButtonCancel, SIGNAL(clicked()), this, SLOT(close()));

        // 更新列表
        _ui->lineEditType->setText("SOLID");
        updateDropSolid();

    }

    void GUIPropertySolidDialog::initTableWidget(int rowCount, int columnCount, QStringList listFirst)
    {
        // 清空
        _ui->tableWidgetValue->clear();
        // 设置行数
        _ui->tableWidgetValue->setRowCount(rowCount);
        // 设置列数
        _ui->tableWidgetValue->setColumnCount(columnCount);
        // 设置其余列显示方式
        for (int i = 0; i < columnCount - 1; ++i) _ui->tableWidgetValue->horizontalHeader()->setSectionResizeMode(i, QHeaderView::ResizeToContents);
        // 设置最后一列显示方式
        _ui->tableWidgetValue->horizontalHeader()->setSectionResizeMode(columnCount - 1, QHeaderView::Stretch);
        // 设置行高调节方式
        _ui->tableWidgetValue->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

        // 初始化表单
        for (int row = 0; row < rowCount; ++row)
        {
            for (int column = 0; column < columnCount; ++column)
            {
                QTableWidgetItem* item = new QTableWidgetItem();
                item->setFlags(Qt::NoItemFlags);
                if (column == 0 && listFirst.size() > row) item->setText(listFirst[row]);
                if (column == 1 && row == 0) item->setText("Value");
                _ui->tableWidgetValue->setItem(row, column, item);
            }
        }
    }

    void GUIPropertySolidDialog::updateDropSolid()
    {
        // 初始化表格
        initTableWidget(15, 2, QStringList() << QObject::tr("Name") << QObject::tr("Solid_Element_Formulation") << QObject::tr("Strain_Formulation") << QObject::tr("Constant_Pressure") << QObject::tr("Tetra10_Formulation") << QObject::tr("No_Of_Integration_Points") << QObject::tr("Tetra4_Formulation") << QObject::tr("Element_Coordinate") << QObject::tr("Numerical_Damping") << QObject::tr("Navier_Stokes_Viscosity_mu") << QObject::tr("Minimum_Time_Step") << QObject::tr("Compute_Strain") << QObject::tr("Hourglass_Tangent") << QObject::tr("No_Of_Particle") << QObject::tr("Integration_type"));

        // 数据加载
        QList<double> list;
        _oper->argValue("AnalysisPropertyData", list);
        if (list.size() < 14) 
            for (int i = list.size(); i < 14; i++) 
                list.append(0.0);

        // 1 Solid_Element_Formulation
        _ui->tableWidgetValue->setCellWidget(1, 1, createComboBox(QStringList() << QObject::tr("Default") << QObject::tr("Hex8-Belytschko") << QObject::tr("Hex8-Hallquist") << QObject::tr("Hex8-No hourglass") << QObject::tr("HA8") << QObject::tr("Hex20") << QObject::tr("Hex8-H8C") << QObject::tr("Hex8-HEPH") << QObject::tr("Hex8-Lock free"), list[0]));
        // 2 Strain_Formulation
        _ui->tableWidgetValue->setCellWidget(2, 1, createComboBox(QStringList() << QObject::tr("Default") << QObject::tr("Small Strain t=0") << QObject::tr("Full geo") << QObject::tr("Simplified small strain") << QObject::tr("Full geom nonlinearities") << QObject::tr("Lagrange total strain") << QObject::tr("Total small strain") << QObject::tr("Lagrange with small strain formulation"), list[1]));
        // 3 Constant_Pressure
        _ui->tableWidgetValue->setCellWidget(3, 1, createComboBox(QStringList() << QObject::tr("Default value") << QObject::tr("Reduced pressure") << QObject::tr("Variable state") << QObject::tr("No reduce pressure"), list[2]));
        // 4 Tetra10_Formulation
        _ui->tableWidgetValue->setCellWidget(4, 1, createComboBox(QStringList() << QObject::tr("Default") << QObject::tr("Quadratic with 4 points"), list[3]));
        // 5 No_Of_Integration_Points
        _ui->tableWidgetValue->setCellWidget(5, 1, createLineEdit("", 1));
        // 6 Tetra4_Formulation
        _ui->tableWidgetValue->setCellWidget(6, 1, createComboBox(QStringList() << QObject::tr("Default") << QObject::tr("Quadratic with 4 points") << QObject::tr("Linear nodal pressure") << QObject::tr("Linear with 1 point"), list[5]));
        // 7 Element_Coordinate - 修复索引错误，应该是行7列1
        _ui->tableWidgetValue->setCellWidget(7, 1, createComboBox(QStringList() << QObject::tr("Default") << QObject::tr("Non co-rotational") << QObject::tr("Co-rotational"), list[6]));
        // 8 Numerical_Damping
        list[7] == 0.0 ? _ui->tableWidgetValue->setCellWidget(8, 1, createLineEdit("0.0")) : _ui->tableWidgetValue->setCellWidget(8, 1, createLineEdit(QString::number(list[7])));
        // 9 Navier_Stokes_Viscosity_mu
        list[8] == 0.0 ? _ui->tableWidgetValue->setCellWidget(9, 1, createLineEdit("0.0")) : _ui->tableWidgetValue->setCellWidget(9, 1, createLineEdit(QString::number(list[8])));
        // 10 Minimum_Time_Step
        list[9] == 0.0 ? _ui->tableWidgetValue->setCellWidget(10, 1, createLineEdit("0.0")) : _ui->tableWidgetValue->setCellWidget(10, 1, createLineEdit(QString::number(list[9])));
        // 11 Compute_Strain
        _ui->tableWidgetValue->setCellWidget(11, 1, createComboBox(QStringList() << QObject::tr("Default") << QObject::tr("Yes") << QObject::tr("No"), list[10]));
        // 12 Hourglass_Tangent
        _ui->tableWidgetValue->setCellWidget(12, 1, createComboBox(QStringList() << QObject::tr("Default") << QObject::tr("Numerical tangent") << QObject::tr("Advanced tangent"), list[11]));
        // 13 No_Of_Particle
        _ui->tableWidgetValue->setCellWidget(13, 1, createComboBox(QStringList() << QObject::tr("1 particle") << QObject::tr("2 particles") << QObject::tr("3 Particles"), list[12]));
        // 14 Integration_type
        _ui->tableWidgetValue->setCellWidget(14, 1, createComboBox(QStringList() << QObject::tr("Default") << QObject::tr("FULL") << QObject::tr("MODPLAST") << QObject::tr("REDPLAST") << QObject::tr("INTO"), list[13]));
    }

    void GUIPropertySolidDialog::writePythonScript()
    {
        auto getComboIndex = [this](int row) -> int
        {
            QComboBox* comboBox = dynamic_cast<QComboBox*>(_ui->tableWidgetValue->cellWidget(row, 1));
            return comboBox == nullptr ? -1 : comboBox->currentIndex();
        };

        auto getLineValue = [this](int row) -> double
        {
            QLineEdit* lineEdit = dynamic_cast<QLineEdit*>(_ui->tableWidgetValue->cellWidget(row, 1));
            return lineEdit == nullptr ? 0.0 : lineEdit->text().toDouble();
        };

        QString propertyName = _ui->lineEditName->text();
        if (propertyName.isEmpty())
        {
            return;
        }

        int solid = getComboIndex(1);
        int smstr = getComboIndex(2);
        int cpre = getComboIndex(3);
        int tetra10 = getComboIndex(4);
        int tetra4 = getComboIndex(6);
        int frame = getComboIndex(7);
        double dn = getLineValue(8);
        double muV = getLineValue(9);
        double minTimeStep = getLineValue(10);
        int nDir = getComboIndex(13);

        if (solid == 3)
        {
            solid = 0;
        }
        else if (solid == 4)
        {
            solid = 14;
        }
        else if (solid == 5)
        {
            solid = 16;
        }
        else if (solid == 6)
        {
            solid = 17;
        }
        else if (solid == 7)
        {
            solid = 24;
        }
        else if (solid == 8)
        {
            solid = 18;
        }

        if (smstr == 5)
        {
            smstr = 10;
        }
        else if (smstr == 6)
        {
            smstr = 11;
        }
        else if (smstr == 7)
        {
            smstr = 12;
        }

        if (tetra10 == 1)
        {
            tetra10 = 2;
        }

        if (tetra4 == 2)
        {
            tetra4 = 3;
        }
        else if (tetra4 == 3)
        {
            tetra4 = 1000;
        }

        QStringList script;
        if (_isEditMode)
        {
            QString oldName = _oldNameForScript.isEmpty() ? propertyName : _oldNameForScript;
            script.append(QString("p=Property.Solid.GetSolid('%1')").arg(oldName));
            if (oldName != propertyName)
            {
                script.append(QString("p.setName('%1')").arg(propertyName));
            }
        }
        else
        {
            script.append("p=Property.Solid()");
            script.append(QString("p.createProperty('%1')").arg(propertyName));
        }

        script.append(QString("p.setIsolid(%1)").arg(solid));
        script.append(QString("p.setIsmstr(%1)").arg(smstr));
        script.append(QString("p.setIcpre(%1)").arg(cpre));
        script.append(QString("p.setItetra10(%1)").arg(tetra10));
        script.append(QString("p.setItetra4(%1)").arg(tetra4));
        script.append(QString("p.setIFrame(%1)").arg(frame));
        script.append(QString("p.setDn(%1)").arg(dn));
        script.append(QString("p.setMuV(%1)").arg(muV));
        script.append(QString("p.setMinTimeStep(%1)").arg(minTimeStep));
        script.append(QString("p.setNdir(%1)").arg(nDir + 1));

        this->saveScript(script);
    }

    void GUIPropertySolidDialog::changeComboBoxStyle(QComboBox* comboBox)
    {
        comboBox->setFixedHeight(20);
        // 设置下拉框样式表
        comboBox->setStyleSheet("QComboBox {"
            // 背景颜色
            "background-color: white;"
            // 边框颜色和宽度
            "border: 0px solid gray;"
            // 选中项的背景颜色
            "selection-background-color: white;"
            // 选中项的文字颜色
            "selection-color: black;"
            "}"
        );
    }

    QComboBox* GUIPropertySolidDialog::createComboBox(QStringList list, int currentIndex, bool isEnable)
    {
        // 创建组合框
        QComboBox* comboBox = new QComboBox();
        // 改变组合框样式
        changeComboBoxStyle(comboBox);
        // 添加组合框下拉项
        comboBox->addItems(list);
        // 设置组合框当前选项
        comboBox->setCurrentIndex(currentIndex);
        // 设置组合框禁用状态
        comboBox->setEnabled(isEnable);
        // 返回组合框
        return comboBox;
    }

    Comp::FITKSciNotationLineEdit* GUIPropertySolidDialog::createLineEdit(QString text, int model, bool isEnable)
    {
        // 创建输入框
        Comp::FITKSciNotationLineEdit* lineEdit = new Comp::FITKSciNotationLineEdit(this);
        if (lineEdit == nullptr) return nullptr;

        // 设置文本
        lineEdit->setText(text);

        // 初始化正则表达式
        QString strRegularExpression = "";

        // 实数（匹配数字或科学计数法）
        if (model == 0) strRegularExpression = R"(^[+-]?(\d+(\.\d*)?|\.\d+)([eE][+-]?\d+)?$)";
        // 整数
        else if (model == 1) strRegularExpression = R"(^(?:[1-9]\d*|0)$)";
        
        // 创建正则表达式
        QRegularExpression regex(strRegularExpression);
        // 创建正则表达式验证器
        QRegularExpressionValidator* validator = new QRegularExpressionValidator(regex, lineEdit);
        if (validator == nullptr) return nullptr;
        // 设置正则表达式验证器
        lineEdit->setValidator(validator);
        // 设置禁用状态
        lineEdit->setEnabled(isEnable);

        return lineEdit;
    }
}