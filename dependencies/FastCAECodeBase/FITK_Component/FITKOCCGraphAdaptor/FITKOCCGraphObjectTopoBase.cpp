/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCGraphObjectTopoBase.h"

// OCC
#include <Standard_Handle.hxx>
#include <AIS_InteractiveContext.hxx>
#include <AIS_InteractiveObject.hxx>

// Data
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsVirtualTopo.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsVirtualTopoMapper.h"
#include "FITK_Component/FITKGeoCompOCC/FITKOCCVirtualTopoCreator.h"

namespace Exchange
{
    FITKOCCGraphObjectTopoBase::FITKOCCGraphObjectTopoBase(Core::FITKAbstractDataObject* dataObj)
        : FITKOCCGraphObject3D(dataObj)
    {
        // Save the data type.
        m_graphInfo.Type = FITKOCCCommons::OtherGraph;

        // Initialize select modes.
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeCompound] = false;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeVertex] = false;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeEdge] = false;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeWire] = false;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeFace] = false;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeShell] = false;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeSolid] = false;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeCompound] = false;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_MeshNode] = false;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_MeshElement] = false;

        // Save the fixed size flag.
        m_hasFixedSize = true;
    }

    void FITKOCCGraphObjectTopoBase::updateInteractiveObjs()
    {
        if (!m_context)
        {
            return;
        }

        for (const opencascade::handle<AIS_InteractiveObject> & obj : m_objs)
        {
            if (obj)
            {
                // Check if the interactive object is visible.
                if (m_context->DisplayStatus(obj) == AIS_DisplayStatus::AIS_DS_Displayed)
                {
                    m_context->Redisplay(obj, true);
                }
            }
        }
    }

    void FITKOCCGraphObjectTopoBase::setColor(QColor color)
    {
        if (!color.isValid())
        {
            return;
        }

        Quantity_Color quanColor = FITKOCCCommons::QColorToQuan(color);

        for (const opencascade::handle<AIS_InteractiveObject> & obj : m_objs)
        {
            if (obj)
            {
                // Show the object.
                obj->SetColor(quanColor);
            }
        }
    }

    void FITKOCCGraphObjectTopoBase::setEnableTransparency(bool isOn)
    {
        for (const opencascade::handle<AIS_InteractiveObject> & obj : m_objs)
        {
            if (obj)
            {
                // Show the object.
                obj->SetTransparency(isOn ? FITKOCCCommons::s_transpacency : 0.);
            }
        }    
    }

    void FITKOCCGraphObjectTopoBase::setVisible(bool visibility)
    {
        if (!m_context)
        {
            return;
        }

        if (visibility)
        {
            for (const opencascade::handle<AIS_InteractiveObject> & obj : m_objs)
            {
                if (obj)
                {
                    // Show the object.
                    m_context->Display(obj, true);
                }
            }
        }
        else
        {
            for (const opencascade::handle<AIS_InteractiveObject> & obj : m_objs)
            {
                if (obj)
                {
                    // Hide the object.
                    m_context->Erase(obj, true);
                }
            }
        }
    }
}   // namespace Exchange