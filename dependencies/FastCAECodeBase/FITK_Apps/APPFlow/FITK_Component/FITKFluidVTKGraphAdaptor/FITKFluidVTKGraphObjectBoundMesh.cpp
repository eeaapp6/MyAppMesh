/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKFluidVTKGraphObjectBoundMesh.h"

// VTK
#include <vtkDataSet.h>
#include <vtkMapper.h>
#include <vtkProperty.h>
#include <vtkUnstructuredGrid.h>

// Graph and filter
#include "FITK_Interface/FITKVTKAlgorithm/FITKGraphActor.h"
#include "FITKFluidVTKCommons.h"
#include "FITKFluidVTKGraphObjectSelect.h"

// Data
#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredFluidMeshVTK.h"

namespace Exchange
{
    FITKFluidVTKGraphObjectBoundMesh::FITKFluidVTKGraphObjectBoundMesh(Interface::FITKBoundaryMeshVTK* meshData)
        : FITKFluidVTKGraphObject3D(meshData)
    {
        if (!meshData)
        {
            return;
        }

        // Initialize the polygon offset.
        m_polygonOffset = FITKFluidVTKCommons::s_polygonOffsetMesh;

        // Initialize.
        init();

        // Generate the mesh actor.
        generateGraph();

        // Set the layer need to be rendered.
        setRenderLayer(0, 2);

        // Save the data type.
        m_shapeInfo.Type = FITKFluidVTKCommons::MeshShape;
    }

    void FITKFluidVTKGraphObjectBoundMesh::init()
    {
        // Initialize view properties.
        FITKFluidVTKGraphObject3D::setViewMode(FITKFluidVTKCommons::ShapeMeshViewMode::SMVM_Vertex, false);

        // Create the highlight selector.
        m_highlightSelector = new FITKFluidVTKGraphObjectSelect;
        m_addinGraphObjList.push_back(m_highlightSelector);
    }

    void FITKFluidVTKGraphObjectBoundMesh::update(bool forceUpdate)
    {
        Q_UNUSED(forceUpdate);

        // Update the actor.( Modify input. )
        if (m_fActor)
        {
            m_fActor->update();
        }

        // Update visibility only.
        updateVisibility();
    }

    void FITKFluidVTKGraphObjectBoundMesh::setVisible(bool visibility)
    {
        // The mesh.
        if (m_fActor)
        {
            m_fActor->SetVisibility(visibility && getDataVisibility());
        }

        // The highlightor.
        if (m_highlightSelector)
        {
            m_highlightSelector->setVisible(visibility && getDataVisibility() && (m_isHighlighting || m_isAdvHighlighting));
        }
    }

    void FITKFluidVTKGraphObjectBoundMesh::setColor(QColor color)
    {
        // Set the actor color.
        if (m_fActor && color.isValid())
        {
            double color3[3]{ 0., 0., 0. };
            FITKFluidVTKCommons::QColorToDouble3(color, color3);
            m_fActor->GetProperty()->SetColor(color3);
        }
    }

    void FITKFluidVTKGraphObjectBoundMesh::setViewMode(FITKFluidVTKCommons::ShapeMeshViewMode type, bool visible)
    {
        if (!m_fActor)
        {
            return;
        }

        // Save the property first.
        FITKFluidVTKGraphObject3D::setViewMode(type, visible);

        // Check the view properties.
        bool showFace = m_viewModes[FITKFluidVTKCommons::ShapeMeshViewMode::SMVM_Shade];
        bool showEdge = m_viewModes[FITKFluidVTKCommons::ShapeMeshViewMode::SMVM_Wireframe];
        bool showVert = m_viewModes[FITKFluidVTKCommons::ShapeMeshViewMode::SMVM_Vertex];

        // Set the representation.
        if (showFace)
        {
            m_fActor->GetProperty()->SetRepresentation(2);
            m_fActor->GetProperty()->SetEdgeVisibility(showEdge);
            m_fActor->GetProperty()->SetVertexVisibility(showVert);
            return;
        }

        if (showEdge && !showFace)
        {
            m_fActor->GetProperty()->SetRepresentation(1);
            m_fActor->GetProperty()->SetVertexVisibility(showVert);
            return;
        }

        if (showVert && !showEdge && !showFace)
        {
            m_fActor->GetProperty()->SetRepresentation(0);
            m_fActor->GetProperty()->SetEdgeVisibility(false);
            return;
        }
    }

    void FITKFluidVTKGraphObjectBoundMesh::setTransparent(bool isOn)
    {
        // Get the opacity.
        double opa = isOn ? 1. - FITKFluidVTKCommons::s_transparency : 1.;

        if (m_fActor)
        {
            m_fActor->GetProperty()->SetOpacity(opa);
        }
    }

    void FITKFluidVTKGraphObjectBoundMesh::highlight(FITKFluidVTKCommons::ShapeType type, QColor color)
    {
        // Show the actor and initialize the data.
        if (!m_highlightSelector || !getDataVisibility())
        {
            return;
        }

        // Check the model shape type.
        switch (type)
        {
        case FITKFluidVTKCommons::MeshNode:
        case FITKFluidVTKCommons::MeshElement:
            break;
        case FITKFluidVTKCommons::ShapeTypeNone:
            // Default highlight all elements.
            type = FITKFluidVTKCommons::MeshElement;
            break;
        case FITKFluidVTKCommons::ModelVertex:
        case FITKFluidVTKCommons::ModelEdge:
        case FITKFluidVTKCommons::ModelFace:
        case FITKFluidVTKCommons::ModelSolid:
        default:
            return;
        }

        vtkDataSet* meshDataSet = getMesh(type);
        if (!meshDataSet)
        {
            return;
        }

        QVector<int> indice;
        for (int i = 0; i < meshDataSet->GetNumberOfCells(); i++)
        {
            indice.push_back(i);
        }

        // Check the color.
        if (!color.isValid())
        {
            color = Qt::red;
        }

        m_highlightSelector->setColor(color);
        m_highlightSelector->setSelectData(meshDataSet, indice, type);
        m_highlightSelector->setVisible(getDataVisibility());

        // Save the highlight flag.
        FITKFluidVTKGraphObject3D::highlight();
    }

    void FITKFluidVTKGraphObjectBoundMesh::disHighlight()
    {
        // Hide the actor only.
        if (m_highlightSelector)
        {
            m_highlightSelector->setVisible(false);
        }

        // Save the highlight flag.
        FITKFluidVTKGraphObject3D::disHighlight();
    }

    void FITKFluidVTKGraphObjectBoundMesh::advanceHighlight(FITKFluidVTKCommons::ShapeType type, QVector<int> indice, QColor color)
    {
        // Show the actor and initialize the data.
        if (!m_highlightSelector)
        {
            return;
        }

        if (indice.isEmpty())
        {
            return;
        }

        // Check the model shape type.
        switch (type)
        {
        case FITKFluidVTKCommons::MeshNode:
        case FITKFluidVTKCommons::MeshElement:
            break;
        case FITKFluidVTKCommons::ShapeTypeNone:
        case FITKFluidVTKCommons::ModelVertex:
        case FITKFluidVTKCommons::ModelEdge:
        case FITKFluidVTKCommons::ModelFace:
        case FITKFluidVTKCommons::ModelSolid:
        default:
            return;
        }

        vtkDataSet* meshDataSet = getMesh(type);
        if (!meshDataSet)
        {
            return;
        }

        // Check the color.
        if (!color.isValid())
        {
            color = Qt::red;
        }

        m_highlightSelector->setColor(color);
        m_highlightSelector->setSelectData(meshDataSet, indice, type);
        m_highlightSelector->setVisible(getDataVisibility());

        // Save the highlight flag.
        FITKFluidVTKGraphObject3D::advanceHighlight(type, indice, color);
    }

    void FITKFluidVTKGraphObjectBoundMesh::disAdvanceHighlight()
    {
        // Clear data and hide the actor.
        if (m_highlightSelector)
        {
            m_highlightSelector->clearData();
            m_highlightSelector->setVisible(false);
        }

        // Save the highlight flag.
        FITKFluidVTKGraphObject3D::disAdvanceHighlight();
    }

    vtkDataSet* FITKFluidVTKGraphObjectBoundMesh::getMesh(FITKFluidVTKCommons::ShapeType type)
    {
        // The mesh object only has one data set for both nodes and elements.
        Q_UNUSED(type);

        Interface::FITKBoundaryMeshVTK* meshData = dynamic_cast<Interface::FITKBoundaryMeshVTK*>(_dataObj);
        if (!meshData)
        {
            return nullptr;
        }

        return meshData->getGrid();
    }

    void FITKFluidVTKGraphObjectBoundMesh::generateGraph()
    {
        Interface::FITKBoundaryMeshVTK* meshData = dynamic_cast<Interface::FITKBoundaryMeshVTK*>(_dataObj);
        if (!meshData)
        {
            return;
        }

        // Check the grid data.
        vtkUnstructuredGrid* ugrid = meshData->getGrid();
        if (!ugrid)
        {
            return;
        }

        double color3[3]{ 0., 0., 0. };
        FITKFluidVTKCommons::QColorToDouble3(FITKFluidVTKCommons::GetRandomColor(), color3);

        // Create actor.
        m_fActor = FITKGraphActor::New();
        m_fActor->setGraphObject(this);
        m_fActor->setActorType(ActorType::SurfaceActor);
        m_fActor->SetPickable(false);
        m_fActor->SetVisibility(true);
        m_fActor->setScalarVisibility(false);
        m_fActor->GetProperty()->SetRepresentation(2);
        m_fActor->GetProperty()->SetEdgeVisibility(true);
        m_fActor->GetProperty()->SetColor(color3);
        m_fActor->setInputDataObject(ugrid);
        m_fActor->setRelativeCoincidentTopologyPolygonOffsetParameters(m_polygonOffset);
        addActor(m_fActor);
    }
}   // namespace Exchange