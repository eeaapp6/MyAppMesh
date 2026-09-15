/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKOFCreatePatchDriver.h
 * @brief  createPatch 网格边界驱动
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-08-28
 *********************************************************************/
#ifndef FITKOFCREATEPATCHDRIVER_H
#define FITKOFCREATEPATCHDRIVER_H

#include "FITK_Kernel/FITKAppFramework/FITKAbstractProgramDriver.h"
#include "FITK_Kernel/FITKAppFramework/FITKProgramDriverFactory.h"
#include "FITKOpenFoamMeshingDriverBase.h"

namespace AppFrame
{
    class FITKProgramInputInfo;
}

namespace FoamDriver
{
    /**
     * @brief  createPatch驱动
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-28
     */
    class FITKOFCreatePatchDriver : public FITKOpenFoamMeshingDriverBase
    {
        Q_OBJECT
    public:
        explicit FITKOFCreatePatchDriver() = default;
        virtual ~FITKOFCreatePatchDriver() = default;

        /**
         * @brief   获取createPatch程序类型
         * @return  1
         * @author  liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-08-28
         */
        int getProgramType() override;

        /**
         * @brief   获取程序名称
         * @return  "createPatch"
         * @author  liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-08-28
         */
        QString getProgramName() override;

        /**
         * @brief   启动createPatch格划分程序
         * @author  liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-08-28
         */
        void start() override;

        /**
         * @brief   终止程序
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-08-28
         */
        void stop() override;

    };
    Register2FITKProgramDriverFactory(1, FITKOFCreatePatchDriver, FITKOFCreatePatchDriver)
}

#endif // FITKOFCREATEPATCHDRIVER_H
