/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCModelMergeEdges.h"

// App
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

// Geometry
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoEnum.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKVirtualTopoManager.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelSurface.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeomTools.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsVirtualTopoMapper.h"

// OCC
#include "FITKOCCComandCommon.h"
#include "FITKOCCExtendTool.h"
#include "FITKOCCModelPart.h"
#include "FITKOCCVirtualTopoCreator.h"

#include <ShapeUpgrade_UnifySameDomain.hxx>
#include <TopoDS_Builder.hxx>
#include <TopoDS_Compound.hxx>
#include <TopExp_Explorer.hxx>
#include <BRepTools_ReShape.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <GeomConvert_CompCurveToBSplineCurve.hxx>
#include <BRep_Tool.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <Geom_BSplineCurve.hxx>
#include <BRepTools.hxx>
#include <GeomLib_Tool.hxx>
#include <Geom_Curve.hxx>

#include <QDebug>

namespace OCC
{
    FITKOCCModelMergeEdges::FITKOCCModelMergeEdges() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelMergeEdges::update()
    {
        int nEdges = m_Edges.size();
        if (nEdges < 2)
        {
            return false;
        }

        // 获取部件。
        FITKOCCModelPart* part = _occShapeAgent->getPart();
        if (!part)
        {
            return false;
        }

        Interface::FITKGeoCommandList* cmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (!cmdList)
        {
            return false;
        }

        Interface::FITKShapeVirtualTopoManager* vEdgeMgr = part->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VSEdge);
        if (!vEdgeMgr)
        {
            return false;
        }

        int nAllEdges = vEdgeMgr->getDataCount();

        Interface::FITKVirtualTopoMapper vMapperEV;
        Interface::FITKVirtualTopoMapper vMapperEF;

        // 拾取线连续性与分支检查。
        QHash<Interface::FITKAbsVirtualTopo*, int> vertCounter;

        // 分支计数，超过1则无法进行合并。
        QHash<Interface::FITKAbsVirtualTopo*, int> faceCounter;

        QList<int> topoIds;
        for (const Interface::VirtualShape & s : m_Edges)
        {
            Interface::FITKAbsGeoCommand* edgeCmd = cmdList->getDataByID(s.CmdId);
            if (!edgeCmd)
            {
                return false;
            }

            Interface::FITKAbsVirtualTopo* vEdge = edgeCmd->getVirtualTopoByIndex(
                Interface::FITKGeoEnum::VTopoShapeType::VSEdge, s.VirtualTopoIndex);
            if (!vEdge)
            {
                return false;
            }

            FITKOCCTopoShape* vEdgeShape = vEdge->getShapeT<FITKOCCTopoShape>();
            if (!vEdgeShape)
            {
                return false;
            }

            // 查询合并边所使用点并计数。
            vMapperEV.mapTopo(vEdge, Interface::FITKGeoEnum::VTopoShapeType::VSPoint);
            int nEdgeVerts = vMapperEV.length();
            if (nEdgeVerts == 0)
            {
                return false;
            }

            for (int i = 0; i < nEdgeVerts; i++)
            {
                Interface::FITKAbsVirtualTopo* vVert = vMapperEV.virtualTopo(i);
                if (!vVert)
                {
                    return false;
                }

                vertCounter.insert(vVert, vertCounter.value(vVert, 0) + 1);
            }
  
            // 查询合并边所在面并计数。
            vMapperEF.mapTopo(vEdge, Interface::FITKGeoEnum::VTopoShapeType::VSFace);
            int nEdgeFaces = vMapperEF.length();
            if (nEdgeFaces == 0)
            {
                return false;
            }

            for (int i = 0; i < nEdgeFaces; i++)
            {
                Interface::FITKAbsVirtualTopo* vFace = vMapperEF.virtualTopo(i);
                if (!vFace)
                {
                    return false;
                }

                faceCounter.insert(vFace, faceCounter.value(vFace, 0) + 1);
            }

            const TopoDS_Shape & edge = vEdgeShape->getTopoShape();
            if (edge.IsNull())
            {
                return false;
            }

            topoIds.push_back(vEdge->getDataObjectID());
        }

        if (faceCounter.isEmpty())
        {
            return false;
        }

        QList<int> fts = faceCounter.values();
        if (fts.count(fts[0]) != fts.count())
        {
            return false;
        }

        // 线排序。
        QList<QPair<int, QPair<int, int>>> loop = Interface::FITKAbstractGeomEdgeTool::sortEdgeInfos(topoIds);
        if (loop.count() != topoIds.count())
        {
            return false;
        }

        // 处理线节点顺序。
        for (int i = 0; i < loop.count(); i++)
        {
            QPair<int, QPair<int, int>> edgeInfo = loop[0];

            // 前置步骤已判空。
            Interface::FITKAbsVirtualTopo* vEdge = vEdgeMgr->getDataByID(edgeInfo.first);
            FITKOCCTopoShape* vEdgeShape = vEdge->getShapeT<FITKOCCTopoShape>();
            const TopoDS_Edge & edge = TopoDS::Edge(vEdgeShape->getTopoShape());

            // 判断是否为反向。
            bool isReverse = edge.Orientation() != TopAbs_Orientation::TopAbs_FORWARD;
            if (isReverse)
            {
                QPair<int, QPair<int, int>> edgeInfoSwap = loop.takeFirst();
                edgeInfoSwap.second.first = edgeInfo.second.second;
                edgeInfoSwap.second.second = edgeInfo.second.first;
                loop.push_back(edgeInfoSwap);
            }
            else
            {
                loop.push_back(loop.takeFirst());
            }
        }

        BRepTools_ReShape reShape;

        // 周期曲线判断。
        Interface::FITKVirtualTopoMapper vMapperVE;
        QHashIterator<Interface::FITKAbsVirtualTopo*, int> vertIter(vertCounter);

        bool isLoop = false;
        gp_Pnt startPos;
        if (vertCounter.values().count(2) == vertCounter.count())
        {
            isLoop = true;
            int vertBranchId = -1;
            int nBranches = 0;

            // 首尾闭合可最多出现一次分支，且分支必须作为起始与终止位置。
            while (vertIter.hasNext())
            {
                vertIter.next();

                Interface::FITKAbsVirtualTopo* vVert = vertIter.key();
                int t = vertIter.value();

                // 分叉计数。
                if (t > 2)
                {
                    return false;
                }

                FITKOCCTopoShape* vVertShape = vVert->getShapeT<FITKOCCTopoShape>();
                const TopoDS_Shape & s = vVertShape->getTopoShape();

                vMapperVE.mapTopo(vVert, Interface::FITKGeoEnum::VSEdge);
                if (vMapperVE.length() > 2)
                {
                    vertBranchId = vVert->getDataObjectID();
                    nBranches++;

                    // 记录环形起始坐标。
                    const TopoDS_Vertex & v = TopoDS::Vertex(s);
                    startPos = BRep_Tool::Pnt(v);
                }
                else
                {
                    // 移除非分支共点。
                    reShape.Remove(s);
                }

                if (nBranches > 1)
                {
                    return false;
                }
            }

            // 尝试重新排序，防止端点位置分支点合并错误。
            for (int i = 0; i < loop.count(); i++)
            {
                QPair<int, QPair<int, int>> edgeInfo = loop[0];
                QPair<int, QPair<int, int>> edgeInfoLast = loop[loop.count() - 1];

                if (edgeInfo.second.first == vertBranchId && edgeInfoLast.second.second == vertBranchId)
                {
                    break;
                }
                else if (edgeInfo.second.second == vertBranchId && edgeInfoLast.second.first == vertBranchId)
                {
                    std::reverse(loop.begin(), loop.end());
                    break;
                }
                else
                {
                    loop.push_back(loop.takeFirst());
                }
            }
        }
        else
        {
            // 线框分支检查。
            int exts = 0;
            while (vertIter.hasNext())
            {
                vertIter.next();

                int t = vertIter.value();
                if (t == 1)
                {
                    exts++;
                }
                // 分叉跳出。
                else if (t > 2)
                {
                    return false;
                }
                // 共点判断模型内分叉数量。
                else
                {
                    vMapperVE.mapTopo(vertIter.key(), Interface::FITKGeoEnum::VSEdge);
                    if (vMapperVE.length() > 2)
                    {
                        return false;
                    }
                }
            }

            // 多段，不连续跳出。
            if (exts > 2)
            {
                return false;
            }

            // 尝试重新排序。
            QPair<int, QPair<int, int>> edgeInfo1 = loop[0];
            QPair<int, QPair<int, int>> edgeInfo2 = loop[1];
            if (edgeInfo1.second.second != edgeInfo2.second.first)
            {
                std::reverse(loop.begin(), loop.end());
            }
        }

        // 合并曲线。
        TopTools_ListOfShape edges;
        GeomConvert_CompCurveToBSplineCurve mergeCurve;
        for (const QPair<int, QPair<int, int>> & edgeInfo : loop)
        {
            // 前置步骤已判空。
            Interface::FITKAbsVirtualTopo* vEdge = vEdgeMgr->getDataByID(edgeInfo.first);
            FITKOCCTopoShape* vEdgeShape = vEdge->getShapeT<FITKOCCTopoShape>();
            const TopoDS_Edge & edge = TopoDS::Edge(vEdgeShape->getTopoShape());

            // 获取曲线。
            Standard_Real first, last;
            Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, first, last);
            if (curve.IsNull())
            {
                return false;
            }

            // 创建截取曲线。
            Handle(Geom_TrimmedCurve) tCurve = new Geom_TrimmedCurve(curve, first, last);
            if (tCurve.IsNull())
            {
                return false;
            }

            if (edge.Orientation() != TopAbs_Orientation::TopAbs_FORWARD)
            {
                tCurve->Reverse();
            }

            if (!mergeCurve.Add(tCurve, Precision::Confusion(), true))
            {
                return false;
            }

            edges.Append(edge);
        }

        Handle(Geom_BSplineCurve) newCurve = mergeCurve.BSplineCurve();
        if (newCurve.IsNull())
        {
            return false;
        }

        // 创建合并边。     
        TopoDS_Shape newShape;

        try
        {
            const TopoDS_Shape & newEdge = BRepBuilderAPI_MakeEdge(newCurve);
            if (newEdge.IsNull())
            {
                return false;
            }

            reShape.Merge(edges, newEdge);
            newShape = reShape.Apply(*part->getPartShape());
        }
        catch (...)
        {
            return false;
        } 

        // 简单检查模型是否发生变化。
        if (newShape.IsNull() || newShape.IsSame(*part->getPartShape()))
        {
            return false;
        }

        // 检查新模型边数量。
        int iEdge = 0;
        for (TopExp_Explorer iter(newShape, TopAbs_ShapeEnum::TopAbs_EDGE); iter.More(); iter.Next())
        {
            iEdge++;
        }

        if (iEdge == nAllEdges)
        {
            return false;
        }

        part->updatePartShape(newShape);
        _occShapeAgent->updateShape(part->getCopiedPartShape(), true);

        return true;
    }
}
