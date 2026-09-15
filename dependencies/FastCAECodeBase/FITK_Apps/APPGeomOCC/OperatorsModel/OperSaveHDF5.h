/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file OperSaveINP.h
 * @brief 保存工程文件操作器
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-04-14
 *
 */
#ifndef __OPER_SAVE_HDF5_H___
#define __OPER_SAVE_HDF5_H___

#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"

namespace ModelOper
{
    /**
     * @brief 保存工程文件操作器
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-04-14
     */
    class OperSaveHDF5 : public Core::FITKActionOperator
    {
        Q_OBJECT
    public:
        explicit OperSaveHDF5() = default;
        virtual ~OperSaveHDF5() = default;

    private slots:
        /**
         * @brief 读取线程结束执行该槽函数
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-26
         */
        void ioThreadFinishedSlot();

    private:
        /**
         * @brief 选择文件
         * @return true
         * @return false
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-26
         */
        bool execGUI() override;
        /**
         * @brief 执行读取
         * @return true
         * @return false
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-26
         */
        bool execProfession() override;

    };

    Register2FITKOperatorRepo(actionSaveProject, OperSaveHDF5);
}



#endif // !__OPER_EXPORT_INP_H___
