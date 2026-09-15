/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCViewAdaptorMesh.h"

// Graph and data
#include "FITKOCCGraphObjectMesh.h"
#include "FITK_Interface/FITKInterfaceModel/FITKUnstructuredMesh.h"
// #include "FITK_Interface/FITKInterfaceModel/FITKStructuredMesh.h"

namespace Exchange
{
    bool FITKOCCViewAdaptorMesh::update()
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
        Interface::FITKAbstractMesh* mesh = dynamic_cast<Interface::FITKAbstractMesh*>(_dataObj);
        if (!mesh)
        {
            return false;
        }

        // If output is exist, then delete it and create a new one.
        if (m_outputData)
        {
            delete m_outputData;
            m_outputData = nullptr;
        }

        // The output data object for visualization.
        FITKOCCGraphObjectMesh* outputData = new FITKOCCGraphObjectMesh(mesh);

        m_needUpdate = false;

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
