/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file    FITKOFFoamToVTKDriver
 * @brief   FoamToVTK求解器区驱动类
 * @author  fulipeng (fulipengqd@yeah.net)
 * @date    2024-07-29
 *
 */
#ifndef FITKOFFOAMTOVTKDRIVER_H
#define FITKOFFOAMTOVTKDRIVER_H


#include "FITK_Kernel/FITKAppFramework/FITKAbstractProgramDriver.h"
#include "FITK_Kernel/FITKAppFramework/FITKProgramDriverFactory.h"
#include "FITKOpenFoamMeshingDriverBase.h"

namespace AppFrame
{
    class FITKProgramInputInfo;
}

namespace FoamDriver
{

    class FITKOFFoamToVTKDriver : public FITKOpenFoamMeshingDriverBase
    {
        Q_OBJECT
    public:
        explicit FITKOFFoamToVTKDriver() = default;
        virtual ~FITKOFFoamToVTKDriver() = default;

        /**
         * @brief   获取blockMesh程序类型
         * @return  1
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-07-29
         */
        int getProgramType() override;

        /**
         * @brief   获取程序名称
         * @return  “blockMesh”
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-07-29
         */
        QString getProgramName() override;

        /**
         * @brief   启动blockMesh网格划分程序
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-07-29
         */
        void start() override;

        /**
         * @brief   终止程序
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-07-18
         */
        void stop() override;
    };
    Register2FITKProgramDriverFactory(2, FITKOFFoamToVTKDriver, FITKOFFoamToVTKDriver)
}

#endif // FITKOFFoamToVTKDriver_H
