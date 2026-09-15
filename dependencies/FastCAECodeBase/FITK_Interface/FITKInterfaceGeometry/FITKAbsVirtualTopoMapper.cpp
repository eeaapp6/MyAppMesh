/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbsVirtualTopoMapper.h"
#include "FITKAbsVirtualTopo.h"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"

namespace Interface
{
    void FITKVirtualTopoMapper::mapTopo(FITKAbsVirtualTopo* vtp, FITKGeoEnum::VTopoShapeType shapeType, bool basicTopoOnly)
    {
        _mappedTopo.clear();

        if (vtp == nullptr) return;
        FITKGeoEnum::VTopoShapeType st = vtp->getShapeType();

        //寻找的层级低于当前层级
        if ((int)shapeType <= (int)st)
        {
            this->iteratorLower(vtp, shapeType, basicTopoOnly);
        }
        //寻找的层级高于当前层级
        else
        {
            this->iteratorUpper(vtp, shapeType, basicTopoOnly);
        }

    }

    void FITKVirtualTopoMapper::mapTopo(int vtpID, FITKGeoEnum::VTopoShapeType shapeType, bool basicTopoOnly)
    {
        //从全局数据仓库查找
        FITKAbsVirtualTopo* vtp = FITKDATAREPO->getTDataByID<FITKAbsVirtualTopo>(vtpID);
        this->mapTopo(vtp, shapeType, basicTopoOnly);
    }

    int FITKVirtualTopoMapper::length()
    {
        return _mappedTopo.size();
    }

    Interface::FITKAbsVirtualTopo* FITKVirtualTopoMapper::virtualTopo(const int index)
    {
        //错误判断
        if (index < 0 || index >= _mappedTopo.size()) return nullptr;
        return _mappedTopo[index];

    }

    void FITKVirtualTopoMapper::iteratorLower(FITKAbsVirtualTopo* topo, FITKGeoEnum::VTopoShapeType shapeType, bool basicTopoOnly)
    {
        if (topo == nullptr) return;

        // 2025/07/02 跳过被合并的拓扑对象。
        if (basicTopoOnly)
        {
            if (topo->getCombinedTopoCount() != 0)
            {
                return;
            }
        }
        else
        {
            if (topo->getCombinedToTopo())
            {
                return;
            }
        }

        FITKGeoEnum::VTopoShapeType t = topo->getShapeType();
        //当前类型低于要查找的类型，则直接退出
        if ((int)t < (int)shapeType) return;
        //类型一致
        if ((int)t == (int)shapeType && !_mappedTopo.contains(topo))
            _mappedTopo.append(topo);

        const int n = topo->getSubTopoCount();
        for (int i = 0; i < n; ++i)
        {
            FITKAbsVirtualTopo* t = topo->getSubTopo(i);
            //进入递归
            this->iteratorLower(t, shapeType, basicTopoOnly);
        }

    }

    void FITKVirtualTopoMapper::iteratorUpper(FITKAbsVirtualTopo* topo, FITKGeoEnum::VTopoShapeType shapeType, bool basicTopoOnly)
    {
        if (topo == nullptr) return;

        // 2025/07/02 跳过被合并的拓扑对象。
        if (basicTopoOnly)
        {
            if (topo->getCombinedTopoCount() != 0)
            {
                return;
            }
        }
        else
        {
            if (topo->getCombinedToTopo())
            {
                return;
            }
        }

        FITKGeoEnum::VTopoShapeType t = topo->getShapeType();
        //当前类型高于要查找的类型，则直接退出
        if ((int)t > (int)shapeType) return;
        //类型一致
        if ((int)t == (int)shapeType && !_mappedTopo.contains(topo))
        {
            _mappedTopo.append(topo);
        }

        const int n = topo->getParentTopoCount();
        for (int i = 0; i < n; ++i)
        {
            FITKAbsVirtualTopo* t = topo->getParentTopo(i);
            //递归查找
            this->iteratorUpper(t, shapeType, basicTopoOnly);
        }
    }
}
