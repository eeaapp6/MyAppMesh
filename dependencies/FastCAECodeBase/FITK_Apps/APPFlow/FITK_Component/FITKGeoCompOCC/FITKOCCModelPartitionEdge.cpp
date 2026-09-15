/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCModelPartitionEdge.h"

// OCC base
#include <TopoDS.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Compound.hxx>
#include <TopoDS_Builder.hxx>
#include <BOPAlgo_Splitter.hxx>
#include <BRepTools_ReShape.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <gp_Pln.hxx>
#include <IntTools_EdgeFace.hxx>
#include <IntTools_SequenceOfCommonPrts.hxx>
#include <IntTools_CommonPrt.hxx>
#include <BRep_Tool.hxx>
#include <Geom_Curve.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <GeomAPI_IntCS.hxx>

// App
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

// Geometry
#include "FITKOCCVirtualTopoCreator.h"
#include "FITKOCCModelPart.h"
#include "FITKOCCGeomTools.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoEnum.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKVirtualTopoManager.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"

namespace OCC
{
    // 参数值分割边。
    //@{
    FITKOCCModelPartitionEdgeWithParameter::FITKOCCModelPartitionEdgeWithParameter() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelPartitionEdgeWithParameter::update()
    {
        FITKOCCModelPart* part = _occShapeAgent->getPart();
        //if (!part)
        //{
        //    return false;
        //}

        Interface::FITKGeoCommandList* geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (!geoCmdList)
        {
            return false;
        }

        QList<const TopoDS_Shape*> fullShapes;
        QList<const TopoDS_Edge*> topoEdges;
        QList<Interface::FITKAbsVirtualTopo*> vEdges;
        for (const Interface::VirtualShape & edge : m_Edges)
        {
            Interface::FITKAbsGeoCommand* edgeCmd = geoCmdList->getDataByID(edge.CmdId);
            if (!edgeCmd)
            {
                return false;
            }

            FITKAbstractOCCModel* occModelEdge = edgeCmd->getTShapeAgent<FITKAbstractOCCModel>();
            if (!occModelEdge)
            {
                return false;
            }         

            Interface::FITKAbsVirtualTopo* vEdge = edgeCmd->getVirtualTopoByIndex
            (Interface::FITKGeoEnum::VTopoShapeType::VSEdge, edge.VirtualTopoIndex);
            if (!vEdge)
            {
                return false;
            }
            
            FITKOCCTopoShape* edgeShape = vEdge->getShapeT<FITKOCCTopoShape>();
            if (!edgeShape)
            {
                return false;
            }

            vEdges.push_back(vEdge);

            const TopoDS_Edge & topoEdge = TopoDS::Edge(edgeShape->getTopoShape());
            topoEdges.push_back(&topoEdge);

            TopoDS_Shape* fullShape = occModelEdge->getShape();
            if (!fullShapes.contains(fullShape))
            {
                fullShapes.push_back(fullShape);
            }
        }

        if (topoEdges.isEmpty())
        {
            return false;
        }

        TopoDS_Builder builder;
        TopoDS_Compound compound;
        builder.MakeCompound(compound);  

        try
        {
            Handle(BRepTools_ReShape) reShape = new BRepTools_ReShape;
            FITKOCCEdgeTool edgeTool;

            // 分割边并替换。
            for (int i = 0; i < topoEdges.count(); i++)
            {
                const TopoDS_Edge* edge = topoEdges[i];
                Interface::FITKAbsVirtualTopo* vEdge = vEdges[i];

                // 根据参数值创建点。
                double pos3[3]{ 0., 0., 0. };
                bool flag = edgeTool.evalPoint(vEdge, m_Parameter, pos3, true);
                if (!flag)
                {
                    return false;
                }

                const TopoDS_Vertex & vert = BRepBuilderAPI_MakeVertex(gp_Pnt(pos3[0], pos3[1], pos3[2]));
                if (vert.IsNull())
                {
                    return false;
                }

                BOPAlgo_Splitter splitter;
                splitter.AddArgument(*edge);
                splitter.AddTool(vert);
                splitter.Perform();

                const TopoDS_Shape & resultEdges = splitter.Shape();
                if (resultEdges.IsNull())
                {
                    return false;
                }

                // 替换形状。
                reShape->Replace(*edge, resultEdges);
            }

            // 刷新形状。
            for (const TopoDS_Shape* fullShape : fullShapes)
            {
                // TopAbs_ShapeEnum sType = fullShape->ShapeType();
                TopoDS_Shape newShape = reShape->Apply(*fullShape);
                if (newShape.IsNull())
                {
                    continue;
                }

                builder.Add(compound, newShape);
            }
        }
        catch (...)
        {
            return false;
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

        return true;
    }
    //@}

    // 坐标分割边。
    //@{
    FITKOCCModelPartitionEdgeWithPoint::FITKOCCModelPartitionEdgeWithPoint() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelPartitionEdgeWithPoint::update()
    {
        FITKOCCModelPart* part = _occShapeAgent->getPart();
        //if (!part)
        //{
        //    return false;
        //}

        Interface::FITKGeoCommandList* geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (!geoCmdList)
        {
            return false;
        }

        QList<const TopoDS_Shape*> fullShapes;
        QList<const TopoDS_Edge*> topoEdges;
        QList<Interface::FITKAbsVirtualTopo*> vEdges;
        for (const Interface::VirtualShape & edge : m_Edges)
        {
            Interface::FITKAbsGeoCommand* edgeCmd = geoCmdList->getDataByID(edge.CmdId);
            if (!edgeCmd)
            {
                return false;
            }

            FITKAbstractOCCModel* occModelEdge = edgeCmd->getTShapeAgent<FITKAbstractOCCModel>();
            if (!occModelEdge)
            {
                return false;
            }

            Interface::FITKAbsVirtualTopo* vEdge = edgeCmd->getVirtualTopoByIndex
            (Interface::FITKGeoEnum::VTopoShapeType::VSEdge, edge.VirtualTopoIndex);
            if (!vEdge)
            {
                return false;
            }

            FITKOCCTopoShape* edgeShape = vEdge->getShapeT<FITKOCCTopoShape>();
            if (!edgeShape)
            {
                return false;
            }

            vEdges.push_back(vEdge);

            const TopoDS_Edge & topoEdge = TopoDS::Edge(edgeShape->getTopoShape());
            topoEdges.push_back(&topoEdge);

            TopoDS_Shape* fullShape = occModelEdge->getShape();
            if (!fullShapes.contains(fullShape))
            {
                fullShapes.push_back(fullShape);
            }
        }

        if (topoEdges.isEmpty())
        {
            return false;
        }

        TopoDS_Builder builder;
        TopoDS_Compound compound;
        builder.MakeCompound(compound);

        try
        {
            Handle(BRepTools_ReShape) reShape = new BRepTools_ReShape;
            FITKOCCEdgeTool edgeTool;

            // 分割边并替换。
            for (int i = 0; i < topoEdges.count(); i++)
            {
                const TopoDS_Edge* edge = topoEdges[i];
                Interface::FITKAbsVirtualTopo* vEdge = vEdges[i];

                // 根据坐标创建点。
                const TopoDS_Vertex & vert = BRepBuilderAPI_MakeVertex(gp_Pnt(m_Point[0], m_Point[1], m_Point[2]));
                if (vert.IsNull())
                {
                    return false;
                }

                BOPAlgo_Splitter splitter;
                splitter.AddArgument(*edge);
                splitter.AddTool(vert);
                splitter.Perform();

                const TopoDS_Shape & resultEdges = splitter.Shape();
                if (resultEdges.IsNull())
                {
                    return false;
                }

                // 替换形状。
                reShape->Replace(*edge, resultEdges);
            }

            // 刷新形状。
            for (const TopoDS_Shape* fullShape : fullShapes)
            {
                // TopAbs_ShapeEnum sType = fullShape->ShapeType();
                TopoDS_Shape newShape = reShape->Apply(*fullShape);
                if (newShape.IsNull())
                {
                    continue;
                }

                builder.Add(compound, newShape);
            }
        }
        catch (...)
        {
            return false;
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

        return true;
    }
    //@}

    // 基准面分割边。
    //@{
    FITKOCCModelPartitionEdgeWithDatumPlane::FITKOCCModelPartitionEdgeWithDatumPlane() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelPartitionEdgeWithDatumPlane::update()
    {
        FITKOCCModelPart* part = _occShapeAgent->getPart();
        if (!m_DatumPlane)
        {
            return false;
        }

        //if (!part)
        //{
        //    return false;
        //}

        Interface::FITKGeoCommandList* geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (!geoCmdList)
        {
            return false;
        }

        // 获取基准面信息创建平面。
        double pos3[3], nor3[3];
        m_DatumPlane->getPosition(pos3);
        m_DatumPlane->getNormal(nor3);

        gp_Pln pln(gp_Pnt(pos3[0], pos3[1], pos3[2]), gp_Dir(nor3[0], nor3[1], nor3[2]));
        const TopoDS_Face & facePlane = BRepBuilderAPI_MakeFace(pln);
        if (facePlane.IsNull())
        {
            return false;
        }

        Handle(Geom_Surface) surface = BRep_Tool::Surface(facePlane);
        if (surface.IsNull())
        {
            return false;
        }

        QList<const TopoDS_Shape*> fullShapes;
        QList<const TopoDS_Edge*> topoEdges;
        QList<Interface::FITKAbsVirtualTopo*> vEdges;
        for (const Interface::VirtualShape & edge : m_Edges)
        {
            Interface::FITKAbsGeoCommand* edgeCmd = geoCmdList->getDataByID(edge.CmdId);
            if (!edgeCmd)
            {
                return false;
            }

            FITKAbstractOCCModel* occModelEdge = edgeCmd->getTShapeAgent<FITKAbstractOCCModel>();
            if (!occModelEdge)
            {
                return false;
            }

            Interface::FITKAbsVirtualTopo* vEdge = edgeCmd->getVirtualTopoByIndex
            (Interface::FITKGeoEnum::VTopoShapeType::VSEdge, edge.VirtualTopoIndex);
            if (!vEdge)
            {
                return false;
            }

            FITKOCCTopoShape* edgeShape = vEdge->getShapeT<FITKOCCTopoShape>();
            if (!edgeShape)
            {
                return false;
            }

            vEdges.push_back(vEdge);

            const TopoDS_Edge & topoEdge = TopoDS::Edge(edgeShape->getTopoShape());
            topoEdges.push_back(&topoEdge);

            TopoDS_Shape* fullShape = occModelEdge->getShape();
            if (!fullShapes.contains(fullShape))
            {
                fullShapes.push_back(fullShape);
            }
        }

        if (topoEdges.isEmpty())
        {
            return false;
        }

        TopoDS_Builder builder;
        TopoDS_Compound compound;
        builder.MakeCompound(compound);

        try
        {
            Handle(BRepTools_ReShape) reShape = new BRepTools_ReShape;
            FITKOCCEdgeTool edgeTool;

            // 分割边并替换。
            for (int i = 0; i < topoEdges.count(); i++)
            {
                const TopoDS_Edge* edge = topoEdges[i];
                Interface::FITKAbsVirtualTopo* vEdge = vEdges[i];

                Standard_Real first, last;
                Handle(Geom_Curve) curve = BRep_Tool::Curve(*edge, first, last);
                if (curve.IsNull())
                {
                    return false;
                }

                Handle(Geom_TrimmedCurve) tCurve = new Geom_TrimmedCurve(curve, first, last);
                if (tCurve.IsNull())
                {
                    return false;
                }

                // 线面相交。
                GeomAPI_IntCS intCS(tCurve, surface);
                if (!intCS.IsDone())
                {
                    continue;
                }

                TopoDS_Shape resultEdges = *edge;
                int nPts = intCS.NbPoints();
                for (int j = 1; j <= nPts; j++)
                {
                    const gp_Pnt & pos = intCS.Point(j);

                    // 根据坐标创建点。
                    const TopoDS_Vertex & vert = BRepBuilderAPI_MakeVertex(pos);
                    if (vert.IsNull())
                    {
                        continue;
                    }

                    BOPAlgo_Splitter splitter;
                    splitter.AddArgument(resultEdges);
                    splitter.AddTool(vert);
                    splitter.Perform();

                    resultEdges = splitter.Shape();
                    if (resultEdges.IsNull())
                    {
                        return false;
                    }
                }

                //// 线面相交。
                //IntTools_EdgeFace intEdFa;
                //intEdFa.SetEdge(*edge);
                //intEdFa.SetFace(facePlane);
                //intEdFa.Perform();

                //const IntTools_SequenceOfCommonPrts & cps = intEdFa.CommonParts();
                //int nCps = cps.Size();
                //if (nCps == 0)
                //{
                //    return false;
                //}

                //TopoDS_Shape resultEdges = *edge;
                //for (int j = 0; j < nCps; j++)
                //{
                //    IntTools_CommonPrt cp = cps.Value(j);
                //    TopAbs_ShapeEnum sType = cp.Type();

                //    // 跳过平行线。
                //    if (sType != TopAbs_ShapeEnum::TopAbs_VERTEX)
                //    {
                //        continue;
                //    }

                //    gp_Pnt pos = curve->Value(cp.VertexParameter1());

                //    // 根据坐标创建点。
                //    const TopoDS_Vertex & vert = BRepBuilderAPI_MakeVertex(pos);
                //    if (vert.IsNull())
                //    {
                //        continue;
                //    }

                //    BOPAlgo_Splitter splitter;
                //    splitter.AddArgument(resultEdges);
                //    splitter.AddTool(vert);
                //    splitter.Perform();

                //    resultEdges = splitter.Shape();
                //    if (resultEdges.IsNull())
                //    {
                //        return false;
                //    }
                //}

                //IntTools_CommonPrt cp = cps.First();
                //TopAbs_ShapeEnum sType = cp.Type();

                //// 跳过平行线。
                //if (sType != TopAbs_ShapeEnum::TopAbs_VERTEX)
                //{
                //    return false;
                //}

                //gp_Pnt pos = curve->Value(cp.VertexParameter1());

                //// 根据坐标创建点。
                //const TopoDS_Vertex & vert = BRepBuilderAPI_MakeVertex(pos);
                //if (vert.IsNull())
                //{
                //    return false;
                //}

                // 替换形状。
                reShape->Replace(*edge, resultEdges);
            }

            // 刷新形状。
            for (const TopoDS_Shape* fullShape : fullShapes)
            {
                // TopAbs_ShapeEnum sType = fullShape->ShapeType();
                TopoDS_Shape newShape = reShape->Apply(*fullShape);
                if (newShape.IsNull())
                {
                    continue;
                }

                builder.Add(compound, newShape);
            }
        }
        catch (...)
        {
            return false;
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

        return true;
    }
    //@}
}
