/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKStruPost3DManager.h"

namespace Interface
{
    FITKAbstractStructuralPostVTK* FITKStruPost3DManager::getDataByID(const int id)
    {
        //首先检查是否全局ID
        FITKAbstractStructuralPostVTK* post =
            Core::FITKAbstractDataManager<FITKAbstractStructuralPostVTK>::getDataByID(id);
        if (post) return post;
        //不是独立的后处理数据，根据前处理模型ID检索
        const int n = this->getDataCount();
        for (int i = 0; i < n; ++i)
        {
            //遍历全部模型
            FITKAbstractStructuralPostVTK* d = this->getDataByIndex(i);
            if(d == nullptr) continue;
            if (id == d->getPreCaseID())
                return d;
        }
        return nullptr;
    }


    QList<FITKAbstractStructuralPostVTK*> FITKStruPost3DManager::getDataIndependence()
    {
        QList<FITKAbstractStructuralPostVTK*> postList;
        const int n = this->getDataCount();
        //遍历全部模型数据
        for (int i = 0; i < n; ++i)
        {
            FITKAbstractStructuralPostVTK* d = this->getDataByIndex(i);
            if (d == nullptr) continue;
            //前处理ID为-1认为是独立数据
            if (-1 == d->getPreCaseID())
                postList.append(d);
        }
        return postList;
    }

}


