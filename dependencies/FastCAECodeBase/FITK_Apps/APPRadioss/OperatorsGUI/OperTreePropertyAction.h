/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file    OperTreePropertyAction.h
 * @brief   属性树形菜单动作操作器
 * @author  lilongyuan(lilongyuan@diso.cn)
 * @date    2025-09-18
 *********************************************************************/
#ifndef __OPER_TREE_PROPERTY_ACTION_H__
#define __OPER_TREE_PROPERTY_ACTION_H__

#include "OperatorsGUI/OperTreeAction.h"

namespace GUIOper
{
    /**
     * @brief   属性树形菜单动作操作器
     * @author  lilongyuan(lilongyuan@diso.cn)
     * @date    2025-09-18
     */
    class OperTreePropertyAction :public OperTreeAction
    {
    public:
        /**
         * @brief   构造函数
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-18
         */
        explicit OperTreePropertyAction() = default;
        /**
         * @brief   析构函数
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-18
         */
        virtual ~OperTreePropertyAction();
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
    // Material
    Register2FITKOperatorRepo(actionPropertyTreeCreateMaterial, OperTreePropertyAction);
    Register2FITKOperatorRepo(actionPropertyTreeRenameMaterial, OperTreePropertyAction);
    Register2FITKOperatorRepo(actionPropertyTreeEditMaterial, OperTreePropertyAction);
    Register2FITKOperatorRepo(actionPropertyTreeDeleteMaterial, OperTreePropertyAction);
    // Property
    Register2FITKOperatorRepo(actionPropertyTreeCreateProperty, OperTreePropertyAction);
    Register2FITKOperatorRepo(actionPropertyTreeRenameProperty, OperTreePropertyAction);
    Register2FITKOperatorRepo(actionPropertyTreeEditProperty, OperTreePropertyAction);
    Register2FITKOperatorRepo(actionPropertyTreeDeleteProperty, OperTreePropertyAction);
    // Curve
    Register2FITKOperatorRepo(actionPropertyTreeCreateCurve, OperTreePropertyAction);
    Register2FITKOperatorRepo(actionPropertyTreeRenameCurve, OperTreePropertyAction);
    Register2FITKOperatorRepo(actionPropertyTreeEditCurve, OperTreePropertyAction);
    Register2FITKOperatorRepo(actionPropertyTreeDeleteCurve, OperTreePropertyAction);
    // EOS
    Register2FITKOperatorRepo(actionPropertyTreeCreateEOS, OperTreePropertyAction);
    Register2FITKOperatorRepo(actionPropertyTreeRenameEOS, OperTreePropertyAction);
    Register2FITKOperatorRepo(actionPropertyTreeEditEOS, OperTreePropertyAction);
    Register2FITKOperatorRepo(actionPropertyTreeDeleteEOS, OperTreePropertyAction);
    // Failure Model
    Register2FITKOperatorRepo(actionPropertyTreeCreateFailureModel, OperTreePropertyAction);
    Register2FITKOperatorRepo(actionPropertyTreeRenameFailureModel, OperTreePropertyAction);
    Register2FITKOperatorRepo(actionPropertyTreeEditFailureModel, OperTreePropertyAction);
    Register2FITKOperatorRepo(actionPropertyTreeDeleteFailureModel, OperTreePropertyAction);
}
#endif  // __OPER_TREE_PROPERTY_ACTION_H__