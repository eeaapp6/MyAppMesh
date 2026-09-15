/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCGeomTools.h"

// OCC
#include <TopoDS.hxx>
#include <BRep_Tool.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <gp_Pln.hxx>
#include <Geom_Surface.hxx>
#include <Geom_Circle.hxx>
#include <Geom_Ellipse.hxx>
#include <Geom_Hyperbola.hxx>
#include <Geom_Parabola.hxx>
#include <Geom_Line.hxx>
#include <Geom_BSplineCurve.hxx>
#include <Geom_BezierCurve.hxx>
#include <ShapeAnalysis_Surface.hxx>
#include <GProp_GProps.hxx>
#include <BRepGProp.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <GeomLib_Tool.hxx>
#include <BRep_CurveOnSurface.hxx>
#include <BRepExtrema_DistShapeShape.hxx>
#include <BRepLib_MakeVertex.hxx>
#include <Geom_Plane.hxx>
#include <Geom_CylindricalSurface.hxx>
#include <Geom_ConicalSurface.hxx>
#include <Geom_SphericalSurface.hxx>
#include <Geom_BSplineSurface.hxx>
#include <Geom_BezierSurface.hxx>
#include <Geom_ToroidalSurface.hxx>
#include <TopExp_Explorer.hxx>
#include <TopTools_IndexedDataMapOfShapeListOfShape.hxx>
#include <TopExp.hxx>
#include <BRepLProp_SLProps.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <BRepLProp_CLProps.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <BRepBndLib.hxx>
#include <BRepTools.hxx>
#include <gp_Hypr.hxx>
#include <gp_Circ.hxx>
#include <gp_Parab.hxx>
#include <gp_Elips.hxx>
#include <ShapeAnalysis_ShapeTolerance.hxx>
#include <ShapeAnalysis_Shell.hxx>
#include <ShapeAnalysis_Wire.hxx>
#include <ShapeAnalysis_Edge.hxx>
#include <ShapeAnalysis.hxx>
#include <TopoDS_Compound.hxx>
#include <TopoDS_Wire.hxx>
#include <Adaptor3d_TopolTool.hxx>
#include <Standard_Version.hxx>

#if OCC_VERSION_HEX < 0x070600
#include <GeomAdaptor_HSurface.hxx>
#else
#include <GeomAdaptor_Surface.hxx>
#endif

#include <BRepAlgoAPI_Check.hxx>
#include <BOPAlgo_ArgumentAnalyzer.hxx>
#include <BOPAlgo_ListOfCheckResult.hxx>
#include <GeomProjLib.hxx>
#include <BRepLib_MakeEdge.hxx>
#include <BRepAlgoAPI_Section.hxx>
#include <BRepBuilderAPI_Transform.hxx>

// Data
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
#include "FITK_Kernel/FITKCore/FITKCircleAlg.h"
#include "FITK_Kernel/FITKCore/FITKVec3D.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsVirtualTopo.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractModel.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractAssInstance.h"
#include "FITKOCCVirtualTopoCreator.h"
#include "FITKAbstractOCCModel.h"
#include "FITKOCCAlgTools.h"

// Qt 
#include <QDebug>

// C++
#include <cmath>

namespace OCC
{
    const TopoDS_Shape FITKOCCToolCreator::GetVirtualTopoShape(Interface::FITKAbsVirtualTopo* vtp, Interface::FITKGeoEnum::VTopoShapeType sType)
    {
        if (!vtp)
        {
            return TopoDS_Shape();
        }

        // Check the shape type.
        if (vtp->getShapeType() != sType)
        {
            return TopoDS_Shape();
        }

        FITKOCCTopoShape* vShape = vtp->getShapeT<FITKOCCTopoShape>();
        if (!vShape)
        {
            return TopoDS_Shape();
        }

        return vShape->getTopoShape();
    }

    const TopoDS_Shape FITKOCCToolCreator::GetVirtualTopoShape(Interface::FITKAbsVirtualTopo* vtp)
    {
        if (!vtp)
        {
            return TopoDS_Shape();
        }

        FITKOCCTopoShape* vShape = vtp->getShapeT<FITKOCCTopoShape>();
        if (!vShape)
        {
            return TopoDS_Shape();
        }

        return vShape->getTopoShape();
    }

    bool FITKOCCToolCreator::IsEqual(const gp_Ax1 & first, const gp_Ax1 & second)
    {
        bool isEqual = first.Location().IsEqual(second.Location(), Precision::Confusion());
        isEqual &= first.Direction().IsEqual(second.Direction(), Precision::Angular());
        return isEqual;
    }

    bool FITKOCCToolCreator::IsEqual(const gp_Ax3 & first, const gp_Ax3 & second)
    {
        bool isEqual = first.Location().IsEqual(second.Location(), Precision::Confusion());
        isEqual &= first.Direction().IsEqual(second.Direction(), Precision::Angular());
        isEqual &= first.XDirection().IsEqual(second.XDirection(), Precision::Angular());
        // isEqual &= first.YDirection().IsEqual(second.YDirection(), Precision::Angular());
        isEqual &= first.Direct() == second.Direct();
        return isEqual;
    }

    Interface::FITKAbstractGeomPointTool* FITKOCCToolCreator::createPointTool()
    {
        //创建点工具
        return new FITKOCCPointTool;
    }

    Interface::FITKAbstractGeomEdgeTool* FITKOCCToolCreator::createEdgeTool()
    {
        //创建边工具
        return new FITKOCCEdgeTool;
    }

    Interface::FITKAbstractGeomFaceTool* FITKOCCToolCreator::createFaceTool()
    {
        //创建平面工具
        return new FITKOCCFaceTool;
    }

    Interface::FITKAbstractGeomCommonTool* FITKOCCToolCreator::createCommonTool()
    {
        //创建通用工具
        return new FITKOCCCommonTool;
    }

    // 点查询工具。
    //@{
    bool FITKOCCPointTool::getXYZ(Interface::FITKAbsVirtualTopo* topo, double* xyz, int iModelId)
    {
        if (topo == nullptr || xyz == nullptr) return false;
        if (topo->getShapeType() != Interface::FITKGeoEnum::VSPoint) return false;
        //获取形状数据
        FITKOCCTopoShape* occShape = topo->getShapeT<FITKOCCTopoShape>();
        if (occShape == nullptr) return false;
        const TopoDS_Shape& shape = occShape->getTopoShape();
        //获取坐标
        TopoDS_Vertex vertex = TopoDS::Vertex(shape);
        gp_Pnt pt = BRep_Tool::Pnt(vertex);
        xyz[0] = pt.X(); xyz[1] = pt.Y(); xyz[2] = pt.Z();

        // Try to transform the point.
        Interface::FITKAbstractModel* model = FITKDATAREPO->getTDataByID<Interface::FITKAbstractModel>(iModelId);
        if (model)
        {
            model->transformPoint(xyz, xyz);
        }

        return true;
    }
    //@}

    // 边查询工具。
    //@{
    bool FITKOCCEdgeTool::getStartPointDirection(Interface::FITKAbsVirtualTopo* vtp, double* oPos, double* oDir, int iModelId)
    {
        if (!vtp)
        {
            return false;
        }

        // 检查拓扑类型。
        if (vtp->getShapeType() != Interface::FITKGeoEnum::VSEdge)
        {
            return false;
        }

        // 获取形状。
        FITKOCCTopoShape* vShape = vtp->getShapeT<FITKOCCTopoShape>();
        if (!vShape)
        {
            return false;
        }

        const TopoDS_Edge & edge = TopoDS::Edge(vShape->getTopoShape());
        if (edge.IsNull())
        {
            return false;
        }

        Standard_Real first, last;
        Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, first, last);
        if (!curve)
        {
            return false;
        }

        gp_Pnt startPt;
        gp_Vec startDir;
        curve->D1(first, startPt, startDir);

        // 翻转。
        if (edge.Orientation() == TopAbs_Orientation::TopAbs_REVERSED)
        {
            startDir.Reverse();
        }

        for (int i = 0; i < 3; i++)
        {
            oPos[i] = startPt.Coord(i + 1);
            oDir[i] = startDir.Coord(i + 1);
        }

        // 变换坐标与方向。
        Interface::FITKAbstractModel* model = FITKDATAREPO->getTDataByID<Interface::FITKAbstractModel>(iModelId);
        if (model)
        {
            model->transformDirection(oDir, oDir);
            model->transformPoint(oPos, oPos);
        }

        return true;
    }

    bool FITKOCCEdgeTool::getEndPointDirection(Interface::FITKAbsVirtualTopo* vtp, double* oPos, double* oDir, int iModelId)
    {
        if (!vtp)
        {
            return false;
        }

        // 检查拓扑类型。
        if (vtp->getShapeType() != Interface::FITKGeoEnum::VSEdge)
        {
            return false;
        }

        // 获取形状。
        FITKOCCTopoShape* vShape = vtp->getShapeT<FITKOCCTopoShape>();
        if (!vShape)
        {
            return false;
        }

        const TopoDS_Edge & edge = TopoDS::Edge(vShape->getTopoShape());
        if (edge.IsNull())
        {
            return false;
        }

        Standard_Real first, last;
        Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, first, last);
        if (!curve)
        {
            return false;
        }

        gp_Pnt lastPt;
        gp_Vec lastDir;
        curve->D1(last, lastPt, lastDir);

        // 翻转。
        if (edge.Orientation() == TopAbs_Orientation::TopAbs_REVERSED)
        {
            lastDir.Reverse();
        }

        for (int i = 0; i < 3; i++)
        {
            oPos[i] = lastPt.Coord(i + 1);
            oDir[i] = lastDir.Coord(i + 1);
        }

        // 变换坐标与方向。
        Interface::FITKAbstractModel* model = FITKDATAREPO->getTDataByID<Interface::FITKAbstractModel>(iModelId);
        if (model)
        {
            model->transformDirection(oDir, oDir);
            model->transformPoint(oPos, oPos);
        }

        return true;
    }

    bool FITKOCCEdgeTool::getDirection(Interface::FITKAbsVirtualTopo* vtp, double* iPos, double* oDir, int iModelId)
    {
        if (!vtp)
        {
            return false;
        }

        // 检查拓扑类型。
        if (vtp->getShapeType() != Interface::FITKGeoEnum::VSEdge)
        {
            return false;
        }

        // 获取形状。
        FITKOCCTopoShape* vShape = vtp->getShapeT<FITKOCCTopoShape>();
        if (!vShape)
        {
            return false;
        }

        const TopoDS_Edge & edge = TopoDS::Edge(vShape->getTopoShape());
        if (edge.IsNull())
        {
            return false;
        }

        Standard_Real first, last;
        Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, first, last);
        if (!curve)
        {
            return false;
        }

        gp_Pnt pt(iPos[0], iPos[1], iPos[2]);
        Standard_Real u;
        bool isOnEdge = GeomLib_Tool::Parameter(curve, pt, Precision::Confusion(), u);
        if (!isOnEdge)
        {
            return false;
        }

        if (u < first || u > last)
        {
            return false;
        }

        gp_Vec dir;
        curve->D1(u, pt, dir);

        // 翻转。
        if (edge.Orientation() == TopAbs_Orientation::TopAbs_REVERSED)
        {
            dir.Reverse();
        }

        for (int i = 0; i < 3; i++)
        {
            oDir[i] = dir.Coord(i + 1);
        }

        // 变换坐标与方向。
        Interface::FITKAbstractModel* model = FITKDATAREPO->getTDataByID<Interface::FITKAbstractModel>(iModelId);
        if (model)
        {
            model->transformDirection(oDir, oDir);
        }

        return true;
    }

    bool FITKOCCEdgeTool::getCoedgeDirection(Interface::FITKAbsVirtualTopo* vtpFace, Interface::FITKAbsVirtualTopo* vtpEdge,
        double* iPos, double* oDir, int iModelId)
    {
        Q_UNUSED(vtpFace);
        return getDirection(vtpEdge, iPos, oDir, iModelId);

        //if (!vtpFace || !vtpEdge || !iPos || !oDir)
        //{
        //    return false;
        //}

        //// 获取抽象形状。
        //FITKOCCTopoShape* occShapeFace = vtpFace->getShapeT<FITKOCCTopoShape>();
        //FITKOCCTopoShape* occShapeEdge = vtpEdge->getShapeT<FITKOCCTopoShape>();
        //if (!occShapeFace || !occShapeEdge)
        //{
        //    return false;
        //}

        //// 获取面与边。
        //const TopoDS_Shape & shapeFace = occShapeFace->getTopoShape();
        //const TopoDS_Shape & shapeEdge = occShapeEdge->getTopoShape();
        //const TopoDS_Face & face = TopoDS::Face(shapeFace);
        //const TopoDS_Edge & edge = TopoDS::Edge(shapeEdge);
        //if (face.IsNull() || edge.IsNull())
        //{
        //    return false;
        //}

        //Handle(Geom_Surface) surface = BRep_Tool::Surface(face);

        //Standard_Real first, last;
        //Handle(Geom2d_Curve) curve2d = BRep_Tool::CurveOnSurface(edge, face, first, last);
        //if (!curve2d || !surface)
        //{
        //    return false;
        //}

        //BRep_CurveOnSurface cos(curve2d, surface, TopLoc_Location());
        //Handle(Geom_Curve) curve = cos.Curve3D();
        //if (!curve)
        //{
        //    return false;
        //}

        //gp_Pnt pt(iPos[0], iPos[1], iPos[2]);
        //Standard_Real u;
        //bool isOnEdge = GeomLib_Tool::Parameter(curve, pt, Precision::Confusion(), u);
        //if (!isOnEdge)
        //{
        //    return false;
        //}

        //if (u < first || u > last)
        //{
        //    return false;
        //}

        //gp_Vec dir;
        //curve->D1(u, pt, dir);

        //// 翻转。
        //if (edge.Orientation() == TopAbs_Orientation::TopAbs_REVERSED)
        //{
        //    dir.Reverse();
        //}

        //for (int i = 0; i < 3; i++)
        //{
        //    oDir[i] = dir.Coord(i + 1);
        //}

        //// 变换坐标与方向。
        //Interface::FITKAbstractModel* model = FITKDATAREPO->getTDataByID<Interface::FITKAbstractModel>(iModelId);
        //if (model)
        //{
        //    model->transformDirection(oDir, oDir);
        //}

        //return true;
    }

    bool FITKOCCEdgeTool::getBoundaryNormal(Interface::FITKAbsVirtualTopo* vtpFace, Interface::FITKAbsVirtualTopo* vtpEdge,
        double* iPos, double* oDir, int iModelId)
    {
        if (!vtpFace || !vtpEdge)
        {
            return false;
        }

        // 获取抽象形状。
        FITKOCCTopoShape* occShapeFace = vtpFace->getShapeT<FITKOCCTopoShape>();
        FITKOCCTopoShape* occShapeEdge = vtpEdge->getShapeT<FITKOCCTopoShape>();
        if (!occShapeFace || !occShapeEdge)
        {
            return false;
        }

        // 获取面与边。
        const TopoDS_Shape & shapeFace = occShapeFace->getTopoShape();
        const TopoDS_Shape & shapeEdge = occShapeEdge->getTopoShape();
        const TopoDS_Face & face = TopoDS::Face(shapeFace);
        const TopoDS_Edge & edge = TopoDS::Edge(shapeEdge);
        if (face.IsNull() || edge.IsNull())
        {
            return false;
        }

        // 获取曲面与曲线。
        Handle(Geom_Surface) surface = BRep_Tool::Surface(face);

        Standard_Real first, last;
        Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, first, last);

        if (!surface || !curve)
        {
            return false;
        }
    
        gp_Pnt pnt(iPos[0], iPos[1], iPos[2]);

        // 获取给定位置在曲线上的参数值。
        Standard_Real t;
        GeomLib_Tool::Parameter(curve, pnt, Precision::Confusion(), t);

        // 获取给定位置在表面上的参数值。
        Standard_Real u, v;
        GeomLib_Tool::Parameters(surface, pnt, Precision::Confusion(), u, v);

        // 获取给定坐标在面上的法向。
        gp_Vec du, dv;
        surface->D1(u, v, pnt, du, dv);

        // 获取曲线切向。
        gp_Vec vec;
        curve->D1(t, pnt, vec);
        vec *= shapeEdge.Orientation() == TopAbs_Orientation::TopAbs_REVERSED ? -1. : 1.;
        vec.Normalize();

        // 叉积取法向。
        gp_Vec nor = du.Crossed(dv);
        if (nor.SquareMagnitude() < gp::Resolution())
        {
            return false;
        }

        nor.Normalize();

        // 计算方向。
        Standard_Real dot = vec.Dot(nor);
        gp_Vec cross = vec.Crossed(nor);
        gp_Vec dir = dot * vec + cross;

        // 获取线中点位置在曲线上的参数值。
        gp_Pnt pntMid;
        Standard_Real tMid = first + (last - first) * 0.5;
        GeomLib_Tool::Parameter(curve, pntMid, Precision::Confusion(), tMid);

        // 获取线中点位置在表面上的参数值。
        Standard_Real uMid, vMid;
        GeomLib_Tool::Parameters(surface, pntMid, Precision::Confusion(), uMid, vMid);

        // 获取线中点坐标在面上的法向。
        gp_Vec duMid, dvMid;
        surface->D1(uMid, vMid, pntMid, duMid, dvMid);

        // 获取线中点曲线切向。
        gp_Vec vecMid;
        curve->D1(tMid, pntMid, vecMid);
        vecMid *= shapeEdge.Orientation() == TopAbs_Orientation::TopAbs_REVERSED ? -1. : 1.;
        vecMid.Normalize();

        // 叉积取法向。
        gp_Vec norMid = duMid.Crossed(dvMid);
        if (norMid.SquareMagnitude() < gp::Resolution())
        {
            return false;
        }

        norMid.Normalize();

        // 计算方向。
        Standard_Real dotMid = vecMid.Dot(norMid);
        gp_Vec crossMid = vecMid.Crossed(norMid);
        gp_Vec dirMid = dotMid * vecMid + crossMid;

        // 从中点位置创建直线并投影到面上。
        const TopoDS_Edge & edgeMidNor =  BRepLib_MakeEdge(gp_Lin(pntMid, norMid));

        Standard_Real firstMidNor, lastMidNor;
        Handle(Geom_Curve) curveMidNor = BRep_Tool::Curve(edgeMidNor, firstMidNor, lastMidNor);
        Handle(Geom_Curve) curveMidNorProj = GeomProjLib::Project(curveMidNor, surface);
        if (!curveMidNorProj)
        {
            return false;
        }

        Standard_Real tMidNor;
        GeomLib_Tool::Parameter(curveMidNorProj, pntMid, Precision::Confusion(), tMidNor);

        // 调整方向。
        gp_Pnt testPntMid;
        curveMidNorProj->D0(tMidNor + 0.0001, testPntMid);

        Standard_Real tempU, tempV;
        bool isOnSurf = GeomLib_Tool::Parameters(surface, testPntMid, Precision::Confusion(), tempU, tempV);
        if (!isOnSurf)
        {
            dir.Reverse();
        }

        for (int i = 0; i < 3; i++)
        {
            oDir[i] = dir.Coord(i + 1);
        }

        // 变换方向。
        Interface::FITKAbstractModel* model = FITKDATAREPO->getTDataByID<Interface::FITKAbstractModel>(iModelId);
        if (model)
        {
            model->transformDirection(oDir, oDir);
        }

        return true;
    }

    bool FITKOCCEdgeTool::isPosOnEdge(Interface::FITKAbsVirtualTopo* vtp, double* iPos)
    {
        if (!vtp)
        {
            return false;
        }

        // 检查拓扑类型。
        if (vtp->getShapeType() != Interface::FITKGeoEnum::VSEdge)
        {
            return false;
        }

        // 获取形状。
        FITKOCCTopoShape* vShape = vtp->getShapeT<FITKOCCTopoShape>();
        if (!vShape)
        {
            return false;
        }

        const TopoDS_Edge & edge = TopoDS::Edge(vShape->getTopoShape());
        if (edge.IsNull())
        {
            return false;
        }

        Standard_Real first, last;
        Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, first, last);
        if (!curve)
        {
            return false;
        }

        gp_Pnt pt(iPos[0], iPos[1], iPos[2]);
        Standard_Real u;
        bool isOnEdge = GeomLib_Tool::Parameter(curve, pt, Precision::Confusion(), u);
        if (!isOnEdge)
        {
            return false;
        }

        if (u < first || u > last)
        {
            return false;
        }

        return true;
    }

    bool FITKOCCEdgeTool::isLine(Interface::FITKAbsVirtualTopo* vtp)
    {
        if (!vtp)
        {
            return false;
        }

        // 检查拓扑类型。
        if (vtp->getShapeType() != Interface::FITKGeoEnum::VSEdge)
        {
            return false;
        }

        // 获取形状。
        FITKOCCTopoShape* vShape = vtp->getShapeT<FITKOCCTopoShape>();
        if (!vShape)
        {
            return false;
        }

        const TopoDS_Edge & edge = TopoDS::Edge(vShape->getTopoShape());
        if (edge.IsNull())
        {
            return false;
        }
         
        BRepAdaptor_Curve baCurve(edge);
        return baCurve.GetType() == GeomAbs_CurveType::GeomAbs_Line;
    }

    bool FITKOCCEdgeTool::getLength(Interface::FITKAbsVirtualTopo* vtp, double & oLen)
    {
        if (!vtp)
        {
            return false;
        }

        // 检查拓扑类型。
        if (vtp->getShapeType() != Interface::FITKGeoEnum::VSEdge)
        {
            return false;
        }

        // 获取形状。
        FITKOCCTopoShape* vShape = vtp->getShapeT<FITKOCCTopoShape>();
        if (!vShape)
        {
            return false;
        }

        const TopoDS_Edge & edge = TopoDS::Edge(vShape->getTopoShape());
        if (edge.IsNull())
        {
            return false;
        }

        // 获取长度。
        GProp_GProps props;
        BRepGProp::LinearProperties(edge, props);
        oLen = props.Mass();

        return true;
    }

    bool FITKOCCEdgeTool::getMidPoint(Interface::FITKAbsVirtualTopo* vtp, double* oPos, int iModelId)
    {
        if (!vtp)
        {
            return false;
        }

        // 检查拓扑类型。
        if (vtp->getShapeType() != Interface::FITKGeoEnum::VSEdge)
        {
            return false;
        }

        // 获取形状。
        FITKOCCTopoShape* vShape = vtp->getShapeT<FITKOCCTopoShape>();
        if (!vShape)
        {
            return false;
        }

        const TopoDS_Edge & edge = TopoDS::Edge(vShape->getTopoShape());
        if (edge.IsNull())
        {
            return false;
        }

        // 获取曲线。
        Standard_Real first, last;
        Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, first, last);
        if (!curve)
        {
            return false;
        }

        gp_Pnt midPt;
        curve->D0(first + (last - first) * 0.5, midPt);
        oPos[0] = midPt.Coord(1);
        oPos[1] = midPt.Coord(2);
        oPos[2] = midPt.Coord(3);

        // 变换坐标。
        Interface::FITKAbstractModel* model = FITKDATAREPO->getTDataByID<Interface::FITKAbstractModel>(iModelId);
        if (model)
        {
            model->transformPoint(oPos, oPos);
        }

        return true;
    }

    bool FITKOCCEdgeTool::getMidPointDirection(Interface::FITKAbsVirtualTopo* vtp, double* oPos, double* oDir, int iModelId)
    {
        if (!vtp)
        {
            return false;
        }

        // 检查拓扑类型。
        if (vtp->getShapeType() != Interface::FITKGeoEnum::VSEdge)
        {
            return false;
        }

        // 获取形状。
        FITKOCCTopoShape* vShape = vtp->getShapeT<FITKOCCTopoShape>();
        if (!vShape)
        {
            return false;
        }

        const TopoDS_Edge & edge = TopoDS::Edge(vShape->getTopoShape());
        if (edge.IsNull())
        {
            return false;
        }

        // 获取曲线。
        Standard_Real first, last;
        Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, first, last);
        if (!curve)
        {
            return false;
        }

        gp_Pnt midPt;
        gp_Vec dir;
        curve->D1(first + (last - first) * 0.5, midPt, dir);
        oPos[0] = midPt.Coord(1);
        oPos[1] = midPt.Coord(2);
        oPos[2] = midPt.Coord(3);

        oDir[0] = dir.Coord(1);
        oDir[1] = dir.Coord(2);
        oDir[2] = dir.Coord(3);

        // 变换坐标与方向。
        Interface::FITKAbstractModel* model = FITKDATAREPO->getTDataByID<Interface::FITKAbstractModel>(iModelId);
        if (model)
        {
            model->transformPoint(oPos, oPos);
            model->transformDirection(oDir, oDir);
        }

        return true;
    }

    bool FITKOCCEdgeTool::getFeaturePoints(Interface::FITKAbsVirtualTopo* vtp, QList<QList<double>> & oPoses, bool skipMidPt, int iModelId)
    {
        if (!vtp)
        {
            return false;
        }

        // 检查拓扑类型。
        if (vtp->getShapeType() != Interface::FITKGeoEnum::VSEdge)
        {
            return false;
        }

        // 获取形状。
        FITKOCCTopoShape* vShape = vtp->getShapeT<FITKOCCTopoShape>();
        if (!vShape)
        {
            return false;
        }

        const TopoDS_Edge & edge = TopoDS::Edge(vShape->getTopoShape());
        if (edge.IsNull())
        {
            return false;
        }

        // 获取曲线。
        Standard_Real first, last;
        Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, first, last);
        if (!curve)
        {
            return false;
        }

        Interface::FITKAbstractModel* model = FITKDATAREPO->getTDataByID<Interface::FITKAbstractModel>(iModelId);

        double pos3[3]{ 0., 0., 0. };

        if (!skipMidPt)
        {
            // 获取中点。
            gp_Pnt midPt;
            curve->D0(first + (last - first) * 0.5, midPt);
            pos3[0] = midPt.Coord(1);
            pos3[1] = midPt.Coord(2);
            pos3[2] = midPt.Coord(3);

            // 变换坐标。
            if (model)
            {
                model->transformPoint(pos3, pos3);
            }

            // 保存坐标。
            oPoses.push_back(QList<double>{ pos3[0], pos3[1], pos3[2] });
        }

        // 获取各类型曲线特征点。
        BRepAdaptor_Curve baCurve(edge);
        GeomAbs_CurveType curveType = baCurve.GetType();
        switch (curveType)
        {
        case GeomAbs_Circle:
        case GeomAbs_Ellipse:
        {
            Handle(Geom_Conic) conic = Handle(Geom_Conic)::DownCast(curve);
            if (!conic)
            {
                break;
            }

            // 获取(椭)圆心坐标。
            const gp_Pnt & center = conic->Position().Location();
            pos3[0] = center.Coord(1);
            pos3[1] = center.Coord(2);
            pos3[2] = center.Coord(3);

            // 变换坐标。
            if (model)
            {
                model->transformPoint(pos3, pos3);
            }

            // 保存坐标。
            oPoses.push_back(QList<double>{ pos3[0], pos3[1], pos3[2] });

            break;
        }
        case GeomAbs_Hyperbola:
            break;
        case GeomAbs_Parabola:
            break;
        case GeomAbs_BezierCurve:
            break;
        case GeomAbs_BSplineCurve:
            break;
        case GeomAbs_Line:
        case GeomAbs_OffsetCurve:
        case GeomAbs_OtherCurve:
        default:
            break;
        }

        return true;
    }

    bool FITKOCCEdgeTool::evalPoint(Interface::FITKAbsVirtualTopo* vtp, double iParam, double* oPos, bool iNormalized, int iModelId)
    {
        if (!vtp)
        {
            return false;
        }

        // 检查拓扑类型。
        if (vtp->getShapeType() != Interface::FITKGeoEnum::VSEdge)
        {
            return false;
        }

        // 获取形状。
        FITKOCCTopoShape* vShape = vtp->getShapeT<FITKOCCTopoShape>();
        if (!vShape)
        {
            return false;
        }

        const TopoDS_Edge & edge = TopoDS::Edge(vShape->getTopoShape());
        if (edge.IsNull())
        {
            return false;
        }

        // 获取曲线。
        Standard_Real first, last;
        Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, first, last);
        if (!curve)
        {
            return false;
        }

        // 参数值归一化。
        if (iNormalized)
        {
            if (iParam < 0 || iParam > 1)
            {
                return false;
            }

            iParam = first + (last - first) * iParam;
        }

        // 获取坐标。
        gp_Pnt pt;
        curve->D0(iParam, pt);
        oPos[0] = pt.Coord(1);
        oPos[1] = pt.Coord(2);
        oPos[2] = pt.Coord(3);

        // 变换坐标。
        Interface::FITKAbstractModel* model = FITKDATAREPO->getTDataByID<Interface::FITKAbstractModel>(iModelId);
        if (model)
        {
            model->transformPoint(oPos, oPos);
        }

        return true;
    }
    
    QList<int> FITKOCCEdgeTool::getFreeEdgeIDs(int commandId)
    {
        QList<int> freeEdgeIDs;
        Interface::FITKAbsGeoCommand* command = FITKDATAREPO->getTDataByID<Interface::FITKAbsGeoCommand>(commandId);
        if (command == nullptr)
            return freeEdgeIDs;
    
        Interface::FITKShapeVirtualTopoManager* edges = command->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VTopoShapeType::VSEdge);
        if(edges == nullptr)
            return freeEdgeIDs;
        
        const int edgeCount = edges->getDataCount();
        for (int i = 0; i < edgeCount; i++)
        {
            Interface::FITKAbsVirtualTopo* edge = edges->getDataByIndex(i);
            if (edge == nullptr)
                continue;
            if (edge->getShapeType() != Interface::FITKGeoEnum::VSEdge)
                continue;

            FITKOCCTopoShape* accEdge = edge->getShapeT<FITKOCCTopoShape>(); 
            if (accEdge == nullptr)
                continue;
            const TopoDS_Shape & shape = accEdge->getTopoShape();

            //shape相关联的面
            TopTools_IndexedDataMapOfShapeListOfShape edgeFaceMap;
            TopExp::MapShapesAndAncestors(shape, TopAbs_EDGE, TopAbs_FACE, edgeFaceMap);
            const int edgeCount = edges->getDataCount();

            if(edgeFaceMap.Extent() <2)
            {
                //没有面关联
                freeEdgeIDs.push_back(edge->getDataObjectID());
            }
        }
        
        return freeEdgeIDs;
        
    }

    QList<int> FITKOCCEdgeTool::getEdgesOnPlane(int cmdId, double* iOrg, double* iNor)
    {
        QList<int> edgeIds;
        Interface::FITKAbsGeoCommand* cmd = FITKDATAREPO->getTDataByID<Interface::FITKAbsGeoCommand>(cmdId);
        if (!cmd)
        {
            return edgeIds;
        }

        Interface::FITKShapeVirtualTopoManager* vEdgeMgr = cmd->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VSEdge);
        if (!vEdgeMgr)
        {
            return edgeIds;
        }

        double tolConf = Precision::Confusion();
        double tolAng = Precision::Angular();
        //double tolConf = 1e-3;
        //double tolAng = 1e-3;

        try
        {
            // 归一化。
            double uDir[3];
            double den = (sqrt(pow(iNor[0], 2) + pow(iNor[1], 2) + pow(iNor[2], 2)));
            if (den != 0.)
            {
                for (int i = 0; i < 3; ++i)
                {
                    uDir[i] = iNor[i] / den;
                }
            }

            // 创建平面。
            gp_Dir pDir(uDir[0], uDir[1], uDir[2]);
            gp_Pln pln(gp_Pnt(iOrg[0], iOrg[1], iOrg[2]), pDir);

            double pos1[3]{ 0., 0., 0. };
            double pos2[3]{ 0., 0., 0. };

            int nEdges = vEdgeMgr->getDataCount();
            for (int i = 0; i < nEdges; i++)
            {
                Interface::FITKAbsVirtualTopo* vEdge = vEdgeMgr->getDataByIndex(i);
                if (!vEdge)
                {
                    continue;
                }

                // 检查拓扑类型。
                if (vEdge->getShapeType() != Interface::FITKGeoEnum::VSEdge)
                {
                    continue;
                }

                // 获取形状。
                FITKOCCTopoShape* vShape = vEdge->getShapeT<FITKOCCTopoShape>();
                if (!vShape)
                {
                    continue;
                }

                const TopoDS_Edge & edge = TopoDS::Edge(vShape->getTopoShape());
                if (edge.IsNull())
                {
                    continue;
                }

                Standard_Real first, last;
                Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, first, last);
                if (!curve)
                {
                    continue;
                }

                // 获取各类型曲线特征点。
                BRepAdaptor_Curve baCurve(edge);
                //if (baCurve.Is3DCurve())
                //{
                //    continue;
                //}

                GeomAbs_CurveType curveType = baCurve.GetType();

                switch (curveType)
                {
                case GeomAbs_Line:
                {
                    //gp_Lin lin = baCurve.Line();

                    gp_Pnt start, end;
                    curve->D0(first, start);
                    curve->D0(last, end);

                    double dist1 = qAbs(pln.Distance(start));
                    double dist2 = qAbs(pln.Distance(end));
                    if (dist1 <= tolConf && dist2 <= tolConf)
                    {
                        int vEdgeId = vEdge->getDataObjectID();
                        edgeIds.push_back(vEdgeId);
                    }

                    break;
                }
                case GeomAbs_Circle:
                {
                    gp_Circ circ = baCurve.Circle();

                    // 获取圆心坐标与法向。
                    const gp_Pnt & center = circ.Position().Location();
                    const gp_Dir & dir = circ.Position().Direction();

                    double dist = qAbs(pln.Distance(center));
                    if (pDir.IsParallel(dir, tolAng) && dist <= tolConf)
                    {
                        int vEdgeId = vEdge->getDataObjectID();
                        edgeIds.push_back(vEdgeId);
                    }

                    break;
                }
                case GeomAbs_Ellipse:
                {
                    gp_Elips elli = baCurve.Ellipse();

                    // 获取圆心坐标与法向。
                    const gp_Pnt & center = elli.Position().Location();
                    const gp_Dir & dir = elli.Position().Direction();

                    double dist = qAbs(pln.Distance(center));
                    if (pDir.IsParallel(dir, tolAng) && dist <= tolConf)
                    {
                        int vEdgeId = vEdge->getDataObjectID();
                        edgeIds.push_back(vEdgeId);
                    }

                    break;
                }
                case GeomAbs_Hyperbola:
                {
                    gp_Hypr hyperB = baCurve.Hyperbola();

                    const gp_Pnt & loc = hyperB.Position().Location();
                    const gp_Dir & dir = hyperB.Position().Direction();

                    double dist = qAbs(pln.Distance(loc));
                    if (pDir.IsParallel(dir, tolAng) && dist <= tolConf)
                    {
                        int vEdgeId = vEdge->getDataObjectID();
                        edgeIds.push_back(vEdgeId);
                    }

                    break;
                }
                case GeomAbs_Parabola:
                {
                    gp_Parab paraB = baCurve.Parabola();

                    const gp_Pnt & loc = paraB.Position().Location();
                    const gp_Dir & dir = paraB.Position().Direction();

                    double dist = qAbs(pln.Distance(loc));
                    if (pDir.IsParallel(dir, tolAng) && dist <= tolConf)
                    {
                        int vEdgeId = vEdge->getDataObjectID();
                        edgeIds.push_back(vEdgeId);
                    }

                    break;
                }
                case GeomAbs_BezierCurve:
                {
                    Handle(Geom_BezierCurve) bzr = baCurve.Bezier();
                    if (!bzr)
                    {
                        break;
                    }

                    bool isPtsOnPln = true;
                    int nPts = bzr->NbPoles();
                    for (int j = 1; j <= nPts; j++)
                    {
                        const gp_Pnt & pt = bzr->Pole(j);
                        double dist = qAbs(pln.Distance(pt));
                        isPtsOnPln &= dist <= tolConf;
                    }

                    if (isPtsOnPln)
                    {
                        int vEdgeId = vEdge->getDataObjectID();
                        edgeIds.push_back(vEdgeId);
                    }

                    break;
                }
                case GeomAbs_BSplineCurve:
                {
                    Handle(Geom_BSplineCurve) bspl = baCurve.BSpline();
                    if (!bspl)
                    {
                        break;
                    }

                    bool isPtsOnPln = true;
                    int nPts = bspl->NbPoles();
                    for (int j = 1; j <= nPts; j++)
                    {
                        const gp_Pnt & pt = bspl->Pole(j);
                        double dist = qAbs(pln.Distance(pt));
                        isPtsOnPln &= dist <= tolConf;
                    }

                    if (isPtsOnPln)
                    {
                        int vEdgeId = vEdge->getDataObjectID();
                        edgeIds.push_back(vEdgeId);
                    }

                    break;
                }
                case GeomAbs_OffsetCurve:
                case GeomAbs_OtherCurve:
                default:
                {
                    qDebug() << "Unsupported curve type.";
                    break;
                }
                }

                //switch (curveType)
                //{
                //case GeomAbs_Circle:
                //case GeomAbs_Ellipse:
                //{
                //    Handle(Geom_Conic) conic = Handle(Geom_Conic)::DownCast(curve);
                //    if (!conic)
                //    {
                //        break;
                //    }

                //    // 获取(椭)圆心坐标与法向。
                //    const gp_Pnt & center = conic->Position().Location();
                //    const gp_Dir & dir = conic->Position().Direction();

                //    double dist = qAbs(pln.Distance(center));
                //    if (pDir.IsParallel(dir, tolAng) && dist <= tolConf)
                //    {
                //        int vEdgeId = vEdge->getDataObjectID();
                //        edgeIds.push_back(vEdgeId);
                //    }

                //    break;
                //}
                //case GeomAbs_Hyperbola:
                //{
                //    Handle(Geom_Hyperbola) hyperb = Handle(Geom_Hyperbola)::DownCast(curve);
                //    if (!hyperb)
                //    {
                //        break;
                //    }

                //    const gp_Pnt & loc = hyperb->Hypr().Position().Location();
                //    const gp_Dir & dir = hyperb->Hypr().Position().Direction();

                //    double dist = qAbs(pln.Distance(loc));
                //    if (pDir.IsParallel(dir, tolAng) && dist <= tolConf)
                //    {
                //        int vEdgeId = vEdge->getDataObjectID();
                //        edgeIds.push_back(vEdgeId);
                //    }

                //    break;
                //}
                //case GeomAbs_Parabola:
                //{
                //    Handle(Geom_Parabola) parab = Handle(Geom_Parabola)::DownCast(curve);
                //    if (!parab)
                //    {
                //        break;
                //    }

                //    const gp_Pnt & loc = parab->Parab().Position().Location();
                //    const gp_Dir & dir = parab->Parab().Position().Direction();

                //    double dist = qAbs(pln.Distance(loc));
                //    if (pDir.IsParallel(dir, tolAng) && dist <= tolConf)
                //    {
                //        int vEdgeId = vEdge->getDataObjectID();
                //        edgeIds.push_back(vEdgeId);
                //    }

                //    break;
                //}
                //case GeomAbs_BezierCurve:
                //{
                //    Handle(Geom_BezierCurve) bzr = Handle(Geom_BezierCurve)::DownCast(curve);
                //    if (!bzr)
                //    {
                //        break;
                //    }

                //    bool isPtsOnPln = true;
                //    int nPts = bzr->NbPoles();
                //    for (int j = 1; j <= nPts; j++)
                //    {
                //        const gp_Pnt & pt = bzr->Pole(j);
                //        double dist = qAbs(pln.Distance(pt));
                //        isPtsOnPln &= dist <= tolConf;
                //    }

                //    if (isPtsOnPln)
                //    {
                //        int vEdgeId = vEdge->getDataObjectID();
                //        edgeIds.push_back(vEdgeId);
                //    }
                //    
                //    break;
                //}
                //case GeomAbs_BSplineCurve:
                //{
                //    Handle(Geom_BSplineCurve) bspl = Handle(Geom_BSplineCurve)::DownCast(curve);
                //    if (!bspl)
                //    {
                //        break;
                //    }

                //    bool isPtsOnPln = true;
                //    int nPts = bspl->NbPoles();
                //    for (int j = 1; j <= nPts; j++)
                //    {
                //        const gp_Pnt & pt = bspl->Pole(j);
                //        double dist = qAbs(pln.Distance(pt));
                //        isPtsOnPln &= dist <= tolConf;
                //    }

                //    if (isPtsOnPln)
                //    {
                //        int vEdgeId = vEdge->getDataObjectID();
                //        edgeIds.push_back(vEdgeId);
                //    }

                //    break;
                //}
                //case GeomAbs_Line:
                //{
                //    //Handle(Geom_Line) line = Handle(Geom_Line)::DownCast(curve);
                //    //if (!line)
                //    //{
                //    //    break;
                //    //}

                //    gp_Pnt start, end;
                //    curve->D0(first, start);
                //    curve->D0(last, end);

                //    double dist1 = qAbs(pln.Distance(start));
                //    double dist2 = qAbs(pln.Distance(end));
                //    if (dist1 <= tolConf && dist2 <= tolConf)
                //    {
                //        int vEdgeId = vEdge->getDataObjectID();
                //        edgeIds.push_back(vEdgeId);
                //    }

                //    break;
                //}
                //case GeomAbs_OffsetCurve:
                //case GeomAbs_OtherCurve:
                //default:
                //{
                //    qDebug() << "Unsupported curve type.";
                //    break;
                //}
                //}

                //// 获取包围盒顶点。
                //Bnd_Box box;
                //BRepBndLib::AddClose(edge, box);
                //box.Get(pos1[0], pos1[1], pos1[2], pos2[0], pos2[1], pos2[2]);

                //double dist1 = qAbs(pln.Distance(gp_Pnt(pos1[0], pos1[1], pos1[2])));
                //double dist2 = qAbs(pln.Distance(gp_Pnt(pos2[0], pos2[1], pos2[2])));
                //if (dist1 <= tolConf && dist2 <= tolConf)
                //{
                //    int vEdgeId = vEdge->getDataObjectID();
                //    edgeIds.push_back(vEdgeId);
                //}
            }
        }
        catch (...)
        {
            edgeIds.clear();
            return edgeIds;
        }

        return edgeIds;
    }
    //@}

    // 面查询工具。
    //@{
    bool FITKOCCFaceTool::getArea(Interface::FITKAbsVirtualTopo* vtp, double & oArea)
    {
        if (!vtp)
        {
            return false;
        }

        // 获取抽象形状。
        FITKOCCTopoShape* occShape = vtp->getShapeT<FITKOCCTopoShape>();
        if (!occShape)
        {
            return false;
        }

        // 获取面。
        const TopoDS_Shape & shape = occShape->getTopoShape();
        const TopoDS_Face & face = TopoDS::Face(shape);
        if (face.IsNull())
        {
            return false;
        }

        GProp_GProps props;
        BRepGProp::SurfaceProperties(face, props);

        oArea = props.Mass();

        return true;
    }

    bool FITKOCCFaceTool::getMeanCurvatureAtPos(Interface::FITKAbsVirtualTopo* vtp, double* iPos, double & oCur)
    {
        if (!vtp)
        {
            return false;
        }

        // 获取抽象形状。
        FITKOCCTopoShape* occShape = vtp->getShapeT<FITKOCCTopoShape>();
        if (!occShape)
        {
            return false;
        }

        // 获取面。
        const TopoDS_Shape & shape = occShape->getTopoShape();
        const TopoDS_Face & face = TopoDS::Face(shape);
        if (face.IsNull())
        {
            return false;
        }

        Handle(Geom_Surface) surface = BRep_Tool::Surface(face);
        if (!surface)
        {
            return false;
        }

        try
        {
            gp_Pnt iPt(iPos[0], iPos[1], iPos[2]);
            ShapeAnalysis_Surface surfTool(surface);
            gp_Pnt2d uv = surfTool.ValueOfUV(iPt, 1e-3);

            BRepAdaptor_Surface brepFace(face);
            BRepLProp_SLProps props(brepFace, uv.X(), uv.Y(), 2, Precision::Confusion());
            oCur = props.MeanCurvature();
        }
        catch (...)
        {
            return false;
        }

        return true;
    }

    bool FITKOCCFaceTool::getCurvatureAtPos(Interface::FITKAbsVirtualTopo* vtp, double* iPos,
        double & oCur1, double & oCur2, double* oDir1, double* oDir2)
    {
        if (!vtp)
        {
            return false;
        }

        // 获取抽象形状。
        FITKOCCTopoShape* occShape = vtp->getShapeT<FITKOCCTopoShape>();
        if (!occShape)
        {
            return false;
        }

        // 获取面。
        const TopoDS_Shape & shape = occShape->getTopoShape();
        const TopoDS_Face & face = TopoDS::Face(shape);
        if (face.IsNull())
        {
            return false;
        }

        Handle(Geom_Surface) surface = BRep_Tool::Surface(face);
        if (!surface)
        {
            return false;
        }

        try
        {
            gp_Pnt iPt(iPos[0], iPos[1], iPos[2]);
            ShapeAnalysis_Surface surfTool(surface);
            gp_Pnt2d uv = surfTool.ValueOfUV(iPt, 1e-3);

            BRepAdaptor_Surface brepFace(face);
            BRepLProp_SLProps props(brepFace, uv.X(), uv.Y(), 2, Precision::Confusion());
            oCur1 = props.MaxCurvature();
            oCur2 = props.MinCurvature();

            gp_Dir maxDir, minDir;
            props.CurvatureDirections(maxDir, minDir);

            for (int i = 0; i < 3; i++)
            {
                oDir1[i] = maxDir.Coord(i);
                oDir2[i] = minDir.Coord(i);
            }
        }
        catch (...)
        {
            return false;
        }

        return true;
    }

    bool FITKOCCFaceTool::getPlane(Interface::FITKAbsVirtualTopo* vtp, double* oPos, double* oNor, double* oUp, int iModelId)
    {
        if (!vtp || !oPos || !oNor || !oUp)
        {
            return false;
        }

        // 获取抽象形状。
        FITKOCCTopoShape* occShape = vtp->getShapeT<FITKOCCTopoShape>();
        if (!occShape)
        {
            return false;
        }

        // 获取面。
        const TopoDS_Shape & shape = occShape->getTopoShape();
        const TopoDS_Face & face = TopoDS::Face(shape);
        if (face.IsNull())
        {
            return false;
        }

        // 判断是否为平面。
        BRepAdaptor_Surface baSurf(face);
        if (baSurf.GetType() != GeomAbs_SurfaceType::GeomAbs_Plane)
        {
            return false;
        }

        Handle(Geom_Plane) plane = Handle(Geom_Plane)::DownCast(BRep_Tool::Surface(face));
        if (!plane)
        {
            return false;
        }

        try
        {
            gp_Pnt gpPos;

            // 判断面是否有限。
            Standard_Real uFirst, uLast, vFirst, vLast;
            BRepTools::UVBounds(face, uFirst, uLast, vFirst, vLast);
           
            //plane->Bounds(uFirst, uLast, vFirst, vLast);

            if (qAbs(uFirst) >= Precision::Infinite()
                || qAbs(uLast) >= Precision::Infinite()
                || qAbs(vFirst) >= Precision::Infinite()
                || qAbs(vLast) >= Precision::Infinite())
            {
                // 获取包围盒中心。
                Bnd_Box box;
                BRepBndLib::AddClose(face, box);
                double pos1[3], pos2[3];
                box.Get(pos1[0], pos1[1], pos1[2], pos2[0], pos2[1], pos2[2]);
                gpPos = gp_Pnt((pos1[0] + pos2[0]) * 0.5, (pos1[1] + pos2[1]) * 0.5, (pos1[2] + pos2[2]) * 0.5);
            }
            else
            {
                // 获取重心。
                GProp_GProps props;
                BRepGProp::SurfaceProperties(face, props);
                gpPos = props.CentreOfMass();//ptMid;//gpPln.Location();
            }
            
            //ShapeAnalysis_Surface surfTool(surface);
            //gp_Pnt ptMid = surfTool.Value(uMid, vMid);

            //gp_Vec du, dv;
            //surface->D1(uMid, vMid, ptMid, du, dv);

            // 获取信息。
            gp_Pln gpPln = baSurf.Plane();
            gp_Dir gpNor = gpPln.Position().Direction();
            gp_Dir gpUp = gpPln.Position().YDirection();

            // 翻转。
            if (shape.Orientation() == TopAbs_Orientation::TopAbs_REVERSED)
            {
                gpNor.Reverse();
                gpUp.Reverse();
            }

            // 返回数据。
            for (int i = 0; i < 3; i++)
            {
                oPos[i] = gpPos.Coord(i + 1);
                oNor[i] = gpNor.Coord(i + 1);
                oUp[i] = gpUp.Coord(i + 1);
            }

            // 变换坐标与方向。
            Interface::FITKAbstractModel* model = FITKDATAREPO->getTDataByID<Interface::FITKAbstractModel>(iModelId);
            if (model)
            {
                model->transformPoint(oPos, oPos);
                model->transformDirection(oNor, oNor);
                model->transformDirection(oUp, oUp);
            }
        }
        catch (...)
        {
            return false;
        }

        return true;
    }

    bool FITKOCCFaceTool::getNormal(Interface::FITKAbsVirtualTopo* vtp, double* oPos, double* oNormal, int iModelId)
    {
        if (!vtp || !oPos || !oNormal)
        {
            return false;
        }

        // 获取抽象形状。
        FITKOCCTopoShape* occShape = vtp->getShapeT<FITKOCCTopoShape>();
        if (!occShape)
        {
            return false;
        }

        // 获取面。
        const TopoDS_Shape & shape = occShape->getTopoShape();
        const TopoDS_Face & face = TopoDS::Face(shape);
        if (face.IsNull())
        {
            return false;
        }

        Handle(Geom_Surface) surface = BRep_Tool::Surface(face);
        ShapeAnalysis_Surface surfTool(surface);
        if (surfTool.Gap() > 1e-3)
        {
            return false;
        }

        //Standard_Real uFirst, uLast, vFirst, vLast;
        //surface->Bounds(uFirst, uLast, vFirst, vLast);

        //Standard_Real uMid = uFirst + (uLast - uFirst) * 0.5;
        //Standard_Real vMid = vFirst + (vLast - vFirst) * 0.5;
        //gp_Pnt ptMid = surfTool.Value(uMid, vMid);

        GProp_GProps props;
        BRepGProp::SurfaceProperties(face, props);
        gp_Pnt ptMid = props.CentreOfMass();
        gp_Pnt2d uv = surfTool.ValueOfUV(ptMid, 1e-3);

        gp_Vec du, dv;
        surface->D1(uv.X(), uv.Y(), ptMid, du, dv);

        // 叉积取方向。
        gp_Vec normalVec = du.Crossed(dv);
        if (normalVec.SquareMagnitude() < gp::Resolution())
        {
            return false;
        }

        normalVec.Normalize();
        normalVec = gp_Dir(normalVec);

        // 翻转。
        if (shape.Orientation() == TopAbs_Orientation::TopAbs_REVERSED)
        {
            normalVec.Reverse();
        }

        // 返回数据。
        for (int i = 0; i < 3; i++)
        {
            oPos[i] = ptMid.Coord(i + 1);
            oNormal[i] = normalVec.Coord(i + 1);
        }

        // 变换坐标与方向。
        Interface::FITKAbstractModel* model = FITKDATAREPO->getTDataByID<Interface::FITKAbstractModel>(iModelId);
        if (model)
        {
            model->transformPoint(oPos, oPos);
            model->transformDirection(oNormal, oNormal);
        }

        return true;
    }

    bool FITKOCCFaceTool::getNormalAtPos(Interface::FITKAbsVirtualTopo* vtp, double* iPos, double* oNormal, int iModelId)
    {
        if (!vtp || !iPos || !oNormal)
        {
            return false;
        }

        // 获取抽象形状。
        FITKOCCTopoShape* occShape = vtp->getShapeT<FITKOCCTopoShape>();
        if (!occShape)
        {
            return false;
        }

        // 获取面。
        const TopoDS_Shape & shape = occShape->getTopoShape();
        const TopoDS_Face & face = TopoDS::Face(shape);
        if (face.IsNull())
        {
            return false;
        }

        gp_Pnt iPt(iPos[0], iPos[1], iPos[2]);
        Handle(Geom_Surface) surface = BRep_Tool::Surface(face);
        ShapeAnalysis_Surface surfTool(surface);
        gp_Pnt2d uv = surfTool.ValueOfUV(iPt, 1e-3);

        if (surfTool.Gap() > 1e-3) 
        {
            return false;
        }

        gp_Vec du, dv;
        Standard_Real u = uv.X();
        Standard_Real v = uv.Y();
        surface->D1(u, v, iPt, du, dv);

        // 叉积取方向。
        gp_Vec normalVec = du.Crossed(dv);
        if (normalVec.SquareMagnitude() < gp::Resolution()) 
        {
            return false;
        }

        normalVec.Normalize();
        normalVec = gp_Dir(normalVec);

        // 翻转。
        if (shape.Orientation() == TopAbs_Orientation::TopAbs_REVERSED)
        {
            normalVec.Reverse();
        }

        // 返回数据。
        for (int i = 0; i < 3; i++)
        {
            oNormal[i] = normalVec.Coord(i + 1);
        }

        // 变换坐标与方向。
        Interface::FITKAbstractModel* model = FITKDATAREPO->getTDataByID<Interface::FITKAbstractModel>(iModelId);
        if (model)
        {
            model->transformDirection(oNormal, oNormal);
        }

        return true;
    }

    bool FITKOCCFaceTool::isPlane(Interface::FITKAbsVirtualTopo* vtp)
    {
        if (!vtp)
        {
            return false;
        }

        // 获取抽象形状。
        FITKOCCTopoShape* occShape = vtp->getShapeT<FITKOCCTopoShape>();
        if (!occShape)
        {
            return false;
        }

        // 获取面。
        const TopoDS_Shape & shape = occShape->getTopoShape();
        const TopoDS_Face & face = TopoDS::Face(shape);
        if (face.IsNull())
        {
            return false;
        }

        // 判断是否为平面。
        BRepAdaptor_Surface baSurf(face);
        if (baSurf.GetType() != GeomAbs_SurfaceType::GeomAbs_Plane)
        {
            return false;
        }

        return true;
    }

    bool FITKOCCFaceTool::isCylind(Interface::FITKAbsVirtualTopo* vtp)
    {
        if (!vtp)
        {
            return false;
        }

        // 获取抽象形状。
        FITKOCCTopoShape* occShape = vtp->getShapeT<FITKOCCTopoShape>();
        if (!occShape)
        {
            return false;
        }

        // 获取面。
        const TopoDS_Shape & shape = occShape->getTopoShape();
        const TopoDS_Face & face = TopoDS::Face(shape);
        if (face.IsNull())
        {
            return false;
        }

        // 判断是否为圆柱面。
        BRepAdaptor_Surface baSurf(face);
        if (baSurf.GetType() != GeomAbs_SurfaceType::GeomAbs_Cylinder)
        {
            return false;
        }

        return true;
    }

    bool FITKOCCFaceTool::getCylindSurf(Interface::FITKAbsVirtualTopo* vtp, double* oCt, double* oDir)
    {
        if (!vtp)
        {
            return false;
        }

        // 获取抽象形状。
        FITKOCCTopoShape* occShape = vtp->getShapeT<FITKOCCTopoShape>();
        if (!occShape)
        {
            return false;
        }

        // 获取面。
        const TopoDS_Shape & shape = occShape->getTopoShape();
        const TopoDS_Face & face = TopoDS::Face(shape);
        if (face.IsNull())
        {
            return false;
        }

        // 判断是否为圆柱面。
        BRepAdaptor_Surface baSurf(face);
        if (baSurf.GetType() != GeomAbs_SurfaceType::GeomAbs_Cylinder)
        {
            return false;
        }

        Handle(Geom_Surface) surface = BRep_Tool::Surface(face);
        if (!surface)
        {
            return false;
        }

        // 获取圆柱面圆柱信息。
        Handle(Geom_CylindricalSurface) cySurf = Handle(Geom_CylindricalSurface)::DownCast(surface);
        gp_Cylinder cy = cySurf->Cylinder();

        const gp_Dir & dir = cy.Position().Direction();
        const gp_Pnt & ct = cy.Position().Location();

        for (int i = 0; i < 3; i++)
        {
            oCt[i] = ct.Coord(i + 1);
            oDir[i] = dir.Coord(i + 1);
        }

        return true;
    }

    bool FITKOCCFaceTool::isCone(Interface::FITKAbsVirtualTopo* vtp)
    {
        if (!vtp)
        {
            return false;
        }

        // 获取抽象形状。
        FITKOCCTopoShape* occShape = vtp->getShapeT<FITKOCCTopoShape>();
        if (!occShape)
        {
            return false;
        }

        // 获取面。
        const TopoDS_Shape & shape = occShape->getTopoShape();
        const TopoDS_Face & face = TopoDS::Face(shape);
        if (face.IsNull())
        {
            return false;
        }

        // 判断是否为圆锥或圆柱面。
        BRepAdaptor_Surface baSurf(face);
        if (baSurf.GetType() != GeomAbs_SurfaceType::GeomAbs_Cone &&
            baSurf.GetType() != GeomAbs_SurfaceType::GeomAbs_Cylinder)
        {
            return false;
        }

        return true;
    }

    bool FITKOCCFaceTool::getConeSurf(Interface::FITKAbsVirtualTopo* vtp, double* oCt, double* oDir)
    {
        if (!vtp)
        {
            return false;
        }

        // 获取抽象形状。
        FITKOCCTopoShape* occShape = vtp->getShapeT<FITKOCCTopoShape>();
        if (!occShape)
        {
            return false;
        }

        // 获取面。
        const TopoDS_Shape & shape = occShape->getTopoShape();
        const TopoDS_Face & face = TopoDS::Face(shape);
        if (face.IsNull())
        {
            return false;
        }

        Handle(Geom_Surface) surface = BRep_Tool::Surface(face);
        if (!surface)
        {
            return false;
        }

        // 判断是否为圆锥或圆柱面。
        BRepAdaptor_Surface baSurf(face);
        if (baSurf.GetType() == GeomAbs_SurfaceType::GeomAbs_Cone)
        {
            // 获取圆锥面圆锥信息。
            Handle(Geom_ConicalSurface) coneSurf = Handle(Geom_ConicalSurface)::DownCast(surface);
            gp_Cone cone = coneSurf->Cone();

            const gp_Dir & dir = cone.Position().Direction();
            const gp_Pnt & ct = cone.Position().Location();

            for (int i = 0; i < 3; i++)
            {
                oCt[i] = ct.Coord(i + 1);
                oDir[i] = dir.Coord(i + 1);
            }
        }
        else if (baSurf.GetType() == GeomAbs_SurfaceType::GeomAbs_Cylinder)
        {
            // 获取圆柱面圆柱信息。
            Handle(Geom_CylindricalSurface) cySurf = Handle(Geom_CylindricalSurface)::DownCast(surface);
            gp_Cylinder cy = cySurf->Cylinder();

            const gp_Dir & dir = cy.Position().Direction();
            const gp_Pnt & ct = cy.Position().Location();

            for (int i = 0; i < 3; i++)
            {
                oCt[i] = ct.Coord(i + 1);
                oDir[i] = dir.Coord(i + 1);
            }
        }
        else
        {
            return false;
        }

        return true;
    }

    bool FITKOCCFaceTool::isCoplanar(QList<int> ids)
    {
        // 异常判断。
        if (ids.isEmpty())
        {
            return false;
        }

        // 检查唯一一个拓扑是否为平面。
        if (ids.count() == 1)
        {
            Interface::FITKAbsVirtualTopo* vtp = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(ids[0]);
            return isPlane(vtp);
        }

        // 检查所有拓扑。
        auto getTopoPlane = [&](const int & id, Handle(Geom_Plane) & pln)
        {
            Interface::FITKAbsVirtualTopo* vtp = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(id);
            if (!vtp)
            {
                return false;
            }

            // 获取抽象形状。
            FITKOCCTopoShape* occShape = vtp->getShapeT<FITKOCCTopoShape>();
            if (!occShape)
            {
                return false;
            }

            // 获取面。
            const TopoDS_Shape & shape = occShape->getTopoShape();
            const TopoDS_Face & face = TopoDS::Face(shape);
            if (face.IsNull())
            {
                return false;
            }

            Handle(Geom_Plane) surface = Handle(Geom_Plane)::DownCast(BRep_Tool::Surface(face));
            if (!surface)
            {
                return false;
            }

            pln = surface;

            return true;
        };

        // 获取第一个平面。
        int firstTopo = ids.takeAt(0);
        Handle(Geom_Plane) firstPln;
        bool hasFirstPlane = getTopoPlane(firstTopo, firstPln);
        if (!hasFirstPlane || !firstPln)
        {
            return false;
        }

        // 获取第一个平面信息。
        gp_Ax3 firstPosAx3 =  firstPln->Position();
        const gp_Dir & firstNor = firstPosAx3.Direction();
        //const gp_Pnt & firstOrg = firstPosAx3.Location();

        for (const int & id : ids)
        {
            Handle(Geom_Plane) pln;
            bool hasPlane = getTopoPlane(id, pln);
            if (!hasPlane)
            {
                return false;
            }

            gp_Ax3 posAx3 = pln->Position();
            const gp_Dir & nor = posAx3.Direction();
            const gp_Pnt & org = posAx3.Location();

            // 检查中心是否在第一个平面上。
            Standard_Real u, v;
            bool flag = GeomLib_Tool::Parameters(firstPln, org, Precision::Confusion(), u, v);
            if (!flag)
            {
                return false;
            }

            // 比较法向。
            for (int i = 0; i < 3; i++)
            {
                flag &= (qFuzzyCompare(firstNor.Coord(i + 1), nor.Coord(i + 1))
                    || qFuzzyCompare(firstNor.Coord(i + 1), nor.Coord(i + 1) * -1.));
            }

            if (!flag)
            {
                return false;
            }
        }

        return true;
    }

    bool FITKOCCFaceTool::isSameSurface(QList<int> ids)
    {
        // 异常判断。
        if (ids.isEmpty())
        {
            return false;
        }

        // 检查唯一一个拓扑是否为面。
        if (ids.count() == 1)
        {
            Interface::FITKAbsVirtualTopo* vtp = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(ids[0]);
            if (!vtp)
            {
                return false;
            }

            return vtp->getShapeType() == Interface::FITKGeoEnum::VSFace;
        }

        // 检查所有拓扑并获取面。
        QList<TopoDS_Face> faces;
        for (const int & id : ids)
        {
            Interface::FITKAbsVirtualTopo* vtp = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(id);
            if (!vtp)
            {
                return false;
            }

            // 获取抽象形状。
            FITKOCCTopoShape* occShape = vtp->getShapeT<FITKOCCTopoShape>();
            if (!occShape)
            {
                return false;
            }

            // 获取面。
            const TopoDS_Shape & shape = occShape->getTopoShape();
            const TopoDS_Face & face = TopoDS::Face(shape);
            if (face.IsNull())
            {
                return false;
            }

            faces.push_back(face);
        }

        // 获取第一个面曲面。
        TopoDS_Face firstFace = faces.takeAt(0);
        TopAbs_Orientation firstOrient = firstFace.Orientation();
        Handle(Geom_Surface) firstSurface = BRep_Tool::Surface(firstFace);

        // 获取曲面类型。
        BRepAdaptor_Surface firstBaSurf(firstFace);
        GeomAbs_SurfaceType firstSurfType = firstBaSurf.GetType();

        // 判断是否为平面。
        if (firstSurfType == GeomAbs_SurfaceType::GeomAbs_Plane)
        {
            return isCoplanar(ids);
        }

        // 获取其他面。
        for (const TopoDS_Face & face : faces)
        {
            // 与第一个面进行比较。
            Handle(Geom_Surface) surface = BRep_Tool::Surface(face);
            if (!surface)
            {
                return false;
            }

            TopAbs_Orientation orient = face.Orientation();

            BRepAdaptor_Surface baSurf(face);
            GeomAbs_SurfaceType surfType = baSurf.GetType();

            // 检查面类型与方向。
            if (surfType != firstSurfType || orient != firstOrient)
            {
                return false;
            }

            switch (surfType)
            {
            case GeomAbs_SurfaceType::GeomAbs_Cylinder:
            {
                Handle(Geom_CylindricalSurface) cySurf = Handle(Geom_CylindricalSurface)::DownCast(surface);
                Handle(Geom_CylindricalSurface) fCySurf = Handle(Geom_CylindricalSurface)::DownCast(firstSurface);
                gp_Cylinder cy = cySurf->Cylinder();
                gp_Cylinder fCy = fCySurf->Cylinder();

                bool isEqual = FITKOCCToolCreator::IsEqual(cy.Position(), fCy.Position());
                isEqual &= qFuzzyCompare(cy.Radius(), fCy.Radius());

                if (!isEqual)
                {
                    return false;
                }

                break;
            }
            case GeomAbs_SurfaceType::GeomAbs_Cone:
            {
                Handle(Geom_ConicalSurface) conicSurf = Handle(Geom_ConicalSurface)::DownCast(surface);
                Handle(Geom_ConicalSurface) fConicSurf = Handle(Geom_ConicalSurface)::DownCast(firstSurface);
                gp_Cone cone = conicSurf->Cone();
                gp_Cone fCone = fConicSurf->Cone();

                bool isEqual = FITKOCCToolCreator::IsEqual(cone.Position(), fCone.Position());
                isEqual &= qFuzzyCompare(cone.RefRadius(), fCone.RefRadius());
                isEqual &= qFuzzyCompare(cone.SemiAngle(), fCone.SemiAngle());

                if (!isEqual)
                {
                    return false;
                }

                break;
            }
            case GeomAbs_SurfaceType::GeomAbs_Sphere:
            {
                Handle(Geom_SphericalSurface) sphSurf = Handle(Geom_SphericalSurface)::DownCast(surface);
                Handle(Geom_SphericalSurface) fSphSurf = Handle(Geom_SphericalSurface)::DownCast(firstSurface);
                gp_Sphere sph = sphSurf->Sphere();
                gp_Sphere fSph = fSphSurf->Sphere();

                bool isEqual = FITKOCCToolCreator::IsEqual(sph.Position(), fSph.Position());
                isEqual &= qFuzzyCompare(sph.Radius(), fSph.Radius());

                if (!isEqual)
                {
                    return false;
                }

                break;
            }
            case GeomAbs_SurfaceType::GeomAbs_Torus:
            {
                Handle(Geom_ToroidalSurface) torusSurf = Handle(Geom_ToroidalSurface)::DownCast(surface);
                Handle(Geom_ToroidalSurface) fTorusSurf = Handle(Geom_ToroidalSurface)::DownCast(firstSurface);
                gp_Torus torus = torusSurf->Torus();
                gp_Torus fTorus = fTorusSurf->Torus();

                bool isEqual = FITKOCCToolCreator::IsEqual(torus.Position(), fTorus.Position());
                isEqual &= qFuzzyCompare(torus.MajorRadius(), fTorus.MajorRadius());
                isEqual &= qFuzzyCompare(torus.MinorRadius(), fTorus.MinorRadius());

                if (!isEqual)
                {
                    return false;
                }

                break;
            }
            //case GeomAbs_SurfaceType::GeomAbs_BezierSurface:
            //{
            //    Handle(Geom_BezierSurface) bezierSurf = Handle(Geom_BezierSurface)::DownCast(surface);
            //    Handle(Geom_BezierSurface) fBezierSurf = Handle(Geom_BezierSurface)::DownCast(firstSurface);
            //    break;
            //}
            //case GeomAbs_SurfaceType::GeomAbs_BSplineSurface:
            //{
            //    Handle(Geom_BSplineSurface) bspineSurf = Handle(Geom_BSplineSurface)::DownCast(surface);
            //    Handle(Geom_BSplineSurface) fBspineSurf = Handle(Geom_BSplineSurface)::DownCast(firstSurface);
            //    break;
            //}
            //case GeomAbs_SurfaceType::GeomAbs_Plane:
            //case GeomAbs_SurfaceType::GeomAbs_SurfaceOfRevolution:
            //case GeomAbs_SurfaceType::GeomAbs_SurfaceOfExtrusion:
            //case GeomAbs_SurfaceType::GeomAbs_OffsetSurface:
            //case GeomAbs_SurfaceType::GeomAbs_OtherSurface:
            default:
                return false; // TODO
            }
        }

        return true;
    }
    //@}

    // 通用工具。
    //@{
    bool FITKOCCCommonTool::getDistanceByTopos(Interface::FITKAbsVirtualTopo* vtp_1, Interface::FITKAbsVirtualTopo* vtp_2,
        double & oDist, double oPos_1[3], double oPos_2[3], int iModelId_1, int iModelId_2)
    {
        if (!vtp_1 || !vtp_2)
        {
            return false;
        }

        // 获取抽象形状。
        FITKOCCTopoShape* occShape_1 = vtp_1->getShapeT<FITKOCCTopoShape>();
        FITKOCCTopoShape* occShape_2 = vtp_2->getShapeT<FITKOCCTopoShape>();
        if (!occShape_1 || !occShape_2)
        {
            return false;
        }

        // 获取形状。
        const TopoDS_Shape & shape_1 = occShape_1->getTopoShape();
        const TopoDS_Shape & shape_2 = occShape_2->getTopoShape();
        if(shape_1.IsNull() || shape_2.IsNull())
        {
            return false;
        }

        // 尝试变换形状。
        Interface::FITKAbstractAssInstance* instance1 = FITKDATAREPO->getTDataByID<Interface::FITKAbstractAssInstance>(iModelId_1);
        Interface::FITKAbstractAssInstance* instance2 = FITKDATAREPO->getTDataByID<Interface::FITKAbstractAssInstance>(iModelId_2);

        try
        {
            TopoDS_Shape s1, s2;
            bool transShape1 = false;
            bool transShape2 = false;

            if (instance1)
            {
                FITKOCCTransformTool* transTool = dynamic_cast<FITKOCCTransformTool*>(instance1->getInternalTransformTool());
                if (transTool)
                {
                    gp_Trsf* transf = transTool->getInternalTransform();
                    if (transf)
                    {
                        s1 = BRepBuilderAPI_Transform(shape_1, *transf, true);
                        if (s1.IsNull())
                        {
                            s1 = shape_1;
                        }

                        transShape1 = true;
                    }
                }
            }

            if (!transShape1)
            {
                s1 = shape_1;
            }

            if (instance2)
            {
                FITKOCCTransformTool* transTool = dynamic_cast<FITKOCCTransformTool*>(instance2->getInternalTransformTool());
                if (transTool)
                {
                    gp_Trsf* transf = transTool->getInternalTransform();
                    if (transf)
                    {
                        s2 = BRepBuilderAPI_Transform(shape_2, *transf, true);
                        if (s2.IsNull())
                        {
                            s2 = shape_2;
                        }

                        transShape2 = true;
                    }
                }
            }

            if (!transShape2)
            {
                s2 = shape_2;
            }

            BRepExtrema_DistShapeShape distSS;
            distSS.LoadS1(s1);
            distSS.LoadS2(s2);
            distSS.Perform();

            if (!distSS.IsDone())
            {
                return false;
            }

            const gp_Pnt & p1 = distSS.PointOnShape1(1);
            const gp_Pnt & p2 = distSS.PointOnShape2(1);

            oDist = distSS.Value();

            for (int i = 0; i < 3; i++)
            {
                oPos_1[i] = p1.Coord(i + 1);
                oPos_2[i] = p2.Coord(i + 1);
            }
        }
        catch (...)
        {
            return false;
        }

        return true;
    }

    bool FITKOCCCommonTool::getDistanceByTopoAndPos(Interface::FITKAbsVirtualTopo* vtp, double iPos3[3],
        double & oDist, double oPos[3], int iModelId)
    {
        if (!vtp)
        {
            return false;
        }

        // 获取抽象形状。
        FITKOCCTopoShape* occShape = vtp->getShapeT<FITKOCCTopoShape>();
        if (!occShape)
        {
            return false;
        }

        // 获取形状。
        const TopoDS_Shape & shape = occShape->getTopoShape();
        if (shape.IsNull())
        {
            return false;
        }

        // 尝试变换形状。
        Interface::FITKAbstractAssInstance* instance = FITKDATAREPO->getTDataByID<Interface::FITKAbstractAssInstance>(iModelId);

        // 创建点形状。
        const TopoDS_Vertex & vert = BRepLib_MakeVertex(gp_Pnt(iPos3[0], iPos3[1], iPos3[2]));

        try
        {
            TopoDS_Shape s;
            bool transShape = false;

            if (instance)
            {
                FITKOCCTransformTool* transTool = dynamic_cast<FITKOCCTransformTool*>(instance->getInternalTransformTool());
                if (transTool)
                {
                    gp_Trsf* transf = transTool->getInternalTransform();
                    if (transf)
                    {
                        s = BRepBuilderAPI_Transform(shape, *transf, true);
                        if (s.IsNull())
                        {
                            s = shape;
                        }

                        transShape = true;
                    }
                }
            }

            if (!transShape)
            {
                s = shape;
            }

            BRepExtrema_DistShapeShape distSS;
            distSS.LoadS1(s);
            distSS.LoadS2(vert);
            distSS.SetFlag(Extrema_ExtFlag::Extrema_ExtFlag_MIN);
            distSS.Perform();

            if (!distSS.IsDone())
            {
                return false;
            }

            const gp_Pnt & pos = distSS.PointOnShape1(1);

            oDist = distSS.Value();

            for (int i = 0; i < 3; i++)
            {
                oPos[i] = pos.Coord(i + 1);
            }
        }
        catch (...)
        {
            return false;
        }

        return true;
    }

    bool FITKOCCCommonTool::getCrossByEdgeAndLine(Interface::FITKAbsVirtualTopo* vtp, double iPos1[3], double iPos2[3],
        double oPos[3])
    {
        if (!vtp)
        {
            return false;
        }

        // 获取抽象形状。
        FITKOCCTopoShape* occShape = vtp->getShapeT<FITKOCCTopoShape>();
        if (!occShape)
        {
            return false;
        }

        // 获取边。
        const TopoDS_Shape & edge1 = TopoDS::Edge(occShape->getTopoShape());
        if (edge1.IsNull())
        {
            return false;
        }

        // 创建直线。
        gp_Pnt pnt1(iPos1[0], iPos1[1], iPos1[2]);
        gp_Pnt pnt2(iPos2[0], iPos2[1], iPos2[2]);
        double dist = pnt1.Distance(pnt2);

        TopoDS_Edge edge2;
        try
        {
            gp_Dir dir(pnt2.XYZ() - pnt1.XYZ());
            gp_Pnt pnt3 = dir.XYZ() * dist * 2 + pnt2.XYZ();
            edge2 = BRepLib_MakeEdge(pnt1, pnt3);
        }
        catch (...)
        {
            return false;
        }

        if (edge2.IsNull())
        {
            return false;
        }

        // 求交点。
        BRepAlgoAPI_Section sec(edge1, edge2);
        if (!sec.IsDone())
        {
            return false;
        }

        // 寻找距离终止位置最近的交点。
        bool hasPt = false;
        double distClosest = -1.;
        gp_Pnt ptClosest;

        gp_Pnt ptScale(iPos2[0], iPos2[1], iPos2[2]);
        for (TopExp_Explorer iter(sec, TopAbs_ShapeEnum::TopAbs_VERTEX); iter.More(); iter.Next())
        {
            const TopoDS_Vertex & v = TopoDS::Vertex(iter.Value());
            gp_Pnt pt = BRep_Tool::Pnt(v);
            double dist = pt.Distance(ptScale);

            if (distClosest < 0)
            {
                distClosest = dist;
                ptClosest = pt;
            }
            else
            {
                if (dist < distClosest)
                {
                    distClosest = dist;
                    ptClosest = pt;
                }
            }        

            hasPt = true;
        }

        if (!hasPt)
        {
            return false;
        }

        for (int i = 0; i < 3; i++)
        {
            oPos[i] = ptClosest.Coord(i + 1);
        }

        return true;
    }

    void FITKOCCCommonTool::canCheckTopos(bool & oInvalid, bool & oImprecise)
    {
        oInvalid = true;
        oImprecise = true;
    }

    bool FITKOCCCommonTool::getInvalidTopos(int cmdId, QList<Interface::VirtualShape> & oTopos, bool warnings)
    {
        Q_UNUSED(warnings);

        oTopos.clear();

        // 获取模型数据。
        Interface::FITKAbsGeoCommand* cmd = FITKDATAREPO->getTDataByID<Interface::FITKAbsGeoCommand>(cmdId);
        if (!cmd)
        {
            return false;
        }

        FITKAbstractOCCModel* shapeAgent = cmd->getTShapeAgent<FITKAbstractOCCModel>();
        if (!shapeAgent)
        {
            return false;
        }

        Interface::FITKVirtualTopoManager* vMgr = shapeAgent->getVirtualTopoManager();
        if (!vMgr)
        {
            return false;
        }

        Interface::FITKShapeVirtualTopoManager* vEdgeMgr = vMgr->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VSEdge);
        Interface::FITKShapeVirtualTopoManager* vFaceMgr = vMgr->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VSFace);
        if (!vEdgeMgr || !vFaceMgr)
        {
            return false;
        }

        TopoDS_Shape* topoShape = shapeAgent->getShape();
        if (!topoShape)
        {
            return false;
        }

        // 去重。
        auto hasTopoInList = [&](Interface::VirtualShape & newVShape)
        {
            for (const Interface::VirtualShape & vShape : oTopos)
            {
                if (newVShape.Type == vShape.Type &&
                    newVShape.CmdId == vShape.CmdId &&
                    newVShape.VirtualTopoId == vShape.VirtualTopoId)
                {
                    return true;
                }
            }

            return false;
        };

        try
        {
            // 检查模型自相交。
            BOPAlgo_ArgumentAnalyzer checker;
            checker.ArgumentTypeMode() = Standard_False;
            checker.SelfInterMode() = Standard_True;
            checker.SmallEdgeMode() = Standard_False;
            checker.RebuildFaceMode() = Standard_False;
            checker.TangentMode() = Standard_False;
            checker.MergeVertexMode() = Standard_False;
            checker.MergeEdgeMode() = Standard_False;
            checker.ContinuityMode() = Standard_False;
            checker.CurveOnSurfaceMode() = Standard_False;
            checker.SetShape1(*topoShape);
            checker.Perform();

            if (checker.HasFaulty())
            {
                const BOPAlgo_ListOfCheckResult & errs = checker.GetCheckResult();
                int nErrs = errs.Size();

                // 获取错误信息。
                for (int i = 0; i < nErrs; i++)
                {
                    for (BOPAlgo_ListIteratorOfListOfCheckResult iter(errs); iter.More(); iter.Next())
                    {
                        const BOPAlgo_CheckResult & err = iter.Value();
                        const TopTools_ListOfShape & errShapes = err.GetFaultyShapes1();

                        for (TopTools_ListIteratorOfListOfShape iterShape(errShapes); iterShape.More(); iterShape.Next())
                        {
                            const TopoDS_Shape & errShape = iterShape.Value();
                            if (errShape.IsNull())
                            {
                                continue;
                            }

                            // 查询虚拓扑对象。
                            Interface::FITKGeoEnum::VTopoShapeType vType = Interface::FITKGeoEnum::VSNone;
                            TopAbs_ShapeEnum sType = errShape.ShapeType();
                            switch (sType)
                            {
                            case TopAbs_SOLID:
                                vType = Interface::FITKGeoEnum::VSSolid;
                                break;
                            case TopAbs_SHELL:
                                vType = Interface::FITKGeoEnum::VSShell;
                                break;
                            case TopAbs_FACE:
                                vType = Interface::FITKGeoEnum::VSFace;
                                break;
                            case TopAbs_WIRE:
                                vType = Interface::FITKGeoEnum::VSWire;
                                break;
                            case TopAbs_EDGE:
                                vType = Interface::FITKGeoEnum::VSEdge;
                                break;
                            case TopAbs_VERTEX:
                                vType = Interface::FITKGeoEnum::VSPoint;
                                break;
                            default:
                                continue;
                                break;
                            }

                            Interface::FITKShapeVirtualTopoManager* vTopoMgr =
                                vMgr->getShapeVirtualTopoManager(vType);
                            if (!vTopoMgr)
                            {
                                continue;
                            }

                            // 查询拓扑。
                            for (int j = 0; j < vTopoMgr->getDataCount(); j++)
                            {
                                Interface::FITKAbsVirtualTopo* vTopo = vTopoMgr->getDataByIndex(j);
                                if (!vTopo)
                                {
                                    continue;
                                }

                                FITKOCCTopoShape* occShape = vTopo->getShapeT<FITKOCCTopoShape>();
                                if (!occShape)
                                {
                                    continue;
                                }
                                
                                bool isSame = occShape->getTopoShape().IsSame(errShape);
                                if (isSame)
                                {
                                    Interface::VirtualShape vShape;
                                    vShape.CmdId = cmdId;
                                    vShape.Type = vType;
                                    vShape.VirtualTopoId = vTopo->getDataObjectID();
                                    vShape.VirtualTopoIndex = j;

                                    if (!hasTopoInList(vShape))
                                    {
                                        oTopos.push_back(vShape);
                                    }
                                    
                                    break;
                                }
                            }
                        }
                    }
                }
            }

            // 查询壳面坏边。
            ShapeAnalysis_Shell anaShell;
            anaShell.LoadShells(*topoShape);
            if (anaShell.HasBadEdges())
            {
                TopoDS_Compound edges = anaShell.BadEdges();
                if (edges.NbChildren() != 0)
                {
                    TopoDS_Iterator iter(edges);
                    for (; iter.More(); iter.Next())
                    {
                        // 获取形状。
                        const TopoDS_Shape & subShape = iter.Value();

                        for (int i = 0; i < vEdgeMgr->getDataCount(); i++)
                        {
                            Interface::FITKAbsVirtualTopo* vEdge = vEdgeMgr->getDataByIndex(i);
                            if (!vEdge)
                            {
                                continue;
                            }

                            FITKOCCTopoShape* occShape = vEdge->getShapeT<FITKOCCTopoShape>();
                            if (!occShape)
                            {
                                continue;
                            }

                            bool isSame = occShape->getTopoShape().IsSame(subShape);
                            if (isSame)
                            {
                                Interface::VirtualShape vShape;
                                vShape.CmdId = cmdId;
                                vShape.Type = Interface::FITKGeoEnum::VSEdge;
                                vShape.VirtualTopoId = vEdge->getDataObjectID();
                                vShape.VirtualTopoIndex = i;

                                if (!hasTopoInList(vShape))
                                {
                                    oTopos.push_back(vShape);
                                }

                                break;
                            }
                        }
                    }
                }
            }

            // 检查多段线连接性。
            for (int i = 0; i < vFaceMgr->getDataCount(); i++)
            {
                Interface::FITKAbsVirtualTopo* vFace = vFaceMgr->getDataByIndex(i);
                if (!vFace)
                {
                    continue;
                }

                FITKOCCTopoShape* occShape = vFace->getShapeT<FITKOCCTopoShape>();
                if (!occShape)
                {
                    continue;
                }

                const TopoDS_Face & face = TopoDS::Face(occShape->getTopoShape());
                if (face.IsNull())
                {
                    continue;
                }

                // 遍历线框。
                TopExp_Explorer iter;
                for (iter.Init(face, TopAbs_ShapeEnum::TopAbs_WIRE); iter.More(); iter.Next())
                {
                    const TopoDS_Wire & faceWire = TopoDS::Wire(iter.Value());
                    if (faceWire.IsNull())
                    {
                        continue;
                    }

                    ShapeAnalysis_Wire anaWire;
                    anaWire.Init(faceWire, face, 1e-3); // Precision::Confusion()
                    if (!anaWire.Perform())
                    {
                        continue;
                    }

                    // bool flag = anaWire.CheckEdgeCurves();
                    bool flag = anaWire.CheckDegenerated();
                    flag &= anaWire.CheckClosed(1e-3); // Precision::Confusion()
                    if (!flag)
                    {
                        Interface::VirtualShape vShape;
                        vShape.CmdId = cmdId;
                        vShape.Type = Interface::FITKGeoEnum::VSFace;
                        vShape.VirtualTopoId = vFace->getDataObjectID();
                        vShape.VirtualTopoIndex = i;

                        if (!hasTopoInList(vShape))
                        {
                            oTopos.push_back(vShape);
                        }

                        break;
                    }
                }
            }
        }
        catch (...)
        {
            return false;
        }

        return true;
    }

    bool FITKOCCCommonTool::getImpreciseTopos(int cmdId, QList<Interface::VirtualShape> & oTopos)
    {
        oTopos.clear();

        // 获取模型数据。
        Interface::FITKAbsGeoCommand* cmd = FITKDATAREPO->getTDataByID<Interface::FITKAbsGeoCommand>(cmdId);
        if (!cmd)
        {
            return false;
        }

        FITKAbstractOCCModel* shapeAgent = cmd->getTShapeAgent<FITKAbstractOCCModel>();
        if (!shapeAgent)
        {
            return false;
        }

        Interface::FITKVirtualTopoManager* vMgr = shapeAgent->getVirtualTopoManager();
        if (!vMgr)
        {
            return false;
        }

        // 去重。
        auto hasTopoInList = [&](Interface::VirtualShape & newVShape)
        {
            for (const Interface::VirtualShape & vShape : oTopos)
            {
                if (newVShape.Type == vShape.Type &&
                    newVShape.CmdId == vShape.CmdId &&
                    newVShape.VirtualTopoId == vShape.VirtualTopoId)
                {
                    return true;
                }
            }

            return false;
        };

        // 查询拓扑
        auto getTopo = [&](Interface::FITKShapeVirtualTopoManager* vShapeMgr, const TopoDS_Shape & shape, Interface::VirtualShape & vShape, int cmdId)
        {
            for (int i = 0; i < vShapeMgr->getDataCount(); i++)
            {
                Interface::FITKAbsVirtualTopo* vTopo = vShapeMgr->getDataByIndex(i);
                if (!vTopo)
                {
                    continue;
                }

                FITKOCCTopoShape* occShape = vTopo->getShapeT<FITKOCCTopoShape>();                
                if (!occShape)
                {
                    continue;
                }

                const TopoDS_Shape & topoShape = occShape->getTopoShape();
                if (topoShape.IsSame(shape))
                {
                    vShape.CmdId = cmdId;
                    vShape.Type = vTopo->getShapeType();
                    vShape.VirtualTopoId = vTopo->getDataObjectID();
                    vShape.VirtualTopoIndex = i;

                    if (!hasTopoInList(vShape))
                    {
                        oTopos.push_back(vShape);
                    }

                    return true;
                }
            }

            return false;
        };

        TopoDS_Shape* topoShape = shapeAgent->getShape();

        // 检查模型容差。
        ShapeAnalysis_ShapeTolerance tolCheck;
        Handle(TopTools_HSequenceOfShape) shapes = tolCheck.OverTolerance(*topoShape, 1e-3); // Precision::Confusion()
        for (int i = 1; i <= shapes->Size(); i++)
        {
            const TopoDS_Shape & shape = shapes->Value(i);
            if (shape.IsNull())
            {
                continue;
            }

            Interface::VirtualShape vShape;
            bool flag = false;

            TopAbs_ShapeEnum shapeType = shape.ShapeType();
            switch (shapeType)
            {
            case TopAbs_SOLID:
                flag = getTopo(vMgr->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VSSolid), shape, vShape, cmdId);
                break;
            case TopAbs_SHELL:
                flag = getTopo(vMgr->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VSShell), shape, vShape, cmdId);
                break;
            case TopAbs_FACE:
                flag = getTopo(vMgr->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VSFace), shape, vShape, cmdId);
                break;
            case TopAbs_WIRE:
                flag = getTopo(vMgr->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VSWire), shape, vShape, cmdId);
                break;
            case TopAbs_EDGE:
                flag = getTopo(vMgr->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VSEdge), shape, vShape, cmdId);
                break;
            case TopAbs_VERTEX:
                flag = getTopo(vMgr->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VSPoint), shape, vShape, cmdId);
                break;
            default:
                break;
            }

            if (flag)
            {
                if (!hasTopoInList(vShape))
                {
                    oTopos.push_back(vShape);
                }
            }
        }

        return true;
    }

    bool FITKOCCCommonTool::calcCircleInfoBy3Points(QVector<double> pt1, QVector<double> pt2, QVector<double> pt3,
        QVector<double> & center, QVector<double> & normal, double & radius)
    {
        center.clear();
        normal.clear();

        Core::FITKPoint ct;
        Core::FITKVec3D nor(0., 0., 0.);
        Core::GetCircleBy3Points(Core::FITKPoint(pt1[0], pt1[1], pt1[2]), Core::FITKPoint(pt2[0], pt2[1], pt2[2]),
            Core::FITKPoint(pt3[0], pt3[1], pt3[2]), ct, nor, radius);

        double nor3[3], ct3[3];
        nor.getCoor(nor3);
        ct.getCoor(ct3);

        for (int i = 0; i < 3; i++)
        {
            normal.push_back(nor3[i]);
            center.push_back(ct[i]);
        }

        return true;
    }

    bool FITKOCCCommonTool::getFaceSamplePoints(Interface::FITKAbsVirtualTopo* vtp, QList<QList<double>> & oPoses, int iSpacing, int iModelId)
    {
        oPoses.clear();

        if (!vtp)
        {
            return false;
        }

        Interface::FITKAbstractModel* model = FITKDATAREPO->getTDataByID<Interface::FITKAbstractModel>(iModelId);

        // 获取抽象形状。
        FITKOCCTopoShape* occShape = vtp->getShapeT<FITKOCCTopoShape>();
        if (!occShape)
        {
            return false;
        }

        // 获取形状。
        const TopoDS_Face & face = TopoDS::Face(occShape->getTopoShape());
        if (face.IsNull())
        {
            return false;
        }

        Handle(Geom_Surface) surface = BRep_Tool::Surface(face);
        if (surface.IsNull())
        {
            return false;
        }

        Standard_Real uFirst, uLast, vFirst, vLast;   
        //surface->Bounds(uFirst, uLast, vFirst, vLast);

        BRepAdaptor_Surface brepFace(face);
        uFirst = brepFace.FirstUParameter();
        uLast = brepFace.LastUParameter();
        vFirst = brepFace.FirstVParameter();
        vLast = brepFace.LastVParameter();      
        
        // 创建适配器采样。
#if OCC_VERSION_HEX < 0x070600
        Handle(GeomAdaptor_HSurface) adpSurf = new GeomAdaptor_HSurface(surface, uFirst, uLast, vFirst, vLast);
#else
        Handle(GeomAdaptor_Surface) adpSurf = new GeomAdaptor_Surface(surface, uFirst, uLast, vFirst, vLast);
#endif

        Adaptor3d_TopolTool tool(adpSurf);
        tool.SamplePnts(1e-2, qMax(1, iSpacing / (brepFace.UPeriod() ? 5 : 1)),
            qMax(1, iSpacing / (brepFace.VPeriod() ? 5 : 1)));
        //tool.ComputeSamplePoints();

        int nSamples = tool.NbSamples();
        for (int i = 1; i <= nSamples; i++)
        {
            gp_Pnt2d p2; 
            gp_Pnt p3;
            tool.SamplePoint(i, p2, p3);

            double pos3[3]{ p3.Coord(1), p3.Coord(2), p3.Coord(3) };

            // 变换坐标。
            if (model)
            {
                model->transformPoint(pos3, pos3);
            }

            oPoses.push_back(QList<double>{ pos3[0], pos3[1], pos3[2] });
        }

        return !oPoses.isEmpty();
    }
    //@}
}
