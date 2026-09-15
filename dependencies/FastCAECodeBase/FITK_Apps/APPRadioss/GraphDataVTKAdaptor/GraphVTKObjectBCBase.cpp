/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GraphVTKObjectBCBase.h"
#include "GraphDataVTKPartSetManager.h"

#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
#include "FITK_Kernel/FITKCore/FITKVec3D.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractBCS.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossMeshModel.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossNodesParts.h"
#include "FITK_Interface/FITKInterfaceModel/FITKModelSet.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeometryMeshEntity.h"
#include <vtkPoints.h>
#include <vtkPolyData.h>
namespace Exchange {
    GraphVTKObjectBCBase::GraphVTKObjectBCBase(Radioss::FITKAbstractBCS* loadData) :
        GraphVTKObject3D(loadData)
    {
        _renderLayer = 2;
    }

    GraphVTKObjectBCBase::~GraphVTKObjectBCBase()
    {

    }

    void GraphVTKObjectBCBase::generatePolyData(vtkPolyData* ploydata)
    {
        if (ploydata == nullptr) return;
        Radioss::FITKAbstractBCS* bc = dynamic_cast<Radioss::FITKAbstractBCS*>(_dataObj);
        if (bc == nullptr) return;
        //获取集合
        Interface::FITKModelSet* set = FITKDATAREPO->getTDataByID<Interface::FITKModelSet>(bc->getGroupID1());
        //获取所有网格数据
        Radioss::FITKRadiossCase* radiossCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (radiossCase == nullptr) return;
        Radioss::FITKRadiossMeshModel* meshModel = radiossCase->getMeshModel();
        if (meshModel == nullptr) return;
        Radioss::FITKRadiossNodes* nodesPart = meshModel->getNodes();
        if (nodesPart == nullptr) return;

        auto insertPoints = [&](Interface::FITKModelSet* set) {
            QList<int>memberIds = set->getAbsoluteMember();
            auto type = set->getModelSetType();
            switch (type)
            {
            case Interface::FITKModelEnum::FMSNode:
            {
                //获取点集
                vtkPoints* points = ploydata->GetPoints();
                if (points == nullptr)break;

                for (int memberId : memberIds) {
                    //获取对应的点数据
                    Core::FITKNode* node = nodesPart->getNodeByID(memberId);
                    if (node == nullptr)continue;
                    //添加点
                    int vtkPointID = points->InsertNextPoint(node->x(), node->y(), node->z());
                }
                break;
            }
            }
            };

        if (set)
        {
            insertPoints(set);
        }
        else
        {
            //获取点集
            vtkPoints* points = ploydata->GetPoints();
            if (points == nullptr)return;
            //获取所有点数据
            for (int i = 0; i < nodesPart->getNodeCount(); i++)
            {
                Core::FITKNode* node = nodesPart->getNodeAt(i);
                if (node == nullptr) continue;
                //添加点
                int vtkPointID = points->InsertNextPoint(node->x(), node->y(), node->z());
            }
        }
    }
}

