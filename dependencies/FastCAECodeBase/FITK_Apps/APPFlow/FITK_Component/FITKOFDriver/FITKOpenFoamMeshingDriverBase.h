/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file    FITKOpenFoamMeshingDriverBase
 * @brief   OpenFoam网格划分父类
 * @author  fulipeng (fulipengqd@yeah.net)
 * @date    2024-07-18
 *
 */
#ifndef FITKOPENFOAMMESHINGDRIVERBASE_H
#define FITKOPENFOAMMESHINGDRIVERBASE_H

#include "FITK_Kernel/FITKAppFramework/FITKLinuxShellDriver.h"
#include "FITK_Kernel/FITKAppFramework/FITKProgramDriverFactory.h"

namespace FoamDriver
{

    class FITKOpenFoamMeshingDriverBase : public AppFrame::FITKLinuxShellDriver
    {
        Q_OBJECT
    public:
        explicit FITKOpenFoamMeshingDriverBase() = default;
        virtual ~FITKOpenFoamMeshingDriverBase();


    protected slots:
        /**
         * @brief   线程执行结束
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-07-29
         */
        void threadFinishedSlot();
    };
}
#endif // FITKOFBLOCKMESHDRIVER_H
