/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef _OperatorExportLSDyna_H_
#define _OperatorExportLSDyna_H_


#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"

namespace ModelOper
{
    class FITKOperatorExportLSDyna : public Core::FITKActionOperator
    {
        Q_OBJECT
    public:
        explicit FITKOperatorExportLSDyna() = default;
        virtual ~FITKOperatorExportLSDyna() = default;
        /**
         * @brief    执行界面操作
         * @return true     执行成功
         * @return false     执行失败        
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-06-30
         */
         virtual bool execGUI() override;
         /**
          * @brief    执行写出操作
          * @return   bool
          * @author   guqingtao (15598887859@163.com)
          * @date     2025-06-30
          */
         virtual bool execProfession() override;
 
    private slots:
        /**
         * @brief  线程结束槽函数
         * @author guqingtao (15598887859@163.com)
         * @date   2025-06-30
         */
        void ioThreadFinishedSlot();
 
    };
    
    // 按钮注册相关操作
    Register2FITKOperatorRepo(actionExportLSDyna, FITKOperatorExportLSDyna);
}

#endif