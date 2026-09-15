/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKLinuxShellDriver.h
 * @brief 第三方程序驱动抽象类声明
 * @author libaojun (libaojunqd@Foxmail.com)
 * @date 2024-06-09
 *
 */
#ifndef  __FITK_LINUXSHELL_DRIVER_H__
#define  __FITK_LINUXSHELL_DRIVER_H__

#include "FITKAppFrameworkAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"
#include "FITKAbstractProgramDriver.h"


namespace AppFrame
{
    class FITKAbstractCommandRunner;
    /**
     * @brief 第三方程序driver抽象类
     * @author libaojun (libaojunqd@Foxmail.com)
     * @date 2024-06-09
     */
    class FITKAppFrameworkAPI FITKLinuxShellDriver : public FITKAbstractProgramerDriver
    {
        Q_OBJECT
    public:
        /**
         * @brief Construct a new FITKLinuxProgramerDriver object
         * @author libaojun (libaojunqd@Foxmail.com)
         * @date 2024-06-09
         */
        explicit FITKLinuxShellDriver();
        /**
         * @brief Destroy the FITKLinuxProgramerDriver object
         * @author libaojun (libaojunqd@Foxmail.com)
         * @date 2024-06-09
         */
        virtual ~FITKLinuxShellDriver();


    private:
        /**
         * @brief   发送输出到控制台
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-07-18
         */
        void sendOutputMessage(const QString &str);

    protected:
        /**
         * @brief   进程ID
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-07-18
         */
        qint64 _processId = -1;
        /**
         * @brief 终端命令执行 子类中实例
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-07-23
         */
        FITKAbstractCommandRunner* _commandRunner{};
    };
}

#endif
