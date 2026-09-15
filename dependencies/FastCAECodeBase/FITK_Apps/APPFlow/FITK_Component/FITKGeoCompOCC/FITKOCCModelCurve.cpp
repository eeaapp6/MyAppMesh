/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCModelCurve.h"
#include <gp_Pnt.hxx>
#include <gp_Circ.hxx>
#include <gp_Elips.hxx>
#include <gp_Hypr.hxx>
#include <gp_Parab.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <Geom_OffsetCurve.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS.hxx>
#include <BRep_Tool.hxx>
#include <Geom_Curve.hxx>
#include <gce_MakeCirc.hxx>
#include <gce_MakeElips.hxx>
#include <gce_MakeHypr.hxx>
#include <gce_MakeParab.hxx>
#include <GeomAPI_IntSS.hxx>
#include <GeomProjLib.hxx>
#include <TopoDS_Face.hxx>
#include <BRepAlgoAPI_Section.hxx>
#include <BOPAlgo_PaveFiller.hxx>
#include <BOPDS_DS.hxx>
#include <GeomAPI_PointsToBSpline.hxx>
#include <Geom_BSplineCurve.hxx>
#include <GeomLib_Tool.hxx>
#include <Geom_BezierCurve.hxx>
#include <TopExp_Explorer.hxx>
#include <GC_MakeArcOfCircle.hxx>
#include <GC_MakeArcOfEllipse.hxx>
#include <GC_MakeArcOfHyperbola.hxx>
#include <GC_MakeArcOfParabola.hxx>
#include <GeomAPI_Interpolate.hxx>
#include <GeomAPI_ProjectPointOnSurf.hxx>

#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoEnum.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKVirtualTopoManager.h"
#include "FITKOCCVirtualTopoCreator.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelSurface.h"

#include <FITKOCCExtendTool.h>

namespace OCC
{
    // 构造函数
    FITKOCCModelLine::FITKOCCModelLine() :OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }
    // 更新数据
    bool FITKOCCModelLine::update()
    {
        return true;
    }

    // 构造函数
    FITKOCCModelSegment::FITKOCCModelSegment() :OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }
    // 更新数据
    bool FITKOCCModelSegment::update()
    {
        gp_Pnt start(m_StartPoint[0], m_StartPoint[1], m_StartPoint[2]);
        gp_Pnt end(m_EndPoint[0], m_EndPoint[1], m_EndPoint[2]);

        try {
            _occShapeAgent->updateShape(BRepBuilderAPI_MakeEdge(start, end).Shape());
        }
        catch (...)
        {
            printLog(tr("Failed to make curve!"), 3);
            return false;
        }
        return true;
    }

    // 构造函数
    FITKOCCModelWire::FITKOCCModelWire() :OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }
    // 更新数据
    bool FITKOCCModelWire::update()
    {
        int count = m_Points.count();
        if (count < 2) return false;

        BRepBuilderAPI_MakeWire wire;

        try {
            for (int i = 1; i < count; ++i) {
                gp_Pnt start(m_Points.at(i - 1)[0], m_Points.at(i - 1)[1], m_Points.at(i - 1)[2]);
                gp_Pnt end(m_Points.at(i)[0], m_Points.at(i)[1], m_Points.at(i)[2]);
                wire.Add(BRepBuilderAPI_MakeEdge(start, end));
            }

            _occShapeAgent->updateShape(wire.Shape());
        }
        catch (...)
        {
            printLog(tr("Failed to make curve!"), 3);
            return false;
        }
        return true;
    }

    // 构造函数
    FITKOCCModelCircle::FITKOCCModelCircle() :OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }
    // 更新数据
    bool FITKOCCModelCircle::update()
    {
        gp_Ax2 ax2;
        gp_Pnt center(m_Origin[0], m_Origin[1], m_Origin[2]);
        gp_Dir dir(m_ZDirection[0], m_ZDirection[1], m_ZDirection[2]);
        gp_Dir yDir(m_YDirection[0], m_YDirection[1], m_YDirection[2]);

        ax2.SetLocation(center);
        ax2.SetDirection(dir);
        ax2.SetYDirection(yDir);

        gp_Circ c(ax2, m_Radius);

        try {
            _occShapeAgent->updateShape(BRepBuilderAPI_MakeEdge(c).Shape());
        }
        catch (...)
        {
            printLog(tr("Failed to make curve!"), 3);
            return false;
        }
        return true;
    }
    // 构造函数
    FITKOCCModelThreePointsCircle::FITKOCCModelThreePointsCircle() :OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }
    // 更新数据
    bool FITKOCCModelThreePointsCircle::update()
    {
        try
        {
            auto p1 = m_Points.at(0);
            auto p2 = m_Points.at(1);
            auto p3 = m_Points.at(2);
            gce_MakeCirc circle(gp_Pnt(p1[0], p1[1], p1[2]), gp_Pnt(p2[0], p2[1], p2[2]), gp_Pnt(p3[0], p3[1], p3[2]));
            if (!circle.IsDone()) {
                printLog("Failed to make circle!", 3);
                return false;
            }
            gp_Circ c = circle.Value();
            BRepBuilderAPI_MakeEdge edge(c);
            if (!edge.IsDone()) {
                printLog("Failed to build shape!", 3);
                return false;
            }

            auto loc = c.Location();
            m_Origin = { loc.X(), loc.Y(), loc.Z() };
            m_Radius = c.Radius();

            auto pos = c.Position();
            gp_Dir dir = pos.XDirection();
            m_XDirection = { dir.X(), dir.Y(), dir.Z() };
            dir = pos.YDirection();
            m_XDirection = { dir.X(), dir.Y(), dir.Z() };
            dir = pos.Direction();
            m_ZDirection = { dir.X(), dir.Y(), dir.Z() };

            _occShapeAgent->updateShape(edge.Shape());

            return true;
        }
        catch (...)
        {
            printLog("Failed to make circle!", 3);
            return false;
        }
    }

    // 构造函数
    FITKOCCModelCircleArc::FITKOCCModelCircleArc() :OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }
    // 更新数据
    bool FITKOCCModelCircleArc::update()
    {
        gp_Ax2 ax2;
        gp_Pnt center(m_Origin[0], m_Origin[1], m_Origin[2]);
        gp_Dir dir(m_ZDirection[0], m_ZDirection[1], m_ZDirection[2]);
        gp_Dir yDir(m_YDirection[0], m_YDirection[1], m_YDirection[2]);

        ax2.SetLocation(center);
        ax2.SetDirection(dir);
        ax2.SetYDirection(yDir);

        gp_Circ circle(ax2, m_Radius);

        gp_Pnt start(m_StartPoint[0], m_StartPoint[1], m_StartPoint[2]);
        gp_Pnt end(m_EndPoint[0], m_EndPoint[1], m_EndPoint[2]);

        try {
            GC_MakeArcOfCircle circleArc(circle, start, end, true);
            if (!circleArc.IsDone()) return false;

            _occShapeAgent->updateShape(BRepBuilderAPI_MakeEdge(circleArc.Value()).Shape());
        }
        catch (...)
        {
            printLog(tr("Failed to make curve!"), 3);
            return false;
        }
        return true;
    }

    // 构造函数
    FITKOCCModelEllipse::FITKOCCModelEllipse() :OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }
    // 更新数据
    bool FITKOCCModelEllipse::update()
    {
        gp_Elips e;
        gp_Pnt center(m_Origin[0], m_Origin[1], m_Origin[2]);

        e.SetPosition(
            gp_Ax2(center,
                gp_Dir(m_ZDirection[0], m_ZDirection[1], m_ZDirection[2]),
                gp_Dir(m_XDirection[0], m_XDirection[1], m_XDirection[2]))
        );
        e.SetMajorRadius(m_MajorRadius);
        e.SetMinorRadius(m_MinorRadius);
        try {
            _occShapeAgent->updateShape(BRepBuilderAPI_MakeEdge(e).Shape());
        }
        catch (...)
        {
            printLog(tr("Failed to make curve!"), 3);
            return false;
        }
        return true;
    }
    // 构造函数
    FITKOCCModelThreePointsEllipse::FITKOCCModelThreePointsEllipse() :OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }
    // 更新数据
    bool FITKOCCModelThreePointsEllipse::update()
    {

        try
        {
            gce_MakeElips ellipse(
                gp_Pnt(m_MajorPoint[0], m_MajorPoint[1], m_MajorPoint[2]),
                gp_Pnt(m_MinorPoint[0], m_MinorPoint[1], m_MinorPoint[2]),
                gp_Pnt(m_Origin[0], m_Origin[1], m_Origin[2]));

            gp_Elips e = ellipse.Value();
            BRepBuilderAPI_MakeEdge edge(e);
            if (!edge.IsDone()) {
                printLog("Failed to build shape!", 3);
                return false;
            }

            auto loc = e.Location();

            m_Origin = { loc.X(), loc.Y(), loc.Z() };

            auto pos = e.Position();
            gp_Dir dir = pos.XDirection();
            m_XDirection = { dir.X(), dir.Y(), dir.Z() };
            dir = pos.YDirection();
            m_XDirection = { dir.X(), dir.Y(), dir.Z() };
            dir = pos.Direction();
            m_ZDirection = { dir.X(), dir.Y(), dir.Z() };

            _occShapeAgent->updateShape(edge.Shape());
            return true;
        }
        catch (...)
        {
            printLog("Failed to make ellipse!", 3);
            return false;
        }

    }
    // 构造函数
    FITKOCCModelEllipseArc::FITKOCCModelEllipseArc() :OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }
    // 更新数据
    bool FITKOCCModelEllipseArc::update()
    {
        gp_Elips e;
        gp_Pnt center(m_Origin[0], m_Origin[1], m_Origin[2]);
        e.SetPosition(
            gp_Ax2(center,
                gp_Dir(m_ZDirection[0], m_ZDirection[1], m_ZDirection[2]),
                gp_Dir(m_XDirection[0], m_XDirection[1], m_XDirection[2]))
        );
        e.SetMajorRadius(m_MajorRadius);
        e.SetMinorRadius(m_MinorRadius);

        gp_Pnt start(m_StartPoint[0], m_StartPoint[1], m_StartPoint[2]);
        gp_Pnt end(m_EndPoint[0], m_EndPoint[1], m_EndPoint[2]);

        try {
            GC_MakeArcOfEllipse ellipseArc(e, start, end, true);
            if (!ellipseArc.IsDone()) return false;

            _occShapeAgent->updateShape(BRepBuilderAPI_MakeEdge(ellipseArc.Value()).Shape());
        }
        catch (...)
        {
            printLog(tr("Failed to make curve!"), 3);
            return false;
        }
        return true;
    }

    // 构造函数
    FITKOCCModelHyperbola::FITKOCCModelHyperbola() :OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }
    // 更新数据
    bool FITKOCCModelHyperbola::update()
    {
        gp_Hypr h;
        gp_Pnt center(m_Origin[0], m_Origin[1], m_Origin[2]);
        h.SetPosition(
            gp_Ax2(center,
                gp_Dir(m_ZDirection[0], m_ZDirection[1], m_ZDirection[2]),
                gp_Dir(m_XDirection[0], m_XDirection[1], m_XDirection[2]))
        );
        h.SetMajorRadius(m_MajorRadius);
        h.SetMinorRadius(m_MinorRadius);
        try {
            _occShapeAgent->updateShape(BRepBuilderAPI_MakeEdge(h).Shape());
        }
        catch (...)
        {
            printLog(tr("Failed to make curve!"), 3);
            return false;
        }
        return true;
    }
    // 构造函数
    FITKOCCModelThreePointsHyperbola::FITKOCCModelThreePointsHyperbola() :OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }
    // 更新数据
    bool FITKOCCModelThreePointsHyperbola::update()
    {
        try
        {
            gce_MakeHypr hyper(
                gp_Pnt(m_MajorPoint[0], m_MajorPoint[1], m_MajorPoint[2]),
                gp_Pnt(m_MinorPoint[0], m_MinorPoint[1], m_MinorPoint[2]),
                gp_Pnt(m_Origin[0], m_Origin[1], m_Origin[2]));

            gp_Hypr h = hyper.Value();
            BRepBuilderAPI_MakeEdge edge(h);
            if (!edge.IsDone()) {
                printLog("Failed to build shape!", 3);
                return false;
            }

            auto loc = h.Location();
            m_Origin = { loc.X(), loc.Y(), loc.Z() };
            m_MajorRadius = h.MajorRadius();
            m_MinorRadius = h.MinorRadius();

            auto pos = h.Position();
            gp_Dir dir = pos.XDirection();
            m_XDirection = { dir.X(), dir.Y(), dir.Z() };
            dir = pos.YDirection();
            m_XDirection = { dir.X(), dir.Y(), dir.Z() };
            dir = pos.Direction();
            m_ZDirection = { dir.X(), dir.Y(), dir.Z() };

            _occShapeAgent->updateShape(edge.Shape());
            return true;
        }
        catch (...)
        {
            printLog("Failed to make hyperbola!", 3);
            return false;
        }
    }

    FITKOCCModelHyperbolaArc::FITKOCCModelHyperbolaArc() :OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelHyperbolaArc::update()
    {
        gp_Hypr h;
        gp_Pnt center(m_Origin[0], m_Origin[1], m_Origin[2]);
        h.SetPosition(
            gp_Ax2(center,
                gp_Dir(m_ZDirection[0], m_ZDirection[1], m_ZDirection[2]),
                gp_Dir(m_XDirection[0], m_XDirection[1], m_XDirection[2]))
        );
        h.SetMajorRadius(m_MajorRadius);
        h.SetMinorRadius(m_MinorRadius);

        gp_Pnt start(m_StartPoint[0], m_StartPoint[1], m_StartPoint[2]);
        gp_Pnt end(m_EndPoint[0], m_EndPoint[1], m_EndPoint[2]);

        try {
            GC_MakeArcOfHyperbola hyperArc(h, start, end, false);
            if (!hyperArc.IsDone()) return false;

            _occShapeAgent->updateShape(BRepBuilderAPI_MakeEdge(hyperArc.Value()).Shape());
        }
        catch (...)
        {
            printLog(tr("Failed to make curve!"), 3);
            return false;
        }
        return true;
    }



    // 构造函数
    FITKOCCModelParabola::FITKOCCModelParabola() :OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }
    // 更新数据
    bool FITKOCCModelParabola::update()
    {
        gp_Parab p;
        gp_Pnt center(m_Origin[0], m_Origin[1], m_Origin[2]);
        p.SetPosition(
            gp_Ax2(center,
                gp_Dir(m_ZDirection[0], m_ZDirection[1], m_ZDirection[2]),
                gp_Dir(m_XDirection[0], m_XDirection[1], m_XDirection[2]))
        );

        p.SetFocal(m_FocalLength);
        try {
            _occShapeAgent->updateShape(BRepBuilderAPI_MakeEdge(p).Shape());
        }
        catch (...)
        {
            printLog(tr("Failed to make curve!"), 3);
            return false;
        }
        return true;
    }
    // 构造函数
    FITKOCCModelThreePointsParabola::FITKOCCModelThreePointsParabola() :OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }
    // 更新数据
    bool FITKOCCModelThreePointsParabola::update()
    {
        try
        {
            auto p1 = m_Points.at(1);
            auto p2 = m_Points.at(2);
            auto p3 = m_Points.at(0);

            gp_Ax1 ax1(gp_Pnt(p1[0], p1[1], p1[2]), gp_Dir(p2[0] - p1[0], p2[1] - p1[1], p2[2] - p1[2]));

            gce_MakeParab parab(ax1, gp_Pnt(p3[0], p3[1], p3[2]));
            if (!parab.IsDone()) {
                printLog("Failed to make parabola!", 3);
                return false;
            }
            gp_Parab p = parab.Value();
            BRepBuilderAPI_MakeEdge edge(p);
            if (!edge.IsDone()) {
                printLog("Failed to build shape!", 3);
                return false;
            }

            auto loc = p.Location();
            m_Origin = { loc.X(), loc.Y(), loc.Z() };
            m_FocalLength = p.Focal();

            auto pos = p.Position();
            gp_Dir dir = pos.XDirection();
            m_XDirection = { dir.X(), dir.Y(), dir.Z() };
            dir = pos.YDirection();
            m_XDirection = { dir.X(), dir.Y(), dir.Z() };
            dir = pos.Direction();
            m_ZDirection = { dir.X(), dir.Y(), dir.Z() };

            _occShapeAgent->updateShape(edge.Shape());
            return true;
        }
        catch (const std::exception&)
        {
            printLog("Failed to make parabola!", 3);
            return false;
        }
    }
    FITKOCCModelParabolaArc::FITKOCCModelParabolaArc() :OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelParabolaArc::update()
    {

        gp_Pnt center(m_Origin[0], m_Origin[1], m_Origin[2]);
        gp_Ax2 ax2(center,
            gp_Dir(m_ZDirection[0], m_ZDirection[1], m_ZDirection[2]),
            gp_Dir(m_XDirection[0], m_XDirection[1], m_XDirection[2]));

        auto x = ax2.Direction();
        x = ax2.XDirection();

        gp_Parab p(ax2, m_FocalLength);

        gp_Pnt start(m_StartPoint[0], m_StartPoint[1], m_StartPoint[2]);
        gp_Pnt end(m_EndPoint[0], m_EndPoint[1], m_EndPoint[2]);

        try {
            GC_MakeArcOfParabola paraArc(p, start, end, true);
            if (!paraArc.IsDone()) return false;

            _occShapeAgent->updateShape(BRepBuilderAPI_MakeEdge(paraArc.Value()).Shape());
        }
        catch (...)
        {
            printLog(tr("Failed to make curve!"), 3);
            return false;
        }

        return true;
    }

    // 构造函数
    FITKOCCModelBeizeByControlPoints::FITKOCCModelBeizeByControlPoints() :OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }
    // 更新数据
    bool FITKOCCModelBeizeByControlPoints::update()
    {
        TColgp_Array1OfPnt aPoles(1, m_ControlPoints.size());
        for (int i = 0; i < m_ControlPoints.size(); ++i)
        {
            auto xyz = m_ControlPoints[i];
            aPoles.SetValue(i + 1, gp_Pnt(xyz[0], xyz[1], xyz[2]));
        }
        try {
            // Create Bezier curve.
            Handle(Geom_BezierCurve) aBezierCurve = new Geom_BezierCurve(aPoles);
            BRepBuilderAPI_MakeEdge anAisBezierCurve(aBezierCurve);
            if (!anAisBezierCurve.IsDone()) return false;

            _occShapeAgent->updateShape(anAisBezierCurve.Shape());
        }
        catch (...)
        {
            printLog(tr("Failed to make curve!"), 3);
            return false;
        }
        return true;
    }
    // 构造函数
    FITKOCCModelBsplineByThroughPoints::FITKOCCModelBsplineByThroughPoints() :OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }
    // 更新数据
    bool FITKOCCModelBsplineByThroughPoints::update()
    {
        if (!m_ThroughPointShapes.isEmpty()) {
            m_ThroughPoints.clear();
            auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
            for (auto virtualShape : m_ThroughPointShapes) {
                auto cmd = geoCmdList->getDataByID(virtualShape.CmdId);
                if (cmd == nullptr) return false;
                auto vShape = cmd->getShapeTopoByIndexT<FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSPoint, virtualShape.VirtualTopoIndex);
                if (vShape == nullptr) return false;
                TopoDS_Shape shape = vShape->getTopoShape();
                if (shape.ShapeType() != TopAbs_ShapeEnum::TopAbs_VERTEX) return false;
                TopoDS_Vertex vertex = TopoDS::Vertex(shape);
                if (vertex.IsNull()) return false;
                gp_Pnt pnt = BRep_Tool::Pnt(vertex);
                m_ThroughPoints.append({ pnt.X(), pnt.Y(), pnt.Z() });
            }
        }
        Handle(TColgp_HArray1OfPnt) aPoles = new TColgp_HArray1OfPnt(1, m_ThroughPoints.size());
        for (int i = 0; i < m_ThroughPoints.size(); ++i)
        {
            auto xyz = m_ThroughPoints[i];
            aPoles->SetValue(i + 1, gp_Pnt(xyz[0], xyz[1], xyz[2]));
        }

        try {
            GeomAPI_Interpolate bspline_generator(aPoles, false, 1e-7);
            bspline_generator.Perform();
            if (!bspline_generator.IsDone()) return false;

            Handle(Geom_BSplineCurve) bSplineCurve = bspline_generator.Curve();
            BRepBuilderAPI_MakeEdge anBCurve(bSplineCurve);
            if (!anBCurve.IsDone()) return false;

            _occShapeAgent->updateShape(anBCurve.Shape());
        }
        catch (...)
        {
            printLog(tr("Failed to make curve!"), 3);
            return false;
        }

        return true;
    }
    // 构造函数
    FITKOCCModelTrimmedCurve::FITKOCCModelTrimmedCurve() :OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }
    // 更新数据
    bool FITKOCCModelTrimmedCurve::update()
    {
        //gp_Pnt pt(m_X, m_Y, m_Z);

        //_occShapeAgent->updateShape(BRepBuilderAPI_MakeVertex(pt).Shape());
        return false;
    }



    FITKOCCModelOffsetCurve::FITKOCCModelOffsetCurve() :OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelOffsetCurve::update()
    {
        if (m_SourceCurve.isNull()) return false;

        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        auto cmd = geoCmdList->getDataByID(m_SourceCurve.CmdId);
        if (cmd == nullptr) return false;
        auto vShape = cmd->getShapeT<FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSEdge, m_SourceCurve.VirtualTopoId);
        if (vShape == nullptr) return false;
        TopoDS_Shape shape = vShape->getTopoShape();
        if (shape.ShapeType() != TopAbs_ShapeEnum::TopAbs_EDGE) return false;
        TopoDS_Edge edge = TopoDS::Edge(shape);
        if (edge.IsNull()) return false;

        try {
            Standard_Real first, last;
            Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, first, last);
            if (curve.IsNull()) return false;
            gp_Vec vec = curve->DN(first, 1);
            vec.Cross(gp_Vec(m_Dir[0], m_Dir[1], m_Dir[2]));
            Handle(Geom_OffsetCurve) offsetCurve = new Geom_OffsetCurve(curve, m_Offset, gp_Dir(-vec.X(), -vec.Y(), -vec.Z()));
            if (offsetCurve.IsNull()) return false;
            BRepBuilderAPI_MakeEdge offsetEdge(offsetCurve, first, last);
            offsetEdge.Build();
            if (!offsetEdge.IsDone()) {
                printLog("Failed to build shape!", 3);
                return false;
            }
            _occShapeAgent->updateShape(offsetEdge.Shape());
        }
        catch (...)
        {
            printLog(tr("Failed to make curve!"), 3);
            return false;
        }
        return true;
    }



    FITKOCCModelCurveProjectionOnSurface::FITKOCCModelCurveProjectionOnSurface() :OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }
    bool FITKOCCModelCurveProjectionOnSurface::update()
    {
        // 数据校验
        if (m_SourceCurve.isNull() || m_SourceSurface.isNull()) return false;

        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        /** 获取原始线形状 */
        /*@{*/
        // 实体形状
        auto curveCmd = geoCmdList->getDataByID(m_SourceCurve.CmdId);
        if (curveCmd == nullptr) {
            // 基准形状
            curveCmd = geoCmdList->getDatumManager()->getDataByID(m_SourceCurve.CmdId);
            if (curveCmd == nullptr) return false;
        }
        auto curveVShape = curveCmd->getShapeT<FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSEdge, m_SourceCurve.VirtualTopoId);
        if (curveVShape == nullptr) return false;
        TopoDS_Shape curveShape = curveVShape->getTopoShape();
        if (curveShape.ShapeType() != TopAbs_ShapeEnum::TopAbs_EDGE) return false;
        TopoDS_Edge edge = TopoDS::Edge(curveShape);
        if (edge.IsNull()) return false;

        Standard_Real first, last;
        Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, first, last);
        if (curve.IsNull()) return false;
        Handle(Geom_TrimmedCurve) trimmedCurve = new  Geom_TrimmedCurve(curve, first, last);
        if (trimmedCurve.IsNull()) return false;
        /*@}*/


        /** 获取原始面形状 */
        /*@{*/
        // 实体形状
        auto surfaceCmd = geoCmdList->getDataByID(m_SourceSurface.CmdId);
        if (surfaceCmd == nullptr) {
            // 基准形状
            surfaceCmd = geoCmdList->getDatumManager()->getDataByID(m_SourceSurface.CmdId);
            if (surfaceCmd == nullptr) return false;
        }
        auto surfaceVShape = surfaceCmd->getShapeT<FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSFace, m_SourceSurface.VirtualTopoId);
        if (surfaceVShape == nullptr) return false;
        TopoDS_Shape surfaceShape = surfaceVShape->getTopoShape();
        if (surfaceShape.ShapeType() != TopAbs_ShapeEnum::TopAbs_FACE) return false;
        TopoDS_Face face = TopoDS::Face(surfaceShape);
        if (face.IsNull()) return false;

        Handle(Geom_Surface) surface = BRep_Tool::Surface(face);
        if (surface.IsNull()) return false;
        /*@}*/

        try {
            /** 计算投影曲线 */
            /*@{*/
            Handle(Geom_Curve) projCurve = GeomProjLib::Project(trimmedCurve, surface);
            if (projCurve.IsNull()) {
                printLog("Failed to build shape!", 3);
                return false;
            }
            /*@}*/

            /** 计算端点投影 */
            /*@{*/
            gp_Pnt pntFirst = curve->Value(first);
            gp_Pnt pntLast = curve->Value(last);

            GeomAPI_ProjectPointOnSurf projFirst(pntFirst, surface);
            GeomAPI_ProjectPointOnSurf projLast(pntLast, surface);

            if (projFirst.NbPoints() != 1 || projLast.NbPoints() != 1/* || projFirst.Point(1).Distance(projLast.Point(1)) < Precision::Confusion()*/) {
                printLog("Failed to build projection!", 3);
                return false;
            }
            pntFirst = projFirst.Point(1);
            pntLast = projLast.Point(1);

            Standard_Real newFirst, newLast;
            if (
                !GeomLib_Tool::Parameter(projCurve, pntFirst, Precision::Confusion(), newFirst) ||
                !GeomLib_Tool::Parameter(projCurve, pntLast, Precision::Confusion(), newLast)) {
                printLog("Failed to build projection!", 3);
                return false;
            }
            /*@}*/

            /** 转换拓扑形状 */
            /*@{*/
            BRepBuilderAPI_MakeEdge projEdge(projCurve, newFirst, newLast);
            projEdge.Build();
            if (!projEdge.IsDone()) {
                printLog("Failed to build shape!", 3);
                return false;
            }
            /*@}*/

            _occShapeAgent->updateShape(projEdge.Shape());
            //_occShapeAgent->updateShape(projEdge.Shape());
        }
        catch (...)
        {
            printLog(tr("Failed to make curve!"), 3);
            return false;
        }
        return true;
    }

    FITKOCCModelSurfaceIntersectionSurface::FITKOCCModelSurfaceIntersectionSurface() :OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelSurfaceIntersectionSurface::update()
    {
        // 数据校验
        if (m_SourceSurface1.isNull() || m_SourceSurface2.isNull()) return false;

        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        /** 获取原始面1形状 */
        /*@{*/
        auto surface1Cmd = geoCmdList->getDataByID(m_SourceSurface1.CmdId);
        if (surface1Cmd == nullptr) {
            surface1Cmd = geoCmdList->getDatumManager()->getDataByID(m_SourceSurface1.CmdId);
            if (surface1Cmd == nullptr) return false;
        }
        auto surface1VShape = surface1Cmd->getShapeT<FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSFace, m_SourceSurface1.VirtualTopoId);
        if (surface1VShape == nullptr) return false;
        TopoDS_Shape surface1Shape = surface1VShape->getTopoShape();
        if (surface1Shape.ShapeType() != TopAbs_ShapeEnum::TopAbs_FACE) return false;
        TopoDS_Face face1 = TopoDS::Face(surface1Shape);
        if (face1.IsNull()) return false;

        Handle(Geom_Surface) surface1 = BRep_Tool::Surface(face1);
        if (surface1.IsNull()) return false;
        /*@}*/


        /** 获取原始面2形状 */
        /*@{*/
        auto surface2Cmd = geoCmdList->getDataByID(m_SourceSurface2.CmdId);
        if (surface2Cmd == nullptr) {
            surface2Cmd = geoCmdList->getDatumManager()->getDataByID(m_SourceSurface2.CmdId);
            if (surface2Cmd == nullptr) return false;
        }
        auto surface2VShape = surface2Cmd->getShapeT<FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSFace, m_SourceSurface2.VirtualTopoId);
        if (surface2VShape == nullptr) return false;
        TopoDS_Shape surface2Shape = surface2VShape->getTopoShape();
        if (surface2Shape.ShapeType() != TopAbs_ShapeEnum::TopAbs_FACE) return false;
        TopoDS_Face face2 = TopoDS::Face(surface2Shape);
        if (face2.IsNull()) return false;

        Handle(Geom_Surface) surface2 = BRep_Tool::Surface(face2);

        if (surface2.IsNull()) return false;
        /*@}*/

        try {
            /** 使用BRepAlgoAPI_Section计算相交 */
            /*@{*/
            BRepAlgoAPI_Section ss(face1, face2);
            if (!ss.IsDone()) {
                printLog("Failed to build shape!", 3);
                return false;
            }

            if (FITKOCCExtendTool::IsEmptyShape(ss.Shape())) {
                printLog("Failed to build shape!", 3);
                return false;
            }
            _occShapeAgent->updateShape(ss.Shape());
            /*@}*/


            /** 使用GeomAPI_IntSS计算相交 */
            /*@{*/
            /*GeomAPI_IntSS ss(surface1, surface2, 1.0e-7);
            if (!ss.IsDone()) {
                printLog("Failed to build shape!", 3);
                return false;
            }
            // 转换拓扑形状
            if (ss.NbLines() == 0) return false;
            Handle(Geom_Curve) c = ss.Line(1);
            BRepBuilderAPI_MakeEdge interEdge(c, c->FirstParameter(), c->LastParameter());
            interEdge.Build();
            if (!interEdge.IsDone()) {
                printLog("Failed to build shape!", 3);
                return false;
            }
            _occShapeAgent->updateShape(ss.Shape());
            */
            /*@}*/

            /** 使用BOPAlgo_PaveFiller计算相交 */
            /*@{*/
            /*BOPAlgo_PaveFiller ss;
            ss.AddArgument(face1);
            ss.AddArgument(face2);
            ss.Perform();
            if (ss.HasErrors()) {
                printLog("Failed to build shape!", 3);
                return false;
            }
            // 转换拓扑形状
            int count = ss.DS().NbShapes();
            if (count == 0) return false;

            for (int i = 0; i < count; ++i) {
                auto shape = ss.DS().Shape(i + 1);
                if (shape.ShapeType() != TopAbs_ShapeEnum::TopAbs_EDGE) continue;
                _occShapeAgent->updateShape(shape);
                break;
            }*/
            /*@}*/
        }
        catch (...)
        {
            printLog(tr("Failed to make curve!"), 3);
            return false;
        }
        return true;
    }

    FITKOCCModelBridgeCurve::FITKOCCModelBridgeCurve() :OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelBridgeCurve::update()
    {
        if (m_SourceCurve1.isNull() || m_SourceCurve2.isNull()) return false;
        if (m_Parameter1 < 1 || m_Parameter2 < 1) {
            printLog(tr("Parameter must greater than or equal 1.0."), 3);
            return false;
        }

        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();


        /** 第一条曲线端点 */
        /*@{*/
        auto curveCmd1 = geoCmdList->getDataByID(m_SourceCurve1.CmdId);
        if (curveCmd1 == nullptr) return false;
        auto curveVShape1 = curveCmd1->getShapeT<FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSEdge, m_SourceCurve1.VirtualTopoId);
        if (curveVShape1 == nullptr) return false;
        TopoDS_Shape curveShape1 = curveVShape1->getTopoShape();
        if (curveShape1.ShapeType() != TopAbs_ShapeEnum::TopAbs_EDGE) return false;
        TopoDS_Edge edge1 = TopoDS::Edge(curveShape1);
        if (edge1.IsNull()) return false;

        Standard_Real first;
        Standard_Real last;
        Handle(Geom_Curve) curve1 = BRep_Tool::Curve(edge1, first, last);
        if (curve1.IsNull()) return false;

        try {
            gp_Pnt pnt1 = m_UseStartEnd1 ? curve1->Value(first) : curve1->Value(last);
            gp_Vec vec1 = m_UseStartEnd1 ? curve1->DN(first, 1) : curve1->DN(last, 1);
            vec1.Scale(m_Parameter1 / vec1.Magnitude());
            gp_Pnt pnt2;
            if (m_UseStartEnd1)
                pnt2 = gp_Pnt(pnt1.X() - vec1.X(), pnt1.Y() - vec1.Y(), pnt1.Z() - vec1.Z());
            else
                pnt2 = gp_Pnt(pnt1.X() + vec1.X(), pnt1.Y() + vec1.Y(), pnt1.Z() + vec1.Z());
            /*@}*/

            /** 第二条曲线端点 */
            /*@{*/
            auto curveCmd2 = geoCmdList->getDataByID(m_SourceCurve2.CmdId);
            if (curveCmd2 == nullptr) return false;
            auto curveVShape2 = curveCmd2->getShapeT<FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSEdge, m_SourceCurve2.VirtualTopoId);
            if (curveVShape2 == nullptr) return false;
            TopoDS_Shape curveShape2 = curveVShape2->getTopoShape();
            if (curveShape2.ShapeType() != TopAbs_ShapeEnum::TopAbs_EDGE) return false;
            TopoDS_Edge edge2 = TopoDS::Edge(curveShape2);
            if (edge2.IsNull()) return false;

            Handle(Geom_Curve) curve2 = BRep_Tool::Curve(edge2, first, last);
            if (curve2.IsNull()) return false;

            gp_Pnt pnt4 = m_UseStartEnd2 ? curve2->Value(first) : curve2->Value(last);
            gp_Vec vec2 = m_UseStartEnd2 ? curve2->DN(first, 1) : curve2->DN(last, 1);
            vec2.Scale(m_Parameter2 / vec2.Magnitude());
            gp_Pnt pnt3;
            if (m_UseStartEnd2)
                pnt3 = gp_Pnt(pnt4.X() - vec2.X(), pnt4.Y() - vec2.Y(), pnt4.Z() - vec2.Z());
            else
                pnt3 = gp_Pnt(pnt4.X() + vec2.X(), pnt4.Y() + vec2.Y(), pnt4.Z() + vec2.Z());
            /*@}*/


            TColgp_Array1OfPnt pnts(1, 4);
            pnts.SetValue(1, pnt1);
            pnts.SetValue(2, pnt2);
            pnts.SetValue(3, pnt3);
            pnts.SetValue(4, pnt4);


            Standard_Integer KNum = 3;
            TColStd_Array1OfReal knots(1, KNum);
            for (int i = 0; i < KNum; ++i)
            {
                knots.SetValue(i + 1, i);
            }
            TColStd_Array1OfInteger mults(1, KNum);
            for (int i = 0; i < KNum; ++i)
            {
                if (i == 0 || i == KNum - 1)
                {
                    mults.SetValue(i + 1, 3);
                }
                else
                {
                    mults.SetValue(i + 1, 1);
                }
            }

            Handle(Geom_BSplineCurve) hCurve = new Geom_BSplineCurve(pnts, knots, mults, 2);
            GeomLib_Tool parameterTool;
            Standard_Real p1, p4;
            parameterTool.Parameter(hCurve, pnt1, Precision::Confusion(), p1);
            parameterTool.Parameter(hCurve, pnt4, Precision::Confusion(), p4);
            BRepBuilderAPI_MakeEdge anEdge(hCurve, p1, p4);
            _occShapeAgent->updateShape(anEdge.Shape());
        }
        catch (...)
        {
            printLog(tr("Failed to make curve!"), 3);
            return false;
        }
        return true;
    }

    FITKOCCModelSurfaceEdge::FITKOCCModelSurfaceEdge() :OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelSurfaceEdge::update()
    {
        if (m_SourceSurface.isNull() || m_EdgeVirtualTopoId < 0) return false;
        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        /** 获取原始面形状 */
         /*@{*/
        auto surfaceCmd = geoCmdList->getDataByID(m_SourceSurface.CmdId);
        if (surfaceCmd == nullptr) return false;
        auto edgeVShape = surfaceCmd->getShapeT<FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSEdge, m_EdgeVirtualTopoId);
        if (edgeVShape == nullptr) return false;
        TopoDS_Shape edgeShape = edgeVShape->getTopoShape();
        if (edgeShape.ShapeType() != TopAbs_ShapeEnum::TopAbs_EDGE) return false;
        TopoDS_Edge edge = TopoDS::Edge(edgeShape);
        if (edge.IsNull()) return false;
        /*@}*/

        try {
            Standard_Real first, last;
            Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, first, last);
            if (curve.IsNull()) return false;
            BRepBuilderAPI_MakeEdge anEdge(curve, first, last);

            _occShapeAgent->updateShape(anEdge.Shape());

        }
        catch (...)
        {
            printLog(tr("Failed to make curve!"), 3);
            return false;
        }
        return true;
    }




}
