/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   OperSaveHDF5.h
 * @brief  保存工程文件操作器
 * @author guqingtao (15598887859@163.com)
 * @date   2025-09-28
 *********************************************************************/
#ifndef __OPER_SAVE_HDF5_H___
#define __OPER_SAVE_HDF5_H___

#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"

namespace OperModel
{
    /**
     * @brief  保存工程文件操作器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-09-28
     */
    class OperSaveHDF5 : public Core::FITKActionOperator
    {
        Q_OBJECT
    public:
        explicit OperSaveHDF5() = default;
        virtual ~OperSaveHDF5() = default;

    private slots:
        /**
         * @brief  读取线程结束执行该槽函数
         * @author guqingtao (15598887859@163.com)
         * @date   2025-09-28
         */
        void ioThreadFinishedSlot();

    private:
        /**
         * @brief    选择文件
         * @return   bool
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-28
         */
        bool execGUI() override;
        /**
         * @brief    执行读取
         * @return   bool
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-28
         */
        bool execProfession() override;

    };

    Register2FITKOperatorRepo(actionSave, OperSaveHDF5);

    class WriteRadiossIOHDF5 : public IO::FITKAbstractHDF5Writer
    {
        Q_OBJECT
    public:

        explicit WriteRadiossIOHDF5() = default;

        virtual ~WriteRadiossIOHDF5() = default;
        /**
         * @brief    打印控制台消息
         * @param[i] level 打印级别 1 normal 2 warning 3error
         * @param[i] str 数据
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-10-11
         */
        virtual void consoleMessage(int level, const QString& str) override;
        void run() override;
    };
}
#endif // !__OPER_EXPORT_INP_H___
