/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "TreeWidgetProperty.h"
#include "GUIWidget/GUIEnumType.h"
#include "FITK_Kernel/FITKCore/FITKTreeWidgetOperator.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKMaterialElastic.h"
#include "FITK_Component/FITKRadiossData/FITKMaterialPlasJohns.h"
#include "FITK_Component/FITKRadiossData/FITKMaterialPlasTab.h"
#include "FITK_Component/FITKRadiossData/FITKMaterialPlasBrit.h"
#include "FITK_Component/FITKRadiossData/FITKMaterialHydro.h"
#include "FITK_Component/FITKRadiossData/FITKPropSolid.h"
#include "FITK_Component/FITKRadiossData/FITKPropShell.h"
#include "FITK_Component/FITKRadiossData/FITKPropBeam.h"
#include "FITK_Component/FITKRadiossData/FITKPropSPH.h"
#include "FITK_Component/FITKRadiossData/FITKPropSandwichShell.h"
#include "FITK_Component/FITKRadiossData/FITKPropBeamSPR.h"
#include "FITK_Component/FITKRadiossData/FITKNonLinerCurve.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractEquationOfState.h"
#include "FITK_Component/FITKRadiossData/FITKEquationOfStatePolynomial.h"
#include "FITK_Component/FITKRadiossData/FITKFailureModelJOHNSON.h"
#include "FITK_Component/FITKRadiossData/FITKFailureModelBIQUAD.h"
#include <QHeaderView>

namespace GUI
{
    TreeWidgetProperty::TreeWidgetProperty(QWidget* parent) :GUITreeWidgetBase(parent)
    {
        // 隐藏树形菜单头
        this->header()->hide();
        // 设置选择模式：单选
        this->setSelectionMode(QAbstractItemView::SingleSelection);
        // 更新树形菜单
        updateTreeWidget();
    }

    void TreeWidgetProperty::updateTreeWidget()
    {
        // 锁定信号（防止刷新时触发信号）
        this->blockSignals(true);
        // 清空树
        this->clear();

        // Material
        updateTreeWidgetMaterial(createRootItem(this, tr("Material"), GUI::TreeWidgetPropertyType::TWPropertyMaterial));
        // Property
        updateTreeWidgetProperty(createRootItem(this, tr("Property"), GUI::TreeWidgetPropertyType::TWPropertyProperty));
        // Curve
        updateTreeWidgetCurve(createRootItem(this, tr("Curve"), GUI::TreeWidgetPropertyType::TWPropertyCurve));
        // EOS
        updateTreeWidgetEquationOfState(createRootItem(this, tr("Equation Of State"), GUI::TreeWidgetPropertyType::TWPropertyEOS));
        // Failure Model
        updateTreeWidgetFailureModel(createRootItem(this, tr("Failure Model"), GUI::TreeWidgetPropertyType::TWPropertyFailureModel));

        // 展开全部节点
        this->expandAll();
        // 解锁信号（刷新后允许触发信号）
        this->blockSignals(false);
    }

    Core::FITKTreeWidgetOperator* TreeWidgetProperty::getOperator()
    {
        return getOperatorT<Core::FITKTreeWidgetOperator>("TreePropertyEvent");
    }

    void TreeWidgetProperty::on_itemContextMenu(const QList<QTreeWidgetItem*>& items, QMenu* menu)
    {
        // 检查输入参数
        if (items.size() != 1 || menu == nullptr) return;

        // 获取当前项
        QTreeWidgetItem* item = items.at(0);
        if (item == nullptr) return;

        // 根据节点类型初始化右键菜单
        int type = item->type();

        // 根据节点类型生成菜单
        if (type == GUI::TreeWidgetPropertyType::TWPropertyMaterial) createMenuItem(menu, tr("Create Material"), "actionPropertyTreeCreateMaterial");
        else if (type == GUI::TreeWidgetPropertyType::TWPropertyMaterialChild)
        {
            createMenuItem(menu, tr("Rename Material"), "actionPropertyTreeRenameMaterial");
            createMenuItem(menu, tr("Edit Material"), "actionPropertyTreeEditMaterial");
            createMenuItem(menu, tr("Delete Material"), "actionPropertyTreeDeleteMaterial");
        }
        else if (type == GUI::TreeWidgetPropertyType::TWPropertyProperty) createMenuItem(menu, tr("Create Property"), "actionPropertyTreeCreateProperty");
        else if (type == GUI::TreeWidgetPropertyType::TWPropertyPropertyChild)
        {
            createMenuItem(menu, tr("Rename Property"), "actionPropertyTreeRenameProperty");
            createMenuItem(menu, tr("Edit Property"), "actionPropertyTreeEditProperty");
            createMenuItem(menu, tr("Delete Property"), "actionPropertyTreeDeleteProperty");
        }
        else if (type == GUI::TreeWidgetPropertyType::TWPropertyCurve) createMenuItem(menu, tr("Create Curve"), "actionPropertyTreeCreateCurve");
        else if (type == GUI::TreeWidgetPropertyType::TWPropertyCurveChild)
        {
            createMenuItem(menu, tr("Rename Curve"), "actionPropertyTreeRenameCurve");
            createMenuItem(menu, tr("Edit Curve"), "actionPropertyTreeEditCurve");
            createMenuItem(menu, tr("Delete Curve"), "actionPropertyTreeDeleteCurve");
        }
        else if (type == GUI::TreeWidgetPropertyType::TWPropertyEOS) createMenuItem(menu, tr("Create Equation Of State"), "actionPropertyTreeCreateEOS");
        else if (type == GUI::TreeWidgetPropertyType::TWPropertyEOSChild)
        {
            createMenuItem(menu, tr("Rename Equation Of State"), "actionPropertyTreeRenameEOS");
            createMenuItem(menu, tr("Edit Equation Of State"), "actionPropertyTreeEditEOS");
            createMenuItem(menu, tr("Delete Equation Of State"), "actionPropertyTreeDeleteEOS");
        }
        else if (type == GUI::TreeWidgetPropertyType::TWPropertyFailureModel) createMenuItem(menu, tr("Create Failure Model"), "actionPropertyTreeCreateFailureModel");
        else if (type == GUI::TreeWidgetPropertyType::TWPropertyFailureModelChild)
        {
            createMenuItem(menu, tr("Rename Failure Model"), "actionPropertyTreeRenameFailureModel");
            createMenuItem(menu, tr("Edit Failure Model"), "actionPropertyTreeEditFailureModel");
            createMenuItem(menu, tr("Delete Failure Model"), "actionPropertyTreeDeleteFailureModel");
        }
        else return;
    }

    void TreeWidgetProperty::updateTreeWidgetMaterial(QTreeWidgetItem* itemParent)
    {
        // 检查父节点
        if (itemParent == nullptr) return;

        // 获取算例数据
        Radioss::FITKRadiossCase* dataCase = getDataCase();
        if (dataCase == nullptr) return;

        // 获取材料管理器
        Interface::FITKMaterialManager* manager = dataCase->getMaterialManager();
        if (manager == nullptr) return;

        // 遍历材料管理器
        for (int i = 0; i < manager->getDataCount(); i++)
        {
            // 获取数据对象
            Core::FITKAbstractNDataObject* data = manager->getDataByIndex(i);
            if (data == nullptr) continue;

            // 获取关键字
            QString keyword = getRadiossKeyWord(data), itemName = QString();
            // 获取节点名称
            if (keyword.isEmpty()) itemName = data->getDataObjectName();
            else itemName = QString("%1[%2]").arg(data->getDataObjectName()).arg(getRadiossKeyWord(data));
            if (itemName.isEmpty()) continue;
            
            // 创建子节点
            QTreeWidgetItem* itemChild = createChildItem(itemParent, itemName, GUI::TreeWidgetPropertyType::TWPropertyMaterialChild);
            setObjectID(itemChild, data->getDataObjectID());
        }
    }

    void TreeWidgetProperty::updateTreeWidgetProperty(QTreeWidgetItem* itemParent)
    {
        // 检查父节点
        if (itemParent == nullptr) return;

        // 获取算例数据
        Radioss::FITKRadiossCase* dataCase = getDataCase();
        if (dataCase == nullptr) return;

        // 获取属性管理器
        Interface::FITKAbstractSectionManager* manager = dataCase->getSectionManager();
        if (manager == nullptr) return;

        // 遍历属性管理器
        for (int i = 0; i < manager->getDataCount(); i++)
        {
            // 获取数据对象
            Core::FITKAbstractNDataObject* data = manager->getDataByIndex(i);
            if (data == nullptr) continue;

            // 获取关键字
            QString keyword = getRadiossKeyWord(data), itemName = QString();
            // 获取节点名称
            if (keyword.isEmpty()) itemName = data->getDataObjectName();
            else itemName = QString("%1[%2]").arg(data->getDataObjectName()).arg(getRadiossKeyWord(data));
            if (itemName.isEmpty()) continue;
            
            // 创建子节点
            QTreeWidgetItem* itemChild = createChildItem(itemParent, itemName, GUI::TreeWidgetPropertyType::TWPropertyPropertyChild);
            setObjectID(itemChild, data->getDataObjectID());
        }
    }

    void TreeWidgetProperty::updateTreeWidgetCurve(QTreeWidgetItem* itemParent)
    {
        // 检查父节点
        if (itemParent == nullptr) return;

        // 获取算例数据
        Radioss::FITKRadiossCase* dataCase = getDataCase();
        if (dataCase == nullptr) return;

        // 获取曲线管理器
        Radioss::FITKNonLinerCurveManager* manager = dataCase->getNonLinerCurveManager();
        if (manager == nullptr) return;

        // 遍历曲线管理器
        for (int i = 0; i < manager->getDataCount(); i++)
        {
            // 获取数据对象
            Core::FITKAbstractNDataObject* data = manager->getDataByIndex(i);
            if (data == nullptr) continue;

            // 获取关键字
            QString keyword = getRadiossKeyWord(data), itemName = QString();
            // 获取节点名称
            if (keyword.isEmpty()) itemName = data->getDataObjectName();
            else itemName = QString("%1[%2]").arg(data->getDataObjectName()).arg(getRadiossKeyWord(data));
            if (itemName.isEmpty()) continue;
            
            // 创建子节点
            QTreeWidgetItem* itemChild = createChildItem(itemParent, itemName, GUI::TreeWidgetPropertyType::TWPropertyCurveChild);
            setObjectID(itemChild, data->getDataObjectID());
        }
    }

    void TreeWidgetProperty::updateTreeWidgetEquationOfState(QTreeWidgetItem * itemParent)
    {
        // 检查父节点
        if (itemParent == nullptr) return;

        // 获取算例数据
        Radioss::FITKRadiossCase* dataCase = getDataCase();
        if (dataCase == nullptr) return;

        // 获取曲线管理器
        Radioss::FITKEquationOfStateManager* manager = dataCase->getEquationOfState();
        if (manager == nullptr) return;

        // 遍历曲线管理器
        for (int i = 0; i < manager->getDataCount(); i++)
        {
            // 获取数据对象
            Core::FITKAbstractNDataObject* data = manager->getDataByIndex(i);
            if (data == nullptr) continue;

            // 获取关键字
            QString keyword = getRadiossKeyWord(data), itemName = QString();
            // 获取节点名称
            if (keyword.isEmpty()) itemName = data->getDataObjectName();
            else itemName = QString("%1[%2]").arg(data->getDataObjectName()).arg(getRadiossKeyWord(data));
            if (itemName.isEmpty()) continue;

            // 创建子节点
            QTreeWidgetItem* itemChild = createChildItem(itemParent, itemName, GUI::TreeWidgetPropertyType::TWPropertyEOSChild);
            setObjectID(itemChild, data->getDataObjectID());
        }
    }

    void TreeWidgetProperty::updateTreeWidgetFailureModel(QTreeWidgetItem* itemParent)
    {
        // 检查父节点
        if (itemParent == nullptr) return;

        // 获取算例数据
        Radioss::FITKRadiossCase* dataCase = getDataCase();
        if (dataCase == nullptr) return;

        Radioss::FITKRadiossFailureModelManager* manager = dataCase->getFailureModelManager();
        if (manager == nullptr) return;

        for (int i = 0; i < manager->getDataCount(); i++)
        {
            // 获取数据对象
            Core::FITKAbstractNDataObject* data = manager->getDataByIndex(i);
            if (data == nullptr) continue;

            // 获取关键字
            QString keyword = getRadiossKeyWord(data), itemName = QString();
            // 获取节点名称
            if (keyword.isEmpty()) itemName = data->getDataObjectName();
            else itemName = QString("%1[%2]").arg(data->getDataObjectName()).arg(getRadiossKeyWord(data));
            if (itemName.isEmpty()) continue;

            // 创建子节点
            QTreeWidgetItem* itemChild = createChildItem(itemParent, itemName, GUI::TreeWidgetPropertyType::TWPropertyFailureModelChild);
            setObjectID(itemChild, data->getDataObjectID());
        }
    }

    QString TreeWidgetProperty::getRadiossKeyWord(Core::FITKAbstractNDataObject* data)
    {
        // Empty
        if (data == nullptr) return QString();
        // Material
        else if (dynamic_cast<Radioss::FITKMaterialElastic*>(data)) return Radioss::FITKMaterialElastic::GetFITKMaterialElasticRadiossKeyWord();
        else if (dynamic_cast<Radioss::FITKMaterialPlasJohns*>(data)) return Radioss::FITKMaterialPlasJohns::GetFITKMaterialPlasJohnsRadiossKeyWord();
        else if (dynamic_cast<Radioss::FITKMaterialPlasTab*>(data)) return Radioss::FITKMaterialPlasTab::GetFITKMaterialPlasTabRadiossKeyWord();
        else if (dynamic_cast<Radioss::FITKMaterialPlasBrit*>(data)) return Radioss::FITKMaterialPlasBrit::GetFITKMaterialPlasBritRadiossKeyWord();
        else if (dynamic_cast<Radioss::FITKMaterialHydro*>(data)) return Radioss::FITKMaterialHydro::GetFITKMaterialHydroRadiossKeyWord();

        // Property
        else if (dynamic_cast<Radioss::FITKPropShell*>(data)) return Radioss::FITKPropShell::GetFITKPropShellRadiossKeyWord();
        else if (dynamic_cast<Radioss::FITKPropSolid*>(data)) return Radioss::FITKPropSolid::GetFITKPropSolidRadiossKeyWord();
        else if (dynamic_cast<Radioss::FITKPropBeam*>(data)) return Radioss::FITKPropBeam::GetFITKPropBeamRadiossKeyWord();
        else if (dynamic_cast<Radioss::FITKPropSPH*>(data)) return Radioss::FITKPropSPH::GetFITKPropSPHRadiossKeyWord();
        else if (dynamic_cast<Radioss::FITKPropSandwichShell*>(data)) return Radioss::FITKPropSandwichShell::GetFITKPropSandwichShellRadiossKeyWord();
        else if (dynamic_cast<Radioss::FITKPropBeamSPR*>(data)) return Radioss::FITKPropBeamSPR::GetFITKPropBeamSPRRadiossKeyWord();
        // Curve
        else if (dynamic_cast<Radioss::FITKNonLinerCurve*>(data)) return Radioss::FITKNonLinerCurve::GetFITKNonLinerCurveRadiossKeyWord();
        // Equation Of State
        else if (dynamic_cast<Radioss::FITKEquationOfStatePolynomial*>(data)) return Radioss::FITKEquationOfStatePolynomial::GetFITKEquationOfStatePolynomialRadiossKeyWord();
        // Failure Model
        else if (dynamic_cast<Radioss::FITKFailureModelJOHNSON*>(data)) return Radioss::FITKFailureModelJOHNSON::GetFITKFailureModelJOHNSONRadiossKeyWord();
        else if (dynamic_cast<Radioss::FITKFailureModelBIQUAD*>(data)) return Radioss::FITKFailureModelBIQUAD::GetFITKFailureModelBIQUADRadiossKeyWord();
        // Other
        else return QString();
    }
}
