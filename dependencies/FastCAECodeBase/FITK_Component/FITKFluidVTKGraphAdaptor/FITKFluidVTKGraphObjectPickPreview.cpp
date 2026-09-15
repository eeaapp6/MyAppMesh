/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKFluidVTKGraphObjectPickPreview.h"

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
#include "FITKFluidVTKCommons.h"
#include "FITK_Interface/FITKVTKAlgorithm/FITKGraphActor.h"

namespace Exchange
{
    FITKFluidVTKGraphObjectPickPreview::FITKFluidVTKGraphObjectPickPreview()
        : FITKFluidVTKGraphObject3D(nullptr)
    {
        // Initialize.
        init();

        // Set the layer need to be rendered.
        setRenderLayer(2);

        // Save if the bounds are fixed or dynamic.
        m_hasFixedBounds = false;
    }

    FITKFluidVTKGraphObjectPickPreview::~FITKFluidVTKGraphObjectPickPreview()
    {
        // Delete pointers.
        deleteVtkObj(m_selectNode);
        deleteVtkObj(m_section);
        deleteVtkObj(m_extractSelection);
        deleteVtkObj(m_idArray);
        deleteVtkObj(m_surfaceFilter);
    }

    void FITKFluidVTKGraphObjectPickPreview::init()
    {
        // Initialize the extractor.
        //@{
        m_selectNode = vtkSelectionNode::New();
        m_section = vtkSelection::New();
        m_extractSelection = vtkExtractSelection::New();
        m_selectNode->SetContentType(vtkSelectionNode::INDICES);
        m_selectNode->SetFieldType(vtkSelectionNode::SelectionField::CELL);
        m_section->AddNode(m_selectNode);
        m_extractSelection->SetInputData(1, m_section);

        m_idArray = vtkIdTypeArray::New();
        m_selectNode->SetSelectionList(m_idArray);
        //@}

        // Initialize high-order element filter.
        //@{
        m_surfaceFilter = vtkDataSetSurfaceFilter::New();
        m_surfaceFilter->SetInputConnection(m_extractSelection->GetOutputPort());
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

    void FITKFluidVTKGraphObjectPickPreview::setVisible(bool visibility)
    {
        if (m_fActor)
        {
            m_fActor->SetVisibility(visibility);
        }
    }

    void FITKFluidVTKGraphObjectPickPreview::setPickedData(vtkActor* actor, int type, int index)
    {
        if (!actor || !m_idArray || !m_selectNode || !m_extractSelection || !m_fActor)
        {
            return;
        }

        // Update the extract index array and selection type.
        m_idArray->SetNumberOfValues(0);
        m_idArray->InsertNextValue(index);

        vtkDataSet* dataSet = actor->GetMapper()->GetInputAsDataSet();
        m_extractSelection->SetInputData(dataSet);

        // Update.
        m_extractSelection->Modified();

        // Set the actor properties.
        initActorProperties(m_fActor, type);

        m_fActor->update();
    }

    void FITKFluidVTKGraphObjectPickPreview::setPickedData(vtkDataSet* grid)
    {
        if (!m_fActor || !grid)
        {
            return;
        }

        // Set the full appended picked dataset.
        m_fActor->setInputDataObject(grid);
        initActorProperties(m_fActor, 0);
    }

    void FITKFluidVTKGraphObjectPickPreview::setPickedData(vtkDataSet* dataSet, int type, int index)
    {
        if (!dataSet || !m_idArray || !m_selectNode || !m_extractSelection || !m_fActor)
        {
            return;
        }

        if (type != 0 && type != 1)
        {
            return;
        }

        // Update the extract index array and selection type.
        m_idArray->SetNumberOfValues(0);
        m_idArray->InsertNextValue(index);
        m_extractSelection->SetInputData(dataSet);

        m_selectNode->SetFieldType(vtkSelectionNode::SelectionField(type));

        // Update.
        m_extractSelection->Modified();

        // Set the actor properties.
        initActorProperties(m_fActor, type);

        m_fActor->update();
    }

    void FITKFluidVTKGraphObjectPickPreview::setPickedType(int type)
    {
        initActorProperties(m_fActor, type);
    }

    void FITKFluidVTKGraphObjectPickPreview::initActorProperties(vtkProp* actor, QVariant details)
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

        fActor->GetProperty()->SetColor(1., 0.5, 0.);

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

        // Points. ( Should be smaller than pick size )
        fActor->GetProperty()->SetPointSize(FITKFluidVTKCommons::s_highlightPointSize - 1.);

        // Edge. ( Should be smaller than pick size )
        fActor->GetProperty()->SetLineWidth(FITKFluidVTKCommons::s_highlightLineWidth - 1.);
    }
}   // namespace Exchange