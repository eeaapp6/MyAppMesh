/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCGraphObjectModelPreviewBase.h"

// OCC
#include <TopoDS_Shape.hxx>
#include <AIS_DisplayMode.hxx>
#include <Prs3d_Drawer.hxx>
#include <Prs3d_ShadingAspect.hxx>
#include <Prs3d_LineAspect.hxx>
#include <Prs3d_PointAspect.hxx>
#include <AIS_InteractiveContext.hxx>

// Graph
#include "FITKOCCAIS_ColoredShape.h"

// Data
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"
#include "FITK_Component/FITKGeoCompOCC/FITKAbstractOCCModel.h"

namespace Exchange
{
    FITKOCCGraphObjectModelPreviewBase::FITKOCCGraphObjectModelPreviewBase(Core::FITKAbstractDataObject* dataObj)
        : FITKOCCGraphObject3D(dataObj)
    {
        // Save the data type.
        m_graphInfo.Type = FITKOCCCommons::OtherGraph;

        // Set the render layer to TopMost.
        enableTopMost(true);

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
    }

    FITKOCCGraphObjectModelPreviewBase::~FITKOCCGraphObjectModelPreviewBase()
    {
        // Don't need to delete pointers here.
        m_fAisShape = nullptr;
        m_tpShape = nullptr;
    }

    void FITKOCCGraphObjectModelPreviewBase::updateInteractiveObjs()
    {
        if (m_context && m_fAisShape)
        {
            // Check if the interactive object is visible.
            if (m_context->DisplayStatus(m_fAisShape) == AIS_DisplayStatus::AIS_DS_Displayed)
            {
                m_context->Redisplay(m_fAisShape, true);
            }
        }
    }

    void FITKOCCGraphObjectModelPreviewBase::setColor(QColor color)
    {
        if (!m_fAisShape || !color.isValid())
        {
            return;
        }

        // OCC color.
        Quantity_Color quanColor = FITKOCCCommons::QColorToQuan(color);

        // Initialize line default color.
        m_fAisShape->SetColor(quanColor);

        // Get the properties.
        Handle(Prs3d_Drawer) properties = m_fAisShape->Attributes();

        // Face properties.
        properties->ShadingAspect()->SetColor(quanColor); 

        // Line properties.
        properties->WireAspect()->SetColor(quanColor);
        properties->LineAspect()->SetColor(quanColor);

        // Face-line properties.
        properties->FaceBoundaryAspect()->SetColor(quanColor);

        // Point properties.
        properties->PointAspect()->SetColor(quanColor);
    }

    void FITKOCCGraphObjectModelPreviewBase::setEnableTransparency(bool isOn)
    {
        if (!m_fAisShape)
        {
            return;
        }

        m_fAisShape->SetTransparency(isOn ? FITKOCCCommons::s_transpacency : 0.);
    }

    void FITKOCCGraphObjectModelPreviewBase::setPointMark(FITKOCCCommons::PointMarkShape type)
    {
        if (!m_fAisShape)
        {
            return;
        }

        // Get the properties.
        Handle(Prs3d_Drawer) properties = m_fAisShape->Attributes();

        // Point properties.
        properties->PointAspect()->SetTypeOfMarker(Aspect_TypeOfMarker(type));
    }

    void FITKOCCGraphObjectModelPreviewBase::setPointSizeScale(double factor)
    {
        if (!m_fAisShape || factor <= 0)
        {
            return;
        }

        // Get the properties.
        Handle(Prs3d_Drawer) properties = m_fAisShape->Attributes();

        // Get the point aspect.
        properties->PointAspect()->SetScale(factor);
        m_fAisShape->SetAttributes(properties);
    }

    void FITKOCCGraphObjectModelPreviewBase::setLineWidth(double size)
    {
        if (!m_fAisShape)
        {
            return;
        }

        // Get the properties.
        Handle(Prs3d_Drawer) properties = m_fAisShape->Attributes();

        // Get the line aspect.
        properties->LineAspect()->SetWidth(size);
    }

    void FITKOCCGraphObjectModelPreviewBase::setVisible(bool visibility)
    {
        if (!m_fAisShape || !m_context)
        {
            return;
        }

        if (visibility)
        {
            // Show the object.
            m_context->Display(m_fAisShape, true);
        }
        else
        {
            // Hide the object.
            m_context->Erase(m_fAisShape, true);
        }
    }

    void FITKOCCGraphObjectModelPreviewBase::generateShape()
    {
        if (!m_tpShape)
        {
            return;
        }

        // Create the FITKOCCAIS_ColoredShape.
        m_fAisShape = new FITKOCCAIS_ColoredShape(*m_tpShape);

        // Save the graph object.
        m_fAisShape->setGraphObject(this);

        // Create handle.
        Handle(FITKOCCAIS_ColoredShape) shapeHandle(m_fAisShape);

        // Get the color of previewing.
        Quantity_Color quanColor = FITKOCCCommons::QColorToQuan(FITKOCCCommons::s_previewColor);

        double lineWidth = FITKOCCCommons::s_toolLineWidth;

        // Initialize properties.
        //@{
       // Display mode - AIS_Shaded or AIS_WireFrame
        m_fAisShape->SetDisplayMode(AIS_DisplayMode::AIS_Shaded);

        // Initialize line width and default color and also other properties.
        m_fAisShape->SetWidth(lineWidth);
        m_fAisShape->SetColor(quanColor);
        m_fAisShape->SetTransparency(FITKOCCCommons::s_transpacency);

        // Properties.
        //@{
        Handle(Prs3d_Drawer) properties = m_fAisShape->Attributes();

        // Face properties.
        properties->SetupOwnShadingAspect();
        properties->ShadingAspect()->SetColor(quanColor);

        // Line and wire properties.
        properties->LineAspect()->SetColor(quanColor);
        properties->WireAspect()->SetColor(quanColor);
        properties->LineAspect()->SetWidth(lineWidth);
        properties->WireAspect()->SetWidth(lineWidth);

        // Face-line properties.
        properties->SetupOwnFaceBoundaryAspect();
        properties->SetFaceBoundaryDraw(Standard_True);
        properties->FaceBoundaryAspect()->SetColor(quanColor);
        properties->FaceBoundaryAspect()->SetWidth(lineWidth);

        // Point properties.
        properties->SetupOwnPointAspect();
        properties->SetVertexDrawMode(Prs3d_VertexDrawMode::Prs3d_VDM_All);
        properties->PointAspect()->SetTypeOfMarker(Aspect_TypeOfMarker::Aspect_TOM_POINT);
        properties->PointAspect()->SetColor(quanColor);
        properties->PointAspect()->SetScale(9.);

        m_fAisShape->SetAttributes(properties);
        //@}
        //@}

        // Add to the interactive object list.
        addInteractiveObj(shapeHandle);

        // Save the shape's hash code.
        m_hashCode = m_tpShape->HashCode(INT_MAX);
    }
}   // namespace Exchange