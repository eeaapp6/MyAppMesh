/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKActorClipTool.h"

// VTK
#include <vtkActor.h>
#include <vtkDataSet.h>
#include <vtkAlgorithmOutput.h>
#include <vtkDataSetMapper.h>
#include <vtkPassThrough.h>

// Filters
#include <vtkTableBasedClipDataSet.h>
#include <vtkCutter.h>
#include <vtkExtractGeometry.h>

FITKActorClipTool::FITKActorClipTool(vtkActor* actor) : m_actor(actor)
{
    // Create the filter.
    m_passThrough = vtkPassThrough::New();
    m_passThrough->SetAllowNullInput(true);

    // Connect the pass through.
    if (!m_actor)
    {
        return;
    }

    vtkMapper* mapper = m_actor->GetMapper();
    if (!mapper)
    {
        return;
    }

    mapper->SetInputConnection(m_passThrough->GetOutputPort());
}

FITKActorClipTool::~FITKActorClipTool()
{
    // Delete the filters.
    if (m_passThrough)
    {
        m_passThrough->Delete();
        m_passThrough = nullptr;
    }

    if (m_postAlg)
    {
        m_postAlg->Delete();
        m_postAlg = nullptr;
    }

    m_actor = nullptr;
}

void FITKActorClipTool::setInputDataObject(vtkDataObject* data, int port)
{
    // Check the filter and input data.
    if (!data || !m_passThrough)
    {
        return;
    }

    m_passThrough->SetInputDataObject(port, data);

    if (!m_actor)
    {
        return;
    }

    vtkMapper* mapper = m_actor->GetMapper();
    if (!mapper)
    {
        return;
    }

    // Change the rendering filter.
    if (m_postAlg)
    {
        mapper->SetInputConnection(m_postAlg->GetOutputPort());
    }
    else
    {
        mapper->SetInputConnection(m_passThrough->GetOutputPort());
    }
}

void FITKActorClipTool::setInputConnection(vtkAlgorithmOutput* input, int port)
{
    // Check the filter and input connection.
    if (!input || !m_passThrough)
    {
        return;
    }

    m_passThrough->SetInputConnection(port, input);

    if (!m_actor)
    {
        return;
    }

    vtkMapper* mapper = m_actor->GetMapper();
    if (!mapper)
    {
        return;
    }

    // Change the rendering filter.
    if (m_postAlg)
    {
        mapper->SetInputConnection(m_postAlg->GetOutputPort());
    }
    else
    {
        mapper->SetInputConnection(m_passThrough->GetOutputPort());
    }
}

void FITKActorClipTool::setClipType(ClipType type, bool deleteOldFilter)
{
    // Save the clip type.
    m_clipType = type;

    switch (type)
    {
    case ClipType::Clip:
    {
        setClipAlgorithm(vtkTableBasedClipDataSet::New(), deleteOldFilter);
        break;
    }
    case ClipType::Cut:
    {
        setClipAlgorithm(vtkCutter::New(), deleteOldFilter);
        break;
    }
    case ClipType::ExtractGeometry:
    {
        vtkExtractGeometry* extractGeometry = vtkExtractGeometry::New();
        //默认保留裁切平面所经过单元
        extractGeometry->ExtractBoundaryCellsOn();
        setClipAlgorithm(extractGeometry, deleteOldFilter);
        break;
    }
    default:
    {
        m_clipType = ClipType::NoneType;
        setClipAlgorithm(nullptr, deleteOldFilter);
        break;
    }
    }
}

void FITKActorClipTool::setClipInside(ClipInside type)
{
    m_insideType = type;

    switch (m_clipType){
    case ClipType::Clip:{
        vtkTableBasedClipDataSet* alg = vtkTableBasedClipDataSet::SafeDownCast(m_postAlg);
        if (alg == nullptr) {
            return;
        }
        switch (type){
        case ClipInside::Off: alg->InsideOutOff(); break;
        case ClipInside::On: alg->InsideOutOn(); break;
        }
        break;
    }
    case ClipType::ExtractGeometry:{
        vtkExtractGeometry* alg = vtkExtractGeometry::SafeDownCast(m_postAlg);
        if (alg == nullptr) {
            return;
        }
        switch (type) {
        case ClipInside::Off: alg->ExtractInsideOff(); break;
        case ClipInside::On: alg->ExtractInsideOn(); break;
        }
        break;
    }
    }
}

void FITKActorClipTool::setClipImplicitFunction(vtkImplicitFunction* func)
{
    switch (m_clipType)
    {
    case ClipType::Clip:
    {
        vtkTableBasedClipDataSet* alg = vtkTableBasedClipDataSet::SafeDownCast(m_postAlg);
        if (alg)
        {
            alg->SetClipFunction(func);
        }

        break;
    }
    case ClipType::Cut:
    {
        vtkCutter* alg = vtkCutter::SafeDownCast(m_postAlg);
        if (alg)
        {
            alg->SetCutFunction(func);
        }

        break;
    }
    case ClipType::ExtractGeometry:
    {
        vtkExtractGeometry* alg = vtkExtractGeometry::SafeDownCast(m_postAlg);
        if (alg)
        {
            alg->SetImplicitFunction(func);
        }

        break;
    }
    default:
        return;
    }
}

void FITKActorClipTool::setClipAlgorithm(vtkAlgorithm* func, bool deleteOldFilter)
{
    if (m_postAlg && deleteOldFilter)
    {
        m_postAlg->Delete();
        m_postAlg = nullptr;
    }

    m_postAlg = func;

    if (m_postAlg)
    {
        m_postAlg->SetInputConnection(m_passThrough->GetOutputPort());
    }

    // Connect the new filter.
    if (!m_actor)
    {
        return;
    }

    vtkMapper* mapper = m_actor->GetMapper();
    if (!mapper)
    {
        return;
    }

    // Change the rendering filter.
    if (m_postAlg)
    {
        mapper->SetInputConnection(m_postAlg->GetOutputPort());
    }
    else
    {
        mapper->SetInputConnection(m_passThrough->GetOutputPort());
    }
}

void FITKActorClipTool::setNewMapper(vtkMapper* mapper)
{
    if (mapper)
    {
        vtkAlgorithmOutput* conn = mapper->GetInputConnection(0, 0);
        vtkDataObject* data = mapper->GetInputDataObject(0, 0);
        if (conn)
        {
            m_passThrough->SetInputConnection(conn);
        }
        else if (data)
        {
            m_passThrough->SetInputDataObject(data);
        }
        else
        {
            m_passThrough->RemoveAllInputs();
            m_passThrough->RemoveAllInputConnections(0);
        }

        mapper->SetInputConnection(m_passThrough->GetOutputPort());
    }
}