/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKV3d_RectangularGrid.h"

// OCC
#include <Graphic3d_ArrayOfPoints.hxx>
#include <Graphic3d_ArrayOfSegments.hxx>
#include <Graphic3d_AspectLine3d.hxx>
#include <Graphic3d_AspectMarker3d.hxx>
#include <Graphic3d_AspectText3d.hxx>
#include <Graphic3d_Group.hxx>
#include <Graphic3d_Structure.hxx>
#include <Quantity_Color.hxx>
#include <Standard_Type.hxx>
#include <TColgp_SequenceOfPnt.hxx>
#include <TColStd_Array2OfReal.hxx>
#include <V3d_Viewer.hxx>
#include <Standard_Version.hxx>

#if OCC_VERSION_HEX < 0x070600
#include <Geom_Transformation.hxx>
#else
#include <TopLoc_Datum3D.hxx>
#endif

namespace
{
    static const Standard_Real THE_DEFAULT_GRID_STEP = 10.0;
#define MYFACTOR 50.
}

//! Dummy implementation of Graphic3d_Structure overriding ::Compute() method for handling Device Lost.
class FITKV3d_RectangularGrid::FITKRectangularGridStructure : public Graphic3d_Structure
{
public:
    //! Main constructor.
    FITKRectangularGridStructure(const Handle(Graphic3d_StructureManager)& theManager, FITKV3d_RectangularGrid* theGrid)
        : Graphic3d_Structure(theManager), myGrid(theGrid) {}

    //! Override method initiating recomputing in FITKV3d_RectangularGrid.
    virtual void Compute() Standard_OVERRIDE
    {
        GraphicClear(Standard_False);
        myGrid->myGroup = NewGroup();
        myGrid->myCurAreDefined = Standard_False;
        myGrid->UpdateDisplay();
    }

private:
    FITKV3d_RectangularGrid* myGrid;
};

/*----------------------------------------------------------------------*/

FITKV3d_RectangularGrid::FITKV3d_RectangularGrid(const V3d_ViewerPointer& aViewer, const Quantity_Color& aColor, const Quantity_Color& aTenthColor)
    : Aspect_RectangularGrid(1., 1.),
    myViewer(aViewer),
    myCurAreDefined(Standard_False),
    myToComputePrs(Standard_True),
    myCurDrawMode(Aspect_GDM_Lines),
    myCurXo(0.0),
    myCurYo(0.0),
    myCurAngle(0.0),
    myCurXStep(0.0),
    myCurYStep(0.0),
    myXSize(0.5 * aViewer->DefaultViewSize()),
    myYSize(0.5 * aViewer->DefaultViewSize()),
    myOffSet(THE_DEFAULT_GRID_STEP / MYFACTOR)
{
    myColor = aColor;
    myTenthColor = aTenthColor;

    myStructure = new FITKRectangularGridStructure(aViewer->StructureManager(), this);
    myGroup = myStructure->NewGroup();
    myStructure->SetInfiniteState(Standard_True);

    // Added by ChengHaotian
    myStructure->SetZLayer(Graphic3d_ZLayerId_Topmost);

    SetXStep(THE_DEFAULT_GRID_STEP);
    SetYStep(THE_DEFAULT_GRID_STEP);
}

FITKV3d_RectangularGrid::~FITKV3d_RectangularGrid()
{
    myGroup.Nullify();
    if (!myStructure.IsNull())
    {
        myStructure->Erase();
    }
}

void FITKV3d_RectangularGrid::SetColors(const Quantity_Color& aColor, const Quantity_Color& aTenthColor)
{
    if (myColor != aColor || myTenthColor != aTenthColor) {
        myColor = aColor;
        myTenthColor = aTenthColor;
        myCurAreDefined = Standard_False;
        UpdateDisplay();
    }
}

void FITKV3d_RectangularGrid::Display()
{
    myStructure->SetDisplayPriority(1);
    myStructure->Display();
    UpdateDisplay();
}

void FITKV3d_RectangularGrid::Erase() const
{
    myStructure->Erase();
}

Standard_Boolean FITKV3d_RectangularGrid::IsDisplayed() const
{
    return myStructure->IsDisplayed();
}

void FITKV3d_RectangularGrid::UpdateDisplay()
{
    gp_Ax3 ThePlane = myViewer->PrivilegedPlane();

    Standard_Boolean MakeTransform = Standard_False;
    Standard_Real xl, yl, zl;
    Standard_Real xdx, xdy, xdz;
    Standard_Real ydx, ydy, ydz;
    Standard_Real dx, dy, dz;
    ThePlane.Location().Coord(xl, yl, zl);
    ThePlane.XDirection().Coord(xdx, xdy, xdz);
    ThePlane.YDirection().Coord(ydx, ydy, ydz);
    ThePlane.Direction().Coord(dx, dy, dz);
    if (!myCurAreDefined)
        MakeTransform = Standard_True;
    else {
        if (RotationAngle() != myCurAngle || XOrigin() != myCurXo || YOrigin() != myCurYo)
            MakeTransform = Standard_True;
        if (!MakeTransform) {
            Standard_Real curxl, curyl, curzl;
            Standard_Real curxdx, curxdy, curxdz;
            Standard_Real curydx, curydy, curydz;
            Standard_Real curdx, curdy, curdz;
            myCurViewPlane.Location().Coord(curxl, curyl, curzl);
            myCurViewPlane.XDirection().Coord(curxdx, curxdy, curxdz);
            myCurViewPlane.YDirection().Coord(curydx, curydy, curydz);
            myCurViewPlane.Direction().Coord(curdx, curdy, curdz);
            if (xl != curxl || yl != curyl || zl != curzl ||
                xdx != curxdx || xdy != curxdy || xdz != curxdz ||
                ydx != curydx || ydy != curydy || ydz != curydz ||
                dx != curdx || dy != curdy || dz != curdz)
                MakeTransform = Standard_True;
        }
    }

    if (MakeTransform) {
        const Standard_Real CosAlpha = Cos(RotationAngle());
        const Standard_Real SinAlpha = Sin(RotationAngle());

        gp_Trsf aTrsf;
        // Translation
        // Transformation of change of marker
        aTrsf.SetValues(xdx, ydx, dx, xl,
            xdy, ydy, dy, yl,
            xdz, ydz, dz, zl);

        // Translation of the origin
        // Rotation Alpha around axis -Z
        gp_Trsf aTrsf2;
        aTrsf2.SetValues(CosAlpha, SinAlpha, 0.0, -XOrigin(),
            -SinAlpha, CosAlpha, 0.0, -YOrigin(),
            0.0, 0.0, 1.0, 0.0);
        aTrsf.Multiply(aTrsf2);

#if OCC_VERSION_HEX < 0x070600
        Handle(Geom_Transformation) gTransf = new Geom_Transformation(aTrsf);
        myStructure->SetTransformation(gTransf);
#else
        Handle(TopLoc_Datum3D) tld = new TopLoc_Datum3D(aTrsf);
        myStructure->SetTransformation(tld);
#endif

        myCurAngle = RotationAngle();
        myCurXo = XOrigin(), myCurYo = YOrigin();
        myCurViewPlane = ThePlane;
    }

    switch (myDrawMode)
    {
    case Aspect_GDM_Points:
        DefinePoints();
        myCurDrawMode = Aspect_GDM_Points;
        break;
    case Aspect_GDM_Lines:
        DefineLines();
        myCurDrawMode = Aspect_GDM_Lines;
        break;
    case Aspect_GDM_None:
        myCurDrawMode = Aspect_GDM_None;
        break;
    }
    myCurAreDefined = Standard_True;
}

void FITKV3d_RectangularGrid::DefineLines()
{
    const Standard_Real aXStep = XStep();
    const Standard_Real aYStep = YStep();
    const Standard_Boolean toUpdate = !myCurAreDefined
        || myCurDrawMode != Aspect_GDM_Lines
        || aXStep != myCurXStep
        || aYStep != myCurYStep;
    if (!toUpdate
        && !myToComputePrs)
    {
        return;
    }
    else if (!myStructure->IsDisplayed())
    {
        myToComputePrs = Standard_True;
        return;
    }

    myToComputePrs = Standard_False;
    myGroup->Clear();

    Standard_Integer nblines;
    Standard_Real xl, yl, zl = myOffSet;

    TColgp_SequenceOfPnt aSeqLines, aSeqTenth;

    // verticals
    aSeqTenth.Append(gp_Pnt(0., -myYSize, -zl));
    aSeqTenth.Append(gp_Pnt(0., myYSize, -zl));
    for (nblines = 1, xl = aXStep; xl < myXSize; xl += aXStep, nblines++)
    {
        TColgp_SequenceOfPnt &aSeq = (Modulus(nblines, 10) != 0) ? aSeqLines : aSeqTenth;
        aSeq.Append(gp_Pnt(xl, -myYSize, -zl));
        aSeq.Append(gp_Pnt(xl, myYSize, -zl));
        aSeq.Append(gp_Pnt(-xl, -myYSize, -zl));
        aSeq.Append(gp_Pnt(-xl, myYSize, -zl));
    }

    // horizontals
    aSeqTenth.Append(gp_Pnt(-myXSize, 0., -zl));
    aSeqTenth.Append(gp_Pnt(myXSize, 0., -zl));
    for (nblines = 1, yl = aYStep; yl < myYSize; yl += aYStep, nblines++)
    {
        TColgp_SequenceOfPnt &aSeq = (Modulus(nblines, 10) != 0) ? aSeqLines : aSeqTenth;
        aSeq.Append(gp_Pnt(-myXSize, yl, -zl));
        aSeq.Append(gp_Pnt(myXSize, yl, -zl));
        aSeq.Append(gp_Pnt(-myXSize, -yl, -zl));
        aSeq.Append(gp_Pnt(myXSize, -yl, -zl));
    }

    if (aSeqLines.Length())
    {
        Handle(Graphic3d_AspectLine3d) aLineAspect = new Graphic3d_AspectLine3d(myColor, Aspect_TOL_SOLID, 1.0);
        myGroup->SetPrimitivesAspect(aLineAspect);
        const Standard_Integer nbv = aSeqLines.Length();
        Handle(Graphic3d_ArrayOfSegments) aPrims = new Graphic3d_ArrayOfSegments(nbv);
        Standard_Integer n = 1;
        while (n <= nbv)
            aPrims->AddVertex(aSeqLines(n++));
        myGroup->AddPrimitiveArray(aPrims, Standard_False);
    }
    if (aSeqTenth.Length())
    {
        Handle(Graphic3d_AspectLine3d) aLineAspect = new Graphic3d_AspectLine3d(myTenthColor, Aspect_TOL_SOLID, 1.0);
        myGroup->SetPrimitivesAspect(aLineAspect);
        const Standard_Integer nbv = aSeqTenth.Length();
        Handle(Graphic3d_ArrayOfSegments) aPrims = new Graphic3d_ArrayOfSegments(nbv);
        Standard_Integer n = 1;
        while (n <= nbv)
            aPrims->AddVertex(aSeqTenth(n++));
        myGroup->AddPrimitiveArray(aPrims, Standard_False);
    }

    myGroup->SetMinMaxValues(-myXSize, -myYSize, -myOffSet, myXSize, myYSize, -myOffSet);
    myCurXStep = aXStep, myCurYStep = aYStep;

    // update bounding box
    myStructure->CalculateBoundBox();
    myViewer->StructureManager()->Update(myStructure->GetZLayer());
}

void FITKV3d_RectangularGrid::DefinePoints()
{
    const Standard_Real aXStep = XStep();
    const Standard_Real aYStep = YStep();
    const Standard_Boolean toUpdate = !myCurAreDefined
        || myCurDrawMode != Aspect_GDM_Points
        || aXStep != myCurXStep
        || aYStep != myCurYStep;
    if (!toUpdate
        && !myToComputePrs)
    {
        return;
    }
    else if (!myStructure->IsDisplayed())
    {
        myToComputePrs = Standard_True;
        return;
    }

    myToComputePrs = Standard_False;
    myGroup->Clear();

    // horizontals
    Standard_Real xl, yl;
    TColgp_SequenceOfPnt aSeqPnts;
    for (xl = 0.0; xl <= myXSize; xl += aXStep) {
        aSeqPnts.Append(gp_Pnt(xl, 0.0, -myOffSet));
        aSeqPnts.Append(gp_Pnt(-xl, 0.0, -myOffSet));
        for (yl = aYStep; yl <= myYSize; yl += aYStep) {
            aSeqPnts.Append(gp_Pnt(xl, yl, -myOffSet));
            aSeqPnts.Append(gp_Pnt(xl, -yl, -myOffSet));
            aSeqPnts.Append(gp_Pnt(-xl, yl, -myOffSet));
            aSeqPnts.Append(gp_Pnt(-xl, -yl, -myOffSet));
        }
    }
    if (aSeqPnts.Length())
    {
        Standard_Integer i;
        Standard_Real X, Y, Z;
        const Standard_Integer nbv = aSeqPnts.Length();
        Handle(Graphic3d_ArrayOfPoints) Vertical = new Graphic3d_ArrayOfPoints(nbv);
        for (i = 1; i <= nbv; i++)
        {
            aSeqPnts(i).Coord(X, Y, Z);
            Vertical->AddVertex(X, Y, Z);
        }

        Handle(Graphic3d_AspectMarker3d) aMarkerAspect = new Graphic3d_AspectMarker3d(Aspect_TOM_POINT, myColor, 3.0);
        myGroup->SetGroupPrimitivesAspect(aMarkerAspect);
        myGroup->AddPrimitiveArray(Vertical, Standard_False);
    }

    myGroup->SetMinMaxValues(-myXSize, -myYSize, -myOffSet, myXSize, myYSize, -myOffSet);
    myCurXStep = aXStep, myCurYStep = aYStep;

    // update bounding box
    myStructure->CalculateBoundBox();
    myViewer->StructureManager()->Update(myStructure->GetZLayer());
}

void FITKV3d_RectangularGrid::GraphicValues(Standard_Real& theXSize, Standard_Real& theYSize, Standard_Real& theOffSet) const
{
    theXSize = myXSize;
    theYSize = myYSize;
    theOffSet = myOffSet;
}

void FITKV3d_RectangularGrid::SetGraphicValues(const Standard_Real theXSize, const Standard_Real theYSize, const Standard_Real theOffSet)
{
    if (!myCurAreDefined) {
        myXSize = theXSize;
        myYSize = theYSize;
        myOffSet = theOffSet;
    }
    if (myXSize != theXSize) {
        myXSize = theXSize;
        myCurAreDefined = Standard_False;
    }
    if (myYSize != theYSize) {
        myYSize = theYSize;
        myCurAreDefined = Standard_False;
    }
    if (myOffSet != theOffSet) {
        myOffSet = theOffSet;
        myCurAreDefined = Standard_False;
    }
    if (!myCurAreDefined) UpdateDisplay();
}
