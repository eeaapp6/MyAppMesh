/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKFluidVTKViewAdaptorModelShape.h"

// Graph and data
// #include "FITKFluidVTKGraphObjectModelShape.h"
#include "FITKFluidVTKGraphObjectModelCmd.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoShapeAgent.h"

namespace Exchange
{
    bool FITKFluidVTKViewAdaptorModelShape::update()
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
        Interface::FITKAbsGeoShapeAgent* shapeAgent = dynamic_cast<Interface::FITKAbsGeoShapeAgent*>(_dataObj);
        if (!shapeAgent)
        {
            return false;
        }

        Interface::FITKAbsGeoCommand* cmd = shapeAgent->getGeoCommand();
        if (!cmd)
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
        FITKFluidVTKGraphObjectModelCmd* outputData = new FITKFluidVTKGraphObjectModelCmd(cmd);

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
