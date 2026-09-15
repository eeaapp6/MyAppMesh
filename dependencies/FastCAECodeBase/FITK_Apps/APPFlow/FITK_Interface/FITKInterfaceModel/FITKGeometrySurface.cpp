/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKGeometrySurface.h"

// Data
#include "FITKModelSet.h"
#include "FITKAbstractModel.h"

namespace Interface
{
    FITKModelSurface::ModelSurfaceType FITKGeometrySurface::getModelSurfaceType()
    {
        // 几何类型。
        return FITKModelSurface::ModelSurfaceType::MST_Geom;
    }

    void FITKGeometrySurface::addGeometrySet(int modelid, int setID, int surfIndex)
    {
        //包含原来集合，则修改
        const int index = this->indexOf(modelid, setID);
        if (index > -1)
        {
            _modelSetIDs[index] = QPair<int, int>(modelid, setID);
            //首先获取原来ID对应的索引
            if (index >= 0 && index < m_surfIndice.size())
                m_surfIndice[index] = surfIndex;
        }
        //不存在则追加
        else
        {
            _modelSetIDs.append(QPair<int, int>(modelid, setID));
            m_surfIndice.append(surfIndex);
        }
    }

    void FITKGeometrySurface::addGeometrySet(FITKAbstractModel* model, FITKModelSet* set, int surfIndex)
    {
        //错误判断
        if (set == nullptr) return;
        if (set->getModelSetType() != FITKModelEnum::FITKModelSetType::FMSEdge  && set->getModelSetType() != FITKModelEnum::FITKModelSetType::FMSSurface) return;

        int modelID = -1;
        if (model)
            modelID = model->getDataObjectID();

        //通过ID追加
        const int id = set->getDataObjectID();
        this->addGeometrySet(modelID, id, surfIndex);
    }

    void FITKGeometrySurface::clearGeometrySet()
    {
        clearSets();
        m_surfIndice.clear();
    }

    int FITKGeometrySurface::getSurfaceIndexAt(const int index) const
    {
        //错误判断
        if (index < 0 || index >= m_surfIndice.size()) return -1;
        return m_surfIndice.at(index);
    }

    void FITKGeometrySurface::setSurfaceIndex(const int index, const int surfIndex)
    {
        //错误判断
        if (index < 0 || index >= m_surfIndice.size()) return;
        m_surfIndice[index] = surfIndex;
    }

    QList<SurfaceAbsSetInfo> FITKGeometrySurface::getAbsoluteSetInfo()
    {
        QList<SurfaceAbsSetInfo> surfInfo{};
        const int n = _modelSetIDs.size();
        if (n != m_surfIndice.size()) return surfInfo;

        for (int i = 0; i < n; ++i)
        {
            FITKAbstractModel* model = this->getModelAt(i);
            FITKModelSet* set = this->getSetAt(i);
            const int surfIndex = m_surfIndice.at(i);
            if (!model || !set || surfIndex < 0) continue;

            QList<FITKModelSet*> surfSets, edgeSets;
            set->getAbsoluteSet(surfSets, Interface::FITKModelEnum::FITKModelSetType::FMSSurface);
            set->getAbsoluteSet(edgeSets, Interface::FITKModelEnum::FITKModelSetType::FMSEdge);

            //是否为装配
            bool isAss = model->getAbsModelType() == Interface::FITKModelEnum::AbsModelType::Assembly;
            for (FITKModelSet* set : surfSets + edgeSets)
            {
                if (set == nullptr) continue;
                SurfaceAbsSetInfo info;
                info._model = model;
                if (isAss)
                    info._model = set->getAbstractModel();
                info._set = set;
                info._surfaceIndex = surfIndex;
                surfInfo.append(info);
            }
        }

        return surfInfo;
    }
}
