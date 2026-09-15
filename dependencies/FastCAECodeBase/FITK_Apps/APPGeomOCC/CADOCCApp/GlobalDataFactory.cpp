/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GlobalDataFactory.h"

#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"

Core::FITKAbstractDataObject *GlobalDataFactory::createMeshData()
{
    return nullptr;
}

Core::FITKAbstractDataObject* GlobalDataFactory::createGeoData()
{
    // 创建几何数据
    return new Interface::FITKGeoCommandList;
}

Core::FITKAbstractDataObject *GlobalDataFactory::createPhysicsData()
{
    return nullptr;
}

Core::FITKAbstractDataObject *GlobalDataFactory::createPostData()
{
    return nullptr;
}

QHash<int, Core::FITKAbstractDataObject *> GlobalDataFactory::createOtherData()
{
    // 不创建其他数据
    return QHash<int, Core::FITKAbstractDataObject *>();
}
