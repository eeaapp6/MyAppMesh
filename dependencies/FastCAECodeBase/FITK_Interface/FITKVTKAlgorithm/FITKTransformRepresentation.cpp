/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKTransformRepresentation.h"

#include "vtkActor.h"
#include "vtkAssemblyPath.h"
#include "vtkBox.h"
#include "vtkCallbackCommand.h"
#include "vtkCamera.h"
#include "vtkCellArray.h"
#include "vtkCellPicker.h"
#include "vtkDoubleArray.h"
#include "vtkEventData.h"
#include "vtkInteractorObserver.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkPickingManager.h"
#include "vtkPlane.h"
#include "vtkPlanes.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkQuaternion.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkSphereSource.h"
#include "vtkTransform.h"
#include "vtkVectorOperators.h"
#include "vtkWindow.h"
#include "vtkGlyph3D.h"
#include "vtkArrowSource.h"
#include "vtkTransformFilter.h"
#include "vtkPointData.h"
#include "vtkCubeSource.h"

#include <assert.h>

vtkStandardNewMacro(FITKTransformRepresentation);

//----------------------------------------------------------------------------
FITKTransformRepresentation::FITKTransformRepresentation()
{
    // The initial state
    this->InteractionState = FITKTransformRepresentation::Outside;
    this->EnableReverseTranslate = true;
    this->HandleAtCenter = false;
    this->HandleFactor = 1.;

    // Handle size is in pixels for this widget
    this->HandleSize = 80.;

    // Set up the initial properties
    this->CreateDefaultProperties();

    // Create the bounding box actor.
    this->BdBox = vtkCubeSource::New();
    this->BdBoxMapper = vtkPolyDataMapper::New();
    this->BdBoxMapper->SetInputConnection(this->BdBox->GetOutputPort());
    this->BdBoxMapper->SetScalarVisibility(false);
    this->BdBoxActor = vtkActor::New();
    this->BdBoxActor->SetMapper(this->BdBoxMapper);
    this->BdBoxActor->GetProperty()->SetRepresentation(1);
    this->BdBoxActor->GetProperty()->SetColor(1., 1., 1.);
    this->BdBoxActor->GetProperty()->SetLineWidth(2.);
    this->BdBoxActor->GetProperty()->SetLighting(false);
    this->BdBoxActor->SetPickable(false);

    // Create handle position points.
    this->Points = new vtkPoints*[7];
    this->PolyData = new vtkPolyData*[6];
    this->Normals = new vtkDoubleArray*[6];
    this->HandleProperty = new vtkProperty*[7];

    // Create handle and center source
    this->HandleSource = vtkArrowSource::New();
    this->HandleSource->SetTipResolution(15);
    this->HandleSource->SetShaftResolution(15);
    this->HandleSource->SetTipLength(0.35);
    this->HandleSource->SetTipRadius(0.1);
    this->CenterSource = vtkSphereSource::New();
    this->CenterSource->SetThetaResolution(30);
    this->CenterSource->SetPhiResolution(30);

    // Create glyph and poly data.
    this->HandleGlyph3D = new vtkGlyph3D*[6];

    for (int i = 0; i < 6; i++)
    {
        int index = i % 3;

        double nor[3]{ 0., 0., 0. };
        nor[index] = (i >= 3 ? -1. : 1.);

        this->Points[i] = vtkPoints::New();
        this->Points[i]->SetNumberOfPoints(1);
        this->PolyData[i] = vtkPolyData::New();
        this->PolyData[i]->SetPoints(this->Points[i]);
        this->Normals[i] = vtkDoubleArray::New();
        this->Normals[i]->SetNumberOfComponents(3);
        this->Normals[i]->InsertNextTuple3(nor[0], nor[1], nor[2]);
        this->PolyData[i]->GetPointData()->SetNormals(this->Normals[i]);

        this->HandleGlyph3D[i] = vtkGlyph3D::New();
        this->HandleGlyph3D[i]->SetInputData(this->PolyData[i]);
        this->HandleGlyph3D[i]->SetScaling(true);
        this->HandleGlyph3D[i]->SetScaleModeToScaleByScalar();
        this->HandleGlyph3D[i]->SetVectorModeToUseNormal();

        // Set the soure data and input data.
        this->HandleGlyph3D[i]->SetSourceConnection(this->HandleSource->GetOutputPort());
    }

    // Create the handles
    this->Handle = new vtkActor*[7];
    this->HandleMapper = new vtkPolyDataMapper*[7];

    // Create arrow handle actors
    for (int i = 0; i < 6; i++)
    {
        int index = i % 3;

        double color[3]{ 0., 0., 0. };
        color[index] = 1.;

        this->HandleMapper[i] = vtkPolyDataMapper::New();
        this->HandleMapper[i]->SetScalarVisibility(false);
        this->HandleMapper[i]->SetInputConnection(this->HandleGlyph3D[i]->GetOutputPort());
        this->Handle[i] = vtkActor::New();
        this->Handle[i]->SetMapper(this->HandleMapper[i]);
        this->HandleProperty[i] = vtkProperty::New();
        this->HandleProperty[i]->SetColor(color);
        this->Handle[i]->SetProperty(this->HandleProperty[i]);
    }

    // Create center sphere actors
    this->Points[6] = vtkPoints::New();
    this->Points[6]->SetNumberOfPoints(1); // o
    this->HandleMapper[6] = vtkPolyDataMapper::New();
    this->HandleMapper[6]->SetScalarVisibility(false);
    this->HandleMapper[6]->SetInputConnection(this->CenterSource->GetOutputPort());
    this->Handle[6] = vtkActor::New();
    this->Handle[6]->SetMapper(this->HandleMapper[6]);
    this->HandleProperty[6] = vtkProperty::New();
    this->HandleProperty[6]->SetColor(1., 1., 1.);
    this->Handle[6]->SetProperty(this->HandleProperty[6]);

    // Define the point coordinates
    double bounds[6];
    bounds[0] = -0.5;
    bounds[1] = 0.5;
    bounds[2] = -0.5;
    bounds[3] = 0.5;
    bounds[4] = -0.5;
    bounds[5] = 0.5;

    this->BoundingBox = vtkBox::New();
    this->PlaceWidget(bounds);

    // Manage the picking stuff
    this->HandlePicker = vtkCellPicker::New();
    this->HandlePicker->SetTolerance(0.001);
    for (int i = 0; i < 7; i++)
    {
        this->HandlePicker->AddPickList(this->Handle[i]);
    }

    this->HandlePicker->PickFromListOn();

    this->CurrentHandle = nullptr;

    // Internal data members for performance
    this->Transform = vtkTransform::New();

    this->SetPosition(this->Position);
}

//----------------------------------------------------------------------------
FITKTransformRepresentation::~FITKTransformRepresentation()
{
    this->BdBox->Delete();
    this->BdBoxMapper->Delete();
    this->BdBoxActor->Delete();

    for (int i = 0; i < 6; i++)
    {
        this->Normals[i]->Delete();
        this->Points[i]->Delete();
        this->PolyData[i]->Delete();
        this->HandleMapper[i]->Delete();
        this->HandleProperty[i]->Delete();
        this->HandleGlyph3D[i]->Delete();
        this->Handle[i]->Delete();
    }

    this->Points[6]->Delete();
    this->HandleMapper[6]->Delete();
    this->HandleProperty[6]->Delete();
    this->Handle[6]->Delete();

    delete[] this->Normals;
    delete[] this->Points;
    delete[] this->PolyData;
    delete[] this->HandleMapper;
    delete[] this->HandleProperty;
    delete[] this->HandleGlyph3D;
    delete[] this->Handle;

    this->HandlePicker->Delete();

    this->Transform->Delete();
    this->BoundingBox->Delete();

    this->SelectedHandleProperty->Delete();

    this->HandleSource->Delete();
    this->CenterSource->Delete();
}

double* FITKTransformRepresentation::GetPosition()
{
    // The sphere points.
    return this->Points[6]->GetPoint(0);
}

void FITKTransformRepresentation::SetTranslateTo(double* pos)
{
    double* prevPickPoint = this->LastEventPosition;
    double* pickPoint = pos;

    if (this->InteractionState == FITKTransformRepresentation::TranslatingXPos ||
        this->InteractionState == FITKTransformRepresentation::TranslatingXNeg)
    {
        pickPoint[1] = prevPickPoint[1];
        pickPoint[2] = prevPickPoint[2];
        this->Translate(prevPickPoint, pickPoint);
    }
    else if (this->InteractionState == FITKTransformRepresentation::TranslatingYPos ||
        this->InteractionState == FITKTransformRepresentation::TranslatingYNeg)
    {
        pickPoint[0] = prevPickPoint[0];
        pickPoint[2] = prevPickPoint[2];
        this->Translate(prevPickPoint, pickPoint);
    }
    else if (this->InteractionState == FITKTransformRepresentation::TranslatingZPos ||
        this->InteractionState == FITKTransformRepresentation::TranslatingZNeg)
    {
        pickPoint[0] = prevPickPoint[0];
        pickPoint[1] = prevPickPoint[1];
        this->Translate(prevPickPoint, pickPoint);
    }
    else if (this->InteractionState == FITKTransformRepresentation::TranslatingO)
    {
        this->Translate(prevPickPoint, pickPoint);
    }
}

void FITKTransformRepresentation::GetTransform(vtkTransform* trans)
{
    if (trans)
    {
        trans->DeepCopy(this->Transform);
    }
}

void FITKTransformRepresentation::SetPosition(double* pos3)
{
    this->Position[0] = pos3[0];
    this->Position[1] = pos3[1];
    this->Position[2] = pos3[2];

    double size = this->vtkWidgetRepresentation::SizeHandlesInPixels(1., pos3);
    this->SetPosition(pos3, size);
}

void FITKTransformRepresentation::SetPosition(double* pos3, double size)
{
    this->BdBox->SetCenter(pos3);

    this->CenterSource->SetCenter(pos3);
    this->CenterSource->SetRadius(size * 0.1);

    this->Points[6]->SetPoint(0, pos3);
    this->Points[6]->Modified();

    // Handle size.
    double fac = size;
    double* ct = this->Points[6]->GetPoint(0);
    if (this->HandleAtCenter)
    {
        for (int i = 0; i < 3; i++)
        {
            double pos[3]{ 0., 0., 0. };

            pos[i] = 0.25;
            this->Points[i]->SetPoint(0, ct[0] + pos[0], ct[1] + pos[1], ct[2] + pos[2]);
            this->Points[i]->Modified();

            pos[i] = -0.25;
            this->Points[i + 3]->SetPoint(0, ct[0] + pos[0], ct[1] + pos[1], ct[2] + pos[2]);
            this->Points[i + 3]->Modified();

            this->HandleGlyph3D[i]->SetScaleFactor(fac * this->HandleFactor);
            this->HandleGlyph3D[i + 3]->SetScaleFactor(fac * this->HandleFactor);
        }
    }
    else
    {
        for (int i = 0; i < 3; i++)
        {
            double len = (this->InitialBounds[i * 2 + 1] - this->InitialBounds[i * 2]) * 0.5;
            double pos[3]{ 0., 0., 0. };

            pos[i] = len;
            this->Points[i]->SetPoint(0, ct[0] + pos[0], ct[1] + pos[1], ct[2] + pos[2]);
            this->Points[i]->Modified();

            pos[i] = -len;
            this->Points[i + 3]->SetPoint(0, ct[0] + pos[0], ct[1] + pos[1], ct[2] + pos[2]);
            this->Points[i + 3]->Modified();

            this->HandleGlyph3D[i]->SetScaleFactor(fac * this->HandleFactor);
            this->HandleGlyph3D[i + 3]->SetScaleFactor(fac * this->HandleFactor);
        }
    }
}

//----------------------------------------------------------------------
void FITKTransformRepresentation::StartWidgetInteraction(double e[2])
{
    // Store the start position
    this->StartEventPosition[0] = e[0];
    this->StartEventPosition[1] = e[1];
    this->StartEventPosition[2] = 0.0;

    // Store the start position
    this->LastEventPosition[0] = e[0];
    this->LastEventPosition[1] = e[1];
    this->LastEventPosition[2] = 0.0;

    this->ComputeInteractionState(static_cast<int>(e[0]), static_cast<int>(e[1]), 0);
}

//----------------------------------------------------------------------
void FITKTransformRepresentation::WidgetInteraction(double e[2])
{
    // Convert events to appropriate coordinate systems
    vtkCamera* camera = this->Renderer->GetActiveCamera();
    if (!camera)
    {
        return;
    }
    double focalPoint[4], pickPoint[4], prevPickPoint[4];
    double z, vpn[3];
    camera->GetViewPlaneNormal(vpn);

    // Compute the two points defining the motion vector
    double pos[3];
    this->HandlePicker->GetPickPosition(pos);

    vtkInteractorObserver::ComputeWorldToDisplay(this->Renderer, pos[0], pos[1], pos[2], focalPoint);
    z = focalPoint[2];
    vtkInteractorObserver::ComputeDisplayToWorld(this->Renderer, this->LastEventPosition[0], this->LastEventPosition[1], z, prevPickPoint);
    vtkInteractorObserver::ComputeDisplayToWorld(this->Renderer, e[0], e[1], z, pickPoint);

    // Process the motion
    if (this->EnableReverseTranslate)
    {
        if (this->InteractionState == FITKTransformRepresentation::TranslatingXPos ||
            this->InteractionState == FITKTransformRepresentation::TranslatingXNeg)
        {
            pickPoint[1] = prevPickPoint[1];
            pickPoint[2] = prevPickPoint[2];
            this->Translate(prevPickPoint, pickPoint);
        }
        else if (this->InteractionState == FITKTransformRepresentation::TranslatingYPos ||
            this->InteractionState == FITKTransformRepresentation::TranslatingYNeg)
        {
            pickPoint[0] = prevPickPoint[0];
            pickPoint[2] = prevPickPoint[2];
            this->Translate(prevPickPoint, pickPoint);
        }
        else if (this->InteractionState == FITKTransformRepresentation::TranslatingZPos ||
            this->InteractionState == FITKTransformRepresentation::TranslatingZNeg)
        {
            pickPoint[0] = prevPickPoint[0];
            pickPoint[1] = prevPickPoint[1];
            this->Translate(prevPickPoint, pickPoint);
        }
        else if (this->InteractionState == FITKTransformRepresentation::TranslatingO)
        {
            this->Translate(prevPickPoint, pickPoint);
        }
    }
    else
    {
        if (this->InteractionState == FITKTransformRepresentation::TranslatingXPos)
        {
            if (prevPickPoint[0] < pickPoint[0])
            {
                pickPoint[1] = prevPickPoint[1];
                pickPoint[2] = prevPickPoint[2];
                this->Translate(prevPickPoint, pickPoint);
            }
        }
        else if (this->InteractionState == FITKTransformRepresentation::TranslatingXNeg)
        {
            if (prevPickPoint[0] > pickPoint[0])
            {
                pickPoint[1] = prevPickPoint[1];
                pickPoint[2] = prevPickPoint[2];
                this->Translate(prevPickPoint, pickPoint);
            }
        }
        else if (this->InteractionState == FITKTransformRepresentation::TranslatingYPos)
        {
            if (prevPickPoint[1] < pickPoint[1])
            {
                pickPoint[0] = prevPickPoint[0];
                pickPoint[2] = prevPickPoint[2];
                this->Translate(prevPickPoint, pickPoint);
            }
        }
        else if (this->InteractionState == FITKTransformRepresentation::TranslatingYNeg)
        {
            if (prevPickPoint[1] > pickPoint[1])
            {
                pickPoint[0] = prevPickPoint[0];
                pickPoint[2] = prevPickPoint[2];
                this->Translate(prevPickPoint, pickPoint);
            }
        }
        else if (this->InteractionState == FITKTransformRepresentation::TranslatingZPos)
        {
            if (prevPickPoint[2] < pickPoint[2])
            {
                pickPoint[0] = prevPickPoint[0];
                pickPoint[1] = prevPickPoint[1];
                this->Translate(prevPickPoint, pickPoint);
            }
        }
        else if (this->InteractionState == FITKTransformRepresentation::TranslatingZNeg)
        {
            if (prevPickPoint[2] > pickPoint[2])
            {
                pickPoint[0] = prevPickPoint[0];
                pickPoint[1] = prevPickPoint[1];
                this->Translate(prevPickPoint, pickPoint);
            }
        }
        else if (this->InteractionState == FITKTransformRepresentation::TranslatingO)
        {
            this->Translate(prevPickPoint, pickPoint);
        }
    }

    // Store the start position
    this->LastEventPosition[0] = e[0];
    this->LastEventPosition[1] = e[1];
    this->LastEventPosition[2] = 0.0;
}

void FITKTransformRepresentation::ComplexInteraction(
    vtkRenderWindowInteractor*, vtkAbstractWidget*, unsigned long, void* calldata)
{
    vtkEventData* edata = static_cast<vtkEventData*>(calldata);
    vtkEventDataDevice3D* edd = edata->GetAsEventDataDevice3D();
    if (edd)
    {
        // all others
        double eventPos[3];
        edd->GetWorldPosition(eventPos);
        double eventDir[4];
        edd->GetWorldOrientation(eventDir);

        double* prevPickPoint = this->LastEventPosition;
        double* pickPoint = eventPos;

        if (this->EnableReverseTranslate)
        {
            if (this->InteractionState == FITKTransformRepresentation::TranslatingXPos ||
                this->InteractionState == FITKTransformRepresentation::TranslatingXNeg)
            {
                pickPoint[1] = prevPickPoint[1];
                pickPoint[2] = prevPickPoint[2];
                this->Translate(prevPickPoint, pickPoint);
            }
            else if (this->InteractionState == FITKTransformRepresentation::TranslatingYPos ||
                this->InteractionState == FITKTransformRepresentation::TranslatingYNeg)
            {
                pickPoint[0] = prevPickPoint[0];
                pickPoint[2] = prevPickPoint[2];
                this->Translate(prevPickPoint, pickPoint);
            }
            else if (this->InteractionState == FITKTransformRepresentation::TranslatingZPos ||
                this->InteractionState == FITKTransformRepresentation::TranslatingZNeg)
            {
                pickPoint[0] = prevPickPoint[0];
                pickPoint[1] = prevPickPoint[1];
                this->Translate(prevPickPoint, pickPoint);
            }
            else if (this->InteractionState == FITKTransformRepresentation::TranslatingO)
            {
                this->Translate(prevPickPoint, pickPoint);
            }
        }
        else
        {
            if (this->InteractionState == FITKTransformRepresentation::TranslatingXPos)
            {
                if (prevPickPoint[0] < pickPoint[0])
                {
                    pickPoint[1] = prevPickPoint[1];
                    pickPoint[2] = prevPickPoint[2];
                    this->Translate(prevPickPoint, pickPoint);
                }
            }
            else if (this->InteractionState == FITKTransformRepresentation::TranslatingXNeg)
            {
                if (prevPickPoint[0] > pickPoint[0])
                {
                    pickPoint[1] = prevPickPoint[1];
                    pickPoint[2] = prevPickPoint[2];
                    this->Translate(prevPickPoint, pickPoint);
                }
            }
            else if (this->InteractionState == FITKTransformRepresentation::TranslatingYPos)
            {
                if (prevPickPoint[1] < pickPoint[1])
                {
                    pickPoint[0] = prevPickPoint[0];
                    pickPoint[2] = prevPickPoint[2];
                    this->Translate(prevPickPoint, pickPoint);
                }
            }
            else if (this->InteractionState == FITKTransformRepresentation::TranslatingYNeg)
            {
                if (prevPickPoint[1] > pickPoint[1])
                {
                    pickPoint[0] = prevPickPoint[0];
                    pickPoint[2] = prevPickPoint[2];
                    this->Translate(prevPickPoint, pickPoint);
                }
    }
            else if (this->InteractionState == FITKTransformRepresentation::TranslatingZPos)
            {
                if (prevPickPoint[2] < pickPoint[2])
                {
                    pickPoint[0] = prevPickPoint[0];
                    pickPoint[1] = prevPickPoint[1];
                    this->Translate(prevPickPoint, pickPoint);
                }
            }
            else if (this->InteractionState == FITKTransformRepresentation::TranslatingZNeg)
            {
                if (prevPickPoint[2] > pickPoint[2])
                {
                    pickPoint[0] = prevPickPoint[0];
                    pickPoint[1] = prevPickPoint[1];
                    this->Translate(prevPickPoint, pickPoint);
                }
            }
            else if (this->InteractionState == FITKTransformRepresentation::TranslatingO)
            {
                this->Translate(prevPickPoint, pickPoint);
            }
        }

        // Book keeping
        std::copy(eventPos, eventPos + 3, this->LastEventPosition);
        std::copy(eventDir, eventDir + 4, this->LastEventOrientation);
        this->Modified();
    }
}

void FITKTransformRepresentation::EndComplexInteraction(
    vtkRenderWindowInteractor*, vtkAbstractWidget*, unsigned long, void*)
{
}

//----------------------------------------------------------------------------
// Loop through all points and translate them
void FITKTransformRepresentation::Translate(const double* p1, const double* p2)
{
    double v[3] = { p2[0] - p1[0], p2[1] - p1[1], p2[2] - p1[2] };
    double* ct = this->Points[6]->GetPoint(0);
    // Get the new center.
    for (int i = 0; i < 3; i++)
    {
        ct[i] += v[i];
    }

    //this->Transform->Identity();
    this->Transform->Translate(v);

    double size = this->vtkWidgetRepresentation::SizeHandlesInPixels(1, ct);
    this->SetPosition(ct, size);
}

namespace
{
    bool snapToAxis(vtkVector3d& in, vtkVector3d& out, double snapAngle)
    {
        int largest = 0;
        if (fabs(in[1]) > fabs(in[0]))
        {
            largest = 1;
        }
        if (fabs(in[2]) > fabs(in[largest]))
        {
            largest = 2;
        }
        vtkVector3d axis(0, 0, 0);
        axis[largest] = 1.0;
        // 3 degrees of sticky
        if (fabs(in.Dot(axis)) > cos(vtkMath::Pi() * snapAngle / 180.0))
        {
            if (in.Dot(axis) < 0)
            {
                axis[largest] = -1;
            }
            out = axis;
            return true;
        }
        return false;
    }
}

//----------------------------------------------------------------------------
void FITKTransformRepresentation::CreateDefaultProperties()
{
    // Handle properties
    this->SelectedHandleProperty = vtkProperty::New();
    this->SelectedHandleProperty->SetColor(1., 1., 0.);
}

//----------------------------------------------------------------------------
void FITKTransformRepresentation::PlaceWidget(double bds[6])
{
    int i;
    double bounds[6], center[3];

    this->AdjustBounds(bds, bounds, center);

    this->Points[6]->SetPoint(0, center);

    // Update the box bounds.
    this->BdBox->SetXLength(bounds[1] - bounds[0]);
    this->BdBox->SetYLength(bounds[3] - bounds[2]);
    this->BdBox->SetZLength(bounds[5] - bounds[4]);

    for (i = 0; i < 6; i++)
    {
        this->InitialBounds[i] = bounds[i];
    }

    this->InitialLength = sqrt((bounds[1] - bounds[0]) * (bounds[1] - bounds[0]) +
        (bounds[3] - bounds[2]) * (bounds[3] - bounds[2]) +
        (bounds[5] - bounds[4]) * (bounds[5] - bounds[4]));

    this->ValidPick = 1; // since we have set up widget
    this->SizeHandles();
}

//----------------------------------------------------------------------------
int FITKTransformRepresentation::ComputeInteractionState(int X, int Y, int vtkNotUsed(modify))
{
    // Okay, we can process this. Try to pick handles first;
    // if no handles picked, then pick the bounding box.
    if (!this->Renderer || !this->Renderer->IsInViewport(X, Y))
    {
        this->InteractionState = FITKTransformRepresentation::Outside;
        return this->InteractionState;
    }

    // Try and pick a handle first
    this->CurrentHandle = nullptr;

    vtkAssemblyPath* path = this->GetAssemblyPath(X, Y, 0., this->HandlePicker);

    if (path != nullptr)
    {
        this->ValidPick = 1;
        this->CurrentHandle = reinterpret_cast<vtkActor*>(path->GetFirstNode()->GetViewProp());
        if (this->CurrentHandle == this->Handle[0])
        {
            this->InteractionState = FITKTransformRepresentation::TranslatingXPos;
        }
        else if (this->CurrentHandle == this->Handle[1])
        {
            this->InteractionState = FITKTransformRepresentation::TranslatingYPos;
        }
        else if (this->CurrentHandle == this->Handle[2])
        {
            this->InteractionState = FITKTransformRepresentation::TranslatingZPos;
        }
        else if (this->CurrentHandle == this->Handle[3])
        {
            this->InteractionState = FITKTransformRepresentation::TranslatingXNeg;
        }
        else if (this->CurrentHandle == this->Handle[4])
        {
            this->InteractionState = FITKTransformRepresentation::TranslatingYNeg;
        }
        else if (this->CurrentHandle == this->Handle[5])
        {
            this->InteractionState = FITKTransformRepresentation::TranslatingZNeg;
        }
        else if (this->CurrentHandle == this->Handle[6])
        {
            this->InteractionState = FITKTransformRepresentation::TranslatingO;
        }
    }
    else
    {
        this->InteractionState = FITKTransformRepresentation::Outside;
    }

    return this->InteractionState;
}

int FITKTransformRepresentation::ComputeComplexInteractionState(
    vtkRenderWindowInteractor*, vtkAbstractWidget*, unsigned long, void* calldata, int)
{
    this->InteractionState = FITKTransformRepresentation::Outside;

    vtkEventData* edata = static_cast<vtkEventData*>(calldata);
    vtkEventDataDevice3D* edd = edata->GetAsEventDataDevice3D();
    if (edd)
    {
        double pos[3];
        edd->GetWorldPosition(pos);

        // Try and pick a handle first
        this->CurrentHandle = nullptr;

        vtkAssemblyPath* path = this->GetAssemblyPath3DPoint(pos, this->HandlePicker);

        if (path != nullptr)
        {
            this->ValidPick = 1;
            this->CurrentHandle = reinterpret_cast<vtkActor*>(path->GetFirstNode()->GetViewProp());
            if (this->CurrentHandle == this->Handle[0])
            {
                this->InteractionState = FITKTransformRepresentation::TranslatingXPos;
            }
            else if (this->CurrentHandle == this->Handle[1])
            {
                this->InteractionState = FITKTransformRepresentation::TranslatingYPos;
            }
            else if (this->CurrentHandle == this->Handle[2])
            {
                this->InteractionState = FITKTransformRepresentation::TranslatingZPos;
            }
            else if (this->CurrentHandle == this->Handle[3])
            {
                this->InteractionState = FITKTransformRepresentation::TranslatingXNeg;
            }
            else if (this->CurrentHandle == this->Handle[4])
            {
                this->InteractionState = FITKTransformRepresentation::TranslatingYNeg;
            }
            else if (this->CurrentHandle == this->Handle[5])
            {
                this->InteractionState = FITKTransformRepresentation::TranslatingZNeg;
            }
            else if (this->CurrentHandle == this->Handle[6])
            {
                this->InteractionState = FITKTransformRepresentation::TranslatingO;
            }
        }
    }

    return this->InteractionState;
}

//----------------------------------------------------------------------
void FITKTransformRepresentation::SetInteractionState(int state)
{
    // Clamp to allowable values
    state = (state < FITKTransformRepresentation::Outside
        ? FITKTransformRepresentation::Outside
        : (state > FITKTransformRepresentation::TranslatingO ? FITKTransformRepresentation::TranslatingO : state));

    // Depending on state, highlight appropriate parts of representation
    int handle;
    this->InteractionState = state;
    switch (state)
    {
    case FITKTransformRepresentation::TranslatingXPos:
    case FITKTransformRepresentation::TranslatingXNeg:
    case FITKTransformRepresentation::TranslatingYPos:
    case FITKTransformRepresentation::TranslatingYNeg:
    case FITKTransformRepresentation::TranslatingZPos:
    case FITKTransformRepresentation::TranslatingZNeg:
    case FITKTransformRepresentation::TranslatingO:
        handle = this->HighlightHandle(this->CurrentHandle);
        break;
    default:
        this->HighlightHandle(nullptr);
    }
}

//----------------------------------------------------------------------
double* FITKTransformRepresentation::GetBounds()
{
    this->BuildRepresentation();

    this->BoundingBox->SetBounds(this->Handle[0]->GetBounds());

    for (int j = 0; j < 7; j++)
    {
        this->BoundingBox->AddBounds(this->Handle[j]->GetBounds());
    }

    this->BoundingBox->AddBounds(this->BdBoxActor->GetBounds());

    return this->BoundingBox->GetBounds();
}

//----------------------------------------------------------------------------
void FITKTransformRepresentation::BuildRepresentation()
{
    // Rebuild only if necessary
    if (this->GetMTime() > this->BuildTime ||
        (this->Renderer && this->Renderer->GetVTKWindow() &&
        (this->Renderer->GetVTKWindow()->GetMTime() > this->BuildTime ||
            this->Renderer->GetActiveCamera()->GetMTime() > this->BuildTime)))
    {
        this->SizeHandles();
        this->BuildTime.Modified();
    }
}

//----------------------------------------------------------------------------
void FITKTransformRepresentation::ReleaseGraphicsResources(vtkWindow* w)
{
    // render the handles
    for (int j = 0; j < 7; j++)
    {
        this->Handle[j]->ReleaseGraphicsResources(w);
    }

    this->BdBoxActor->ReleaseGraphicsResources(w);
}

//----------------------------------------------------------------------------
int FITKTransformRepresentation::RenderOpaqueGeometry(vtkViewport* v)
{
    int count = 0;
    this->BuildRepresentation();

    for (int j = 0; j < 7; j++)
    {
        if (this->Handle[j]->GetVisibility())
        {
            this->Handle[j]->SetPropertyKeys(this->GetPropertyKeys());
            count += this->Handle[j]->RenderOpaqueGeometry(v);
        }
    }

    if (this->BdBoxActor->GetVisibility())
    {
        this->BdBoxActor->SetPropertyKeys(this->GetPropertyKeys());
        count += this->BdBoxActor->RenderOpaqueGeometry(v);
    }

    return count;
}

//----------------------------------------------------------------------------
int FITKTransformRepresentation::RenderTranslucentPolygonalGeometry(vtkViewport* v)
{
    int count = 0;
    this->BuildRepresentation();

    // render the handles
    for (int j = 0; j < 7; j++)
    {
        if (this->Handle[j]->GetVisibility())
        {
            this->Handle[j]->SetPropertyKeys(this->GetPropertyKeys());
            count += this->Handle[j]->RenderTranslucentPolygonalGeometry(v);
        }
    }

    if (this->BdBoxActor->GetVisibility())
    {
        this->BdBoxActor->SetPropertyKeys(this->GetPropertyKeys());
        count += this->BdBoxActor->RenderTranslucentPolygonalGeometry(v);
    }

    return count;
}

//----------------------------------------------------------------------------
vtkTypeBool FITKTransformRepresentation::HasTranslucentPolygonalGeometry()
{
    int result = 0;
    this->BuildRepresentation();

    // render the handles
    for (int j = 0; j < 7; j++)
    {
        result |= this->Handle[j]->HasTranslucentPolygonalGeometry();
    }
    
    result |= this->BdBoxActor->HasTranslucentPolygonalGeometry();

    return result;
}

//----------------------------------------------------------------------------
vtkProperty* FITKTransformRepresentation::GetActorProperty(HandleActorType type)
{
    switch (type)
    {
    case FITKTransformRepresentation::HandleXPos:
    case FITKTransformRepresentation::HandleXNeg:
    case FITKTransformRepresentation::HandleYPos:
    case FITKTransformRepresentation::HandleYNeg:
    case FITKTransformRepresentation::HandleZPos:
    case FITKTransformRepresentation::HandleZNeg:
    case FITKTransformRepresentation::HandleO:
        return this->HandleProperty[type];
        break;
    case FITKTransformRepresentation::BoundBox:
        return this->BdBoxActor->GetProperty();
        break;
    default:
        break;
    }

    return nullptr;
}

//----------------------------------------------------------------------------
void FITKTransformRepresentation::HandlesOn()
{
    for (int i = 0; i < 7; i++)
    {
        this->Handle[i]->VisibilityOn();
    }
}

//----------------------------------------------------------------------------
void FITKTransformRepresentation::HandlesOff()
{
    for (int i = 0; i < 7; i++)
    {
        this->Handle[i]->VisibilityOff();
    }
}

//----------------------------------------------------------------------------
void FITKTransformRepresentation::BoundBoxOn()
{
    this->BdBoxActor->VisibilityOn();
}

//----------------------------------------------------------------------------
void FITKTransformRepresentation::BoundBoxOff()
{
    this->BdBoxActor->VisibilityOff();
}

//----------------------------------------------------------------------------
void FITKTransformRepresentation::SizeHandles()
{
    double* ct = this->Points[6]->GetPoint(0);
    double radius = this->vtkWidgetRepresentation::SizeHandlesInPixels(1., ct);

    // Handle position and radius.
    this->SetPosition(ct, radius);
}

//----------------------------------------------------------------------------
int FITKTransformRepresentation::HighlightHandle(vtkProp* prop)
{
    // first unhighlight anything picked
    if (this->CurrentHandle)
    {
        for (int i = 0; i < 7; i++)
        {
            if (this->CurrentHandle == this->Handle[i])
            {
                this->CurrentHandle->SetProperty(this->HandleProperty[i]);
                break;
            }
        }
    }

    this->CurrentHandle = static_cast<vtkActor*>(prop);

    if (this->CurrentHandle)
    {
        this->CurrentHandle->SetProperty(this->SelectedHandleProperty);
        for (int i = 0; i < 7; i++)
        {
            if (this->CurrentHandle == this->Handle[i])
            {
                return i;
            }
        }
    }

    return -1;
}

//------------------------------------------------------------------------------
void FITKTransformRepresentation::RegisterPickers()
{
    vtkPickingManager* pm = this->GetPickingManager();
    if (!pm)
    {
        return;
    }
    pm->AddPicker(this->HandlePicker, this);
}

//----------------------------------------------------------------------------
void FITKTransformRepresentation::PrintSelf(ostream& os, vtkIndent indent)
{
    this->Superclass::PrintSelf(os, indent);

    double* bounds = this->InitialBounds;
    os << indent << "Initial Bounds: "
        << "(" << bounds[0] << "," << bounds[1] << ") "
        << "(" << bounds[2] << "," << bounds[3] << ") "
        << "(" << bounds[4] << "," << bounds[5] << ")\n";

    if (this->HandleProperty)
    {
        os << indent << "Handle Property: " << this->HandleProperty << "\n";
    }
    else
    {
        os << indent << "Handle Property: (none)\n";
    }
    if (this->SelectedHandleProperty)
    {
        os << indent << "Selected Handle Property: " << this->SelectedHandleProperty << "\n";
    }
    else
    {
        os << indent << "SelectedHandle Property: (none)\n";
    }
}