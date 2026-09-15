/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCGraphObjectTopoDirection.h"

// OCC
#include <Standard_Handle.hxx>
#include <BRepGProp_Face.hxx>
#include <BRep_Tool.hxx>
#include <TopoDS.hxx>
#include <AIS_Shape.hxx>
#include <AIS_InteractiveContext.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <Prs3d_Drawer.hxx>
#include <Prs3d_ShadingAspect.hxx>
#include <Prs3d_LineAspect.hxx>
#include <Standard_Version.hxx>

// Data
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsVirtualTopo.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsVirtualTopoMapper.h"
#include "FITK_Component/FITKGeoCompOCC/FITKOCCVirtualTopoCreator.h"

namespace Exchange
{
    FITKOCCGraphObjectTopoDirection::FITKOCCGraphObjectTopoDirection(Interface::FITKAbsVirtualTopo* vTopoData)
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

        // Save the default output role.
        m_defaultOutputRole = ROLE_OUTPUT_TOPO_DIRECTION;
    }

    FITKOCCGraphObjectTopoDirection::~FITKOCCGraphObjectTopoDirection()
    {
        // Don't need to delete pointers here.
        m_aisShape = nullptr;
    }

    bool FITKOCCGraphObjectTopoDirection::initShapeData()
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

        bool flag = vTopoData->getShapeType() == Interface::FITKGeoEnum::VTopoShapeType::VSEdge ||
            vTopoData->getShapeType() == Interface::FITKGeoEnum::VTopoShapeType::VSFace;
        return flag;
    }

    void FITKOCCGraphObjectTopoDirection::generateShape()
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

        // Get the shape and calculate the shape normal or direction.
        TopoDS_Shape shape = occShape->getTopoShape();
        gp_Vec dir;
        gp_Pnt pt;
        bool flag = FITKOCCCommons::GetShapeDirection(shape, dir, pt);
        if (!flag)
        {
            return;
        }

        dir.Normalize();
        gp_Dir gDir(dir.XYZ());
        gp_Pnt zeroPt(0., 0., 0.);
        TopoDS_Shape arrow = FITKOCCCommons::CreateArrow(zeroPt, gDir);

        if (arrow.IsNull())
        {
            return;
        }

        // Create the shape and handle.
        m_aisShape = new AIS_Shape(arrow);
        Handle(AIS_Shape) shapeHandle(m_aisShape);

        // Initialize properties.
#if OCC_VERSION_HEX < 0x070600
        Handle(Graphic3d_TransformPers) transPer = Graphic3d_TransformPers::FromDeprecatedParams(
            Graphic3d_TransModeFlags::Graphic3d_TMF_ZoomPers, pt);
#else
        Handle(Graphic3d_TransformPers) transPer = new Graphic3d_TransformPers(
            Graphic3d_TransModeFlags::Graphic3d_TMF_ZoomPers, pt);
#endif

        m_aisShape->SetTransformPersistence(transPer);

        // Drawer properties.
        //@{
        Handle(Prs3d_Drawer) properties = m_aisShape->Attributes();

        // Face properties.
        properties->SetupOwnShadingAspect();
        properties->ShadingAspect()->SetColor(FITKOCCCommons::QColorToQuan(FITKOCCCommons::s_markColor));
        //@}
        //@}

        // Add to the interactive object list.
        addInteractiveObj(shapeHandle);

        // Save the direction.
        QList<double> dirList{ dir.X(), dir.Y(), dir.Z() };
        QVariant var = QVariant::fromValue<QList<double>>(dirList);
        setOutputVariantData(var, ROLE_OUTPUT_TOPO_DIRECTION);
    }
}   // namespace Exchange