/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKMeshSurface.h"
#include "FITKModelSet.h"
#include "FITKAbstractModel.h"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"


namespace Interface
{
    FITKMeshSurface::~FITKMeshSurface()
    {
    }

    FITKMeshSurface::MeshSurfaceType FITKMeshSurface::getMeshSurfaceType()
    {
        //错误值
        return FITKMeshSurface::SurNone;
    }

    int FITKMeshSurface::getMeshSetCount() const
    {
        return this->getSetCount();

        ////集合长度
        //return _modelSetIDs.size();
    }
    Interface::FITKModelSet* FITKMeshSurface::getMeshSetAt(const int index) const
    {
        return this->getSetAt(index);

        ////错误判断
        //if (index < 0 || index >= _modelSetIDs.size()) return nullptr;
        //const int id = _modelSetIDs.at(index).second;
        ////类型转换
        //return FITKDATAREPO->getTDataByID<Interface::FITKModelSet>(id);
    }
}
