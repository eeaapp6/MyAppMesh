/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperatorProperty.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKPropSolid.h"
#include "GUIDialog/GUIAnalysisDialog/GUIPropertySolidDialog.h"
#include "GUIDialog/GUIAnalysisDialog/GUIPropertyShellDialog.h"
#include "GUIDialog/GUIAnalysisDialog/GUIPropertyBeamDialog.h"
#include "GUIDialog/GUIAnalysisDialog/GUIPropertyBeamSPRDialog.h"
#include "GUIDialog/GUIAnalysisDialog/GUIPropertySPHDialog.h"
#include "GUIDialog/GUIAnalysisDialog/GUIPropertyTypeDialog.h"
#include "GUIDialog/GUIAnalysisDialog/GUIPropertySandwichDialog.h"
#include "GUIDialog/GUIToolDialog/GUIRenameDialog.h"
#include "GUIFrame/MainWindow.h"
#include <QTableWidget>
#include <QComboBox>
#include <QCheckBox>
#include <QTreeWidgetItem>
#include "OperatorsInterface/TreeEventOperator.h"
#include <QLineEdit>

namespace OperModel
{
    bool OperatorProperty::execGUI()
    {
        if (_emitter == nullptr)
            return false;

        QString name = _emitter->objectName();
        if (name.isEmpty()) return false;

        // 获取主窗口
        GUI::MainWindow* mainWindow = FITKAPP->getGlobalData()->getMainWindowT<GUI::MainWindow>();
        if (!mainWindow) return false;

        if (name == "actionPropertyCreate")
        {
            int type = -1;
            if (!this->argValue("PropType", type))return false;

            // 创建属性对话框
            if (type== Radioss::FITKPropAbstract::RadPropType::Solid) {
                GUI::GUIPropertySolidDialog* dialog = new GUI::GUIPropertySolidDialog(this, nullptr, mainWindow);
                dialog->show();
            }
            else if(type == Radioss::FITKPropAbstract::RadPropType::Shell){
                GUI::GUIPropertyShellDialog* dialog = new GUI::GUIPropertyShellDialog(this, nullptr, mainWindow);
                dialog->show();
            }
            else if (type == Radioss::FITKPropAbstract::RadPropType::Beam) {
                GUI::GUIPropertyBeamDialog* dialog = new GUI::GUIPropertyBeamDialog(this, nullptr, mainWindow);
                dialog->show();
            }
            else if (type == Radioss::FITKPropAbstract::RadPropType::SPH) {
                GUI::GUIPropertySPHDialog* dialog = new GUI::GUIPropertySPHDialog(this, nullptr, mainWindow);
                dialog->show();
            }
            else if (type == Radioss::FITKPropAbstract::RadPropType::Sandwich) {
                GUI::GUIPropertySandwichDialog* dialog = new GUI::GUIPropertySandwichDialog(this, nullptr, mainWindow);
                dialog->show();
            }
            else if (type == Radioss::FITKPropAbstract::RadPropType::BeamSPR) {
                GUI::GUIPropertyBeamSPRDialog* dialog = new GUI::GUIPropertyBeamSPRDialog(this, nullptr, mainWindow);
                dialog->show();
            }
        }
        else if (name == "actionPropertyRename")
        {
            return this->propertyRenameOper();
        }
        else if (name == "actionPropertyEdit")
        {
            // 获取选中的属性
            Radioss::FITKPropAbstract* selectedProperty = getSelectedProperty();
            if (selectedProperty == nullptr) return false;

            int type = -1;
            if (!this->argValue("PropType", type))return false;

            // 创建编辑对话框
            if (type == Radioss::FITKPropAbstract::RadPropType::Solid) {
                GUI::GUIPropertySolidDialog* dialog = new GUI::GUIPropertySolidDialog(this, selectedProperty, mainWindow);
                dialog->show();
            }
            else if(type == Radioss::FITKPropAbstract::RadPropType::Shell){
                GUI::GUIPropertyShellDialog* dialog = new GUI::GUIPropertyShellDialog(this, selectedProperty, mainWindow);
                dialog->show();
            }
            else if (type == Radioss::FITKPropAbstract::RadPropType::Beam) {
                GUI::GUIPropertyBeamDialog* dialog = new GUI::GUIPropertyBeamDialog(this, selectedProperty, mainWindow);
                dialog->show();
            }
            else if (type == Radioss::FITKPropAbstract::RadPropType::SPH) {
                GUI::GUIPropertySPHDialog* dialog = new GUI::GUIPropertySPHDialog(this, selectedProperty, mainWindow);
                dialog->show();
            }
            else if (type == Radioss::FITKPropAbstract::RadPropType::Sandwich) {
                GUI::GUIPropertySandwichDialog* dialog = new GUI::GUIPropertySandwichDialog(this, selectedProperty, mainWindow);
                dialog->show();
            }
            else if (type == Radioss::FITKPropAbstract::RadPropType::BeamSPR) {
                GUI::GUIPropertyBeamSPRDialog* dialog = new GUI::GUIPropertyBeamSPRDialog(this, selectedProperty, mainWindow);
                dialog->show();
            }
        }
        else if (name == "actionPropertyDelete")
        {
            return deleteProperty();
        }

        return true;
    }

    bool OperatorProperty::execProfession()
    {
        QString actionName;
        this->argValue("ActionName", actionName);
        if (actionName.isEmpty()) return false;
        int propType = -1;
        this->argValue("PropType", propType);

        // 界面执行结果处理（仅对创建和编辑有效）
        if (actionName == "actionPropertyCreate" || actionName == "actionPropertyEdit")
        {
            if (propType== Radioss::FITKPropAbstract::RadPropType::Solid) {
                bool result = false;
                this->argValue("AnalysisPropertyResult", result);
                if (!result) return false;
                result = dataProcessing();
            }
        }
        this->setArgs("PropType", -1);//重置
        // 更新界面
        updateTree();

        return true;
    }

    void OperatorProperty::preArgs()
    {
        // 当点击事件触发时，预先进行数据处理
        if (_emitter == nullptr) return;

        // 保存操作名称，供后续使用
        QString name = _emitter->objectName();
        if (name.isEmpty()) return;
        this->setArgs("ActionName", QVariant::fromValue(name));

        int type = -1;
        if (name == "actionPropertyCreate")
        {
            GUI::GUIPropertyTypeDialog dlg;
            if (dlg.exec() != QDialog::Accepted)return;
            type = dlg.getPropType();
            this->setArgs("AnalysisPropertyName", dlg.getName());
        }
        if (name == "actionPropertyEdit")
        {
            _data = getSelectedProperty();
            if (!_data)return;
            type = _data->getRadPropType();
        }
        this->setArgs("PropType", type);

        // 数据加载
        if (type== Radioss::FITKPropAbstract::RadPropType::Solid)
            dataLoading();
        return;
    }

    void OperatorProperty::dataLoading()
    {
        QString actionName = _emitter->objectName();

        if (actionName == "actionPropertyCreate")
        {
            // 创建新属性
            _data = new Radioss::FITKPropSolid();
        }
        else if (actionName == "actionPropertyEdit")
        {
            // 编辑现有属性
            _data = getSelectedProperty();
            if (_data == nullptr) return;
        }
        else
        {
            // 删除操作不需要加载数据
            return;
        }

        Radioss::FITKPropSolid* data = dynamic_cast<Radioss::FITKPropSolid*>(_data);
        if (data == nullptr) return;

        // 获取属性值
        Radioss::FITKPropSolidValue* value = data->getValue();
        if (value == nullptr) return;

        QList<double> list;
        //this->setArgs("AnalysisPropertyName", QVariant::fromValue(data->getDataObjectName()));

        // 1 Solid_Element_Formulation（部分缺失）
        int flagSolid = value->getIsolid();
        if (flagSolid == 0) list.append(flagSolid);
        else if (flagSolid == 1) list.append(flagSolid);
        else if (flagSolid == 2) list.append(flagSolid);
        else if (flagSolid == 3) list.append(0);  // 缺失
        else if (flagSolid == 14) list.append(4);
        else if (flagSolid == 16) list.append(5);
        else if (flagSolid == 17) list.append(6);
        else if (flagSolid == 24) list.append(7);
        else if (flagSolid == 18) list.append(8);
        else list.append(0);

        // 2 Strain_Formulation（部分一致）
        int flagSmstr = value->getIsmstr();
        if (flagSmstr == 0) list.append(flagSmstr);
        else if (flagSmstr == 1) list.append(flagSmstr);
        else if (flagSmstr == 2) list.append(flagSmstr);
        else if (flagSmstr == 3) list.append(flagSmstr);
        else if (flagSmstr == 4) list.append(flagSmstr);
        else if (flagSmstr == 10) list.append(5);
        else if (flagSmstr == 11) list.append(6);
        else if (flagSmstr == 12) list.append(7);
        else list.append(0);

        // 3 Constant_Pressure（顺序一致）
        int flagCpre = value->getIcpre();
        if (flagCpre == 0) list.append(flagCpre);
        else if (flagCpre == 1) list.append(flagCpre);
        else if (flagCpre == 2) list.append(flagCpre);
        else if (flagCpre == 3) list.append(flagCpre);
        else list.append(0.0);

        // 4 Tetra10_Formulation（部分一致）
        int flagTetra10 = value->getItetra10();
        if (flagTetra10 == 0) list.append(flagTetra10);
        else if (flagTetra10 == 2) list.append(1);
        else list.append(0.0);

        // 5 No_Of_Integration_Points
        list.append(0.0);

        // 6 Tetra4_Formulation（部分一致）
        int flagTetra4 = value->getItetra4();
        if (flagTetra4 == 0) list.append(flagTetra4);
        else if (flagTetra4 == 1) list.append(flagTetra4);
        else if (flagTetra4 == 3) list.append(2);
        else if (flagTetra4 == 1000) list.append(3);
        else list.append(0.0);

        // 7 Element_Coordinate（顺序一致）
        list.append(value->getIFrame());
        // 8 Numerical_Damping
        list.append(data->getDn());
        // 9 Navier_Stokes_Viscosity_mu
        list.append(data->getMuV());
        // 10 Minimum_Time_Step
        list.append(data->getMinTimeStep());
        // 11 Compute_Strain
        list.append(0.0);
        // 12 Hourglass_Tangent
        list.append(0.0);
        // 13 No_Of_Particle
        list.append(data->getNdir() - 1);
        // 14 Integration_type
        list.append(0.0);

        this->setArgs("AnalysisPropertyData", QVariant::fromValue(list));
    }

    bool OperatorProperty::dataProcessing()
    {
        // 获取表格指针
        this->argValue<QTableWidget*>("AnalysisPropertyTableWidget", _tableWidget);
        if (_tableWidget == nullptr) return false;

        // 从参数中获取操作名称
        QString actionName;
        this->argValue("ActionName", actionName);
        if (actionName.isEmpty()) return false;
        bool isCreate = (actionName == "actionPropertyCreate");

        if (isCreate && _data == nullptr)
        {
            _data = new Radioss::FITKPropSolid();
        }

        // 获取控件数据
        // 1 Solid_Element_Formulation
        int solid = getCurrentIndex(1, 1);
        // 2 Strain_Formulation
        int smstr = getCurrentIndex(2, 1);
        // 3 Constant_Pressure
        int cpre = getCurrentIndex(3, 1);
        // 4 Tetra10_Formulation
        int tetra10 = getCurrentIndex(4, 1);
        // 5 No_Of_Integration_Points
        //getDoubleValue(5, 1);
        // 6 Tetra4_Formulation
        int tetra4 = getCurrentIndex(6, 1);
        // 7 Element_Coordinate
        int frame = getCurrentIndex(7, 1);
        // 8 Numerical_Damping
        double dn = getDoubleValue(8, 1);
        // 9 Navier_Stokes_Viscosity_mu
        double mV = getDoubleValue(9, 1);
        // 10 Minimum_Time_Step
        double minTimeStep = getDoubleValue(10, 1);
        // 11 Compute_Strain
        //getCurrentIndex(11, 1);
        // 12 Hourglass_Tangent
        //getCurrentIndex(12, 1);
        // 13 No_Of_Particle
        int nDir = getCurrentIndex(13, 1);
        // 14 Integration_type
        //getCurrentIndex(14, 1);

        // 获取属性管理器
        Interface::FITKAbstractSectionManager* manager = getPropertyManager();
        if (manager == nullptr) return false;

        // 获取实体属性数据
        Radioss::FITKPropSolid* data = dynamic_cast<Radioss::FITKPropSolid*>(_data);
        if (data == nullptr) return false;

        // 获取属性值
        Radioss::FITKPropSolidValue* value = data->getValue();
        if (value == nullptr) return false;

        // 属性名称
        QString name;
        this->argValue("AnalysisPropertyName", name);
        if (isCreate)
        {
            data->setDataObjectName(manager->checkName(name));
        }
        else
        {
            data->setDataObjectName(name);
        }

        // 设置属性值
        // 1 Solid_Element_Formulation（部分缺失）
        if (solid == 3) value->setIsolid(0);  // 缺失
        else if (solid == 4) value->setIsolid(14);
        else if (solid == 5) value->setIsolid(16);
        else if (solid == 6) value->setIsolid(17);
        else if (solid == 7) value->setIsolid(24);
        else if (solid == 8) value->setIsolid(18);
        else value->setIsolid(solid);

        // 2 Strain_Formulation（部分一致）
        if (smstr == 5) value->setIsmstr(10);
        else if (smstr == 6) value->setIsmstr(11);
        else if (smstr == 7) value->setIsmstr(12);
        else value->setIsmstr(smstr);

        // 3 Constant_Pressure（顺序一致）
        value->setIcpre(cpre);
        // 4 Tetra10_Formulation（部分一致）
        if (tetra10 == 1) value->setItetra10(2);
        else value->setItetra10(tetra10);
        // 6 Tetra4_Formulation（部分一致）
        if (tetra4 == 2) tetra4 = 3;
        else if (tetra4 == 3) tetra4 = 1000;
        value->setItetra4(tetra4);
        // 7 Element_Coordinate（顺序一致）
        value->setIFrame(frame);
        // 8 Numerical_Damping
        data->setDn(dn);
        // 9 Navier_Stokes_Viscosity_mu
        data->setMuV(mV);
        // 10 Minimum_Time_Step
        data->setMinTimeStep(minTimeStep);
        // 13 No_Of_Particle
        data->setNdir(nDir + 1);

        // 添加数据（仅创建时）
        if (isCreate)
        {
            manager->appendDataObj(data);
        }

        // 重置数据
        if (isCreate)
        {
            _data = nullptr;  // 创建时释放临时数据指针
        }
        _tableWidget = nullptr;
        this->setArgs("AnalysisPropertyResult", QVariant::fromValue(false));

        return true;
    }

    Radioss::FITKPropAbstract* OperatorProperty::getSelectedProperty()
    {
        // 获取属性ID
        int propertyID = -1;
        this->argValue("SelectedItemDataID", propertyID);
        if (propertyID < 0) return nullptr;

        // 获取属性管理器
        Interface::FITKAbstractSectionManager* manager = getPropertyManager();
        if (!manager) return nullptr;

        // 获取属性
        Interface::FITKAbstractSection* data = manager->getDataByID(propertyID);
        if (data == nullptr) return nullptr;

        return dynamic_cast<Radioss::FITKPropAbstract*>(data);
    }

    Interface::FITKAbstractSectionManager* OperatorProperty::getPropertyManager()
    {
        // 获取算例数据
        Radioss::FITKRadiossCase* dataCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (dataCase == nullptr) return nullptr;

        // 获取属性管理器
        Interface::FITKAbstractSectionManager* managerSection = dataCase->getSectionManager();
        return managerSection == nullptr ? nullptr : managerSection;
    }

    bool OperatorProperty::deleteProperty()
    {
        // 获取选中的属性
        Radioss::FITKPropAbstract* selectedProperty = getSelectedProperty();
        if (selectedProperty == nullptr) return false;

        // 获取属性管理器
        Interface::FITKAbstractSectionManager* manager = getPropertyManager();
        if (manager == nullptr) return false;

        // 从管理器中移除属性
        manager->removeDataObj(selectedProperty);

        // 更新界面
        updateTree();

        return true;
    }

    int OperatorProperty::getCurrentIndex(int row, int column)
    {
        // 检查参数
        if (_tableWidget == nullptr || row >= _tableWidget->rowCount() || row < 1 || column < 1 || column >= _tableWidget->columnCount()) return -1;

        // 获取数据
        QComboBox* comboBox = dynamic_cast<QComboBox*>(_tableWidget->cellWidget(row, column));
        return comboBox == nullptr ? -1 : comboBox->currentIndex();
    }

    bool OperatorProperty::propertyRenameOper()
    {
        // 获取属性ID
        int propertyID = 0;
        if (!this->argValue<int>("PropID", propertyID)) return false;
        // 获取 Radioss Case
        Radioss::FITKRadiossCase* caseData = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (caseData == nullptr) return false;
        // 获取材料管理器
        Interface::FITKAbstractSectionManager* manager = caseData->getSectionManager();
        if (manager == nullptr) return false;
        Interface::FITKAbstractSection* propData = manager->getDataByID(propertyID);
        if (!propData) return false;
        GUI::GUIRenameDialog renameDialog(this, propData, manager, FITKAPP->getGlobalData()->getMainWindow());
        renameDialog.exec();
        return true;
    }

    double OperatorProperty::getDoubleValue(int row, int column)
    {
        // 检查参数
        if (_tableWidget == nullptr || row >= _tableWidget->rowCount() || row < 1 || column < 1 || column >= _tableWidget->columnCount()) return 0.0;

        // 获取文本框
        QLineEdit* line = dynamic_cast<QLineEdit*>(_tableWidget->cellWidget(row, column));
        if (line == nullptr) return 0.0;
        
        // 获取数据
        QString text = line->text();
        if (text.isEmpty()) return 0.0;

        bool ok = true;
        double value = text.toDouble(&ok);
        if (!ok) return 0.0;

        return value;
    }

    bool OperatorProperty::getIsChecked(int row, int column)
    {
        // 检查参数
        if (_tableWidget == nullptr || row >= _tableWidget->rowCount() || row < 1 || column < 1 || column >= _tableWidget->columnCount()) return false;

        // 获取数据
        QCheckBox* checkBox = dynamic_cast<QCheckBox*>(_tableWidget->cellWidget(row, column));
        return checkBox == nullptr ? false : checkBox->isChecked();
    }

    void OperatorProperty::updateTree()
    {
        // 通过树形菜单事件处理器刷新树
        QStringList listOpertor = { "TreeAssemblyEvent", "TreeGroupEvent" ,"TreePropertyEvent","TreeSolutionEvent" };
        for (int i = 0; i < listOpertor.size(); i++)
        {
            EventOper::TreeEventOperator* oper = dynamic_cast<EventOper::TreeEventOperator*>(Core::FITKOperatorRepo::getInstance()->getOperator(listOpertor[i]));
            if (oper != nullptr) oper->updateTree();
        }
    }
}