/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCReferencePlane.h"

#include <FITK_Interface/FITKInterfaceGeometry/FITKGeoEnum.h>
#include <FITK_Interface/FITKInterfaceGeometry/FITKVirtualTopoManager.h>
#include "FITKOCCVirtualTopoCreator.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelSurface.h"

#include <TopoDS.hxx>
#include <TopoDS_Face.hxx>
#include <Geom_Plane.hxx>
#include <BRep_Tool.hxx>
#include <gp_Vec.hxx>
#include <BRepOffset_MakeOffset.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <TopoDS_Shell.hxx>
#include <TopExp_Explorer.hxx>
#include <gce_MakePln.hxx>
#include <gp_Trsf.hxx>


namespace OCC {
    FITKOCCReferencePlane::FITKOCCReferencePlane() :OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }
    bool FITKOCCReferencePlane::update()
    {
        return false;
    }

    FITKOCCReferenceOffsetPlane::FITKOCCReferenceOffsetPlane() :OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCReferenceOffsetPlane::update()
    {
        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        auto cmd = geoCmdList->getDataByID(m_SourceSurface.CmdId);

        // 实体面
        if (cmd == nullptr) 
        {
            // 基准面
            cmd = geoCmdList->getDatumManager()->getDataByID(m_SourceSurface.CmdId);
        }

        gp_Dir normalDir, direction;
        TopoDS_Shape topoShape;

        // 兼容老版本使用几何数据。
        if (cmd)
        {
            auto vshape = cmd->getShapeT<FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSFace, m_SourceSurface.VirtualTopoId);
            if (vshape == nullptr) return false;
            auto topoShape = vshape->getTopoShape();
            if (topoShape.IsNull() || topoShape.ShapeType() != TopAbs_FACE) return false;

            Handle(Geom_Surface) surface = BRep_Tool::Surface(TopoDS::Face(topoShape));
            if (surface.IsNull() || surface->DynamicType() != STANDARD_TYPE(Geom_Plane)) return false;
            Handle(Geom_Plane) plane = Handle(Geom_Plane)::DownCast(surface);
            auto dir = plane->Position().Direction();
            normalDir = gp_Dir(dir.X(), dir.Y(), dir.Z());
            direction = gp_Dir(m_Direction[0], m_Direction[1], m_Direction[2]);
        }
        else
        {
            gp_Pnt org(m_SourcePlaneOrigin[0], m_SourcePlaneOrigin[1], m_SourcePlaneOrigin[2]);
            normalDir = gp_Dir(m_SourcePlaneNormal[0], m_SourcePlaneNormal[1], m_SourcePlaneNormal[2]);
            direction = normalDir;
            gp_Pln pln(org, normalDir);
        }

        if (!normalDir.IsParallel(direction, Precision::Angular())) {
            printLog(tr("The offset direction must be parallel to the source plane normal."), 3);
            return false;
        }
        // 反向
        if (normalDir.IsOpposite(direction, Precision::Angular())) {
            m_Offset *= -1;
        }

        if (cmd)
        {
            BRepOffset_MakeOffset offsetSurface(topoShape, m_Offset, 1e-6);
            auto newShape = offsetSurface.Shape();
            if (newShape.IsNull()) return false;

            if (newShape.ShapeType() == TopAbs_FACE)
            {
                TopoDS_Face face = TopoDS::Face(newShape);
                if (face.IsNull()) return false;
                Handle(Geom_Surface) surface = BRep_Tool::Surface(face);
                auto type = surface->DynamicType();
                if (surface.IsNull() || surface->DynamicType() != STANDARD_TYPE(Geom_Plane)) return false;
                Handle(Geom_Plane) plane = Handle(Geom_Plane)::DownCast(surface);
                auto pos = plane->Position();
                auto loc = plane->Position().Location();
                auto direction = plane->Position().Direction();
                auto yDirection = plane->Position().YDirection();
                setPosition(loc.X(), loc.Y(), loc.Z());
                setNormal(direction.X(), direction.Y(), direction.Z());
                setUp(yDirection.X(), yDirection.Y(), yDirection.Z());
                try
                {
                    gp_Pln pln(gp_Pnt(m_pos[0], m_pos[1], m_pos[2]), gp_Dir(m_nor[0], m_nor[1], m_nor[2]));

                    BRepBuilderAPI_MakeFace face(pln);
                    _occShapeAgent->updateShape(face);
                    return true;
                }
                catch (...)
                {
                    printLog(tr("Failed to make plane."), 3);
                    return false;
                }
            }
            else if (newShape.ShapeType() == TopAbs_SHELL)
            {
                TopoDS_Shell shell = TopoDS::Shell(newShape);
                if (shell.IsNull()) return false;

                TopExp_Explorer exp;
                for (exp.Init(shell, TopAbs_FACE); exp.More(); exp.Next()) {
                    if (exp.Current().ShapeType() != TopAbs_FACE) continue;
                    TopoDS_Face face = TopoDS::Face(exp.Current());
                    if (face.IsNull()) continue;
                    Handle(Geom_Surface) surface = BRep_Tool::Surface(face);
                    if (surface.IsNull() || surface->DynamicType() != STANDARD_TYPE(Geom_Plane)) continue;
                    Handle(Geom_Plane) plane = Handle(Geom_Plane)::DownCast(surface);

                    auto pos = plane->Position();
                    auto loc = plane->Position().Location();
                    auto direction = plane->Position().Direction();
                    auto yDirection = plane->Position().YDirection();
                    setPosition(loc.X(), loc.Y(), loc.Z());
                    setNormal(direction.X(), direction.Y(), direction.Z());
                    setUp(yDirection.X(), yDirection.Y(), yDirection.Z());
                    try
                    {
                        gp_Pln pln(gp_Pnt(m_pos[0], m_pos[1], m_pos[2]), gp_Dir(m_nor[0], m_nor[1], m_nor[2]));

                        BRepBuilderAPI_MakeFace face(pln);
                        _occShapeAgent->updateShape(face);
                        return true;
                    }
                    catch (...)
                    {
                        printLog(tr("Failed to make plane."), 3);
                        return false;
                    }
                }
            }
        }
        else
        {
            auto mag = std::sqrt(std::pow(m_SourcePlaneNormal[0], 2) + std::pow(m_SourcePlaneNormal[1], 2) + std::pow(m_SourcePlaneNormal[2], 2));
            if (qFuzzyIsNull(mag)) return false;

            for (int i = 0; i < 3; ++i) 
            {
                m_pos[i] = m_SourcePlaneOrigin[i] + m_SourcePlaneNormal[i] * m_Offset / mag;
                m_nor[i] = m_SourcePlaneNormal[i];
                m_up[i] = m_SourcePlaneUp[i];
            }

            try
            {
                gp_Pln pln(gp_Pnt(m_pos[0], m_pos[1], m_pos[2]), gp_Dir(m_nor[0], m_nor[1], m_nor[2]));

                BRepBuilderAPI_MakeFace face(pln);
                _occShapeAgent->updateShape(face);
                return true;
            }
            catch (...)
            {
                printLog(tr("Failed to make plane."), 3);
                return false;
            }
        }

        return false;
    }


    FITKOCCReferenceThreePointsPlane::FITKOCCReferenceThreePointsPlane() :OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCReferenceThreePointsPlane::update()
    {
        gp_Pnt p1(m_Points.at(0).at(0), m_Points.at(0).at(1), m_Points.at(0).at(2));
        gp_Pnt p2(m_Points.at(1).at(0), m_Points.at(1).at(1), m_Points.at(1).at(2));
        gp_Pnt p3(m_Points.at(2).at(0), m_Points.at(2).at(1), m_Points.at(2).at(2));
        gce_MakePln pln(p1, p2, p3);
        if (!pln.IsDone()) return false;
        auto plane = pln.Value();
        auto loc = plane.Position().Location();
        auto direction = plane.Position().Direction();
        auto yDirection = plane.Position().YDirection();
        setPosition(loc.X(), loc.Y(), loc.Z());
        setNormal(direction.X(), direction.Y(), direction.Z());
        setUp(yDirection.X(), yDirection.Y(), yDirection.Z());
        try
        {
            gp_Pln pln(gp_Pnt(m_pos[0], m_pos[1], m_pos[2]), gp_Dir(m_nor[0], m_nor[1], m_nor[2]));

            BRepBuilderAPI_MakeFace face(pln);
            _occShapeAgent->updateShape(face);
            return true;
        }
        catch (...)
        {
            printLog(tr("Failed to make plane."), 3);
            return false;
        }
    }

    FITKOCCReferenceEquationPlane::FITKOCCReferenceEquationPlane() :OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCReferenceEquationPlane::update()
    {
        gce_MakePln pln(m_A, m_B, m_C, m_D);
        if (!pln.IsDone()) return false;
        auto plane = pln.Value();
        auto loc = plane.Position().Location();
        auto direction = plane.Position().Direction();
        auto yDirection = plane.Position().YDirection();
        setPosition(loc.X(), loc.Y(), loc.Z());
        setNormal(direction.X(), direction.Y(), direction.Z());
        setUp(yDirection.X(), yDirection.Y(), yDirection.Z());

        try
        {
            gp_Pln pln(gp_Pnt(m_pos[0], m_pos[1], m_pos[2]), gp_Dir(m_nor[0], m_nor[1], m_nor[2]));

            BRepBuilderAPI_MakeFace face(pln);
            _occShapeAgent->updateShape(face);
            return true;
        }
        catch (...)
        {
            printLog(tr("Failed to make plane."), 3);
            return false;
        }
    }

    FITKOCCReferencePointAndDirectionPlane::FITKOCCReferencePointAndDirectionPlane() :OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCReferencePointAndDirectionPlane::update()
    {
        try
        {
            gce_MakePln pln(gp_Pnt(m_Point[0], m_Point[1], m_Point[2]), gp_Dir(m_Direction[0], m_Direction[1], m_Direction[2]));
            if (!pln.IsDone()) return false;

            auto plane = pln.Value();
            auto loc = plane.Position().Location();
            auto direction = plane.Position().Direction();
            auto yDirection = plane.Position().YDirection();
            setPosition(loc.X(), loc.Y(), loc.Z());
            setNormal(direction.X(), direction.Y(), direction.Z());
            setUp(yDirection.X(), yDirection.Y(), yDirection.Z());

            gp_Pln pln2(gp_Pnt(m_pos[0], m_pos[1], m_pos[2]), gp_Dir(m_nor[0], m_nor[1], m_nor[2]));

            BRepBuilderAPI_MakeFace face(pln2);
            _occShapeAgent->updateShape(face);
            return true;
        }
        catch (...)
        {
            printLog(tr("Failed to make plane."), 3);
            return false;
        }
    }

    // 旋转平面创建基准面。
    //@{
    FITKOCCReferenceRotateFromPlanePlane::FITKOCCReferenceRotateFromPlanePlane() :OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCReferenceRotateFromPlanePlane::update()
    {
        try
        {
            // 创建旋转变换。
            gp_Trsf tsf;
            gp_Pnt pnt(m_AxisStart[0], m_AxisStart[1], m_AxisStart[2]);
            gp_Dir dir(m_AxisEnd[0] - m_AxisStart[0], m_AxisEnd[1] - m_AxisStart[1], m_AxisEnd[2] - m_AxisStart[2]);
            gp_Ax1 axis(pnt, dir);
            tsf.SetRotation(axis, m_Angle / 180 * 3.1415926);

            gp_Pnt org(m_PlanePos[0], m_PlanePos[1], m_PlanePos[2]);
            gp_Dir nor(m_PlaneNor[0], m_PlaneNor[1], m_PlaneNor[2]);
            org.Transform(tsf);
            nor.Transform(tsf);

            gp_Pln pln(org, nor);

            for (int i = 0; i < 3; i++)
            {
                m_pos[i] = org.Coord(i + 1);
                m_nor[i] = nor.Coord(i + 1);
                m_up[i] = pln.YAxis().Direction().Coord(i + 1);
            }
            
            BRepBuilderAPI_MakeFace face(pln);
            _occShapeAgent->updateShape(face);
            return true;
        }
        catch (...)
        {
            printLog(tr("Failed to make plane."), 3);
            return false;
        }
    }
    //@}
}

