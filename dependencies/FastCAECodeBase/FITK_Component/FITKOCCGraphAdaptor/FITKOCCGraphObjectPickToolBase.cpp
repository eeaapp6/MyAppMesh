/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCGraphObjectPickToolBase.h"

// OCC
#include <Standard_Handle.hxx>
#include <AIS_InteractiveContext.hxx>
#include <AIS_ColoredShape.hxx>

// Graph
#include "FITKOCCCommons.h"

namespace Exchange
{
    FITKOCCGraphObjectPickToolBase::FITKOCCGraphObjectPickToolBase()
    {
        // Initialize the visibility hash.( not used )
        m_viewModes[FITKOCCCommons::ShapeMeshViewMode::SMVM_Shade] = true;
        m_viewModes[FITKOCCCommons::ShapeMeshViewMode::SMVM_Wireframe] = true;
        m_viewModes[FITKOCCCommons::ShapeMeshViewMode::SMVM_Vertex] = true;

        // Initialize select modes.
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeShape] = false;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeVertex] = false;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeEdge] = true;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeWire] = false;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeFace] = true;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeShell] = false;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeSolid] = false;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeCompound] = false;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_MeshNode] = false;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_MeshElement] = false;
    }

    FITKOCCGraphObjectPickToolBase::~FITKOCCGraphObjectPickToolBase()
    {
        // Clear the pointer.
        m_aisPickShape = nullptr;
    }

    void FITKOCCGraphObjectPickToolBase::addSelectMode(CommonOCC::SelectMode mode, bool force)
    {
        // This graph object cannot change the select mode.
        Q_UNUSED(mode);
        Q_UNUSED(force);
    }

    void FITKOCCGraphObjectPickToolBase::clearSelectMode()
    {
        // This graph object cannot change the select mode.
    }
}   // namespace Exchange