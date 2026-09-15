/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCGraphObjectDatumPlane.h"

// OCC
#include <gp_Pln.hxx>
#include <Geom_Plane.hxx>
#include <AIS_Plane.hxx>
#include <AIS_PlaneTrihedron.hxx>
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
    FITKOCCGraphObjectDatumPlane::FITKOCCGraphObjectDatumPlane(Interface::FITKAbsGeoDatumPlane* datumData)
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
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeEdge] = false;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeWire] = false;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeFace] = true;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeShell] = false;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeSolid] = false;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeCompound] = false;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_MeshNode] = false;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_MeshElement] = false;
        m_graphInfo.Type = FITKOCCCommons::DatumPlaneGraph;
        //@}

        // Set the render layer to TopMost.
        // Need to change the selection layer to TopMost, too.
        //( Or the selection may be covered by the interactive object. )
        enableTopMost(true);

        // Save the fixed size flag.
        m_hasFixedSize = false;
    }

    FITKOCCGraphObjectDatumPlane::~FITKOCCGraphObjectDatumPlane()
    {
        // Don't need to delete pointers here.
        m_aisObj = nullptr;
    }

    void FITKOCCGraphObjectDatumPlane::setViewMode(FITKOCCCommons::ShapeMeshViewMode type, bool visible)
    {
        Q_UNUSED(visible);

        // Get the datum data.
        Interface::FITKAbsGeoDatum* datumData = dynamic_cast<Interface::FITKAbsGeoDatum*>(_dataObj);
        if (!datumData || !m_aisObj)
        {
            return;
        }

        // Datum plane.
        switch (type)
        {
        case Exchange::FITKOCCCommons::SMVM_Shade:
            m_aisObj->SetDisplayMode(AIS_DisplayMode::AIS_Shaded);
            break;
        case Exchange::FITKOCCCommons::SMVM_Wireframe:
            m_aisObj->SetDisplayMode(AIS_DisplayMode::AIS_WireFrame);
            break;
        default:
            return;
        }
        //@}

        // No use.
        FITKOCCGraphObject3D::setViewMode(type, true);
    }

    void FITKOCCGraphObjectDatumPlane::update(bool forceUpdate)
    {
        // This grapg object need to be forced updated every time.
        Q_UNUSED(forceUpdate);

        Interface::FITKAbsGeoDatum* datumData = dynamic_cast<Interface::FITKAbsGeoDatum*>(_dataObj);
        if (!datumData)
        {
            return;
        }

        FITKOCCAIS_ShapePlane* aisPln = dynamic_cast<FITKOCCAIS_ShapePlane*>(m_aisObj);
        Interface::FITKAbsGeoDatumPlane* dp = dynamic_cast<Interface::FITKAbsGeoDatumPlane*>(datumData);
        if (!dp || !aisPln)
        {
            return;
        }

        // Get the original color.
        Quantity_Color color;
        aisPln->Color(color);

        // Delete the plane object, create a new one.
        if (m_context)
        {
            m_context->Remove(aisPln, false);
        }

        m_objs.clear();

        // Get the plane parameters.
        double pos[3], nor[3], up[3];
        dp->getPosition(pos);
        dp->getNormal(nor);
        dp->getUp(up);

        gp_Pnt zero(0., 0., 0.);
        gp_Pnt center(pos[0], pos[1], pos[2]);

        // Create the plane and interactive object.
        gp_Ax3 ax3(zero,
            gp_Dir(nor[0], nor[1], nor[2]),
            gp_Dir(up[0], up[1], up[2]));
        Handle(Geom_Plane) pln = new Geom_Plane(ax3);

        aisPln = new FITKOCCAIS_ShapePlane(pln);
        m_aisObj = aisPln;

        // Fixed pixel size.
#if OCC_VERSION_HEX < 0x070600
        Handle(Graphic3d_TransformPers) transPer = Graphic3d_TransformPers::FromDeprecatedParams(
            Graphic3d_TransModeFlags::Graphic3d_TMF_ZoomPers, center);
#else
        Handle(Graphic3d_TransformPers) transPer = new Graphic3d_TransformPers(
            Graphic3d_TransModeFlags::Graphic3d_TMF_ZoomPers, center);
#endif

        aisPln->SetTransformPersistence(transPer);

        // Update the color and shape.
        aisPln->SetColor(color);
        aisPln->update();

        m_aisObj = aisPln;

        // Save the graph info.
        FITKAIS_ObjectBase* aisBase = dynamic_cast<FITKAIS_ObjectBase*>(aisPln);
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

    void FITKOCCGraphObjectDatumPlane::updateInteractiveObjs()
    {
        if (!m_aisObj)
        {
            m_aisObj->SetToUpdate(true);
        }
    }

    void FITKOCCGraphObjectDatumPlane::setColor(QColor color)
    {
        if (!m_aisObj || !color.isValid())
        {
            return;
        }

        m_aisObj->SetColor(FITKOCCCommons::QColorToQuan(color));
    }

    void FITKOCCGraphObjectDatumPlane::setEnableTransparency(bool isOn)
    {
        if (!m_aisObj)
        {
            return;
        }

        // Default 0.8.
        m_aisObj->SetTransparency(isOn ? 0.8 : 0.);
    }

    void FITKOCCGraphObjectDatumPlane::setLineWidth(double size)
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

    void FITKOCCGraphObjectDatumPlane::highlight(bool update)
    {
        // Save the highlight flag.
        FITKOCCGraphObject3D::highlight(update);
    }

    void FITKOCCGraphObjectDatumPlane::disHighlight(bool update)
    {
        // Save the highlight flag.
        FITKOCCGraphObject3D::disHighlight(update);
    }

    void FITKOCCGraphObjectDatumPlane::generateShape()
    {
        Interface::FITKAbsGeoDatum* datumData = dynamic_cast<Interface::FITKAbsGeoDatum*>(_dataObj);
        if (!datumData)
        {
            return;
        }

        Interface::FITKAbsGeoDatumPlane* dp = dynamic_cast<Interface::FITKAbsGeoDatumPlane*>(datumData);
        if (!dp)
        {
            return;
        }

        // Get the plane parameters.
        double pos[3], nor[3], up[3];
        dp->getPosition(pos);
        dp->getNormal(nor);
        dp->getUp(up);

        gp_Pnt zero(0., 0., 0.);
        gp_Pnt center(pos[0], pos[1], pos[2]);

        // Create the plane and interactive object.
        gp_Ax3 ax3(zero,
            gp_Dir(nor[0], nor[1], nor[2]),
            gp_Dir(up[0], up[1], up[2]));
        Handle(Geom_Plane) pln = new Geom_Plane(ax3);

        FITKOCCAIS_ShapePlane* aisPln = new FITKOCCAIS_ShapePlane(pln);
        m_aisObj = aisPln;

        // Fixed pixel size.
#if OCC_VERSION_HEX < 0x070600
        Handle(Graphic3d_TransformPers) transPer = Graphic3d_TransformPers::FromDeprecatedParams(
            Graphic3d_TransModeFlags::Graphic3d_TMF_ZoomPers, center);
#else
        Handle(Graphic3d_TransformPers) transPer = new Graphic3d_TransformPers(
            Graphic3d_TransModeFlags::Graphic3d_TMF_ZoomPers, center);
#endif

        aisPln->SetTransformPersistence(transPer);

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