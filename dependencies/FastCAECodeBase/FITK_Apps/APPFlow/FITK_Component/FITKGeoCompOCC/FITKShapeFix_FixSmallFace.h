/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKShapeFix_FixSmallFace.h
 * @brief       修复小面。
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2025-09-25
 *********************************************************************/

#ifndef _FITKShapeFix_FixSmallFace_HeaderFile
#define _FITKShapeFix_FixSmallFace_HeaderFile

#include <Standard.hxx>
#include <Standard_Type.hxx>

#include <TopoDS_Shape.hxx>
#include <Standard_Integer.hxx>
#include <ShapeAnalysis_CheckSmallFace.hxx>
#include <ShapeFix_Root.hxx>
#include <Standard_Boolean.hxx>
#include <Standard_Real.hxx>
class TopoDS_Shape;
class TopoDS_Face;
class TopoDS_Edge;
class TopoDS_Compound;

class FITKShapeFix_FixSmallFace;
DEFINE_STANDARD_HANDLE(FITKShapeFix_FixSmallFace, ShapeFix_Root)

//! Fixing face with small size
class FITKShapeFix_FixSmallFace : public ShapeFix_Root
{

public:

  
  Standard_EXPORT FITKShapeFix_FixSmallFace();
  
  Standard_EXPORT void Init (const TopoDS_Shape& S);
  
  Standard_EXPORT virtual void SetPrecision(const Standard_Real preci) Standard_OVERRIDE;

  //! Fixing case of spot face
  Standard_EXPORT void Perform();

  Standard_EXPORT void Update();
  
  //! Fixing case of spot face, if tol = -1 used local tolerance.
  Standard_EXPORT TopoDS_Shape FixSpotFace();

  Standard_EXPORT bool FixSpotFace(TopoDS_Face face);
  
  //! Compute average vertex and replacing vertices by new one.
  Standard_EXPORT Standard_Boolean ReplaceVerticesInCaseOfSpot (TopoDS_Face& F, const Standard_Real tol) const;
  
  //! Remove spot face from compound
  Standard_EXPORT Standard_Boolean RemoveFacesInCaseOfSpot (const TopoDS_Face& F) const;
  
  //! Fixing case of strip face, if tol = -1 used local tolerance
  Standard_EXPORT TopoDS_Shape FixStripFace (const Standard_Boolean wasdone = Standard_False);
  
  Standard_EXPORT bool FixStripFace(TopoDS_Face face);

  //! Replace veretces and edges.
  Standard_EXPORT Standard_Boolean ReplaceInCaseOfStrip (TopoDS_Face& F, TopoDS_Edge& E1, TopoDS_Edge& E2, const Standard_Real tol) const;
  
  //! Remove strip face from compound.
  Standard_EXPORT Standard_Boolean RemoveFacesInCaseOfStrip (const TopoDS_Face& F) const;
  
  //! Compute average edge for strip face
  Standard_EXPORT TopoDS_Edge ComputeSharedEdgeForStripFace (const TopoDS_Face& F, const TopoDS_Edge& E1, const TopoDS_Edge& E2, const TopoDS_Face& F1, const Standard_Real tol) const;
  
  Standard_EXPORT TopoDS_Shape FixSplitFace (const TopoDS_Shape& S);
  
  //! Compute data for face splitting.
  Standard_EXPORT Standard_Boolean SplitOneFace (TopoDS_Face& F, TopoDS_Compound& theSplittedFaces);
  
  Standard_EXPORT TopoDS_Face FixFace (const TopoDS_Face& F);
  
  Standard_EXPORT TopoDS_Shape FixShape();
  
  Standard_EXPORT TopoDS_Shape Shape();
  
  Standard_EXPORT Standard_Boolean FixPinFace (TopoDS_Face& F);




  DEFINE_STANDARD_RTTIEXT(FITKShapeFix_FixSmallFace,ShapeFix_Root)

protected:




private:


  TopoDS_Shape myShape;
  TopoDS_Shape myResult;
  Standard_Integer myStatus;
  ShapeAnalysis_CheckSmallFace myAnalyzer;


};







#endif // _FITKShapeFix_FixSmallFace_HeaderFile
