/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKFluidVTKGraphObjectModelShape.h"

// Data
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoShapeAgent.h"

namespace Exchange
{
    FITKFluidVTKGraphObjectModelShape::FITKFluidVTKGraphObjectModelShape(Interface::FITKAbsGeoShapeAgent* modelData)
        : FITKFluidVTKGraphObjectModelBase(modelData)
    {
        if (!modelData)
        {
            return;
        }

        // Initialize.
        init();

        // Convert shape data.
        bool flag = generateDataSet(modelData);
        if (!flag)
        {
            return;
        }

        // Generate the shape actors.
        generateGraph();

        // Save the data type.
        m_shapeInfo.Type = FITKFluidVTKCommons::ModelShape;

        // Set the layer need to be rendered.
        setRenderLayer(0, 2);
    }

    void FITKFluidVTKGraphObjectModelShape::update(bool forceUpdate)
    {
        // Get the abstract shape agent.
        Interface::FITKAbsGeoShapeAgent* shapeAgent = dynamic_cast<Interface::FITKAbsGeoShapeAgent*>(_dataObj);
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