/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCModelPartitionFace.h"

// OCC base
#include <TopoDS.hxx>
#include <TopoDS_Solid.hxx>
#include <BRepFeat_SplitShape.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Compound.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <GeomProjLib.hxx>
#include <Geom_Curve.hxx>
#include <Geom_Surface.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <Geom_RectangularTrimmedSurface.hxx>
#include <BRep_Tool.hxx>
#include <GeomAPI_ProjectPointOnSurf.hxx>
#include <GeomLib_Tool.hxx>
#include <gp_Pln.hxx>
#include <IntTools_EdgeFace.hxx>
#include <IntTools_FaceFace.hxx>
#include <IntTools_SequenceOfCurves.hxx>
#include <IntTools_Curve.hxx>
#include <TopTools_HSequenceOfShape.hxx>
#include <ShapeAnalysis_FreeBounds.hxx>
#include <LocOpe_FindEdgesInFace.hxx>
#include <BOPAlgo_Splitter.hxx>
#include <BRepTools_ReShape.hxx>
#include <BRep_Builder.hxx>
#include <Geom_BSplineCurve.hxx>
#include <GeomAdaptor_HSurface.hxx>
#include <GeomAPI_Interpolate.hxx>
#include <BRepBuilderAPI_Sewing.hxx>
#include <BRepBuilderAPI_MakeSolid.hxx>
#include <BRepAlgoAPI_Section.hxx>
#include <BRepBuilderAPI_Copy.hxx>
#include <BRepOffsetAPI_NormalProjection.hxx>

// App
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

// Geometry
#include "FITKOCCVirtualTopoCreator.h"
#include "FITKOCCModelPart.h"
#include "FITKOCCComandCommon.h"
#include "FITKOCCExtendTool.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoEnum.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKVirtualTopoManager.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsVirtualTopoMapper.h"

#define EXT_FACTOR 0.001

namespace OCC
{
    // 草图分割面。
    //@{
    FITKOCCModelPartitionFaceWithSketch::FITKOCCModelPartitionFaceWithSketch() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelPartitionFaceWithSketch::update()
    {
        FITKOCCModelPart* part = _occShapeAgent->getPart();
        if (!part)
        {
            return false;
        }

        FITKAbstractOCCModel* partShapeAgent = part->getTShapeAgent<FITKAbstractOCCModel>();
        if (!partShapeAgent)
        {
            return false;
        }

        TopoDS_Shape* partShape = partShapeAgent->getShape();
        if (!partShape)
        {
            return false;
        }

        Interface::FITKGeoCommandList* geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (!geoCmdList)
        {
            return false;
        }

        // 获取草图对象。
        Interface::FITKAbsGeoSketch2D* sketchCmd = dynamic_cast<Interface::FITKAbsGeoSketch2D*>(geoCmdList->getDataByID(m_SketchId));
        if (!sketchCmd)
        {
            return false;
        }

        // 获取草图的全部边。
        sketchCmd->update();
        FITKAbstractOCCModel* occModel = sketchCmd->getTShapeAgent<FITKAbstractOCCModel>();
        if (!occModel)
        {
            return false;
        }

        const TopoDS_Compound & sketchShape = TopoDS::Compound(*occModel->getShape());
        if (sketchShape.IsNull())
        {
            return false;
        }

        //TopExp_Explorer iter;
        //QList<TopoDS_Edge> topoEdges;
        //for (iter.Init(*sketchShape, TopAbs_ShapeEnum::TopAbs_EDGE); iter.More(); iter.Next())
        //{
        //    const TopoDS_Edge & subShape = TopoDS::Edge(iter.Value());
        //    topoEdges.push_back(subShape);
        //}

        QList<Interface::FITKAbsVirtualTopo*> faces;
        QHash<Interface::FITKAbsVirtualTopo*, QList<Interface::FITKAbsVirtualTopo*>> solidFacesHash;
        QHash<const TopoDS_Face*, QList<Interface::FITKAbsVirtualTopo*>> faceSolidsHash;
        for (const Interface::VirtualShape & face : m_Faces)
        {
            Interface::FITKAbsGeoCommand* faceCmd = geoCmdList->getDataByID(face.CmdId);
            if (!faceCmd)
            {
                return false;
            }

            Interface::FITKAbsVirtualTopo* vFace = faceCmd->getVirtualTopoByIndex(
                Interface::FITKGeoEnum::VTopoShapeType::VSFace, face.VirtualTopoIndex);
            if (!vFace)
            {
                return false;
            }

            FITKOCCTopoShape* faceShape = vFace->getShapeT<FITKOCCTopoShape>();
            if (!faceShape)
            {
                return false;
            }

            const TopoDS_Face & topoFace = TopoDS::Face(faceShape->getTopoShape());
            if (topoFace.IsNull())
            {
                return false;
            }

            // 查询面是否属于实体。
            Interface::FITKVirtualTopoMapper vMapperFS;
            vMapperFS.mapTopo(vFace, Interface::FITKGeoEnum::VTopoShapeType::VSSolid);

            int nFaceSolids = vMapperFS.length();
            if (nFaceSolids == 0)
            {
                faceSolidsHash.insert(&topoFace, QList<Interface::FITKAbsVirtualTopo*>());
            }
            else
            {             
                for (int i = 0; i < vMapperFS.length(); i++)
                {
                    Interface::FITKAbsVirtualTopo* vSolid = vMapperFS.virtualTopo(i);
                    faceSolidsHash[&topoFace].push_back(vSolid);
                    solidFacesHash[vSolid].push_back(vFace);
                }
            }

            faces.push_back(vFace);
        }

        if (faceSolidsHash.isEmpty())
        {
            return false;
        }

        // 分割面。
        //FITKBRepFeat_SplitShape splitter;
        BRepTools_ReShape reShape;
        TopoDS_Shape newShape;

        try
        {
            TopoDS_Builder builder;
            TopoDS_Compound compoundFullShape;
            builder.MakeCompound(compoundFullShape);

            QHash<Interface::FITKAbsVirtualTopo*, BRepBuilderAPI_Sewing> solidSewerHash;

            // 初始化实体缝合器。
            double sewTol = 5.;
            QHashIterator<Interface::FITKAbsVirtualTopo*, QList<Interface::FITKAbsVirtualTopo*>> iter(solidFacesHash);
            while (iter.hasNext())
            {
                iter.next();

                BRepBuilderAPI_Sewing sewer;
                sewer.SetTolerance(sewTol);
                solidSewerHash.insert(iter.key(), sewer);
            }

            //splitter.Init(*partShape);

            //// 初始化分割数据。
            //for (const TopoDS_Face* face : topoFaces)
            //{
            //    splitter.Add(sketchShape, *face);
            //}

            //splitter.Build();
            //if (!splitter.IsDone())
            //{
            //    return false;
            //}

            //for (const TopoDS_Face* face : topoFacesInSolidHash.keys())
            QHashIterator<const TopoDS_Face*, QList<Interface::FITKAbsVirtualTopo*>> iterFace(faceSolidsHash);
            while (iterFace.hasNext())
            {
                iterFace.next();

                Handle(Geom_Surface) surface = BRep_Tool::Surface(*iterFace.key());
                if (surface.IsNull())
                {
                    return false;
                }

                // 投影草图到面上。
                TopoDS_Compound projSketch;
                builder.MakeCompound(projSketch);

                for (TopExp_Explorer iterEdge(sketchShape, TopAbs_ShapeEnum::TopAbs_EDGE); iterEdge.More(); iterEdge.Next())
                {
                    const TopoDS_Edge & e = TopoDS::Edge(iterEdge.Value());

                    BRepOffsetAPI_NormalProjection bProj(*iterFace.key());
                    bProj.Add(e);
                    bProj.Build();
                    if (!bProj.IsDone())
                    {
                        continue;
                    }

                    builder.Add(projSketch, bProj);
                }

                if (FITKOCCExtendTool::IsEmptyShape(projSketch))
                {
                    continue;
                }

                // 重组线框。
                TopoDS_Compound compoundWires;
                builder.MakeCompound(compoundWires);
                FITKOCCComandCommon::MakeWires(projSketch, compoundWires);

                BOPAlgo_Splitter splitter;
                splitter.AddArgument(*iterFace.key());
                splitter.AddTool(compoundWires);
                splitter.Perform();

                const TopoDS_Shape & newShape = splitter.Shape();
                if (newShape.IsNull())
                {
                    return false;
                }

                // 实体面添加至实体缝合器。
                QList<Interface::FITKAbsVirtualTopo*> vSolids = iterFace.value();
                
                if (!vSolids.isEmpty())
                {
                    for (Interface::FITKAbsVirtualTopo* vSolid : vSolids)
                    {
                        solidSewerHash[vSolid].Add(newShape);
                    }             
                }
                // 壳面直接进行替换。
                else
                {
                    reShape.Replace(*iterFace.key(), newShape);
                }
            }

            // 填充实体缝合器其他未发生变化的面。
            QHashIterator<Interface::FITKAbsVirtualTopo*, BRepBuilderAPI_Sewing> iterSewer(solidSewerHash);
            while (iterSewer.hasNext())
            {
                iterSewer.next();

                Interface::FITKAbsVirtualTopo* vSolid = iterSewer.key();

                // 获取原始实体形状。
                OCC::FITKOCCTopoShape* vOCCShapeSolid = vSolid->getShapeT<OCC::FITKOCCTopoShape>();
                if (!vOCCShapeSolid)
                {
                    return false;
                }

                const TopoDS_Shape & topoSolid = vOCCShapeSolid->getTopoShape();
                if (topoSolid.IsNull())
                {
                    return false;
                }

                // 组合实体其他原始面。
                Interface::FITKVirtualTopoMapper vMapperSF;
                vMapperSF.mapTopo(vSolid, Interface::FITKGeoEnum::VSFace);

                int nSolidFaces = vMapperSF.length();
                for (int i = 0; i < nSolidFaces; i++)
                {
                    Interface::FITKAbsVirtualTopo* vFace = vMapperSF.virtualTopo(i);

                    // 跳过已变化的面。
                    if (!faces.contains(vFace))
                    {
                        OCC::FITKOCCTopoShape* vOCCShapeFace = vFace->getShapeT<OCC::FITKOCCTopoShape>();
                        if (!vOCCShapeFace)
                        {
                            return false;
                        }

                        const TopoDS_Face & topoFace = TopoDS::Face(vOCCShapeFace->getTopoShape());
                        if (topoFace.IsNull())
                        {
                            return false;
                        }

                        solidSewerHash[vSolid].Add(topoFace);
                    }
                }

                // 执行缝合，替换实体。
                solidSewerHash[vSolid].Perform();

                const TopoDS_Shape & newSolidSewed = solidSewerHash[vSolid].SewedShape();

                // 重新创建实体。
                QList<QList<TopoDS_Shell>> shellsGroups;
                for (TopExp_Explorer iter(newSolidSewed, TopAbs_ShapeEnum::TopAbs_SHELL); iter.More(); iter.Next())
                {
                    const TopoDS_Shell & ss = TopoDS::Shell(iter.Value());
                    reShape.Remove(topoSolid);

                    //if (!ss.Closed())
                    //{
                    //    continue;
                    //}

                    TopoDS_Shell newShell = TopoDS::Shell(BRepBuilderAPI_Copy(ss));

                    Bnd_Box box;
                    BRepBndLib::Add(ss, box, false);

                    // 分组判断包围盒。
                    bool breakFlag = false;
                    bool inGroup = false;
                    for (QList<TopoDS_Shell> & shells : shellsGroups)
                    {
                        int index = -1;
                        int nShells = shells.count();
                        for (const TopoDS_Shell & shell : shells)
                        {
                            index++;

                            Bnd_Box boxTemp;
                            BRepBndLib::Add(shell, boxTemp, false);

                            // 比较盒子。
                            bool notThisGroup = false;
                            BoxCompare boxRet = FITKOCCComandCommon::CompareBox(box, boxTemp);
                            switch (boxRet)
                            {
                            case OCC::BC_1_in_2:
                            {
                                if (FITKOCCExtendTool::IsEmptyShape(BRepAlgoAPI_Section(ss, shell)))
                                {
                                    if (nShells - 1 == index)
                                    {
                                        inGroup = true;
                                        breakFlag = true;
                                        shells.push_back(newShell);
                                        break;
                                    }
                                    else
                                    {
                                        continue;
                                        break;
                                    }
                                }
                                else
                                {
                                    notThisGroup = true;
                                    break;
                                }
                            }
                            case OCC::BC_2_in_1:
                            {
                                if (FITKOCCExtendTool::IsEmptyShape(BRepAlgoAPI_Section(ss, shell)))
                                {
                                    inGroup = true;
                                    breakFlag = true;
                                    shells.insert(index, newShell);
                                    break;
                                }
                                else
                                {
                                    if (index > 0)
                                    {
                                        inGroup = true;
                                        breakFlag = true;
                                        shells.insert(index - 1, newShell);
                                    }

                                    break;
                                }
                            }
                            default:
                            {
                                notThisGroup = true;
                                break;
                            }
                            }

                            // 跳出当前组。
                            if (notThisGroup || breakFlag)
                            {
                                break;
                            }
                        }

                        if (breakFlag)
                        {
                            break;
                        }
                    }

                    if (!inGroup)
                    {
                        shellsGroups.push_back(QList<TopoDS_Shell>{ newShell });
                    }
                }

                for (QList<TopoDS_Shell> & shells : shellsGroups)
                {
                    QList<TopoDS_Shell> newList = shells;
                    BRepBuilderAPI_MakeSolid ms(newList.takeFirst());

                    for (const TopoDS_Shell & shell : newList)
                    {
                        ms.Add(shell);
                    }

                    ms.Build();

                    bool hasSolid = false;
                    if (ms.IsDone())
                    {
                        const TopoDS_Solid & solid = ms;
                        if (!solid.IsNull())
                        {
                            builder.Add(compoundFullShape, solid);
                            hasSolid = true;
                        }
                    }

                    if (!hasSolid)
                    {
                        for (const TopoDS_Shell & s : newList)
                        {
                            builder.Add(compoundFullShape, s);
                        }
                    }
                }
            }

            // 形状为空则说明所有体上的面均发生变化。
            const TopoDS_Shape & resultShape = reShape.Apply(*partShape);
            if (!resultShape.IsNull())
            {
                builder.Add(compoundFullShape, resultShape);
            }

            newShape = compoundFullShape;
        }
        catch (...)
        {
            return false;
        }

        //const TopoDS_Shape & newShape = splitter.Shape();
        //if (newShape.IsNull())
        //{
        //    return false;
        //}

        part->updatePartShape(newShape, false);
        _occShapeAgent->updateShape(part->getCopiedPartShape(), false);

        return true;
    }
    //@}

    // 两点分割面。
    //@{
    FITKOCCModelPartitionFaceWithTwoPoints::FITKOCCModelPartitionFaceWithTwoPoints() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelPartitionFaceWithTwoPoints::update()
    {
        FITKOCCModelPart* part = _occShapeAgent->getPart();
        if (!part)
        {
            return false;
        }

        FITKAbstractOCCModel* partShapeAgent = part->getTShapeAgent<FITKAbstractOCCModel>();
        if (!partShapeAgent)
        {
            return false;
        }

        TopoDS_Shape* partShape = partShapeAgent->getShape();
        if (!partShape)
        {
            return false;
        }

        Interface::FITKGeoCommandList* geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (!geoCmdList)
        {
            return false;
        }

        // 创建直边。
        gp_Pnt pt1(m_StartPoint[0], m_StartPoint[1], m_StartPoint[2]);
        gp_Pnt pt2(m_EndPoint[0], m_EndPoint[1], m_EndPoint[2]);
        //gp_Dir lDir(m_EndPoint[0] - m_StartPoint[0], 
        //    m_EndPoint[1] - m_StartPoint[1], m_EndPoint[2] - m_StartPoint[2]);
        //gp_Lin lin(pt1, lDir);
        const TopoDS_Edge & edgeStraight = BRepBuilderAPI_MakeEdge(pt1, pt2);

        Standard_Real first, last;
        Handle(Geom_Curve) curve = BRep_Tool::Curve(edgeStraight, first, last);
        if (curve.IsNull())
        {
            return false;
        }

        Handle(Geom_TrimmedCurve) tCurve = new Geom_TrimmedCurve(curve, first, last);
        if (tCurve.IsNull()) 
        {
            return false;
        }

        QList<const TopoDS_Face*> topoFaces;
        for (const Interface::VirtualShape & face : m_Faces)
        {
            Interface::FITKAbsGeoCommand* faceCmd = geoCmdList->getDataByID(face.CmdId);
            if (!faceCmd)
            {
                return false;
            }

            FITKOCCTopoShape* faceShape = faceCmd->getShapeTopoByIndexT<FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSFace, face.VirtualTopoIndex);
            if (!faceShape)
            {
                return false;
            }

            const TopoDS_Face & topoFace = TopoDS::Face(faceShape->getTopoShape());
            topoFaces.push_back(&topoFace);
        }

        if (topoFaces.isEmpty())
        {
            return false;
        }

        // 分割面。
        BRepFeat_SplitShape splitter;
        double factor = EXT_FACTOR;

        try
        {
            splitter.Init(*partShape);

            // 初始化分割数据。
            for (const TopoDS_Face* face : topoFaces)
            {
                Handle(Geom_Surface) surface = BRep_Tool::Surface(*face);
                if (surface.IsNull())
                {
                    return false;
                }

                // 压印。
                Handle(Geom_Curve) projCurve = GeomProjLib::Project(tCurve, surface);
                if (projCurve.IsNull())
                {
                    return false;
                }

                //const TopoDS_Edge & projEdge = BRepBuilderAPI_MakeEdge(projCurve);

                //// 线面相交。
                //IntTools_EdgeFace intEdFa;
                //intEdFa.SetEdge(projEdge);
                //intEdFa.SetFace(*face);
                //intEdFa.Perform();

                //const IntTools_SequenceOfCommonPrts & cps = intEdFa.CommonParts();
                //int nCps = cps.Size();
                //if (nCps == 0)
                //{
                //    return false;
                //}

                //IntTools_CommonPrt cp = cps.First();
                //const TopoDS_Edge & comEdge = cp.Edge1();
                //if (comEdge.IsNull())
                //{
                //    return false;
                //}

                //gp_Pnt bdPt1, bdPt2;
                //cp.BoundingPoints(bdPt1, bdPt2);

                gp_Pnt pntFirst = curve->Value(first);
                gp_Pnt pntLast = curve->Value(last);

                // 获取投影端点参数值。
                GeomAPI_ProjectPointOnSurf projFirst(pntFirst, surface);
                GeomAPI_ProjectPointOnSurf projLast(pntLast, surface);

                pntFirst = projFirst.Point(1);
                pntLast = projLast.Point(1);

                Standard_Real newFirst, newLast;
                if (!GeomLib_Tool::Parameter(projCurve, pntFirst, Precision::Confusion(), newFirst) ||
                    !GeomLib_Tool::Parameter(projCurve, pntLast, Precision::Confusion(), newLast)) 
                {
                    return false;
                }

                double delta = last - first;
                const TopoDS_Edge & newEdge = BRepBuilderAPI_MakeEdge(projCurve,
                    newFirst - delta * factor, newLast + delta * factor);               
                splitter.Add(newEdge, *face);
            }

            splitter.Build();
            if (!splitter.IsDone())
            {
                return false;
            }
        }
        catch (...)
        {
            return false;
        }

        const TopoDS_Shape & newShape = splitter.Shape();
        if (newShape.IsNull())
        {
            return false;
        }

        part->updatePartShape(newShape, false);
        _occShapeAgent->updateShape(part->getCopiedPartShape(), false);

        return true;
    }
    //@}

    // 基准面分割面。
    //@{
    FITKOCCModelPartitionFaceWithDatumPlane::FITKOCCModelPartitionFaceWithDatumPlane() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelPartitionFaceWithDatumPlane::update()
    {
        FITKOCCModelPart* part = _occShapeAgent->getPart();
        if (!part || !m_DatumPlane)
        {
            return false;
        }

        FITKAbstractOCCModel* partShapeAgent = part->getTShapeAgent<FITKAbstractOCCModel>();
        if (!partShapeAgent)
        {
            return false;
        }

        TopoDS_Shape* partShape = partShapeAgent->getShape();
        if (!partShape)
        {
            return false;
        }

        Interface::FITKGeoCommandList* geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (!geoCmdList)
        {
            return false;
        }

        // 创建平面。
        double pos3[3], nor3[3];
        m_DatumPlane->getPosition(pos3);
        m_DatumPlane->getNormal(nor3);

        gp_Pln pln(gp_Pnt(pos3[0], pos3[1], pos3[2]), gp_Dir(nor3[0], nor3[1], nor3[2]));
        const TopoDS_Face & facePlane = BRepBuilderAPI_MakeFace(pln);

        QList<const TopoDS_Face*> topoFaces;
        for (const Interface::VirtualShape & face : m_Faces)
        {
            Interface::FITKAbsGeoCommand* faceCmd = geoCmdList->getDataByID(face.CmdId);
            if (!faceCmd)
            {
                return false;
            }

            FITKOCCTopoShape* faceShape = faceCmd->getShapeTopoByIndexT<FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSFace, face.VirtualTopoIndex);
            if (!faceShape)
            {
                return false;
            }

            const TopoDS_Face & topoFace = TopoDS::Face(faceShape->getTopoShape());
            topoFaces.push_back(&topoFace);
        }

        if (topoFaces.isEmpty())
        {
            return false;
        }

        // 分割面。
        BRepFeat_SplitShape splitter;
        double factor = EXT_FACTOR;

        try
        {
            splitter.Init(*partShape);

            // 初始化分割数据。
            for (const TopoDS_Face* face : topoFaces)
            {
                Handle(Geom_Surface) surface = BRep_Tool::Surface(*face);
                if (surface.IsNull())
                {
                    return false;
                }

                // 面面求交。
                IntTools_FaceFace intFaFa;
                intFaFa.Perform(*face, facePlane);

                const IntTools_SequenceOfCurves & curves = intFaFa.Lines();
                int nLines = curves.Size();
                if (nLines == 0)
                {
                    return false;
                }

                IntTools_Curve intCurve = curves.First();
                const Handle(Geom_Curve) & curve = intCurve.Curve();
                if (curve.IsNull())
                {
                    return false;
                }

                if (!intCurve.HasBounds())
                {
                    return false;
                }

                Standard_Real first, last;
                gp_Pnt firstPt, lastPt;
                intCurve.Bounds(first, last, firstPt, lastPt);

                // 压印。
                Handle(Geom_Curve) projCurve = GeomProjLib::Project(curve, surface);
                if (projCurve.IsNull())
                {
                    return false;
                }

                gp_Pnt pntFirst = curve->Value(first);
                gp_Pnt pntLast = curve->Value(last);

                // 获取投影端点参数值。
                GeomAPI_ProjectPointOnSurf projFirst(pntFirst, surface);
                GeomAPI_ProjectPointOnSurf projLast(pntLast, surface);

                pntFirst = projFirst.Point(1);
                pntLast = projLast.Point(1);

                Standard_Real newFirst, newLast;
                if (!GeomLib_Tool::Parameter(projCurve, pntFirst, Precision::Confusion(), newFirst) ||
                    !GeomLib_Tool::Parameter(projCurve, pntLast, Precision::Confusion(), newLast))
                {
                    return false;
                }

                double delta = last - first;
                const TopoDS_Edge & newEdge = BRepBuilderAPI_MakeEdge(projCurve,
                    newFirst - delta * factor, newLast + delta * factor);
                splitter.Add(newEdge, *face);
            }

            splitter.Build();
            if (!splitter.IsDone())
            {
                return false;
            }
        }
        catch (...)
        {
            return false;
        }

        const TopoDS_Shape & newShape = splitter.Shape();
        if (newShape.IsNull())
        {
            return false;
        }

        part->updatePartShape(newShape, false);
        _occShapeAgent->updateShape(part->getCopiedPartShape(), false);

        return true;
    }
    //@}

    // 延长线路径分割面。
    //@{
    FITKOCCModelPartitionFaceWithCurvedPath::FITKOCCModelPartitionFaceWithCurvedPath() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelPartitionFaceWithCurvedPath::update()
    {
        FITKOCCModelPart* part = _occShapeAgent->getPart();
        if (!part)
        {
            return false;
        }

        // 该模式仅可分割一个面。
        if (m_Faces.count() != 1 || m_FirstPoint.count() != 3 || m_SecondPoint.count() != 3)
        {
            return false;
        }

        FITKAbstractOCCModel* partShapeAgent = part->getTShapeAgent<FITKAbstractOCCModel>();
        if (!partShapeAgent)
        {
            return false;
        }

        TopoDS_Shape* partShape = partShapeAgent->getShape();
        if (!partShape)
        {
            return false;
        }

        Interface::FITKGeoCommandList* geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (!geoCmdList)
        {
            return false;
        }

        // 获取两条边。
        Interface::FITKAbsGeoCommand* cmdFirstEdge = geoCmdList->getDataByID(m_FirstEdge.CmdId);
        Interface::FITKAbsGeoCommand* cmdSecondEdge = geoCmdList->getDataByID(m_SecondEdge.CmdId);
        if (!cmdFirstEdge || !cmdSecondEdge)
        {
            return false;
        }

        FITKAbstractOCCModel* occModelFirstEdge = cmdFirstEdge->getTShapeAgent<FITKAbstractOCCModel>();
        FITKAbstractOCCModel* occModelSecondEdge = cmdSecondEdge->getTShapeAgent<FITKAbstractOCCModel>();
        if (!occModelFirstEdge || !occModelSecondEdge)
        {
            return false;
        }

        FITKOCCTopoShape* occFirstEdge = cmdFirstEdge->getShapeTopoByIndexT<FITKOCCTopoShape>
            (Interface::FITKGeoEnum::VTopoShapeType::VSEdge, m_FirstEdge.VirtualTopoIndex);
        FITKOCCTopoShape* occSecondEdge = cmdSecondEdge->getShapeTopoByIndexT<FITKOCCTopoShape>
            (Interface::FITKGeoEnum::VTopoShapeType::VSEdge, m_SecondEdge.VirtualTopoIndex);
        if (!occFirstEdge || !occSecondEdge)
        {
            return false;
        }

        const TopoDS_Edge & edgeFirstShape = TopoDS::Edge(occFirstEdge->getTopoShape());
        const TopoDS_Edge & edgeSecondShape = TopoDS::Edge(occSecondEdge->getTopoShape());
        if (edgeFirstShape.IsNull() || edgeSecondShape.IsNull())
        {
            return false;
        }

        // 获取需要分割的面。
        Interface::VirtualShape face = m_Faces.first();
        Interface::FITKAbsGeoCommand* faceCmd = geoCmdList->getDataByID(face.CmdId);
        if (!faceCmd)
        {
            return false;
        }

        FITKOCCTopoShape* faceShape = faceCmd->getShapeTopoByIndexT<FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSFace, face.VirtualTopoIndex);
        if (!faceShape)
        {
            return false;
        }

        // 获取表面。
        const TopoDS_Face & topoFace = TopoDS::Face(faceShape->getTopoShape());
        Handle(Geom_Surface) surfaceInf = BRep_Tool::Surface(topoFace);
        if (surfaceInf.IsNull())
        {
            return false;
        }

        Standard_Real uFirst, uLast, vFirst, vLast;
        BRepAdaptor_Surface brepFace(topoFace);
        uFirst = brepFace.FirstUParameter();
        uLast = brepFace.LastUParameter();
        vFirst = brepFace.FirstVParameter();
        vLast = brepFace.LastVParameter();

        // 创建有限边界曲面。
        Handle(Geom_RectangularTrimmedSurface) surface =
            new Geom_RectangularTrimmedSurface(surfaceInf, uFirst, uLast, vFirst, vLast);

        // 获取两条曲线。
        Standard_Real first1, last1, first2, last2;
        Handle(Geom_Curve) curve1 = BRep_Tool::Curve(edgeFirstShape, first1, last1);
        Handle(Geom_Curve) curve2 = BRep_Tool::Curve(edgeSecondShape, first2, last2);
        if (curve1.IsNull() || curve2.IsNull())
        {
            return false;
        }

        gp_Pnt pnt1(m_FirstPoint[0], m_FirstPoint[1], m_FirstPoint[2]);
        gp_Pnt pnt2(m_SecondPoint[0], m_SecondPoint[1], m_SecondPoint[2]);

        // 获取两个位置在曲线上的参数值。
        Standard_Real t1, t2;
        GeomLib_Tool::Parameter(curve1, pnt1, Precision::Confusion(), t1);
        GeomLib_Tool::Parameter(curve2, pnt2, Precision::Confusion(), t2);

        // 获取两个位置在表面上的参数值。
        Standard_Real u1, v1, u2, v2;
        GeomLib_Tool::Parameters(surface, pnt1, Precision::Confusion(), u1, v1);
        GeomLib_Tool::Parameters(surface, pnt2, Precision::Confusion(), u2, v2);

        // 分割面。
        BRepFeat_SplitShape splitter;
        //BRepTools_ReShape reShape;

        BRep_Builder builder;
        TopoDS_Compound compound;
        builder.MakeCompound(compound);

        try
        {
            splitter.Init(*partShape);

            // 获取两个位置在面上的法向。
            gp_Vec du1, dv1, du2, dv2;
            surface->D1(u1, v1, pnt1, du1, dv1);
            surface->D1(u2, v2, pnt2, du2, dv2);

            // 获取曲线切向。
            gp_Vec vec1, vec2;
            curve1->D1(t1, pnt1, vec1);
            curve2->D1(t2, pnt2, vec2);
            vec1 *= edgeFirstShape.Orientation() == TopAbs_Orientation::TopAbs_REVERSED ? -1. : 1.;
            vec2 *= edgeSecondShape.Orientation() == TopAbs_Orientation::TopAbs_REVERSED ? -1. : 1.;
            vec1.Normalize();
            vec2.Normalize();

            // 叉积取法向。
            gp_Vec nor1 = du1.Crossed(dv1);
            gp_Vec nor2 = du2.Crossed(dv2);
            if (nor1.SquareMagnitude() < gp::Resolution() || 
                nor2.SquareMagnitude() < gp::Resolution())
            {
                return false;
            }

            nor1.Normalize();
            nor2.Normalize();

            // 计算桥接方向。
            Standard_Real dot1 = vec1.Dot(nor1);
            gp_Vec cross1 = vec1.Crossed(nor1);
            gp_Vec dir1 = dot1 * vec1 + cross1;

            Standard_Real dot2 = vec2.Dot(nor2);
            gp_Vec cross2 = vec2.Crossed(nor2);
            gp_Vec dir2 = dot2 * vec2 + cross2;

            // 计算偏移距离，按方向获取第二点坐标。
            double dist = pnt1.Distance(pnt2);
            double fac = dist * 1.;

            // 调整方向。
            gp_Pnt testPnt1 = pnt1.XYZ() - dir1.XYZ() * dist * 0.01;
            gp_Pnt testPnt2 = pnt2.XYZ() - dir2.XYZ() * dist * 0.01;

            Standard_Real tempU, tempV;
            bool isOnSurf = GeomLib_Tool::Parameters(surface, testPnt1, Precision::Confusion(), tempU, tempV);
            if (isOnSurf)
            {
                dir1.Reverse();
            }

            isOnSurf = GeomLib_Tool::Parameters(surface, testPnt2, Precision::Confusion(), tempU, tempV);
            if (isOnSurf)
            {
                dir2.Reverse();
            }

            gp_Pnt pntStart = pnt1.XYZ() - dir1.XYZ() * fac;
            gp_Pnt pntEnd = pnt2.XYZ() - dir2.XYZ() * fac;

            // 创建桥接样条曲线。
            const Handle(TColgp_HArray1OfPnt) pnts = new TColgp_HArray1OfPnt(1, 4);
            pnts->SetValue(1, pntStart);
            pnts->SetValue(2, pnt1);
            pnts->SetValue(3, pnt2);
            pnts->SetValue(4, pntEnd);

            GeomAPI_Interpolate inter(pnts, Standard_False, Precision::Confusion());
            inter.Perform();

            Handle(Geom_BSplineCurve) bsplCurve = inter.Curve();
            if (bsplCurve.IsNull())
            {
                return false;
            }

            Standard_Real p1, p4;
            GeomLib_Tool::Parameter(bsplCurve, pntStart, Precision::Confusion(), p1);
            GeomLib_Tool::Parameter(bsplCurve, pntEnd, Precision::Confusion(), p4);
            const TopoDS_Edge & bridgeEdge = BRepBuilderAPI_MakeEdge(bsplCurve, p1, p4);
            if (bridgeEdge.IsNull())
            {
                return false;
            }

            // 获取桥接曲线进行压印。
            BRepOffsetAPI_NormalProjection bProj(topoFace);
            bProj.Add(bridgeEdge);
            bProj.Build();
            if (!bProj.IsDone())
            {
                return false;
            }

            const TopoDS_Shape & shapeProj = bProj.Projection();
            if (shapeProj.IsNull())
            {
                return false;
            }

            //builder.Add(compound, shapeProj);
            //builder.Add(compound, BRepBuilderAPI_MakeEdge(pntStart, pnt1));
            //builder.Add(compound, BRepBuilderAPI_MakeEdge(pnt1, pnt2));
            //builder.Add(compound, BRepBuilderAPI_MakeEdge(pnt2, pntEnd));

            //BOPAlgo_Splitter splitter;
            //splitter.AddArgument(topoFace);
            //splitter.AddTool(shapeProj);
            //splitter.Perform();

            //const TopoDS_Shape & newShape = splitter.Shape();
            //if (newShape.IsNull())
            //{
            //    return false;
            //}

            //reShape.Replace(topoFace, newShape);

            TopAbs_ShapeEnum st = shapeProj.ShapeType();
            if (st == TopAbs_ShapeEnum::TopAbs_EDGE)
            {
                const TopoDS_Edge & newEdge = TopoDS::Edge(shapeProj);
                splitter.Add(newEdge, topoFace);
            }
            else if (st == TopAbs_ShapeEnum::TopAbs_WIRE)
            {
                const TopoDS_Wire & newWire = TopoDS::Wire(shapeProj);
                splitter.Add(newWire, topoFace);
            }
            else
            {
                for (TopExp_Explorer iter(shapeProj, TopAbs_ShapeEnum::TopAbs_EDGE); iter.More(); iter.Next())
                {
                    const TopoDS_Edge & e = TopoDS::Edge(iter.Value());
                    splitter.Add(e, topoFace);
                }
            }

            splitter.Build();
            if (!splitter.IsDone())
            {
                return false;
            }
        }
        catch (...)
        {
            return false;
        }

        //const TopoDS_Shape & newShape = reShape.Apply(*partShape);
        //if (newShape.IsNull())
        //{
        //    return false;
        //}

        //builder.Add(compound, newShape);
        //part->updatePartShape(compound, false);

        //const TopoDS_Shape & newShape = reShape.Apply(*partShape);
        //if (newShape.IsNull())
        //{
        //    return false;
        //}

        const TopoDS_Shape & newShape = splitter.Shape();
        if (newShape.IsNull())
        {
            return false;
        }

        part->updatePartShape(newShape, false);
        _occShapeAgent->updateShape(part->getCopiedPartShape(), false);

        return true;
    }
    //@}

    // 延伸面分割面。
    //@{
    FITKOCCModelPartitionFaceWithExtendFace::FITKOCCModelPartitionFaceWithExtendFace() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelPartitionFaceWithExtendFace::update()
    {
        FITKOCCModelPart* part = _occShapeAgent->getPart();
        if (!part)
        {
            return false;
        }

        FITKAbstractOCCModel* partShapeAgent = part->getTShapeAgent<FITKAbstractOCCModel>();
        if (!partShapeAgent)
        {
            return false;
        }

        TopoDS_Shape* partShape = partShapeAgent->getShape();
        if (!partShape)
        {
            return false;
        }

        Interface::FITKGeoCommandList* geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (!geoCmdList)
        {
            return false;
        }

        Interface::FITKAbsGeoCommand* cmdFace = geoCmdList->getDataByID(m_ExtendFace.CmdId);
        if (!cmdFace)
        {
            return false;
        }

        FITKAbstractOCCModel* occModelFace = cmdFace->getTShapeAgent<FITKAbstractOCCModel>();
        if (!occModelFace)
        {
            return false;
        }

        FITKOCCTopoShape* occFace = cmdFace->getShapeTopoByIndexT<FITKOCCTopoShape>
            (Interface::FITKGeoEnum::VTopoShapeType::VSFace, m_ExtendFace.VirtualTopoIndex);
        if (!occFace)
        {
            return false;
        }

        // 获取延伸面曲面。
        const TopoDS_Face & topoFace = TopoDS::Face(occFace->getTopoShape());
        if (topoFace.IsNull())
        {
            return false;
        }

        Handle(Geom_Surface) surface = BRep_Tool::Surface(topoFace);
        if (!surface)
        {
            return false;
        }

        QList<const TopoDS_Face*> topoFaces;
        for (const Interface::VirtualShape & face : m_Faces)
        {
            Interface::FITKAbsGeoCommand* faceCmd = geoCmdList->getDataByID(face.CmdId);
            if (!faceCmd)
            {
                return false;
            }

            FITKOCCTopoShape* faceShape = faceCmd->getShapeTopoByIndexT<FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSFace, face.VirtualTopoIndex);
            if (!faceShape)
            {
                return false;
            }

            const TopoDS_Face & topoFace = TopoDS::Face(faceShape->getTopoShape());
            topoFaces.push_back(&topoFace);
        }

        if (topoFaces.isEmpty())
        {
            return false;
        }

        // 分割面。
        BRepFeat_SplitShape splitter;
        double factor = EXT_FACTOR;

        try
        {
            // 创建延伸曲面。
            const TopoDS_Face & faceExt = BRepBuilderAPI_MakeFace(surface, 1e-6);
            if (faceExt.IsNull())
            {
                return false;
            }

            splitter.Init(*partShape);

            // 初始化分割数据。
            for (const TopoDS_Face* face : topoFaces)
            {
                Handle(Geom_Surface) surface = BRep_Tool::Surface(*face);
                if (surface.IsNull())
                {
                    return false;
                }

                // 面面求交。
                IntTools_FaceFace intFaFa;
                intFaFa.Perform(*face, faceExt);

                const IntTools_SequenceOfCurves & curves = intFaFa.Lines();
                int nLines = curves.Size();
                if (nLines == 0)
                {
                    return false;
                }

                IntTools_Curve intCurve = curves.First();
                const Handle(Geom_Curve) & curve = intCurve.Curve();
                if (curve.IsNull())
                {
                    return false;
                }

                if (!intCurve.HasBounds())
                {
                    return false;
                }

                Standard_Real first, last;
                gp_Pnt firstPt, lastPt;
                intCurve.Bounds(first, last, firstPt, lastPt);

                // 压印。
                Handle(Geom_Curve) projCurve = GeomProjLib::Project(curve, surface);
                if (projCurve.IsNull())
                {
                    return false;
                }

                gp_Pnt pntFirst = curve->Value(first);
                gp_Pnt pntLast = curve->Value(last);

                // 获取投影端点参数值。
                GeomAPI_ProjectPointOnSurf projFirst(pntFirst, surface);
                GeomAPI_ProjectPointOnSurf projLast(pntLast, surface);

                pntFirst = projFirst.Point(1);
                pntLast = projLast.Point(1);

                Standard_Real newFirst, newLast;
                if (!GeomLib_Tool::Parameter(projCurve, pntFirst, Precision::Confusion(), newFirst) ||
                    !GeomLib_Tool::Parameter(projCurve, pntLast, Precision::Confusion(), newLast))
                {
                    return false;
                }

                double delta = last - first;
                const TopoDS_Edge & newEdge = BRepBuilderAPI_MakeEdge(projCurve,
                    newFirst - delta * factor, newLast + delta * factor);
                splitter.Add(newEdge, *face);
            }

            splitter.Build();
            if (!splitter.IsDone())
            {
                return false;
            }
        }
        catch (...)
        {
            return false;
        }

        const TopoDS_Shape & newShape = splitter.Shape();
        if (newShape.IsNull())
        {
            return false;
        }

        part->updatePartShape(newShape, false);
        _occShapeAgent->updateShape(part->getCopiedPartShape(), false);

        return true;
    }
    //@}

    // 相交面分割面。
    //@{
    FITKOCCModelPartitionFaceWithIntersectFace::FITKOCCModelPartitionFaceWithIntersectFace() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelPartitionFaceWithIntersectFace::update()
    {
        FITKOCCModelPart* part = _occShapeAgent->getPart();
        if (!part)
        {
            return false;
        }

        FITKAbstractOCCModel* partShapeAgent = part->getTShapeAgent<FITKAbstractOCCModel>();
        if (!partShapeAgent)
        {
            return false;
        }

        TopoDS_Shape* partShape = partShapeAgent->getShape();
        if (!partShape)
        {
            return false;
        }

        Interface::FITKGeoCommandList* geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (!geoCmdList)
        {
            return false;
        }

        QList<const TopoDS_Face*> topoFacesTool;
        for (const Interface::VirtualShape & face : m_ToolFaces)
        {
            Interface::FITKAbsGeoCommand* faceCmd = geoCmdList->getDataByID(face.CmdId);
            if (!faceCmd)
            {
                continue;
            }

            FITKOCCTopoShape* faceShape = faceCmd->getShapeTopoByIndexT<FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSFace, face.VirtualTopoIndex);
            if (!faceShape)
            {
                continue;
            }

            const TopoDS_Face & topoFace = TopoDS::Face(faceShape->getTopoShape());
            topoFacesTool.push_back(&topoFace);
        }

        QList<const TopoDS_Face*> topoFaces;
        for (const Interface::VirtualShape & face : m_Faces)
        {
            Interface::FITKAbsGeoCommand* faceCmd = geoCmdList->getDataByID(face.CmdId);
            if (!faceCmd)
            {
                return false;
            }

            FITKOCCTopoShape* faceShape = faceCmd->getShapeTopoByIndexT<FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSFace, face.VirtualTopoIndex);
            if (!faceShape)
            {
                return false;
            }

            const TopoDS_Face & topoFace = TopoDS::Face(faceShape->getTopoShape());
            topoFaces.push_back(&topoFace);
        }

        if (topoFaces.isEmpty() || topoFacesTool.isEmpty())
        {
            return false;
        }

        // 分割面。
        BRepFeat_SplitShape splitter;
        double factor = EXT_FACTOR;

        try
        {
            splitter.Init(*partShape);

            // 初始化分割数据。
            int nValidInt = 0;
            for (const TopoDS_Face* face : topoFaces)
            {
                // 获取被分割面曲面。
                Handle(Geom_Surface) surface = BRep_Tool::Surface(*face);
                if (surface.IsNull())
                {
                    return false;
                }

                Handle(TopTools_HSequenceOfShape) edges = new TopTools_HSequenceOfShape;
                for (const TopoDS_Face* faceTool : topoFacesTool)
                {
                    // 面面求交。
                    IntTools_FaceFace intFaFa;
                    intFaFa.Perform(*face, *faceTool);

                    const IntTools_SequenceOfCurves & curves = intFaFa.Lines();
                    int nLines = curves.Size();
                    if (nLines == 0)
                    {
                        // 判断工具面的边是否在被分割面上。
                        LocOpe_FindEdgesInFace fef(*faceTool, *face);
                        for (fef.Init(); fef.More(); fef.Next())
                        {
                            const TopoDS_Edge & edge = fef.Edge();
                            if (edge.IsNull())
                            {
                                continue;
                            }

                            edges->Append(edge);
                        }
                    }
                    else
                    {
                        IntTools_Curve intCurve = curves.First();
                        const Handle(Geom_Curve) & curve = intCurve.Curve();
                        if (curve.IsNull())
                        {
                            continue;
                        }

                        if (!intCurve.HasBounds())
                        {
                            continue;
                        }

                        Standard_Real first, last;
                        gp_Pnt firstPt, lastPt;
                        intCurve.Bounds(first, last, firstPt, lastPt);

                        // 压印。
                        Handle(Geom_Curve) projCurve = GeomProjLib::Project(curve, surface);
                        if (projCurve.IsNull())
                        {
                            continue;
                        }

                        gp_Pnt pntFirst = curve->Value(first);
                        gp_Pnt pntLast = curve->Value(last);

                        // 获取投影端点参数值。
                        GeomAPI_ProjectPointOnSurf projFirst(pntFirst, surface);
                        GeomAPI_ProjectPointOnSurf projLast(pntLast, surface);

                        pntFirst = projFirst.Point(1);
                        pntLast = projLast.Point(1);

                        Standard_Real newFirst, newLast;
                        if (!GeomLib_Tool::Parameter(projCurve, pntFirst, Precision::Confusion(), newFirst) ||
                            !GeomLib_Tool::Parameter(projCurve, pntLast, Precision::Confusion(), newLast))
                        {
                            continue;
                        }

                        double delta = last - first;
                        const TopoDS_Edge & newEdge = BRepBuilderAPI_MakeEdge(projCurve,
                            newFirst - delta * factor, newLast + delta * factor);
                        edges->Append(newEdge);
                    }
                }

                Handle(TopTools_HSequenceOfShape) ws = new TopTools_HSequenceOfShape;
                ShapeAnalysis_FreeBounds::ConnectEdgesToWires(edges, 1e-6, false, ws);
                int nWires = ws->Size();
                if (nWires == 0)
                {
                    continue;
                }

                for (int i = 1; i <= nWires; i++)
                {
                    const TopoDS_Wire & wire = TopoDS::Wire(ws->Value(i));
                    if (wire.IsNull())
                    {
                        continue;
                    }

                    splitter.Add(wire, *face);
                    nValidInt++;
                }           
            }

            splitter.Build();
            if (!splitter.IsDone() || nValidInt == 0)
            {
                return false;
            }
        }
        catch (...)
        {
            return false;
        }

        const TopoDS_Shape & newShape = splitter.Shape();
        if (newShape.IsNull())
        {
            return false;
        }

        part->updatePartShape(newShape, false);
        _occShapeAgent->updateShape(part->getCopiedPartShape(), false);

        return true;
    }
    //@}

    // 投影边分割面。
    //@{
    FITKOCCModelPartitionFaceWithProjectEdges::FITKOCCModelPartitionFaceWithProjectEdges() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelPartitionFaceWithProjectEdges::update()
    {
        FITKOCCModelPart* part = _occShapeAgent->getPart();
        if (!part || m_ProjectEdges.isEmpty())
        {
            return false;
        }

        FITKAbstractOCCModel* partShapeAgent = part->getTShapeAgent<FITKAbstractOCCModel>();
        if (!partShapeAgent)
        {
            return false;
        }

        TopoDS_Shape* partShape = partShapeAgent->getShape();
        if (!partShape)
        {
            return false;
        }

        Interface::FITKGeoCommandList* geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (!geoCmdList)
        {
            return false;
        }

        QList<TopoDS_Shape> edgeShapes;
        for (const Interface::VirtualShape & edge : m_ProjectEdges)
        {
            Interface::FITKAbsGeoCommand* cmd = geoCmdList->getDataByID(edge.CmdId);
            if (!cmd)
            {
                return false;
            }

            FITKAbstractOCCModel* occModel = cmd->getTShapeAgent<FITKAbstractOCCModel>();
            if (!occModel)
            {
                return false;
            }

            FITKOCCTopoShape* occEdge = cmd->getShapeTopoByIndexT<FITKOCCTopoShape>
                (Interface::FITKGeoEnum::VTopoShapeType::VSEdge, edge.VirtualTopoIndex);
            if (!occEdge)
            {
                return false;
            }

            const TopoDS_Shape & edgeShape = occEdge->getTopoShape();
            edgeShapes.push_back(edgeShape);
        }

        if (edgeShapes.isEmpty())
        {
            return false;
        }


        QList<Interface::FITKAbsVirtualTopo*> faces;
        QHash<Interface::FITKAbsVirtualTopo*, QList<Interface::FITKAbsVirtualTopo*>> solidFacesHash;
        QHash<const TopoDS_Face*, QList<Interface::FITKAbsVirtualTopo*>> faceSolidsHash;
        for (const Interface::VirtualShape & face : m_Faces)
        {
            Interface::FITKAbsGeoCommand* faceCmd = geoCmdList->getDataByID(face.CmdId);
            if (!faceCmd)
            {
                return false;
            }

            Interface::FITKAbsVirtualTopo* vFace = faceCmd->getVirtualTopoByIndex(
                Interface::FITKGeoEnum::VTopoShapeType::VSFace, face.VirtualTopoIndex);
            if (!vFace)
            {
                return false;
            }

            FITKOCCTopoShape* faceShape = vFace->getShapeT<FITKOCCTopoShape>();
            if (!faceShape)
            {
                return false;
            }

            const TopoDS_Face & topoFace = TopoDS::Face(faceShape->getTopoShape());
            if (topoFace.IsNull())
            {
                return false;
            }

            // 查询面是否属于实体。
            Interface::FITKVirtualTopoMapper vMapperFS;
            vMapperFS.mapTopo(vFace, Interface::FITKGeoEnum::VTopoShapeType::VSSolid);

            int nFaceSolids = vMapperFS.length();
            if (nFaceSolids == 0)
            {
                faceSolidsHash.insert(&topoFace, QList<Interface::FITKAbsVirtualTopo*>());
            }
            else
            {
                for (int i = 0; i < vMapperFS.length(); i++)
                {
                    Interface::FITKAbsVirtualTopo* vSolid = vMapperFS.virtualTopo(i);
                    faceSolidsHash[&topoFace].push_back(vSolid);
                    solidFacesHash[vSolid].push_back(vFace);
                }
            }

            faces.push_back(vFace);
        }

        if (faceSolidsHash.isEmpty())
        {
            return false;
        }

        // 分割面。
        //BRepFeat_SplitShape splitter;
        BRepTools_ReShape reShape;
        TopoDS_Shape newShape;

        try
        {
            TopoDS_Builder builder;
            TopoDS_Compound compoundFullShape;
            builder.MakeCompound(compoundFullShape);

            QHash<Interface::FITKAbsVirtualTopo*, BRepBuilderAPI_Sewing> solidSewerHash;

            // 初始化实体缝合器。
            double sewTol = 5.;
            QHashIterator<Interface::FITKAbsVirtualTopo*, QList<Interface::FITKAbsVirtualTopo*>> iter(solidFacesHash);
            while (iter.hasNext())
            {
                iter.next();

                BRepBuilderAPI_Sewing sewer;
                sewer.SetTolerance(sewTol);
                solidSewerHash.insert(iter.key(), sewer);
            }

            //splitter.Init(*partShape);

            // 初始化分割数据。
            QHashIterator<const TopoDS_Face*, QList<Interface::FITKAbsVirtualTopo*>> iterFace(faceSolidsHash);
            while (iterFace.hasNext())
            {
                iterFace.next();

                BRep_Builder builder;
                TopoDS_Compound compound;
                builder.MakeCompound(compound);

                // 获取截取曲线进行压印。
                for (const TopoDS_Shape & shape : edgeShapes)
                {
                    const TopoDS_Edge & edgeShape = TopoDS::Edge(shape);
                    if (edgeShape.IsNull())
                    {
                        return false;
                    }

                    BRepOffsetAPI_NormalProjection bProj(*iterFace.key());
                    bProj.Add(edgeShape);
                    bProj.Build();
                    if (!bProj.IsDone())
                    {
                        continue;
                    }

                    builder.Add(compound, bProj);
                }

                if (FITKOCCExtendTool::IsEmptyShape(compound))
                {
                    continue;
                }

                // 重组线框。
                TopoDS_Compound compoundWires;
                builder.MakeCompound(compoundWires);
                FITKOCCComandCommon::MakeWires(compound, compoundWires);

                BOPAlgo_Splitter splitter;
                splitter.AddArgument(*iterFace.key());
                splitter.AddTool(compoundWires);
                splitter.Perform();

                const TopoDS_Shape & newShape = splitter.Shape();
                if (newShape.IsNull())
                {
                    return false;
                }

                // 实体面添加至实体缝合器。
                QList<Interface::FITKAbsVirtualTopo*> vSolids = iterFace.value();

                if (!vSolids.isEmpty())
                {
                    for (Interface::FITKAbsVirtualTopo* vSolid : vSolids)
                    {
                        solidSewerHash[vSolid].Add(newShape);
                    }
                }
                // 壳面直接进行替换。
                else
                {
                    reShape.Replace(*iterFace.key(), newShape);
                }
            }

            // 填充实体缝合器其他未发生变化的面。
            QHashIterator<Interface::FITKAbsVirtualTopo*, BRepBuilderAPI_Sewing> iterSewer(solidSewerHash);
            while (iterSewer.hasNext())
            {
                iterSewer.next();

                Interface::FITKAbsVirtualTopo* vSolid = iterSewer.key();

                // 获取原始实体形状。
                OCC::FITKOCCTopoShape* vOCCShapeSolid = vSolid->getShapeT<OCC::FITKOCCTopoShape>();
                if (!vOCCShapeSolid)
                {
                    return false;
                }

                const TopoDS_Shape & topoSolid = vOCCShapeSolid->getTopoShape();
                if (topoSolid.IsNull())
                {
                    return false;
                }

                // 组合实体其他原始面。
                Interface::FITKVirtualTopoMapper vMapperSF;
                vMapperSF.mapTopo(vSolid, Interface::FITKGeoEnum::VSFace);

                int nSolidFaces = vMapperSF.length();
                for (int i = 0; i < nSolidFaces; i++)
                {
                    Interface::FITKAbsVirtualTopo* vFace = vMapperSF.virtualTopo(i);

                    // 跳过已变化的面。
                    if (!faces.contains(vFace))
                    {
                        OCC::FITKOCCTopoShape* vOCCShapeFace = vFace->getShapeT<OCC::FITKOCCTopoShape>();
                        if (!vOCCShapeFace)
                        {
                            return false;
                        }

                        const TopoDS_Face & topoFace = TopoDS::Face(vOCCShapeFace->getTopoShape());
                        if (topoFace.IsNull())
                        {
                            return false;
                        }

                        solidSewerHash[vSolid].Add(topoFace);
                    }
                }

                // 执行缝合，替换实体。
                solidSewerHash[vSolid].Perform();

                const TopoDS_Shape & newSolidSewed = solidSewerHash[vSolid].SewedShape();

                // 重新创建实体。
                QList<QList<TopoDS_Shell>> shellsGroups;
                for (TopExp_Explorer iter(newSolidSewed, TopAbs_ShapeEnum::TopAbs_SHELL); iter.More(); iter.Next())
                {
                    const TopoDS_Shell & ss = TopoDS::Shell(iter.Value());
                    reShape.Remove(topoSolid);

                    //if (!ss.Closed())
                    //{
                    //    continue;
                    //}

                    TopoDS_Shell newShell = TopoDS::Shell(BRepBuilderAPI_Copy(ss));

                    Bnd_Box box;
                    BRepBndLib::Add(ss, box, false);

                    // 分组判断包围盒。
                    bool breakFlag = false;
                    bool inGroup = false;
                    for (QList<TopoDS_Shell> & shells : shellsGroups)
                    {
                        int index = -1;
                        int nShells = shells.count();
                        for (const TopoDS_Shell & shell : shells)
                        {
                            index++;

                            Bnd_Box boxTemp;
                            BRepBndLib::Add(shell, boxTemp, false);

                            // 比较盒子。
                            bool notThisGroup = false;
                            BoxCompare boxRet = FITKOCCComandCommon::CompareBox(box, boxTemp);
                            switch (boxRet)
                            {
                            case OCC::BC_1_in_2:
                            {
                                if (FITKOCCExtendTool::IsEmptyShape(BRepAlgoAPI_Section(ss, shell)))
                                {
                                    if (nShells - 1 == index)
                                    {
                                        inGroup = true;
                                        breakFlag = true;
                                        shells.push_back(newShell);
                                        break;
                                    }
                                    else
                                    {
                                        continue;
                                        break;
                                    }
                                }
                                else
                                {
                                    notThisGroup = true;
                                    break;
                                }
                            }
                            case OCC::BC_2_in_1:
                            {
                                if (FITKOCCExtendTool::IsEmptyShape(BRepAlgoAPI_Section(ss, shell)))
                                {
                                    inGroup = true;
                                    breakFlag = true;
                                    shells.insert(index, newShell);
                                    break;
                                }
                                else
                                {
                                    if (index > 0)
                                    {
                                        inGroup = true;
                                        breakFlag = true;
                                        shells.insert(index - 1, newShell);
                                    }

                                    break;
                                }
                            }
                            default:
                            {
                                notThisGroup = true;
                                break;
                            }
                            }

                            // 跳出当前组。
                            if (notThisGroup || breakFlag)
                            {
                                break;
                            }
                        }

                        if (breakFlag)
                        {
                            break;
                        }
                    }

                    if (!inGroup)
                    {
                        shellsGroups.push_back(QList<TopoDS_Shell>{ newShell });
                    }
                }

                for (QList<TopoDS_Shell> & shells : shellsGroups)
                {
                    QList<TopoDS_Shell> newList = shells;
                    BRepBuilderAPI_MakeSolid ms(newList.takeFirst());

                    for (const TopoDS_Shell & shell : newList)
                    {
                        ms.Add(shell);
                    }

                    ms.Build();

                    bool hasSolid = false;
                    if (ms.IsDone())
                    {
                        const TopoDS_Solid & solid = ms;
                        if (!solid.IsNull())
                        {
                            builder.Add(compoundFullShape, solid);
                            hasSolid = true;
                        }
                    }

                    if (!hasSolid)
                    {
                        for (const TopoDS_Shell & s : newList)
                        {
                            builder.Add(compoundFullShape, s);
                        }
                    }
                }
            }

            // 形状为空则说明所有体上的面均发生变化。
            const TopoDS_Shape & resultShape = reShape.Apply(*partShape);
            if (!resultShape.IsNull())
            {
                builder.Add(compoundFullShape, resultShape);
            }

            newShape = compoundFullShape;
        }
        catch (...)
        {
            return false;
        }

        //const TopoDS_Shape & newShape = splitter.Shape();
        //if (newShape.IsNull())
        //{
        //    return false;
        //}

        part->updatePartShape(newShape, false);
        _occShapeAgent->updateShape(part->getCopiedPartShape(), false);

        return true;
    }
    //@}
}
