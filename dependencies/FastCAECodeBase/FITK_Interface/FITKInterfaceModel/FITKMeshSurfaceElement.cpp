/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKMeshSurfaceElement.h"
#include "FITKModelSet.h"
#include "FITKAbstractModel.h"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"

namespace Interface
{
    FITKMeshSurface::MeshSurfaceType FITKMeshSurfaceElement::getMeshSurfaceType()
    {
        return FITKMeshSurface::MeshSurfaceType::SurEle;
    }

    FITKModelSurface::ModelSurfaceType FITKMeshSurfaceElement::getModelSurfaceType()
    {
        return FITKModelSurface::ModelSurfaceType::MST_MeshEle;
    }

    void FITKMeshSurfaceElement::addMeshSet(int modelid, int setID, int surfIndex)
    {
        //包含原来集合，则修改
        const int index = this->indexOf(modelid, setID);
        if (index > -1)
        {
            _modelSetIDs[index] = QPair<int, int>(modelid, setID);
            //首先获取原来ID对应的索引
            if (index >= 0 && index < _surfIndex.size())
                _surfIndex[index] = surfIndex;
        }

        //不存在则追加
        else
        {
            _modelSetIDs.append(QPair<int, int>(modelid, setID));
            _surfIndex.append(surfIndex);
        }
    }

    void FITKMeshSurfaceElement::addMeshSet(FITKAbstractModel* model, FITKModelSet* set, int surfIndex)
    {
        //错误判断
        if (set == nullptr) return;
        if (set->getModelSetType() != FITKModelEnum::FITKModelSetType::FMSElem) return;

        int modelID = -1;
        if (model)
            modelID = model->getDataObjectID();

        //通过ID追加
        const int id = set->getDataObjectID();
        this->addMeshSet(modelID, id, surfIndex);
    }

    void FITKMeshSurfaceElement::clearMeshSet()
    {
        clearSets();
        _surfIndex.clear();
    }


    int FITKMeshSurfaceElement::getSurfaceIndexAt(const int index) const
    {
        //错误判断
        if (index < 0 || index >= _surfIndex.size()) return -1;
        return _surfIndex.at(index);
    }

    void FITKMeshSurfaceElement::setSurfaceIndex(const int index, const int surfIndex)
    {
        //错误判断
        if (index < 0 || index >= _surfIndex.size()) return;
        _surfIndex[index] = surfIndex;
    }

    QList<SurfaceAbsSetInfo> FITKMeshSurfaceElement::getAbsoluteSetInfo()
    {
        QList<SurfaceAbsSetInfo> eleSurfInfo{};
        const int n = _modelSetIDs.size();
        if (n != _surfIndex.size()) return eleSurfInfo;

        for (int i = 0; i < n; ++i)
        {
            FITKAbstractModel* model = this->getModelAt(i);
            FITKModelSet* set = this->getMeshSetAt(i);
            const int surfIndex = _surfIndex.at(i);
            if (!model || !set || surfIndex < 0) continue;

            QList<FITKModelSet*> eleSets;
            set->getAbsoluteSet(eleSets, Interface::FITKModelEnum::FITKModelSetType::FMSElem);
            //是否为装配
            bool isAss = model->getAbsModelType() == Interface::FITKModelEnum::AbsModelType::Assembly;
            for (FITKModelSet* set : eleSets)
            {
                if (set == nullptr) continue;
                SurfaceAbsSetInfo info;
                info._model = model;
                if (isAss)
                    info._model = set->getAbstractModel();
                info._set = set;
                info._surfaceIndex = surfIndex;
                eleSurfInfo.append(info);
            }
        }

        return eleSurfInfo;
    }
}
