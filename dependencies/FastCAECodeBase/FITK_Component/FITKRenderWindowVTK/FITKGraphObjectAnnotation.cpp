/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKGraphObjectAnnotation.h"

// VTK
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkPolyData.h>
#include <vtkProperty2D.h>
#include <vtkTextProperty.h>
#include <vtkActor2D.h>
#include <vtkTextActor.h>
#include <vtkPolyDataMapper2D.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorObserver.h>
#include <vtkRenderer.h>
#include <vtkAlgorithmOutput.h>

// C++
#include <QtMath>
#include <cmath>
#include <QDebug>

// Data
#include "FITKGraphAnnotation.h"
#include "FITKGraph3DWindowVTK.h"

namespace Comp
{
    FITKGraphObjectAnnotation* FITKGraphObjectAnnotation::New(FITKGraphAnnotation* annotation)
    {
        if (!annotation)
        {
            return nullptr;
        }

        FITKGraphObjectAnnotation* instance = new FITKGraphObjectAnnotation(annotation);
        return instance;
    }

    FITKGraphObjectAnnotation::FITKGraphObjectAnnotation(FITKGraphAnnotation* annotation)
        : FITKGraphObjectVTK(annotation)
    {

    }

    FITKGraphObjectAnnotation::~FITKGraphObjectAnnotation()
    {
        // Delete vtk objects.
        if (m_pointsArrow)
        {
            m_pointsArrow->Delete();
            m_pointsArrow = nullptr;
        }

        if (m_cellsArrow)
        {
            m_cellsArrow->Delete();
            m_cellsArrow = nullptr;
        }

        if (m_polyDataArrow)
        {
            m_polyDataArrow->Delete();
            m_polyDataArrow = nullptr;
        }
    }

    void FITKGraphObjectAnnotation::init()
    {
        // Create the vtk objects.
        m_pointsArrow = vtkPoints::New();
        m_cellsArrow = vtkCellArray::New();
        m_polyDataArrow = vtkPolyData::New();
        m_polyDataArrow->SetPoints(m_pointsArrow);
        m_polyDataArrow->SetLines(m_cellsArrow);

        // Initialize the arrow geometry.
        m_pointsArrow->InsertNextPoint(0., 0., 0.); // The start point of the arrow.
        m_pointsArrow->InsertNextPoint(0., 0., 0.); // The end point of the arrow.
        m_pointsArrow->InsertNextPoint(0., 0., 0.); // The first point of the arrow head.
        m_pointsArrow->InsertNextPoint(0., 0., 0.); // The second point of the arrow head.

        vtkIdType line1[2] = { 0, 1 };
        vtkIdType line2[2] = { 1, 2 };
        vtkIdType line3[2] = { 1, 3 };
        m_cellsArrow->InsertNextCell(2, line1);
        m_cellsArrow->InsertNextCell(2, line2);
        m_cellsArrow->InsertNextCell(2, line3);
    }

    void FITKGraphObjectAnnotation::initializeObject()
    {
        if (m_initialized)
        {
            return;
        }

        FITKGraphAnnotation* annotation = dynamic_cast<FITKGraphAnnotation*>(_dataObj);
        if (!annotation)
        {
            return;
        }

        FITKGraph3DWindowVTK* graphWindow = annotation->getGraph3DWindow();
        if (!graphWindow)
        {
            return;
        }

        // Initialize.
        init();

        // Generate the actors.
        generateGraph();

        // Save if the bounds are fixed or dynamic.
        m_hasFixedBounds = false;
        m_constFlag = true;
        m_initialized = true;

        // Update the actors.
        update();
    }

    void FITKGraphObjectAnnotation::setVisible(bool visibility)
    {
        if (m_fActorArrow)
        {
            m_fActorArrow->SetVisibility(visibility);
        }
    }

    bool FITKGraphObjectAnnotation::getVisibility()
    {
        if (m_fActorArrow && m_fActorArrow->GetVisibility())
        {
            return true;
        }

        if (m_fActorText && m_fActorText->GetVisibility())
        {
            return true;
        }

        return false;
    }

    void FITKGraphObjectAnnotation::updateProperties()
    {
        FITKGraphAnnotation* annotation = dynamic_cast<FITKGraphAnnotation*>(_dataObj);
        if (!annotation)
        {
            return;
        }

        if (!m_fActorArrow || !m_fActorText)
        {
            return;
        }

        bool dataVisible = annotation->isEnable();

        // Update the arrow.
        m_fActorArrow->SetVisibility(annotation->getWithArrow() && dataVisible);

        // Update the text.
        m_fActorText->SetVisibility(annotation->getWithText() && dataVisible);
        m_fActorText->SetInput(annotation->getText().toUtf8());

        // Update the properties.
        annotation->updateProperty(m_fActorArrow->GetProperty());
        annotation->updateProperty(m_fActorText->GetTextProperty());
    }

    void FITKGraphObjectAnnotation::updatePositions()
    {
        FITKGraphAnnotation* annotation = dynamic_cast<FITKGraphAnnotation*>(_dataObj);
        if (!annotation)
        {
            return;
        }

        FITKGraph3DWindowVTK* graphWindow = annotation->getGraph3DWindow();
        if (!graphWindow)
        {
            return;
        }

        if (!annotation->isEnable())
        {
            return;
        }

        // Get the upper renderer.
        vtkRenderer* renderer = graphWindow->getVTKRenderer(graphWindow->getRenderCount() - 1);

        if (!renderer || !m_pointsArrow || !m_fActorText)
        {
            return;
        }

        // Get the two anchors.
        const AnnoAnchor& startAnchor = annotation->getStartAnchor();
        const AnnoAnchor& endAnchor = annotation->getEndAnchor();

        // Get the screen positions of two anchors.
        double pos1[3]{ 0., 0., 0. };
        double pos2[3]{ 0., 0., 0. };

        // Get the viewport size.
        int* sz2 = renderer->GetSize();
        vtkRenderWindow* rw = renderer->GetRenderWindow();
        if (rw)
        {
            sz2 = rw->GetSize();
        }

        if (sz2[0] == 0)
        {
            sz2[0] = 1.;
        }

        if (sz2[1] == 0)
        {
            sz2[1] = 1.;
        }

        vtkRenderWindow* renderWindow = graphWindow->getVTKRenderWindow();
        vtkRenderWindowInteractor* interactor = renderWindow->GetInteractor();

        // Update by mouse event.
        if (interactor && m_movingStart)
        {
            int* ePos2 = interactor->GetEventPosition();
            pos1[0] = ePos2[0];
            pos1[1] = ePos2[1];

            double pr2[2]{ double(ePos2[0]) / double(sz2[0]) * 100., double(ePos2[1]) / double(sz2[1]) * 100. };
            annotation->setEventPostionRatio(pr2);
        }
        else
        {
            switch (startAnchor.Type)
            {
            case ViewportRatioXY:
            {
                pos1[0] = startAnchor.Pos[0] * sz2[0] * 0.01;
                pos1[1] = startAnchor.Pos[1] * sz2[1] * 0.01;
                break;
            }
            case WorldXYZ:
            default:
            {
                vtkInteractorObserver::ComputeWorldToDisplay(
                    renderer, startAnchor.Pos[0], startAnchor.Pos[1], startAnchor.Pos[2], pos1);
                break;
            }
            }
        }

        // Update by mouse event.
        if (interactor && m_movingEnd)
        {
            int* ePos2 = interactor->GetEventPosition();
            pos2[0] = ePos2[0];
            pos2[1] = ePos2[1];

            double pr2[2]{ double(ePos2[0]) / double(sz2[0]) * 100., double(ePos2[1]) / double(sz2[1]) * 100. };
            annotation->setEventPostionRatio(pr2);
        }
        else
        {
            switch (endAnchor.Type)
            {
            case ViewportRatioXY:
            {
                pos2[0] = endAnchor.Pos[0] * sz2[0] * 0.01;
                pos2[1] = endAnchor.Pos[1] * sz2[1] * 0.01;
                break;
            }
            case WorldXYZ:
            default:
            {
                vtkInteractorObserver::ComputeWorldToDisplay(
                    renderer, endAnchor.Pos[0], endAnchor.Pos[1], endAnchor.Pos[2], pos2);
                break;
            }
            }
        }

        // Update the text actor position.
        double textSz2[2]{ 0., 0. };
        m_fActorText->GetSize(renderer, textSz2);

        TextAlign aType = annotation->getTextAlign();
        switch (aType)
        {
        case TA_Left:
            m_fActorText->SetPosition((pos1[0] - textSz2[0] * 0.5) / sz2[0],
                (pos1[1] - textSz2[1] * 0.5) / sz2[1]);
            break;
        case TA_Center:
            m_fActorText->SetPosition(pos1[0] / sz2[0], 
                (pos1[1] - textSz2[1] * 0.5) / sz2[1]);
            break;
        case TA_Right:
            m_fActorText->SetPosition((pos1[0] + textSz2[0] * 0.5) / sz2[0],
                (pos1[1] - textSz2[1] * 0.5) / sz2[1]);
            break;
        default:
            break;
        }

        bool isTextVisible = m_fActorText->GetVisibility();

        // Update the arrow points.
        double dir3[3]{ pos2[0] - pos1[0], pos2[1] - pos1[1], 0. };
        vtkMath::Normalize(dir3);

        if (isTextVisible)
        {
            double margin = 4.;
            pos1[0] += dir3[0] * (textSz2[0] * 0.5 + margin);
            pos1[1] += dir3[1] * (textSz2[1] * 0.5 + margin);
        }

        m_pointsArrow->SetPoint(0, pos1[0], pos1[1], 0.);
        m_pointsArrow->SetPoint(1, pos2[0], pos2[1], 0.);

        for (int i = 0; i < 3; i++)
        {
            dir3[i] = pos2[i] - pos1[i];
        }

        double pos3[3]{ pos2[0], pos2[1], 0. };
        double pos4[3]{ pos2[0], pos2[1], 0. };

        // Get the arrow direction, and calculate the other two positions.
        double dirLen = std::sqrt(dir3[0] * dir3[0] + dir3[1] * dir3[1]);
        if (dirLen > 0.001)
        {
            const double arrowHeadLength = 8.;
            const double halfAngle = 30. * M_PI / 180.;

            double perpX = -dir3[1] / dirLen;
            double perpY = dir3[0] / dirLen;

            pos3[0] = pos2[0] - dir3[0] / dirLen * arrowHeadLength + perpX * arrowHeadLength * std::tan(halfAngle);
            pos3[1] = pos2[1] - dir3[1] / dirLen * arrowHeadLength + perpY * arrowHeadLength * std::tan(halfAngle);

            pos4[0] = pos2[0] - dir3[0] / dirLen * arrowHeadLength - perpX * arrowHeadLength * std::tan(halfAngle);
            pos4[1] = pos2[1] - dir3[1] / dirLen * arrowHeadLength - perpY * arrowHeadLength * std::tan(halfAngle);
        }

        m_pointsArrow->SetPoint(2, pos3[0], pos3[1], 0.);
        m_pointsArrow->SetPoint(3, pos4[0], pos4[1], 0.);
        m_pointsArrow->Modified();

        // Update the actors.
        m_fActorArrow->Modified();
        m_fActorText->Modified();
    }

    void FITKGraphObjectAnnotation::update(bool forceUpdate)
    {
        Q_UNUSED(forceUpdate);

        if (!m_initialized)
        {
            return;
        }

        // Update the properties of the actors by the annotation data.
        updateProperties();

        // Update the visibility.
        updateVisibility();

        // Update the actors.
        if (m_fActorArrow)
        {
            m_fActorArrow->Modified();
        }

        if (m_fActorText)
        {
            m_fActorText->Modified();
        }
    }

    void FITKGraphObjectAnnotation::updateVisibility()
    {
        FITKGraphAnnotation* annotation = dynamic_cast<FITKGraphAnnotation*>(_dataObj);
        if (!annotation)
        {
            return;
        }

        if (!m_fActorArrow || !m_fActorText)
        {
            return;
        }

        bool dataVisible = annotation->isEnable() && !annotation->getIsEditing();

        // Update the arrow.
        m_fActorArrow->SetVisibility(annotation->getWithArrow() && dataVisible);

        // Update the text.
        m_fActorText->SetVisibility(annotation->getWithText() && dataVisible);

        // Update the actors position.
        updatePositions();
    }

    void FITKGraphObjectAnnotation::modifyStartAnchor()
    {
        if (m_movingEnd)
        {
            m_movingEnd = false;
        }

        m_movingStart = true;
    }

    void FITKGraphObjectAnnotation::modifyEndAnchor()
    {
        if (m_movingStart)
        {
            m_movingStart = false;
        }

        m_movingEnd = true;
    }

    void FITKGraphObjectAnnotation::endModifyingAnchor()
    {
        m_movingStart = false;
        m_movingEnd = false;
    }

    bool FITKGraphObjectAnnotation::isModifying()
    {
        return m_movingStart || m_movingEnd;
    }

    void FITKGraphObjectAnnotation::generateGraph()
    {
        FITKGraphAnnotation* annotation = dynamic_cast<FITKGraphAnnotation*>(_dataObj);
        if (!annotation)
        {
            return;
        }

        FITKGraph3DWindowVTK* graphWindow = annotation->getGraph3DWindow();
        if (!graphWindow)
        {
            return;
        }

        // Create the arrow actor.
        m_fActorArrow = vtkActor2D::New();
        m_fActorArrow->SetPickable(false);
        m_fActorArrow->SetDragable(false);
        m_fActorArrow->SetVisibility(false);

        vtkSmartPointer<vtkPolyDataMapper2D> mapper = vtkSmartPointer<vtkPolyDataMapper2D>::New();
        mapper->SetInputData(m_polyDataArrow);
        m_fActorArrow->SetMapper(mapper);
        m_fActorArrow->GetProperty()->SetColor(1., 1., 1.);
        m_fActorArrow->GetProperty()->SetLineWidth(1.);
        addActor(m_fActorArrow);

        // Create the text actor.
        m_fActorText = vtkTextActor::New();
        m_fActorText->SetPickable(false);
        m_fActorText->SetDragable(false);
        m_fActorText->SetVisibility(false);
        m_fActorText->GetTextProperty()->SetColor(1., 1., 1.);
        // m_fActorText->GetTextProperty()->SetUseTightBoundingBox(false);
        m_fActorText->SetTextScaleModeToNone();
        m_fActorText->GetPositionCoordinate()->SetCoordinateSystemToNormalizedDisplay();
        addActor(m_fActorText);
    }

    //--------------------------------------------------------------------------------------------!
    FITKGraphObjectAnnotationManager::~FITKGraphObjectAnnotationManager()
    {
        // Remove all annotation objects.
        for (FITKGraphObjectAnnotation* gObj : m_annoList)
        {
            if (gObj)
            {
                gObj->removeFromGraphWidget();
                delete gObj;
            }
        }

        m_annoList.clear();
    }

    void FITKGraphObjectAnnotationManager::appendObject(FITKGraphObjectAnnotation* obj)
    {
        if (!obj)
        {
            return;
        }

        if (m_annoList.contains(obj))
        {
            return;
        }

        connect(obj, &FITKGraphObjectAnnotation::graphObjDestroyedSig, this,
            &FITKGraphObjectAnnotationManager::slot_dataDeleted, Qt::UniqueConnection);
        m_annoList.push_back(obj);
    }

    void FITKGraphObjectAnnotationManager::removeObject(FITKGraphObjectAnnotation* obj, bool delData)
    {
        if (!obj)
        {
            return;
        }

        if (m_annoList.removeOne(obj))
        {
            if (delData)
            {
                delete obj;
            }
            else
            {
                disconnect(obj, &FITKGraphObjectAnnotation::graphObjDestroyedSig, this,
                    &FITKGraphObjectAnnotationManager::slot_dataDeleted);
            }
        }
    }

    int FITKGraphObjectAnnotationManager::getNumberOfObjects()
    {
        return m_annoList.count();
    }

    FITKGraphObjectAnnotation* FITKGraphObjectAnnotationManager::getObjectAt(int index)
    {
        if (index < 0 || index >= m_annoList.count())
        {
            return nullptr;
        }

        return m_annoList[index];
    }

    FITKGraphObjectAnnotation* FITKGraphObjectAnnotationManager::getObjectByAnnoID(int id)
    {
        // Find the annotation object.
        for (FITKGraphObjectAnnotation* gObj : m_annoList)
        {
            if (!gObj)
            {
                continue;
            }

            Core::FITKAbstractDataObject* dObj = gObj->getDataOject();
            if (!dObj)
            {
                continue;
            }

            if (dObj->getDataObjectID() == id)
            {
                return gObj;
            }
        }

        return nullptr;
    }

    void FITKGraphObjectAnnotationManager::slot_dataDeleted(Core::FITKAbstractGraphObject* gobj)
    {
        m_annoList.removeOne(FITKGraphObjectAnnotation::SafeDownCast(gobj));
    }
}   // namespace Comp