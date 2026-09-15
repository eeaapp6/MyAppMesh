/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCAIS_ColoredShape.h"

// OCC
#include <Standard.hxx>
#include <Standard_Type.hxx>
#include <TopoDS_Shape.hxx>
#include <Prs3d.hxx>
#include <StdSelect_BRepSelectionTool.hxx>
#include <BRepTools.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <TopoDS_Iterator.hxx>

FITKOCCAIS_ColoredShape::FITKOCCAIS_ColoredShape(const TopoDS_Shape& theShape) : AIS_ColoredShape(theShape)
{
    // Nothing to do here.
}

FITKOCCAIS_ColoredShape::FITKOCCAIS_ColoredShape(const Handle(FITKOCCAIS_ColoredShape)& theShape) : AIS_ColoredShape(theShape)
{
    // Nothing to do here.
}

void FITKOCCAIS_ColoredShape::setSubShapeColor(const TopoDS_Shape & shape, QColor color)
{
    //if (!m_coloredShapes.contains(shape))
    //{
    //    m_coloredShapes.push_back(shape);
    //}

    Quantity_Color quanColor;
    quanColor.SetValues(color.redF(), color.greenF(), color.blueF(), Quantity_TypeOfColor::Quantity_TOC_RGB);

    this->SetCustomColor(shape, quanColor);
}

void FITKOCCAIS_ColoredShape::setSubShapeTransparency(const TopoDS_Shape & shape, double trans)
{
    //if (!m_transShapes.contains(shape))
    //{
    //    m_transShapes.push_back(shape);
    //}

    this->SetCustomTransparency(shape, trans);
}

void FITKOCCAIS_ColoredShape::setSubShapeHidden(const TopoDS_Shape & shape)
{
    // Hide the face.
    const Handle(AIS_ColoredDrawer)& drawer = CustomAspects(shape);
    if (!drawer)
    {
        return;
    }

    //if (!m_hiddenShapes.contains(shape))
    //{
    //    m_hiddenShapes.push_back(shape);
    //}

    drawer->SetHidden(true);

    // Remove trianglations.
    //BRepTools::Clean(shape);
    //BRepTools::Update(shape);
}

void FITKOCCAIS_ColoredShape::clearSubShapeTransparency()
{
    // Reset transparency and hidden flag.
    for (AIS_DataMapOfShapeDrawer::Iterator anIter(myShapeColors); anIter.More(); anIter.Next())
    {
        const Handle(AIS_ColoredDrawer)& aDrawer = anIter.Value();
        if (aDrawer->HasOwnWidth())
        {
            continue;
        }

        aDrawer->SetOwnTransparency(0.);
        aDrawer->SetHidden(false);
    }

    //m_transShapes.clear();
    //m_transShapes.clear();
}

void FITKOCCAIS_ColoredShape::clearSubShapeAspect()
{
    // Reset transparency and hidden flag.
    clearSubShapeTransparency();

    this->ClearCustomAspects();
    //m_coloredShapes.clear();
}

//void FITKOCCAIS_ColoredShape::ComputeSelection(const Handle(SelectMgr_Selection) & theSelection,
//    const Standard_Integer theMode)
//{
//    // From OCC.
//    //@{
//    if (myshape.IsNull())
//    {
//        return;
//    }
//    else if (isShapeEntirelyVisible())
//    {
//        base_type::ComputeSelection(theSelection, theMode);
//        return;
//    }
//
//    const TopAbs_ShapeEnum aTypOfSel = AIS_Shape::SelectionType(theMode);
//    const Standard_Real    aDeflection = Prs3d::GetDeflection(myshape, myDrawer);
//    const Standard_Real    aDeviationAngle = myDrawer->HLRAngle();
//    const Standard_Integer aPriority = StdSelect_BRepSelectionTool::GetStandardPriority(myshape, aTypOfSel);
//    if (myDrawer->IsAutoTriangulation()
//        && !BRepTools::Triangulation(myshape, Precision::Infinite()))
//    {
//        BRepMesh_IncrementalMesh aMesher(myshape, aDeflection, Standard_False, aDeviationAngle);
//    }
//
//    AIS_DataMapOfShapeDrawer aSubshapeDrawerMap;
//    fillSubshapeDrawerMap(aSubshapeDrawerMap);
//
//    Handle(StdSelect_BRepOwner) aBrepOwner = new StdSelect_BRepOwner(myshape, aPriority);
//    if (aTypOfSel == TopAbs_SHAPE)
//    {
//        aBrepOwner = new StdSelect_BRepOwner(myshape, aPriority);
//    }
//
//    Handle(AIS_ColoredDrawer) aBaseDrawer;
//    myShapeColors.Find(myshape, aBaseDrawer);
//    computeSubshapeSelectionSkipInVis(aBaseDrawer, aSubshapeDrawerMap, myshape, aBrepOwner, theSelection,
//        aTypOfSel, aPriority, aDeflection, aDeviationAngle);
//
//    Handle(SelectMgr_SelectableObject) aThis(this);
//    for (NCollection_Vector<Handle(SelectMgr_SensitiveEntity)>::Iterator aSelEntIter(theSelection->Entities()); aSelEntIter.More(); aSelEntIter.Next())
//    {
//        const Handle(SelectMgr_EntityOwner)& anOwner = aSelEntIter.Value()->BaseSensitive()->OwnerId();
//        anOwner->SetSelectable(aThis);
//    }
//
//    StdSelect_BRepSelectionTool::PreBuildBVH(theSelection);
//    //@}
//}
//
//void FITKOCCAIS_ColoredShape::computeSubshapeSelectionSkipInVis(const Handle(AIS_ColoredDrawer)& theParentDrawer,
//    const AIS_DataMapOfShapeDrawer& theShapeDrawerMap,
//    const TopoDS_Shape& theShape,
//    const Handle(StdSelect_BRepOwner)& theOwner,
//    const Handle(SelectMgr_Selection)& theSelection,
//    const TopAbs_ShapeEnum theTypOfSel,
//    const Standard_Integer thePriority,
//    const Standard_Real theDeflection,
//    const Standard_Real theDeflAngle)
//{
//    Handle(AIS_ColoredDrawer) aDrawer = theParentDrawer;
//    theShapeDrawerMap.Find(theShape, aDrawer);
//    if (!aDrawer.IsNull()
//        && aDrawer->IsHidden())
//    {
//        return;
//    }
//
//    const Standard_Integer aNbPOnEdge = 9;
//    const Standard_Real    aMaximalParameter = 500.0;
//    if (theTypOfSel == TopAbs_SHAPE
//        && theShape.ShapeType() >= TopAbs_FACE)
//    {
//        StdSelect_BRepSelectionTool::ComputeSensitive(theShape, theOwner, theSelection,
//            theDeflection, theDeflAngle, aNbPOnEdge, aMaximalParameter, myDrawer->IsAutoTriangulation());
//        return;
//    }
//    else if (theShape.ShapeType() == theTypOfSel)
//    {
//        const Standard_Boolean isComesFromDecomposition = !theShape.IsEqual(myshape);
//        Handle(StdSelect_BRepOwner) aBrepOwner = new StdSelect_BRepOwner(theShape, thePriority, isComesFromDecomposition);
//        StdSelect_BRepSelectionTool::ComputeSensitive(theShape, aBrepOwner, theSelection,
//            theDeflection, theDeflAngle, aNbPOnEdge, aMaximalParameter, myDrawer->IsAutoTriangulation());
//        return;
//    }
//
//    for (TopoDS_Iterator aSubShapeIter(theShape); aSubShapeIter.More(); aSubShapeIter.Next())
//    {
//        const TopoDS_Shape& aSubShape = aSubShapeIter.Value();
//
//        computeSubshapeSelectionSkipInVis(aDrawer, theShapeDrawerMap, aSubShape,
//            theOwner, theSelection, theTypOfSel, thePriority,
//            theDeflection, theDeflAngle);
//    }
//}
