/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "TreeWidgetSolution.h"
#include "GUIWidget/GUIEnumType.h"
#include "FITK_Kernel/FITKCore/FITKTreeWidgetOperator.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSolution.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSolverSettings.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossResultRequest.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractInteraction.h"
#include "FITK_Component/FITKRadiossData/FITKInteractionTied.h"
#include "FITK_Component/FITKRadiossData/FITKInteractionMultiUsageImpact.h"
#include "FITK_Component/FITKRadiossData/FITKRWall.h"
#include "FITK_Component/FITKRadiossData/FITKGravity.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractBCS.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossBCS.h"
#include "FITK_Component/FITKRadiossData/FITKInitialField.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractConnection.h"
#include "FITK_Component/FITKRadiossData/FITKConnectionRBODY.h"
#include "FITK_Component/FITKRadiossData/FITKProbeAcceleration.h"
#include "FITK_Component/FITKRadiossData/FITKProbeSection.h"
#include "OperatorsInterface/TreeEventOperator.h"
#include <QHeaderView>

namespace GUI
{
    TreeWidgetSolution::TreeWidgetSolution(QWidget* parent) : GUITreeWidgetBase(parent)
    {
        // 隐藏树形菜单头
        this->header()->hide();
        // 设置选择模式
        this->setSelectionMode(QAbstractItemView::SingleSelection);
        // 更新树形菜单
        updateTreeWidget();
    }

    void TreeWidgetSolution::updateTreeWidget()
    {
        // 锁定信号（防止刷新时触发信号）
        this->blockSignals(true);
        // 清空树
        this->clear();

        // 创建根节点（根节点）
        QTreeWidgetItem* root = createRootItem(this, tr("Solution"), GUI::TreeWidgetSolutionType::TWSolutionRoot);
        if (root == nullptr) return;

        // 获取算例数据
        Radioss::FITKRadiossCase* dataCase = getDataCase();
        if (dataCase == nullptr) return;

        // 获取解决方案管理器
        Radioss::FITKRadiossSolutionManager* manager = dataCase->getSolutionManager();
        if (manager == nullptr) return;

        // 获取当前解决方案
        Radioss::FITKRadiossSolution* currentSolution = manager->getCurrentSolution();
        if (currentSolution == nullptr) return;

        // 遍历解决方案
        for (int i = 0; i < manager->getDataCount(); i++)
        {
            // 获取解决方案数据
            Radioss::FITKRadiossSolution* solution = manager->getDataByIndex(i);
            if (solution == nullptr) continue;
            
            // Solution子节点（子节点）
            QString name = solution->getDataObjectName();
            // 加*
            if (solution == currentSolution) name += " *";
            QTreeWidgetItem* itemSolution = createChildItem(root, name, GUI::TreeWidgetSolutionType::TWSolutionChild);
            if (itemSolution == nullptr) continue;
            if (solution == currentSolution)
            {
                // 加粗
                QFont font = itemSolution->font(0);
                font.setBold(true);
                itemSolution->setFont(0, font);
            }
            setObjectID(itemSolution, solution->getDataObjectID());
            itemSolution->setData(2, 0, solution->getDataObjectID());

            // 更新分支节点
            updateSetting(createChildItem(itemSolution, tr("Setting"), GUI::TreeWidgetSolutionType::TWSolutionChildSetting), solution);
            updateContact(createChildItem(itemSolution, tr("Interaction"), GUI::TreeWidgetSolutionType::TWSolutionChildInteraction), solution);
            updateInitialCondition(createChildItem(itemSolution, tr("Initial Condition"), GUI::TreeWidgetSolutionType::TWSolutionChildInitialCondition), solution);
            updateLoad(createChildItem(itemSolution, tr("Load"), GUI::TreeWidgetSolutionType::TWSolutionChildLoad), solution);
            updateConstraint(createChildItem(itemSolution, tr("Constraint"), GUI::TreeWidgetSolutionType::TWSolutionChildConstraint), solution);
            updateBoundary(createChildItem(itemSolution, tr("Boundary"), GUI::TreeWidgetSolutionType::TWSolutionChildBoundary), solution);
            updateConnection(createChildItem(itemSolution, tr("Connection"), GUI::TreeWidgetSolutionType::TWSolutionChildConnection), solution);
            QTreeWidgetItem* probeItem = new QTreeWidgetItem(itemSolution, QStringList{ tr("Probe") });
            updateAccel(createChildItem(probeItem, tr("Accelermeter"), GUI::TreeWidgetSolutionType::TWSolutionChildProbeAccelRoot), solution);
            updateSection(createChildItem(probeItem, tr("Section"), GUI::TreeWidgetSolutionType::TWSolutionChildProbeSectionRoot), solution);
        }

        // 展开全部节点
        this->expandAll();
        // 解锁信号（刷新后允许触发信号）
        this->blockSignals(false);
    }

    Core::FITKTreeWidgetOperator* TreeWidgetSolution::getOperator()
    {
        return getOperatorT<Core::FITKTreeWidgetOperator>("TreeSolutionEvent");
    }

    void TreeWidgetSolution::on_itemStateChanged(QTreeWidgetItem* item, bool state, int column)
    {
        EventOper::TreeEventOperator* oper = dynamic_cast<EventOper::TreeEventOperator*>(this->getOperator());
        if (oper == nullptr) return;
        oper->on_itemStateChanged(this, item, state, column);
    }

    void TreeWidgetSolution::on_itemContextMenu(const QList<QTreeWidgetItem*>& items, QMenu* menu)
    {
        // 右键菜单动作生成
        if (items.size() != 1 || menu == nullptr) return;

        // 获取当前节点
        QTreeWidgetItem* item = items.at(0);
        if (item == nullptr) return;

        // 获取节点类型
        int type = item->type();
        bool isActive = item->data(3, 0).toBool();

        // 根据节点类型生成菜单
        if (type == GUI::TreeWidgetSolutionType::TWSolutionRoot) createMenuItem(menu, tr("Create Solution"), "actionSolutionTreeCreateSolution");
        else if (type == GUI::TreeWidgetSolutionType::TWSolutionChild)
        {
            createMenuItem(menu, tr("Rename Solution"), "actionSolutionTreeRenameSolution");
            createMenuItem(menu, tr("Delete Solution"), "actionSolutionTreeDeleteSolution");
        }
        else if (type == GUI::TreeWidgetSolutionType::TWSolutionChildSettingSolverSetting) createMenuItem(menu, tr("Edit Solver Setting"), "actionSolutionTreeEditSolverSetting");
        else if (type == GUI::TreeWidgetSolutionType::TWSolutionChildSettingResultRequest) createMenuItem(menu, tr("Edit Result Request"), "actionSolutionTreeEditResultRequest");
        else if (type == GUI::TreeWidgetSolutionType::TWSolutionChildInteraction)
        {
            createMenuItem(menu, tr("Create KinematicTied"), "actionSolutionTreeCreateKinematicTied");
            createMenuItem(menu, tr("Create MultiUsageImpact"), "actionSolutionTreeCreateMultiUsageImpact");
        }
        else if (type == GUI::TreeWidgetSolutionType::TWSolutionChildInteractionKinematicTied)
        {
            isActive ? createMenuItem(menu, tr("Inhibit KinematicTied"), "actionSolutionTreeChangeInteractionActivationStatus") : createMenuItem(menu, tr("Activate Interaction"), "actionSolutionTreeChangeInteractionActivationStatus");
            createMenuItem(menu, tr("Rename KinematicTied"), "actionSolutionTreeRenameInteraction");
            createMenuItem(menu, tr("Edit KinematicTied"), "actionSolutionTreeEditKinematicTied");
            createMenuItem(menu, tr("Delete KinematicTied"), "actionSolutionTreeDeleteInteraction");
        }
        else if (type == GUI::TreeWidgetSolutionType::TWSolutionChildInteractionMultiUsageImpact)
        {
            isActive ? createMenuItem(menu, tr("Inhibit MultiUsageImpact"), "actionSolutionTreeChangeInteractionActivationStatus") : createMenuItem(menu, tr("Activate Interaction"), "actionSolutionTreeChangeInteractionActivationStatus");
            createMenuItem(menu, tr("Rename MultiUsageImpact"), "actionSolutionTreeRenameInteraction");
            createMenuItem(menu, tr("Edit MultiUsageImpact"), "actionSolutionTreeEditMultiUsageImpact");
            createMenuItem(menu, tr("Delete MultiUsageImpact"), "actionSolutionTreeDeleteInteraction");
        }
        else if (type == GUI::TreeWidgetSolutionType::TWSolutionChildInitialCondition)
        {
            createMenuItem(menu, tr("Create Initial Field"), "actionSolutionTreeCreateInitialField");
        }
        else if (type == GUI::TreeWidgetSolutionType::TWSolutionChildInitialConditionInitialField)
        {
            isActive ? createMenuItem(menu, tr("Inhibit Initial Field"), "actionSolutionTreeChangeInitialFieldActivationStatus") : createMenuItem(menu, tr("Activate Initial Field"), "actionSolutionTreeChangeInitialFieldActivationStatus");
            createMenuItem(menu, tr("Rename Initial Field"), "actionSolutionTreeRenameInitialField");
            createMenuItem(menu, tr("Edit Initial Field"), "actionSolutionTreeEditInitialField");
            createMenuItem(menu, tr("Delete Initial Field"), "actionSolutionTreeDeleteInitialField");
        }
        else if (type == GUI::TreeWidgetSolutionType::TWSolutionChildConstraint)
        {
            createMenuItem(menu, tr("Create Rigid Wall"), "actionSolutionTreeCreateRigidWall");
            createMenuItem(menu, tr("Create Gravity"), "actionSolutionTreeCreateGravity");
        }
        else if (type == GUI::TreeWidgetSolutionType::TWSolutionChildConstraintRigidWall)
        {
            isActive ? createMenuItem(menu, tr("Inhibit Rigid Wall"), "actionSolutionTreeChangeRigidWallActivationStatus") : createMenuItem(menu, tr("Activate Rigid Wall"), "actionSolutionTreeChangeRigidWallActivationStatus");
            createMenuItem(menu, tr("Rename Rigid Wall"), "actionSolutionTreeRenameRigidWall");
            createMenuItem(menu, tr("Edit Rigid Wall"), "actionSolutionTreeEditRigidWall");
            createMenuItem(menu, tr("Delete Rigid Wall"), "actionSolutionTreeDeleteRigidWall");
        }
        else if (type == GUI::TreeWidgetSolutionType::TWSolutionChildConstraintGravity)
        {
            isActive ? createMenuItem(menu, tr("Inhibit Gravity"), "actionSolutionTreeChangeGravityActivationStatus") : createMenuItem(menu, tr("Activate Gravity"), "actionSolutionTreeChangeGravityActivationStatus");
            createMenuItem(menu, tr("Rename Gravity"), "actionSolutionTreeRenameGravity");
            createMenuItem(menu, tr("Edit Gravity"), "actionSolutionTreeEditGravity");
            createMenuItem(menu, tr("Delete Gravity"), "actionSolutionTreeDeleteGravity");
        }
        else if (type == GUI::TreeWidgetSolutionType::TWSolutionChildBoundary)
        {
            createMenuItem(menu, tr("Create BCS"), "actionSolutionTreeCreateBCS");
        }
        else if (type == GUI::TreeWidgetSolutionType::TWSolutionChildBoundaryBCS)
        {
            isActive ? createMenuItem(menu, tr("Inhibit BCS"), "actionSolutionTreeChangeBCSActivationStatus") : createMenuItem(menu, tr("Activate BCS"), "actionSolutionTreeChangeBCSActivationStatus");
            createMenuItem(menu, tr("Rename BCS"), "actionSolutionTreeRenameBCS");
            createMenuItem(menu, tr("Edit BCS"), "actionSolutionTreeEditBCS");
            createMenuItem(menu, tr("Delete BCS"), "actionSolutionTreeDeleteBCS");
        }
        else if (type == GUI::TreeWidgetSolutionType::TWSolutionChildConnection)
        {
            createMenuItem(menu, tr("Create Rigid Body"), "actionSolutionTreeCreateRBody");
        }
        else if (type == GUI::TreeWidgetSolutionType::TWSolutionChildConnectionRBody)
        {
            isActive ? createMenuItem(menu, tr("Inhibit Rigid Body"), "actionSolutionTreeChangeRBodyActivationStatus") : createMenuItem(menu, tr("Activate Rigid Body"), "actionSolutionTreeChangeRBodyActivationStatus");
            createMenuItem(menu, tr("Rename Rigid Body"), "actionSolutionTreeRenameRBody");
            createMenuItem(menu, tr("Edit Rigid Body"), "actionSolutionTreeEditRBody");
            createMenuItem(menu, tr("Delete Rigid Body"), "actionSolutionTreeDeleteRBody");
        }
        else if (type == GUI::TreeWidgetSolutionType::TWSolutionChildProbeAccelRoot)
        {
            createMenuItem(menu, tr("Create Probe Accel"), "actionSolutionTreeCreateProbeAccel");
        }
        else if (type == GUI::TreeWidgetSolutionType::TWSolutionChildProbeSectionRoot)
        {
            createMenuItem(menu, tr("Create Probe Section"), "actionSolutionTreeCreateProbeSection");
        }
        else if (type == GUI::TreeWidgetSolutionType::TWSolutionChildProbeAccel)
        {
            isActive ? createMenuItem(menu, tr("Inhibit Probe Accel"), "actionSolutionTreeChangeProbeAccelActivationStatus") : createMenuItem(menu, tr("Activate Probe Accel"), "actionSolutionTreeChangeProbeAccelActivationStatus");
            createMenuItem(menu, tr("Rename Probe Accel"), "actionSolutionTreeRenameProbeAccel");
            createMenuItem(menu, tr("Edit Probe Accel"), "actionSolutionTreeEditProbeAccel");
            createMenuItem(menu, tr("Delete Probe Accel"), "actionSolutionTreeDeleteProbeAccel");
        }
        else if (type == GUI::TreeWidgetSolutionType::TWSolutionChildProbeSection)
        {
            isActive ? createMenuItem(menu, tr("Inhibit Probe Section"), "actionSolutionTreeChangeProbeSectionActivationStatus") : createMenuItem(menu, tr("Activate Probe Section"), "actionSolutionTreeChangeProbeSectionActivationStatus");
            createMenuItem(menu, tr("Rename Probe Section"), "actionSolutionTreeRenameProbeSection");
            createMenuItem(menu, tr("Edit Probe Section"), "actionSolutionTreeEditProbeSection");
            createMenuItem(menu, tr("Delete Probe Section"), "actionSolutionTreeDeleteProbeSection");
        }
        else return;
    }

    void TreeWidgetSolution::updateSetting(QTreeWidgetItem* itemParent, Radioss::FITKRadiossSolution* solution)
    {
        // 检查参数
        if (itemParent == nullptr || solution == nullptr) return;

        // 存储当前解决方案ID
        itemParent->setData(2, 0, solution->getDataObjectID());
        // 父节点禁止选中
        itemParent->setFlags(Qt::ItemFlag::ItemIsEnabled);

        // Solver Setting 求解器设置（分支子节点）
        QTreeWidgetItem* itemSolverSetting = createChildItem(itemParent, tr("Solver Setting"), GUI::TreeWidgetSolutionType::TWSolutionChildSettingSolverSetting);
        if (itemSolverSetting == nullptr) return;
        Radioss::FITKRadiossSolverSettings* dataSolverSetting = solution->getSolverSettings();
        if (dataSolverSetting == nullptr) return;
        setObjectID(itemSolverSetting, dataSolverSetting->getDataObjectID());
        itemSolverSetting->setData(2, 0, solution->getDataObjectID());

        // Result Request 结果请求（分支子节点）
        QTreeWidgetItem* itemResultRequest = createChildItem(itemParent, tr("Result Request"), GUI::TreeWidgetSolutionType::TWSolutionChildSettingResultRequest);
        if (itemResultRequest == nullptr) return;
        Radioss::FITKRadiossResultRequest* dataResultRequest = solution->getResultRequest();
        if (dataResultRequest == nullptr) return;
        setObjectID(itemResultRequest, dataResultRequest->getDataObjectID());
        itemResultRequest->setData(2, 0, solution->getDataObjectID());
    }

    void TreeWidgetSolution::updateContact(QTreeWidgetItem* itemParent, Radioss::FITKRadiossSolution* solution)
    {
        // 检查参数
        if (itemParent == nullptr || solution == nullptr) return;

        // 存储当前解决方案ID
        itemParent->setData(2, 0, solution->getDataObjectID());
        // 更新子节点
        updateTreeWidgetInteraction(itemParent, solution);
    }

    void TreeWidgetSolution::updateInitialCondition(QTreeWidgetItem* itemParent, Radioss::FITKRadiossSolution* solution)
    {
        // 检查参数
        if (itemParent == nullptr || solution == nullptr) return;

        // 存储当前解决方案ID
        itemParent->setData(2, 0, solution->getDataObjectID());
        // 更新子节点
        updateTreeWidgetInitialField(itemParent, solution);
    }

    void TreeWidgetSolution::updateLoad(QTreeWidgetItem* itemParent, Radioss::FITKRadiossSolution* solution)
    {
        // 检查参数
        if (itemParent == nullptr || solution == nullptr) return;

        // 存储当前解决方案ID
        itemParent->setData(2, 0, solution->getDataObjectID());
    }

    void TreeWidgetSolution::updateConstraint(QTreeWidgetItem* itemParent, Radioss::FITKRadiossSolution* solution)
    {
        // 检查参数
        if (itemParent == nullptr || solution == nullptr) return;

        // 存储当前解决方案ID
        itemParent->setData(2, 0, solution->getDataObjectID());

        // 更新子节点
        updateTreeWidgetRigidWall(itemParent, solution);
        updateTreeWidgetGravity(itemParent, solution);
    }

    void TreeWidgetSolution::updateBoundary(QTreeWidgetItem * itemParent, Radioss::FITKRadiossSolution * solution)
    {
        // 检查参数
        if (itemParent == nullptr || solution == nullptr) return;

        // 存储当前解决方案ID
        itemParent->setData(2, 0, solution->getDataObjectID());

        // 更新子节点
        updateTreeWidgetBCS(itemParent, solution);
    }

    void TreeWidgetSolution::updateConnection(QTreeWidgetItem * itemParent, Radioss::FITKRadiossSolution * solution)
    {
        // 检查参数
        if (itemParent == nullptr || solution == nullptr) return;

        // 存储当前解决方案ID
        itemParent->setData(2, 0, solution->getDataObjectID());

        // 更新子节点
        updateTreeWidgetConnection(itemParent, solution);
    }

    void TreeWidgetSolution::updateTreeWidgetInteraction(QTreeWidgetItem* itemParent, Radioss::FITKRadiossSolution* solution)
    {
        // 检查参数
        if (itemParent == nullptr || solution == nullptr) return;

        // 获取管理器
        Radioss::FITKInteractionManager* manager = solution->getInteractionManager();
        if (manager == nullptr) return;

        // 遍历数据管理器
        for (int i = 0; i < manager->getDataCount(); i++)
        {
            // 获取数据对象
            Radioss::FITKAbstractInteraction* data = manager->getDataByIndex(i);
            if (data == nullptr) continue;

            // 获取关键字
            QString keyword = getRadiossKeyWord(data), itemName = QString();
            // 获取节点名称
            if (keyword.isEmpty()) itemName = data->getDataObjectName();
            else itemName = QString("%1[%2]").arg(data->getDataObjectName()).arg(getRadiossKeyWord(data));
            if (itemName.isEmpty()) continue;
            // 激活状态（未激活）
            bool isActive = data->isEnable();
            if (!isActive) itemName += tr("(Inhibit)");

            // 获取类型
            Radioss::FITKAbstractInteraction::InteractionType type = data->getInteractionType();

            // 创建子节点
            QTreeWidgetItem* itemChild = nullptr;
            if (type == Radioss::FITKAbstractInteraction::IT_KinematicTied) itemChild = createChildItem(itemParent, itemName, GUI::TreeWidgetSolutionType::TWSolutionChildInteractionKinematicTied);
            else if (type == Radioss::FITKAbstractInteraction::IT_MultiUsageImpact) itemChild = createChildItem(itemParent, itemName, GUI::TreeWidgetSolutionType::TWSolutionChildInteractionMultiUsageImpact);
            if (itemChild == nullptr) continue;
            isActive ? setItemChecked(itemChild, 0, data->getVisible()) : setItemChecked(itemChild, 0, false);
            isActive ? show(data->getDataObjectID(), data->getVisible()) : show(data->getDataObjectID(), false);
            setObjectID(itemChild, data->getDataObjectID());
            itemChild->setData(2, 0, solution->getDataObjectID());
            itemChild->setData(3, 0, isActive);
        }
    }

    void TreeWidgetSolution::updateTreeWidgetInitialField(QTreeWidgetItem* itemParent, Radioss::FITKRadiossSolution* solution)
    {
        // 检查参数
        if (itemParent == nullptr || solution == nullptr) return;

        // 获取管理器
        Radioss::FITKInitialFieldManager* manager = solution->getInitialFieldManager();
        if (manager == nullptr) return;

        // 遍历数据管理器
        for (int i = 0; i < manager->getDataCount(); i++)
        {
            // 获取数据对象
            Radioss::FITKAbstractInitialField* data = manager->getDataByIndex(i);
            if (data == nullptr) continue;

            // 获取关键字
            QString keyword = getRadiossKeyWord(data), itemName = QString();
            // 获取节点名称
            if (keyword.isEmpty()) itemName = data->getDataObjectName();
            else itemName = QString("%1[%2]").arg(data->getDataObjectName()).arg(getRadiossKeyWord(data));
            if (itemName.isEmpty()) continue;
            // 激活状态（未激活）
            bool isActive = data->isEnable();
            if (!isActive) itemName += tr("(Inhibit)");
            
            // 创建子节点
            QTreeWidgetItem* itemChild = createChildItem(itemParent, itemName, GUI::TreeWidgetSolutionType::TWSolutionChildInitialConditionInitialField);
            isActive ? setItemChecked(itemChild, 0, data->getVisible()) : setItemChecked(itemChild, 0, false);
            isActive ? show(data->getDataObjectID(), data->getVisible()) : show(data->getDataObjectID(), false);
            setObjectID(itemChild, data->getDataObjectID());
            itemChild->setData(2, 0, solution->getDataObjectID());
            itemChild->setData(3, 0, isActive);
        }
    }

    void TreeWidgetSolution::updateTreeWidgetRigidWall(QTreeWidgetItem* itemParent, Radioss::FITKRadiossSolution* solution)
    {
        // 检查参数
        if (itemParent == nullptr || solution == nullptr) return;

        // 获取管理器
        Radioss::FITKRadiossRWallManager* manager = solution->getRWallManager();
        if (manager == nullptr) return;

        // 遍历数据管理器
        for (int i = 0; i < manager->getDataCount(); i++)
        {
            // 获取数据对象
            Radioss::FITKAbstractRWall* data = manager->getDataByIndex(i);
            if (data == nullptr) continue;

            // 获取关键字
            QString keyword = getRadiossKeyWord(data), itemName = QString();
            // 获取节点名称
            if (keyword.isEmpty()) itemName = data->getDataObjectName();
            else itemName = QString("%1[%2]").arg(data->getDataObjectName()).arg(getRadiossKeyWord(data));
            if (itemName.isEmpty()) continue;
            // 激活状态（未激活）
            bool isActive = data->isEnable();
            if (!isActive) itemName += tr("(Inhibit)");
            
            // 创建子节点
            QTreeWidgetItem* itemChild = createChildItem(itemParent, itemName, GUI::TreeWidgetSolutionType::TWSolutionChildConstraintRigidWall);
            isActive ? setItemChecked(itemChild, 0, data->getVisible()) : setItemChecked(itemChild, 0, false);
            isActive ? show(data->getDataObjectID(), data->getVisible()) : show(data->getDataObjectID(), false);
            setObjectID(itemChild, data->getDataObjectID());
            itemChild->setData(2, 0, solution->getDataObjectID());
            itemChild->setData(3, 0, isActive);
        }
    }

    void TreeWidgetSolution::updateTreeWidgetGravity(QTreeWidgetItem* itemParent, Radioss::FITKRadiossSolution* solution)
    {
        // 检查参数
        if (itemParent == nullptr || solution == nullptr) return;

        // 获取管理器
        Radioss::FITKGravityManager* manager = solution->getGravityManager();
        if (manager == nullptr) return;

        // 遍历数据管理器
        for (int i = 0; i < manager->getDataCount(); i++)
        {
            // 获取数据对象
            Radioss::FITKGravity* data = manager->getDataByIndex(i);
            if (data == nullptr) continue;

            // 获取关键字
            QString keyword = getRadiossKeyWord(data), itemName = QString();
            // 获取节点名称
            if (keyword.isEmpty()) itemName = data->getDataObjectName();
            else itemName = QString("%1[%2]").arg(data->getDataObjectName()).arg(getRadiossKeyWord(data));
            if (itemName.isEmpty()) continue;
            // 激活状态（未激活）
            bool isActive = data->isEnable();
            if (!isActive) itemName += tr("(Inhibit)");
            
            // 创建子节点
            QTreeWidgetItem* itemChild = createChildItem(itemParent, itemName, GUI::TreeWidgetSolutionType::TWSolutionChildConstraintGravity);
            isActive ? setItemChecked(itemChild, 0, data->getVisible()) : setItemChecked(itemChild, 0, false);
            isActive ? show(data->getDataObjectID(), data->getVisible()) : show(data->getDataObjectID(), false);
            setObjectID(itemChild, data->getDataObjectID());
            itemChild->setData(2, 0, solution->getDataObjectID());
            itemChild->setData(3, 0, isActive);
        }
    }

    void TreeWidgetSolution::updateTreeWidgetBCS(QTreeWidgetItem * itemParent, Radioss::FITKRadiossSolution * solution)
    {
        // 检查参数
        if (itemParent == nullptr || solution == nullptr) return;

        // 获取管理器
        Radioss::FITKRadiossBCSManager* manager = solution->getBCSManager();
        if (manager == nullptr) return;

        // 遍历数据管理器
        for (int i = 0; i < manager->getDataCount(); i++)
        {
            // 获取数据对象
            Radioss::FITKAbstractBCS* data = manager->getDataByIndex(i);
            if (data == nullptr) continue;

            // 获取关键字
            QString keyword = getRadiossKeyWord(data), itemName = QString();
            // 获取节点名称
            if (keyword.isEmpty()) itemName = data->getDataObjectName();
            else itemName = QString("%1[%2]").arg(data->getDataObjectName()).arg(getRadiossKeyWord(data));
            if (itemName.isEmpty()) continue;
            // 激活状态（未激活）
            bool isActive = data->isEnable();
            if (!isActive) itemName += tr("(Inhibit)");

            // 创建子节点
            QTreeWidgetItem* itemChild = createChildItem(itemParent, itemName, GUI::TreeWidgetSolutionType::TWSolutionChildBoundaryBCS);
            isActive ? setItemChecked(itemChild, 0, data->getVisible()) : setItemChecked(itemChild, 0, false);
            isActive ? show(data->getDataObjectID(), data->getVisible()) : show(data->getDataObjectID(), false);
            setObjectID(itemChild, data->getDataObjectID());
            itemChild->setData(2, 0, solution->getDataObjectID());
            itemChild->setData(3, 0, isActive);
        }
    }

    void TreeWidgetSolution::updateTreeWidgetConnection(QTreeWidgetItem * itemParent, Radioss::FITKRadiossSolution * solution)
    {
        // 检查参数
        if (itemParent == nullptr || solution == nullptr) return;

        // 获取管理器
        Radioss::FITKConnectionManager* manager = solution->getConnectionManager();
        if (manager == nullptr) return;

        // 遍历数据管理器
        for (int i = 0; i < manager->getDataCount(); i++)
        {
            // 获取数据对象
            Radioss::FITKAbstractConnection* data = manager->getDataByIndex(i);
            if (data == nullptr) continue;

            // 获取关键字
            QString keyword = getRadiossKeyWord(data), itemName = QString();
            // 获取节点名称
            if (keyword.isEmpty()) itemName = data->getDataObjectName();
            else itemName = QString("%1[%2]").arg(data->getDataObjectName()).arg(getRadiossKeyWord(data));
            if (itemName.isEmpty()) continue;
            // 激活状态（未激活）
            bool isActive = data->isEnable();
            if (!isActive) itemName += tr("(Inhibit)");

            // 创建子节点
            QTreeWidgetItem* itemChild = createChildItem(itemParent, itemName, GUI::TreeWidgetSolutionType::TWSolutionChildConnectionRBody);
            isActive ? setItemChecked(itemChild, 0, data->getVisible()) : setItemChecked(itemChild, 0, false);
            isActive ? show(data->getDataObjectID(), data->getVisible()) : show(data->getDataObjectID(), false);
            setObjectID(itemChild, data->getDataObjectID());
            itemChild->setData(2, 0, solution->getDataObjectID());
            itemChild->setData(3, 0, isActive);
        }
    }

    void TreeWidgetSolution::updateAccel(QTreeWidgetItem * itemParent, Radioss::FITKRadiossSolution * solution)
    {
        // 检查参数
        if (itemParent == nullptr || solution == nullptr) return;

        // 存储当前解决方案ID
        itemParent->setData(2, 0, solution->getDataObjectID());

        // 获取管理器
        Radioss::FITKProbeManager* manager = solution->getProbeManager();
        if (manager == nullptr) return;

        // 遍历数据管理器
        for (int i = 0; i < manager->getDataCount(); i++)
        {
            // 获取数据对象
            Radioss::FITKAbstractProbe* data = manager->getDataByIndex(i);
            if (data == nullptr|| data->getType()!= Radioss::FITKAbstractProbe::ProbeType::Probe_Acceleration) continue;

            // 获取关键字
            QString keyword = getRadiossKeyWord(data), itemName = QString();
            // 获取节点名称
            if (keyword.isEmpty()) itemName = data->getDataObjectName();
            else itemName = QString("%1[%2]").arg(data->getDataObjectName()).arg(getRadiossKeyWord(data));
            if (itemName.isEmpty()) continue;
            // 激活状态（未激活）
            bool isActive = data->isEnable();
            if (!isActive) itemName += tr("(Inhibit)");

            // 创建子节点
            QTreeWidgetItem* itemChild = createChildItem(itemParent, itemName, GUI::TreeWidgetSolutionType::TWSolutionChildProbeAccel);
            isActive ? setItemChecked(itemChild, 0, data->getVisible()) : setItemChecked(itemChild, 0, false);
            isActive ? show(data->getDataObjectID(), data->getVisible()) : show(data->getDataObjectID(), false);
            setObjectID(itemChild, data->getDataObjectID());
            itemChild->setData(2, 0, solution->getDataObjectID());
            itemChild->setData(3, 0, isActive);
        }
    }

    void TreeWidgetSolution::updateSection(QTreeWidgetItem * itemParent, Radioss::FITKRadiossSolution * solution)
    {
        // 检查参数
        if (itemParent == nullptr || solution == nullptr) return;

        // 存储当前解决方案ID
        itemParent->setData(2, 0, solution->getDataObjectID());

        // 更新子节点
          // 获取管理器
        Radioss::FITKProbeManager* manager = solution->getProbeManager();
        if (manager == nullptr) return;

        // 遍历数据管理器
        for (int i = 0; i < manager->getDataCount(); i++)
        {
            // 获取数据对象
            Radioss::FITKAbstractProbe* data = manager->getDataByIndex(i);
            if (data == nullptr || data->getType() != Radioss::FITKAbstractProbe::ProbeType::Probe_Section) continue;

            // 获取关键字
            QString keyword = getRadiossKeyWord(data), itemName = QString();
            // 获取节点名称
            if (keyword.isEmpty()) itemName = data->getDataObjectName();
            else itemName = QString("%1[%2]").arg(data->getDataObjectName()).arg(getRadiossKeyWord(data));
            if (itemName.isEmpty()) continue;
            // 激活状态（未激活）
            bool isActive = data->isEnable();
            if (!isActive) itemName += tr("(Inhibit)");

            // 创建子节点
            QTreeWidgetItem* itemChild = createChildItem(itemParent, itemName, GUI::TreeWidgetSolutionType::TWSolutionChildProbeSection);
            isActive ? setItemChecked(itemChild, 0, data->getVisible()) : setItemChecked(itemChild, 0, false);
            isActive ? show(data->getDataObjectID(), data->getVisible()) : show(data->getDataObjectID(), false);
            setObjectID(itemChild, data->getDataObjectID());
            itemChild->setData(2, 0, solution->getDataObjectID());
            itemChild->setData(3, 0, isActive);
        }
    }

    QString TreeWidgetSolution::getRadiossKeyWord(Core::FITKAbstractNDataObject* data)
    {
        // Empty
        if (data == nullptr) return QString();
        // KinematicTied
        else if (dynamic_cast<Radioss::FITKInteractionKinematicTied*>(data)) return Radioss::FITKInteractionKinematicTied::GetFITKInteractionKinematicTiedRadiossKeyWord();
        // MultiUsageImpact
        else if (dynamic_cast<Radioss::FITKInteractionMultiUsageImpact*>(data)) return Radioss::FITKInteractionMultiUsageImpact::GetFITKInteractionMultiUsageImpactRadiossKeyWord();
        // Initial Field
        else if (dynamic_cast<Radioss::FITKInitialFieldTra*>(data)) return Radioss::FITKInitialFieldTra::GetFITKInitialFieldTraRadiossKeyWord();
        else if (dynamic_cast<Radioss::FITKInitialFieldAxis*>(data)) return Radioss::FITKInitialFieldAxis::GetFITKInitialFieldAxisRadiossKeyWord();
        // RigidWall
        else if (dynamic_cast<Radioss::FITKRWallPlane*>(data)) return Radioss::FITKRWallPlane::GetFITKRWallPlaneRadiossKeyWord();
        else if (dynamic_cast<Radioss::FITKRWallCylinder*>(data)) return Radioss::FITKRWallCylinder::GetFITKRWallCylinderRadiossKeyWord();
        // Gravity
        else if (dynamic_cast<Radioss::FITKGravity*>(data)) return Radioss::FITKGravity::GetFITKGravityRadiossKeyWord();
        else if (dynamic_cast<Radioss::FITKRadiossBCS*>(data)) return Radioss::FITKRadiossBCS::GetFITKRadiossBCSRadiossKeyWord();
        //Connection
        else if (dynamic_cast<Radioss::FITKConnectionRBODY*>(data)) return Radioss::FITKConnectionRBODY::GetFITKConnectionRBODYRadiossKeyWord();
        // probe 
        else if (dynamic_cast<Radioss::FITKProbeAcceleration*>(data)) return Radioss::FITKProbeAcceleration::GetFITKProbeAccelerationRadiossKeyWord();
        else if (dynamic_cast<Radioss::FITKProbeSection*>(data)) return Radioss::FITKProbeSection::GetFITKProbeSectionRadiossKeyWord();
        else return QString();
    }
}