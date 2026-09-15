/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    FITKOCCModelNearestPointLine.cpp
 * @brief   OCC Nearest Point Line Command Class Implementation
 * @date    2025-03-27
 * @author  libaojun
 *********************************************************************/

#include "FITKOCCModelNearestPointLine.h"
#include <GeomAPI_ProjectPointOnCurve.hxx>
#include <GeomAPI_ProjectPointOnSurf.hxx>
#include <gp_Pnt.hxx>
#include <Geom_Curve.hxx>
#include <Geom_Surface.hxx>
#include <BRep_Tool.hxx>
#include <TopoDS.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <TopoDS_Edge.hxx>
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
#include "FITKOCCVirtualTopoCreator.h"

namespace OCC
{
    FITKOCCModelNearestPointLine::FITKOCCModelNearestPointLine()
        :OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelNearestPointLine::update()
    {
        bool ok = false;
        if (getShapeType() == Curve)
        {
            ok = handleCurve();
        }
        else if (getShapeType() == Surface)
        {
            ok = handleSurface();
        }
        if (!ok) return false;

        // 创建一条线，起点是_point，终点是_nearestPoint
        try
        {
            gp_Pnt startPoint(_point[0], _point[1], _point[2]);
            gp_Pnt endPoint(_nearestPoint[0], _nearestPoint[1], _nearestPoint[2]);
            TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(startPoint, endPoint);
            // 更新形状代理
            _occShapeAgent->updateShape(edge);
        }
        catch (const Standard_Failure& e)
        {
            std::cerr << "Error: " << e.GetMessageString() << std::endl;
            return false;
        }
        catch (...)
        {
            std::cerr << "Unknown error occurred while creating the edge." << std::endl;
            return false;
        }

        return true;
    }

    bool FITKOCCModelNearestPointLine::handleCurve()
    {
        try
        {
            gp_Pnt point(_point[0], _point[1], _point[2]);
            FITKAbsGeoCommand* comm = FITKDATAREPO->getTDataByID<FITKAbsGeoCommand>(_shape.CmdId);
            if (comm == nullptr) return false;
            FITKOCCTopoShape* vOCCShapeEdge = comm->getShapeTopoByIndexT<OCC::FITKOCCTopoShape>
                (Interface::FITKGeoEnum::VTopoShapeType::VSEdge, _shape.VirtualTopoIndex);
            if (vOCCShapeEdge == nullptr) return false;

            double start = 0.0, end = 0.0;
            Handle(Geom_Curve) curve = BRep_Tool::Curve(TopoDS::Edge(vOCCShapeEdge->getTopoShape()), start, end);

            if (curve.IsNull() || fabs(start - end) < 1e-6)
            {
                return false;
            }

            GeomAPI_ProjectPointOnCurve projector(point, curve);
            if (projector.NbPoints() > 0)
            {
                gp_Pnt nearestPoint = projector.NearestPoint();
                _nearestPoint[0] = nearestPoint.X();
                _nearestPoint[1] = nearestPoint.Y();
                _nearestPoint[2] = nearestPoint.Z();
                return true;
            }
        }
        catch (const Standard_Failure& e)
        {
            std::cerr << "Error: " << e.GetMessageString() << std::endl;
            return false;
        }
        catch (...)
        {
            std::cerr << "Unknown error occurred while handling the curve." << std::endl;
            return false;
        }

        return false;
    }

    bool FITKOCCModelNearestPointLine::handleSurface()
    {
        try
        {
            gp_Pnt point(_point[0], _point[1], _point[2]);
            FITKAbsGeoCommand* comm = FITKDATAREPO->getTDataByID<FITKAbsGeoCommand>(_shape.CmdId);
            if (comm == nullptr) return false;
            FITKOCCTopoShape* vOCCShapeFace = comm->getShapeTopoByIndexT<OCC::FITKOCCTopoShape>
                (Interface::FITKGeoEnum::VTopoShapeType::VSFace, _shape.VirtualTopoIndex);
            if (vOCCShapeFace == nullptr) return false;

            Handle(Geom_Surface) surface = BRep_Tool::Surface(TopoDS::Face(vOCCShapeFace->getTopoShape()));

            GeomAPI_ProjectPointOnSurf projector(point, surface);
            if (projector.NbPoints() > 0)
            {
                gp_Pnt nearestPoint = projector.NearestPoint();
                _nearestPoint[0] = nearestPoint.X();
                _nearestPoint[1] = nearestPoint.Y();
                _nearestPoint[2] = nearestPoint.Z();
                return true;
            }
        }
        catch (const Standard_Failure& e)
        {
            std::cerr << "Error: " << e.GetMessageString() << std::endl;
            return false;
        }
        catch (...)
        {
            std::cerr << "Unknown error occurred while handling the surface." << std::endl;
            return false;
        }

        return false;
    }
}


