/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    FITKOCCModelExtractCenter.cpp
 * @brief   OCC Extract Center Command Class Implementation
 * @date    2025-03-27
 * @author  libaojun
 *********************************************************************/

#include "FITKOCCModelExtractCenter.h"
#include <Geom_SphericalSurface.hxx>
#include <Geom_Circle.hxx>
#include <BRep_Tool.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>
#include <GeomAdaptor_Surface.hxx>
#include <GeomAdaptor_Curve.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <gp_Pnt.hxx>
#include <TopoDS_Vertex.hxx>
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
#include "FITKOCCVirtualTopoCreator.h"

namespace OCC
{
    FITKOCCModelExtractCenter::FITKOCCModelExtractCenter()
        : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelExtractCenter::update()
    {
        bool ok = false;
        if (getShapeType() == Sphere)
        {
            ok = handleSphere();
        }
        else if (getShapeType() == Arc)
        {
            ok = handleArc();
        }
        if (!ok) return false;

        try
        {
            // 定义点的坐标
            gp_Pnt point(_center[0], _center[1], _center[2]);
            // 使用 BRepBuilderAPI_MakeVertex 创建一个 TopoDS_Vertex 对象
            TopoDS_Vertex vertex = BRepBuilderAPI_MakeVertex(point);

            _occShapeAgent->updateShape(vertex);
        }
        catch (const Standard_Failure& e)
        {
            std::cerr << "Error: " << e.GetMessageString() << std::endl;
            return false;
        }
        catch (...)
        {
            std::cerr << "Unknown error occurred while creating the vertex." << std::endl;
            return false;
        }

        return ok;
    }

    bool FITKOCCModelExtractCenter::handleSphere()
    {
        try
        {
            FITKAbsGeoCommand* comm = FITKDATAREPO->getTDataByID<FITKAbsGeoCommand>(_shape.CmdId);
            if (comm == nullptr) return false;
            FITKOCCTopoShape* vOCCShapeFace = comm->getShapeTopoByIndexT<OCC::FITKOCCTopoShape>
                (Interface::FITKGeoEnum::VTopoShapeType::VSFace, _shape.VirtualTopoIndex);
            if (vOCCShapeFace == nullptr) return false;

            Handle(Geom_Surface) surface = BRep_Tool::Surface(TopoDS::Face(vOCCShapeFace->getTopoShape()));
            GeomAdaptor_Surface adaptorSurface(surface);
            if (adaptorSurface.GetType() == GeomAbs_Sphere)
            {
                Handle(Geom_SphericalSurface) sphericalSurface = Handle(Geom_SphericalSurface)::DownCast(surface);
                if (!sphericalSurface.IsNull())
                {
                    gp_Pnt center = sphericalSurface->Location();
                    _center[0] = center.X();
                    _center[1] = center.Y();
                    _center[2] = center.Z();
                    return true;
                }
            }
        }
        catch (const Standard_Failure& e)
        {
            std::cerr << "Error: " << e.GetMessageString() << std::endl;
            return false;
        }
        catch (...)
        {
            std::cerr << "Unknown error occurred while handling the sphere." << std::endl;
            return false;
        }

        return false;
    }

    bool FITKOCCModelExtractCenter::handleArc()
    {
        try
        {
            FITKAbsGeoCommand* comm = FITKDATAREPO->getTDataByID<FITKAbsGeoCommand>(_shape.CmdId);
            if (comm == nullptr) return false;
            FITKOCCTopoShape* vOCCShapeEdge = comm->getShapeTopoByIndexT<OCC::FITKOCCTopoShape>
                (Interface::FITKGeoEnum::VTopoShapeType::VSEdge, _shape.VirtualTopoIndex);
            if (vOCCShapeEdge == nullptr) return false;
            double start = 0.0, end = 0.0;
            Handle(Geom_Curve) curve = BRep_Tool::Curve(TopoDS::Edge(vOCCShapeEdge->getTopoShape()), start, end);
            GeomAdaptor_Curve adaptorCurve(curve);
            if (adaptorCurve.GetType() == GeomAbs_Circle)
            {
                Handle(Geom_Circle) circle = Handle(Geom_Circle)::DownCast(curve);
                if (!circle.IsNull())
                {
                    gp_Pnt center = circle->Location();
                    _center[0] = center.X();
                    _center[1] = center.Y();
                    _center[2] = center.Z();
                    return true;
                }
            }
        }
        catch (const Standard_Failure& e)
        {
            std::cerr << "Error: " << e.GetMessageString() << std::endl;
            return false;
        }
        catch (...)
        {
            std::cerr << "Unknown error occurred while handling the arc." << std::endl;
            return false;
        }

        return false;
    }
}




