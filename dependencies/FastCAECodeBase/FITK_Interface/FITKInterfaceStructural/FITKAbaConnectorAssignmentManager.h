/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaConnectorAssignmentManager.h
 * @brief  连接器截面赋予管理器
 * @author LiJin (1003680421@qq.com)
 * @date   2024-06-20
 * 
 */
#ifndef FITKABACONNECTORASSIGNMENTMANAGER_H
#define FITKABACONNECTORASSIGNMENTMANAGER_H
#include "FITKAbaConnectorAssignment.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"
#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"

namespace Interface {

class FITKInterfaceStructuralAPI FITKAbaConnectorAssignmentManager: public Core::FITKAbstractDataManager<FITKAbaConnectorAssignment>,
                                                                     public Core::FITKAbstractDataObject
{
public:
    FITKAbaConnectorAssignmentManager() = default;
    ~FITKAbaConnectorAssignmentManager() = default;
};
}
#endif // FITKABACONNECTORASSIGNMENTMANAGER_H
