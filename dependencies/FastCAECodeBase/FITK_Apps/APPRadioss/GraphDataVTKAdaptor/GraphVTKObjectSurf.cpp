/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GraphVTKObjectSurf.h"
#include "GraphDataSetCompomentManager.h"

#include "FITK_Kernel/FITKCore/FITKCoreMacros.h"
#include "FITK_Interface/FITKInterfaceModel/FITKMeshSurface.h"
#include "FITK_Interface/FITKVTKAlgorithm/FITKGraphActor.h"

#include <vtkUnstructuredGrid.h>
#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkProperty.h>

namespace Exchange
{
    Exchange::GraphVTKObjectSurf::GraphVTKObjectSurf(Interface::FITKMeshSurface* surfData, int modeId) :
        GraphVTKObject3D(surfData), _modeId(modeId)
    {
        init();

        if (!generateDataSet()) {
            return;
        }

        generateGraph();

        _renderLayer = 2;
    }

    GraphVTKObjectSurf::~GraphVTKObjectSurf()
    {
        deleteVtkObj(_grid);
    }

    void GraphVTKObjectSurf::highlight(QColor color)
    {
        if (_actor == nullptr) {
            return;
        }
        if (!color.isValid()) {
            color = Qt::red;
        }

        _actor->SetVisibility(true);
        _actor->setColor(color);

        _isHighlighting = true;
    }

    void GraphVTKObjectSurf::disHighlight()
    {
        if (_actor == nullptr) {
            return;
        }
        _actor->SetVisibility(false);

        _isHighlighting = false;
    }

    int GraphVTKObjectSurf::getHightlightRenderLayer()
    {
        return 2;
    }

    void GraphVTKObjectSurf::init()
    {
        _grid = vtkUnstructuredGrid::New();
        _grid->SetPoints(vtkSmartPointer<vtkPoints>::New());
    }

    bool GraphVTKObjectSurf::generateDataSet()
    {
        if (_grid == nullptr) {
            return false;
        }

        GraphDataSetCompomentManager* manager = GraphDataSetCompomentManager::getInstance();
        if (manager == nullptr) {
            return false;
        }
        vtkUnstructuredGrid* setGrid = manager->getSurfGridData(_dataObj->getDataObjectID(), _modeId);
        if (setGrid == nullptr) {
            return false;
        }

        //数据清空
        _grid->Reset();
        _grid->GetPoints()->Reset();
        //数据拷贝
        _grid->DeepCopy(setGrid);

        return true;
    }

    void GraphVTKObjectSurf::generateGraph()
    {
        _actor = FITKGraphActor::New();
        _actor->SetPickable(false);
        _actor->setGraphObject(this);
        _actor->GetProperty()->SetRepresentation(VTK_WIREFRAME);
        _actor->GetProperty()->SetEdgeVisibility(true);
        _actor->GetProperty()->SetLineWidth(GraphVTKCommons::_highlightLineWidth);
        _actor->GetProperty()->SetPointSize(GraphVTKCommons::_highlightPointSize);
        _actor->setInputDataObject(_grid);
        addActor(_actor);
    }
}
