/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "PostGraphObjectPick.h"

#include "FITK_Interface/FITKVTKAlgorithm/FITKGraphActor.h"

#include <vtkDataSet.h>
#include <vtkDataSetSurfaceFilter.h>
#include <vtkProperty.h>
#include <vtkMapper.h>
#include <vtkDataSetMapper.h>
#include <vtkPolyDataMapper.h>
#include <vtkUnstructuredGrid.h>

namespace Interface
{
    PostGraphObjectPick::PostGraphObjectPick(FITKAbstractCFDPostData * postData, Comp::FITKGraph3DWindowVTK * graph3DWidget) :
        PostGraphObjectBase(postData, graph3DWidget)
    {
        _actor = vtkActor::New();
        _actor->GetProperty()->SetColor(1, 0, 0);
        _actor->GetProperty()->SetPointSize(10);
        _actor->GetProperty()->SetLineWidth(3);

        _grid = vtkUnstructuredGrid::New();

        _mapper = vtkDataSetMapper::New();
        _mapper->SetInputData(_grid);

        _actor->SetMapper(_mapper);
        _actor->SetPickable(false);

        this->addActor(_actor);
    }

    PostGraphObjectPick::~PostGraphObjectPick()
    {
        if (_actor) {
            _actor->Delete();
            _actor = nullptr;
        }
    }

    void PostGraphObjectPick::setPickeData(vtkUnstructuredGrid* grid)
    {
        _pickGrid = grid;
    }

    void PostGraphObjectPick::setPickedType(int type)
    {
        if (_actor == nullptr)return;
        if (type == 1)
        {
            // Points.
            _actor->GetProperty()->SetRepresentation(0);
            _actor->GetProperty()->SetVertexVisibility(true);
            _actor->GetProperty()->SetEdgeVisibility(false);
            _actor->GetProperty()->SetOpacity(1.);
        }
        else if (type == 2)
        {
            //// Cell
            //_actor->GetProperty()->SetRepresentation(1);
            //_actor->GetProperty()->SetVertexVisibility(false);
            //_actor->GetProperty()->SetEdgeVisibility(true);
            _actor->GetProperty()->SetOpacity(1.);
            _actor->GetProperty()->SetRepresentationToSurface();
            _actor->GetProperty()->EdgeVisibilityOff();
        }
    }

    void PostGraphObjectPick::setVisible(bool visibility)
    {
        if (_actor){
            _actor->SetVisibility(visibility);
        }
    }

    void PostGraphObjectPick::setColor(QColor color)
    {
        // Set the actor color.
        if (_actor && color.isValid()){
            double color3[3]{ 0., 0., 0. };
            color3[0] = color.redF();
            color3[1] = color.greenF();
            color3[2] = color.blueF();
            _actor->GetProperty()->SetColor(color3);
        }
    }

    void PostGraphObjectPick::update()
    {
        if (_mapper == nullptr || _pickGrid == nullptr || _grid == nullptr)return;

        _grid->DeepCopy(_pickGrid);
        _mapper->Update();
    }

    int PostGraphObjectPick::getRenderLayer()
    {
        return 1;
    }
}

