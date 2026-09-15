/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCModelOperLine.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include <TopoDS.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Compound.hxx>

#include <gp_Pnt.hxx>
#include <BRepLib.hxx>
#include <ShapeFix_Wire.hxx>
#include <ShapeBuild.hxx>
#include <Geom_TrimmedCurve.hxx>

#include <BRep_Builder.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepTools_ReShape.hxx>
#include <TopExp_Explorer.hxx>
#include <ShapeFix_Shape.hxx>
#include <BRepFill_Filling.hxx>
#include <BRepBuilderAPI_Sewing.hxx>
#include <BRepBuilderAPI_MakeSolid.hxx>
#include <BRepGProp.hxx>
#include <GProp_GProps.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>

// Global data
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"

// Geometry
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsVirtualTopoMapper.h"
#include "FITKOCCVirtualTopoCreator.h"
#include "FITKOCCModelPart.h"
#include "FITKOCCComandCommon.h"

#include <QDebug>
namespace OCC
{
    FITKOCCModelOperLine::FITKOCCModelOperLine() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }
    bool FITKOCCModelOperLine::update()
    {
        if (m_tempVShapes.size() < 1) return false;
        QList<TopoDS_Shape> shapeLines;

        for (int i = 0; i < m_tempVShapes.size(); ++i)
        {
            //数据获取
            FITKAbsGeoCommand* comm = FITKDATAREPO->getTDataByID<FITKAbsGeoCommand>(m_tempVShapes[i].CmdId);
            if (comm == nullptr) return false;
            OCC::FITKOCCTopoShape* vOCCShapeEdge = comm->getShapeTopoByIndexT<OCC::FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSEdge, m_tempVShapes[i].VirtualTopoIndex);
            if (vOCCShapeEdge == nullptr) return false;

            shapeLines.append(vOCCShapeEdge->getTopoShape());
        }
        //按类型处理
        if (_lineOperType == Interface::FITKAbsGeoModelOperLine::GBTMerge)
        {
            return updateMerge(shapeLines);
        }
        if (_lineOperType == Interface::FITKAbsGeoModelOperLine::GBTSplitByAngle)
        {
            return updateSplitByAngle(shapeLines);
        }
        return false;
    }

    bool FITKOCCModelOperLine::updateMerge(QList<TopoDS_Shape> shapes)
    {
        if (shapes.size() < 1) return false;

        BRepBuilderAPI_MakeWire mergeWires;
        for (int i = 0; i < shapes.size(); i++)
        {
            TopoDS_Edge edge = TopoDS::Edge(shapes[i]);
            if (edge.IsNull()) return false;
            mergeWires.Add(BRepBuilderAPI_MakeWire(edge));
        }
        mergeWires.Build();
        qDebug() << mergeWires.Error();
        if (!mergeWires.IsDone()) return false;
        _occShapeAgent->updateShape(mergeWires);
        return true;
    }
    
    bool FITKOCCModelOperLine::updateSplitByAngle(QList<TopoDS_Shape> shapes)
    {
        if (shapes.size() != 1) return false;
        if (shapes[0].ShapeType() != TopAbs_ShapeEnum::TopAbs_EDGE && shapes[0].ShapeType() != TopAbs_ShapeEnum::TopAbs_WIRE) return false;



        return true;
    }


    FITKOCCModelMiddleOperLine::FITKOCCModelMiddleOperLine() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelMiddleOperLine::update()
    {
        if (m_tempVShapes.size() < 1) return false;
        QList<TopoDS_Shape> shapeLines;

        for (int i = 0; i < m_tempVShapes.size(); ++i)
        {
            //数据获取
            FITKAbsGeoCommand* comm = FITKDATAREPO->getTDataByID<FITKAbsGeoCommand>(m_tempVShapes[i].CmdId);
            if (comm == nullptr) return false;

            OCC::FITKOCCTopoShape* vOCCShapeEdge = comm->getShapeTopoByIndexT<OCC::FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSEdge, m_tempVShapes[i].VirtualTopoIndex);
            if (vOCCShapeEdge == nullptr) return false;

            shapeLines.append(vOCCShapeEdge->getTopoShape());
        }

        return updateSplitByMiddlePosition(shapeLines);
    }

    bool FITKOCCModelMiddleOperLine::updateSplitByMiddlePosition(QList<TopoDS_Shape> shapes)
    {
        if (shapes.size() != 1) return false;
        //暂时只处理了edge
        if (shapes[0].ShapeType() != TopAbs_ShapeEnum::TopAbs_EDGE /*&& shapes[0].ShapeType() != TopAbs_ShapeEnum::TopAbs_WIRE*/) return false;

        TopoDS_Edge edge = TopoDS::Edge(shapes[0]);
        if (edge.IsNull()) return false;
        TopoDS_Edge partOfEOne, partOfETwo;
        Standard_Real startParam(0.), endParam(0.);
        Handle(Geom_Curve) c = BRep_Tool::Curve(edge, startParam, endParam);//得到底层曲线
        //底层曲线没有创建，要手动创建
        if (c.IsNull())
        {
            BRepLib::BuildCurves3d(edge, 1.0e-5, GeomAbs_C1);//创建曲线
            c = BRep_Tool::Curve(edge, startParam, endParam);
        }
        //简单分割方法，该方法分割的可能不是均匀的曲线
        if (!c.IsNull())
        {
            //截取曲线并创建边
            Handle(Geom_TrimmedCurve) tc1 = new Geom_TrimmedCurve(c, startParam, (startParam + endParam) * 0.5);//截取边的的一半
            partOfEOne = BRepBuilderAPI_MakeEdge(tc1);
            //截取曲线并创建边
            Handle(Geom_TrimmedCurve) tc2 = new Geom_TrimmedCurve(c, (startParam + endParam) * 0.5, endParam);//截取边的的一半
            partOfETwo = BRepBuilderAPI_MakeEdge(tc2);
            _occShapeAgent->updateShape(BRepBuilderAPI_MakeWire(partOfEOne, partOfETwo));
            return true;
        }
        return false;
    }

    // 修复小边。
    //@{
    FITKOCCModelRepairSmallEdges::FITKOCCModelRepairSmallEdges() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelRepairSmallEdges::update()
    {
        QList<Interface::FITKAbsVirtualTopo*> edgeVertsFaces;
        QList<Interface::FITKAbsVirtualTopo*> edgeVerts;
        QHash<Interface::FITKAbsVirtualTopo*, gp_Pnt> orgVertNewVertHash;
        QHash<Interface::FITKAbsVirtualTopo*, QList<Interface::FITKAbsVirtualTopo*>> faceSolidsHash;
        QList<TopoDS_Shape*> fullShapes;
        QList<const TopoDS_Edge*> edges;
        for (const Interface::VirtualShape & shape : m_Edges)
        {
            Interface::FITKAbsGeoCommand* cmd = FITKDATAREPO->getTDataByID<Interface::FITKAbsGeoCommand>(shape.CmdId);
            if (!cmd)
            {
                return false;
            }

            TopoDS_Shape* topoShape{ nullptr };
            OCC::FITKAbstractOCCModel* occModel = cmd->getTShapeAgent<OCC::FITKAbstractOCCModel>();
            if (occModel)
            {
                topoShape = occModel->getShape();
            }

            Interface::FITKAbsVirtualTopo* vEdge = cmd->getVirtualTopoByIndex(
                Interface::FITKGeoEnum::VTopoShapeType::VSEdge, shape.VirtualTopoIndex);
            if (!vEdge)
            {
                continue;
            }

            OCC::FITKOCCTopoShape* vOCCShapeEdge = vEdge->getShapeT<OCC::FITKOCCTopoShape>();
            if (!vOCCShapeEdge)
            {
                continue;
            }

            const TopoDS_Edge & topoEdge = TopoDS::Edge(vOCCShapeEdge->getTopoShape());
            if (topoEdge.IsNull())
            {
                continue;
            }

            // 降维边，获取中点。
            Standard_Real first, last;
            Handle(Geom_Curve) curve = BRep_Tool::Curve(topoEdge, first, last);
            if (curve.IsNull())
            {
                continue;
            }

            gp_Pnt mid;
            curve->D0(first + (last - first) * 0.5, mid);

            if (!fullShapes.contains(topoShape))
            {
                fullShapes.push_back(topoShape);
            }

            edges.push_back(&topoEdge);

            // 尝试查询边构成边的实体。
            QList<Interface::FITKAbsVirtualTopo*> vSolids;

            Interface::FITKVirtualTopoMapper vMapperES;
            vMapperES.mapTopo(vEdge, Interface::FITKGeoEnum::VSSolid);

            int nEdgeSolids = vMapperES.length();

            // 获取边所属实体。
            for (int i = 0; i < nEdgeSolids; i++)
            {
                vSolids.push_back(vMapperES.virtualTopo(i));
            }

            // 查询构成边的节点。
            Interface::FITKVirtualTopoMapper vMapperEV;
            vMapperEV.mapTopo(vEdge, Interface::FITKGeoEnum::VSPoint);

            int nEdgeVerts = vMapperEV.length();
            for (int i = 0; i < nEdgeVerts; i++)
            {
                Interface::FITKAbsVirtualTopo* vVert = vMapperEV.virtualTopo(i);
                if (!vVert)
                {
                    return false;
                }

                if (!edgeVerts.contains(vVert))
                {
                    edgeVerts.push_back(vVert);
                }
                else
                {
                    continue;
                }

                // 保存新点映射。
                orgVertNewVertHash.insert(vVert, mid);

                // 查询使用该节点的面。
                Interface::FITKVirtualTopoMapper vMapperVF;
                vMapperVF.mapTopo(vVert, Interface::FITKGeoEnum::VSFace);

                int nVertFaces = vMapperVF.length();
                for (int j = 0; j < nVertFaces; j++)
                {
                    Interface::FITKAbsVirtualTopo* vFace = vMapperVF.virtualTopo(j);
                    if (!vFace)
                    {
                        return false;
                    }

                    if (!edgeVertsFaces.contains(vFace))
                    {
                        edgeVertsFaces.push_back(vFace);

                        // 查询使用该面的实体。
                        Interface::FITKVirtualTopoMapper vMapperFS;
                        vMapperFS.mapTopo(vFace, Interface::FITKGeoEnum::VSSolid);

                        int nFaceSolids = vMapperFS.length();
                        for (int k = 0; k < nFaceSolids; k++)
                        {
                            Interface::FITKAbsVirtualTopo* vSolid = vMapperFS.virtualTopo(k);

                            if (vSolids.contains(vSolid))
                            {
                                faceSolidsHash[vFace].push_back(vSolid);
                            }
                        }
                    }
                }
            }
        }

        if (edges.isEmpty())
        {
            return false;
        }

        FITKOCCModelPart* part = _occShapeAgent->getPart();

        // 移除边。
        //@{
        try
        {
            double sewTol = 5.;
            QHash<Interface::FITKAbsVirtualTopo*, BRepBuilderAPI_Sewing> solidSewerHash;

            // 初始化实体缝合器。
            QHashIterator<Interface::FITKAbsVirtualTopo*, QList<Interface::FITKAbsVirtualTopo*>> iter(faceSolidsHash);
            while (iter.hasNext())
            {
                iter.next();

                for (Interface::FITKAbsVirtualTopo* vSolid : iter.value())
                {
                    BRepBuilderAPI_Sewing sewer;
                    sewer.SetTolerance(sewTol);
                    solidSewerHash.insert(vSolid, sewer);
                }
            }

            TopoDS_Builder builder;
            TopoDS_Compound compound;
            builder.MakeCompound(compound);

            Handle(BRepTools_ReShape) reShape = new BRepTools_ReShape;

            // 移除面内的边，重新连接缺失点。
            for (Interface::FITKAbsVirtualTopo* vFace : edgeVertsFaces)
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

                BRep_Builder builder;
                TopoDS_Compound compoundEdges;
                builder.MakeCompound(compoundEdges);

                BRepFill_Filling nSideFill;

                // 查询该面的边。
                Interface::FITKVirtualTopoMapper vMapperFE;
                vMapperFE.mapTopo(vFace, Interface::FITKGeoEnum::VSEdge);

                int nFaceEdges = vMapperFE.length();
                for (int i = 0; i < nFaceEdges; i++)
                {
                    Interface::FITKAbsVirtualTopo* vEdge = vMapperFE.virtualTopo(i);
                    if (!vEdge)
                    {
                        return false;
                    }

                    // 查询边的点，如果与被删除边吻合则跳过。
                    OCC::FITKOCCTopoShape* vOCCShapeEdge = vEdge->getShapeT<OCC::FITKOCCTopoShape>();
                    if (!vOCCShapeEdge)
                    {
                        return false;
                    }

                    const TopoDS_Edge & topoEdge = TopoDS::Edge(vOCCShapeEdge->getTopoShape());
                    if (topoEdge.IsNull())
                    {
                        return false;
                    }

                    if (edges.contains(&topoEdge))
                    {
                        continue;
                    }

                    // 如果有一个点与移除的边相连则将该边修改为直边。
                    Interface::FITKVirtualTopoMapper vMapperEV;
                    vMapperEV.mapTopo(vEdge, Interface::FITKGeoEnum::VSPoint);

                    int nEdgeVerts = vMapperEV.length();
                    QList<Interface::FITKAbsVirtualTopo*> verts;
                    for (int j = 0; j < nEdgeVerts; j++)
                    {
                        Interface::FITKAbsVirtualTopo* vVert = vMapperEV.virtualTopo(j);
                        verts.push_back(vVert);
                    }

                    if (verts.isEmpty())
                    {
                        return false;
                    }

                    OCC::FITKOCCTopoShape* vOCCShapeVertLast = verts.last()->getShapeT<OCC::FITKOCCTopoShape>();
                    if (!vOCCShapeVertLast)
                    {
                        return false;
                    }

                    const TopoDS_Vertex & topoVertLast = TopoDS::Vertex(vOCCShapeVertLast->getTopoShape());
                    if (topoVertLast.IsNull())
                    {
                        return false;
                    }

                    OCC::FITKOCCTopoShape* vOCCShapeVertFirst = verts.first()->getShapeT<OCC::FITKOCCTopoShape>();
                    if (!vOCCShapeVertFirst)
                    {
                        return false;
                    }

                    const TopoDS_Vertex & topoVertFirst = TopoDS::Vertex(vOCCShapeVertFirst->getTopoShape());
                    if (topoVertFirst.IsNull())
                    {
                        return false;
                    }

                    // 起始点相连则与终止点重组直边。
                    if (edgeVerts.contains(verts.first()))
                    {
                        // 降维点 -> 终止点
                        gp_Pnt pnt = orgVertNewVertHash.value(verts.first(), gp_Pnt());
                        const TopoDS_Edge & newEdge = BRepBuilderAPI_MakeEdge(
                            BRepBuilderAPI_MakeVertex(pnt), topoVertLast);

                        nSideFill.Add(newEdge, GeomAbs_Shape::GeomAbs_C0, true);
                        builder.Add(compoundEdges, newEdge);
                    }
                    // 终止点相连则与起始点重组直边。
                    else if (edgeVerts.contains(verts.last()))
                    {
                        // 起始点 -> 降维点
                        gp_Pnt pnt = orgVertNewVertHash.value(verts.last(), gp_Pnt());
                        const TopoDS_Edge & newEdge = BRepBuilderAPI_MakeEdge(
                            topoVertFirst, BRepBuilderAPI_MakeVertex(pnt));   

                        nSideFill.Add(newEdge, GeomAbs_Shape::GeomAbs_C0, true);
                        builder.Add(compoundEdges, newEdge);
                    }
                    // 不相连则保持原有边。
                    else
                    {
                        builder.Add(compoundEdges, topoEdge);
                        nSideFill.Add(topoEdge, GeomAbs_Shape::GeomAbs_C0, true);
                    }
                }

                // 重组。
                QList<TopoDS_Wire> wires;
                FITKOCCComandCommon::MakeWires(compoundEdges, wires, 1e-5);

                TopoDS_Face face;

                // 如果产生嵌套多段线则尝试使用原始曲面创建新面。
                if (wires.count() == 1)
                {
                    // 覆盖边。
                    nSideFill.Build(); // TODO 此处可能出现系统无法捕获的异常导致崩溃。
                    if (!nSideFill.IsDone())
                    {
                        return false;
                    }

                    face = nSideFill.Face();
                }
                else
                {
                    Handle(Geom_Surface) surface = BRep_Tool::Surface(topoFace);
                    if (!surface)
                    {
                        return false;
                    }

                    // 根据线框周长排序。
                    QList<TopoDS_Wire> wiresSort;
                    for (const TopoDS_Wire & wireUnsort : wires)
                    {
                        GProp_GProps props;
                        BRepGProp::LinearProperties(wireUnsort, props);
                        double len = props.Mass();

                        bool hasInstered = false;
                        for (int i = 0 ; i < wiresSort.count() ; i ++)
                        {
                            const TopoDS_Wire & wireSorted = wiresSort[i];

                            GProp_GProps propsOther;
                            BRepGProp::LinearProperties(wireSorted, propsOther);
                            double lenOther = propsOther.Mass();

                            if (len > lenOther)
                            {
                                hasInstered = true;
                                wiresSort.insert(i, wireUnsort);
                                break;
                            }
                        }

                        if (!hasInstered)
                        {
                            wiresSort.push_back(wireUnsort);
                        }
                    }

                    if (wiresSort.isEmpty())
                    {
                        return false;
                    }

                    BRepBuilderAPI_MakeFace mkFace(wiresSort.takeFirst());
                    mkFace.Init(surface, false, Precision::Confusion());
                    for (const TopoDS_Wire & wire : wiresSort)
                    {
                        mkFace.Add(wire);
                    }

                    mkFace.Build();

                    if (!mkFace.IsDone())
                    {
                        return false;
                    }

                    face = mkFace.Face();
                }
                
                if (face.IsNull())
                {
                    return false;
                }

                // 尝试将面添加至缝合列表。
                QList<Interface::FITKAbsVirtualTopo*> vSolids = faceSolidsHash.value(vFace, QList<Interface::FITKAbsVirtualTopo*>());
                if (!vSolids.isEmpty())
                {
                    for (Interface::FITKAbsVirtualTopo* vSolid : vSolids)
                    {
                        solidSewerHash[vSolid].Add(face);
                    }
                }
                else
                {
                    // 替换面。
                    reShape->Replace(topoFace, face);
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

                Interface::FITKVirtualTopoMapper vMapperSF;
                vMapperSF.mapTopo(vSolid, Interface::FITKGeoEnum::VSFace);

                int nSolidFaces = vMapperSF.length();
                for (int i = 0; i < nSolidFaces; i++)
                {
                    Interface::FITKAbsVirtualTopo* vFace = vMapperSF.virtualTopo(i);

                    // 跳过已重组的面。
                    if (!edgeVertsFaces.contains(vFace))
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

                // 重新创建实体。
                bool hasShell = false;
                TopoDS_Shape sewedShape = solidSewerHash[vSolid].SewedShape();
                for (TopExp_Explorer iter(sewedShape, TopAbs_ShapeEnum::TopAbs_SHELL); iter.More(); iter.Next())
                {
                    const TopoDS_Shell & ss = TopoDS::Shell(iter.Value());
                    //if (!ss.Closed())
                    //{
                    //    continue;
                    //}

                    const TopoDS_Solid & newSolid = BRepBuilderAPI_MakeSolid(ss);
                    if (!newSolid.IsNull())
                    {
                        builder.Add(compound, newSolid);
                        hasShell = true;
                    }
                }

                if (hasShell)
                {
                    reShape->Remove(topoSolid);
                }
            }

            for (TopoDS_Shape* fullShape : fullShapes)
            {
                // TopAbs_ShapeEnum sType = fullShape->ShapeType();
                const TopoDS_Shape & newShape = reShape->Apply(*fullShape);
                if (newShape.IsNull())
                {
                    continue;
                }

                ShapeFix_Shape fix(newShape);
                fix.Perform();

                //BRepBuilderAPI_Sewing sewing;
                //sewing.Load(fix.Shape());
                //sewing.SetTolerance(3);
                //sewing.Perform();

                //builder.Add(compound, sewing.SewedShape());

                builder.Add(compound, fix.Shape());
            }

            if (part)
            {
                part->updatePartShape(compound, false);
                _occShapeAgent->updateShape(part->getCopiedPartShape(), false);
            }
            else
            {
                _occShapeAgent->updateShape(compound);
            }
        }
        catch (...)
        {
            return false;
        }
        //@}

        return true;
    }
    //@}

}