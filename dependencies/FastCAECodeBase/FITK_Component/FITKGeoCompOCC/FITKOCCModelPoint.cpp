/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCModelPoint.h"
#include <gp_Pnt.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>
#include <Geom_Curve.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <GeomAPI_ProjectPointOnCurve.hxx>
#include <BRep_Tool.hxx>
#include <BRepExtrema_ExtCC.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopoDS_Face.hxx>
#include <GeomAPI_ProjectPointOnSurf.hxx>
#include <GeomAPI_IntCS.hxx>
#include <GProp_GProps.hxx>
#include <BRepGProp.hxx>
#include <BRep_Builder.hxx>
#include <Geom_TrimmedCurve.hxx>

#include <FITK_Interface/FITKInterfaceGeometry/FITKGeoEnum.h>
#include <FITK_Interface/FITKInterfaceGeometry/FITKVirtualTopoManager.h>
#include "FITKOCCVirtualTopoCreator.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelSurface.h"

namespace OCC
{

    FITKOCCModelPoint::FITKOCCModelPoint() :OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelPoint::update()
    {
        gp_Pnt pt(m_X, m_Y, m_Z);

        _occShapeAgent->updateShape(BRepBuilderAPI_MakeVertex(pt).Shape());
        return true;
    }

    FITKOCCModelCurveEnd::FITKOCCModelCurveEnd() :OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelCurveEnd::update()
    {
        if (m_SourceCurve.isNull()) return false;

        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        auto curveCmd = geoCmdList->getDataByID(m_SourceCurve.CmdId);
        if (curveCmd == nullptr) return false;
        auto curveVShape = curveCmd->getShapeTopoByIndexT<FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSEdge, m_SourceCurve.VirtualTopoIndex);
        if (curveVShape == nullptr) return false;
        TopoDS_Shape curveShape = curveVShape->getTopoShape();
        if (curveShape.ShapeType() != TopAbs_ShapeEnum::TopAbs_EDGE) return false;
        TopoDS_Edge edge = TopoDS::Edge(curveShape);
        if (edge.IsNull()) return false;

        Standard_Real first;
        Standard_Real last;
        Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, first, last);
        if (curve.IsNull()) return false;

        gp_Pnt pnt = m_UseStartEnd ? curve->Value(first) : curve->Value(last);

        m_X = pnt.X();
        m_Y = pnt.Y();
        m_Z = pnt.Z();

        _occShapeAgent->updateShape(BRepBuilderAPI_MakeVertex(pnt).Shape());

        return true;
    }

    FITKOCCModelCurveEndDistance::FITKOCCModelCurveEndDistance() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelCurveEndDistance::update()
    {
        if (m_SourceCurve.isNull()) return false;

        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        auto curveCmd = geoCmdList->getDataByID(m_SourceCurve.CmdId);
        if (curveCmd == nullptr) return false;
        auto curveVShape = curveCmd->getShapeTopoByIndexT<FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSEdge, m_SourceCurve.VirtualTopoIndex);
        if (curveVShape == nullptr) return false;
        TopoDS_Shape curveShape = curveVShape->getTopoShape();
        if (curveShape.ShapeType() != TopAbs_ShapeEnum::TopAbs_EDGE) return false;
        TopoDS_Edge edge = TopoDS::Edge(curveShape);
        if (edge.IsNull()) return false;

        Standard_Real first;
        Standard_Real last;
        Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, first, last);
        if (curve.IsNull()) return false;


        // 定义变量来存储属性
        GProp_GProps props;
        // 计算边的属性
        BRepGProp::LinearProperties(edge, props);
        // 获取边的长度
        auto length = props.Mass();

        if (length < 1e-7 || m_Distance >= length) {
            printLog(QString(tr("The specified distance(%1) is greater than the total length(%2).")).arg(m_Distance).arg(length), 3);
            return false;
        }

        auto radio = m_UseStartEnd ? first + m_Distance / length * (last - first) : last - m_Distance / length * (last - first);
        gp_Pnt pnt = curve->Value(radio);

        m_X = pnt.X();
        m_Y = pnt.Y();
        m_Z = pnt.Z();

        _occShapeAgent->updateShape(BRepBuilderAPI_MakeVertex(pnt).Shape());

        return true;
    }

    FITKOCCModelCurveRadio::FITKOCCModelCurveRadio() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelCurveRadio::update()
    {
        if (m_SourceCurve.isNull()) return false;

        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        auto curveCmd = geoCmdList->getDataByID(m_SourceCurve.CmdId);
        if (curveCmd == nullptr) return false;
        auto curveVShape = curveCmd->getShapeTopoByIndexT<FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSEdge, m_SourceCurve.VirtualTopoIndex);
        if (curveVShape == nullptr) return false;
        TopoDS_Shape curveShape = curveVShape->getTopoShape();
        if (curveShape.ShapeType() != TopAbs_ShapeEnum::TopAbs_EDGE) return false;
        TopoDS_Edge edge = TopoDS::Edge(curveShape);
        if (edge.IsNull()) return false;

        Standard_Real first;
        Standard_Real last;
        Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, first, last);
        if (curve.IsNull()) return false;

        auto radio = first + m_Radio * (last - first);
        gp_Pnt pnt = curve->Value(radio);

        m_X = pnt.X();
        m_Y = pnt.Y();
        m_Z = pnt.Z();

        _occShapeAgent->updateShape(BRepBuilderAPI_MakeVertex(pnt).Shape());

        return true;
    }

    FITKOCCModelPointProjectionOnCurve::FITKOCCModelPointProjectionOnCurve() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }
    bool FITKOCCModelPointProjectionOnCurve::update()
    {
        if (m_SourceCurve.isNull() || m_SourcePoint.isNull()) return false;

        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        auto pointCmd = geoCmdList->getDataByID(m_SourcePoint.CmdId);
        if (pointCmd == nullptr) return false;
        auto pointVShape = pointCmd->getShapeTopoByIndexT<FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSPoint, m_SourcePoint.VirtualTopoIndex);
        if (pointVShape == nullptr) return false;
        TopoDS_Shape pointShape = pointVShape->getTopoShape();
        if (pointShape.ShapeType() != TopAbs_ShapeEnum::TopAbs_VERTEX) return false;
        TopoDS_Vertex vertex = TopoDS::Vertex(pointShape);
        if (vertex.IsNull()) return false;
        gp_Pnt pnt = BRep_Tool::Pnt(vertex);

        auto curveCmd = geoCmdList->getDataByID(m_SourceCurve.CmdId);
        if (curveCmd == nullptr) return false;
        auto curveVShape = curveCmd->getShapeTopoByIndexT<FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSEdge, m_SourceCurve.VirtualTopoIndex);
        if (curveVShape == nullptr) return false;
        TopoDS_Shape curveShape = curveVShape->getTopoShape();
        if (curveShape.ShapeType() != TopAbs_ShapeEnum::TopAbs_EDGE) return false;
        TopoDS_Edge edge = TopoDS::Edge(curveShape);
        if (edge.IsNull()) return false;

        Standard_Real first;
        Standard_Real last;
        Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, first, last);
        if (curve.IsNull()) return false;

        GeomAPI_ProjectPointOnCurve proj(pnt, curve);

        if (proj.NbPoints() == 0) return false;


        m_Projections.resize(proj.NbPoints());

        for (int i = 0; i < proj.NbPoints(); ++i) {
            gp_Pnt pnt = proj.Point(i + 1);
            m_Projections[i] = { pnt.X(), pnt.Y(), pnt.Z() };
        }
        // 坐标永远保存第一个点的坐标
        m_X = m_Projections.at(0)[0];
        m_Y = m_Projections.at(0)[1];
        m_Z = m_Projections.at(0)[2];

        if (m_Projections.size() == 1) {
            _occShapeAgent->updateShape(BRepBuilderAPI_MakeVertex(gp_Pnt(m_X, m_Y, m_Z)).Shape());
        }
        else {
            // 多个点的复合体
            BRep_Builder builder;
            TopoDS_Compound multiPoints;
            builder.MakeCompound(multiPoints);

            for (auto xyz : m_Projections) {
                builder.Add(multiPoints, BRepBuilderAPI_MakeVertex(gp_Pnt(xyz[0], xyz[1], xyz[2])));
            }

            _occShapeAgent->updateShape(multiPoints);
        }

        return true;
    }

    FITKOCCModelTwoCurveIntersection::FITKOCCModelTwoCurveIntersection() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }
    bool FITKOCCModelTwoCurveIntersection::update()
    {
        if (m_SourceCurve1.isNull() || m_SourceCurve2.isNull()) return false;

        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        // 第一条曲线
        auto curve1Cmd = geoCmdList->getDataByID(m_SourceCurve1.CmdId);
        if (curve1Cmd == nullptr) {
            curve1Cmd = geoCmdList->getDatumManager()->getDataByID(m_SourceCurve1.CmdId);
            if (curve1Cmd == nullptr) return false;
        }
        auto vshape1 = curve1Cmd->getShapeTopoByIndexT<FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSEdge, m_SourceCurve1.VirtualTopoIndex);
        if (vshape1 == nullptr) return false;
        TopoDS_Shape topoShape1 = vshape1->getTopoShape();
        if (topoShape1.IsNull() || topoShape1.ShapeType() != TopAbs_ShapeEnum::TopAbs_EDGE) return false;
        TopoDS_Edge edge1 = TopoDS::Edge(topoShape1);
        if (edge1.IsNull()) return false;
        // 第二条曲线
        auto curve2Cmd = geoCmdList->getDataByID(m_SourceCurve2.CmdId);
        if (curve2Cmd == nullptr) {
            curve2Cmd = geoCmdList->getDatumManager()->getDataByID(m_SourceCurve2.CmdId);
            if (curve2Cmd == nullptr) return false;
        }
        auto vshape2 = curve2Cmd->getShapeTopoByIndexT<FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSEdge, m_SourceCurve2.VirtualTopoIndex);
        if (vshape2 == nullptr) return false;
        TopoDS_Shape topoShape2 = vshape2->getTopoShape();
        if (topoShape2.IsNull() || topoShape2.ShapeType() != TopAbs_ShapeEnum::TopAbs_EDGE) return false;
        TopoDS_Edge edge2 = TopoDS::Edge(topoShape2);
        if (edge2.IsNull()) return false;

        BRepExtrema_ExtCC ext(edge1, edge2);


        if (ext.IsParallel()) {
            printLog(QString(tr("The two lines selected are parallel lines.")), 3);
            return false;
        }

        auto count = ext.NbExt();

        for (int i = 0; i < count; ++i) {
            auto pnt1 = ext.PointOnE1(i + 1);
            auto pnt2 = ext.PointOnE2(i + 1);
            if (!m_IsNearestPoint && pnt1.Distance(pnt2) > Precision::Approximation()) continue;
            m_Intersections.push_back({ pnt1.X(), pnt1.Y(), pnt1.Z() });
        }
        if (m_Intersections.size() == 0) {
            printLog(QString(tr("The two lines selected do not intersect.")), 3);
            return false;
        }
        // 坐标永远保存第一个点的坐标
        m_X = m_Intersections.at(0)[0];
        m_Y = m_Intersections.at(0)[1];
        m_Z = m_Intersections.at(0)[2];

        if (m_Intersections.size() == 1) {
            _occShapeAgent->updateShape(BRepBuilderAPI_MakeVertex(gp_Pnt(m_X, m_Y, m_Z)).Shape());
        }
        else {
            // 多个点的复合体
            BRep_Builder builder;
            TopoDS_Compound multiPoints;
            builder.MakeCompound(multiPoints);

            for (auto xyz : m_Intersections) {
                builder.Add(multiPoints, BRepBuilderAPI_MakeVertex(gp_Pnt(xyz[0], xyz[1], xyz[2])));
            }

            _occShapeAgent->updateShape(multiPoints);
        }
        return true;
    }

    FITKOCCModelPointProjectionOnSurface::FITKOCCModelPointProjectionOnSurface() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelPointProjectionOnSurface::update()
    {
        if (m_SourcePoint.isNull() || m_SourceSurface.isNull()) return false;

        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        auto pointCmd = geoCmdList->getDataByID(m_SourcePoint.CmdId);
        if (pointCmd == nullptr) return false;
        auto pointVShape = pointCmd->getShapeTopoByIndexT<FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSPoint, m_SourcePoint.VirtualTopoIndex);
        if (pointVShape == nullptr) return false;
        TopoDS_Shape pointShape = pointVShape->getTopoShape();
        if (pointShape.ShapeType() != TopAbs_ShapeEnum::TopAbs_VERTEX) return false;
        TopoDS_Vertex vertex = TopoDS::Vertex(pointShape);
        if (vertex.IsNull()) return false;
        gp_Pnt pnt = BRep_Tool::Pnt(vertex);

        auto surfaceCmd = geoCmdList->getDataByID(m_SourceSurface.CmdId);
        if (surfaceCmd == nullptr) return false;
        auto surfaceVShape = surfaceCmd->getShapeTopoByIndexT<FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSFace, m_SourceSurface.VirtualTopoIndex);
        if (surfaceVShape == nullptr) return false;
        TopoDS_Shape surfaceShape = surfaceVShape->getTopoShape();
        if (surfaceShape.ShapeType() != TopAbs_ShapeEnum::TopAbs_FACE) return false;
        TopoDS_Face face = TopoDS::Face(surfaceShape);
        if (face.IsNull()) return false;

        Handle(Geom_Surface) surface = BRep_Tool::Surface(face);
        if (surface.IsNull()) return false;

        GeomAPI_ProjectPointOnSurf proj(pnt, surface);

        if (proj.NbPoints() == 0) return false;


        m_Projections.resize(proj.NbPoints());

        for (int i = 0; i < proj.NbPoints(); ++i) {
            gp_Pnt pnt = proj.Point(i + 1);
            m_Projections[i] = { pnt.X(), pnt.Y(), pnt.Z() };
        }

        // 坐标永远保存第一个点的坐标
        m_X = m_Projections.at(0)[0];
        m_Y = m_Projections.at(0)[1];
        m_Z = m_Projections.at(0)[2];

        if (m_Projections.size() == 1) {
            _occShapeAgent->updateShape(BRepBuilderAPI_MakeVertex(gp_Pnt(m_X, m_Y, m_Z)).Shape());
        }
        else {
            // 多个点的复合体
            BRep_Builder builder;
            TopoDS_Compound multiPoints;
            builder.MakeCompound(multiPoints);

            for (auto xyz : m_Projections) {
                builder.Add(multiPoints, BRepBuilderAPI_MakeVertex(gp_Pnt(xyz[0], xyz[1], xyz[2])));
            }

            _occShapeAgent->updateShape(multiPoints);
        }
        return true;
    }

    FITKOCCModelCurveIntersectionSurface::FITKOCCModelCurveIntersectionSurface() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelCurveIntersectionSurface::update()
    {
        if (m_SourceCurve.isNull() || m_SourceSurface.isNull()) return false;

        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        // 曲线
        auto curveCmd = geoCmdList->getDataByID(m_SourceCurve.CmdId);
        if (curveCmd == nullptr) {
            curveCmd = geoCmdList->getDatumManager()->getDataByID(m_SourceCurve.CmdId);
            if (curveCmd == nullptr)  return false;
        }
        auto curveVShape = curveCmd->getShapeTopoByIndexT<FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSEdge, m_SourceCurve.VirtualTopoIndex);
        if (curveVShape == nullptr) return false;
        TopoDS_Shape curveShape = curveVShape->getTopoShape();
        if (curveShape.IsNull() || curveShape.ShapeType() != TopAbs_ShapeEnum::TopAbs_EDGE) return false;
        TopoDS_Edge edge = TopoDS::Edge(curveShape);
        if (edge.IsNull()) return false;
        Standard_Real first, last;
        Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, first, last);
        if (curve.IsNull()) return false;
        Handle(Geom_TrimmedCurve) trimmedCurve = new Geom_TrimmedCurve(curve, first, last);
        if (trimmedCurve.IsNull()) return false;

        // 曲面
        auto surfaceCmd = geoCmdList->getDataByID(m_SourceSurface.CmdId);
        if (surfaceCmd == nullptr)
        {
            surfaceCmd = geoCmdList->getDatumManager()->getDataByID(m_SourceSurface.CmdId);
            if (surfaceCmd == nullptr) return false;
        }
        auto surfaceVShape = surfaceCmd->getShapeTopoByIndexT<FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSFace, m_SourceSurface.VirtualTopoIndex);
        if (surfaceVShape == nullptr) return false;
        TopoDS_Shape surfaceShape = surfaceVShape->getTopoShape();
        if (surfaceShape.IsNull() || surfaceShape.ShapeType() != TopAbs_ShapeEnum::TopAbs_FACE) return false;
        TopoDS_Face face = TopoDS::Face(surfaceShape);
        if (face.IsNull()) return false;
        Handle(Geom_Surface) surface = BRep_Tool::Surface(face);
        if (surface.IsNull()) return false;

        GeomAPI_IntCS ext(trimmedCurve, surface);

        auto count = ext.NbPoints();
        if (count <= 0) return false;

        for (int i = 0; i < count; ++i) {
            auto pnt = ext.Point(i + 1);
            m_Intersections.push_back({ pnt.X(), pnt.Y(), pnt.Z() });
        }

        // 坐标永远保存第一个点的坐标
        m_X = m_Intersections.at(0)[0];
        m_Y = m_Intersections.at(0)[1];
        m_Z = m_Intersections.at(0)[2];

        if (m_Intersections.size() == 1) {
            _occShapeAgent->updateShape(BRepBuilderAPI_MakeVertex(gp_Pnt(m_X, m_Y, m_Z)).Shape());
        }
        else {
            // 多个点的复合体
            BRep_Builder builder;
            TopoDS_Compound multiPoints;
            builder.MakeCompound(multiPoints);

            for (auto xyz : m_Intersections) {
                builder.Add(multiPoints, BRepBuilderAPI_MakeVertex(gp_Pnt(xyz[0], xyz[1], xyz[2])));
            }

            _occShapeAgent->updateShape(multiPoints);
        }

        return true;
    }

    FITKOCCModelInteractionPoint::FITKOCCModelInteractionPoint() :OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelInteractionPoint::update()
    {
        gp_Pnt pt(m_X, m_Y, m_Z);

        _occShapeAgent->updateShape(BRepBuilderAPI_MakeVertex(pt).Shape());
        return true;
    }
}
