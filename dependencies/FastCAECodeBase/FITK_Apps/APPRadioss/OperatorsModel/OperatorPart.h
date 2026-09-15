/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   OperatorPart.h
 * @brief  部件操作器
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-08-22
 *********************************************************************/
#ifndef __OPERATOR_PROPERTY_H__
#define __OPERATOR_PROPERTY_H__

#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"

namespace OperModel
{
    /**
     * @brief  部件操作器类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-08-22
     */
    class OperatorPart :public Core::FITKActionOperator
    {
    public:
        /**
         * @brief    构造函数
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-22
         */
        explicit OperatorPart() = default;
        /**
         * @brief    析构函数
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-22
         */
        virtual ~OperatorPart() = default;
        /**
         * @brief    GUI执行
         * @return   bool 执行结果
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-22
         */
        virtual bool execGUI();
        /**
         * @brief    业务逻辑执行
         * @return   bool 执行结果
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-22
         */
        virtual bool execProfession();

    private:
        /**
         * @brief    部件创建操作
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-22
         */
        bool partCreateOper();
        /**
         * @brief    部件重命名操作
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-22
         */
        bool partRenameOper();
        /**
         * @brief    部件删除操作
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-22
         */
        bool partDeleteOper();
    };
    // 按钮注册相关操作
    Register2FITKOperatorRepo(actionPartCreate, OperatorPart);
    Register2FITKOperatorRepo(actionPartRename, OperatorPart);
    Register2FITKOperatorRepo(actionPartDelete, OperatorPart);
}
#endif  // __OPERATOR_PROPERTY_H__