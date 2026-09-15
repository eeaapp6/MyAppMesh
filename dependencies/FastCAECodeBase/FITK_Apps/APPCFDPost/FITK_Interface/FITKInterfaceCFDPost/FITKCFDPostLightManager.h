/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @brief  流体三维后处理灯光管理器
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-06
 */
#ifndef __FITKCFDPostLightManager_H_
#define __FITKCFDPostLightManager_H_

#include "FITKInterfaceCFDPostAPI.h"
#include "FITKCFDPostLight.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"

namespace Interface
{
    /**
     * @brief  流体三维后处理灯光管理器
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-09-06
     */
    class FITKInterfaceCFDPostAPI FITKCFDPostLightManager : public Core::FITKAbstractDataManager<FITKCFDPostLight>
    {
    public:
        /**
         * @brief    构造函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-06
         */
        explicit FITKCFDPostLightManager() = default;
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-06
         */
        virtual ~FITKCFDPostLightManager();
    };
}
#endif
