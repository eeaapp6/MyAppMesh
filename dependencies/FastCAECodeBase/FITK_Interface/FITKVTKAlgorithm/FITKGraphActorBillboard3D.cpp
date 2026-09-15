/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKGraphActorBillboard3D.h"

#include <vtkObjectFactory.h>

#include <vtkAlgorithmOutput.h>
#include <vtkDataSet.h>
#include <vtkRenderer.h>

FITKGraphActorBillboard3D* FITKGraphActorBillboard3D::New()
{
    // Return the instance.
    return new FITKGraphActorBillboard3D;
}

FITKGraphActorBillboard3D::FITKGraphActorBillboard3D()
{
    // Nothing to do here.
}

FITKGraphActorBillboard3D::~FITKGraphActorBillboard3D()
{
    // Clear the graph object's pointer.
    m_graphObj = nullptr;
}

void FITKGraphActorBillboard3D::setGraphObject(Comp::FITKGraphObjectVTK* obj)
{
    m_graphObj = obj;
}

Comp::FITKGraphObjectVTK* FITKGraphActorBillboard3D::getGraphObject()
{
    return m_graphObj;
}

void FITKGraphActorBillboard3D::ShallowCopy(vtkProp* prop)
{
    // Override shallow copy.
    // FITKGraphActorBillboard3D* f = FITKGraphActorBillboard3D::SafeDownCast(prop);
    this->Superclass::ShallowCopy(prop);
}
//@}

// vtkStandardNewMacro(vtkActorEx);