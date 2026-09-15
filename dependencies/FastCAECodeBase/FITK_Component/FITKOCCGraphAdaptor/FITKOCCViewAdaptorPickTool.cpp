/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCViewAdaptorPickTool.h"

// Graph
#include "FITKOCCCommons.h"
#include "FITKOCCGraphObjectPickDirection.h"
#include "FITKOCCGraphObjectPickProjPos.h"

namespace Exchange
{
    bool FITKOCCViewAdaptorPickTool::update()
    {
        // Check the details.
        QString typeName = m_details.value<QString>();
        if (typeName.isEmpty())
        {
            return false;
        }

        // If do not need to update and the output is not empty,
        // it means the output has been created, so return true.
        m_needUpdate = true;
        if (!m_needUpdate)
        {
            return (m_outputData != nullptr);
        }

        // If output is exist, then delete it and create a new one.
        if (m_outputData)
        {
            delete m_outputData;
            m_outputData = nullptr;
        }

        // The output data object for visualization.
        FITKOCCGraphObject3D* outputData{ nullptr };
        if (typeName == "EDGE_POS")
        {
            outputData = new FITKOCCGraphObjectPickProjPos;
        }
        else if (typeName == "DIRECTION")
        {
            outputData = new FITKOCCGraphObjectPickDirection;
        }

        m_needUpdate = false;

        if (!outputData)
        {
            return false;
        }

        // If the interactive object count is 0, then this graph object is invalid.
        if (outputData->getNumberOfInteractiveObjs() == 0)
        {
            delete outputData;
            return false;
        }

        m_outputData = outputData;

        return true;
    }
}   // namespace Exchange
