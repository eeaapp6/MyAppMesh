/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCGraphObjectPickDirection.h"

// OCC
#include <Standard_Handle.hxx>
#include <AIS_InteractiveContext.hxx>
#include <V3d_View.hxx>
#include <TopoDS_Shape.hxx>
#include <AIS_ColoredShape.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <TopoDS.hxx>
#include <BRepBndLib.hxx>
#include <Standard_Version.hxx>

// Graph
#include "FITKOCCCommons.h"
#include "FITKAIS_ObjectBase.h"
#include "FITKOCCAIS_ColoredShape.h"

// Global data
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"

// Data
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoSketch2D.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoDatum.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsVirtualTopo.h"
#include "FITK_Component/FITKGeoCompOCC/FITKOCCVirtualTopoCreator.h"
#include "FITK_Component/FITKGeoCompOCC/FITKAbstractOCCModel.h"

namespace Exchange
{
    FITKOCCGraphObjectPickDirection::FITKOCCGraphObjectPickDirection()
    {
        // Generate the empty shape first.
        generateShape();

        // Save the data type.
        m_graphInfo.Type = FITKOCCCommons::PickToolDirection;

        // Set the render layer to TopMost.
        enableTopMost(m_aisArrowShape, true);
        m_aisPickShape->SetZLayer(Graphic3d_ZLayerId_Top);

        // Initialize select modes.
        m_currSelectModes.push_back(CommonOCC::SelectMode::S_ShapeShape);

        // Disable highlight.
        disHighlight();

        // Save the default output role.
        m_defaultOutputRole = ROLE_OUTPUT_PICK_DIRECTION_DIRECTION;
    }

    FITKOCCGraphObjectPickDirection::~FITKOCCGraphObjectPickDirection()
    {
        // Clear the pointer.
        m_aisArrowShape = nullptr;
    }

    void FITKOCCGraphObjectPickDirection::clearCache()
    {
        // Clear history shapes.
        if (m_aisArrowShape)
        {
            m_aisArrowShape->SetShape(TopoDS_Shape());
            m_aisArrowShape->SetToUpdate();
        }

        if (m_aisPickShape)
        {
            m_aisPickShape->SetShape(TopoDS_Shape());
            m_aisPickShape->SetToUpdate();
        }

        // Clear input and output.
        m_userInfo.clear();
    }

    void FITKOCCGraphObjectPickDirection::updateSelectModes()
    {
        if (!m_context || !getVisibility())
        {
            return;
        }

        if (!m_aisPickShape || !m_aisArrowShape)
        {
            return;
        }

        // Disable the preview shape's pickablity.
        m_context->Deactivate(m_aisPickShape);

        // Deactive first.
        m_context->Deactivate(m_aisArrowShape);
        m_context->Load(m_aisArrowShape, -1);

        // Enable modes.
        for (const CommonOCC::SelectMode & mode : m_currSelectModes)
        {
            m_context->Activate(m_aisArrowShape, mode, true);
        }
    }

    void FITKOCCGraphObjectPickDirection::update(bool forceUpdate)
    {
        Q_UNUSED(forceUpdate);

        // Get and then reset the reverse flag.
        bool reverseFlag = getInputFlagForReverseNormal();
        //if (reverseFlag)
        //{
        //    // Data role: 1001.
        //    setInputVariantData(false, m_defaultInputRole + 1);
        //}

        // Get the direction and position.
        QList<double> dirList = getTempVariantData(ROLE_TEMP_PICK_DIRECTION_DIRECTION).value<QList<double>>();
        QList<double> posList = getOutputVariantData(ROLE_OUTPUT_PICK_DIRECTION_POSITION).value<QList<double>>();

        // Get the user defined direction.
        QList<double> userDire;
        bool hasUserDire = getInputUserDirection(userDire);
        if (hasUserDire)
        {
            dirList = userDire;
        }

        // Get the anchor position and direction.
        QList<double> anchorPosList;
        QList<double> anchorDirList;
        TopoDS_Shape anchorShape;
        bool hasAnchor = getInputAnchor(anchorPosList, anchorDirList, anchorShape);
        if (hasAnchor)
        {
            posList = anchorPosList;

            //if (dirList.count() != 3)
            //{
            //    dirList = anchorDirList;
            //}
        }

        if (dirList.count() != 3 || posList.count() != 3)
        {
            return;
        }

        // Re-created the arrow.
        gp_Pnt zeroPt(0., 0., 0.);
        gp_Pnt pt(posList[0], posList[1], posList[2]);
        gp_Dir gDir(dirList[0], dirList[1], dirList[2]);

        if (reverseFlag)
        {
            gDir.Reverse();
        }

        TopoDS_Shape arrowShape = FITKOCCCommons::CreateArrow(zeroPt, gDir);
        if (arrowShape.IsNull())
        {
            return;
        }

        // Update the shape and reset the update flag.
        m_aisArrowShape->SetShape(arrowShape);
        m_aisArrowShape->SetToUpdate();

        // Update zoomable properties.
#if OCC_VERSION_HEX < 0x070600
        Handle(Graphic3d_TransformPers) transPer = Graphic3d_TransformPers::FromDeprecatedParams(
            Graphic3d_TransModeFlags::Graphic3d_TMF_ZoomPers, pt);
#else
        Handle(Graphic3d_TransformPers) transPer = new Graphic3d_TransformPers(
            Graphic3d_TransModeFlags::Graphic3d_TMF_ZoomPers, pt);
#endif

        m_aisArrowShape->SetTransformPersistence(transPer);

        // Save the direction only.
        QList<double> newDirList{ gDir.X(), gDir.Y(), gDir.Z() };
        QVariant varDir = QVariant::fromValue<QList<double>>(newDirList);
        setOutputVariantData(varDir, ROLE_OUTPUT_PICK_DIRECTION_DIRECTION);

        // Redisplay.
        this->Superclass::update(true);
    }

    void FITKOCCGraphObjectPickDirection::setColor(QColor color)
    {
        if (!m_aisArrowShape || !color.isValid())
        {
            return;
        }

        // OCC color.
        Quantity_Color quanColor = FITKOCCCommons::QColorToQuan(color);

        // Initialize line default color.
        m_aisArrowShape->SetColor(quanColor);

        // Get the properties.
        Handle(Prs3d_Drawer) properties = m_aisArrowShape->Attributes();

        // Face properties.
        properties->ShadingAspect()->SetColor(quanColor);
    }

    void FITKOCCGraphObjectPickDirection::updateByMousePick(int mx, int my)
    {
        if (!m_view || !m_context || !m_aisArrowShape || !m_aisPickShape)
        {
            return;
        }

        // Return if the tool is invisible.
        if (m_context->DisplayStatus(m_aisArrowShape) != AIS_DisplayStatus::AIS_DS_Displayed)
        {
            return;
        }

        // Try to get the picked shape.
        m_context->InitSelected();
        if (!m_context->HasSelectedShape() || m_context->SelectedInteractive() == m_aisArrowShape)
        {
            return;
        }

        // Clear the reverse flag first.
        m_userInfo[ROLE_INPUT_PICK_DIRECTION_REVERSE] = false;

        // Get the picked data object and check if it is a sketch.
        TopoDS_Shape shape = m_context->SelectedShape();
        Handle(AIS_InteractiveObject) interObjPicked = m_context->SelectedInteractive();
        FITKAIS_ObjectBase* fobjBase = dynamic_cast<FITKAIS_ObjectBase*>(interObjPicked.get());
        if (!fobjBase)
        {
            return;
        }

        FITKOCCGraphObject3D* obj3d = fobjBase->getGraphObject();
        if (!obj3d)
        {
            return;
        }

        // Get the world position with the screen pixel position.
        gp_Pnt pt;
        gp_Vec dir;
        bool flag = false;
        bool centerFlag = getInputFlagForCenterNormal();

        Interface::FITKAbsGeoSketch2D* sketch = obj3d->getDataObjAs<Interface::FITKAbsGeoSketch2D>();
        if (sketch && shape.ShapeType() == TopAbs_ShapeEnum::TopAbs_COMPOUND)
        {
            // Get the sketch plane normal.
            double nor[3];
            sketch->getNormal(nor);
            dir.SetCoord(nor[0], nor[1], nor[2]);

            // Get the center of the shape.
            Bnd_Box bdBox;
            BRepBndLib::Add(shape, bdBox, false);
            double xMin, yMin, zMin, xMax, yMax, zMax;
            bdBox.Get(xMin, yMin, zMin, xMax, yMax, zMax);
            pt.SetCoord((xMin + xMax) * 0.5, (yMin + yMax) * 0.5, (zMin + zMax) * 0.5);

            flag = true;
        }
        else
        {
            // Check the shape and shape type.
            if (shape.IsNull() || (shape.ShapeType() != TopAbs_ShapeEnum::TopAbs_EDGE &&
                shape.ShapeType() != TopAbs_ShapeEnum::TopAbs_FACE))
            {
                return;
            }    

            if (centerFlag)
            {
                // Get the normal of the shape center.
                flag = FITKOCCCommons::GetShapeDirection(shape, dir, pt);
            }
            else
            {
                // Get the position of the shape with the picked point.
                if (shape.ShapeType() == TopAbs_ShapeEnum::TopAbs_EDGE)
                {
                    // Get the curve with the picked edge and get the extrema.
                    flag = FITKOCCCommons::ScreenToWorldWithCurve(mx, my, shape, m_view, pt);
                }
                else if (shape.ShapeType() == TopAbs_ShapeEnum::TopAbs_FACE)
                {
                    // Get the surface with the picked face and get the cross point.
                    flag = FITKOCCCommons::ScreenToWorldWithSurf(mx, my, shape, m_view, pt);
                }

                if (flag)
                {
                    // Get the direction on the picked world position.       
                    gp_Pnt ppt;
                    flag = FITKOCCCommons::GetShapeDirectionByPos(shape, dir, pt, ppt);
                }
            }
        }

        if (!flag)
        {
            return;
        }

        // Create the arrow shape.
        dir.Normalize();

        // Get and then reset the reverse flag.
        bool reverseFlag = getInputFlagForReverseNormal();
        if (reverseFlag)
        {
            // Data role: 1002.
            setUserData(false, ROLE_INPUT_PICK_DIRECTION_REVERSE);

            // dir.Reverse();
        }

        // Get the anchor position and direction.
        QList<double> anchorPosList;
        QList<double> anchorDirList;
        TopoDS_Shape anchorShape;
        bool hasAnchor = getInputAnchor(anchorPosList, anchorDirList, anchorShape);
        if (hasAnchor)
        {
            pt = gp_Pnt(anchorPosList[0], anchorPosList[1], anchorPosList[2]);
            // dir = gp_Vec(anchorDirList[0], anchorDirList[1], anchorDirList[2]);
        }

        gp_Dir gDir(dir.XYZ());
        gp_Pnt zeroPt(0., 0., 0.);
        TopoDS_Shape arrowShape = FITKOCCCommons::CreateArrow(zeroPt, gDir);
        if (arrowShape.IsNull())
        {
            return;
        }

        // Update the shape and reset the update flag.
        m_aisArrowShape->SetShape(arrowShape);
        m_aisArrowShape->SetToUpdate();

        // Update the previewing shape.   
        //if (centerFlag && !sketch)
        //{
        //    // m_aisPickShape->SetShape(TopoDS_Shape());

        //    // Preview the anchored shape.
        //    if (hasAnchor)
        //    {
        //        if (!anchorShape.IsNull())
        //        {
        //            m_aisPickShape->SetShape(anchorShape);
        //        }
        //    }
        //    // Preview the picked shape.
        //    else
        //    {
        //        m_aisPickShape->SetShape(shape);
        //    }
        //}
        //else
        //{
        //    m_aisPickShape->SetShape(TopoDS_Shape());
        //}

        //m_aisPickShape->SetToUpdate();

        // Update zoomable properties.
#if OCC_VERSION_HEX < 0x070600
        Handle(Graphic3d_TransformPers) transPer = Graphic3d_TransformPers::FromDeprecatedParams(
            Graphic3d_TransModeFlags::Graphic3d_TMF_ZoomPers, pt);
#else
        Handle(Graphic3d_TransformPers) transPer = new Graphic3d_TransformPers(
            Graphic3d_TransModeFlags::Graphic3d_TMF_ZoomPers, pt);
#endif

        m_aisArrowShape->SetTransformPersistence(transPer);

        // Save the direction.
        QList<double> dirList{ dir.X(), dir.Y(), dir.Z() };
        QVariant varDir = QVariant::fromValue<QList<double>>(dirList);
        setOutputVariantData(varDir, ROLE_OUTPUT_PICK_DIRECTION_DIRECTION);

        // Save the position.
        QList<double> posList{ pt.X(), pt.Y(), pt.Z() };
        QVariant varPos = QVariant::fromValue<QList<double>>(posList);
        setOutputVariantData(varPos, ROLE_OUTPUT_PICK_DIRECTION_POSITION);

        // Save the const direction.( for reverse )
        setTempVariantData(varDir, ROLE_TEMP_PICK_DIRECTION_DIRECTION);

        // Reset the reverse flag.
        setUserData(false, ROLE_INPUT_PICK_DIRECTION_REVERSE);

        // Update the interactive objects.
        this->Superclass::update(true);
    }

    void FITKOCCGraphObjectPickDirection::generateShape()
    {
        // Create the arrow shape.
        //@{
        m_aisArrowShape = new FITKOCCAIS_ColoredShape(TopoDS_Shape());

        // Save graph info.
        m_aisArrowShape->setGraphObject(this);

        // Drawer properties.
        //@{
        m_aisArrowShape->SetDisplayMode(AIS_DisplayMode::AIS_Shaded);

        Quantity_Color colorForArrow = FITKOCCCommons::QColorToQuan(FITKOCCCommons::s_markColor);

        // Initialize the default color.
        m_aisArrowShape->SetColor(colorForArrow);

        Handle(Prs3d_Drawer) arrowProperties = m_aisArrowShape->Attributes();

        // Face properties.
        arrowProperties->SetupOwnShadingAspect();
        arrowProperties->ShadingAspect()->SetColor(colorForArrow);
        //@}

        // Disable highlight properties from context.
        //@{
        // Setup own highlight drawer properties.( The same as the shape properties. )
        Handle(Prs3d_Drawer) highlightProperties = new Prs3d_Drawer(*arrowProperties.get());
        m_aisArrowShape->SetHilightAttributes(highlightProperties);
        //@}

        // Add to the interactive object list.
        Handle(AIS_Shape) arrowShapeHandle(m_aisArrowShape);
        addInteractiveObj(arrowShapeHandle);
        //@}

        // Create the previewing shape.
        //@{
        m_aisPickShape = new AIS_ColoredShape(TopoDS_Shape());

        // Drawer properties.
        //@{
        m_aisPickShape->SetDisplayMode(AIS_DisplayMode::AIS_WireFrame);

        Quantity_Color colorForShape = FITKOCCCommons::QColorToQuan(FITKOCCCommons::s_markPreviewShapeColor);
        double lineWidthForShape = FITKOCCCommons::s_modelLineWidth;

        // Initialize line width and default color.
        m_aisPickShape->SetWidth(lineWidthForShape);
        m_aisPickShape->SetColor(colorForShape);

        // Properties.
        //@{
        Handle(Prs3d_Drawer) previewProperties = m_aisPickShape->Attributes();

        // Face properties.
        previewProperties->SetupOwnShadingAspect();
        previewProperties->ShadingAspect()->SetColor(colorForShape);

        // Line and wire properties.
        previewProperties->LineAspect()->SetColor(colorForShape);
        previewProperties->WireAspect()->SetColor(colorForShape);
        previewProperties->LineAspect()->SetWidth(lineWidthForShape);
        previewProperties->WireAspect()->SetWidth(lineWidthForShape);

        // Face-line properties.
        previewProperties->SetupOwnFaceBoundaryAspect();
        previewProperties->SetFaceBoundaryDraw(Standard_True);
        previewProperties->FaceBoundaryAspect()->SetColor(colorForShape);
        previewProperties->FaceBoundaryAspect()->SetWidth(lineWidthForShape);
        //@}
        //@}
        //@}

        // Add to the interactive object list.
        Handle(AIS_Shape) previewShapeHandle(m_aisPickShape);
        addInteractiveObj(previewShapeHandle);
        //@
    }

    bool FITKOCCGraphObjectPickDirection::getInputUserDirection(QList<double> & dir)
    {
        // Data role: 1006.
        int role = ROLE_INPUT_PICK_DIRECTION_USER_DIRECTION;
        if (!m_userInfo.contains(role))
        {
            return false;
        }

        dir = m_userInfo[role].value<QList<double>>();
        if (dir.count() != 3)
        {
            return false;
        }

        return true;
    }

    bool FITKOCCGraphObjectPickDirection::getInputAnchor(QList<double> & pos, QList<double> & dir, TopoDS_Shape & anchoredShape)
    {
        pos.clear();
        dir.clear();

        int mainId, subId;
        bool hasAnchor = getInputAnchorId(mainId, subId);
        if (!hasAnchor)
        {
            return false;
        }

        bool isValid = false;

        // Check if the main data is a datum( line or plane ).
        Interface::FITKAbsGeoDatum* datum = FITKDATAREPO->getTDataByID<Interface::FITKAbsGeoDatum>(mainId);
        if (datum && !isValid)
        {
            // Datum line.
            Interface::FITKAbsGeoDatumLine* datumLine = dynamic_cast<Interface::FITKAbsGeoDatumLine*>(datum);
            if (datumLine && !isValid)
            {
                // Infinite line.
                if (datumLine->getIsInfinite())
                {
                    // Get the position and direction.
                    double pos3[3]{ 0., 0., 0. }, dir3[3]{ 0., 0., 0. };
                    datumLine->getInfiniteLine(pos3, dir3);

                    // Get the direction and mid point.
                    for (int i = 0; i < 3; i++)
                    {
                        pos << pos3[i];
                        dir << dir3[i];
                    }
                }
                // Finite line.
                else
                {
                    // Get the line points.
                    double pos1[3]{ 0., 0., 0. }, pos2[3]{ 0., 0., 0. };
                    datumLine->getFiniteLine(pos1, pos2);

                    // Get the direction and mid point.
                    for (int i = 0; i < 3; i++)
                    {
                        pos << (pos1[i] + pos2[i]) * 0.5;
                        dir << pos2[i] - pos1[i];
                    }
                }

                isValid = true;
            }

            // Datum plane.
            Interface::FITKAbsGeoDatumPlane* datumPlane = dynamic_cast<Interface::FITKAbsGeoDatumPlane*>(datum);
            if (datumPlane && !isValid)
            {
                // Get the normal and center.
                double center[3]{ 0., 0., 0. };
                double normal[3]{ 0., 0., 0. };
                double temp[3]{ 0., 0., 0. };
                datumPlane->getPlane(center, normal, temp);

                for (int i = 0; i < 3; i++)
                {
                    pos << center[i];
                    dir << normal[i];
                }

                isValid = true;
            }
        }

        // Check if the main data is a sketch.
        Interface::FITKAbsGeoSketch2D* sketch = FITKDATAREPO->getTDataByID<Interface::FITKAbsGeoSketch2D>(mainId);
        if (sketch && !isValid)
        {
            // Get the sketch plane normal.
            double nor[3];
            sketch->getNormal(nor);
            
            // Get the shape.
            OCC::FITKAbstractOCCModel* modelOCC = dynamic_cast<OCC::FITKAbstractOCCModel*>(sketch->getShapeAgent());
            if (!modelOCC)
            {
                return false;
            }

            TopoDS_Shape* shape = modelOCC->getShape();

            // Get the center of the shape.
            Bnd_Box bdBox;
            BRepBndLib::Add(*shape, bdBox, false);
            double xMin, yMin, zMin, xMax, yMax, zMax;
            bdBox.Get(xMin, yMin, zMin, xMax, yMax, zMax);

            // Save the values.
            pos << (xMin + xMax) * 0.5 << (yMin + yMax) * 0.5 << (zMin + zMax) * 0.5;
            dir << nor[0] << nor[1] << nor[2];

            isValid = true;
            anchoredShape = *shape;
        }

        // Check if the main data is a command.
        Interface::FITKAbsGeoCommand* command = FITKDATAREPO->getTDataByID<Interface::FITKAbsGeoCommand>(mainId);
        if (command && !isValid)
        {
            Interface::FITKAbsVirtualTopo* vTopo = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(subId);
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
            TopoDS_Shape shape = occVTopo->getTopoShape();

            // Get the center of the shape.
            gp_Pnt center;
            gp_Vec ctDir;
            bool flag = FITKOCCCommons::GetShapeDirection(shape, ctDir, center);
            if (!flag)
            {
                return false;
            }

            // Save the position and direction.
            pos << center.X() << center.Y() << center.Z();
            dir << ctDir.X() << ctDir.Y() << ctDir.Z();

            isValid = true;
            anchoredShape = shape;
        }

        return isValid;
    }

    bool FITKOCCGraphObjectPickDirection::getInputAnchorId(int & mainId, int & subId)
    {
        // Data role: 1004 & 1005.
        int roleMain = ROLE_INPUT_PICK_DIRECTION_ANCHOR_MAIN_ID;
        int roleSub = ROLE_INPUT_PICK_DIRECTION_ANCHOR_SUB_ID;
        if (!m_userInfo.contains(roleMain) || !m_userInfo.contains(roleSub))
        {
            return false;
        }

        mainId = m_userInfo[roleMain].value<int>();
        subId = m_userInfo[roleSub].value<int>();

        return true;
    }

    bool FITKOCCGraphObjectPickDirection::getInputFlagForCenterNormal()
    {
        // Data role: 1003.
        int role = ROLE_INPUT_PICK_DIRECTION_CENTER;
        if (!m_userInfo.contains(role))
        {
            return true;
        }

        return m_userInfo[role].value<bool>();
    }

    bool FITKOCCGraphObjectPickDirection::getInputFlagForReverseNormal()
    {
        // Data role: 1002.
        int role = ROLE_INPUT_PICK_DIRECTION_REVERSE;
        if (!m_userInfo.contains(role))
        {
            return false;
        }

        return m_userInfo[role].value<bool>();
    }
}   // namespace Exchange