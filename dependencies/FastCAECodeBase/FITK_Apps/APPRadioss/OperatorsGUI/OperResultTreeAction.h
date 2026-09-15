/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   OperResultTreeAction.h
 * @brief  结果树 菜单动作操作器
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-09-16
 *********************************************************************/
#ifndef __OPER_RESULT_TREE_ACTION_H__
#define __OPER_RESULT_TREE_ACTION_H__

#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "OperatorsGUI/OperTreeAction.h"

namespace Core
{
    class FITKAbstractDataManagerPrivate;
}

namespace GUIOper
{
    /**
     * @brief   结果树形菜单动作操作器
     * @author  liuzhonghua (liuzhonghuaszch@163.com)
     * @date    2025-09-16
     */
    class OperatorsResultTreeAction :public OperTreeAction
    {
    public:
        /**
         * @brief   构造函数
         * @author  liuzhonghua (liuzhonghuaszch@163.com)
         * @date    2025-09-16
         */
        explicit OperatorsResultTreeAction() = default;
        /**
         * @brief   析构函数
         * @author  liuzhonghua (liuzhonghuaszch@163.com)
         * @date    2025-09-16
         */
        virtual ~OperatorsResultTreeAction();
        /**
         * @brief   界面逻辑，生成UI交互
         * @return  bool 执行结果
         * @author  liuzhonghua (liuzhonghuaszch@163.com)
         * @date    2025-09-16
         */
        virtual bool execGUI();
        /**
         * @brief   业务处理逻辑，在execGUI后执行
         * @return  bool  执行结果
         * @author  liuzhonghua (liuzhonghuaszch@163.com)
         * @date    2025-09-16
         */
        virtual bool execProfession();
    protected:
        /**
         * @brief    参数预处理
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-10-21
         */
        virtual void preArgs();
    private:
        /**
         * @brief  当前选中节点的数据ID
         * @author guqingtao (15598887859@163.com)
         * @date   2025-10-21
         */
        int _selectedItemDataID = -1;
    };
    // 按钮注册相关操作
    Register2FITKOperatorRepo(actionDelete3DPost, OperatorsResultTreeAction);
    Register2FITKOperatorRepo(actionDelete2DPost, OperatorsResultTreeAction);
    Register2FITKOperatorRepo(actionShowXYPlot, OperatorsResultTreeAction);
}
#endif  // __OPER_PROPERTY_TREE_ACTION_H__