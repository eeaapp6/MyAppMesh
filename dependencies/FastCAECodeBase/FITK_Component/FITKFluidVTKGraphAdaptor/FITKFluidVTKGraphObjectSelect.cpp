/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKFluidVTKGraphObjectSelect.h"

// VTK
#include <vtkDataSet.h>
#include <vtkMapper.h>
#include <vtkProperty.h>
#include <vtkSelectionNode.h>
#include <vtkSelection.h>
#include <vtkExtractSelection.h>
#include <vtkIdTypeArray.h>

// Graph and filter
#include "FITK_Interface/FITKVTKAlgorithm/FITKGraphActor.h"
#include "FITKFluidVTKCommons.h"

namespace Exchange
{
    FITKFluidVTKGraphObjectSelect::FITKFluidVTKGraphObjectSelect()
        : FITKFluidVTKGraphObject3D(nullptr)
    {
        // Initialize.
        init();

        // Set the layer need to be rendered.
        setRenderLayer(2);

        // Save if the bounds are fixed or dynamic.
        m_hasFixedBounds = false;
    }

    FITKFluidVTKGraphObjectSelect::~FITKFluidVTKGraphObjectSelect()
    {
        // Delete pointers.
        deleteVtkObj(m_selectNode);
        deleteVtkObj(m_section);
        deleteVtkObj(m_idArray);
        deleteVtkObj(m_extractSelection);
    }

    void FITKFluidVTKGraphObjectSelect::init()
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

        // Create actor.
        //@{
        m_fActor = FITKGraphActor::New();
        m_fActor->setGraphObject(this);
        m_fActor->SetPickable(false);

        // Point size and line width.
        m_fActor->GetProperty()->SetPointSize(FITKFluidVTKCommons::s_highlightPointSize);
        m_fActor->GetProperty()->SetLineWidth(FITKFluidVTKCommons::s_highlightLineWidth);

        // Transparent.
        m_fActor->GetProperty()->SetOpacity(1 - FITKFluidVTKCommons::s_transparency);

        m_fActor->setInputConnection(m_extractSelection->GetOutputPort());
        addActor(m_fActor);
        //@}
    }

    void FITKFluidVTKGraphObjectSelect::setVisible(bool visibility)
    {
        if (m_fActor)
        {
            m_fActor->SetVisibility(visibility);
        }
    }

    void FITKFluidVTKGraphObjectSelect::setTransparent(bool isOn)
    {
        // Get the opacity.
        double opa = isOn ? 1. - FITKFluidVTKCommons::s_transparency : 1.;

        if (m_fActor)
        {
            m_fActor->GetProperty()->SetOpacity(opa);
        }
    }

    void FITKFluidVTKGraphObjectSelect::setViewMode(FITKFluidVTKCommons::ShapeMeshViewMode type, bool visible)
    {
        if (!m_fActor)
        {
            return;
        }

        // Show the full actor first.
        m_fActor->SetVisibility(true);

        switch (type)
        {
        case Exchange::FITKFluidVTKCommons::SMVM_Shade:
            if (visible)
            {
                m_fActor->GetProperty()->SetRepresentation(2);
            }
            else
            {
                if (m_viewModes[Exchange::FITKFluidVTKCommons::SMVM_Wireframe])
                {
                    m_fActor->GetProperty()->SetRepresentation(1);
                }
                else if (m_viewModes[Exchange::FITKFluidVTKCommons::SMVM_Vertex])
                {
                    m_fActor->GetProperty()->SetRepresentation(0);
                }
                else
                {
                    m_fActor->SetVisibility(false);
                }
            }
            
            break;
        case Exchange::FITKFluidVTKCommons::SMVM_Wireframe:
            m_fActor->GetProperty()->SetEdgeVisibility(visible);
            break;
        case Exchange::FITKFluidVTKCommons::SMVM_Vertex:
            m_fActor->GetProperty()->SetVertexVisibility(visible);
            break;
        default:
            break;
        }

        // Save flag.
        FITKFluidVTKGraphObject3D::setViewMode(type, visible);
    }

    void FITKFluidVTKGraphObjectSelect::setSelectData(vtkDataSet* grid, QVector<int> & indice, FITKFluidVTKCommons::ShapeType type)
    {
        if (!m_extractSelection || !m_selectNode || !m_idArray || !m_fActor || !grid)
        {
            return;
        }

        // Check the model shape or mesh type.
        switch (type)
        {
        case FITKFluidVTKCommons::MeshNode:
        {
            m_selectNode->SetFieldType(vtkSelectionNode::SelectionField::POINT);
            break;
        }
        case FITKFluidVTKCommons::ModelVertex:
        case FITKFluidVTKCommons::ModelEdge:
        case FITKFluidVTKCommons::ModelFace:
        case FITKFluidVTKCommons::ModelSolid:
        case FITKFluidVTKCommons::MeshElement:
        case FITKFluidVTKCommons::Others:
        {
            m_selectNode->SetFieldType(vtkSelectionNode::SelectionField::CELL);
            break;
        }
        case FITKFluidVTKCommons::ShapeTypeNone:
        default:
            return;
        }

        // Reset the indice array.
        m_idArray->Reset();
        m_idArray->SetNumberOfValues(0);

        for (const int & id : indice)
        {
            m_idArray->InsertNextValue(id);
        }

        // Set the full appended picked dataset.
        //m_fActor->setInputDataObject(grid);
        m_extractSelection->SetInputData(grid);
        m_selectNode->Modified();
        m_fActor->update();

        // Initialize the actor properties.
        initActorProperties(m_fActor, type);
    }

    void FITKFluidVTKGraphObjectSelect::setColor(QColor color)
    {
        // Set the actor color.
        if (m_fActor && color.isValid())
        {
            double color3[3]{ 0., 0., 0. };
            FITKFluidVTKCommons::QColorToDouble3(color, color3);
            m_fActor->GetProperty()->SetColor(color3);
            m_fActor->GetProperty()->SetEdgeColor(color3);
            m_fActor->GetProperty()->SetVertexColor(color3);
        }
    }

    void FITKFluidVTKGraphObjectSelect::clearData()
    {
        // Reset the indice array.
        resetVtkObj(m_idArray);

        if (m_selectNode)
        {
            m_selectNode->Modified();
        }

        // Reset the input data set.
        if (m_extractSelection)
        {
            m_extractSelection->RemoveAllInputs();
            m_extractSelection->RemoveAllInputConnections(0);
        }

        // Update the actor.
        if (m_fActor)
        {
            m_fActor->update();
            m_fActor->SetVisibility(false);
        }
    }

    void FITKFluidVTKGraphObjectSelect::initActorProperties(vtkProp* actor, QVariant details)
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

        switch (details.toInt())
        {
        case FITKFluidVTKCommons::ModelVertex:
        case FITKFluidVTKCommons::MeshNode:
        {
            // Points.
            fActor->GetProperty()->SetRepresentation(0);
            break;
        }
        case FITKFluidVTKCommons::ModelEdge:
        case FITKFluidVTKCommons::MeshElement:
        {
            // Wireframe.
            fActor->GetProperty()->SetRepresentation(1);
            fActor->GetProperty()->SetEdgeVisibility(true);
            break;
        }
        case FITKFluidVTKCommons::ModelFace:
        case FITKFluidVTKCommons::ModelSolid:
        {
            // Surface.
            fActor->GetProperty()->SetRepresentation(2);
            fActor->GetProperty()->SetEdgeVisibility(false);
            break;
        }
        case FITKFluidVTKCommons::Others:
        default:
            return;
        }
    }
}   // namespace Exchange