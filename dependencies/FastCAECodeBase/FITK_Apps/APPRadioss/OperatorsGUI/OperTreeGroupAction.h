/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file    OperTreeGroupAction.h
 * @brief   分组树形菜单动作操作器
 * @author  lilongyuan(lilongyuan@diso.cn)
 * @date    2025-09-18
 *********************************************************************/
#ifndef __OPER_TREE_GROUP_ACTION_H__
#define __OPER_TREE_GROUP_ACTION_H__

#include "OperatorsGUI/OperTreeAction.h"

namespace GUIOper
{
    /**
     * @brief   分组树形菜单动作操作器
     * @author  lilongyuan(lilongyuan@diso.cn)
     * @date    2025-09-18
     */
    class  OperTreeGroupAction : public OperTreeAction
    {
    public:
        /**
         * @brief   构造函数
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-18
         */
        explicit OperTreeGroupAction() = default;
        /**
         * @brief   析构函数
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-18
         */
        virtual ~OperTreeGroupAction();
        /**
         * @brief   界面逻辑，生成UI交互
         * @return  执行结果
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
    //注册事件操作器
    Register2FITKOperatorRepo(actionGroupTreeCreateSetByNodes, OperTreeGroupAction);
    Register2FITKOperatorRepo(actionGroupTreeCreateSetByElement, OperTreeGroupAction);
    Register2FITKOperatorRepo(actionGroupTreeCreateSurfaceByNodes, OperTreeGroupAction);
    Register2FITKOperatorRepo(actionGroupTreeCreateSurfaceByElement, OperTreeGroupAction);
    Register2FITKOperatorRepo(actionGroupTreeRename, OperTreeGroupAction);
    Register2FITKOperatorRepo(actionGroupTreeDelete, OperTreeGroupAction);
}
#endif  // __OPER_TREE_GROUP_ACTION_H__