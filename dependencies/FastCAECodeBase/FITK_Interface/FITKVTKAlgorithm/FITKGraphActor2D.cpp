/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKGraphActor2D.h"

#include <vtkObjectFactory.h>

#include <vtkMapper2D.h>
#include <vtkAlgorithmOutput.h>
#include <vtkDataSet.h>
#include <vtkRenderer.h>

FITKGraphActor2D* FITKGraphActor2D::New()
{
    // Return the instance.
    return new FITKGraphActor2D;
}

FITKGraphActor2D::FITKGraphActor2D()
{
    // Nothing to do here.
}

FITKGraphActor2D::~FITKGraphActor2D()
{
    // Clear the graph object's pointer.
    m_graphObj = nullptr;
}

void FITKGraphActor2D::setGraphObject(Comp::FITKGraphObjectVTK* obj)
{
    m_graphObj = obj;
}

Comp::FITKGraphObjectVTK* FITKGraphActor2D::getGraphObject()
{
    return m_graphObj;
}

void FITKGraphActor2D::setActorType(ActorType2D type)
{
    m_actorType = type;
}

ActorType2D FITKGraphActor2D::getActorType()
{
    return m_actorType;
}

void FITKGraphActor2D::update()
{
    // If mapper is empty, return.
    vtkMapper2D* mapper = GetMapper();
    if (!mapper)
    {
        return;
    }

    // Update the actor's mapper.
    mapper->Update();
}

void FITKGraphActor2D::ShallowCopy(vtkProp* prop)
{
    // Override shallow copy.
    // FITKGraphActor2D* f = FITKGraphActor2D::SafeDownCast(prop);
    this->Superclass::ShallowCopy(prop);
}
//@}

// vtkStandardNewMacro(vtkActorEx);