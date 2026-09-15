/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKModelSurface.h"

#include "FITKModelSet.h"
#include "FITKAbstractModel.h"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"

namespace Interface
{
    FITKModelSurface::~FITKModelSurface()
    {
        // 析构子集。
        // clearSets();
    }

    int FITKModelSurface::getSetCount() const
    {
        //集合长度
        return _modelSetIDs.size();
    }

    Interface::FITKModelSet* FITKModelSurface::getSetAt(const int index) const
    {
        //错误判断
        if (index < 0 || index >= _modelSetIDs.size()) return nullptr;
        const int id = _modelSetIDs.at(index).second;
        //类型转换
        return FITKDATAREPO->getTDataByID<Interface::FITKModelSet>(id);
    }

    int FITKModelSurface::getSetIDAt(const int index) const
    {
        //错误判断
        if (index < 0 || index >= _modelSetIDs.size()) return -1;
        return _modelSetIDs.at(index).second;
    }

    bool FITKModelSurface::isInternal() const
    {
        return _internal;
    }

    void FITKModelSurface::isInternal(bool internal)
    {
        _internal = internal;
    }

    FITKAbstractModel* FITKModelSurface::getModelAt(const int index) const
    {
        if (index < 0 || index >= _modelSetIDs.size()) return nullptr;
        const int id = _modelSetIDs.at(index).first;
        //类型转换
        FITKAbstractModel* model = FITKDATAREPO->getTDataByID<Interface::FITKAbstractModel>(id);
        if (model == nullptr)
        {
            auto set = this->getSetAt(index);
            if (set)
                model = set->getAbstractModel();
        }
        return model;
    }

    QList<Interface::SurfaceAbsSetInfo> FITKModelSurface::getAbsoluteSetInfo()
    {
        return QList<Interface::SurfaceAbsSetInfo>();
    }

    void FITKModelSurface::removeSetAt(int index)
    {
        //错误判断
        if (index < 0 || index >= _modelSetIDs.size()) return;

        QPair<int, int> setInfo = _modelSetIDs.takeAt(index);
        int setId = setInfo.second;
        Interface::FITKModelSet* set = FITKDATAREPO->getTDataByID<Interface::FITKModelSet>(setId);
        if (set)
        {
            delete set;
        }
    }

    void FITKModelSurface::clearSets()
    {
        // 析构子集。
        for (int i = _modelSetIDs.size() - 1; i >= 0; i--)
        {
            int setId = _modelSetIDs[i].second;
            Interface::FITKModelSet* set = FITKDATAREPO->getTDataByID<Interface::FITKModelSet>(setId);
            if (set)
            {
                delete set;
            }
        }

        _modelSetIDs.clear();
    }

    int FITKModelSurface::indexOf(int modelID, int setID)
    {
        QPair<int, int> p(modelID, setID);
        return _modelSetIDs.indexOf(p);
    }
}
