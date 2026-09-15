/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCGraphObjectDatumLine.h"

// OCC
#include <Geom_CartesianPoint.hxx>
#include <AIS_DisplayMode.hxx>
#include <Prs3d_Drawer.hxx>
#include <Prs3d_ShadingAspect.hxx>
#include <Prs3d_LineAspect.hxx>
#include <Prs3d_PointAspect.hxx>
#include <AIS_InteractiveContext.hxx>
#include <Standard_Version.hxx>

// Graph
#include "FITKOCCAIS_Datum.h"

// Data
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoDatum.h"

namespace Exchange
{
    FITKOCCGraphObjectDatumLine::FITKOCCGraphObjectDatumLine(Interface::FITKAbsGeoDatumLine* datumData)
        : FITKOCCGraphObject3D(datumData)
    {
        if (!datumData)
        {
            return;
        }

        // Generate the interactive objects.
        generateShape();

        // Save the data type and initialize select modes.
        //@{
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeShape] = true;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeVertex] = false;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeEdge] = true;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeWire] = false;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeFace] = false;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeShell] = false;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeSolid] = false;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeCompound] = false;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_MeshNode] = false;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_MeshElement] = false;
        m_graphInfo.Type = FITKOCCCommons::DatumLineGraph;
        //@}

        // Set the render layer to TopMost.
        // Need to change the selection layer to TopMost, too.
        //( Or the selection may be covered by the interactive object. )
        enableTopMost(true);

        // Save the fixed size flag.
        m_hasFixedSize = false;
    }

    FITKOCCGraphObjectDatumLine::~FITKOCCGraphObjectDatumLine()
    {
        // Don't need to delete pointers here.
        m_aisObj = nullptr;
    }

    void FITKOCCGraphObjectDatumLine::update(bool forceUpdate)
    {
        // This grapg object need to be forced updated every time.
        Q_UNUSED(forceUpdate);

        Interface::FITKAbsGeoDatum* datumData = dynamic_cast<Interface::FITKAbsGeoDatum*>(_dataObj);
        if (!datumData)
        {
            return;
        }
       
        FITKOCCAIS_ShapeLine* aisLine = dynamic_cast<FITKOCCAIS_ShapeLine*>(m_aisObj);
        Interface::FITKAbsGeoDatumLine* dl = dynamic_cast<Interface::FITKAbsGeoDatumLine*>(datumData);
        if (!dl || !aisLine)
        {
            return;
        }

        // Get the original color.
        Quantity_Color color;
        aisLine->Color(color);

        // Delete the line object, create a new one.
        if (m_context)
        {
            m_context->Remove(aisLine, false);
        }

        m_objs.clear();

        // Infinite line.
        if (dl->getIsInfinite())
        {
            // Get the line parameters.
            double pos[3], dir[3];
            dl->getInfiniteLine(pos, dir);

            gp_Dir gDir(dir[0], dir[1], dir[2]);

            // Replace the points.
            double pixelLen = 240.;
            gp_Pnt lenPt(gDir.X() * pixelLen * 0.5, gDir.Y() * pixelLen * 0.5, gDir.Z() * pixelLen * 0.5);

            // Re-create the ais object.
            Handle(Geom_CartesianPoint) pt1 = new Geom_CartesianPoint(gp_Pnt(-lenPt.X(), -lenPt.Y(), -lenPt.Z()));
            Handle(Geom_CartesianPoint) pt2 = new Geom_CartesianPoint(lenPt);
            aisLine = new FITKOCCAIS_ShapeLine(pt1, pt2);

            // Enable fixed pixel size.
            gp_Pnt center(pos[0], pos[1], pos[2]);

#if OCC_VERSION_HEX < 0x070600
            Handle(Graphic3d_TransformPers) transPer = Graphic3d_TransformPers::FromDeprecatedParams(
                Graphic3d_TransModeFlags::Graphic3d_TMF_ZoomPers, center);
#else
            Handle(Graphic3d_TransformPers) transPer = new Graphic3d_TransformPers(
                Graphic3d_TransModeFlags::Graphic3d_TMF_ZoomPers, center);
#endif

            aisLine->SetTransformPersistence(transPer);
        }
        // Finite segment.
        else
        {
            // Get the line parameters.
            double pos[3], pos2[3];
            dl->getFiniteLine(pos, pos2);

            // Re-create the ais object.
            Handle(Geom_CartesianPoint) pt1 = new Geom_CartesianPoint(gp_Pnt(pos[0], pos[1], pos[2]));
            Handle(Geom_CartesianPoint) pt2 = new Geom_CartesianPoint(gp_Pnt(pos2[0], pos2[1], pos2[2]));
            aisLine = new FITKOCCAIS_ShapeLine(pt1, pt2);
        }

        // Update the color and shape.
        aisLine->SetColor(color);
        aisLine->update();

        m_aisObj = aisLine;

        // Save the graph info.
        FITKAIS_ObjectBase* aisBase = dynamic_cast<FITKAIS_ObjectBase*>(aisLine);
        if (aisBase)
        {
            aisBase->setGraphObject(this);
            aisBase->setDataObjectId(datumData->getDataObjectID());
        }

        // Create handle.    
        if (m_aisObj)
        {
            Handle(AIS_InteractiveObject) datumHandle(m_aisObj);
            addInteractiveObj(datumHandle);
        }

        // Change the render layer.
        enableTopMost(true);

        // Update interactive object if necessary.
        updateInteractiveObjs();

        // Update base.
        FITKOCCGraphObject3D::update(forceUpdate);
    }

    void FITKOCCGraphObjectDatumLine::updateInteractiveObjs()
    {
        if (!m_aisObj)
        {
            m_aisObj->SetToUpdate(true);
        }
    }

    void FITKOCCGraphObjectDatumLine::setColor(QColor color)
    {
        if (!m_aisObj || !color.isValid())
        {
            return;
        }

        m_aisObj->SetColor(FITKOCCCommons::QColorToQuan(color));
    }

    void FITKOCCGraphObjectDatumLine::setEnableTransparency(bool isOn)
    {
        if (!m_aisObj)
        {
            return;
        }

        // Default 0.8.
        m_aisObj->SetTransparency(isOn ? 0.8 : 0.);
    }

    void FITKOCCGraphObjectDatumLine::setLineWidth(double size)
    {
        if (!m_aisObj)
        {
            return;
        }

        // Get the drawer.
        Handle(Prs3d_Drawer) properties = m_aisObj->Attributes();

        // Line and wire properties.
        properties->LineAspect()->SetWidth(size);
        properties->WireAspect()->SetWidth(size);
        properties->FaceBoundaryAspect()->SetWidth(size);
    }

    void FITKOCCGraphObjectDatumLine::highlight(bool update)
    {
        // Save the highlight flag.
        FITKOCCGraphObject3D::highlight(update);
    }

    void FITKOCCGraphObjectDatumLine::disHighlight(bool update)
    {
        // Save the highlight flag.
        FITKOCCGraphObject3D::disHighlight(update);
    }

    void FITKOCCGraphObjectDatumLine::generateShape()
    {
        Interface::FITKAbsGeoDatum* datumData = dynamic_cast<Interface::FITKAbsGeoDatum*>(_dataObj);
        if (!datumData)
        {
            return;
        }

        Interface::FITKAbsGeoDatumLine* dl = dynamic_cast<Interface::FITKAbsGeoDatumLine*>(datumData);
        if (!dl)
        {
            return;
        }

        // Infinite line.
        if (dl->getIsInfinite())
        {
            // Get the line parameters.
            double pos[3], dir[3];
            dl->getInfiniteLine(pos, dir);
            gp_Dir gDir(dir[0], dir[1], dir[2]);

            // Replace the points.
            double pixelLen = 240.;
            gp_Pnt lenPt(gDir.X() * pixelLen * 0.5, gDir.Y() * pixelLen * 0.5, gDir.Z() * pixelLen * 0.5);
            Handle(Geom_CartesianPoint) pt1 = new Geom_CartesianPoint(gp_Pnt(-lenPt.X(), -lenPt.Y(), -lenPt.Z()));
            Handle(Geom_CartesianPoint) pt2 = new Geom_CartesianPoint(lenPt);
            FITKOCCAIS_ShapeLine* aisLine = new FITKOCCAIS_ShapeLine(pt1, pt2);
            m_aisObj = aisLine;

            // Enable fixed pixel size.
            gp_Pnt center(pos[0], pos[1], pos[2]);

#if OCC_VERSION_HEX < 0x070600
            Handle(Graphic3d_TransformPers) transPer = Graphic3d_TransformPers::FromDeprecatedParams(
                Graphic3d_TransModeFlags::Graphic3d_TMF_ZoomPers, center);
#else
            Handle(Graphic3d_TransformPers) transPer = new Graphic3d_TransformPers(
                Graphic3d_TransModeFlags::Graphic3d_TMF_ZoomPers, center);
#endif

            aisLine->SetTransformPersistence(transPer);
        }
        // Finite segment.
        else
        {
            // Get the line parameters.
            double pos[3], pos2[3];
            dl->getFiniteLine(pos, pos2);

            // Create the points and interactive object.
            Handle(Geom_CartesianPoint) pt1 = new Geom_CartesianPoint(gp_Pnt(pos[0], pos[1], pos[2]));
            Handle(Geom_CartesianPoint) pt2 = new Geom_CartesianPoint(gp_Pnt(pos2[0], pos2[1], pos2[2]));
            FITKOCCAIS_ShapeLine* aisLine = new FITKOCCAIS_ShapeLine(pt1, pt2);
            m_aisObj = aisLine;
        }

        // Save the graph info.
        FITKAIS_ObjectBase* aisBase = dynamic_cast<FITKAIS_ObjectBase*>(m_aisObj);
        if (aisBase)
        {
            aisBase->setGraphObject(this);
            aisBase->setDataObjectId(datumData->getDataObjectID());
        }

        // Create handle.    
        if (m_aisObj)
        {
            Handle(AIS_InteractiveObject) datumHandle(m_aisObj);
            addInteractiveObj(datumHandle);
        }
    }
}   // namespace Exchange