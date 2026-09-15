/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file    FITKOFFoamDictionaryDriver
 * @brief   foamDiectionary字典查询类
 * @author  fulipeng (fulipengqd@yeah.net)
 * @date    2024-07-04
 *
 */
#ifndef FITKFOAMDICTIONARYDRIVER_H
#define FITKFOAMDICTIONARYDRIVER_H

#include "FITK_Kernel/FITKAppFramework/FITKAbstractProgramDriver.h"
#include "FITK_Kernel/FITKAppFramework/FITKProgramDriverFactory.h"
#include "FITKOpenFoamMeshingDriverBase.h"

namespace AppFrame
{
    class FITKProgramInputInfo;
}

namespace FoamDriver
{

    class FITKOFFoamDictionaryDriver : public FITKOpenFoamMeshingDriverBase
    {
        Q_OBJECT
    public:
        explicit FITKOFFoamDictionaryDriver() = default;
        virtual ~FITKOFFoamDictionaryDriver() = default;

        /**
         * @brief   获取FoamDictionary程序类型
         * @return  1
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-07-02
         */
        int getProgramType() override;

        /**
         * @brief   获取程序名称
         * @return  "foamDictionary"
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-07-02
         */
        QString getProgramName() override;

        /**
         * @brief   获取子域数量
         * @return  int 子域数量
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-07-02
         */
        int getSubdomainsNumber();

        /**
         * @brief   启动blockMesh网格划分程序
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-07-02
         */
        void start() override;

        /**
         * @brief   终止程序
         * @author fulipeng (fulipengqd@yeah.net)
         * @date    2024-07-18
         */
        void stop() override;

    private:

        /**
         * @brief 子域数量
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-07-01
         */
        int _subdomains = -1;
    };
    Register2FITKProgramDriverFactory(1, FITKOFFoamDictionaryDriver, FITKOFFoamDictionaryDriver)
}
#endif // FITKFOAMDICTIONARYDRIVER_H
