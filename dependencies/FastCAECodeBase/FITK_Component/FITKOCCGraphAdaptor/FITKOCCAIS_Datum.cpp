/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCAIS_Datum.h"

// OCC
#include <AIS_InteractiveContext.hxx>
#include <Prs3d_Drawer.hxx>
#include <Prs3d_PlaneAspect.hxx>
#include <Prs3d_LineAspect.hxx>
#include <Prs3d_PointAspect.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>

// Graph
#include "FITKOCCCommons.h"

// Datum plane.
//@{
FITKOCCAIS_Plane::FITKOCCAIS_Plane(const Handle(Geom_Plane)& aComponent, const Standard_Boolean aCurrentMode) :
    AIS_Plane(aComponent, aCurrentMode)
{
    // Initialize properties.
    initProperties();
}

FITKOCCAIS_Plane::FITKOCCAIS_Plane(const Handle(Geom_Plane)& aComponent, const gp_Pnt& aCenter, const Standard_Boolean aCurrentMode) :
    AIS_Plane(aComponent, aCenter, aCurrentMode)
{
    // Initialize properties.
    initProperties();
}

FITKOCCAIS_Plane::FITKOCCAIS_Plane(const Handle(Geom_Plane)& aComponent, const gp_Pnt& aCenter,
    const gp_Pnt& aPmin, const gp_Pnt& aPmax, const Standard_Boolean aCurrentMode) :
    AIS_Plane(aComponent, aCenter, aPmin, aPmax, aCurrentMode)
{
    // Initialize properties.
    initProperties();
}

FITKOCCAIS_Plane::FITKOCCAIS_Plane(const Handle(Geom_Axis2Placement)& aComponent,
    const AIS_TypeOfPlane aPlaneType, const Standard_Boolean aCurrentMode) :
    AIS_Plane(aComponent, aPlaneType, aCurrentMode)
{
    // Initialize properties.
    initProperties();
}

Standard_Boolean FITKOCCAIS_Plane::AcceptDisplayMode(const Standard_Integer aMode) const
{
    // Override the view mode.
    return aMode == 0 || aMode == 1;
}

void FITKOCCAIS_Plane::initProperties()
{
    // Get the drawer. 
    Handle(Prs3d_Drawer) properties = this->Attributes();
  
    // Line and wire properties.
    double lineWidth = Exchange::FITKOCCCommons::s_datumLineWidth;
    Quantity_Color quanColor = Exchange::FITKOCCCommons::QColorToQuan(Exchange::FITKOCCCommons::s_datumColor);

    properties->PlaneAspect()->EdgesAspect()->SetWidth(lineWidth);

    properties->LineAspect()->SetWidth(lineWidth);
    properties->LineAspect()->SetTypeOfLine(Aspect_TypeOfLine::Aspect_TOL_SOLID);

    properties->WireAspect()->SetWidth(lineWidth);
    properties->WireAspect()->SetTypeOfLine(Aspect_TypeOfLine::Aspect_TOL_SOLID);
    properties->SetWireDraw(true);

    properties->SetupOwnFaceBoundaryAspect();
    properties->FaceBoundaryAspect()->SetWidth(lineWidth);
    properties->FaceBoundaryAspect()->SetTypeOfLine(Aspect_TypeOfLine::Aspect_TOL_SOLID);
    properties->SetFaceBoundaryDraw(true);

    // Initialize properties.
    this->SetDisplayMode(AIS_DisplayMode::AIS_WireFrame);
    this->SetColor(quanColor);
}
//@}

// Custom datum plane.
//@{
FITKOCCAIS_ShapePlane::FITKOCCAIS_ShapePlane(const Handle(Geom_Plane)& aComponent) 
    : AIS_ColoredShape(TopoDS_Shape()), m_plane(aComponent)
{
    // Generate shape plane.
    generateShape();

    // Initialize properties.
    initProperties();
}

FITKOCCAIS_ShapePlane::FITKOCCAIS_ShapePlane(TopoDS_Shape & shape)
    : AIS_ColoredShape(shape)
{
    // Initialize properties.
    initProperties();

    this->SetToUpdate(true);
}

void FITKOCCAIS_ShapePlane::initProperties()
{
    // Get the drawer. 
    Handle(Prs3d_Drawer) properties = this->Attributes();

    // Line and wire properties.
    double lineWidth = Exchange::FITKOCCCommons::s_datumLineWidth;
    Quantity_Color quanColor = Exchange::FITKOCCCommons::QColorToQuan(Exchange::FITKOCCCommons::s_datumColor);

    properties->PlaneAspect()->EdgesAspect()->SetWidth(lineWidth);

    properties->LineAspect()->SetWidth(lineWidth);
    properties->LineAspect()->SetTypeOfLine(Aspect_TypeOfLine::Aspect_TOL_SOLID);

    properties->WireAspect()->SetWidth(lineWidth);
    properties->WireAspect()->SetTypeOfLine(Aspect_TypeOfLine::Aspect_TOL_SOLID);
    properties->SetWireDraw(true);

    properties->SetupOwnFaceBoundaryAspect();
    properties->FaceBoundaryAspect()->SetWidth(lineWidth);
    properties->FaceBoundaryAspect()->SetTypeOfLine(Aspect_TypeOfLine::Aspect_TOL_SOLID);
    properties->SetFaceBoundaryDraw(true);

    // Initialize properties.
    this->SetDisplayMode(AIS_DisplayMode::AIS_WireFrame);
    this->SetColor(quanColor);
}

void FITKOCCAIS_ShapePlane::update()
{
    if (!m_plane)
    {
        return;
    }

    // Re-generate shape.
    generateShape();

    this->SetToUpdate();
}

void FITKOCCAIS_ShapePlane::generateShape()
{
    if (!m_plane)
    {
        return;
    }

    // Try to make the face shape.
    try
    {
        double size = 30.;
        const TopoDS_Shape & face = BRepBuilderAPI_MakeFace(m_plane->Pln(), -size, size, -size, size);
        if (face.IsNull())
        {
            return;
        }

        // Set the plane to the AIS.
        this->SetShape(face);
        this->SetToUpdate(true);
    }
    catch (...)
    {
        return;
    }
}

AIS_KindOfInteractive FITKOCCAIS_ShapePlane::Type() const
{
    return AIS_KindOfInteractive::AIS_KOI_Datum;
}

void FITKOCCAIS_ShapePlane::SetPln(const gp_Pln & Pl)
{
    if (!m_plane)
    {
        m_plane = new Geom_Plane(gp_Pln());
    }

    // Update the plane.
    m_plane->SetPln(Pl);

    // Re-generate the shape.
    generateShape();
}

const Handle(Geom_Plane)& FITKOCCAIS_ShapePlane::Component()
{
    return m_plane;
}
//@}

// Datum line.
//@{
FITKOCCAIS_Line::FITKOCCAIS_Line(const Handle(Geom_Line)& aLine) : AIS_Line(aLine)
{
    // Initialize properties.
    initProperties();
}

FITKOCCAIS_Line::FITKOCCAIS_Line(const Handle(Geom_Point)& aStartPoint, const Handle(Geom_Point)& aEndPoint) : 
    AIS_Line(aStartPoint, aEndPoint)
{
    // Initialize properties.
    initProperties();
}

void FITKOCCAIS_Line::initProperties()
{
    // Get the drawer. 
    Handle(Prs3d_Drawer) properties = this->Attributes();

    // Line and wire properties.
    double lineWidth = Exchange::FITKOCCCommons::s_datumLineWidth;
    Quantity_Color quanColor = Exchange::FITKOCCCommons::QColorToQuan(Exchange::FITKOCCCommons::s_datumColor);

    properties->LineAspect()->SetWidth(lineWidth);
    properties->LineAspect()->SetTypeOfLine(Aspect_TypeOfLine::Aspect_TOL_SOLID);

    properties->WireAspect()->SetWidth(lineWidth);
    properties->WireAspect()->SetTypeOfLine(Aspect_TypeOfLine::Aspect_TOL_SOLID);
    properties->SetWireDraw(true);

    // Initialize properties.
    this->SetDisplayMode(AIS_DisplayMode::AIS_WireFrame);
    this->SetColor(quanColor);
}
//@}

// Custom datum line.
//@{
FITKOCCAIS_ShapeLine::FITKOCCAIS_ShapeLine(const Handle(Geom_Point)& aStartPoint, const Handle(Geom_Point)& aEndPoint)
    : AIS_ColoredShape(TopoDS_Shape()), m_start(aStartPoint), m_end(aEndPoint)
{
    // Generate shape line.
    generateShape();

    // Initialize properties.
    initProperties();
}

FITKOCCAIS_ShapeLine::FITKOCCAIS_ShapeLine(TopoDS_Shape & shape)
    : AIS_ColoredShape(shape)
{
    // Initialize properties.
    initProperties();

    this->SetToUpdate(true);
}

AIS_KindOfInteractive FITKOCCAIS_ShapeLine::Type() const
{
    return AIS_KindOfInteractive::AIS_KOI_Datum;
}

void FITKOCCAIS_ShapeLine::Points(Handle(Geom_Point)& thePStart, Handle(Geom_Point)& thePEnd) const
{
    thePStart = m_start;
    thePEnd = m_end;
}

void FITKOCCAIS_ShapeLine::update()
{
    // Re-generate shape.
    generateShape();

    this->SetToUpdate();
}

void FITKOCCAIS_ShapeLine::initProperties()
{
    // Get the drawer. 
    Handle(Prs3d_Drawer) properties = this->Attributes();

    // Line and wire properties.
    double lineWidth = Exchange::FITKOCCCommons::s_datumLineWidth;
    Quantity_Color quanColor = Exchange::FITKOCCCommons::QColorToQuan(Exchange::FITKOCCCommons::s_datumColor);

    // Initialize properties.
    this->SetDisplayMode(AIS_DisplayMode::AIS_Shaded);
    this->SetColor(quanColor);

    properties->SetOwnLineAspects();
    properties->LineAspect()->SetWidth(lineWidth);
    properties->LineAspect()->SetTypeOfLine(Aspect_TypeOfLine::Aspect_TOL_SOLID);
    properties->LineAspect()->SetColor(quanColor);

    properties->WireAspect()->SetWidth(lineWidth);
    properties->WireAspect()->SetTypeOfLine(Aspect_TypeOfLine::Aspect_TOL_SOLID);
    properties->WireAspect()->SetColor(quanColor);
    properties->SetWireDraw(true);
}

void FITKOCCAIS_ShapeLine::generateShape()
{
    if (!m_start || !m_end)
    {
        return;
    }

    // Try to make the line shape.
    try
    {
        gp_Pnt start = m_start->Pnt();
        gp_Pnt end = m_end->Pnt();
        const TopoDS_Shape & edge = BRepBuilderAPI_MakeEdge(start, end);
        if (edge.IsNull())
        {
            return;
        }

        // Set the line to the AIS.
        this->SetShape(edge);
        this->SetToUpdate(true);
    }
    catch (...)
    {
        return;
    }
}
//@}

// Datum point.
//@{
FITKOCCAIS_Point::FITKOCCAIS_Point(const Handle(Geom_Point)& aComponent) : AIS_Point(aComponent)
{
    // Initialize properties.
    initProperties();
}

void FITKOCCAIS_Point::initProperties()
{
    // Get the drawer. 
    Handle(Prs3d_Drawer) properties = this->Attributes();

    // Point mark.
    this->SetMarker(Aspect_TOM_PLUS);

    // Initialize properties.
    this->SetDisplayMode(AIS_DisplayMode::AIS_WireFrame);

    Quantity_Color quanColor = Exchange::FITKOCCCommons::QColorToQuan(Exchange::FITKOCCCommons::s_datumColor);
    this->SetColor(quanColor);
}
//@}

// Custom datum point.
//@{
FITKOCCAIS_ShapePoint::FITKOCCAIS_ShapePoint(const Handle(Geom_Point)& aComponent)
    : AIS_ColoredShape(TopoDS_Shape()), m_point(aComponent)
{
    // Generate shape point.
    generateShape();

    // Initialize properties.
    initProperties();
}

FITKOCCAIS_ShapePoint::FITKOCCAIS_ShapePoint(TopoDS_Shape & shape)
    : AIS_ColoredShape(shape)
{
    // Initialize properties.
    initProperties();

    this->SetToUpdate(true);
}

AIS_KindOfInteractive FITKOCCAIS_ShapePoint::Type() const
{
    return AIS_KindOfInteractive::AIS_KOI_Datum;
}

Handle(Geom_Point) FITKOCCAIS_ShapePoint::Component()
{
    return m_point;
}

void FITKOCCAIS_ShapePoint::update()
{
    // Re-generate shape.
    generateShape();

    this->SetToUpdate();
}

void FITKOCCAIS_ShapePoint::initProperties()
{
    // Get the drawer. 
    Handle(Prs3d_Drawer) properties = this->Attributes();

    // Point mark.
    properties->SetupOwnPointAspect();
    properties->SetVertexDrawMode(Prs3d_VertexDrawMode::Prs3d_VDM_All);
    properties->PointAspect()->SetTypeOfMarker(Aspect_TOM_PLUS);
    properties->PointAspect()->SetScale(3.);

    // Initialize properties.
    this->SetDisplayMode(AIS_DisplayMode::AIS_WireFrame);

    Quantity_Color quanColor = Exchange::FITKOCCCommons::QColorToQuan(Exchange::FITKOCCCommons::s_datumColor);
    this->SetColor(quanColor);
}

void FITKOCCAIS_ShapePoint::generateShape()
{
    if (!m_point)
    {
        return;
    }

    // Try to make the point shape.
    try
    {
        const TopoDS_Shape & vert = BRepBuilderAPI_MakeVertex(m_point->Pnt());
        if (vert.IsNull())
        {
            return;
        }

        // Set the vertex to the AIS.
        this->SetShape(vert);
        this->SetToUpdate(true);
    }
    catch (...)
    {
        return;
    }
}
//@}
