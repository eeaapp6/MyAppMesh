/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKMeshSurfaceNode.h"
#include "FITKModelSet.h"
#include "FITKAbstractModel.h"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"

namespace Interface
{

    FITKMeshSurface::MeshSurfaceType FITKMeshSurfaceNode::getMeshSurfaceType()
    {
        return Interface::FITKMeshSurface::SurNode;
    }

    FITKModelSurface::ModelSurfaceType FITKMeshSurfaceNode::getModelSurfaceType()
    {
        return FITKModelSurface::ModelSurfaceType::MST_MeshNode;
    }

    void FITKMeshSurfaceNode::addMeshSet(int modelid, int setID, double v)
    {
        //包含原来集合，则修改 
        const int index = this->indexOf(modelid, setID);
        if (index > -1)
        {
            _modelSetIDs[index] = QPair<int, int>(modelid, setID);
            //首先获取原来ID对应的索引
            if (index >= 0 && index < _values.size())
                _values[index] = v;
        }

        //不存在则追加
        else
        {
            _modelSetIDs.append(QPair<int, int>(modelid, setID));
            _values.append(v);
        }
    }

    void FITKMeshSurfaceNode::addMeshSet(FITKAbstractModel* model, FITKModelSet* set, double v)
    {
        //错误判断
        if (set == nullptr) return;
        if (set->getModelSetType() != FITKModelEnum::FITKModelSetType::FMSElem) return;

        int modelID = -1;
        if (model)
            modelID = model->getDataObjectID();

        //通过ID追加
        const int id = set->getDataObjectID();
        this->addMeshSet(modelID, id, v);
    }

    void FITKMeshSurfaceNode::clearMeshSet()
    {
        clearSets();
        _values.clear();
    }


    double FITKMeshSurfaceNode::getValueAt(const int index) const
    {
        //错误判断
        if (index < 0 || index >= _values.size()) return -1;
        return _values.at(index);
    }

    QList<Interface::SurfaceAbsSetInfo> FITKMeshSurfaceNode::getAbsoluteSetInfo()
    {
        QList<SurfaceAbsSetInfo> nodeSurfInfo{};
        const int n = _modelSetIDs.size();
        if (n != _values.size()) return nodeSurfInfo;

        for (int i = 0; i < n; ++i)
        {
            FITKAbstractModel* model = this->getModelAt(i);
            FITKModelSet* set = this->getMeshSetAt(i);
            const double v = _values.at(i);
            if (!model || !set || v < 0) continue;

            QList<FITKModelSet*> nodeSets;
            set->getAbsoluteSet(nodeSets, Interface::FITKModelEnum::FITKModelSetType::FMSNode);
            //是否为装配
            bool isAss = model->getAbsModelType() == Interface::FITKModelEnum::AbsModelType::Assembly;
            for (FITKModelSet* set : nodeSets)
            {
                if (set == nullptr) continue;
                SurfaceAbsSetInfo info;
                info._model = model;
                if (isAss)
                    info._model = set->getAbstractModel();
                info._set = set;
                info._nodeWidght = v;
                nodeSurfInfo.append(info);
            }
        }

        return nodeSurfInfo;
    }

}
