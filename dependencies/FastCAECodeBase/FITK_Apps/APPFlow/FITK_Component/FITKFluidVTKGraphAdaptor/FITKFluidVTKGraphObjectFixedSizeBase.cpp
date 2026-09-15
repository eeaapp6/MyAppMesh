/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKFluidVTKGraphObjectFixedSizeBase.h"

// VTK
#include <vtkGlyph3D.h>
#include <vtkDistanceToCamera.h>

// Graph
#include "FITK_Interface/FITKVTKAlgorithm/FITKGraphActor.h"

// Render
#include "FITK_Component/FITKRenderWindowVTK/FITKGraphRender.h"

namespace Exchange
{
    FITKFluidVTKGraphObjectFixedSizeBase::FITKFluidVTKGraphObjectFixedSizeBase(Core::FITKAbstractDataObject* dataObj)
        : FITKFluidVTKGraphObject3D(dataObj)
    {
        // Nothing to do here.
        // Create actors, glyphs and other filters.( Created in sub-class )

        // Set the layer need to be rendered.
        setRenderLayer(1);

        // Save if the bounds are fixed or dynamic.
        m_hasFixedBounds = false;
    }

    FITKFluidVTKGraphObjectFixedSizeBase::~FITKFluidVTKGraphObjectFixedSizeBase()
    {
        // Delete glyph filters.
        deleteVtkObjs(m_glyphList);

        // Delete fixed-sized filters.
        for (vtkDistanceToCamera* distToCamera : m_distToCameraList)
        {
            distToCamera->SetRenderer(nullptr);
        }

        deleteVtkObjs(m_distToCameraList);
    }

    void FITKFluidVTKGraphObjectFixedSizeBase::setFITKRender(Comp::FITKGraphRender* render)
    {
        if (!render)
        {
            return;
        }

        // Set the renderer for calculating size.
        for (vtkDistanceToCamera* obj : m_distToCameraList)
        {
            if (obj)
            {
                obj->SetRenderer(render->getRenderer());
            }
        }
    }

    double FITKFluidVTKGraphObjectFixedSizeBase::getScreenSize(int index)
    {
        // Get the filter in this index, and get its screen size set.
        if (index < 0 || index >= m_distToCameraList.count())
        {
            return 0.;
        }

        vtkDistanceToCamera* obj = m_distToCameraList[index];
        if (!obj)
        {
            return 0.;
        }

        return obj->GetScreenSize();
    }

    void FITKFluidVTKGraphObjectFixedSizeBase::setScreenSize(double pixelSize, int index)
    {
        // Set pixel size.
        // The value of index -1 means set all actors to this size.
        if (index == -1)
        {
            for (vtkDistanceToCamera* obj : m_distToCameraList)
            {
                if (obj)
                {
                    obj->SetScreenSize(pixelSize);
                }
            }
        }
        // Set indexed actor size.
        else
        {
            if (index < 0 || index >= m_distToCameraList.count())
            {
                return;
            }

            vtkDistanceToCamera* obj = m_distToCameraList[index];
            if (obj)
            {
                obj->SetScreenSize(pixelSize);
            }
        }
    }
}   // namespace Exchange