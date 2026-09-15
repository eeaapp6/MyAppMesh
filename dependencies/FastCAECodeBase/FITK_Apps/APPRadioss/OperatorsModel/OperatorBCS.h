/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   OperatorBCS.h
 * @brief  边界条件操作器
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-10-24
 *********************************************************************/
#ifndef __OPERATOR_BOUNDARY_CONDITIONS_H__
#define __OPERATOR_BOUNDARY_CONDITIONS_H__

#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"

namespace OperModel
{
    /**
     * @brief  边界条件操作器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-10-24
     */
    class OperatorBCS :public Core::FITKActionOperator
    {
    public:
        /**
         * @brief   构造函数
         * @author  liuzhonghua (liuzhonghuaszch@163.com)
         * @date    2025-07-17
         */
        explicit OperatorBCS() = default;
        /**
         * @brief   析构函数
         * @author  liuzhonghua (liuzhonghuaszch@163.com)
         * @date    2025-07-17
         */
        virtual ~OperatorBCS() = default;
        /**
         * @brief   GUI执行
         * @return  bool 执行结果
         * @author  liuzhonghua (liuzhonghuaszch@163.com)
         * @date    2025-07-17
         */
        virtual bool execGUI();
        /**
         * @brief   业务逻辑执行
         * @return  bool 执行结果
         * @author  liuzhonghua (liuzhonghuaszch@163.com)
         * @date    2025-07-17
         */
        virtual bool execProfession();

    protected:

    private:
        /**
         * @brief    边界创建操作
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-24
         */
        bool bcsCreateOper();
        /**
         * @brief    边界重命名
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-24
         */
        bool bcsRenameOper();
        /**
         * @brief    边界编辑操作
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-24
         */
        bool bcsEditOper();
        /**
         * @brief    边界移除
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-24
         */
        bool bcsDeleteOper();
    };
    // 按钮注册相关操作
    Register2FITKOperatorRepo(actionBCSCreate, OperatorBCS);
    Register2FITKOperatorRepo(actionBCSRename, OperatorBCS);
    Register2FITKOperatorRepo(actionBCSEdit, OperatorBCS);
    Register2FITKOperatorRepo(actionBCSDelete, OperatorBCS);
}
#endif  // __OPERATOR_PROPERTY_H__