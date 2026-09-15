/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   OperatorsSaveAs.h
 * @brief  工程另存为操作器
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-11-21
 *********************************************************************/
#ifndef _OperatorsSaveAs_H
#define _OperatorsSaveAs_H

#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"

namespace ModelOper
{
    /**
     * @brief 工程另存为操作器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-11-21
     */
    class OperatorsSaveAs :public Core::FITKActionOperator
    {
        Q_OBJECT;
    public:
        /**
         * @brief  Construct a new Operators SaveAs object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-21
         */
         explicit OperatorsSaveAs() = default;
         /**
          * @brief  Destroy the Operators SaveAs object
          * @author liuzhonghua (liuzhonghuaszch@163.com)
          * @date   2024-11-21
          */
         virtual ~OperatorsSaveAs() = default;
         /**
          * @brief    UI相关操作
          * @return   bool true:执行成功 false:执行失败
          * @author   liuzhonghua (liuzhonghuaszch@163.com)
          * @date     2025-07-16
          */
         virtual bool execGUI() override;
         /**
          * @brief    执行业务操作
          * @return   bool true:执行成功 false:执行失败
          * @author   liuzhonghua (liuzhonghuaszch@163.com)
          * @date     2025-07-16
          */
         virtual bool execProfession() override;
    };

    // 按钮注册相关操作
    Register2FITKOperatorRepo(actionSaveAs, OperatorsSaveAs);

}

#endif