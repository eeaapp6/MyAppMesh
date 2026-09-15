/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCGraphObjectSketchShape.h"

// OCC
#include <Standard_Handle.hxx>
#include <AIS_InteractiveContext.hxx>

// Graph
#include "FITKOCCCommons.h"
#include "FITKOCCAIS_ColoredShape.h"

// Data
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoSketch2D.h"

namespace Exchange
{
    FITKOCCGraphObjectSketchShape::FITKOCCGraphObjectSketchShape(Interface::FITKAbsGeoShapeAgent* sketchData)
        : FITKOCCGraphObjectModelShape()
    {
        if (!sketchData)
        {
            return;
        }

        // Save the object.
        _dataObj = sketchData;

        // Initialize the visibility hash.
        m_viewModes[FITKOCCCommons::ShapeMeshViewMode::SMVM_Shade] = true;
        m_viewModes[FITKOCCCommons::ShapeMeshViewMode::SMVM_Wireframe] = true;
        m_viewModes[FITKOCCCommons::ShapeMeshViewMode::SMVM_Vertex] = true;

        // Initialize.
        bool flag = initShapeData();
        if (!flag)
        {
            return;
        }

        // Generate the interactive objects.
        generateShape();

        // Forced to display as wireframe.
        if (m_fAisShape)
        {
            m_fAisShape->SetDisplayMode(AIS_DisplayMode::AIS_WireFrame);
        }

        // Save the data object id.
        m_graphInfo.DataObjId = sketchData->getDataObjectID();

        // Save the data type.
        m_graphInfo.Type = FITKOCCCommons::SketchGraph;

        // Initialize select modes.
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeShape] = true;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeVertex] = true;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeEdge] = true;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeWire] = true;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeFace] = true;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeShell] = false;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeSolid] = false;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeCompound] = true;  
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_MeshNode] = false;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_MeshElement] = false;

        // Save the fixed size flag.
        m_hasFixedSize = true;
    }

    void FITKOCCGraphObjectSketchShape::addSelectMode(CommonOCC::SelectMode mode, bool force)
    {
        // Clear select modes.
        if (mode == CommonOCC::SelectMode::S_None)
        {
            clearSelectMode();
            return;
        }

        if (!m_context || !m_fAisShape)
        {
            return;
        }

        // Check the select mode.
        CommonOCC::SelectMode realMode = CommonOCC::SelectMode::S_None;
        if (m_supportedSelectModeHash[mode])
        {
            realMode = mode;
        }

        // Change select mode to the full shape.
        if (realMode != CommonOCC::SelectMode::S_None)
        {
            if (!force && realMode != CommonOCC::SelectMode::S_ShapeVertex)
            {
                realMode = CommonOCC::SelectMode::S_ShapeShape;
            }
            else
            {
                realMode = CommonOCC::SelectMode::S_ShapeVertex;
            }
        }

        if (m_currSelectModes.contains(realMode))
        {
            return;
        }

        // Save the mode.
        if (realMode != CommonOCC::SelectMode::S_None)
        {
            m_currSelectModes.push_back(realMode);
        }

        // Set all objects'select mode by default.
        if (realMode == CommonOCC::SelectMode::S_None)
        {
            m_context->Deactivate(m_fAisShape);
        }
        else
        {
            // Check if the object is visible.
            if (getVisibility())
            {
                m_context->Load(m_fAisShape, -1);
                m_context->Activate(m_fAisShape, realMode, true);
            }
        }
    }
}   // namespace Exchange