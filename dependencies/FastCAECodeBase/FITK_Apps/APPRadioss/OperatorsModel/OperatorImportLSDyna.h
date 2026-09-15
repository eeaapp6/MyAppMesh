/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef _OperatorImportLSDyna_H_
#define _OperatorImportLSDyna_H_


#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"

namespace ModelOper
{
    class FITKOperatorImportLSDyna : public Core::FITKActionOperator
    {
        Q_OBJECT
    public:
        explicit FITKOperatorImportLSDyna() = default;
        virtual ~FITKOperatorImportLSDyna() = default;
 /**
          * @brief 执行界面操作
          * @return true     执行成功
          * @return false     执行失败
          * @author liuzhonghua (liuzhonghuaszch@163.com)
          * @date   2024-11-21
          */
         virtual bool execGUI() override;
         /**
          * @brief 执行读入操作
          * @return true     执行成功
          * @return false     执行失败
          * @author liuzhonghua (liuzhonghuaszch@163.com)
          * @date   2024-11-21
          */
         virtual bool execProfession() override;
 
    private slots:
        /**
         * @brief 读取线程结束执行该槽函数
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-26
         */
        void ioThreadFinishedSlot();
 
    };
    
    // 按钮注册相关操作
    Register2FITKOperatorRepo(actionImportLSDyna, FITKOperatorImportLSDyna);
}

#endif