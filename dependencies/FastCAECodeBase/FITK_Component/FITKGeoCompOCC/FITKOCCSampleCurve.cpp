/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCSampleCurve.h"
#include <GCPnts_UniformAbscissa.hxx>
#include <GCPnts_QuasiUniformAbscissa.hxx>
#include <GeomAdaptor_Curve.hxx>
#include <BRep_Builder.hxx>
#include <TopoDS_Compound.hxx>
#include <BRep_Tool.hxx>
#include <TopoDS.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <cmath>
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
#include "FITKOCCVirtualTopoCreator.h"

namespace OCC
{
    FITKOCCSampleCurve::FITKOCCSampleCurve() :OCCShapeAgent(this)
    {
        // 构造函数的具体实现（如果有需要的话）
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCSampleCurve::update()
    {
        // 获取采样的曲线对象 
        if (_sampleCurve.isNull())
        {
            return false;
        }

        // 获取 TopoDS_Shape 对象
        FITKAbsGeoCommand* comm = FITKDATAREPO->getTDataByID<FITKAbsGeoCommand>(_sampleCurve.CmdId);
        if (comm == nullptr) return false;
        FITKOCCTopoShape* vOCCShapeEdge = comm->getShapeTopoByIndexT<OCC::FITKOCCTopoShape>
            (Interface::FITKGeoEnum::VTopoShapeType::VSEdge, _sampleCurve.VirtualTopoIndex);
        if (vOCCShapeEdge == nullptr) return false; 
        // 根据 sampleCurve 获取实际的 TopoDS_Shape 对象
        double start = 0.0,end = 0.0;
        Handle(Geom_Curve) curve = BRep_Tool::Curve(TopoDS::Edge(vOCCShapeEdge->getTopoShape()),start, end);

        if (curve.IsNull() || fabs(start-end )< 1e-6)
        {
            return false;
        }

        std::vector<gp_Pnt> sampledPoints;
        switch (getSampleMethod())
        {
        case Parametric:
            sampledPoints = sampleParametric(curve, start, end, getSamplePointCount());
            break;
        case ArcLength:
            sampledPoints = sampleArcLength(curve, start, end, getSamplePointCount());
            break;
        case ChordLength:
            sampledPoints = sampleChordLength(curve, start, end, getSamplePointCount());
            break;
        default:
            return false;
        }

        // 将采样点转换为 TopoDS_Shape 并添加到组合体中
        BRep_Builder builder;
        TopoDS_Compound compound;
        builder.MakeCompound(compound);
        for (const auto& point : sampledPoints)
        {
            builder.Add(compound, BRepBuilderAPI_MakeVertex(point).Shape());
        }

        // 设置结果形状
        _occShapeAgent->updateShape(compound);
        return true;
    }

    std::vector<gp_Pnt> FITKOCCSampleCurve::sampleParametric(const Handle(Geom_Curve)& curve, double start, double end, int numPoints)
    {
        std::vector<gp_Pnt> points;
        for (int i = 0; i < numPoints; ++i)
        {
            double u = start + (end - start) * i / (numPoints - 1);
            gp_Pnt point;
            curve->D0(u, point);
            points.push_back(point);
        }
        return points;
    }

    std::vector<gp_Pnt> FITKOCCSampleCurve::sampleArcLength(const Handle(Geom_Curve)& curve, double start, double end, int numPoints)
    {
        std::vector<gp_Pnt> points;
        GeomAdaptor_Curve adaptorCurve(curve, start, end);
        GCPnts_UniformAbscissa uniformAbscissa(adaptorCurve, numPoints);
        if (!uniformAbscissa.IsDone())
        {
            return points;
        }
        for (int i = 1; i <= numPoints; ++i)
        {
            double u = uniformAbscissa.Parameter(i);
            gp_Pnt point;
            curve->D0(u, point);
            points.push_back(point);
        }
        return points;
    }

    std::vector<gp_Pnt> FITKOCCSampleCurve::sampleChordLength(const Handle(Geom_Curve)& curve, double start, double end, int numPoints)
    {
        std::vector<gp_Pnt> points;
        GeomAdaptor_Curve adaptorCurve(curve, start, end);
        GCPnts_QuasiUniformAbscissa quasiUniformAbscissa(adaptorCurve, numPoints);
        if (!quasiUniformAbscissa.IsDone())
        {
            return points;
        }
        for (int i = 1; i <= numPoints; ++i)
        {
            double u = quasiUniformAbscissa.Parameter(i);
            gp_Pnt point;
            curve->D0(u, point);
            points.push_back(point);
        }
        return points;
    }
}
