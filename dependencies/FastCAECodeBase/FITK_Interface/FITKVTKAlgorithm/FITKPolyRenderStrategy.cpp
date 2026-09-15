/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPolyRenderStrategy.h"

#include "vtkActor2D.h"
#include "vtkObjectFactory.h"
#include "vtkRenderer.h"
#include "vtkSmartPointer.h"
#include "vtkPolyDataMapper2D.h"
#include "vtkWindow.h"
#include "vtkProperty2D.h"

#include "vtkPolyDataAlgorithm.h"
#include "FITKPolygonSource.h"

vtkStandardNewMacro(FITKPolyRenderStrategy);

// vtkCxxSetObjectMacro(FITKPolyRenderStrategy, Renderer, vtkRenderer);

FITKPolyRenderStrategy::FITKPolyRenderStrategy()
{
    // Initialize label actor and mapper.
    this->Mapper = vtkPolyDataMapper2D::New();
    this->Actor = vtkActor2D::New();
    this->Actor->SetMapper(this->Mapper);

    this->InputSource = FITKPolygonSource::New();
    this->Mapper->SetInputConnection(this->InputSource->GetOutputPort());

    // Initialize to round source.
    SetSourceType(Round);
}

FITKPolyRenderStrategy::~FITKPolyRenderStrategy()
{
    // Delete actor and mapper.
    this->SetRenderer(nullptr);
    this->Mapper->Delete();
    this->Actor->Delete();

    // Delete the source.
    if(this->InputSource)
    {
        this->InputSource->Delete();
        this->InputSource = nullptr;
    }
}

void FITKPolyRenderStrategy::ReleaseGraphicsResources(vtkWindow* window)
{
    // Release resources.
    this->Actor->ReleaseGraphicsResources(window);
}

void FITKPolyRenderStrategy::SetRenderer(vtkRenderer* ren)
{
    this->Renderer = ren;
}

void FITKPolyRenderStrategy::SetSourceType(SourceType type, bool drawPolygon)
{
    if (!this->InputSource)
    {
        this->InputSource = FITKPolygonSource::New();
        this->Mapper->SetInputConnection(this->InputSource->GetOutputPort());
    }

    // Crreate polygon source data and initialize.
    this->InputSource->SetRadius(this->PixelSize / 2);
    this->InputSource->SetGeneratePolyline(true);
    this->InputSource->SetGeneratePolygon(drawPolygon);

    switch (type)
    {
    case Round:
    {
        this->InputSource->SetNumberOfSides(10);
        break;
    }
    case X_Shape:
    {
        this->InputSource->SetNumberOfSides(2);
        break;
    }
    case Triangle:
    {
        this->InputSource->SetNumberOfSides(3);
        break;
    }
    case Rect:
    {
        this->InputSource->SetNumberOfSides(4);
        break;
    }
    default:
        return;
    }

    this->SourceTypeValue = type;
    this->DrawPolygon = drawPolygon;
}

SourceType FITKPolyRenderStrategy::GetSourceType()
{
    // 0 - 4
    return SourceTypeValue;
}

void FITKPolyRenderStrategy::SetPixelSize(int size)
{
    if (size <= 0 || !this->InputSource)
    {
        return;
    }

    this->PixelSize = size;
    this->InputSource->SetRadius(this->PixelSize / 2);
}

void FITKPolyRenderStrategy::SetColor(double rf, double gf, double bf)
{
    this->Actor->GetProperty()->SetColor(rf, gf, bf);
}

void FITKPolyRenderStrategy::SetColor(double* rgbf)
{
    this->Actor->GetProperty()->SetColor(rgbf);
}

void FITKPolyRenderStrategy::RenderPoly(int x[2])
{
    if (!this->Renderer || !this->InputSource)
    {
        vtkErrorMacro("Renderer and input source data must be set before rendering poly labels.");
        return;
    }

    this->Actor->GetPositionCoordinate()->SetCoordinateSystemToDisplay();
    this->Actor->GetPositionCoordinate()->SetValue(x[0], x[1], 0.0);
    this->Mapper->RenderOverlay(this->Renderer, this->Actor);
}

void FITKPolyRenderStrategy::PrintSelf(ostream& os, vtkIndent indent)
{
    this->Superclass::PrintSelf(os, indent);
}
