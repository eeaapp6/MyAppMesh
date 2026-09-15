/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GraphVTKObjectProbeAcceleration.h"
#include "GraphDataSetCompomentManager.h"
#include "GraphVTKCommons.h"
#include "GraphVTKObjectModelBase.h"
#include "GraphVTKObjectHighlighting.h"

#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
#include "FITK_Component/FITKRadiossData/FITKProbeAcceleration.h"
#include "FITK_Interface/FITKInterfaceModel/FITKModelSet.h"

#include "FITK_Interface/FITKVTKAlgorithm/FITKGraphActor.h"

#include <vtkUnstructuredGrid.h>
#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkProperty.h>

namespace Exchange
{
    GraphVTKObjectProbeAcceleration::GraphVTKObjectProbeAcceleration(Radioss::FITKProbeAcceleration* dataObj)
        : GraphVTKObjectModelBase(dataObj)
    {
        init();

        if (!generateDataSet()) {
            return;
        }

        generateGraph();

        updateVisibility();
    }

    GraphVTKObjectProbeAcceleration::~GraphVTKObjectProbeAcceleration()
    {
        deleteVtkObj(_grid);
    }

    void GraphVTKObjectProbeAcceleration::updateModelColor(QColor color)
    {
        // 检查对象是否有效
        Radioss::FITKProbeAcceleration* acceleration = dynamic_cast<Radioss::FITKProbeAcceleration*>(_dataObj);
        if (acceleration == nullptr || _actor == nullptr) return;

        // 如果颜色无效，使用默认蓝色
        if (!color.isValid()) {
            color = Qt::blue;
        }

        // 更新演员颜色 - 用户可以设置任何颜色，包括红色
        _actor->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());
        _actor->GetProperty()->SetOpacity(color.alphaF());
    }

    bool GraphVTKObjectProbeAcceleration::getVisibility()
    {
        if (_actor == nullptr) {
            return false;
        }
        if (_actor->GetVisibility() && _dataObj->isEnable()) {
            return true;
        }
    }

    void GraphVTKObjectProbeAcceleration::updateVisibility()
    {
        if (_actor == nullptr) {
            return;
        }
        _actor->SetVisibility(getVisibility());
    }

    void GraphVTKObjectProbeAcceleration::setVisible(bool visibility)
    {
        if (_actor == nullptr) {
            return;
        }
        _actor->SetVisibility(visibility);
    }

    void GraphVTKObjectProbeAcceleration::update(bool forceUpdate)
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

    void GraphVTKObjectProbeAcceleration::init()
    {
        _grid = vtkUnstructuredGrid::New();
        _grid->SetPoints(vtkSmartPointer<vtkPoints>::New());
    }

    bool GraphVTKObjectProbeAcceleration::generateDataSet()
    {
        if (_grid == nullptr) {
            return false;
        }

        Radioss::FITKProbeAcceleration* acceleration = dynamic_cast<Radioss::FITKProbeAcceleration*>(_dataObj);
        if (acceleration == nullptr) {
            return false;
        }

        // 获取场关联的集合ID
        int setId = acceleration->getSetID();
        if (setId < 0) {
            return false;
        }

        GraphDataSetCompomentManager* manager = GraphDataSetCompomentManager::getInstance();
        if (manager == nullptr) {
            return false;
        }
        Interface::FITKModelSet* set = FITKDATAREPO->getTDataByID<Interface::FITKModelSet>(setId);
        int modelId = -1;
        if (set)
        {
            modelId = set->getAbsModelID();
        }
        vtkUnstructuredGrid* setGrid = manager->getSetGridData(setId, modelId);
        if (setGrid == nullptr) {
            return false;
        }

        _grid->DeepCopy(setGrid);

        return true;
    }

    void GraphVTKObjectProbeAcceleration::generateGraph()
    {
        _actor = FITKGraphActor::New();
        _actor->SetPickable(false);
        _actor->setGraphObject(this);
        _actor->GetProperty()->SetRepresentation(VTK_SURFACE);
        _actor->GetProperty()->SetPointSize(GraphVTKCommons::_highlightPointSize);
        this->setRenderLayer(0);
        // 设置默认颜色为紫色
        QColor defaultColor = QColor(128, 0, 128); // 紫色
        _actor->GetProperty()->SetColor(defaultColor.redF(), defaultColor.greenF(), defaultColor.blueF());
        _actor->setInputDataObject(_grid);
        addActor(_actor);


        m_graphObjHighlight->addModelInputDataObject(_grid);
        m_graphObjHighlight->setVertexSize(GraphVTKCommons::_highlightPointSize);
        m_graphObjHighlight->setActorType(-1);
    }
}