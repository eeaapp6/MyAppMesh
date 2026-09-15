/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file    OperTreeSolutionAction.h
 * @brief   解决方案树形菜单动作操作器
 * @author  lilongyuan(lilongyuan@diso.cn)
 * @date    2025-09-18
 *********************************************************************/
#ifndef __OPER_TREE_SOLUTION_ACTION_H__
#define __OPER_TREE_SOLUTION_ACTION_H__

#include "OperatorsGUI/OperTreeAction.h"

namespace GUIOper
{
    /**
     * @brief   解决方案树形菜单动作操作器
     * @author  lilongyuan(lilongyuan@diso.cn)
     * @date    2025-09-18
     */
    class OperTreeSolutionAction :public OperTreeAction
    {
    public:
        /**
         * @brief   构造函数
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-18
         */
        explicit OperTreeSolutionAction() = default;
        /**
         * @brief   析构函数
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-18
         */
        virtual ~OperTreeSolutionAction();
        /**
         * @brief   界面逻辑，生成UI交互
         * @return  bool 执行结果
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-18
         */
        virtual bool execGUI();
    protected:
        /**
         * @brief   参数预处理
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-18
         */
        virtual void preArgs();
    private:
        /**
         * @brief   当前选中节点的数据ID
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-18
         */
        int _selectedItemDataID = -1;
    };
    // Solution
    Register2FITKOperatorRepo(actionSolutionTreeCreateSolution, OperTreeSolutionAction);
    Register2FITKOperatorRepo(actionSolutionTreeRenameSolution, OperTreeSolutionAction);
    Register2FITKOperatorRepo(actionSolutionTreeDeleteSolution, OperTreeSolutionAction);
    // Solver Setting
    Register2FITKOperatorRepo(actionSolutionTreeEditSolverSetting, OperTreeSolutionAction);
    // Result Request
    Register2FITKOperatorRepo(actionSolutionTreeEditResultRequest, OperTreeSolutionAction);
    // Interaction
    Register2FITKOperatorRepo(actionSolutionTreeCreateKinematicTied, OperTreeSolutionAction);
    Register2FITKOperatorRepo(actionSolutionTreeCreateMultiUsageImpact, OperTreeSolutionAction);
    Register2FITKOperatorRepo(actionSolutionTreeChangeInteractionActivationStatus, OperTreeSolutionAction);
    Register2FITKOperatorRepo(actionSolutionTreeRenameInteraction, OperTreeSolutionAction);
    Register2FITKOperatorRepo(actionSolutionTreeEditKinematicTied, OperTreeSolutionAction);
    Register2FITKOperatorRepo(actionSolutionTreeEditMultiUsageImpact, OperTreeSolutionAction);
    Register2FITKOperatorRepo(actionSolutionTreeDeleteInteraction, OperTreeSolutionAction);
    // Initial Field
    Register2FITKOperatorRepo(actionSolutionTreeCreateInitialField, OperTreeSolutionAction);
    Register2FITKOperatorRepo(actionSolutionTreeChangeInitialFieldActivationStatus, OperTreeSolutionAction);
    Register2FITKOperatorRepo(actionSolutionTreeRenameInitialField, OperTreeSolutionAction);
    Register2FITKOperatorRepo(actionSolutionTreeEditInitialField, OperTreeSolutionAction);
    Register2FITKOperatorRepo(actionSolutionTreeDeleteInitialField, OperTreeSolutionAction);
    // Rigid Wall
    Register2FITKOperatorRepo(actionSolutionTreeCreateRigidWall, OperTreeSolutionAction);
    Register2FITKOperatorRepo(actionSolutionTreeChangeRigidWallActivationStatus, OperTreeSolutionAction);
    Register2FITKOperatorRepo(actionSolutionTreeRenameRigidWall, OperTreeSolutionAction);
    Register2FITKOperatorRepo(actionSolutionTreeEditRigidWall, OperTreeSolutionAction);
    Register2FITKOperatorRepo(actionSolutionTreeDeleteRigidWall, OperTreeSolutionAction);
    // Gravity
    Register2FITKOperatorRepo(actionSolutionTreeCreateGravity, OperTreeSolutionAction);
    Register2FITKOperatorRepo(actionSolutionTreeChangeGravityActivationStatus, OperTreeSolutionAction);
    Register2FITKOperatorRepo(actionSolutionTreeRenameGravity, OperTreeSolutionAction);
    Register2FITKOperatorRepo(actionSolutionTreeEditGravity, OperTreeSolutionAction);
    Register2FITKOperatorRepo(actionSolutionTreeDeleteGravity, OperTreeSolutionAction);
    // Boundary
    Register2FITKOperatorRepo(actionSolutionTreeCreateBCS, OperTreeSolutionAction);
    Register2FITKOperatorRepo(actionSolutionTreeChangeBCSActivationStatus, OperTreeSolutionAction);
    Register2FITKOperatorRepo(actionSolutionTreeRenameBCS, OperTreeSolutionAction);
    Register2FITKOperatorRepo(actionSolutionTreeEditBCS, OperTreeSolutionAction);
    Register2FITKOperatorRepo(actionSolutionTreeDeleteBCS, OperTreeSolutionAction);
    // Rigid Body
    Register2FITKOperatorRepo(actionSolutionTreeCreateRBody, OperTreeSolutionAction);
    Register2FITKOperatorRepo(actionSolutionTreeChangeRBodyActivationStatus, OperTreeSolutionAction);
    Register2FITKOperatorRepo(actionSolutionTreeRenameRBody, OperTreeSolutionAction);
    Register2FITKOperatorRepo(actionSolutionTreeEditRBody, OperTreeSolutionAction);
    Register2FITKOperatorRepo(actionSolutionTreeDeleteRBody, OperTreeSolutionAction);
    // probe
    Register2FITKOperatorRepo(actionSolutionTreeCreateProbeAccel, OperTreeSolutionAction);
    Register2FITKOperatorRepo(actionSolutionTreeChangeProbeAccelActivationStatus, OperTreeSolutionAction);
    Register2FITKOperatorRepo(actionSolutionTreeRenameProbeAccel, OperTreeSolutionAction);
    Register2FITKOperatorRepo(actionSolutionTreeEditProbeAccel, OperTreeSolutionAction);
    Register2FITKOperatorRepo(actionSolutionTreeDeleteProbeAccel, OperTreeSolutionAction);

    Register2FITKOperatorRepo(actionSolutionTreeCreateProbeSection, OperTreeSolutionAction);
    Register2FITKOperatorRepo(actionSolutionTreeChangeProbeSectionActivationStatus, OperTreeSolutionAction);
    Register2FITKOperatorRepo(actionSolutionTreeRenameProbeSection, OperTreeSolutionAction);
    Register2FITKOperatorRepo(actionSolutionTreeEditProbeSection, OperTreeSolutionAction);
    Register2FITKOperatorRepo(actionSolutionTreeDeleteProbeSection, OperTreeSolutionAction);
}
#endif  // __OPER_TREE_SOLUTION_ACTION_H__