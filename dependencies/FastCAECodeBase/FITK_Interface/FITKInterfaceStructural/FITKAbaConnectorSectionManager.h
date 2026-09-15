/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaConnectorSectionManager.h
 * @brief  连接器截面管理器
 * @author LiJin (1003680421@qq.com)
 * @date   2024-06-20
 * 
 */
#ifndef FITKABACONNECTORSECTIONMANAGER_H
#define FITKABACONNECTORSECTIONMANAGER_H
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"
#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"
#include "FITKAbaConnectorSection.h"

namespace Interface {

class FITKInterfaceStructuralAPI FITKAbaConnectorSectionManager: public Core::FITKAbstractDataObject,
    public Core::FITKAbstractDataManager<FITKAbaConnectorSection>
{
public:
    FITKAbaConnectorSectionManager() = default;
    ~FITKAbaConnectorSectionManager() = default;
};
}
#endif // FITKABACONNECTORSECTIONMANAGER_H
