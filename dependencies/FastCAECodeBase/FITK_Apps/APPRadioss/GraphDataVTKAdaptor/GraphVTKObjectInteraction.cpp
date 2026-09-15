/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GraphVTKObjectInteraction.h"
#include "GraphDataSetCompomentManager.h"
#include "GraphVTKObjectHighlighting.h"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
#include "FITK_Interface/FITKInterfaceModel/FITKModelSet.h"
#include "FITK_Interface/FITKInterfaceModel/FITKMeshSurface.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractInteraction.h"

#include "FITK_Interface/FITKVTKAlgorithm/FITKGraphActor.h"
#include <vtkUnstructuredGrid.h>
#include <vtkAppendFilter.h>
#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkProperty.h>

namespace Exchange
{
    GraphVTKObjectInteraction::GraphVTKObjectInteraction(Radioss::FITKAbstractInteraction* dataObj)
        : GraphVTKObjectModelBase(dataObj)
    {
        init();

        if (!generateDataSet()) {
            return;
        }

        generateGraph();

        updateVisibility();
    }

    GraphVTKObjectInteraction::~GraphVTKObjectInteraction()
    {
        deleteVtkObj(_grid);
    }

    void GraphVTKObjectInteraction::updateModelColor(QColor color)
    {
        // 检查对象是否有效
        Radioss::FITKAbstractInteraction* interaction = dynamic_cast<Radioss::FITKAbstractInteraction*>(_dataObj);
        if (interaction == nullptr || _actor == nullptr) return;

        // 如果颜色无效，使用默认蓝色
        if (!color.isValid()) {
            color = Qt::blue;
        }

        // 更新演员颜色 - 用户可以设置任何颜色，包括红色
        _actor->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());
        _actor->GetProperty()->SetOpacity(color.alphaF());
    }

    bool GraphVTKObjectInteraction::getVisibility()
    {
        if (_actor == nullptr) {
            return false;
        }
        if (_actor->GetVisibility() && _dataObj->isEnable()) {
            return true;
        }
    }

    void GraphVTKObjectInteraction::updateVisibility()
    {
        if (_actor == nullptr) {
            return;
        }
        _actor->SetVisibility(getVisibility());
    }

    void GraphVTKObjectInteraction::setVisible(bool visibility)
    {
        if (_actor == nullptr) {
            return;
        }
        _actor->SetVisibility(visibility);
    }

    void GraphVTKObjectInteraction::update(bool forceUpdate)
    {
        //更新可见
        updateVisibility();
        //判断是否需要刷新数据
        if (forceUpdate == false)
        {
            return;
        }
        if (!generateDataSet()) {
            return;
        }
        _actor->setInputDataObject(_grid);
    }

    void GraphVTKObjectInteraction::init()
    {
        _grid = vtkUnstructuredGrid::New();
        _grid->SetPoints(vtkSmartPointer<vtkPoints>::New());
    }

    bool GraphVTKObjectInteraction::generateDataSet()
    {
        if (_grid == nullptr) {
            return false;
        }

        Radioss::FITKAbstractInteraction* interaction = dynamic_cast<Radioss::FITKAbstractInteraction*>(_dataObj);
        if (interaction == nullptr) {
            return false;
        }

        GraphDataSetCompomentManager* manager = GraphDataSetCompomentManager::getInstance();
        if (manager == nullptr) {
            return false;
        }
        vtkSmartPointer< vtkAppendFilter> appendFilter = vtkSmartPointer<vtkAppendFilter>::New();
        //获取主面id
        int surfId = interaction->getMasterSurfaceID();
        Interface::FITKMeshSurface* surf = FITKDATAREPO->getTDataByID<Interface::FITKMeshSurface>(surfId);
        int modelId = -1;
        if (surf)
        {
            modelId = surf->getAbsModelID();
            vtkUnstructuredGrid* surfGrid = manager->getSurfGridData(surfId, modelId);
            if (surfGrid == nullptr) {
                return false;
            }
            appendFilter->AddInputData(surfGrid);
        }
       
        //获取从节点组
        int setId = interaction->getNodeGroupID();
        Interface::FITKModelSet* set = FITKDATAREPO->getTDataByID<Interface::FITKModelSet>(setId);
        if (set)
        {
            modelId = set->getAbsModelID();
            vtkUnstructuredGrid* setGrid = manager->getSetGridData(setId, modelId);
            if (setGrid == nullptr) {
                return false;
            }
            appendFilter->AddInputData(setGrid);
        }

        appendFilter->Update();
        _grid->DeepCopy(appendFilter->GetOutput());

        return true;
    }

    void GraphVTKObjectInteraction::generateGraph()
    {
        _actor = FITKGraphActor::New();
        _actor->SetPickable(false);
        _actor->setGraphObject(this);
        _actor->GetProperty()->SetRepresentation(VTK_WIREFRAME);
        _actor->GetProperty()->SetPointSize(GraphVTKCommons::_highlightPointSize);
        this->setRenderLayer(0);
        // 设置默认颜色
        QColor defaultColor = Qt::red;
        _actor->GetProperty()->SetColor(defaultColor.redF(), defaultColor.greenF(), defaultColor.blueF());
        _actor->setRelativeCoincidentTopologyPolygonOffsetParameters(-10);
        _actor->setInputDataObject(_grid);
        addActor(_actor);

        m_graphObjHighlight->addModelInputDataObject(_grid);
        m_graphObjHighlight->setVertexSize(GraphVTKCommons::_highlightPointSize);
        m_graphObjHighlight->setActorType(2);
    }
}