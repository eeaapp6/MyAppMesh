/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCGraphObjectPickProjPos.h"

// OCC
#include <Standard_Handle.hxx>
#include <AIS_InteractiveContext.hxx>
#include <V3d_View.hxx>
#include <TopoDS_Shape.hxx>
#include <AIS_ColoredShape.hxx>
#include <Prs3d_PointAspect.hxx>
#include <TopoDS.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>

// Graph
#include "FITKOCCCommons.h"

// Global data
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"

// Data
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoDatum.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsVirtualTopo.h"
#include "FITK_Component/FITKGeoCompOCC/FITKOCCVirtualTopoCreator.h"
#include "FITK_Component/FITKGeoCompOCC/FITKAbstractOCCModel.h"

namespace Exchange
{
    FITKOCCGraphObjectPickProjPos::FITKOCCGraphObjectPickProjPos()
    {
        // Generate the empty shape first.
        generateShape();

        // Save the data type.
        m_graphInfo.Type = FITKOCCCommons::PickToolEdgePos;

        // Set the render layer to TopMost.
        enableTopMost(true);

        // Disable highlight.
        disHighlight();

        // Save the default output role.
        m_defaultOutputRole = ROLE_OUTPUT_PICK_PROJ_POSITION;
    }

    FITKOCCGraphObjectPickProjPos::~FITKOCCGraphObjectPickProjPos()
    {
        // Clear the pointer.
        m_aisPreviewShape = nullptr;
    }

    void FITKOCCGraphObjectPickProjPos::clearCache()
    {
        // Clear history shapes.
        if (m_aisPreviewShape)
        {
            m_aisPreviewShape->SetShape(TopoDS_Shape());
            m_aisPreviewShape->SetToUpdate();
        }

        if (m_aisPickShape)
        {
            m_aisPickShape->SetShape(TopoDS_Shape());
            m_aisPickShape->SetToUpdate();
        }

        // Clear input and output.
        m_userInfo.clear();
    }

    void FITKOCCGraphObjectPickProjPos::setColor(QColor color)
    {
        if (!m_aisPickShape || !color.isValid())
        {
            return;
        }

        // OCC color.
        Quantity_Color quanColor = FITKOCCCommons::QColorToQuan(color);

        // Initialize line default color.
        m_aisPickShape->SetColor(quanColor);

        // Get the properties.
        Handle(Prs3d_Drawer) properties = m_aisPickShape->Attributes();

        // Face properties.
        properties->ShadingAspect()->SetColor(quanColor);
    }

    void FITKOCCGraphObjectPickProjPos::updateByMouseMove(int mx, int my)
    {
        if (!m_view || !m_context || !m_aisPreviewShape)
        {
            return;
        }

        // Return if the tool is invisible.
        if (m_context->DisplayStatus(m_aisPickShape) != AIS_DisplayStatus::AIS_DS_Displayed)
        {
            return;
        }

        // The projection target shape.
        TopoDS_Shape shape;

        // Get the projection target.
        bool hasTarget = getTargetShape(shape);

        // Try to get the hovered shape and the target.
        m_context->InitDetected();
        if (!m_context->HasDetectedShape() && !hasTarget)
        {
            return;
        }

        // Use the target shape first.
        if (!hasTarget)
        {
            // Use the hovered shape.
            shape = m_context->DetectedShape();
        }

        // Check the shape and shape type.
        TopAbs_ShapeEnum shapeType = shape.ShapeType();
        if (shape.IsNull() || (shapeType != TopAbs_ShapeEnum::TopAbs_EDGE
            && shapeType != TopAbs_ShapeEnum::TopAbs_FACE))
        {
            return;
        }

        bool flag = false;
        gp_Pnt pt;
        gp_Vec dir;
        gp_Pnt midPt;

        // The edge projection position.
        if (shapeType == TopAbs_ShapeEnum::TopAbs_EDGE)
        {
            // Get the position with the hovered edge and get the extrema.
            flag = FITKOCCCommons::ScreenToWorldWithCurve(mx, my, shape, m_view, pt);
            if (!flag)
            {
                return;
            }

            // Get the mid point of edge.
            flag = FITKOCCCommons::GetEdgeMidPoint(shape, midPt);
        }
        // The face projection position.
        else if (shapeType == TopAbs_ShapeEnum::TopAbs_FACE)
        {
            // Get the position with the hovered edge and get the extrema.
            flag = FITKOCCCommons::ScreenToWorldWithSurf(mx, my, shape, m_view, pt);
            if (!flag)
            {
                return;
            }

            // Get the mid point of face.            
            flag = FITKOCCCommons::GetFaceMidPoint(shape, midPt);
        }

        if (!flag)
        {
            return;
        }

        // Get the screen position with the mid point.
        int sx, sy;
        m_view->Convert(midPt.X(), midPt.Y(), midPt.Z(), sx, sy);

        // Snap to the mid point. Tol: 10px.
        double dist = sqrt(pow(mx - sx, 2) + pow(my - sy, 2));
        if (dist <= 10)
        {
            pt = midPt;

            // Update the shape style.
            setStyleToMidPoint();
        }
        else
        {
            // Update the shape style.
            setStyleToNormalPoint();
        }

        // Get the vertex.
        TopoDS_Shape vert = BRepBuilderAPI_MakeVertex(pt);

        // Update the shape and reset the update flag.
        m_aisPreviewShape->SetShape(vert);
        m_aisPreviewShape->SetToUpdate();

        // Update the interactive objects.
        this->Superclass::update(true);
    }

    void FITKOCCGraphObjectPickProjPos::updateByMousePick(int mx, int my)
    {
        if (!m_view || !m_context || !m_aisPickShape)
        {
            return;
        }

        // Return if the tool is invisible.
        if (m_context->DisplayStatus(m_aisPickShape) != AIS_DisplayStatus::AIS_DS_Displayed)
        {
            return;
        }

        // The projection target shape.
        TopoDS_Shape shape;

        // Get the projection target.
        bool hasTarget = getTargetShape(shape);

        // Try to get the picked shape and the target.
        m_context->InitSelected();
        if (!m_context->HasSelectedShape() && !hasTarget)
        {
            return;
        }

        // Use the target shape first.
        if (!hasTarget)
        {
            // Use the picked shape.
            shape = m_context->SelectedShape();
        }

        // Check the shape and shape type.
        TopAbs_ShapeEnum shapeType = shape.ShapeType();
        if (shape.IsNull() || (shapeType != TopAbs_ShapeEnum::TopAbs_EDGE
            && shapeType != TopAbs_ShapeEnum::TopAbs_FACE))
        {
            return;
        }

        bool flag = false;
        gp_Pnt pt;
        gp_Vec dir;
        gp_Pnt midPt;

        // The edge projection position.
        if (shapeType == TopAbs_ShapeEnum::TopAbs_EDGE)
        {
            // Get the position with the hovered edge and get the extrema.
            flag = FITKOCCCommons::ScreenToWorldWithCurve(mx, my, shape, m_view, pt);
            if (!flag)
            {
                return;
            }
            
            // Get the mid point of edge.
            flag = FITKOCCCommons::GetEdgeMidPoint(shape, midPt);
        }
        // The face projection position.
        else if (shapeType == TopAbs_ShapeEnum::TopAbs_FACE)
        {
            // Get the position with the hovered edge and get the extrema.
            flag = FITKOCCCommons::ScreenToWorldWithSurf(mx, my, shape, m_view, pt);
            if (!flag)
            {
                return;
            }

            // Get the mid point of face.            
            flag = FITKOCCCommons::GetFaceMidPoint(shape, midPt);
        }

        if (!flag)
        {
            return;
        }

        // Get the screen position with the mid point.
        int sx, sy;
        m_view->Convert(midPt.X(), midPt.Y(), midPt.Z(), sx, sy);

        // Snap to the mid point. Tol: 10px.
        double dist = sqrt(pow(mx - sx, 2) + pow(my - sy, 2));
        if (dist <= 10)
        {
            pt = midPt;
        }

        // Get the vertex.
        TopoDS_Shape vert = BRepBuilderAPI_MakeVertex(pt);

        // Update the shape and reset the update flag.
        m_aisPickShape->SetShape(vert);
        m_aisPickShape->SetToUpdate();

        // Save the position.
        QList<double> posList{ pt.X(), pt.Y(), pt.Z() };
        QVariant varPos = QVariant::fromValue<QList<double>>(posList);
        setOutputVariantData(varPos, ROLE_OUTPUT_PICK_PROJ_POSITION);

        // Update the interactive objects.
        this->Superclass::update(true);
    }

    void FITKOCCGraphObjectPickProjPos::generateShape()
    {
        // Create the hovering shape.
        //@{
        m_aisPreviewShape = new AIS_ColoredShape(TopoDS_Shape());

        // Drawer properties.
        //@{
        m_aisPreviewShape->SetDisplayMode(AIS_DisplayMode::AIS_Shaded);

        // Initialize default color.
        Quantity_Color colorForShapePreview = FITKOCCCommons::QColorToQuan(Qt::yellow);
        m_aisPreviewShape->SetColor(colorForShapePreview);

        // Properties.
        //@{
        Handle(Prs3d_Drawer) previewProperties = m_aisPreviewShape->Attributes();

        // Point properties.
        previewProperties->SetupOwnPointAspect();
        previewProperties->SetVertexDrawMode(Prs3d_VertexDrawMode::Prs3d_VDM_All);
        previewProperties->PointAspect()->SetScale(5.);
        previewProperties->PointAspect()->SetTypeOfMarker(Aspect_TypeOfMarker::Aspect_TOM_POINT);
        previewProperties->PointAspect()->SetColor(colorForShapePreview);
        //@}
        //@}

        // Add to the interactive object list.
        Handle(AIS_Shape) previewShapeHandle(m_aisPreviewShape);
        addInteractiveObj(previewShapeHandle);
        //@}

        // Create the previewing shape.
        //@{
        m_aisPickShape = new AIS_ColoredShape(TopoDS_Shape());

        // Drawer properties.
        //@{
        m_aisPickShape->SetDisplayMode(AIS_DisplayMode::AIS_Shaded);

        // Initialize line width and default color.
        Quantity_Color colorForShape = FITKOCCCommons::QColorToQuan(FITKOCCCommons::s_markPreviewShapeColor);
        m_aisPickShape->SetColor(colorForShape);

        // Properties.
        //@{
        Handle(Prs3d_Drawer) pickProperties = m_aisPickShape->Attributes();

        // Point properties.
        pickProperties->SetupOwnPointAspect();
        pickProperties->SetVertexDrawMode(Prs3d_VertexDrawMode::Prs3d_VDM_All);
        pickProperties->PointAspect()->SetScale(5.);
        pickProperties->PointAspect()->SetTypeOfMarker(Aspect_TypeOfMarker::Aspect_TOM_POINT);
        pickProperties->PointAspect()->SetColor(colorForShape);
        //@}
        //@}

        // Add to the interactive object list.
        Handle(AIS_Shape) pickShapeHandle(m_aisPickShape);
        addInteractiveObj(pickShapeHandle);
        //@}
    }

    void FITKOCCGraphObjectPickProjPos::setStyleToMidPoint()
    {
        if (!m_aisPreviewShape)
        {
            return;
        }

        // Initialize color.
        Quantity_Color colorForShapePreview = FITKOCCCommons::QColorToQuan(QColor(255, 255, 200));
        m_aisPreviewShape->SetColor(colorForShapePreview);

        // Properties.
        //@{
        Handle(Prs3d_Drawer) previewProperties = m_aisPreviewShape->Attributes();

        // Point properties.
        previewProperties->SetupOwnPointAspect();
        previewProperties->SetVertexDrawMode(Prs3d_VertexDrawMode::Prs3d_VDM_All);
        previewProperties->PointAspect()->SetScale(8.);
        previewProperties->PointAspect()->SetTypeOfMarker(Aspect_TypeOfMarker::Aspect_TOM_X);
        previewProperties->PointAspect()->SetColor(colorForShapePreview);
        //@}
    }

    void FITKOCCGraphObjectPickProjPos::setStyleToNormalPoint()
    {
        if (!m_aisPreviewShape)
        {
            return;
        }

        // Initialize color.
        Quantity_Color colorForShapePreview = FITKOCCCommons::QColorToQuan(Qt::yellow);
        m_aisPreviewShape->SetColor(colorForShapePreview);

        // Properties.
        //@{
        Handle(Prs3d_Drawer) previewProperties = m_aisPreviewShape->Attributes();

        // Point properties.
        previewProperties->SetupOwnPointAspect();
        previewProperties->SetVertexDrawMode(Prs3d_VertexDrawMode::Prs3d_VDM_All);
        previewProperties->PointAspect()->SetScale(5.);
        previewProperties->PointAspect()->SetTypeOfMarker(Aspect_TypeOfMarker::Aspect_TOM_POINT);
        previewProperties->PointAspect()->SetColor(colorForShapePreview);
        //@}
    }

    bool FITKOCCGraphObjectPickProjPos::getInputTargetIds(int & targetCmdId, int & targetVTopoId)
    {
        int roleCmd = ROLE_INPUT_PICK_PROJ_POSITION_TARGET_CMD_ID;
        int roleTopo = ROLE_INPUT_PICK_PROJ_POSITION_TARGET_TOPO_ID;
        if (!m_userInfo.contains(roleCmd) || !m_userInfo.contains(roleTopo))
        {
            return false;
        }

        targetCmdId = m_userInfo[roleCmd].value<int>();
        targetVTopoId = m_userInfo[roleTopo].value<int>();

        return true;
    }

    bool FITKOCCGraphObjectPickProjPos::getTargetShape(TopoDS_Shape & shape)
    {
        // Check the input role data.
        int targetCmdId = -1, targetTopoId = -1;
        bool hasTarget = getInputTargetIds(targetCmdId, targetTopoId);
        if (!hasTarget)
        {
            return false;
        }

        // Check if the data is a command.
        Interface::FITKAbsGeoCommand* command = FITKDATAREPO->getTDataByID<Interface::FITKAbsGeoCommand>(targetCmdId);
        if (!command)
        {
            return false;
        }

        // Check the command type.
        //@{
        // Make shape by the datum parameters.
        if (command->getGeometryCommandType() == Interface::FITKGeoEnum::FITKGeometryComType::FGTDatum)
        {
            bool isValid = false;

            // Check if the command is a datum plane.
            Interface::FITKAbsGeoDatumPlane* dp = dynamic_cast<Interface::FITKAbsGeoDatumPlane*>(command);
            if (dp && !isValid)
            {
                // Get the plane information.
                double pos3[3], nor3[3], up3[3];
                dp->getPlane(pos3, nor3, up3);

                // Make an infinite plane shape.
                gp_Pln pln(gp_Pnt(pos3[0], pos3[1], pos3[2]), gp_Dir(nor3[0], nor3[1], nor3[2]));
                shape = BRepBuilderAPI_MakeFace(pln);

                isValid = true;
            }

            // Check if the command is a datum line.
            Interface::FITKAbsGeoDatumLine* dl = dynamic_cast<Interface::FITKAbsGeoDatumLine*>(command);
            if (dl && !isValid)
            {
                // The infinite line.
                if (dl->getIsInfinite())
                {
                    // Get the line information.
                    double pos3[3], dir3[3];
                    dl->getInfiniteLine(pos3, dir3);

                    // Make an infinite line.
                    gp_Lin lin(gp_Pnt(pos3[0], pos3[1], pos3[2]), gp_Dir(dir3[0], dir3[1], dir3[2]));
                    shape = BRepBuilderAPI_MakeEdge(lin);
                }
                // The finite line.
                else
                {
                    // Get the line information.
                    double pos3[3], pos2_3[3];
                    dl->getFiniteLine(pos3, pos2_3);

                    // Make a finite line.
                    shape = BRepBuilderAPI_MakeEdge(gp_Pnt(pos3[0], pos3[1], pos3[2]),
                        gp_Pnt(pos2_3[0], pos2_3[1], pos2_3[2]));
                }

                isValid = true;
            }

            if (!isValid)
            {
                return false;
            }
        }
        // Get the virtual topo's shape.
        else
        {
            Interface::FITKAbsVirtualTopo* vTopo = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(targetTopoId);
            if (!vTopo)
            {
                return false;
            }

            // Get the OCC virtual shape.
            OCC::FITKOCCTopoShape* occVTopo = vTopo->getShapeT<OCC::FITKOCCTopoShape>();
            if (!occVTopo)
            {
                return false;
            }

            // Get the shape data.
            shape = occVTopo->getTopoShape();
        }
        //@}

        // Check the shape.
        if (shape.IsNull())
        {
            return false;
        }

        return true;
    }
}   // namespace Exchange