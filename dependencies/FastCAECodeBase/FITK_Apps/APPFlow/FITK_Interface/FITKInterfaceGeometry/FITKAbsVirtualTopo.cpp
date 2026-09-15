/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbsVirtualTopo.h"

namespace Interface
{

    FITKAbsVirtualTopo::FITKAbsVirtualTopo(FITKGeoEnum::VTopoShapeType type)
        :_topoShapeType(type)
    {
    }

    FITKAbsVirtualTopo::~FITKAbsVirtualTopo()
    {
        //清空子对象
        this->clearSubTopo();
        if (_shape) delete _shape;
        _shape = nullptr;
        _combinedTopos.clear();
    }

    FITKGeoEnum::VTopoShapeType FITKAbsVirtualTopo::getShapeType() const
    {
        //获取拓扑类型
        return _topoShapeType;
    }

    void FITKAbsVirtualTopo::setShapeType(FITKGeoEnum::VTopoShapeType s)
    {
        _topoShapeType = s;
    }

    void FITKAbsVirtualTopo::addSubTopo(FITKAbsVirtualTopo* subTopo)
    {
        //添加子对象，已经包含的不再添加
        if (_subTopo.contains(subTopo)) return;
        _subTopo.append(subTopo);
        subTopo->addParentTopo(this);
    }

    void FITKAbsVirtualTopo::removeSubTopo(FITKAbsVirtualTopo* subTopo)
    {
        if (!subTopo)
        {
            return;
        }

        subTopo->removeParentTopo(this);
        _subTopo.removeOne(subTopo);
    }

    void FITKAbsVirtualTopo::addParentTopo(FITKAbsVirtualTopo* pTopo)
    {
        if (nullptr == pTopo || _parents.contains(pTopo)) return;
        return _parents.append(pTopo);
    }

    void FITKAbsVirtualTopo::removeParentTopo(FITKAbsVirtualTopo* pTopo)
    {
        _parents.removeOne(pTopo);
    }

    bool FITKAbsVirtualTopo::addCombinedTopo(FITKAbsVirtualTopo* vTopo)
    {
        // 追加至当前拓扑合并拓扑列表，并将当前拓扑设置为该拓扑的合并至拓扑对象。
        if (nullptr == vTopo || _combinedTopos.contains(vTopo)) return false;
        if (vTopo->getCombinedToTopo())
        {
            return false;
        }

        vTopo->setCombinedToTopo(this);
        _combinedTopos.append(vTopo);
        return true;
    }

    void FITKAbsVirtualTopo::removeCombinedTopo(FITKAbsVirtualTopo* vTopo)
    {
        if (nullptr == vTopo || !_combinedTopos.contains(vTopo)) return;
        vTopo->setCombinedToTopo(nullptr);
        _combinedTopos.removeOne(vTopo);
    }

    void FITKAbsVirtualTopo::clearCombinedTopo()
    {
        for (FITKAbsVirtualTopo* vTopo : _combinedTopos)
        {
            vTopo->setCombinedToTopo(nullptr);
            _combinedTopos.removeOne(vTopo);
        }
    }

    void FITKAbsVirtualTopo::setCombinedToTopo(FITKAbsVirtualTopo* vTopo)
    {
        _combinedToTopo = vTopo;
    }

    int FITKAbsVirtualTopo::getSubTopoCount()
    {
        return _subTopo.count();
    }

    FITKAbsVirtualTopo* FITKAbsVirtualTopo::getSubTopo(const int index)
    {
        //数组是否越界判断
        if (index < 0 || index >= _subTopo.count()) return nullptr;
        return _subTopo[index];
    }

    int FITKAbsVirtualTopo::getParentTopoCount()
    {
        return _parents.count();
    }

    FITKAbsVirtualTopo* FITKAbsVirtualTopo::getParentTopo(const int index)
    {
        //数组是否越界判断
        if (index < 0 || index >= _parents.count()) return nullptr;
        return _parents[index];
    }

    FITKAbsVirtualTopo* FITKAbsVirtualTopo::getRootTopo()
    {
        FITKAbsVirtualTopo* rootTopo = getParentTopo(0);
        do
        {
            auto tempRootTopo = rootTopo->getParentTopo(0);
            if (!tempRootTopo) return rootTopo;
            rootTopo = tempRootTopo;

        } while (rootTopo != nullptr);

        return rootTopo;
    }

    void FITKAbsVirtualTopo::clearSubTopo()
    {
        //      const int n = _subTopo.size();
        //      for (int i = 0; i<n; ++i)
        //      {
        //          FITKAbsVirtualTopo* t = _subTopo.at(i);
        //          delete t;
        //      }
        _subTopo.clear();
    }

    void FITKAbsVirtualTopo::clearParentTopo()
    {
        _parents.clear();
    }

    void FITKAbsVirtualTopo::setShape(FITKAbsVirtualTopoShape* shape)
    {
        _shape = shape;
    }

    FITKAbsVirtualTopoShape* FITKAbsVirtualTopo::getShape()
    {
        return _shape;
    }

    void FITKAbsVirtualTopo::setIndexLabel(const int index)
    {
        _indexLabel = index;
    }


    int FITKAbsVirtualTopo::getIndexLabel() const
    {
        return _indexLabel;
    }

    void FITKAbsVirtualTopo::setTag(const int tag)
    {
        // 设置几何标记。
        _tag = tag;
    }

    int FITKAbsVirtualTopo::getTag()
    {
        return _tag;
    }

    void FITKAbsVirtualTopo::setCreatedGeoCmdID(int id)
    {
        m_createdCmdId = id;
    }

    int FITKAbsVirtualTopo::getCreatedGeoCmdID()
    {
        return m_createdCmdId;
    }

    FITKAbsVirtualTopo* FITKAbsVirtualTopo::getCombinedToTopo()
    {
        return _combinedToTopo;
    }

    int FITKAbsVirtualTopo::getCombinedTopoCount()
    {
        return _combinedTopos.count();
    }

    FITKAbsVirtualTopo* FITKAbsVirtualTopo::getCombinedTopo(const int index)
    {
        //数组是否越界判断
        if (index < 0 || index >= _combinedTopos.count()) return nullptr;
        return _combinedTopos[index];
    }

    QList<FITKAbsVirtualTopo*> FITKAbsVirtualTopo::getCombinedBasicToposRecur()
    {
        QList<FITKAbsVirtualTopo*> subTopos;
        for (FITKAbsVirtualTopo* cTopo : _combinedTopos)
        {
            if (!cTopo)
            {
                continue;
            }

            if (cTopo->getCombinedTopoCount())
            {
                subTopos << cTopo->getCombinedBasicToposRecur();
            }
            else
            {
                subTopos << cTopo;
            }
        }

        return subTopos;
    }

    FITKAbsVirtualTopoShape::~FITKAbsVirtualTopoShape()
    {

    }

}



