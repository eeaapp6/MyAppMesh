/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCGraphObjectTopoIndexLabels.h"

// OCC
#include <TopoDS_Shape.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS.hxx>
#include <BRep_Tool.hxx>
#include <AIS_TextLabel.hxx>
#include <AIS_Shape.hxx>
#include <Bnd_Box.hxx>
#include <BRepBndLib.hxx>
#include <AIS_DisplayMode.hxx>
#include <Prs3d_Drawer.hxx>
#include <Prs3d_ShadingAspect.hxx>
#include <Prs3d_LineAspect.hxx>
#include <Prs3d_PointAspect.hxx>
#include <AIS_InteractiveContext.hxx>
#include <GProp_GProps.hxx>
#include <BRepGProp.hxx>

// Data
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsVirtualTopo.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsVirtualTopoMapper.h"
#include "FITK_Component/FITKGeoCompOCC/FITKOCCVirtualTopoCreator.h"

namespace Exchange
{
    FITKOCCGraphObjectTopoIndexLabels::FITKOCCGraphObjectTopoIndexLabels(Interface::FITKAbsVirtualTopo* vTopoData)
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

    FITKOCCGraphObjectTopoIndexLabels::~FITKOCCGraphObjectTopoIndexLabels()
    {
        // Don't need to delete pointers here.
        m_aisLabels.clear();
        m_aisShape = nullptr;
    }

    bool FITKOCCGraphObjectTopoIndexLabels::initShapeData()
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
            vTopoData->getShapeType() == Interface::FITKGeoEnum::VTopoShapeType::VSFace ||
            vTopoData->getShapeType() == Interface::FITKGeoEnum::VTopoShapeType::VSSolid ||
            vTopoData->getShapeType() == Interface::FITKGeoEnum::VTopoShapeType::VSAssembly;
        return flag;
    }

    void FITKOCCGraphObjectTopoIndexLabels::update(bool forceUpdate)
    {
        Q_UNUSED(forceUpdate);

        // Update interactive object if necessary.
        updateInteractiveObjs();
    }

    void FITKOCCGraphObjectTopoIndexLabels::generateShape()
    {
        Interface::FITKAbsVirtualTopo* vTopoData = dynamic_cast<Interface::FITKAbsVirtualTopo*>(_dataObj);
        if (!vTopoData)
        {
            return;
        }

        // Get the occ shape.
        OCC::FITKOCCTopoShape* occTShape = vTopoData->getShapeT<OCC::FITKOCCTopoShape>();
        if (!occTShape)
        {
            return;
        }

        TopoDS_Shape occShape = occTShape->getTopoShape();
        if (occShape.IsNull())
        {
            return;
        }

        // Lambda for adding label object.
        //@{
        auto addLabel = [&](QString text, gp_Pnt & pos)
        {
            // Create the text label and handle.
            AIS_TextLabel* textLabel = new AIS_TextLabel;
            Handle(AIS_TextLabel) labelHandle(textLabel);

            // Set the text and properties.
            textLabel->SetText(text.toUtf8().data());
            textLabel->SetPosition(pos);
            textLabel->SetColor(FITKOCCCommons::QColorToQuan(Qt::red));
            // textLabel->SetFontAspect(Font_FA_Bold);
            textLabel->SetHeight(25);
            textLabel->SetZoomable(false);
            textLabel->SetFont("Arial");

            // Add to the object list and label list.
            addInteractiveObj(labelHandle);
            m_aisLabels.push_back(textLabel);
        };
        //@}

        // Get the shape type and the sub-topo type.
        Interface::FITKGeoEnum::VTopoShapeType shapeType = vTopoData->getShapeType();

        // For compound.
        if (shapeType == Interface::FITKGeoEnum::VTopoShapeType::VSAssembly)
        {
            // Get the sub shape count.
            int nSub = vTopoData->getSubTopoCount();
            for (int i = 0; i < nSub; i++)
            {
                // Get and check the occ shape.
                Interface::FITKAbsVirtualTopo* subVTopo = vTopoData->getSubTopo(i);
                if (!subVTopo)
                {
                    continue;
                }

                OCC::FITKOCCTopoShape* subOccTShape = subVTopo->getShapeT<OCC::FITKOCCTopoShape>();
                if (!subOccTShape)
                {
                    continue;
                }

                TopoDS_Shape subOccShape = subOccTShape->getTopoShape();
                if (subOccShape.IsNull())
                {
                    continue;
                }

                // Get the shape center.
                Bnd_Box bnd;
                BRepBndLib::Add(subOccShape, bnd, false);

                gp_Pnt midPt = (bnd.CornerMax().XYZ() + bnd.CornerMin().XYZ()) / 2.;

                // Add label.
                addLabel(QString::number(i), midPt);
            }
        }
        // For other simple types of shape.
        else
        {
            Interface::FITKGeoEnum::VTopoShapeType subShapeType;
            switch (shapeType)
            {
            case Interface::FITKGeoEnum::VTopoShapeType::VSEdge:
                subShapeType = Interface::FITKGeoEnum::VTopoShapeType::VSPoint;
                break;
            case Interface::FITKGeoEnum::VTopoShapeType::VSFace:
                subShapeType = Interface::FITKGeoEnum::VTopoShapeType::VSEdge;
                break;
            case Interface::FITKGeoEnum::VTopoShapeType::VSSolid:
                subShapeType = Interface::FITKGeoEnum::VTopoShapeType::VSFace;
                break;
            default:
                return;
            }

            if (shapeType == Interface::FITKGeoEnum::VTopoShapeType::VSEdge)
            {
                // Get the occ topo shape.
                TopoDS_Edge edgeShape = TopoDS::Edge(occShape);

                // Get the mid point of the edge.
                double first = 0., last = 0.;
                Handle(Geom_Curve) curve = BRep_Tool::Curve(edgeShape, first, last);
                if (curve.IsNull())
                {
                    return;
                }

                // Get the start and end point.
                gp_Pnt startPt = curve->Value(first);
                gp_Pnt endPt = curve->Value(last);

                // Add labels.
                addLabel("Start", startPt);
                addLabel("End", endPt);
            }
            else
            {
                // Get the sub-shape of the main shape.
                Interface::FITKVirtualTopoMapper vMapper;
                vMapper.mapTopo(vTopoData, subShapeType);
                int nRet = vMapper.length();
                for (int i = 0; i < nRet; i++)
                {
                    Interface::FITKAbsVirtualTopo* subVTopo = vMapper.virtualTopo(i);
                    if (!subVTopo)
                    {
                        continue;
                    }

                    OCC::FITKOCCTopoShape* subOccTShape = subVTopo->getShapeT<OCC::FITKOCCTopoShape>();
                    if (!subOccTShape)
                    {
                        continue;
                    }

                    TopoDS_Shape subOccShape = subOccTShape->getTopoShape();
                    if (subOccShape.IsNull())
                    {
                        continue;
                    }

                    //// Get shape properties.
                    //GProp_GProps props;
                    //if (subOccShape.ShapeType() == TopAbs_ShapeEnum::TopAbs_FACE)
                    //{
                    //    BRepGProp::SurfaceProperties(subOccShape, props);
                    //}
                    //else if (subOccShape.ShapeType() == TopAbs_ShapeEnum::TopAbs_SOLID)
                    //{
                    //    BRepGProp::VolumeProperties(subOccShape, props);
                    //}
                    //
                    //gp_Pnt midPt = props.CentreOfMass();

                    // Get the shape center.
                    Bnd_Box bnd;
                    BRepBndLib::Add(subOccShape, bnd, false);

                    gp_Pnt midPt = (bnd.CornerMax().XYZ() + bnd.CornerMin().XYZ()) / 2.;

                    // Add label.
                    addLabel(QString::number(i), midPt);
                }
            }
        }

        // Create the full shape.
        //@{
        m_aisShape = new AIS_Shape(occShape);
        Handle(AIS_Shape) shapeHandle(m_aisShape);

        // Initialize properties.
        //@{
        m_aisShape->SetDisplayMode(AIS_DisplayMode::AIS_WireFrame);

        Quantity_Color edgeColor = FITKOCCCommons::QColorToQuan(Qt::yellow);
        double lineWidth = FITKOCCCommons::s_modelLineWidth;

        // Initialize line width and default color.
        m_aisShape->SetWidth(lineWidth);
        m_aisShape->SetColor(edgeColor);
        //@}

        addInteractiveObj(shapeHandle);
        //@}
    }
}   // namespace Exchange