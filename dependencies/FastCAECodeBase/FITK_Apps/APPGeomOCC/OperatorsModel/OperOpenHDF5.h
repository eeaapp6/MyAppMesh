/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file OperOpenHDF5.h
 * @brief 导入工程文件文件操作器
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-04-14
 *
 */
#ifndef __MODELOPERATORS_OPENHDF5_H__
#define __MODELOPERATORS_OPENHDF5_H__

#include "OperManagerBase.h"

#include <QString>

namespace ModelOper
{

    /**
     * @brief 打开HDF5按钮操作
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2023-03-28
     */
    class OperOpenHDF5 : public OperManagerBase
    {
        Q_OBJECT
    public:
         
        explicit OperOpenHDF5() = default;
       
        ~OperOpenHDF5() = default;

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

    Register2FITKOperatorRepo(actionOpenProject, OperOpenHDF5);
}




#endif // !__MODELOPERATORS_INMPORT_INP_H__
