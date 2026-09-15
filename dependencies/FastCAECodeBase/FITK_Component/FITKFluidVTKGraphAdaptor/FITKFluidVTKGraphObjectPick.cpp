/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKFluidVTKGraphObjectPick.h"

// VTK
#include <vtkDataSet.h>
#include <vtkMapper.h>
#include <vtkProperty.h>
#include <vtkSelectionNode.h>
#include <vtkSelection.h>
#include <vtkExtractSelection.h>
#include <vtkIdTypeArray.h>
#include <vtkDataSetSurfaceFilter.h>

// Graph and filter
#include "FITK_Interface/FITKVTKAlgorithm/FITKGraphActor.h"

namespace Exchange
{
    FITKFluidVTKGraphObjectPick::FITKFluidVTKGraphObjectPick()
        : FITKFluidVTKGraphObject3D(nullptr)
    {
        // Initialize.
        init();

        // Set the layer need to be rendered.
        setRenderLayer(2);

        // Save if the bounds are fixed or dynamic.
        m_hasFixedBounds = false;
    }

    FITKFluidVTKGraphObjectPick::~FITKFluidVTKGraphObjectPick()
    {
        // Delete pointers.
        deleteVtkObj(m_surfaceFilter);
    }

    void FITKFluidVTKGraphObjectPick::init()
    {
        // Initialize high-order element filter.
        //@{
        m_surfaceFilter = vtkDataSetSurfaceFilter::New();
        //@}

        // Create actor.
        //@{
        m_fActor = FITKGraphActor::New();
        m_fActor->setGraphObject(this);
        m_fActor->SetPickable(false);
        m_fActor->setInputConnection(m_surfaceFilter->GetOutputPort());
        addActor(m_fActor);
        //@}
    }

    void FITKFluidVTKGraphObjectPick::setVisible(bool visibility)
    {
        if (m_fActor)
        {
            m_fActor->SetVisibility(visibility);
        }
    }

    void FITKFluidVTKGraphObjectPick::setPickedData(vtkDataSet* grid)
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

    void FITKFluidVTKGraphObjectPick::setPickedType(int type)
    {
        initActorProperties(m_fActor, type);
    }

    void FITKFluidVTKGraphObjectPick::setColor(QColor color)
    {
        // Set the actor color.
        if (m_fActor && color.isValid())
        {
            double color3[3]{ 0., 0., 0. };
            FITKFluidVTKCommons::QColorToDouble3(color, color3);
            m_fActor->GetProperty()->SetColor(color3);
        }
    }

    void FITKFluidVTKGraphObjectPick::initActorProperties(vtkProp* actor, QVariant details)
    {
        if (!actor)
        {
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
        }
        else if (details.toInt() == 2)
        {
            // Wireframe.
            fActor->GetProperty()->SetRepresentation(1);
            fActor->GetProperty()->SetVertexVisibility(false);
            fActor->GetProperty()->SetEdgeVisibility(true);
            fActor->GetProperty()->SetOpacity(1.);
        }
        else if (details.toInt() == 3)
        {
            // Surface and wireframe.
            fActor->GetProperty()->SetRepresentation(2);
            fActor->GetProperty()->SetVertexVisibility(false);
            fActor->GetProperty()->SetEdgeVisibility(false);
            fActor->GetProperty()->SetOpacity(1 - FITKFluidVTKCommons::s_transparency);
        }

        // Points. ( Should be larger than pick preview size )
        fActor->GetProperty()->SetPointSize(FITKFluidVTKCommons::s_highlightPointSize);

        // Edge. ( Should be bigger than pick preview size )
        fActor->GetProperty()->SetLineWidth(FITKFluidVTKCommons::s_highlightLineWidth);
    }
}   // namespace Exchange