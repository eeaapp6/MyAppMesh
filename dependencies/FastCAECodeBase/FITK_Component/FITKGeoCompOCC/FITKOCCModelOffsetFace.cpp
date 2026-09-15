/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCModelOffsetFace.h"

#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"

#include "FITKOCCModelPart.h"
#include "FITKOCCVirtualTopoCreator.h"
#include "FITKOCCComandCommon.h"

#include <BOPAlgo_ArgumentAnalyzer.hxx>
#include <BRepAlgoAPI_Check.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <BRepBndLib.hxx>
#include <BRepExtrema_DistShapeShape.hxx>
#include <BRepOffset_MakeOffset.hxx>
#include <BRep_Tool.hxx>
#include <Bnd_Box.hxx>
#include <Extrema_ExtFlag.hxx>
#include <GeomAbs_SurfaceType.hxx>
#include <Precision.hxx>
#include <TopAbs.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Builder.hxx>
#include <TopoDS_Compound.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Vertex.hxx>
#include <gp_Dir.hxx>
#include <gp_Vec.hxx>

#include <cmath>

namespace
{
    constexpr double PrecisionDistanceTol = 1.e-7;

    bool GetFaceFromVirtualShape(const Interface::VirtualShape & vShape, TopoDS_Face & face)
    {
        if (vShape.isNull())
        {
            return false;
        }

        Interface::FITKAbsGeoCommand* cmd = FITKDATAREPO->getTDataByID<Interface::FITKAbsGeoCommand>(vShape.CmdId);
        if (!cmd)
        {
            return false;
        }

        OCC::FITKOCCTopoShape* occShape = cmd->getShapeTopoByIndexT<OCC::FITKOCCTopoShape>(
            Interface::FITKGeoEnum::VTopoShapeType::VSFace, vShape.VirtualTopoIndex);
        if (!occShape)
        {
            return false;
        }

        const TopoDS_Shape & topoShape = occShape->getTopoShape();
        if (topoShape.IsNull() || topoShape.ShapeType() != TopAbs_FACE)
        {
            return false;
        }

        face = TopoDS::Face(topoShape);
        return !face.IsNull();
    }

    bool GetFaceNormal(const TopoDS_Face & face, gp_Dir & normal)
    {
        if (face.IsNull())
        {
            return false;
        }

        try
        {
            BRepAdaptor_Surface adaptor(face);
            const Standard_Real uMid = (adaptor.FirstUParameter() + adaptor.LastUParameter()) / 2.;
            const Standard_Real vMid = (adaptor.FirstVParameter() + adaptor.LastVParameter()) / 2.;

            gp_Pnt pnt;
            gp_Vec uDir;
            gp_Vec vDir;
            adaptor.D1(uMid, vMid, pnt, uDir, vDir);

            gp_Vec normalVec = uDir.Crossed(vDir);
            if (normalVec.Magnitude() <= Precision::Confusion())
            {
                return false;
            }

            normalVec.Normalize();
            normal = gp_Dir(normalVec);
            if (face.Orientation() == TopAbs_REVERSED)
            {
                normal.Reverse();
            }

            return true;
        }
        catch (...)
        {
            return false;
        }
    }

    bool GetFaceCenter(const TopoDS_Face & face, gp_Pnt & center)
    {
        if (face.IsNull())
        {
            return false;
        }

        try
        {
            Bnd_Box box;
            BRepBndLib::Add(face, box, false);
            if (box.IsVoid())
            {
                return false;
            }

            Standard_Real xMin, yMin, zMin, xMax, yMax, zMax;
            box.Get(xMin, yMin, zMin, xMax, yMax, zMax);
            center = gp_Pnt((xMin + xMax) / 2., (yMin + yMax) / 2., (zMin + zMax) / 2.);
            return true;
        }
        catch (...)
        {
            return false;
        }
    }

    bool MakeOffsetFace(const TopoDS_Face & face, double offsetDistance, TopoDS_Shape & offsetShape)
    {
        if (face.IsNull() || std::abs(offsetDistance) <= PrecisionDistanceTol)
        {
            return false;
        }

        try
        {
            BRepOffset_MakeOffset offsetBuilder(face, offsetDistance, 1.e-6);
            if (!offsetBuilder.IsDone())
            {
                return false;
            }

            offsetShape = offsetBuilder.Shape();
            return !offsetShape.IsNull();
        }
        catch (...)
        {
            return false;
        }
    }

    bool GetExtremaDistance(const TopoDS_Face & sourceFace, const TopoDS_Face & targetFace,
        Extrema_ExtFlag flag, double & distance)
    {
        BRepExtrema_DistShapeShape distTool(sourceFace, targetFace, Precision::Confusion(), flag);
        if (!distTool.IsDone() || distTool.NbSolution() < 1)
        {
            distTool.Perform();
        }

        if (!distTool.IsDone() || distTool.NbSolution() < 1)
        {
            return false;
        }

        distance = distTool.Value();
        return std::isfinite(distance) && distance >= 0.;
    }

    bool IsValidShape(const TopoDS_Shape & shape)
    {
        if (shape.IsNull())
        {
            return false;
        }

        BRepAlgoAPI_Check check(shape);
        check.Perform();
        if (!check.IsValid())
        {
            return false;
        }

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
        checker.SetShape1(shape);
        checker.Perform();

        return !checker.HasFaulty();
    }
}

namespace OCC
{
    FITKOCCModelOffsetFace::FITKOCCModelOffsetFace() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelOffsetFace::update()
    {
        const QList<Interface::VirtualShape> & faces = sourceFaces();
        if (faces.isEmpty())
        {
            return false;
        }

        const bool relativeToTarget = offsetMode() == Interface::FITKAbsGeoOffsetFace::RelativeToTarget;
        TopoDS_Face targetTopoFace;
        gp_Pnt targetCenter;
        if (relativeToTarget)
        {
            if (!GetFaceFromVirtualShape(this->targetFace(), targetTopoFace))
            {
                return false;
            }

            if (!GetFaceCenter(targetTopoFace, targetCenter))
            {
                return false;
            }
        }

        QList<TopoDS_Shape> offsetShapes;
        for (const Interface::VirtualShape & vFace : faces)
        {
            TopoDS_Face sourceFace;
            if (!GetFaceFromVirtualShape(vFace, sourceFace))
            {
                return false;
            }

            gp_Dir sourceNormal;
            if (!GetFaceNormal(sourceFace, sourceNormal))
            {
                return false;
            }

            double offsetDistance = this->offsetDistance();
            if (relativeToTarget)
            {
                double baseDistance = 0.;
                if (relativeMode() == Interface::FITKAbsGeoOffsetFace::ROFarthestPointRatio)
                {
                    if (!GetExtremaDistance(sourceFace, targetTopoFace, Extrema_ExtFlag_MAX, baseDistance))
                    {
                        return false;
                    }
                }
                else
                {
                    if (!GetExtremaDistance(sourceFace, targetTopoFace, Extrema_ExtFlag_MIN, baseDistance))
                    {
                        return false;
                    }
                }

                if (baseDistance <= PrecisionDistanceTol)
                {
                    return false;
                }

                double scaleRatio = 0.5;
                if (relativeMode() != Interface::FITKAbsGeoOffsetFace::ROHalfDistance)
                {
                    scaleRatio = ratio();
                    if (scaleRatio < 0. || scaleRatio > 1.)
                    {
                        return false;
                    }
                }

                offsetDistance = baseDistance * scaleRatio;

                gp_Pnt sourceCenter;
                if (!GetFaceCenter(sourceFace, sourceCenter))
                {
                    return false;
                }

                gp_Vec sourceToTarget(sourceCenter, targetCenter);
                if (sourceToTarget.Dot(gp_Vec(sourceNormal.X(), sourceNormal.Y(), sourceNormal.Z())) < 0.)
                {
                    offsetDistance = -offsetDistance;
                }
            }

            if (std::abs(offsetDistance) <= PrecisionDistanceTol)
            {
                return false;
            }

            TopoDS_Shape offsetShape;
            if (!MakeOffsetFace(sourceFace, offsetDistance, offsetShape))
            {
                return false;
            }

            if (!IsValidShape(offsetShape))
            {
                return false;
            }

            offsetShapes.append(offsetShape);
        }

        TopoDS_Shape offsetShape;
        if (!FITKOCCComandCommon::MergeShapes(offsetShapes, offsetShape))
        {
            return false;
        }

        if (!IsValidShape(offsetShape))
        {
            return false;
        }

        FITKOCCModelPart* part = _occShapeAgent->getPart();
        if (part)
        {
            TopoDS_Shape outputShape;
            TopoDS_Shape* partShape = part->getPartShape();
            if (!partShape)
            {
                return false;
            }

            bool flag = FITKOCCComandCommon::MergeShapes(*partShape, offsetShape, outputShape, true);
            if (!flag)
            {
                return false;
            }

            part->updatePartShape(outputShape, false);
            _occShapeAgent->updateShape(part->getCopiedPartShape(), false);
        }
        else
        {
            _occShapeAgent->updateShape(offsetShape);
        }

        return true;
    }
}
