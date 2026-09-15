/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCModelSketch2D.h"

// OCC
#include <TopoDS_Shape.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Compound.hxx>
#include <BRep_Builder.hxx>
#include <TopoDS.hxx>
#include <TopExp_Explorer.hxx>
#include <ShapeAnalysis_FreeBounds.hxx>
#include <TopTools_HSequenceOfShape.hxx>
#include <ShapeAnalysis_Wire.hxx>
#include <BRepAlgoAPI_Section.hxx>

// Data
#include "FITKOCCExtendTool.h"
#include "FITKOCCGeoSketchDeleter.h"
#include "FITKOCCVirtualTopoCreator.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeomTools.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"

#include <QDebug>

namespace OCC
{
    FITKOCCModelSketch2D::FITKOCCModelSketch2D() :OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelSketch2D::update()
    {
        if (!_occShapeAgent)
        {
            return false;
        }

        int nSubCmds = this->getDataCount();

        // 形状变化校验。
        bool hasChanged = false;
        if (nSubCmds != m_cmdIdShapeHash.count())
        {
            hasChanged = true;
        }
        else
        {
            for (int i = 0; i < nSubCmds; i++)
            {
                Interface::FITKAbsGeoCommand* subModel = this->getDataByIndex(i);
                if (!subModel)
                {
                    continue;
                }

                // 获取形状代理器。
                FITKAbstractOCCModel* occSubModel = dynamic_cast<FITKAbstractOCCModel*>(subModel->getShapeAgent());
                if (!occSubModel)
                {
                    continue;
                }

                QVariant shapeId = -1;
                occSubModel->createShapeState(shapeId);

                int shapeStateOrg = m_cmdIdShapeHash.value(subModel->getDataObjectID(), -1);
                if (/*shapeStateOrg == -1 || */shapeStateOrg != shapeId)
                {
                    hasChanged = true;
                    break;
                }
            }
        }

        if (!hasChanged)
        {
            return true;
        }

        // 初始化组合对象。
        TopoDS_Shape shape;
        bool flag = getShape(shape);
        Q_UNUSED(flag);

        m_cmdIdShapeHash.clear();

        // 更新形状数据。
        if (nSubCmds == 0)
        {
            _occShapeAgent->updateShape(TopoDS_Shape());
        }
        else
        {
            _occShapeAgent->updateShape(shape);
        }

        return true;
    }

    bool FITKOCCModelSketch2D::isClosed()
    {
        // 合并所有子数据。
        QList<TopoDS_Shape> es;
        bool flag = getShape(es);
        if (!flag)
        {
            return false;
        }

        if (es.isEmpty())
        {
            return false;
        }

        Handle(TopTools_HSequenceOfShape) edges = new TopTools_HSequenceOfShape;
        for (const TopoDS_Shape & s : es)
        {
            edges->Append(s);
        }

        try
        {
            Handle(TopTools_HSequenceOfShape) wires = new TopTools_HSequenceOfShape;
            ShapeAnalysis_FreeBounds::ConnectEdgesToWires(edges, 1e-5, false, wires);
            int nWires = wires->Size();
            if (nWires == 0)
            {
                return false;
            }

            for (int i = 1; i <= nWires; i++)
            {
                const TopoDS_Wire & wire = TopoDS::Wire(wires->Value(i));
                if (wire.IsNull() || !wire.Closed())
                {
                    return false;
                }
            }
        }
        catch (...)
        {
            return false;
        }

        return true;
    }

    bool FITKOCCModelSketch2D::isSelfIntersection()
    {
        // 合并所有子数据。
        QList<TopoDS_Shape> es;
        bool flag = getShape(es);
        if (!flag)
        {
            return false;
        }

        if (es.isEmpty())
        {
            return false;
        }

        Handle(TopTools_HSequenceOfShape) edges = new TopTools_HSequenceOfShape;
        for (const TopoDS_Shape & s : es)
        {
            edges->Append(s);
        }

        try
        {
            Handle(TopTools_HSequenceOfShape) wires = new TopTools_HSequenceOfShape;
            ShapeAnalysis_FreeBounds::ConnectEdgesToWires(edges, 1e-5, false, wires);
            int nWires = wires->Size();
            if (nWires == 0)
            {
                return false;
            }

            for (int i = 1; i <= nWires; i++)
            {
                const TopoDS_Wire & wire = TopoDS::Wire(wires->Value(i));
                if (wire.IsNull())
                {
                    return false;
                }

                // 判断自身自相交。
                //ShapeAnalysis_Wire::CheckSelfIntersection();

                // 判断线框间自相交。
                for (int j = 1; j <= nWires; j++)
                {
                    if (i == j)
                    {
                        continue;
                    }

                    const TopoDS_Wire & wire2 = TopoDS::Wire(wires->Value(j));
                    if (wire2.IsNull())
                    {
                        continue;
                    }

                    BRepAlgoAPI_Section intSec(wire, wire2);
                    intSec.Build();
                    if (!intSec.IsDone())
                    {
                        return false;
                    }

                    if (!FITKOCCExtendTool::IsEmptyShape(intSec.Shape()))
                    {
                        return true;
                    }
                }
            }
        }
        catch (...)
        {
            return false;
        }

        return false;
    }

    bool FITKOCCModelSketch2D::isConnected()
    {
        // 合并所有子数据。
        QList<TopoDS_Shape> es;
        bool flag = getShape(es);
        if (!flag)
        {
            return false;
        }

        if (es.isEmpty())
        {
            return false;
        }

        Handle(TopTools_HSequenceOfShape) edges = new TopTools_HSequenceOfShape;
        for (const TopoDS_Shape & s : es)
        {
            edges->Append(s);
        }

        try
        {
            Handle(TopTools_HSequenceOfShape) wires = new TopTools_HSequenceOfShape;
            ShapeAnalysis_FreeBounds::ConnectEdgesToWires(edges, 1e-5, false, wires);
            int nWires = wires->Size();

            // 连成多根线框则草图不连贯。
            if (nWires != 1)
            {
                return false;
            }
        }
        catch (...)
        {
            return false;
        }

        return true;
    }

    void FITKOCCModelSketch2D::checkState(bool& isClosed, bool& isSelfIntersection)
    {
        isClosed = false;
        isSelfIntersection = false;

        bool closeCheck = true;
        bool selfIntCheck = true;

        QList<TopoDS_Shape> es;
        bool flag = getShape(es);
        if (!flag)
        {
            return;
        }

        if (es.isEmpty())
        {
            return;
        }

        Handle(TopTools_HSequenceOfShape) edges = new TopTools_HSequenceOfShape;
        for (const TopoDS_Shape & s : es)
        {
            edges->Append(s);
        }

        try
        {
            Handle(TopTools_HSequenceOfShape) wires = new TopTools_HSequenceOfShape;
            ShapeAnalysis_FreeBounds::ConnectEdgesToWires(edges, 1e-5, false, wires);
            int nWires = wires->Size();
            if (nWires == 0)
            {
                return;
            }

            // 闭合判断。
            for (int i = 1; i <= nWires; i++)
            {
                const TopoDS_Wire & wire = TopoDS::Wire(wires->Value(i));
                if (wire.IsNull() || !wire.Closed())
                {
                    closeCheck = false;
                }
            }

            // 自相交判断。
            for (int i = 1; i <= nWires; i++)
            {
                const TopoDS_Wire & wire = TopoDS::Wire(wires->Value(i));
                if (wire.IsNull())
                {
                    continue;
                }

                // 判断自身自相交。
                //ShapeAnalysis_Wire::CheckSelfIntersection();

                // 判断线框间自相交。
                for (int j = 1; j <= nWires; j++)
                {
                    if (i == j)
                    {
                        continue;
                    }

                    const TopoDS_Wire & wire2 = TopoDS::Wire(wires->Value(j));
                    if (wire2.IsNull())
                    {
                        continue;
                    }

                    BRepAlgoAPI_Section intSec(wire, wire2);
                    intSec.Build();
                    if (!intSec.IsDone())
                    {
                        return;
                    }

                    if (!FITKOCCExtendTool::IsEmptyShape(intSec.Shape()))
                    {
                        selfIntCheck = true;
                        break;
                    }
                }
            }
        }
        catch (...)
        {

        }

        isClosed = closeCheck;
        isSelfIntersection = selfIntCheck;
    }

    bool FITKOCCModelSketch2D::getFirstPointAndDir(double* point, double* dir)
    {
        QList<TopoDS_Shape> edges;
        bool flag = getShape(edges);
        if (!flag)
        {
            return false;
        }

        if (edges.isEmpty())
        {
            return false;
        }

        const TopoDS_Edge & edge = TopoDS::Edge(edges.first());
        if (edge.IsNull())
        {
            return false;
        }

        Standard_Real first, last;
        Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, first, last);
        if (!curve)
        {
            return false;
        }

        gp_Pnt firstPt;
        gp_Vec firstDir;
        curve->D1(first, firstPt, firstDir);

        // 翻转。
        if (edge.Orientation() == TopAbs_Orientation::TopAbs_REVERSED)
        {
            firstDir.Reverse();
        }

        for (int i = 0; i < 3; i++)
        {
            point[i] = firstPt.Coord(i + 1);
            dir[i] = firstDir.Coord(i + 1);
        }

        return true;
    }

    bool FITKOCCModelSketch2D::getLastPointAndDir(double* point, double* dir)
    {
        QList<TopoDS_Shape> edges;
        bool flag = getShape(edges);
        if (!flag)
        {
            return false;
        }

        if (edges.isEmpty())
        {
            return false;
        }

        const TopoDS_Edge & edge = TopoDS::Edge(edges.last());
        if (edge.IsNull())
        {
            return false;
        }

        Standard_Real first, last;
        Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, first, last);
        if (!curve)
        {
            return false;
        }

        gp_Pnt lastPt;
        gp_Vec lastDir;
        curve->D1(last, lastPt, lastDir);

        // 翻转。
        if (edge.Orientation() == TopAbs_Orientation::TopAbs_REVERSED)
        {
            lastDir.Reverse();
        }

        for (int i = 0; i < 3; i++)
        {
            point[i] = lastPt.Coord(i + 1);
            dir[i] = lastDir.Coord(i + 1);
        }

        return true;
    }

    void FITKOCCModelSketch2D::appendDataObj(Core::FITKAbstractDataObject* obj)
    {
        Interface::FITKAbsGeoCommand* subCmd = dynamic_cast<Interface::FITKAbsGeoCommand*>(obj);
        if (!subCmd)
        {
            return;
        }

        // 类型转换，获取子数据形状代理器。
        FITKAbstractOCCModel* shapeAgentNew = subCmd->getTShapeAgent<FITKAbstractOCCModel>();
        if (!shapeAgentNew)
        {
            return;
        }

        // 追加形状。
        Interface::FITKAbsGeoSketch2D::appendDataObj(subCmd);

        // 设置草图。
        shapeAgentNew->setSketch(this);

        // 刷新模型。
        update();
    }

    bool FITKOCCModelSketch2D::getShape(QList<TopoDS_Shape> & oEdges)
    {
        oEdges.clear();

        // 模型Id，拓扑索引字典，用于存储移除后的可用模型形状。
        QHash<int, QHash<int, const TopoDS_Shape*>> cmdIdTopoIndexHash;
        for (int i = 0; i < this->getDataCount(); i++)
        {
            Interface::FITKAbsGeoCommand* subModel = this->getDataByIndex(i);
            if (!subModel)
            {
                continue;
            }

            int subCmdId = subModel->getDataObjectID();

            FITKOCCGeoSketchDeleter* deleter = dynamic_cast<FITKOCCGeoSketchDeleter*>(subModel);
            if (deleter)
            {
                //// 移除字典中的边。
                //QList<Interface::VirtualShape> ents = deleter->toBeRemovedEntities();
                //for (const Interface::VirtualShape & entity : ents)
                //{
                //    cmdIdTopoIndexHash[entity.CmdId].remove(entity.VirtualTopoIndex);
                //}
            }
            else
            {
                // 获取OCC边。
                Interface::FITKShapeVirtualTopoManager* vEdgeMgr = subModel->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VSEdge);
                if (!vEdgeMgr)
                {
                    continue;
                }

                int nEdges = vEdgeMgr->getDataCount();
                for (int j = 0; j < nEdges; j++)
                {
                    Interface::FITKAbsVirtualTopo* vEdge = vEdgeMgr->getDataByIndex(j);
                    if (!vEdge)
                    {
                        continue;
                    }

                    FITKOCCTopoShape* occShape = vEdge->getShapeT<FITKOCCTopoShape>();
                    if (!occShape)
                    {
                        continue;
                    }

                    const TopoDS_Shape & edge = occShape->getTopoShape();
                    if (edge.IsNull())
                    {
                        continue;
                    }

                    oEdges.push_back(edge);
                }
            }
        }

        //// 模型Id，拓扑索引字典，用于存储移除后的可用模型形状。
        //QHash<int, QHash<int, const TopoDS_Shape*>> cmdIdTopoIndexHash;
        //for (int i = 0; i < this->getDataCount(); i++)
        //{
        //    Interface::FITKAbsGeoCommand* subModel = this->getDataByIndex(i);
        //    if (!subModel)
        //    {
        //        continue;
        //    }

        //    int subCmdId = subModel->getDataObjectID();

        //    FITKOCCGeoSketchDeleter* deleter = dynamic_cast<FITKOCCGeoSketchDeleter*>(subModel);
        //    if (deleter)
        //    {
        //        //// 移除字典中的边。
        //        //QList<Interface::VirtualShape> ents = deleter->toBeRemovedEntities();
        //        //for (const Interface::VirtualShape & entity : ents)
        //        //{
        //        //    cmdIdTopoIndexHash[entity.CmdId].remove(entity.VirtualTopoIndex);
        //        //}
        //    }
        //    else
        //    {
        //        // 获取OCC边。
        //        Interface::FITKShapeVirtualTopoManager* vEdgeMgr = subModel->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VSEdge);
        //        if (!vEdgeMgr)
        //        {
        //            continue;
        //        }

        //        int nEdges = vEdgeMgr->getDataCount();
        //        for (int j = 0; j < nEdges; j++)
        //        {
        //            Interface::FITKAbsVirtualTopo* vEdge = vEdgeMgr->getDataByIndex(j);
        //            if (!vEdge)
        //            {
        //                continue;
        //            }

        //            FITKOCCTopoShape* occShape = vEdge->getShapeT<FITKOCCTopoShape>();
        //            if (!occShape)
        //            {
        //                continue;
        //            }

        //            const TopoDS_Shape & edge = occShape->getTopoShape();
        //            if (edge.IsNull())
        //            {
        //                continue;
        //            }

        //            cmdIdTopoIndexHash[subCmdId][j] = &edge;
        //        }
        //    }
        //}

        //QHashIterator<int, QHash<int, const TopoDS_Shape*>> iterModel(cmdIdTopoIndexHash);
        //while (iterModel.hasNext())
        //{
        //    iterModel.next();

        //    QHashIterator<int, const TopoDS_Shape*> iterEdge(iterModel.value());
        //    while (iterEdge.hasNext())
        //    {
        //        iterEdge.next();

        //        oEdges.push_back(*iterEdge.value());
        //    }
        //}

        return true;
    }

    bool FITKOCCModelSketch2D::getShape(TopoDS_Shape & oShape)
    {
        oShape = TopoDS_Shape();

        QList<TopoDS_Shape> es;
        bool flag = getShape(es);
        if (!flag)
        {
            return false;
        }

        TopoDS_Builder builder;
        TopoDS_Compound edges;
        builder.MakeCompound(edges);

        for (const TopoDS_Shape & s : es)
        {
            builder.Add(edges, s);
        }

        oShape = edges;

        return true;
    }
}
