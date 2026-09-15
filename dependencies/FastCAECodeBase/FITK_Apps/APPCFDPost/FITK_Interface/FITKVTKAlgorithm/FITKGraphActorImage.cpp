/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKGraphActorImage.h"

#include <vtkObjectFactory.h>

#include <vtkAlgorithmOutput.h>
#include <vtkDataSet.h>
#include <vtkRenderer.h>

FITKGraphActorImage* FITKGraphActorImage::New()
{
    // Return the instance.
    return new FITKGraphActorImage;
}

FITKGraphActorImage::FITKGraphActorImage()
{
    // Nothing to do here.
}

FITKGraphActorImage::~FITKGraphActorImage()
{
    // Clear the graph object's pointer.
    m_graphObj = nullptr;
}

void FITKGraphActorImage::setGraphObject(Comp::FITKGraphObjectVTK* obj)
{
    m_graphObj = obj;
}

Comp::FITKGraphObjectVTK* FITKGraphActorImage::getGraphObject()
{
    return m_graphObj;
}

void FITKGraphActorImage::ShallowCopy(vtkProp* prop)
{
    // Override shallow copy.
    // FITKGraphActorImage* f = FITKGraphActorImage::SafeDownCast(prop);
    this->Superclass::ShallowCopy(prop);
}
//@}

// vtkStandardNewMacro(vtkActorEx);