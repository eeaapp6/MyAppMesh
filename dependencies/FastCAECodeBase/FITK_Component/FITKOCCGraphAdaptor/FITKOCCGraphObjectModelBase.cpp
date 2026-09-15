/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCGraphObjectModelBase.h"

// OCC
#include <TopExp.hxx>
#include <TopoDS_Shape.hxx>
#include <AIS_DisplayMode.hxx>
#include <Prs3d_Drawer.hxx>
#include <Prs3d_ShadingAspect.hxx>
#include <Prs3d_LineAspect.hxx>
#include <Prs3d_PointAspect.hxx>
#include <AIS_InteractiveContext.hxx>
#include <TopTools_IndexedMapOfShape.hxx>

// Graph
#include "FITKOCCAIS_ColoredShape.h"

// Data
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoPart.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Component/FITKGeoCompOCC/FITKAbstractOCCModel.h"

// Test
//@{
#include <QStandardPaths>
#include <QFileDialog>
#include <BRepBuilderAPI_MakeShape.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <STEPControl_Reader.hxx>
#include <IGESControl_Reader.hxx>
#include <StlAPI.hxx>
//@}

namespace Exchange
{
    FITKOCCGraphObjectModelBase::FITKOCCGraphObjectModelBase(Core::FITKAbstractDataObject* dataObj)
        : FITKOCCGraphObject3D(dataObj)
    {
        // Create shape indice map.
        m_indiceMap = new _ShapeIndiceMap;
        m_indiceMap->iVert = new TopTools_IndexedMapOfShape;
        m_indiceMap->iEdge = new TopTools_IndexedMapOfShape;
        m_indiceMap->iWire = new TopTools_IndexedMapOfShape;
        m_indiceMap->iFace = new TopTools_IndexedMapOfShape;
        m_indiceMap->iShell = new TopTools_IndexedMapOfShape;
        m_indiceMap->iSolid = new TopTools_IndexedMapOfShape;
        m_indiceMap->iCompSolid = new TopTools_IndexedMapOfShape;
        m_indiceMap->iComPound = new TopTools_IndexedMapOfShape;

        // Save the data type.
        m_graphInfo.Type = FITKOCCCommons::ModelGraph;

        // Save the fixed size flag.
        m_hasFixedSize = true;
    }

    FITKOCCGraphObjectModelBase::~FITKOCCGraphObjectModelBase()
    {
        // Delete the indice map.
        if (m_indiceMap)
        {
            deleteObj(m_indiceMap->iVert);
            deleteObj(m_indiceMap->iEdge);
            deleteObj(m_indiceMap->iWire);
            deleteObj(m_indiceMap->iFace);
            deleteObj(m_indiceMap->iShell);
            deleteObj(m_indiceMap->iSolid);
            deleteObj(m_indiceMap->iCompSolid);
            deleteObj(m_indiceMap->iComPound);
            deleteObj(m_indiceMap);
        }
    }

    int FITKOCCGraphObjectModelBase::getPartId()
    {
        Interface::FITKAbsGeoCommand* cmd = getGeoCommand();
        if (!cmd)
        {
            return -1;
        }

        Interface::FITKAbsGeoPart* part = dynamic_cast<Interface::FITKAbsGeoPart*>(cmd);
        if (!part)
        {
            part = dynamic_cast<Interface::FITKAbsGeoPart*>(cmd->getGeoCommandManager());
        }

        if (!part)
        {
            return -1;
        }

        return part->getDataObjectID();
    }

    void FITKOCCGraphObjectModelBase::setShapeTransparency(double trans)
    {
        if (!m_fAisShape)
        {
            return;
        }

        m_fAisShape->SetTransparency(trans);
    }

    void FITKOCCGraphObjectModelBase::clearShapeTransparency()
    {
        if (!m_fAisShape)
        {
            return;
        }

        m_fAisShape->UnsetTransparency();
    }

    void FITKOCCGraphObjectModelBase::setSubShapeColor(const TopoDS_Shape & shape, QColor color)
    {
        if (!m_fAisShape)
        {
            return;
        }

        m_fAisShape->setSubShapeColor(shape, color);
        m_fAisShape->SetToUpdate();
        updateInteractiveObjs();
        updateSelectModes();
    }

    void FITKOCCGraphObjectModelBase::setSubShapeTransparency(const TopoDS_Shape & shape, double trans)
    {
        if (!m_fAisShape)
        {
            return;
        }

        m_fAisShape->setSubShapeTransparency(shape, trans);
        m_fAisShape->SetToUpdate();
        updateInteractiveObjs();
        updateSelectModes();
    }

    void FITKOCCGraphObjectModelBase::setSubShapeHidden(const TopoDS_Shape & shape)
    {
        if (!m_fAisShape)
        {
            return;
        }

        m_fAisShape->setSubShapeHidden(shape);
        m_fAisShape->SetToUpdate();
        updateInteractiveObjs();
        updateSelectModes();
    }

    void FITKOCCGraphObjectModelBase::clearSubShapeTransparency()
    {
        if (!m_fAisShape)
        {
            return;
        }

        // Reset transparency.
        m_fAisShape->clearSubShapeTransparency();
        m_fAisShape->SetToUpdate();
        updateInteractiveObjs();
        updateSelectModes();
    }

    void FITKOCCGraphObjectModelBase::clearSubShapeAspect()
    {
        if (!m_fAisShape)
        {
            return;
        }

        m_fAisShape->clearSubShapeAspect();
        m_fAisShape->SetToUpdate();
        updateInteractiveObjs();
        updateSelectModes();
    }

    void FITKOCCGraphObjectModelBase::updateInteractiveObjs()
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

    int FITKOCCGraphObjectModelBase::getShapeIndex(FITKOCCCommons::ShapeMeshType type, TopoDS_Shape & shape)
    {
        if (!m_indiceMap || !m_useIndexMap)
        {
            return -1;
        }

        int index = -1;

        switch (type)
        {
        case Exchange::FITKOCCCommons::SMT_ShapeVertex:
            index = m_indiceMap->iVert->FindIndex(shape);
            break;
        case Exchange::FITKOCCCommons::SMT_ShapeEdge:
            index = m_indiceMap->iEdge->FindIndex(shape);
            break;
        case Exchange::FITKOCCCommons::SMT_ShapeFace:
            index = m_indiceMap->iFace->FindIndex(shape);
            break;
        case Exchange::FITKOCCCommons::SMT_ShapeSolid:
            index = m_indiceMap->iSolid->FindIndex(shape);
            break;
        case Exchange::FITKOCCCommons::SMT_TypeNone:
        case Exchange::FITKOCCCommons::SMT_MeshNode:
        case Exchange::FITKOCCCommons::SMT_MeshElement:
        default:
            return -1;
        }

        if (index <= 0)
        {
            index = -1;
        }

        return index;
    }

    TopoDS_Shape FITKOCCGraphObjectModelBase::getIndexedShape(FITKOCCCommons::ShapeMeshType type, int index)
    {
        if (!m_indiceMap || !m_useIndexMap)
        {
            return TopoDS_Shape();
        }

        switch (type)
        {
        case Exchange::FITKOCCCommons::SMT_ShapeVertex:
            return m_indiceMap->iVert->FindKey(index);
            break;
        case Exchange::FITKOCCCommons::SMT_ShapeEdge:
            return m_indiceMap->iEdge->FindKey(index);
            break;
        case Exchange::FITKOCCCommons::SMT_ShapeFace:
            return m_indiceMap->iFace->FindKey(index);
            break;
        case Exchange::FITKOCCCommons::SMT_ShapeSolid:
            return m_indiceMap->iSolid->FindKey(index);
            break;
        case Exchange::FITKOCCCommons::SMT_TypeNone:
        case Exchange::FITKOCCCommons::SMT_MeshNode:
        case Exchange::FITKOCCCommons::SMT_MeshElement:
        default:
            return TopoDS_Shape();
        }
    }

    void FITKOCCGraphObjectModelBase::setViewMode(FITKOCCCommons::ShapeMeshViewMode type, bool visible)
    {
        if (!m_fAisShape)
        {
            return;
        }

        // Properties.
        Handle(Prs3d_Drawer) properties = m_fAisShape->Attributes();

        switch (type)
        {
        case FITKOCCCommons::SMVM_Shade:
        {
            // Show or hide face with the transparency.
            properties->ShadingAspect()->SetTransparency(visible ? 0. : 1.);
            break;
        }
        case FITKOCCCommons::SMVM_Wireframe:
        {
            // Show or hide line with the type of line.
            properties->LineAspect()->SetTypeOfLine(visible ? Aspect_TypeOfLine::Aspect_TOL_SOLID : Aspect_TypeOfLine::Aspect_TOL_EMPTY);
            break;
        }
        case FITKOCCCommons::SMVM_Vertex:
        {
            // Show or hide vertex with the type of marker( point ).
            properties->PointAspect()->SetTypeOfMarker(visible ? Aspect_TypeOfMarker::Aspect_TOM_POINT : Aspect_TypeOfMarker::Aspect_TOM_EMPTY);
            break;
        }
        default:
            break;
        }

        // Save the visibility.
        FITKOCCGraphObject3D::setViewMode(type, visible);
    }

    void FITKOCCGraphObjectModelBase::setColor(QColor color, FITKOCCCommons::ShapeMeshType type, int index)
    {
        if (!m_indiceMap || !m_fAisShape || !color.isValid() || !m_useIndexMap)
        {
            return;
        }

        // Get the shape.
        TopoDS_Shape shape;

        // Lambda for checking whether the index is valid in map.
        auto checkMap = [=](TopTools_IndexedMapOfShape * map, int index, TopoDS_Shape & shape)
        {
            // Check the index.
            if (!map || index < 1 || index > map->Size())
            {
                return false;
            }

            // Get the shape by index.
            shape = map->FindKey(index);

            return true;
        };

        bool isValid = false;

        switch (type)
        {
        case FITKOCCCommons::SMT_ShapeVertex:
        {
            isValid = checkMap(m_indiceMap->iVert, index, shape);
            break;
        }
        case FITKOCCCommons::SMT_ShapeEdge:
        {
            isValid = checkMap(m_indiceMap->iEdge, index, shape);
            break;
        }
        case FITKOCCCommons::SMT_ShapeFace:
        {
            isValid = checkMap(m_indiceMap->iFace, index, shape);
            break;
        }
        case FITKOCCCommons::SMT_ShapeSolid:
        {
            isValid = checkMap(m_indiceMap->iSolid, index, shape);
            break;
        }
        case FITKOCCCommons::SMT_TypeNone:
        default:
            return;
        }

        // Check the shape.
        if (!isValid || shape.IsNull())
        {
            return;
        }

        // Set the color for the shape.
        m_fAisShape->SetCustomColor(shape, FITKOCCCommons::QColorToQuan(color));
    }

    void FITKOCCGraphObjectModelBase::setColor(QColor color, FITKOCCCommons::ShapeMeshType type, QList<int> indice)
    {
        if (!m_indiceMap || !m_fAisShape || !color.isValid() || !m_useIndexMap)
        {
            return;
        }

        // Lambda for checking whether the index is valid in map and change the color at the same time.
        auto changeColor = [=](TopTools_IndexedMapOfShape * map, QList<int> & indice, QColor color, AIS_ColoredShape* aisShape)
        {
            Quantity_Color occColor = FITKOCCCommons::QColorToQuan(color);

            for (const int & index : indice)
            {
                // Check the index.
                if (!map || index < 1 || index > map->Size())
                {
                    continue;
                }

                // Get the shape by index.
                const TopoDS_Shape & shape = map->FindKey(index);
                if (shape.IsNull())
                {
                    continue;
                }

                // Set the color for the shape.
                aisShape->SetCustomColor(shape, occColor);
            }
        };

        switch (type)
        {
        case FITKOCCCommons::SMT_ShapeVertex:
        {
            changeColor(m_indiceMap->iVert, indice, color, m_fAisShape);
            break;
        }
        case FITKOCCCommons::SMT_ShapeEdge:
        {
            changeColor(m_indiceMap->iEdge, indice, color, m_fAisShape);
            break;
        }
        case FITKOCCCommons::SMT_ShapeFace:
        {
            changeColor(m_indiceMap->iFace, indice, color, m_fAisShape);
            break;
        }
        case FITKOCCCommons::SMT_ShapeSolid:
        {
            changeColor(m_indiceMap->iSolid, indice, color, m_fAisShape);
            break;
        }
        case FITKOCCCommons::SMT_TypeNone:
        default:
            return;
        }
    }

    void FITKOCCGraphObjectModelBase::setColor(QColor color)
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

    void FITKOCCGraphObjectModelBase::setEnableTransparency(bool isOn)
    {
        if (!m_fAisShape)
        {
            return;
        }

        m_fAisShape->SetTransparency(isOn ? FITKOCCCommons::s_transpacency : 0.);
    }

    void FITKOCCGraphObjectModelBase::setPointMark(FITKOCCCommons::PointMarkShape type)
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

    void FITKOCCGraphObjectModelBase::setPointSizeScale(double factor)
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

    void FITKOCCGraphObjectModelBase::setLineWidth(double size)
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

    void FITKOCCGraphObjectModelBase::highlight(bool update)
    {
        if (!m_context || !m_fAisShape || m_isHighlighting)
        {
            return;
        }

        // Highlight the whole object.
        m_context->HilightWithColor(m_fAisShape, m_context->HighlightStyle(Prs3d_TypeOfHighlight::Prs3d_TypeOfHighlight_LocalSelected), update);

        // Save the highlight flag.
        FITKOCCGraphObject3D::highlight(update);
    }

    void FITKOCCGraphObjectModelBase::disHighlight(bool update)
    {
        if (!m_context || !m_fAisShape || !m_isHighlighting)
        {
            return;
        }

        // Dis-highlight the whole object.
        m_context->Unhilight(m_fAisShape, update);

        // Save the highlight flag.
        FITKOCCGraphObject3D::disHighlight(update);
    }

    void FITKOCCGraphObjectModelBase::setVisible(bool visibility)
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
            // Dishighight first.
            disHighlight();

            // Hide the object.
            m_context->Erase(m_fAisShape, true);
        }
    }

    void FITKOCCGraphObjectModelBase::addSelectMode(CommonOCC::SelectMode mode, bool force)
    {
        // Clear select modes.
        if (mode == CommonOCC::SelectMode::S_None)
        {
            clearSelectMode();
            return;
        }

        // Check the shape data.
        if (!m_tpShape || !m_context)
        {
            return;
        }

        // Check the select mode.
        CommonOCC::SelectMode realMode = CommonOCC::SelectMode::S_None;
        if (m_supportedSelectModeHash[mode] || force)
        {
            realMode = mode;
        }

        // Deal with the compound select mode.
        //@{
        if (realMode == CommonOCC::SelectMode::S_ShapeCompound &&
            m_tpShape->ShapeType() != TopAbs_ShapeEnum::TopAbs_COMPOUND)
        {
            realMode = CommonOCC::SelectMode::S_None;
        }
        //@}

        if (m_currSelectModes.contains(realMode))
        {
            return;
        }

        // Save the mode.
        if (realMode != CommonOCC::SelectMode::S_None)
        {
            m_currSelectModes.push_back(realMode);
        }

        // Set all objects'select mode by default.
        for (const opencascade::handle<AIS_InteractiveObject> & obj : m_objs)
        {
            if (!obj)
            {
                continue;
            }

            if (realMode == CommonOCC::SelectMode::S_None)
            {
                m_context->Deactivate(obj);
            }
            else
            {
                // Check if the object is visible.
                if (getVisibility())
                {
                    m_context->Load(obj, -1);
                    m_context->Activate(obj, realMode, true);
                }
            }
        }
    }

    bool FITKOCCGraphObjectModelBase::test(QVariant& details)
    {
        if (!details.isValid())
        {
            return false;
        }

        if (!m_tpShape)
        {
            m_tpShape = new TopoDS_Shape;
        }

        // Lambda for modeling.
        auto modeling = [=](BRepBuilderAPI_MakeShape & cmd)
        {
            // Custom build model.
            //@{
            cmd.Build();
            if (cmd.IsDone())
            {
                TopoDS_Shape shape = cmd.Shape();
                *m_tpShape = shape;
            }
            //@}
        };

        switch (details.toInt())
        {
        case 1:
        {
            // Make Box
            //@{
            BRepPrimAPI_MakeBox cmd(gp_Pnt(0, 0, 0), 1, 1, 1);
            modeling(cmd);
            //@}
            break;
        }
        case 2:
        {
            // Make Cylinder
            //@{
            BRepPrimAPI_MakeCylinder cmd(1000, 500);
            modeling(cmd);
            //@}
            break;
        }
        case 0:
        {
            QString desktop = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);

            // Read STP/IGS/STL
            //@{
            QString filePath = QFileDialog::getOpenFileName(nullptr, tr("Choose STP/IGS/STL file"), "",
                tr("*.stp;*step;*.igs;*.iges;*.stl"));
            if (filePath.isEmpty())
            {
                return false;
            }

            if (filePath.toLower().endsWith("stp") || filePath.toLower().endsWith("step"))
            {
                STEPControl_Reader reader;
                Standard_Integer status = reader.ReadFile(QStringToCharA(filePath));
                if (!status)
                {
                    return false;
                }

                reader.TransferRoots();
                TopoDS_Shape shape = reader.OneShape();
                *m_tpShape = shape;
            }
            else if (filePath.toLower().endsWith("igs") || filePath.toLower().endsWith("iges"))
            {
                IGESControl_Reader reader;
                Standard_Integer status = reader.ReadFile(QStringToCharA(filePath));
                if (!status)
                {
                    return false;
                }

                reader.TransferRoots();
                TopoDS_Shape shape = reader.OneShape();
                *m_tpShape = shape;
            }
            else if (filePath.toLower().endsWith("stl"))
            {
                TopoDS_Shape shape;
                StlAPI::Read(shape, QStringToCharA(filePath));
                *m_tpShape = shape;
            }
            //@}

            break;
        }
        default:
            return false;
        }

        if (!m_tpShape)
        {
            return false;
        }

        // Create shape indice map.
        m_indiceMap = new _ShapeIndiceMap;

        if (m_useIndexMap)
        {
            initShapeIndiceMap(*m_tpShape, m_indiceMap);
        }

        // Generate the shape interactive objects.
        generateShape();

        return true;
    }

    void FITKOCCGraphObjectModelBase::initShapeIndiceMap(TopoDS_Shape& shape, _ShapeIndiceMap* indiceMap)
    {
        // Check the shape first.
        if (shape.IsNull())
        {
            return;
        }

        // Initialize maps.
        TopExp::MapShapes(shape, TopAbs_VERTEX, *indiceMap->iVert);
        TopExp::MapShapes(shape, TopAbs_EDGE, *indiceMap->iEdge);
        // TopExp::MapShapes(shape, TopAbs_WIRE, *indiceMap->iWire);
        // TopExp::MapShapes(shape, TopAbs_SHELL, i*ndiceMap->iShell);
        TopExp::MapShapes(shape, TopAbs_FACE, *indiceMap->iFace);
        TopExp::MapShapes(shape, TopAbs_SOLID, *indiceMap->iSolid);
        TopExp::MapShapes(shape, TopAbs_COMPSOLID, *indiceMap->iCompSolid);
        // TopExp::MapShapes(shape, TopAbs_COMPOUND, *indiceMap->iComPound);
    }

    void FITKOCCGraphObjectModelBase::generateShape()
    {
        if (!m_tpShape || !_dataObj)
        {
            return;
        }

        // Create the FITKOCCAIS_ColoredShape.
        m_fAisShape = new FITKOCCAIS_ColoredShape(*m_tpShape);
        m_fAisShape->SetTypeOfPresentation(PrsMgr_TypeOfPresentation3d::PrsMgr_TOP_AllView);

        // Save the graph object.
        m_fAisShape->setGraphObject(this);
        m_fAisShape->setDataObjectId(_dataObj->getDataObjectID());

        // Create handle.
        Handle(FITKOCCAIS_ColoredShape) shapeHandle(m_fAisShape);

        // Initialize properties.
        //@{
       // Display mode - AIS_Shaded or AIS_WireFrame
        m_fAisShape->SetDisplayMode(AIS_DisplayMode::AIS_Shaded);
        m_fAisShape->SetMaterial(Graphic3d_MaterialAspect(Graphic3d_NameOfMaterial::Graphic3d_NOM_STONE));

        Quantity_Color edgeColor = FITKOCCCommons::QColorToQuan(FITKOCCCommons::s_edgeColor);
        double lineWidth = FITKOCCCommons::s_modelLineWidth;

        // Initialize line width and default color.
        m_fAisShape->SetWidth(lineWidth);
        m_fAisShape->SetColor(edgeColor);

        // Properties.
        //@{
        Handle(Prs3d_Drawer) properties = m_fAisShape->Attributes();

        // Face properties.
        properties->SetupOwnShadingAspect();
        properties->ShadingAspect()->SetColor(FITKOCCCommons::QColorToQuan(FITKOCCCommons::s_faceColor));

        // Line and wire properties.
        properties->LineAspect()->SetColor(edgeColor);
        properties->WireAspect()->SetColor(edgeColor);
        properties->LineAspect()->SetWidth(lineWidth);
        properties->WireAspect()->SetWidth(lineWidth);

        // Face-line properties.
        properties->SetupOwnFaceBoundaryAspect();
        properties->SetFaceBoundaryDraw(Standard_True);
        properties->FaceBoundaryAspect()->SetColor(edgeColor);
        properties->FaceBoundaryAspect()->SetWidth(lineWidth);

        // Point properties.
        properties->SetupOwnPointAspect();
        properties->SetVertexDrawMode(Prs3d_VertexDrawMode::Prs3d_VDM_All);
        properties->PointAspect()->SetScale(5.);
        properties->PointAspect()->SetTypeOfMarker(Aspect_TypeOfMarker::Aspect_TOM_POINT);
        properties->PointAspect()->SetColor(FITKOCCCommons::QColorToQuan(FITKOCCCommons::s_vertexColor));
        m_fAisShape->SetAttributes(properties);
        //@}
        //@}

        // Add to the interactive object list.
        addInteractiveObj(shapeHandle);

        // Save the shape's hash code.
        m_hashCode = m_tpShape->HashCode(INT_MAX);
    }
}   // namespace Exchange
