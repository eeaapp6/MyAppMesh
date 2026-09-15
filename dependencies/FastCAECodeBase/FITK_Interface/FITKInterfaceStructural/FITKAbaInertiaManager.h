/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaInertiaManager.h
 * @brief  惯性元素管理器
 * @author LiJin (1003680421@qq.com)
 * @date   2024-05-10
 * 
 */
#ifndef FITKABAINERTIAMANAGER_H
#define FITKABAINERTIAMANAGER_H
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"
#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"
#include "FITKInterfaceStructuralAPI.h"
#include "FITKAbaAbstractInertia.h"


namespace Interface {
    /**
     * @brief  惯性元素管理器
     * @author LiJin (1003680421@qq.com)
     * @date   2024-05-10
     */
    class FITKInterfaceStructuralAPI FITKAbaInertiaManager: public Core::FITKAbstractDataManager<FITKAbaAbstractInertia>,
                                                         public Core::FITKAbstractNDataObject
    {
    public:
        /**
         * @brief  构造函数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-05-10
         */
        FITKAbaInertiaManager() = default;
        /**
         * @brief  析构函数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-05-10
         */
        ~FITKAbaInertiaManager() = default;
    };
}


#endif // FITKABAINERTIAMANAGER_H
