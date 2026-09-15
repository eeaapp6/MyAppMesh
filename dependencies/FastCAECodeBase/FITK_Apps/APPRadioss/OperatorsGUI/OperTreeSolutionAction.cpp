/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperTreeSolutionAction.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSolution.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractInteraction.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractInitialField.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractRWall.h"
#include "FITK_Component/FITKRadiossData/FITKGravity.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractBCS.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractConnection.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractProbe.h"
#include <QTreeWidgetItem>

namespace GUIOper
{
    OperTreeSolutionAction::~OperTreeSolutionAction()
    {
        // 重置选中节点的数据ID
        _selectedItemDataID = -1;
    }

    bool OperTreeSolutionAction::execGUI()
    {
        // 检查触发器是否为空，如果为空则返回false
        if (_emitter == nullptr) return false;

        // 获取触发器的对象名称
        QString name = _emitter->objectName();
        if (name.isEmpty()) return false;

        // 获取Radioss案例数据对象
        Radioss::FITKRadiossCase* dataCase = getRadiossCase();
        if (dataCase == nullptr) return false;

        // 初始化参数哈希表
        QHash<QString, QVariant> args = QHash<QString, QVariant>();
        args.insert("SelectedItemDataID", _selectedItemDataID);
        
        // 根据不同的对象名称调用相应的操作函数
        // Solution
        if (name == "actionSolutionTreeCreateSolution") callOperator("actionSolutionCreate");
        else if (name == "actionSolutionTreeRenameSolution") renameData<Radioss::FITKRadiossSolutionManager*>(this, dataCase->getSolutionManager(), _selectedItemDataID);
        else if (name == "actionSolutionTreeDeleteSolution") deleteData(dataCase->getSolutionManager(), _selectedItemDataID);
        // Solver Setting
        else if (name == "actionSolutionTreeEditSolverSetting") callOperator("actionSolverSettings", args);
        // Result Request
        else if (name == "actionSolutionTreeEditResultRequest") callOperator("actionResultRequest", args);
        // Interaction
        else if (name == "actionSolutionTreeCreateKinematicTied") callOperator("actionContactTieCreate");
        else if (name == "actionSolutionTreeCreateMultiUsageImpact") callOperator("actionContactMultiUsageImpactCreate");
        else if (name == "actionSolutionTreeChangeInteractionActivationStatus") changeActivationStatus<Radioss::FITKInteractionManager*>(dataCase->getInteractionManager(), _selectedItemDataID);
        else if (name == "actionSolutionTreeRenameInteraction") renameData<Radioss::FITKInteractionManager*>(this, dataCase->getInteractionManager(), _selectedItemDataID);
        else if (name == "actionSolutionTreeEditKinematicTied") callOperator("actionContactTieEdit", args);
        else if (name == "actionSolutionTreeEditMultiUsageImpact") callOperator("actionContactMultiUsageImpactEdit", args);
        else if (name == "actionSolutionTreeDeleteInteraction") deleteData(dataCase->getInteractionManager(), _selectedItemDataID);
        // Initial Field
        else if (name == "actionSolutionTreeCreateInitialField") callOperator("actionInitialFieldCreate");
        else if (name == "actionSolutionTreeChangeInitialFieldActivationStatus") changeActivationStatus<Radioss::FITKInitialFieldManager*>(dataCase->getInitialFieldManager(), _selectedItemDataID);
        else if (name == "actionSolutionTreeRenameInitialField") renameData<Radioss::FITKInitialFieldManager*>(this, dataCase->getInitialFieldManager(), _selectedItemDataID);
        else if (name == "actionSolutionTreeEditInitialField") callOperator("actionInitialFieldEdit", args);
        else if (name == "actionSolutionTreeDeleteInitialField") deleteData(dataCase->getInitialFieldManager(), _selectedItemDataID);
        // Rigid Wall
        else if (name == "actionSolutionTreeCreateRigidWall") callOperator("actionRigidWallCreate");
        else if (name == "actionSolutionTreeChangeRigidWallActivationStatus") changeActivationStatus<Radioss::FITKRadiossRWallManager*>(dataCase->getRWallManager(), _selectedItemDataID);
        else if (name == "actionSolutionTreeRenameRigidWall") renameData<Radioss::FITKRadiossRWallManager*>(this, dataCase->getRWallManager(), _selectedItemDataID);
        else if (name == "actionSolutionTreeEditRigidWall") callOperator("actionRigidWallEdit", args);
        else if (name == "actionSolutionTreeDeleteRigidWall") deleteData(dataCase->getRWallManager(), _selectedItemDataID);
        // Gravity
        else if (name == "actionSolutionTreeCreateGravity") callOperator("actionGravityCreate");
        else if (name == "actionSolutionTreeChangeGravityActivationStatus") changeActivationStatus<Radioss::FITKGravityManager*>(dataCase->getGravityManager(), _selectedItemDataID);
        else if (name == "actionSolutionTreeRenameGravity") renameData<Radioss::FITKGravityManager*>(this, dataCase->getGravityManager(), _selectedItemDataID);
        else if (name == "actionSolutionTreeEditGravity") callOperator("actionGravityEdit", args);
        else if (name == "actionSolutionTreeDeleteGravity") deleteData(dataCase->getGravityManager(), _selectedItemDataID);
        // Boundary
        else if (name == "actionSolutionTreeCreateBCS") callOperator("actionBCSCreate");
        else if (name == "actionSolutionTreeChangeBCSActivationStatus") changeActivationStatus<Radioss::FITKRadiossBCSManager*>(dataCase->getBCSManager(), _selectedItemDataID);
        else if (name == "actionSolutionTreeRenameBCS") renameData<Radioss::FITKRadiossBCSManager*>(this, dataCase->getBCSManager(), _selectedItemDataID);
        else if (name == "actionSolutionTreeEditBCS") callOperator("actionBCSEdit", args);
        else if (name == "actionSolutionTreeDeleteBCS") deleteData(dataCase->getBCSManager(), _selectedItemDataID);
        // Rigid Body
        else if (name == "actionSolutionTreeCreateRBody") callOperator("actionRBodyCreate");
        else if (name == "actionSolutionTreeChangeRBodyActivationStatus") changeActivationStatus<Radioss::FITKConnectionManager*>(dataCase->getConnectionManager(), _selectedItemDataID);
        else if (name == "actionSolutionTreeRenameRBody") renameData<Radioss::FITKConnectionManager*>(this, dataCase->getConnectionManager(), _selectedItemDataID);
        else if (name == "actionSolutionTreeEditRBody") callOperator("actionRBodyEdit", args);
        else if (name == "actionSolutionTreeDeleteRBody") deleteData(dataCase->getConnectionManager(), _selectedItemDataID);
        // probe
        else if (name == "actionSolutionTreeCreateProbeAccel") callOperator("actionCreateAcceleration");
        else if (name == "actionSolutionTreeChangeProbeAccelActivationStatus") changeActivationStatus<Radioss::FITKProbeManager*>(dataCase->getCurrentSolution()->getProbeManager(), _selectedItemDataID);
        else if (name == "actionSolutionTreeRenameProbeAccel") renameData<Radioss::FITKProbeManager*>(this, dataCase->getCurrentSolution()->getProbeManager(), _selectedItemDataID);
        else if (name == "actionSolutionTreeEditProbeAccel") callOperator("actionEditAcceleration", args);
        else if (name == "actionSolutionTreeDeleteProbeAccel") deleteData(dataCase->getCurrentSolution()->getProbeManager(), _selectedItemDataID);
        else if (name == "actionSolutionTreeCreateProbeSection") callOperator("actionCreateSection");
        else if (name == "actionSolutionTreeChangeProbeSectionActivationStatus") changeActivationStatus<Radioss::FITKProbeManager*>(dataCase->getCurrentSolution()->getProbeManager(), _selectedItemDataID);
        else if (name == "actionSolutionTreeRenameProbeSection") renameData<Radioss::FITKProbeManager*>(this, dataCase->getCurrentSolution()->getProbeManager(), _selectedItemDataID);
        else if (name == "actionSolutionTreeEditProbeSection") callOperator("actionEditSection", args);
        else if (name == "actionSolutionTreeDeleteProbeSection") deleteData(dataCase->getCurrentSolution()->getProbeManager(), _selectedItemDataID);
        else return false;

        // 清除高亮显示
        clearAllHighLight();

        return true;
    }

    void OperTreeSolutionAction::preArgs()
    {
        // 初始化选中节点的数据ID
        _selectedItemDataID = -1;

        // 获取选中项列表参数
        QVariant selectedItems;
        this->argValue("SelectedItems", selectedItems);
        if (selectedItems.isNull()) return;

        // 将参数转换为TreeWidgetItem列表指针并检查有效性
        QList<QTreeWidgetItem*>* listSelectedItems = static_cast<QList<QTreeWidgetItem*>*>(selectedItems.value<void*>());
        if (listSelectedItems == nullptr || listSelectedItems->isEmpty()) return;

        // 获取第一个选中的项的ID
        _selectedItemDataID = listSelectedItems->first()->data(1, 0).toInt();


        // 获取Radioss案例数据对象
        Radioss::FITKRadiossCase* dataCase = getRadiossCase();
        if (dataCase == nullptr) return;

        // 获取解决方案管理器
        Radioss::FITKRadiossSolutionManager* managerSolution = dataCase->getSolutionManager();
        if (managerSolution == nullptr) return;

        // 设置当前解决方案
        managerSolution->setCurrentSolution(listSelectedItems->first()->data(2, 0).toInt());
    }
}