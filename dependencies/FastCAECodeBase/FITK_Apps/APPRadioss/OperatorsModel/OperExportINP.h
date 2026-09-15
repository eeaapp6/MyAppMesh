/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file OperExportINP.h
 * @brief 导出inp文件操作器
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-04-14
 *
 */
#ifndef __OPER_EXPORT_INP_H___
#define __OPER_EXPORT_INP_H___

#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"

namespace Interface
{
    class FITKExportFileThread;
}

namespace OperModel
{
    /**
     * @brief 导出inpw文件操作器
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-04-14
     */
    class OperExportINP : public Core::FITKActionOperator
    {
        Q_OBJECT
    public:
        explicit OperExportINP() = default;
        virtual ~OperExportINP() = default;

    private slots:
        /**
         * @brief 读取线程结束执行该槽函数
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-26
         */
        void ioThreadFinishedSlot();

        /**
         * @brief   part读取线程结束执行该槽函数
         * @author  fulipeng (flipengqd@yeah.net)
         * @date    2025-01-16
         */
        void slot_geometryPartsIOFinished(bool status);

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

    private:

        /**
         * @brief   几何部件IO线程。
         * @author  fulipeng (flipengqd@yeah.net)
         * @date    2025-01-16
         */
        Interface::FITKExportFileThread* m_partsIO{};


    };

    Register2FITKOperatorRepo(actionExportAsParts, OperExportINP);
    Register2FITKOperatorRepo(actionExportAsModel, OperExportINP);
}



#endif // !__OPER_EXPORT_INP_H___
