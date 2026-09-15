/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCGraphObjectTopoShape.h"

// OCC
#include <Standard_Handle.hxx>
#include <AIS_ColoredShape.hxx>
#include <AIS_InteractiveContext.hxx>
#include <Prs3d_Drawer.hxx>
#include <Prs3d_ShadingAspect.hxx>
#include <Prs3d_LineAspect.hxx>

// Data
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsVirtualTopo.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsVirtualTopoMapper.h"
#include "FITK_Component/FITKGeoCompOCC/FITKOCCVirtualTopoCreator.h"

namespace Exchange
{
    FITKOCCGraphObjectTopoShape::FITKOCCGraphObjectTopoShape(Interface::FITKAbsVirtualTopo* vTopoData)
        : FITKOCCGraphObjectTopoBase(vTopoData)
    {
        if (!vTopoData)
        {
            return;
        }

        // Initialize.
        bool flag = initShapeData();
        if (!flag)
        {
            return;
        }

        // Generate the interactive objects.
        generateShape();

        // Set the render layer to TopMost.
        enableTopMost(true);
    }

    FITKOCCGraphObjectTopoShape::~FITKOCCGraphObjectTopoShape()
    {
        // Don't need to delete pointers here.
        m_aisShape = nullptr;
    }

    bool FITKOCCGraphObjectTopoShape::initShapeData()
    {
        Interface::FITKAbsVirtualTopo* vTopoData = dynamic_cast<Interface::FITKAbsVirtualTopo*>(_dataObj);
        if (!vTopoData)
        {
            return false;
        }

        OCC::FITKOCCTopoShape* occTShape = vTopoData->getShapeT<OCC::FITKOCCTopoShape>();
        if (!occTShape)
        {
            return false;
        }

        // Check the shape data.
        TopoDS_Shape shape = occTShape->getTopoShape();
        if (shape.IsNull())
        {
            return false;
        }

        return true;
    }

    void FITKOCCGraphObjectTopoShape::generateShape()
    {
        Interface::FITKAbsVirtualTopo* vTopoData = dynamic_cast<Interface::FITKAbsVirtualTopo*>(_dataObj);
        if (!vTopoData)
        {
            return;
        }

        OCC::FITKOCCTopoShape* occShape = vTopoData->getShapeT<OCC::FITKOCCTopoShape>();
        if (!occShape)
        {
            return;
        }

        // Get the shape.
        TopoDS_Shape shape = occShape->getTopoShape();
        
        // Create the shape and handle.
        m_aisShape = new AIS_ColoredShape(shape);
        Handle(AIS_Shape) shapeHandle(m_aisShape);

        Quantity_Color color = FITKOCCCommons::QColorToQuan(Qt::yellow);
        double lineWidth = FITKOCCCommons::s_toolLineWidth;

        // Initialize line width and default color.
        m_aisShape->SetWidth(lineWidth);
        m_aisShape->SetColor(color);

        // Drawer properties.
        //@{
        m_aisShape->SetDisplayMode(AIS_DisplayMode::AIS_Shaded);
        m_aisShape->SetTransparency(FITKOCCCommons::s_transpacency);

        Handle(Prs3d_Drawer) properties = m_aisShape->Attributes();

        // Face properties.
        properties->SetupOwnShadingAspect();
        properties->ShadingAspect()->SetColor(color);
        //@}

        // Add to the interactive object list.
        addInteractiveObj(shapeHandle);
    }
}   // namespace Exchange