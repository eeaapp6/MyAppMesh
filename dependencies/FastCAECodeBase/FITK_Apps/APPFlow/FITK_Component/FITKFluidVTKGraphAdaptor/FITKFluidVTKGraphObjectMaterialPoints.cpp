/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKFluidVTKGraphObjectMaterialPoints.h"

// VTK
#include <vtkUnstructuredGrid.h>
#include <vtkMapper.h>
#include <vtkProperty.h>
#include <vtkIdList.h>
#include <vtkCellType.h>
#include <vtkGlyph3D.h>
#include <vtkDistanceToCamera.h>
#include <vtkArrowSource.h>
#include <vtkTransform.h>
#include <vtkTransformFilter.h>
#include <vtkDoubleArray.h>
#include <vtkPointData.h>

// Graph
#include "FITK_Interface/FITKVTKAlgorithm/FITKGraphActor.h"
#include "FITK_Interface/FITKVTKAlgorithm/FITKTransformRepresentation.h"
#include "FITK_Interface/FITKVTKAlgorithm/FITKTransformWidget.h"
#include "FITKFluidVTKCommons.h"

// Data
#include "FITK_Interface/FITKInterfaceMeshGen/FITKZonePoints.h"

namespace Exchange
{
    FITKFluidVTKGraphObjectMaterialPoints::FITKFluidVTKGraphObjectMaterialPoints(Interface::FITKZonePointManager* mPtsMgr)
        : FITKFluidVTKGraphObjectFixedSizeBase(mPtsMgr)
    {
        if (!mPtsMgr)
        {
            return;
        }

        // Initialize.
        init();

        // Generate all material points' geometry.
        bool flag = generateDataSet();
        if (!flag)
        {
            return;
        }

        // Generate the actor.
        generateGraph();

        // Set the layer need to be rendered.
        setRenderLayer(2);
    }

    FITKFluidVTKGraphObjectMaterialPoints::~FITKFluidVTKGraphObjectMaterialPoints()
    {
        // Delete grids.
        deleteVtkObj(m_points);
        deleteVtkObj(m_ugrid);

        deleteVtkObj(m_pointsMarker);
        deleteVtkObj(m_ugridMarker);

        // Delete normals array.
        deleteVtkObj(m_normals);

        // Delete source and filter.
        deleteVtkObj(m_arrowSource);
        deleteVtkObj(m_transFilter);

        // Delete the widget representation only.
        deleteVtkObj(m_moveRep);

        // Set the pointer to NULL.( Deleted in base class. )
        m_glyph = nullptr;
        m_moveWidget = nullptr;
    }

    void FITKFluidVTKGraphObjectMaterialPoints::init()
    {
        // Initialize the glyph size first of all.
        m_glyphSize = 70.;

        // Create the grid data.
        m_ugrid = vtkUnstructuredGrid::New();
        m_points = vtkPoints::New();
        m_ugrid->SetPoints(m_points);

        m_ugridMarker = vtkUnstructuredGrid::New();
        m_pointsMarker = vtkPoints::New();
        m_ugridMarker->SetPoints(m_pointsMarker);

        // Create normals array.
        m_normals = vtkDoubleArray::New();
        m_normals->SetNumberOfComponents(3);
        m_ugridMarker->GetPointData()->SetNormals(m_normals);

        // Create source.
        m_arrowSource = vtkArrowSource::New();
        m_arrowSource->SetTipLength(0.35);
        m_arrowSource->SetTipRadius(0.1);

        // Create the transformer.
        vtkSmartPointer<vtkTransform> trans = vtkSmartPointer<vtkTransform>::New();
        trans->Translate(0.25, 0., 0.);
        trans->Update();

        m_transFilter = vtkTransformFilter::New();
        m_transFilter->SetInputConnection(m_arrowSource->GetOutputPort());
        m_transFilter->SetTransform(trans);

        // Create filter.
        //@{
        // Glyph.
        vtkGlyph3D* glyph = vtkGlyph3D::New();
        vtkDistanceToCamera* distanceToCamera = vtkDistanceToCamera::New();
        distanceToCamera->SetScreenSize(m_glyphSize);

        glyph->SetInputConnection(distanceToCamera->GetOutputPort());

        // Add the point array name need to be used.      
#if VTK_MAJOR_VERSION < 8
        glyph->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS, "DistanceToCamera");
#else
        glyph->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS, distanceToCamera->GetDistanceArrayName());
#endif

        glyph->SetScaling(true);
        glyph->SetScaleModeToScaleByScalar();
        glyph->SetVectorModeToUseNormal();

        // Set the soure data and input data.
        distanceToCamera->SetInputData(m_ugridMarker);
        glyph->SetSourceConnection(m_transFilter->GetOutputPort());

        // Add to data list.
        m_glyphList.push_back(glyph);
        m_distToCameraList.push_back(distanceToCamera);

        m_glyph = glyph;
        //@}

        // Create the movable marker widget.
        m_moveWidget = FITKTransformWidget::New();
        m_moveRep = FITKTransformRepresentation::New();

        // Set the color.
        double colorMark3[3]{ 0., 0., 0. };
        FITKFluidVTKCommons::QColorToDouble3(FITKFluidVTKCommons::s_matPointMarkerColor, colorMark3);
        m_moveRep->GetActorProperty(FITKTransformRepresentation::HandleActorType::HandleXPos)->SetColor(colorMark3);
        m_moveRep->GetActorProperty(FITKTransformRepresentation::HandleActorType::HandleXNeg)->SetColor(colorMark3);
        m_moveRep->GetActorProperty(FITKTransformRepresentation::HandleActorType::HandleYPos)->SetColor(colorMark3);
        m_moveRep->GetActorProperty(FITKTransformRepresentation::HandleActorType::HandleYNeg)->SetColor(colorMark3);
        m_moveRep->GetActorProperty(FITKTransformRepresentation::HandleActorType::HandleZPos)->SetColor(colorMark3);
        m_moveRep->GetActorProperty(FITKTransformRepresentation::HandleActorType::HandleZNeg)->SetColor(colorMark3);

        // The marker size.
        m_moveRep->SetHandleSize(10.);
        m_moveWidget->SetRepresentation(m_moveRep);
        m_moveWidget->EnabledOff();
        addWidget(m_moveWidget);
    }

    void FITKFluidVTKGraphObjectMaterialPoints::setVisible(bool visibility)
    {
        if (!m_fActor || !m_fActorMarker)
        {
            return;
        }

        // Show or hide actors.
        m_fActor->SetVisibility(visibility);
        m_fActorMarker->SetVisibility(m_fActor->GetVisibility() && isAdvancedHighlighting());
    }

    void FITKFluidVTKGraphObjectMaterialPoints::advanceHighlight(FITKFluidVTKCommons::ShapeType type, QVector<int> indice, QColor color)
    {
        Q_UNUSED(type);

        if (!getDataVisibility())
        {
            return;
        }

        // Check the data.
        Interface::FITKZonePointManager* mPtsMgr = dynamic_cast<Interface::FITKZonePointManager*>(_dataObj);
        if (!mPtsMgr)
        {
            return;
        }

        // Check the pointer.
        if (!m_normals || !m_pointsMarker || !m_ugridMarker)
        {
            return;
        }

        // Check the input.
        if (indice.isEmpty())
        {
            return;
        }

        // Reset the marker data.
        clearMarkerData();

        // Generate points and normals.
        for (const int & id : indice)
        {
            Interface::FITKZonePoint* point = mPtsMgr->getDataByID(id);
            if (!point)
            {
                continue;
            }

            // Get the position of the point.
            double pos[3]{ 0., 0., 0. };
            point->getCoor(pos);

            if (m_useMovableMarkerStyle)
            {
                // Insert three points.
                for (int i = 0; i < 3; i++)
                {
                    m_pointsMarker->InsertNextPoint(pos);
                }

                // Insert three normal tuple.
                m_normals->InsertNextTuple3(1., 0., 0.);
                m_normals->InsertNextTuple3(0., 1., 0.);
                m_normals->InsertNextTuple3(0., 0., 1.);

                m_arrowSource->SetInvert(false);

                // Larger arrow.
                m_glyphSize = 70.;
            }
            else
            {
                // Insert six points.
                for (int i = 0; i < 6; i++)
                {
                    m_pointsMarker->InsertNextPoint(pos);
                }

                // Insert six normal tuple.   
                double nor[3]{ 0. ,0., 0. };
                for (int i = 0; i < 3; i++)
                {
                    nor[i] = 1.;
                    m_normals->InsertNextTuple3(nor[0], nor[1], nor[2]);
                    nor[i] = -1.;
                    m_normals->InsertNextTuple3(nor[0], nor[1], nor[2]);
                    nor[i] = 0.;
                }

                // Point to the position.
                m_arrowSource->SetInvert(true);

                // Smaller arrow.
                m_glyphSize = 20.;
            }
        }

        // Update the size.
        this->Superclass::setScreenSize(m_glyphSize);

        // Show the marker actor.
        if (m_fActorMarker)
        {
            m_fActorMarker->SetVisibility(true);
        }

        // Show the widget.
        if (m_moveWidget)
        {
            // Place the widget.
            m_moveRep->PlaceWidget(m_ugridMarker->GetBounds());
            m_moveRep->SetPosition(m_ugridMarker->GetCenter());
            m_moveWidget->EnabledOff(); // Hide the widget in current version.
        }

        // Save the highlight flag.
        FITKFluidVTKGraphObject3D::advanceHighlight(type, indice, color);
    }

    void FITKFluidVTKGraphObjectMaterialPoints::disAdvanceHighlight()
    {
        // Reset the marker data.
        clearMarkerData();

        // Hide the marker actor.
        if (m_fActorMarker)
        {
            m_fActorMarker->SetVisibility(false);
        }

        // Hide the widget.
        if (m_moveWidget)
        {
            m_moveWidget->EnabledOff();
        }

        // Save the highlight flag.
        FITKFluidVTKGraphObject3D::disAdvanceHighlight();
    }

    void FITKFluidVTKGraphObjectMaterialPoints::update(bool forceUpdate)
    {
        // This graph object will be updated forcly every time.
        Q_UNUSED(forceUpdate);

        // Clear data first.
        clearData();

        // Regenerate the data set.
        generateDataSet();
    }

    bool FITKFluidVTKGraphObjectMaterialPoints::generateDataSet()
    {
        Interface::FITKZonePointManager* mPtsMgr = dynamic_cast<Interface::FITKZonePointManager*>(_dataObj);
        if (!mPtsMgr || !m_points || !m_ugrid)
        {
            return false;
        }

        // Generate vertex cells.
        int nPoints = mPtsMgr->getDataCount();

        for (int i = 0; i < nPoints; i++)
        {
            Interface::FITKZonePoint* point = mPtsMgr->getDataByIndex(i);
            if (!point)
            {
                continue;
            }

            // Get the position of the point.
            double pos[3]{ 0., 0., 0. };
            point->getCoor(pos);

            // Add point and cell.
            int ptId = m_points->InsertNextPoint(pos);
            
            vtkSmartPointer<vtkIdList> cell = vtkSmartPointer<vtkIdList>::New();
            cell->InsertNextId(ptId);

            m_ugrid->InsertNextCell(VTKCellType::VTK_VERTEX, cell);
        }

        return true;
    }

    void FITKFluidVTKGraphObjectMaterialPoints::generateGraph()
    {
        if (!m_ugrid)
        {
            return;
        }

        // Get color.
        double colorVert3[3]{ 0., 0., 0. };
        FITKFluidVTKCommons::QColorToDouble3(FITKFluidVTKCommons::s_matPointsColor, colorVert3);

        double colorMark3[3]{ 0., 0., 0. };
        FITKFluidVTKCommons::QColorToDouble3(FITKFluidVTKCommons::s_matPointMarkerColor, colorMark3);

        // Create points actor.
        m_fActor = FITKGraphActor::New();
        m_fActor->setGraphObject(this);
        m_fActor->setActorType(ActorType::VertexActor);
        m_fActor->SetPickable(false);
        m_fActor->SetVisibility(true);
        m_fActor->setScalarVisibility(false);
        m_fActor->GetProperty()->SetRepresentation(1);
        m_fActor->GetProperty()->SetColor(colorVert3);
        m_fActor->GetProperty()->SetPointSize(m_pointSize);
        m_fActor->setInputDataObject(m_ugrid);
        addActor(m_fActor);

        // Create marker actor.
        m_fActorMarker = FITKGraphActor::New();
        m_fActorMarker->setGraphObject(this);
        m_fActorMarker->setActorType(ActorType::OtherActor);
        m_fActorMarker->SetPickable(false);
        m_fActorMarker->SetVisibility(false);
        m_fActorMarker->setScalarVisibility(false);
        m_fActorMarker->GetProperty()->SetRepresentation(2);
        m_fActorMarker->GetProperty()->SetEdgeVisibility(false);
        m_fActorMarker->GetProperty()->SetColor(colorMark3);
        m_fActorMarker->setInputConnection(m_glyph->GetOutputPort());
        addActor(m_fActorMarker);
    }

    void FITKFluidVTKGraphObjectMaterialPoints::clearData()
    {
        // Reset the points grid.
        resetVtkObj(m_points);
        resetVtkObj(m_ugrid);

        // Reset marker's data.
        clearMarkerData();
    }

    void FITKFluidVTKGraphObjectMaterialPoints::clearMarkerData()
    {
        // Reset the grid.
        resetVtkObj(m_pointsMarker);
        resetVtkObj(m_ugridMarker);

        // Reset normals.
        resetVtkObj(m_normals);
    }
}   // namespace Exchange