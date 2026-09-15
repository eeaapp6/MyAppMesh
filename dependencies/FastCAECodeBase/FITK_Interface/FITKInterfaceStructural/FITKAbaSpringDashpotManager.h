/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaSpringDashpotManager.h
 * @brief  弹簧阻尼管理器
 * @author LiJin (1003680421@qq.com)
 * @date   2024-06-19
 * 
 */
#ifndef FITKABASPRINGDASHPOTMANAGER_H
#define FITKABASPRINGDASHPOTMANAGER_H
#include "FITKAbaAbstractSpringDashpot.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"
#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"


namespace Interface {

    class FITKAbaSpringDashpotManager: public Core::FITKAbstractDataObject,
        public Core::FITKAbstractDataManager<FITKAbaAbstractSpringDashpot>
    {
    public:
        FITKAbaSpringDashpotManager() = default;
        ~FITKAbaSpringDashpotManager() = default;
    };
}
#endif // FITKABASPRINGDASHPOTMANAGER_H
