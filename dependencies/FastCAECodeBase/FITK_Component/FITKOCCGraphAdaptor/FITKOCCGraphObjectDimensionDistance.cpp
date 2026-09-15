/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCGraphObjectDimensionDistance.h"

// OCC
#include <Standard_Handle.hxx>
#include <AIS_InteractiveContext.hxx>
#include <AIS_ColoredShape.hxx>
#include <BRep_Builder.hxx>
#include <AIS_LengthDimension.hxx>
#include <Standard_Version.hxx>

#if OCC_VERSION_HEX >= 0x070600
#include <PrsDim_LengthDimension.hxx>
#endif

// Data
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsVirtualTopo.h"
#include "FITK_Component/FITKGeoCompOCC/FITKOCCVirtualTopoCreator.h"

namespace Exchange
{
    FITKOCCGraphObjectDimensionDistance::FITKOCCGraphObjectDimensionDistance()
    {
        // Generate shape.
        generateShape();

        // Set the render layer to TopMost.
        enableTopMost(true);
    }

    FITKOCCGraphObjectDimensionDistance::~FITKOCCGraphObjectDimensionDistance()
    {
        // Clear the pointer.
        m_aisTool = nullptr;
    }

    void FITKOCCGraphObjectDimensionDistance::generateShape()
    {
        // Create the ruler.
#if OCC_VERSION_HEX < 0x070600
        m_aisTool = new AIS_LengthDimension(gp_Pnt(), gp_Pnt(), gp_Pln());
#else
        m_aisTool = new PrsDim_LengthDimension(gp_Pnt(), gp_Pnt(), gp_Pln());
#endif

        // Initialize properties.
        Handle(Prs3d_DimensionAspect) dimAspect = new Prs3d_DimensionAspect;
        dimAspect->MakeArrows3d(false);
        dimAspect->MakeText3d(false);
        dimAspect->MakeTextShaded(true);
        dimAspect->MakeUnitsDisplayed(false);
        dimAspect->SetCommonColor(Quantity_NameOfColor::Quantity_NOC_RED);
        dimAspect->TextAspect()->SetHeight(m_defaultFontHeight);
        dimAspect->LineAspect()->SetWidth(FITKOCCCommons::s_toolLineWidth);

        // Text and arrow attibutes.
        dimAspect->SetTextHorizontalPosition(Prs3d_DimensionTextHorizontalPosition::Prs3d_DTHP_Center);
        dimAspect->SetTextVerticalPosition(Prs3d_DimensionTextVerticalPosition::Prs3d_DTVP_Center);
        dimAspect->SetArrowOrientation(Prs3d_DimensionArrowOrientation::Prs3d_DAO_Internal);

        // Reset the aspect.
        m_aisTool->SetDimensionAspect(dimAspect);

        // Add to object list.
#if OCC_VERSION_HEX < 0x070600
        Handle(AIS_LengthDimension) aisHandle(m_aisTool);
#else
        Handle(PrsDim_LengthDimension) aisHandle(m_aisTool);
#endif
        
        addInteractiveObj(aisHandle);
    }

    bool FITKOCCGraphObjectDimensionDistance::updateMeasureGeometrys()
    {
        // Get the input virtual topo ids.
        QList<int> ids = getInputItems();

        // Check the interactive object and the ids.
        if (!m_aisTool || !m_aisShape || (ids.count() != 1 && ids.count() != 2))
        {
            return false;
        }

        // One curve.
        if (ids.count() == 1)
        {
            // Get the virtual topo type and shape.
            Interface::FITKAbsVirtualTopo* vTopo = getVirtualTopoById(ids.first());
            if (!vTopo)
            {
                return false;
            }

            TopoDS_Shape shape = getTopoShapeByVirtualTopo(vTopo);
            TopoDS_Edge edge = TopoDS::Edge(shape);

            // Check the edge data.
            if (edge.IsNull())
            {
                return false;
            }

            // Update the shapes.
            m_aisTool->SetMeasuredGeometry(edge, gp_Pln());
            m_aisShape->SetShape(shape);
        }
        // Two shapes.
        else
        {
            // Get the virtual topo types and shapes.
            Interface::FITKAbsVirtualTopo* vTopo_1 = getVirtualTopoById(ids.first());
            Interface::FITKAbsVirtualTopo* vTopo_2 = getVirtualTopoById(ids.last());
            if (!vTopo_1 || !vTopo_2)
            {
                return false;
            }

            TopoDS_Shape shape_1 = getTopoShapeByVirtualTopo(vTopo_1);
            TopoDS_Shape shape_2 = getTopoShapeByVirtualTopo(vTopo_2);

            // Check the shape data.
            if (shape_1.IsNull() || shape_2.IsNull())
            {
                return false;
            }

            // Update the shapes.
            m_aisTool->SetMeasuredShapes(shape_1, shape_2);

            // Make compound for previewing.
            BRep_Builder builder;
            TopoDS_Compound compound;
            builder.MakeCompound(compound);
            builder.Add(compound, shape_1);
            builder.Add(compound, shape_2);
            m_aisShape->SetShape(compound);
        }

        // Save the value.
        setOutputVariantData(m_aisTool->GetValue());

        return true;
    }

    void FITKOCCGraphObjectDimensionDistance::rescale()
    {
        if (!m_aisTool)
        {
            return;
        }

        // Set the distance between the tool and the measured shapes.
        double dist = m_aisTool->GetValue();
        double scaledDist = dist / 10.;
        m_aisTool->SetFlyout(scaledDist);

        // Set the arrow size.
        Handle(Prs3d_DimensionAspect) dimAspect = m_aisTool->DimensionAspect();
        dimAspect->ArrowAspect()->SetLength(scaledDist * 0.5);
    }
}   // namespace Exchange