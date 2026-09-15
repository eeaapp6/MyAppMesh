/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbstractGlobalDataFactory.h"
#include "FITKGlobalData.h"

namespace AppFrame
{
    FITKAbstractGlobalDataFactory::~FITKAbstractGlobalDataFactory()
    {
    }

    void FITKAbstractGlobalDataFactory::createData(FITKGlobalData* globalData)
    {
        //错误判断
        if (globalData == nullptr) return;
        //创建网格
        auto d = this->createMeshData();
        globalData->insertData(GDTMesh, d);
        //创建几何
        d = this->createGeoData();
        globalData->insertData(GDTGeom, d);
        //创建物理数据
        d = this->createPhysicsData();
        globalData->insertData(GDTPhysics, d);
        //创建后处理数据
        d = this->createPostData();
        globalData->insertData(GDTPost, d);
        //创建软件运行时数据
//         d = this->createRunTimeSetting();
//         globalData->insertData(GDTRunTime, d);
        //创建其他数据
        QHash<int, Core::FITKAbstractDataObject*>  othersData = this->createOtherData();
        QList<int> dts = othersData.keys();
        for (auto dt : dts)
        {
            globalData->insertData(dt, othersData.value(dt));
        }

    }

    Core::FITKAbstractDataObject* FITKAbstractGlobalDataFactory::createMeshData()
    {
        return nullptr;
    }

    Core::FITKAbstractDataObject* FITKAbstractGlobalDataFactory::createGeoData()
    {
        return nullptr;
    }

    Core::FITKAbstractDataObject* FITKAbstractGlobalDataFactory::createPhysicsData()
    {
        return nullptr;

    }

    Core::FITKAbstractDataObject* FITKAbstractGlobalDataFactory::createPostData()
    {
        return nullptr;
    }

//     Core::FITKAbstractDataObject* FITKAbstractGlobalDataFactory::createRunTimeSetting()
//     {
//         return nullptr;
//     }

    QHash<int, Core::FITKAbstractDataObject*> FITKAbstractGlobalDataFactory::createOtherData()
    {
        return QHash<int, Core::FITKAbstractDataObject*>();
    }

}

