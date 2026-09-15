/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCGraphObjectToolBase.h"

// OCC
#include <Standard_Handle.hxx>
#include <TopoDS_Shape.hxx>
#include <AIS_InteractiveContext.hxx>
#include <AIS_InteractiveObject.hxx>
#include <AIS_ColoredShape.hxx>
#include <Prs3d_Drawer.hxx>
#include <Prs3d_ShadingAspect.hxx>
#include <Prs3d_PointAspect.hxx>

// Global data
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"

// Data
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsVirtualTopo.h"
#include "FITK_Component/FITKGeoCompOCC/FITKOCCVirtualTopoCreator.h"

#include <QList>
#include <QVariant>

namespace Exchange
{
    FITKOCCGraphObjectToolBase::FITKOCCGraphObjectToolBase()
        : FITKOCCGraphObject3D(nullptr)
    {
        // Save the fixed size flag.
        m_hasFixedSize = false;
    }

    void FITKOCCGraphObjectToolBase::updateInteractiveObjs()
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

    void FITKOCCGraphObjectToolBase::setColor(QColor color)
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

    void FITKOCCGraphObjectToolBase::setEnableTransparency(bool isOn)
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

    void FITKOCCGraphObjectToolBase::setVisible(bool visibility)
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

    Interface::FITKAbsVirtualTopo* FITKOCCGraphObjectToolBase::getVirtualTopoById(int vId)
    {
        // Get the virtual topo data object.
        return FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(vId);
    }

    TopoDS_Shape FITKOCCGraphObjectToolBase::getTopoShapeByVirtualTopo(Interface::FITKAbsVirtualTopo* vTopo)
    {
        if (!vTopo)
        {
            return TopoDS_Shape();
        }

        // Get the OCC virtual shape.
        OCC::FITKOCCTopoShape* occVTopo =  vTopo->getShapeT<OCC::FITKOCCTopoShape>();
        if (!occVTopo)
        {
            return TopoDS_Shape();
        }

        // Get the shape data.
        TopoDS_Shape shape = occVTopo->getTopoShape();
        return shape;
    }

    bool FITKOCCGraphObjectToolBase::getDataVisibility()
    {
        return true;
    }
}   // namespace Exchange