/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GraphVTKObjectPick.h"

#include "FITK_Interface/FITKVTKAlgorithm/FITKGraphActor.h"

#include <vtkDataSet.h>
#include <vtkMapper.h>
#include <vtkProperty.h>
#include <vtkSelectionNode.h>
#include <vtkSelection.h>
#include <vtkExtractSelection.h>
#include <vtkIdTypeArray.h>
#include <vtkDataSetSurfaceFilter.h>

namespace Exchange
{
    GraphVTKObjectPick::GraphVTKObjectPick()
        : GraphVTKObject3D(nullptr)
    {
        init();
        m_hasFixedBounds = false;
        _renderLayer = 2;
    }

    GraphVTKObjectPick::~GraphVTKObjectPick()
    {
        // Delete pointers.
        deleteVtkObj(m_surfaceFilter);
    }

    void GraphVTKObjectPick::init()
    {
        m_surfaceFilter = vtkDataSetSurfaceFilter::New();
        // m_surfaceFilter->SetPassThroughCellIds(true);
        m_surfaceFilter->SetNonlinearSubdivisionLevel(1);

        m_fActor = FITKGraphActor::New();
        m_fActor->setGraphObject(this);
        m_fActor->SetPickable(false);
        m_fActor->setInputConnection(m_surfaceFilter->GetOutputPort());
        addActor(m_fActor);
    }

    void GraphVTKObjectPick::setVisible(bool visibility)
    {
        if (m_fActor)
        {
            m_fActor->SetVisibility(visibility);
        }
    }

    void GraphVTKObjectPick::setPickedData(vtkDataSet* grid)
    {
        if (!m_surfaceFilter || !m_fActor || !grid)
        {
            return;
        }

        // Set the full appended picked dataset.
        //m_fActor->setInputDataObject(grid);
        m_surfaceFilter->SetInputData(grid);
        initActorProperties(m_fActor);
    }

    void GraphVTKObjectPick::setPickedType(int type)
    {
        initActorProperties(m_fActor, type);
    }

    void GraphVTKObjectPick::setColor(QColor color)
    {
        // Set the actor color.
        if (m_fActor && color.isValid())
        {
            m_fActor->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());
        }
    }

    void GraphVTKObjectPick::initActorProperties(vtkProp* actor, QVariant details)
    {
        if (!actor){
            return;
        }

        // Set model actor properties.
        // Default show face and edge.
        FITKGraphActor* fActor = FITKGraphActor::SafeDownCast(actor);
        if (!fActor)
        {
            return;
        }

        // Red wireframe or point.
        fActor->setScalarVisibility(false);
        fActor->SetPickable(false);

        fActor->GetProperty()->SetColor(1., 0., 0.);

        if (details.toInt() == 1)
        {
            // Points.
            fActor->GetProperty()->SetRepresentation(0);
            fActor->GetProperty()->SetVertexVisibility(true);
            fActor->GetProperty()->SetEdgeVisibility(false);
            fActor->GetProperty()->SetOpacity(1.);
            // Points. ( Should be larger than pick preview size )
            fActor->GetProperty()->SetPointSize(GraphVTKCommons::_highlightPointSize);
        }
        else if (details.toInt() == 2)
        {
            // Wireframe.
            fActor->GetProperty()->SetRepresentation(1);
            fActor->GetProperty()->SetVertexVisibility(false);
            fActor->GetProperty()->SetEdgeVisibility(true);
            fActor->GetProperty()->SetOpacity(1.);
            // Edge. ( Should be bigger than pick preview size )
            fActor->GetProperty()->SetLineWidth(GraphVTKCommons::_highlightLineWidth);
        }
        else if (details.toInt() == 3)
        {
            // Surface and wireframe.
            fActor->GetProperty()->SetPointSize(.0);
            fActor->GetProperty()->SetRepresentation(2);
            fActor->GetProperty()->SetVertexVisibility(false);
            fActor->GetProperty()->SetEdgeVisibility(false);
            fActor->GetProperty()->SetOpacity(1 - GraphVTKCommons::_pickTransparency);
        }
    }
}   // namespace Exchange