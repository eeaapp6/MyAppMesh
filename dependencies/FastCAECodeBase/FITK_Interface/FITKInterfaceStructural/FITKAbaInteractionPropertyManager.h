/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaInteractionPropertyManager.h
 * @brief  相互作用属性管理器
 * @author LiJin (1003680421@qq.com)
 * @date   2024-04-28
 * 
 */
#ifndef FITKABAINTERACTIONPROPERTYMANAGER_H
#define FITKABAINTERACTIONPROPERTYMANAGER_H
#include "FITKInterfaceStructuralAPI.h"
#include "FITKAbaAbstractInteractionProperty.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"
#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"


namespace Interface {
    /**
     * @brief  相互作用属性管理器
     * @author LiJin (1003680421@qq.com)
     * @date   2024-04-28
     */
    class FITKInterfaceStructuralAPI FITKAbaInteractionPropertyManager:public Core::FITKAbstractDataObject,
    public Core::FITKAbstractDataManager<FITKAbaAbstractInteractionProperty>
    {
    public:
        /**
         * @brief  构造函数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        FITKAbaInteractionPropertyManager() = default;
        /**
         * @brief  析构函数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        ~FITKAbaInteractionPropertyManager() = default;
    };
}


#endif // FITKABAINTERACTIONPROPERTYMANAGER_H
