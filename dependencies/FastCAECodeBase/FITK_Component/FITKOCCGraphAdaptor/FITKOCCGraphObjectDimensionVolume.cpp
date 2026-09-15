/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCGraphObjectDimensionVolume.h"

// OCC
#include <Standard_Handle.hxx>
#include <TopoDS.hxx>
#include <AIS_InteractiveContext.hxx>
#include <AIS_ColoredShape.hxx>
#include <AIS_TextLabel.hxx>
#include <Prs3d_Drawer.hxx>
#include <Prs3d_ShadingAspect.hxx>
#include <Bnd_Box.hxx>
#include <BRepBndLib.hxx>
#include <GProp_GProps.hxx>
#include <BRepGProp.hxx>

// Data
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsVirtualTopo.h"

namespace Exchange
{
    FITKOCCGraphObjectDimensionVolume::FITKOCCGraphObjectDimensionVolume()
    {
        // Generate shape.
        generateShape();

        // Set the render layer to TopMost.
        enableTopMost(true);
    }

    FITKOCCGraphObjectDimensionVolume::~FITKOCCGraphObjectDimensionVolume()
    {
        // Clear the pointer.
        m_aisLabel = nullptr;
    }

    void FITKOCCGraphObjectDimensionVolume::generateShape()
    {
        // Create the text label for view the volume value.
        //@{
        m_aisLabel = new AIS_TextLabel;

        // Set the text and properties.
        m_aisLabel->SetText("V = 0");
        m_aisLabel->SetColor(FITKOCCCommons::QColorToQuan(Qt::red));
        // m_aisLabel->SetFontAspect(Font_FA_Bold);
        m_aisLabel->SetHeight(m_defaultFontHeight);
        m_aisLabel->SetZoomable(false);
        m_aisLabel->SetFont("Arial");

        // Add to object list.
        Handle(AIS_TextLabel) labelHandle(m_aisLabel);
        addInteractiveObj(labelHandle);
        //@}
    }

    bool FITKOCCGraphObjectDimensionVolume::updateMeasureGeometrys()
    {
        // Get the input virtual topo ids.
        QList<int> ids = getInputItems();

        // Check the interactive object and the ids.
        if (!m_aisShape || !m_aisLabel || ids.count() != 1)
        {
            return false;
        }

        // Get the virtual topo types and shapes.
        Interface::FITKAbsVirtualTopo* vTopo = getVirtualTopoById(ids.first());
        if (!vTopo)
        {
            return false;
        }

        TopoDS_Shape shape = getTopoShapeByVirtualTopo(vTopo);

        // Check the shape data.
        if (shape.IsNull())
        {
            return false;
        }

        // Check the shape type.( Only solid shape can be measured. )
        if (shape.ShapeType() != TopAbs_ShapeEnum::TopAbs_SOLID)
        {
            return false;
        }

        // Get volume properties.
        GProp_GProps props;
        BRepGProp::VolumeProperties(shape, props);
        double vol = props.Mass();

        // Get the shapes's center.
        Bnd_Box bnd;
        if (!shape.IsNull())
        {
            BRepBndLib::Add(shape, bnd, false);
        }

        double xMin, yMin, zMin, xMax, yMax, zMax;
        bnd.Get(xMin, yMin, zMin, xMax, yMax, zMax);

        gp_Pnt center = gp_Pnt((xMin + xMax) / 2, (yMin + yMax) / 2, (zMin + zMax) / 2);
        //@}

        // Copy shape for previewing.
        m_aisShape->SetShape(shape);

        // Set the text and position.
        m_aisLabel->SetText(QString("V = %1").arg(vol).toUtf8().data());
        m_aisLabel->SetPosition(center);

        // Set the update flag.
        m_aisShape->SetToUpdate();
        m_aisLabel->SetToUpdate();

        // Save the value.
        setOutputVariantData(vol);

        return true;
    }

    void FITKOCCGraphObjectDimensionVolume::rescale()
    {
        // Nothing to do here.
    }
}   // namespace Exchange