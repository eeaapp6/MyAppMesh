/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaPredefinedFieldManager.h
 * @brief  预定义场管理器
 * @author LiJin (1003680421@qq.com)
 * @date   2024-05-10
 * 
 */
#ifndef FITKABAPREDEFINEDFIELDMANAGER_H
#define FITKABAPREDEFINEDFIELDMANAGER_H
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"
#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"
#include "FITKInterfaceStructuralAPI.h"
#include "FITKAbaAbstractPredefinedField.h"


namespace Interface {
    /**
     * @brief  预定义场管理器
     * @author LiJin (1003680421@qq.com)
     * @date   2024-05-10
     */
    class FITKInterfaceStructuralAPI FITKAbaPredefinedFieldManager: public Core::FITKAbstractDataManager<FITKAbaAbstractPredefinedField>,
                                                                 public Core::FITKAbstractNDataObject
    {
    public:
        FITKAbaPredefinedFieldManager() = default;
        ~FITKAbaPredefinedFieldManager() = default;
    };
}


#endif // FITKABAPREDEFINEDFIELDMANAGER_H
