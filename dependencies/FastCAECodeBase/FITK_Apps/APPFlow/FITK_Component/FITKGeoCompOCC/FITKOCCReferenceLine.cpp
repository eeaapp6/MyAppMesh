/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCReferenceLine.h"

// OCC
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <gp_Pln.hxx>
#include <TopoDS_Edge.hxx>
#include <IntAna_QuadQuadGeo.hxx>
#include <gp_Trsf.hxx>

namespace OCC {
    FITKOCCReferenceLine::FITKOCCReferenceLine() :OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }
    bool FITKOCCReferenceLine::update()
    {
        try
        {
            BRepBuilderAPI_MakeEdge edge(gp_Pnt(m_pos[0], m_pos[1], m_pos[2]), gp_Pnt(m_pos2[0], m_pos2[1], m_pos2[2]));
            _occShapeAgent->updateShape(edge);
            return true;
        }
        catch (...)
        {
            printLog(tr("Failed to make line."), 3);
            return false;
        }
    }

    // 面面交线创建基准线。
    //@{
    FITKOCCReferenceLineByIntersectPlanes::FITKOCCReferenceLineByIntersectPlanes() :OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCReferenceLineByIntersectPlanes::update()
    {
        try
        {
            // 创建两个平面。
            gp_Pln pln1(gp_Pnt(m_Plane1.at(0), m_Plane1.at(1), m_Plane1.at(2)), 
                gp_Dir(m_Plane1.at(3), m_Plane1.at(4), m_Plane1.at(5)));
            gp_Pln pln2(gp_Pnt(m_Plane2.at(0), m_Plane2.at(1), m_Plane2.at(2)), 
                gp_Dir(m_Plane2.at(3), m_Plane2.at(4), m_Plane2.at(5)));

            // 尝试获取交线。
            IntAna_QuadQuadGeo intQQ(pln1, pln2, Precision::Angular(), Precision::Confusion());
            if (!intQQ.IsDone())
            {
                return false;
            }

            IntAna_ResultType iType = intQQ.TypeInter();
            if (iType != IntAna_ResultType::IntAna_Line || intQQ.NbSolutions() == 0)
            {
                return false;
            }

            gp_Lin lin = intQQ.Line(1);
            const gp_Ax1 & ax1 = lin.Position();
            m_pos[0] = ax1.Location().Coord(1);
            m_pos[1] = ax1.Location().Coord(2);
            m_pos[2] = ax1.Location().Coord(3);

            m_pos2[0] = m_pos[0] + ax1.Direction().Coord(1);
            m_pos2[1] = m_pos[1] + ax1.Direction().Coord(2);
            m_pos2[2] = m_pos[2] + ax1.Direction().Coord(3);

            TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(lin);
            _occShapeAgent->updateShape(edge);
            return true;
        }
        catch (...)
        {
            printLog(tr("Failed to make line."), 3);
            return false;
        }
    }
    //@}

    // 旋转轴创建基准线。
    //@{
    FITKOCCReferenceLineByRotateLine::FITKOCCReferenceLineByRotateLine() :OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCReferenceLineByRotateLine::update()
    {
        try
        {
            // 创建旋转变换。
            gp_Trsf tsf;
            gp_Pnt pnt(m_AxisStart[0], m_AxisStart[1], m_AxisStart[2]);
            gp_Dir dir(m_AxisEnd[0] - m_AxisStart[0], m_AxisEnd[1] - m_AxisStart[1], m_AxisEnd[2] - m_AxisStart[2]);
            gp_Ax1 axis(pnt, dir);
            tsf.SetRotation(axis, m_Angle / 180 * 3.1415926);

            gp_Pnt pt1(m_LineStart[0], m_LineStart[1], m_LineStart[2]);
            gp_Pnt pt2(m_LineEnd[0], m_LineEnd[1], m_LineEnd[2]);
            pt1.Transform(tsf);
            pt2.Transform(tsf);

            m_pos[0] = pt1.Coord(1);
            m_pos[1] = pt1.Coord(2);
            m_pos[2] = pt1.Coord(3);

            m_pos2[0] = pt2.Coord(1);
            m_pos2[1] = pt2.Coord(2);
            m_pos2[2] = pt2.Coord(3);

            TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(pt1, pt2);
            _occShapeAgent->updateShape(edge);
            return true;
        }
        catch (...)
        {
            printLog(tr("Failed to make line."), 3);
            return false;
        }
    }
    //@}
}
