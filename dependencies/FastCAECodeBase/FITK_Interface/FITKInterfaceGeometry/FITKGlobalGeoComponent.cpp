/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKGlobalGeoComponent.h"

namespace Interface
{
    FITKGlobalGeoComponent::FITKGlobalGeoComponent(const Interface::FITKModelEnum::FITKModelSetType type) :
        m_geoType(type)
    {
        // 创建指定类型几何分组数据。
    }
}
