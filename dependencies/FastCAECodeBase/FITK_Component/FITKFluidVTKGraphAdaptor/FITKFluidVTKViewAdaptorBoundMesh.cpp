/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKFluidVTKViewAdaptorBoundMesh.h"

// Graph and data
#include "FITKFluidVTKGraphObjectBoundMesh.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredFluidMeshVTK.h"

namespace Exchange
{
    bool FITKFluidVTKViewAdaptorBoundMesh::update()
    {
        if (!_dataObj)
        {
            return false;
        }

        // If do not need to update and the output is not empty,
        // it means the output has been created, so return true.
        if (!m_needUpdate)
        {
            return (m_outputData != nullptr);
        }

        // Down cast the input data.
        Interface::FITKBoundaryMeshVTK* data = dynamic_cast<Interface::FITKBoundaryMeshVTK*>(_dataObj);
        if (!data)
        {
            return false;
        }

        // If output is exist, then delete it and create a new one.
        if (m_outputData)
        {
            delete m_outputData;
            m_outputData = nullptr;
        }

        // The output data object for view
        FITKFluidVTKGraphObjectBoundMesh* outputData = new FITKFluidVTKGraphObjectBoundMesh(data);

        m_needUpdate = false;

        // If the actor count is 0, then this graph object is invalid.
        if (outputData->getActorCount() == 0)
        {
            delete outputData;
            return false;
        }

        m_outputData = outputData;

        return true;
    }
}   // namespace Exchange
