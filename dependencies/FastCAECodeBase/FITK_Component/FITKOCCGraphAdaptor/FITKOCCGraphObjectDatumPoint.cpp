/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCGraphObjectDatumPoint.h"

// OCC
#include <Geom_CartesianPoint.hxx>
#include <Prs3d_Drawer.hxx>
#include <Prs3d_ShadingAspect.hxx>
#include <Prs3d_LineAspect.hxx>
#include <Prs3d_PointAspect.hxx>
#include <AIS_InteractiveContext.hxx>

// Graph
#include "FITKOCCAIS_Datum.h"

// Data
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoDatum.h"

namespace Exchange
{
    FITKOCCGraphObjectDatumPoint::FITKOCCGraphObjectDatumPoint(Interface::FITKAbsGeoDatumPoint* datumData)
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
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeVertex] = true;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeEdge] = false;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeWire] = false;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeFace] = false;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeShell] = false;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeSolid] = false;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_ShapeCompound] = false;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_MeshNode] = false;
        m_supportedSelectModeHash[CommonOCC::SelectMode::S_MeshElement] = false;
        m_graphInfo.Type = FITKOCCCommons::DatumPointGraph;
        //@}

        // Set the render layer to TopMost.
        // Need to change the selection layer to TopMost, too.
        //( Or the selection may be covered by the interactive object. )
        enableTopMost(true);

        // Save the fixed size flag.
        m_hasFixedSize = false;
    }

    FITKOCCGraphObjectDatumPoint::~FITKOCCGraphObjectDatumPoint()
    {
        // Don't need to delete pointers here.
        m_aisObj = nullptr;
    }

    void FITKOCCGraphObjectDatumPoint::update(bool forceUpdate)
    {
        // This grapg object need to be forced updated every time.
        Q_UNUSED(forceUpdate);

        Interface::FITKAbsGeoDatum* datumData = dynamic_cast<Interface::FITKAbsGeoDatum*>(_dataObj);
        if (!datumData)
        {
            return;
        }

        FITKOCCAIS_ShapePoint* aisPoint = dynamic_cast<FITKOCCAIS_ShapePoint*>(m_aisObj);
        Interface::FITKAbsGeoDatumPoint* dpt = dynamic_cast<Interface::FITKAbsGeoDatumPoint*>(datumData);
        if (!dpt || !aisPoint)
        {
            return;
        }

        Handle(Geom_CartesianPoint) pt = Handle(Geom_CartesianPoint)::DownCast(aisPoint->Component());
        if (!pt)
        {
            return;
        }

        // Get the point parameters.
        double pos[3];
        dpt->getPosition(pos);

        // Replace the point.
        pt->SetCoord(pos[0], pos[1], pos[2]);

        aisPoint->update();

        // Update interactive object if necessary.
        updateInteractiveObjs();

        // Update base.
        FITKOCCGraphObject3D::update(forceUpdate);
    }

    void FITKOCCGraphObjectDatumPoint::updateInteractiveObjs()
    {
        if (!m_aisObj)
        {
            m_aisObj->SetToUpdate(true);
        }
    }

    void FITKOCCGraphObjectDatumPoint::setColor(QColor color)
    {
        if (!m_aisObj || !color.isValid())
        {
            return;
        }

        m_aisObj->SetColor(FITKOCCCommons::QColorToQuan(color));
    }

    void FITKOCCGraphObjectDatumPoint::setEnableTransparency(bool isOn)
    {
        if (!m_aisObj)
        {
            return;
        }

        // Default 0.8.
        m_aisObj->SetTransparency(isOn ? 0.8 : 0.);
    }

    void FITKOCCGraphObjectDatumPoint::setLineWidth(double size)
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

    void FITKOCCGraphObjectDatumPoint::highlight(bool update)
    {
        // Save the highlight flag.
        FITKOCCGraphObject3D::highlight(update);
    }

    void FITKOCCGraphObjectDatumPoint::disHighlight(bool update)
    {
        // Save the highlight flag.
        FITKOCCGraphObject3D::disHighlight(update);
    }

    void FITKOCCGraphObjectDatumPoint::generateShape()
    {
        Interface::FITKAbsGeoDatum* datumData = dynamic_cast<Interface::FITKAbsGeoDatum*>(_dataObj);
        if (!datumData)
        {
            return;
        }

        Interface::FITKAbsGeoDatumPoint* dpt = dynamic_cast<Interface::FITKAbsGeoDatumPoint*>(datumData);
        if (!dpt)
        {
            return;
        }

        // Get the point parameters.
        double pos[3];
        dpt->getPosition(pos);

        // Create the point and interactive object.
        gp_Pnt center(pos[0], pos[1], pos[2]);
        Handle(Geom_CartesianPoint) pt = new Geom_CartesianPoint(center);
        FITKOCCAIS_ShapePoint* aisPoint = new FITKOCCAIS_ShapePoint(pt);
        m_aisObj = aisPoint;

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