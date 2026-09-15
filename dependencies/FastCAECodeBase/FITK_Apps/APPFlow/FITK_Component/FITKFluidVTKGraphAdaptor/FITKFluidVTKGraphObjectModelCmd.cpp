/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKFluidVTKGraphObjectModelCmd.h"

// Data
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoShapeAgent.h"

namespace Exchange
{
    FITKFluidVTKGraphObjectModelCmd::FITKFluidVTKGraphObjectModelCmd(Interface::FITKAbsGeoCommand* modelData)
        : FITKFluidVTKGraphObjectModelBase(modelData)
    {
        if (!modelData)
        {
            return;
        }

        Interface::FITKAbsGeoShapeAgent* shapeAgent = modelData->getShapeAgent();
        if (!shapeAgent)
        {
            return;
        }

        // Initialize.
        init();

        // Convert shape data.
        bool flag = generateDataSet(shapeAgent);
        if (!flag)
        {
            return;
        }

        // Generate the shape actors.
        generateGraph();
    }

    void FITKFluidVTKGraphObjectModelCmd::update(bool forceUpdate)
    {
        Interface::FITKAbsGeoCommand* modelData = dynamic_cast<Interface::FITKAbsGeoCommand*>(_dataObj);
        if (!modelData)
        {
            return;
        }

        // Get the abstract shape agent.
        Interface::FITKAbsGeoShapeAgent* shapeAgent = modelData->getShapeAgent();
        if (!shapeAgent)
        {
            return;
        }

        if (!shapeAgent || m_testFlag)
        {
            return;
        }

        // Update visibility first.
        updateVisibility();

        // Check if the shape has been changed.
        QVariant newState;
        shapeAgent->createShapeState(newState);
        if (newState == m_shapeState && !forceUpdate)
        {
            return;
        }

        // Clear data set first.
        clearData();

        // Re-generate the shape.
        generateDataSet(shapeAgent);
    }
}   // namespace Exchange