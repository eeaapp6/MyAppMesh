/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperatorMaterialProp.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Interface/FITKInterfacePhysics/FITKAbstractMaterial.h"
#include "FITK_Component/FITKRadiossData/FITKMaterialElastic.h"
#include "FITK_Component/FITKRadiossData/FITKMaterialPlasJohns.h"
#include "FITK_Component/FITKRadiossData/FITKMaterialPlasTab.h"
#include "FITK_Component/FITKRadiossData/FITKMaterialPlasBrit.h"
#include "FITK_Component/FITKRadiossData/FITKMaterialHydro.h"
#include "GUIFrame/MainWindow.h"
#include "GUIWidget/ControlPanel.h"

#include "GUIDialog/GUIAnalysisDialog/GUIMaterialTypeDialog.h"
#include "GUIDialog/GUIAnalysisDialog/GUIMaterialJohnsonCookDialog.h"
#include "GUIDialog/GUIAnalysisDialog/GUIMaterialElasticDialog.h"
#include "GUIDialog/GUIAnalysisDialog/GUIMaterialPlasTabDialog.h"
#include "GUIDialog/GUIAnalysisDialog/GUIMaterialPlasBritDialog.h"
#include "GUIDialog/GUIAnalysisDialog/GUIMaterialHydroDialog.h"

#include "GUIDialog/GUIToolDialog/GUIRenameDialog.h"
#include "OperatorsInterface/TreeEventOperator.h"

namespace OperModel
{
    bool OperatorMaterialProp::execGUI()
    {
        if (_emitter == nullptr)
            return false;
        QWidget* mw = FITKAPP->getGlobalData()->getMainWindow();
        if (!mw) return false;
        QString name = _emitter->objectName();
        if (name.isEmpty())return false;
        if (name == "actionMaterialPropCreate")
        {
            GUI::GUIMaterialTypeDialog dialog;
            if (dialog.exec() != QDialog::Accepted)return false;
            int type = dialog.selectedMaterialType();
            this->setArgs("MaterialName", dialog.getMaterialName());
            switch (type)
            {
            case 0: {
                GUI::GUIMaterialJohnsonCookDialog* dialog = new GUI::GUIMaterialJohnsonCookDialog(nullptr,this, mw);
                dialog->show();
                break;
            }
            case 1: {
                GUI::GUIMaterialElasticDialog* dialog = new GUI::GUIMaterialElasticDialog(nullptr, this, mw);
                dialog->show();
                break;
            }
            case 2: {
                GUI::GUIMaterialPlasTabDialog* dialog = new GUI::GUIMaterialPlasTabDialog(nullptr, this, mw);
                dialog->show();
                break;
            }
            case 3: {
                GUI::GUIMaterialPlasBritDialog* dialog = new GUI::GUIMaterialPlasBritDialog(nullptr, this, mw);
                dialog->show();
                break;
            }
            case 4: {
                GUI::GUIMaterialHydroDialog* dialog = new GUI::GUIMaterialHydroDialog(nullptr, this, mw);
                dialog->show();
                break;
            }
            default:
                return false;
            }
        }
        else if (name == "actionMaterialPropRename")
        {
            return this->materialPropRenameOper();
        }
        else if (name == "actionMaterialPropEdit")
        {
            // 获取选中的材料
            Interface::FITKAbstractMaterial* selectedMaterial = getSelectedMaterial();
            if (selectedMaterial == nullptr) return false;

            // 创建编辑对话框
            if (dynamic_cast<Radioss::FITKMaterialPlasJohns*>(selectedMaterial) != nullptr)
            {
                GUI::GUIMaterialJohnsonCookDialog* dialog = new GUI::GUIMaterialJohnsonCookDialog(selectedMaterial, this, mw);
                dialog->show();
            }
            else if (dynamic_cast<Radioss::FITKMaterialElastic*>(selectedMaterial) != nullptr)
            {
                GUI::GUIMaterialElasticDialog* dialog = new GUI::GUIMaterialElasticDialog(selectedMaterial, this, mw);
                dialog->show();
            }
            else if (dynamic_cast<Radioss::FITKMaterialPlasTab*>(selectedMaterial) != nullptr)
            {
                GUI::GUIMaterialPlasTabDialog* dialog = new GUI::GUIMaterialPlasTabDialog(selectedMaterial, this, mw);
                dialog->show();
            }
            else if (dynamic_cast<Radioss::FITKMaterialPlasBrit*>(selectedMaterial) != nullptr)
            {
                GUI::GUIMaterialPlasBritDialog* dialog = new GUI::GUIMaterialPlasBritDialog(selectedMaterial, this, mw);
                dialog->show();
            }
            else if (dynamic_cast<Radioss::FITKMaterialHydro*>(selectedMaterial) != nullptr)
            {
                GUI::GUIMaterialHydroDialog* dialog = new GUI::GUIMaterialHydroDialog(selectedMaterial, this, mw);
                dialog->show();
            }
        }
        else if (name == "actionMaterialPropDelete")
        {
            return deleteMaterial();
        }
        return false;
    }
    bool OperatorMaterialProp::execProfession()
    {
        // 通过树形菜单事件处理器刷新树
        QStringList listOpertor = { "TreeAssemblyEvent", "TreeGroupEvent" ,"TreePropertyEvent","TreeSolutionEvent" };
        for (int i = 0; i < listOpertor.size(); i++)
        {
            EventOper::TreeEventOperator* oper = dynamic_cast<EventOper::TreeEventOperator*>(Core::FITKOperatorRepo::getInstance()->getOperator(listOpertor[i]));
            if (oper != nullptr) oper->updateTree();
        }
        return false;
    }

    Interface::FITKAbstractMaterial* OperatorMaterialProp::getSelectedMaterial()
    {
        //// 获取主窗口
        //GUI::MainWindow* mainWindow = GUI::MainWindow::GetMainWindowFromFramework();
        //if (!mainWindow) return nullptr;

        //// 获取控制面板
        //GUI::ControlPanel* controlPanel = mainWindow->getControlPanel();
        //if (!controlPanel) return nullptr;

        //// 获取属性树控件
        //GUI::PropertyTreeWidget* propertyTree = controlPanel->getPropertyTree();
        //if (!propertyTree) return nullptr;

        //// 获取当前选中的项目
        //QList<QTreeWidgetItem*> selectedItems = propertyTree->selectedItems();
        //if (selectedItems.isEmpty()) return nullptr;

        //QTreeWidgetItem* selectedItem = selectedItems.first();
        //if (!selectedItem) return nullptr;

        //// 获取材料名称
        //QString materialName = selectedItem->text(0);
        //if (materialName.isEmpty()) return nullptr;

        // 从材料管理器中查找对应的材料
        int materialID = -1;
        this->argValue("SelectedItemDataID", materialID);
        Interface::FITKMaterialManager* manager = getMaterialManager();
        if (!manager) return nullptr;
        Interface::FITKAbstractMaterial* material = manager->getDataByID(materialID);
        if(!material) return nullptr;
        return material;
        // 遍历所有材料，找到匹配的材料
        //for (int i = 0; i < manager->getDataCount(); i++)
        //{
        //    Interface::FITKAbstractMaterial* material = manager->getDataByIndex(i);
        //    if (material && material->getDataObjectName() == materialName)
        //    {
        //        return material;
        //    }
        //}

       
    }
    Interface::FITKMaterialManager* OperatorMaterialProp::getMaterialManager()
    {
        // 获取算例数据
        Radioss::FITKRadiossCase* dataCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (dataCase == nullptr) return nullptr;

        // 获取材料管理器
        Interface::FITKMaterialManager* materialManager = dataCase->getMaterialManager();
        return materialManager;
    }
    bool OperatorMaterialProp::materialPropRenameOper()
    {
        // 获取部件ID
        int materialID = 0;
        if (!this->argValue<int>("MaterialID", materialID)) return false;
        // 获取 Radioss Case
        Radioss::FITKRadiossCase* caseData = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (caseData == nullptr) return false;
        // 获取材料管理器
        Interface::FITKMaterialManager* manager = getMaterialManager();
        if (manager == nullptr) return false;
        Interface::FITKAbstractMaterial* materialData = manager->getDataByID(materialID);
        if (!materialData) return false;
        GUI::GUIRenameDialog renameDialog(this, materialData, manager, FITKAPP->getGlobalData()->getMainWindow());
        renameDialog.exec();
        return true;
    }
    bool OperatorMaterialProp::deleteMaterial()
    {
        // 获取选中的材料
        Interface::FITKAbstractMaterial* selectedMaterial = getSelectedMaterial();
        if (selectedMaterial == nullptr) return false;

        // 获取材料管理器
        Interface::FITKMaterialManager* manager = getMaterialManager();
        if (manager == nullptr) return false;

        // 从管理器中移除材料
        manager->removeDataObj(selectedMaterial);

        // 通过树形菜单事件处理器刷新树
        QStringList listOpertor = { "TreeAssemblyEvent", "TreeGroupEvent" ,"TreePropertyEvent","TreeSolutionEvent" };
        for (int i = 0; i < listOpertor.size(); i++)
        {
            EventOper::TreeEventOperator* oper = dynamic_cast<EventOper::TreeEventOperator*>(Core::FITKOperatorRepo::getInstance()->getOperator(listOpertor[i]));
            if (oper != nullptr) oper->updateTree();
        }
        
        return true;
    }
}

