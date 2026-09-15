/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   OperOpenHDF5.h
 * @brief  导入工程文件文件操作器
 * @author guqingtao (15598887859@163.com)
 * @date   2025-09-28
 *********************************************************************/
#ifndef __OperOpenHDF5_H__
#define __OperOpenHDF5_H__

#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"

#include <QString>

namespace OperModel
{
    /**
     * @brief  打开HDF5按钮操作
     * @author guqingtao (15598887859@163.com)
     * @date   2025-09-28
     */
    class OperOpenHDF5 : public Core::FITKActionOperator
    {
        Q_OBJECT
    public:
         
        explicit OperOpenHDF5() = default;
        ~OperOpenHDF5() = default;
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

    Register2FITKOperatorRepo(actionOpen, OperOpenHDF5);

    class ReadRadiossIOHDF5 : public IO::FITKAbstractHDF5Reader
    {
        Q_OBJECT
    public:

        explicit ReadRadiossIOHDF5() = default;

        virtual ~ReadRadiossIOHDF5() = default;
        /**
         * @brief    打印控制台消息
         * @param[i] level 
         * @param[i] str 
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-10-11
         */
        virtual void consoleMessage(int level, const QString& str) override;

    private:

        void run() override;

    };
}

#endif // !__MODELOPERATORS_INMPORT_INP_H__
