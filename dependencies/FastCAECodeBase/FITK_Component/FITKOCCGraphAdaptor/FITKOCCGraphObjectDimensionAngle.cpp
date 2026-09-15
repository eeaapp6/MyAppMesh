/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCGraphObjectDimensionAngle.h"

// OCC
#include <Standard_Handle.hxx>
#include <TopoDS.hxx>
#include <AIS_InteractiveContext.hxx>
#include <AIS_AngleDimension.hxx>
#include <AIS_ColoredShape.hxx>
#include <Bnd_Box.hxx>
#include <BRepBndLib.hxx>
#include <BRep_Builder.hxx>
#include <Standard_Version.hxx>

#if OCC_VERSION_HEX >= 0x070600
#include <PrsDim_AngleDimension.hxx>
#endif

// Data
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsVirtualTopo.h"

namespace Exchange
{
    FITKOCCGraphObjectDimensionAngle::FITKOCCGraphObjectDimensionAngle()
    {
        // Generate shape.
        generateShape();

        // Set the render layer to TopMost.
        enableTopMost(true);
    }

    FITKOCCGraphObjectDimensionAngle::~FITKOCCGraphObjectDimensionAngle()
    {
        // Clear the pointer.
        m_aisTool = nullptr;
    }

    void FITKOCCGraphObjectDimensionAngle::generateShape()
    {
        // Create the ruler.
#if OCC_VERSION_HEX < 0x070600
        m_aisTool = new AIS_AngleDimension(gp_Pnt(1., 0., 0.), gp_Pnt(0., 0., 0.), gp_Pnt(0., 1., 0.));
#else
        m_aisTool = new PrsDim_AngleDimension(gp_Pnt(1., 0., 0.), gp_Pnt(0., 0., 0.), gp_Pnt(0., 1., 0.));
#endif

        // Initialize properties.
        Handle(Prs3d_DimensionAspect) dimAspect = new Prs3d_DimensionAspect;
        dimAspect->MakeArrows3d(false);
        dimAspect->MakeText3d(false);
        dimAspect->MakeTextShaded(true);
        dimAspect->MakeUnitsDisplayed(false);
        dimAspect->TextAspect()->SetHeight(m_defaultFontHeight);
        dimAspect->SetCommonColor(Quantity_NOC_RED);

        // Text and arrow attibutes.
        dimAspect->SetTextHorizontalPosition(Prs3d_DimensionTextHorizontalPosition::Prs3d_DTHP_Center);
        dimAspect->SetTextVerticalPosition(Prs3d_DimensionTextVerticalPosition::Prs3d_DTVP_Center);
        dimAspect->SetArrowOrientation(Prs3d_DimensionArrowOrientation::Prs3d_DAO_Internal);

        // Reset the aspect.
        m_aisTool->SetDimensionAspect(dimAspect);

        // Add to object list.
#if OCC_VERSION_HEX < 0x070600
        Handle(AIS_AngleDimension) aisHandle(m_aisTool);
#else
        Handle(PrsDim_AngleDimension) aisHandle(m_aisTool);
#endif
        
        addInteractiveObj(aisHandle);
    }

    bool FITKOCCGraphObjectDimensionAngle::updateMeasureGeometrys()
    {
        // Get the input virtual topo ids.
        QList<int> ids = getInputItems();

        // Check the interactive object and the ids.
        if (!m_aisTool || !m_aisShape || ids.count() < 2)
        {
            return false;
        }

        // Get the virtual topo types and shapes.
        QList<TopoDS_Shape> shapes;
        QList<Interface::FITKGeoEnum::VTopoShapeType> types;
        for (const int & vId : ids)
        {
            Interface::FITKAbsVirtualTopo* vTopo = getVirtualTopoById(vId);
            if (!vTopo)
            {
                continue;
            }
     
            TopoDS_Shape shape = getTopoShapeByVirtualTopo(vTopo);
            if (shape.IsNull())
            {
                continue;
            }

            // Save the shape.
            shapes.push_back(shape);

            // Save the type.
            types.push_back(vTopo->getShapeType());
        }

        // Check the data length.
        int nV = shapes.count() == types.count() ? types.count() : 0;
        if (nV == 0)
        {
            return false;
        }

        // Update measure shapes.
        //@{
        // 3: Vert - Vert - Vert
        if (nV == 3)
        {
            if (!types.count(Interface::FITKGeoEnum::VTopoShapeType::VSPoint))
            {
                return false;
            }

            TopoDS_Vertex vert_1 = TopoDS::Vertex(shapes[0]);
            TopoDS_Vertex vert_2 = TopoDS::Vertex(shapes[1]);
            TopoDS_Vertex vert_3 = TopoDS::Vertex(shapes[2]);

            try
            {
                m_aisTool->SetMeasuredGeometry(vert_1, vert_2, vert_3);
            }
            catch (...)
            {
                return false;
            }
        }
        // 2: Face - Face OR Edge - Edge
        else if (nV == 2)
        {
            if (types.count(Interface::FITKGeoEnum::VTopoShapeType::VSEdge) == 2)
            {
                TopoDS_Edge edge_1 = TopoDS::Edge(shapes[0]);
                TopoDS_Edge edge_2 = TopoDS::Edge(shapes[1]);

                // Edge to edge.
                try
                {
                    m_aisTool->SetMeasuredGeometry(edge_1, edge_2);
                }
                catch (...)
                {
                    return false;
                }
            }
            else if (types.count(Interface::FITKGeoEnum::VTopoShapeType::VSFace) == 2)
            {
                TopoDS_Face face_1 = TopoDS::Face(shapes[0]);
                TopoDS_Face face_2 = TopoDS::Face(shapes[1]);

                // Face to face.
                try
                {
                    m_aisTool->SetMeasuredGeometry(face_1, face_2);
                }
                catch (...)
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
        else
        {
            // Not develop yet.
            return false;
        }
        //@}

        // Make compound for previewing.
        //@{
        BRep_Builder builder;
        TopoDS_Compound compound;
        builder.MakeCompound(compound);
        for (const TopoDS_Shape & shape : shapes)
        {
            builder.Add(compound, shape);
        }

        m_aisShape->SetShape(compound);
        //@}

        // Save the value.
        setOutputVariantData(m_aisTool->GetValue());

        return true;
    }

    void FITKOCCGraphObjectDimensionAngle::rescale()
    {
        if (!m_aisTool)
        {
            return;
        }

        // Get the shape size.
        //@{
        const TopoDS_Shape & s1 = m_aisTool->FirstShape();
        const TopoDS_Shape & s2 = m_aisTool->SecondShape();
        const TopoDS_Shape & s3 = m_aisTool->ThirdShape();

        // Get the shapes's bounds.
        Bnd_Box bnd;
        if (!s1.IsNull())
        {
            BRepBndLib::Add(s1, bnd, false);
        }

        if (!s2.IsNull())
        {
            BRepBndLib::Add(s2, bnd, false);
        }

        if (!s3.IsNull())
        {
            BRepBndLib::Add(s3, bnd, false);
        }

        double t, xMax, yMax, zMax;
        bnd.Get(t, t, t, xMax, yMax, zMax);
        //@}

        // Set the distance between the tool and the measured shapes.
        double dist = qMax(qMax(xMax, yMax), zMax);
        double scaledDist = dist / 5.;
        m_aisTool->SetFlyout(scaledDist);

        // Set the arrow size.
        Handle(Prs3d_DimensionAspect) dimAspect = m_aisTool->DimensionAspect();
        dimAspect->ArrowAspect()->SetLength(scaledDist * 0.2);
    }
}   // namespace Exchange