/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCModelSurface.h"
#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Wire.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepFill_Filling.hxx>
#include <BRepPrimAPI_MakeRevol.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <BRepOffset_MakeOffset.hxx>
#include <BRepOffsetAPI_MakePipe.hxx>
#include <GeomAPI_PointsToBSplineSurface.hxx>
#include <Geom_BSplineSurface.hxx>
#include <GeomLib_Tool.hxx>
#include <Geom_BezierSurface.hxx>
#include <TopExp_Explorer.hxx>
#include <GeomFill_SimpleBound.hxx>
#include <GeomFill_ConstrainedFilling.hxx>
#include <Geom_BSplineSurface.hxx>
#include <TopoDS_Iterator.hxx>
#include <TopExp_Explorer.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <Precision.hxx>
#include <BRepBndLib.hxx>
#include <Geom_BSplineCurve.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepOffsetAPI_ThruSections.hxx>
#include <BRepBuilderAPI_Copy.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepOffsetAPI_MakePipeShell.hxx>
#include <BRepAlgoAPI_Section.hxx>
#include <BOPAlgo_Splitter.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <BRepExtrema_DistShapeShape.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>

#include <FITK_Interface/FITKInterfaceGeometry/FITKGeoEnum.h>
#include <FITK_Interface/FITKInterfaceGeometry/FITKVirtualTopoManager.h>
#include "FITKOCCVirtualTopoCreator.h"
#include "FITKOCCModelPart.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelSurface.h"

#include <FITKOCCModelCurve.h>
#include "FITKOCCComandCommon.h"
#include "FITKOCCExtendTool.h"

#include <QDebug>

namespace OCC
{

    FITKOCCModelClosedWireSurface::FITKOCCModelClosedWireSurface()
        : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelClosedWireSurface::update()
    {
        if (m_Edges.isEmpty()) return false;

        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        FITKOCCModelPart* part = _occShapeAgent->getPart();

        BRepBuilderAPI_MakeWire wire;
        BRepFill_Filling fillFace;
        for (int i = 0; i < m_Edges.size(); ++i) {
            auto edge = m_Edges.at(i);
            if (edge.isNull()) continue;

            auto cmd = geoCmdList->getDataByID(edge.CmdId);
            if (cmd == nullptr) return false;
            Interface::FITKGeoEnum::VTopoShapeType type = cmd->getGeometryCommandType() == Interface::FITKGeoEnum::FITKGeometryComType::FGTSketch2D ? Interface::FITKGeoEnum::VTopoShapeType::VSAssembly : Interface::FITKGeoEnum::VTopoShapeType::VSEdge;
            auto vshape = cmd->getShapeT<FITKOCCTopoShape>(type, edge.VirtualTopoId);
            if (vshape == nullptr) return false;
            auto topoShape = vshape->getTopoShape();
            if (topoShape.ShapeType() == TopAbs_ShapeEnum::TopAbs_EDGE) {
                const TopoDS_Edge & e = TopoDS::Edge(topoShape);
                if (e.IsNull()) continue;
                fillFace.Add(e, GeomAbs_Shape::GeomAbs_C0, true);
                wire.Add(e);
            }
            else if (topoShape.ShapeType() == TopAbs_ShapeEnum::TopAbs_COMPOUND) {
                TopExp_Explorer exp;
                for (exp.Init(topoShape, TopAbs_EDGE); exp.More(); exp.Next())
                {
                    const TopoDS_Edge & edge = TopoDS::Edge(exp.Current());
                    if (edge.IsNull()) continue;
                    fillFace.Add(edge, GeomAbs_Shape::GeomAbs_C0, true);
                    wire.Add(edge);
                }

            }
            else {
                continue;
            }

        }

        /*@{*/
        // 封闭性检查
        const TopoDS_Wire & topowire = TopoDS::Wire(wire.Shape());
        if (topowire.IsNull() || !topowire.Closed()) {
            printLog(tr("Must be closed wire."), 3);
            return false;
        }
        /*@}*/

        try
        {
            fillFace.Build();
            if (!fillFace.IsDone())
            {
                printLog(tr("Failed to make face!"), 3);
                return false;
            }

            const TopoDS_Face & face = fillFace.Face();
            if (face.IsNull())
            {
                printLog(tr("Failed to make face!"), 3);
                return false;
            }

            if (part)
            {
                part->updatePartShape(face, true);
                _occShapeAgent->updateShape(part->getCopiedPartShape());
            }
            else
            {
                _occShapeAgent->updateShape(face);
            }
        }
        catch (...)
        {
            printLog(tr("Failed to make face!"), 3);
            return false;
        }

        return true;

        /*if (m_Edges.isEmpty()) return false;
        auto aConstrainedFilling = new GeomFill_ConstrainedFilling(5, 2);

        auto edge = m_Edges.at(0);
        if (edge.isNull()) return nullptr;
        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        auto cmd = geoCmdList->getDataByID(edge.CmdId);
        if (cmd == nullptr) return nullptr;
        auto vshape = cmd->getShapeT<FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSEdge, edge.VirtualTopoId);
        if (vshape == nullptr) return nullptr;
        auto topoShape = vshape->getTopoShape();
        TopoDS_Edge e = TopoDS::Edge(topoShape);
        if (e.IsNull()) return nullptr;

        Standard_Real first, last;
        Handle(Geom_Curve) curve = BRep_Tool::Curve(e, first, last);

        Handle(GeomAdaptor_HCurve) SPL1Adaptor = new GeomAdaptor_HCurve(curve);
        Handle(GeomFill_SimpleBound) B1 = new GeomFill_SimpleBound(SPL1Adaptor, Precision::Approximation(), Precision::Angular());


        edge = m_Edges.at(1);
        if (edge.isNull()) return nullptr;
        cmd = geoCmdList->getDataByID(edge.CmdId);
        if (cmd == nullptr) return nullptr;
        vshape = cmd->getShapeT<FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSEdge, edge.VirtualTopoId);
        if (vshape == nullptr) return nullptr;
        topoShape = vshape->getTopoShape();
        e = TopoDS::Edge(topoShape);
        if (e.IsNull()) return nullptr;
        Handle(Geom_Curve) curve1 = BRep_Tool::Curve(e, first, last);

        Handle(GeomAdaptor_HCurve) SPL1Adaptor1 = new GeomAdaptor_HCurve(curve1);
        Handle(GeomFill_SimpleBound) B2 = new GeomFill_SimpleBound(SPL1Adaptor1, Precision::Approximation(), Precision::Angular());


        edge = m_Edges.at(2);
        if (edge.isNull()) return nullptr;
        cmd = geoCmdList->getDataByID(edge.CmdId);
        if (cmd == nullptr) return nullptr;
        vshape = cmd->getShapeT<FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSEdge, edge.VirtualTopoId);
        if (vshape == nullptr) return nullptr;
        topoShape = vshape->getTopoShape();
        e = TopoDS::Edge(topoShape);
        if (e.IsNull()) return nullptr;
        Handle(Geom_Curve) curve2 = BRep_Tool::Curve(e, first, last);

        Handle(GeomAdaptor_HCurve) SPL1Adaptor2 = new GeomAdaptor_HCurve(curve2);
        Handle(GeomFill_SimpleBound) B3 = new GeomFill_SimpleBound(SPL1Adaptor2, Precision::Approximation(), Precision::Angular());


        edge = m_Edges.at(3);
        if (edge.isNull()) return nullptr;
        cmd = geoCmdList->getDataByID(edge.CmdId);
        if (cmd == nullptr) return nullptr;
        vshape = cmd->getShapeT<FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSEdge, edge.VirtualTopoId);
        if (vshape == nullptr) return nullptr;
        topoShape = vshape->getTopoShape();
        e = TopoDS::Edge(topoShape);
        if (e.IsNull()) return nullptr;
        Handle(Geom_Curve) curve3 = BRep_Tool::Curve(e, first, last);

        Handle(GeomAdaptor_HCurve) SPL1Adaptor3 = new GeomAdaptor_HCurve(curve3);
        Handle(GeomFill_SimpleBound) B4 = new GeomFill_SimpleBound(SPL1Adaptor3, Precision::Approximation(), Precision::Angular());

        aConstrainedFilling->Init(B1, B2, B3, B4, false);


        Handle(Geom_BSplineSurface) aBSplineSurface = aConstrainedFilling->Surface();

        BRepBuilderAPI_MakeFace face(aBSplineSurface, Precision::Confusion());
        if (!face.IsDone()) {
            printLog(tr("Failed to make shape."), 3);
            return false;
        }
        _occShapeAgent->updateShape(face.Shape());

        return true;*/



        /*if (m_Edges.isEmpty()) return false;

        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        BRepBuilderAPI_MakeWire wire;
        for (int i = 0; i < m_Edges.size(); ++i) {
            auto edge = m_Edges.at(i);
            if (edge.isNull()) continue;

            auto cmd = geoCmdList->getDataByID(edge.CmdId);
            if (cmd == nullptr) return false;
            auto vshape = cmd->getShapeT<FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSEdge, edge.VirtualTopoId);
            if (vshape == nullptr) return false;
            auto topoShape = vshape->getTopoShape();
            if (topoShape.ShapeType() == TopAbs_ShapeEnum::TopAbs_EDGE) {
                TopoDS_Edge e = TopoDS::Edge(topoShape);
                if (e.IsNull()) continue;
                wire.Add(e);
            }
            else if (topoShape.ShapeType() == TopAbs_ShapeEnum::TopAbs_WIRE) {
                TopoDS_Wire w = TopoDS::Wire(topoShape);
                if (w.IsNull()) continue;
                wire.Add(w);
            }
            else {
                printLog(tr("Error topo shape type for make wire."), 3);
                return false;
            }
        }
        wire.Build();
        if (!wire.IsDone()) {
            switch (wire.Error())
            {
            case BRepBuilderAPI_EmptyWire:
                printLog(tr("Empty Wire."), 3);
                break;
            case BRepBuilderAPI_DisconnectedWire:
                printLog(tr("Disconnected Wire."), 3);
                break;
            case  BRepBuilderAPI_NonManifoldWire:
                printLog(tr("NonManifold Wire."), 3);
                break;
            case  BRepBuilderAPI_WireDone:
                printLog(tr("Wire Done."), 3);
                break;
            default:
                break;
            }
        }
        TopoDS_Wire topowire = TopoDS::Wire(wire.Shape());
        if (topowire.IsNull() || !topowire.Closed()) {
            printLog(tr("Must be closed wire."), 3);
            return false;
        }
        BRepBuilderAPI_MakeFace face(wire);
        if (!face.IsDone()) {
            printLog(tr("Failed to make shape."), 3);
            return false;
        }
        _occShapeAgent->updateShape(face.Shape());

        return true;*/
    }

    FITKOCCModelCoverEdges::FITKOCCModelCoverEdges() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelCoverEdges::update()
    {
        if (m_Edges.isEmpty()) return false;

        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        FITKOCCModelPart* part = _occShapeAgent->getPart();

        //BRepBuilderAPI_MakeWire wire;
        //BRepFill_Filling fillFace;
        //for (int i = 0; i < m_Edges.size(); ++i) {
        //    auto edge = m_Edges.at(i);
        //    if (edge.isNull()) continue;

        //    auto cmd = geoCmdList->getDataByID(edge.CmdId);
        //    if (cmd == nullptr) return false;
        //    Interface::FITKGeoEnum::VTopoShapeType type = cmd->getGeometryCommandType() == Interface::FITKGeoEnum::FITKGeometryComType::FGTSketch2D ? Interface::FITKGeoEnum::VTopoShapeType::VSAssembly : Interface::FITKGeoEnum::VTopoShapeType::VSEdge;
        //    auto vshape = cmd->getShapeT<FITKOCCTopoShape>(type, edge.VirtualTopoId);
        //    if (vshape == nullptr) return false;
        //    auto topoShape = vshape->getTopoShape();
        //    if (topoShape.ShapeType() == TopAbs_ShapeEnum::TopAbs_EDGE) {
        //        const TopoDS_Edge & e = TopoDS::Edge(topoShape);
        //        if (e.IsNull()) continue;
        //        fillFace.Add(e, GeomAbs_Shape::GeomAbs_C0, true);
        //        wire.Add(e);
        //    }
        //    else if (topoShape.ShapeType() == TopAbs_ShapeEnum::TopAbs_COMPOUND) {
        //        TopExp_Explorer exp;
        //        for (exp.Init(topoShape, TopAbs_EDGE); exp.More(); exp.Next())
        //        {
        //            const TopoDS_Edge & edge = TopoDS::Edge(exp.Current());
        //            if (edge.IsNull()) continue;
        //            fillFace.Add(edge, GeomAbs_Shape::GeomAbs_C0, true);
        //            wire.Add(edge);
        //        }

        //    }
        //    else {
        //        continue;
        //    }

        //}

        BRep_Builder builder;
        TopoDS_Compound edges;
        builder.MakeCompound(edges);

        for (int i = 0; i < m_Edges.size(); ++i) 
        {
            auto edge = m_Edges.at(i);
            if (edge.isNull()) continue;

            auto cmd = geoCmdList->getDataByID(edge.CmdId);
            if (cmd == nullptr) return false;
            Interface::FITKGeoEnum::VTopoShapeType type = cmd->getGeometryCommandType() == Interface::FITKGeoEnum::FITKGeometryComType::FGTSketch2D ? Interface::FITKGeoEnum::VTopoShapeType::VSAssembly : Interface::FITKGeoEnum::VTopoShapeType::VSEdge;
            auto vshape = cmd->getShapeT<FITKOCCTopoShape>(type, edge.VirtualTopoId);
            if (vshape == nullptr) return false;
            auto topoShape = vshape->getTopoShape();

            builder.Add(edges, topoShape);
        }

        QList<TopoDS_Wire> wires;
        FITKOCCComandCommon::MakeWires(edges, wires, 1e-5);
        if (wires.count() != 1)
        {
            return false;
        }

        TopoDS_Wire topowire = wires.first();

        /*@{*/
        // 封闭性检查
        // const TopoDS_Wire & topowire = TopoDS::Wire(wire.Shape());
        if (topowire.IsNull() || !topowire.Closed())
        {
            printLog(tr("Must be closed wire."), 3);
            return false;
        }
        /*@}*/

        BRepFill_Filling fillFace;
        for (TopExp_Explorer iter(topowire, TopAbs_ShapeEnum::TopAbs_EDGE); iter.More(); iter.Next())
        {
            fillFace.Add(TopoDS::Edge(iter.Value()), GeomAbs_Shape::GeomAbs_C0, true);
        }

        try
        {
            fillFace.Build();
            if (!fillFace.IsDone())
            {
                printLog(tr("Failed to make face!"), 3);
                return false;
            }

            TopoDS_Shape face = fillFace.Face();
            if (face.IsNull())
            {
                printLog(tr("Failed to make face!"), 3);
                return false;
            }

            if (part)
            {
                TopoDS_Shape* partShape{ nullptr };
                FITKAbstractOCCModel* partAgent = part->getTShapeAgent<FITKAbstractOCCModel>();
                if (!partAgent)
                {
                    return false;
                }

                partShape = partAgent->getShape();

                QList<TopoDS_Shape> partSubShapes, partSubShapes2D, partSubShapes3D;
                FITKOCCComandCommon::FlattenCompShape2D3D(*partShape, partSubShapes, partSubShapes2D, partSubShapes3D);

                // 合并布尔结果。
                BRep_Builder builder;
                TopoDS_Compound compound, compoundResult, compoundInter;
                builder.MakeCompound(compound);
                builder.MakeCompound(compoundResult);
                builder.MakeCompound(compoundInter);

                int nShells = 0;
                for (const TopoDS_Shape & partSubShape : partSubShapes2D)
                {
                    builder.Add(compound, partSubShape);
                    nShells++;
                }

                for (const TopoDS_Shape & partSubShape : partSubShapes3D)
                {
                    BRepAlgoAPI_Section intSecLine(face, partSubShape);
                    intSecLine.Build();
                    if (!intSecLine.IsDone())
                    {
                        return false;
                    }

                    BRepAlgoAPI_Common intSec(face, partSubShape);
                    intSec.Build();
                    if (!intSec.IsDone())
                    {
                        return false;
                    }

                    const TopoDS_Shape & sec = intSec.Shape();
                    if (FITKOCCExtendTool::IsEmptyShape(intSecLine))
                    {
                        builder.Add(compoundResult, partSubShape);
                    }
                    else
                    {
                        BOPAlgo_Splitter splitter;
                        splitter.AddArgument(partSubShape);
                        splitter.AddTool(sec);
                        splitter.Perform();

                        const TopoDS_Shape & newShape = splitter.Shape();
                        if (newShape.IsNull())
                        {
                            return false;
                        }

                        builder.Add(compoundResult, newShape);
                        builder.Add(compoundInter, sec);
                    }
                }

                for (const TopoDS_Shape & partSubShape : partSubShapes)
                {
                    // 处理一维模型与壳面相交。
                    if (partSubShape.ShapeType() == TopAbs_ShapeEnum::TopAbs_VERTEX)
                    {
                        builder.Add(compoundResult, partSubShape);
                    }
                    else
                    {
                        BRepAlgoAPI_Section intSecLine(face, partSubShape);
                        intSecLine.Build();
                        if (!intSecLine.IsDone())
                        {
                            return false;
                        }

                        BRepAlgoAPI_Common intSec(face, partSubShape);
                        intSec.Build();
                        if (!intSec.IsDone())
                        {
                            return false;
                        }

                        const TopoDS_Shape & sec = intSec.Shape();
                        if (FITKOCCExtendTool::IsEmptyShape(intSecLine))
                        {
                            builder.Add(compoundResult, partSubShape);
                        }
                        else
                        {
                            BRepAlgoAPI_Cut edgeCutter(partSubShape, sec);
                            if (!edgeCutter.IsDone())
                            {
                                return false;
                            }

                            const TopoDS_Shape & newEdge = edgeCutter.Shape();
                            if (newEdge.IsNull())
                            {
                                return false;
                            }

                            BOPAlgo_Splitter splitter;
                            splitter.AddArgument(partSubShape);
                            splitter.AddTool(newEdge);
                            splitter.Perform();

                            const TopoDS_Shape & newShape = splitter.Shape();
                            if (newShape.IsNull())
                            {
                                return false;
                            }

                            builder.Add(compoundResult, newEdge);
                        }
                    }
                }

                try
                {
                    // 分割模型。
                    if (!FITKOCCExtendTool::IsEmptyShape(compoundInter))
                    {
                        BOPAlgo_Splitter splitter;
                        splitter.AddArgument(face);
                        splitter.AddTool(compoundInter);
                        splitter.Perform();

                        const TopoDS_Shape & newShape = splitter.Shape();
                        if (!newShape.IsNull())
                        {
                            face = newShape;
                        }
                    }

                    // 源模型无二维形状则直接进行拼接组合对象。
                    if (nShells == 0)
                    {
                        builder.Add(compoundResult, face);
                    }
                    else
                    {
                        BRepAlgoAPI_Fuse add = BRepAlgoAPI_Fuse(compound, face);
                        if (!add.IsDone())
                        {
                            return false;
                        }

                        const TopoDS_Shape & shellsShape = add.Shape();
                        builder.Add(compoundResult, shellsShape);
                    }
                }
                catch (...)
                {
                    return false;
                }

                part->updatePartShape(compoundResult, false);
                _occShapeAgent->updateShape(part->getCopiedPartShape());
            }
            else
            {
                _occShapeAgent->updateShape(face);
            }
        }
        catch (...)
        {
            printLog(tr("Failed to make face!"), 3);
            return false;
        }

        return true;
    }

    FITKOCCModelOffsetSurface::FITKOCCModelOffsetSurface() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelOffsetSurface::update()
    {
        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        auto cmd = geoCmdList->getDataByID(m_SourceSurface.CmdId);
        if (cmd == nullptr) return false;
        auto vshape = cmd->getShapeT<FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSFace, m_SourceSurface.VirtualTopoId);
        if (vshape == nullptr) return false;
        auto topoShape = vshape->getTopoShape();

        try
        {
            BRepOffset_MakeOffset offsetSurface(topoShape, m_Offset, 1e-6);
            if (!offsetSurface.IsDone()) return false;

            _occShapeAgent->updateShape(offsetSurface.Shape());
        }
        catch (...)
        {
            printLog(tr("Failed to make face!"), 3);
            return false;
        }
        return true;
    }
    FITKOCCModelExtrudeSurface::FITKOCCModelExtrudeSurface() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }
    bool FITKOCCModelExtrudeSurface::update()
    {

        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        auto cmd = geoCmdList->getDataByID(m_SourceCurve.CmdId);
        if (cmd == nullptr) return false;

        //m_OriginId;
        gp_Vec dir(m_Direction[0], m_Direction[1], m_Direction[2]);
        auto mag = dir.Magnitude();
        if (mag <= Precision::Confusion()) {
            printLog(tr("The direction cannot be a zero vector!"), 3);
            return false;
        }

        TopoDS_Shape topoShape;

        try
        {
            if (m_ExtrudeType == 1)
            {
                dir = dir * m_Length / mag;

                // 草图。
                if (cmd->getGeometryCommandType() == Interface::FITKGeoEnum::FITKGeometryComType::FGTSketch2D)
                {
                    // 合并草图。
                    cmd->update();

                    FITKAbstractOCCModel* occModel = cmd->getTShapeAgent<FITKAbstractOCCModel>();
                    if (!occModel)
                    {
                        return false;
                    }

                    TopoDS_Shape* sketchShape = occModel->getShape();
                    if (sketchShape->IsNull())
                    {
                        return false;
                    }

                    QList<TopoDS_Wire> wires;
                    FITKOCCComandCommon::MakeWires(*sketchShape, wires, 1e-5);

                    TopoDS_Builder builder;
                    TopoDS_Compound compound;
                    builder.MakeCompound(compound);

                    for (const TopoDS_Wire & wire : wires)
                    {
                        if (wire.IsNull())
                        {
                            continue;
                        }

                        const TopoDS_Shape & shape = BRepPrimAPI_MakePrism(wire, dir);
                        builder.Add(compound, shape);
                    }

                    topoShape = compound;
                }
                // 几何边。
                else
                {
                    auto shape = cmd->getShapeT<FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSEdge, m_SourceCurve.VirtualTopoId);
                    if (shape == nullptr) return false;
                    topoShape = BRepPrimAPI_MakePrism(shape->getTopoShape(), dir);
                }
            }
            else if (m_ExtrudeType == 2)
            {
                // 获取目标面。
                if (m_UpToFace.isNull())
                {
                    return false;
                }

                Interface::FITKAbsGeoCommand* faceCmd = geoCmdList->getDataByID(m_UpToFace.CmdId);
                if (!faceCmd)
                {
                    return false;
                }

                FITKOCCTopoShape* occShape = faceCmd->getShapeT<FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSFace, m_UpToFace.VirtualTopoId);
                if (!occShape)
                {
                    printLog(tr("Failed to get face from virtual topo."), 3);
                    return false;
                }

                const TopoDS_Face & upToFace = TopoDS::Face(occShape->getTopoShape());
                if (upToFace.IsNull())
                {
                    return false;
                }

                Handle(Geom_Surface) upToSurf = BRep_Tool::Surface(upToFace);
                if (upToSurf.IsNull())
                {
                    return false;
                }

                // 计算目标面与原始草图总包围盒，计算拉伸长度。
                Bnd_Box box;
                BRepBndLib::Add(upToFace, box, false);

                // 草图。
                if (cmd->getGeometryCommandType() == Interface::FITKGeoEnum::FITKGeometryComType::FGTSketch2D)
                {
                    // 合并草图。
                    cmd->update();

                    FITKAbstractOCCModel* occModel = cmd->getTShapeAgent<FITKAbstractOCCModel>();
                    if (!occModel)
                    {
                        return false;
                    }

                    TopoDS_Shape* sketchShape = occModel->getShape();
                    if (sketchShape->IsNull())
                    {
                        return false;
                    }

                    QList<TopoDS_Wire> wires;
                    FITKOCCComandCommon::MakeWires(*sketchShape, wires, 1e-5);

                    for (const TopoDS_Wire & wire : wires)
                    {
                        BRepBndLib::Add(wire, box, false);
                    }

                    // 使用包围盒对角线长度1.1倍距离进行拉伸。
                    double extrudeDist = sqrt(box.SquareExtent()) * 1.1;
                    dir = dir * extrudeDist / mag;

                    TopoDS_Builder builder;
                    TopoDS_Compound compound;
                    builder.MakeCompound(compound);

                    for (const TopoDS_Wire & wire : wires)
                    {
                        if (wire.IsNull())
                        {
                            continue;
                        }

                        const TopoDS_Shape & shape = BRepPrimAPI_MakePrism(wire, dir);

                        // 获取面面交线。
                        BRepAlgoAPI_Section intSec(shape, upToFace);
                        intSec.Build();
                        if (!intSec.IsDone())
                        {
                            return false;
                        }

                        const TopoDS_Shape & sec = intSec.Shape();
                        if (sec.IsNull())
                        {
                            return false;
                        }

                        // 重组交线。
                        QList<TopoDS_Wire> wiresSec;
                        FITKOCCComandCommon::MakeWires(sec, wiresSec, 1e-5);

                        for (const TopoDS_Wire & wireSec : wiresSec)
                        {
                            if (wireSec.IsNull())
                            {
                                continue;
                            }

                            // 反向拉伸目标面交线并求交集。
                            const TopoDS_Shape & reverseShape = BRepPrimAPI_MakePrism(wireSec, -dir);
                            builder.Add(compound, BRepAlgoAPI_Common(shape, reverseShape));
                        }
                    }

                    topoShape = compound;
                }
                // 几何边。
                else
                {
                    auto shape = cmd->getShapeT<FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSEdge, m_SourceCurve.VirtualTopoId);
                    if (shape == nullptr) return false;
                    const TopoDS_Shape & edgeShape = shape->getTopoShape();

                    BRepBndLib::Add(edgeShape, box, false);

                    // 使用包围盒对角线长度1.1倍距离进行拉伸。
                    double extrudeDist = sqrt(box.SquareExtent()) * 1.1;
                    dir = dir * extrudeDist / mag;

                    const TopoDS_Shape & extrudeShape = BRepPrimAPI_MakePrism(edgeShape, dir);

                    // 获取面面交线。
                    BRepAlgoAPI_Section intSec(extrudeShape, upToFace);
                    intSec.Build();
                    if (!intSec.IsDone())
                    {
                        return false;
                    }

                    const TopoDS_Shape & sec = intSec.Shape();
                    if (sec.IsNull())
                    {
                        return false;
                    }

                    // 重组交线。
                    QList<TopoDS_Wire> wiresSec;
                    FITKOCCComandCommon::MakeWires(sec, wiresSec, 1e-5);

                    TopoDS_Builder builder;
                    TopoDS_Compound compound;
                    builder.MakeCompound(compound);

                    for (const TopoDS_Wire & wire : wiresSec)
                    {
                        if (wire.IsNull())
                        {
                            continue;
                        }

                        // 反向拉伸目标面交线并求交集。
                        const TopoDS_Shape & reverseShape = BRepPrimAPI_MakePrism(wire, -dir);
                        builder.Add(compound, BRepAlgoAPI_Common(extrudeShape, reverseShape));
                    }

                    topoShape = compound;
                }
            }
        }
        catch (...)
        {
            return false;
        }
 
        if (topoShape.IsNull())
        {
            return false;
        }

        FITKOCCModelPart* part = _occShapeAgent->getPart();

        if (part && part->getAutoBool())
        {
            TopoDS_Shape* partShape{ nullptr };
            FITKAbstractOCCModel* partAgent = part->getTShapeAgent<FITKAbstractOCCModel>();
            if (!partAgent)
            {
                return false;
            }

            partShape = partAgent->getShape();

            QList<TopoDS_Shape> partSubShapes, partSubShapes2D, partSubShapes3D;
            FITKOCCComandCommon::FlattenCompShape2D3D(*partShape, partSubShapes, partSubShapes2D, partSubShapes3D);

            // 合并布尔结果。
            BRep_Builder builder;
            TopoDS_Compound compound, compoundResult, compoundInter, compoundInterEdge;
            builder.MakeCompound(compound);
            builder.MakeCompound(compoundResult);
            builder.MakeCompound(compoundInter);
            builder.MakeCompound(compoundInterEdge);

            int nShells = 0;
            for (const TopoDS_Shape & partSubShape : partSubShapes2D)
            {
                builder.Add(compound, partSubShape);
                nShells++;
            }

            for (const TopoDS_Shape & partSubShape : partSubShapes)
            {
                // 处理一维模型与壳面相交的两种情况：存在交点、存在交线。
                if (partSubShape.ShapeType() == TopAbs_ShapeEnum::TopAbs_VERTEX)
                {
                    builder.Add(compoundResult, partSubShape);
                }
                else
                {
                    BRepExtrema_DistShapeShape distSS;
                    distSS.LoadS1(partSubShape);
                    distSS.LoadS2(topoShape);
                    distSS.Perform();

                    if (distSS.IsDone() && distSS.NbSolution() > 0)
                    {
                        TopoDS_Compound compoundVerts;
                        builder.MakeCompound(compoundVerts);

                        for (int i = 1; i <= distSS.NbSolution(); i++)
                        {
                            const gp_Pnt & cPt = distSS.PointOnShape1(i);

                            const TopoDS_Vertex & vert = BRepBuilderAPI_MakeVertex(cPt);
                            builder.Add(compoundVerts, vert);
                        }           

                        TopoDS_Shape newEdge;

                        BRepAlgoAPI_Section intSecLine(topoShape, partSubShape);
                        intSecLine.Build();
                        if (!intSecLine.IsDone())
                        {
                            return false;
                        }

                        const TopoDS_Shape & sec = intSecLine.Shape();

                        BRepAlgoAPI_Cut edgeCutter(partSubShape, sec);
                        if (edgeCutter.IsDone())
                        {
                            newEdge = edgeCutter.Shape();
                        }
                        else
                        {
                            BOPAlgo_Splitter splitter;
                            splitter.AddArgument(partSubShape);
                            splitter.AddTool(compoundVerts);
                            splitter.Perform();

                            newEdge = splitter.Shape();
                        }
   
                        if (newEdge.IsNull())
                        {
                            return false;
                        }

                        builder.Add(compoundResult, newEdge);

                        builder.Add(compoundInterEdge, compoundVerts);
                    }
                    else
                    {
                        builder.Add(compoundResult, partSubShape);
                    }
                }
            }

            for (const TopoDS_Shape & partSubShape : partSubShapes3D)
            {
                BRepAlgoAPI_Section intSecLine(topoShape, partSubShape);
                intSecLine.Build();
                if (!intSecLine.IsDone())
                {
                    return false;
                }

                BRepAlgoAPI_Common intSec(topoShape, partSubShape);
                intSec.Build();
                if (!intSec.IsDone())
                {
                    return false;
                }

                const TopoDS_Shape & sec = intSec.Shape();
                if (FITKOCCExtendTool::IsEmptyShape(intSecLine))
                {
                    builder.Add(compoundResult, partSubShape);
                }
                else
                {
                    BOPAlgo_Splitter splitter;
                    splitter.AddArgument(partSubShape);
                    splitter.AddTool(intSecLine);
                    splitter.Perform();

                    const TopoDS_Shape & newShape = splitter.Shape();
                    if (newShape.IsNull())
                    {
                        return false;
                    }

                    builder.Add(compoundResult, newShape);
                    builder.Add(compoundInter, sec);
                }
            }

            try
            {
                // 分割模型。
                if (!FITKOCCExtendTool::IsEmptyShape(compoundInter))
                {
                    BOPAlgo_Splitter splitter;
                    splitter.AddArgument(topoShape);
                    splitter.AddTool(compoundInter);
                    splitter.Perform();

                    const TopoDS_Shape & newShape = splitter.Shape();
                    if (!newShape.IsNull())
                    {
                        topoShape = newShape;
                    }
                }

                if (!FITKOCCExtendTool::IsEmptyShape(compoundInterEdge))
                {
                    BOPAlgo_Splitter splitter;
                    splitter.AddArgument(topoShape);
                    splitter.AddTool(compoundInterEdge);
                    splitter.Perform();

                    const TopoDS_Shape & newShape = splitter.Shape();
                    if (!newShape.IsNull())
                    {
                        topoShape = newShape;
                    }
                }

                // 新版本，移除新壳面与原始实体交面部分。
                if (!FITKOCCExtendTool::IsEmptyShape(compoundInter))
                {
                    BRepAlgoAPI_Cut faceCutter(topoShape, compoundInter);
                    if (!faceCutter.IsDone())
                    {
                        return false;
                    }

                    const TopoDS_Shape & newShape = faceCutter.Shape();
                    if (newShape.IsNull())
                    {
                        return false;
                    }

                    topoShape = newShape;
                }

                // 源模型无二维形状则直接进行拼接组合对象。
                if (nShells == 0)
                {
                    builder.Add(compoundResult, topoShape);
                }
                else
                {
                    BRepAlgoAPI_Fuse add = BRepAlgoAPI_Fuse(compound, topoShape);
                    if (!add.IsDone())
                    {
                        return false;
                    }

                    const TopoDS_Shape & shellsShape = add.Shape();
                    builder.Add(compoundResult, shellsShape);
                }
            }
            catch (...)
            {
                return false;
            }

            part->updatePartShape(compoundResult, false);
            _occShapeAgent->updateShape(part->getCopiedPartShape(), false);
        }
        else if (part && !part->getAutoBool())
        {
            TopoDS_Shape* partShape = part->getPartShape();
            TopoDS_Builder builder;
            TopoDS_Compound compound;
            builder.MakeCompound(compound);

            if (!FITKOCCExtendTool::IsEmptyShape(*partShape))
            {
                builder.Add(compound, *partShape);
            }

            builder.Add(compound, topoShape);
            part->updatePartShape(compound, false);
            _occShapeAgent->updateShape(part->getCopiedPartShape(), false);
        }
        else
        {
            _occShapeAgent->updateShape(topoShape);
        }

        //// 如果是草绘则返回的是线的组合体，需要合并面
        //if (topoShape.ShapeType() == TopAbs_ShapeEnum::TopAbs_COMPOUND)
        //{
        //    BRep_Builder builder;
        //    TopoDS_Compound compound;
        //    builder.MakeCompound(compound);
        //    TopExp_Explorer exp;

        //    // 按封闭曲线分组
        //    for (exp.Init(topoShape, TopAbs_EDGE); exp.More(); exp.Next())
        //    {
        //        TopoDS_Edge edge = TopoDS::Edge(exp.Current());
        //        if (edge.IsNull()) continue;

        //        builder.Add(compound, edge);
        //    }
        //    topoShape = compound;
        //}
        //try {
        //    TopoDS_Shape shape = BRepPrimAPI_MakePrism(topoShape, dir).Shape();

        //    if (part)
        //    {
        //        TopoDS_Shape* partShape{ nullptr };
        //        FITKAbstractOCCModel* partAgent = part->getTShapeAgent<FITKAbstractOCCModel>();
        //        if (!partAgent)
        //        {
        //            return false;
        //        }

        //        partShape = partAgent->getShape();

        //        QList<TopoDS_Shape> partSubShapes, partSubShapes2D;
        //        FITKOCCComandCommon::FlattenCompShape2D(*partShape, partSubShapes, partSubShapes2D);

        //        // 合并布尔结果。
        //        BRep_Builder builder;
        //        TopoDS_Compound compound, compoundResult;
        //        builder.MakeCompound(compound);
        //        builder.MakeCompound(compoundResult);

        //        int nShells = 0;
        //        for (const TopoDS_Shape & partSubShape : partSubShapes2D)
        //        {
        //            builder.Add(compound, partSubShape);
        //            nShells++;
        //        }

        //        for (const TopoDS_Shape & partSubShape : partSubShapes)
        //        {
        //            builder.Add(compoundResult, partSubShape);
        //        }

        //        try
        //        {
        //            // 源模型无二维形状则直接进行拼接组合对象。
        //            if (nShells == 0)
        //            {
        //                builder.Add(compoundResult, shape);
        //            }
        //            else
        //            {
        //                BRepAlgoAPI_Fuse add = BRepAlgoAPI_Fuse(compound, shape);
        //                if (!add.IsDone())
        //                {
        //                    return false;
        //                }

        //                const TopoDS_Shape & shellsShape = add.Shape();
        //                builder.Add(compoundResult, shellsShape);
        //            }
        //        }
        //        catch (...)
        //        {
        //            return false;
        //        }

        //        part->updatePartShape(compoundResult, false);
        //        _occShapeAgent->updateShape(part->getCopiedPartShape(), false);
        //    }
        //    else
        //    {
        //        _occShapeAgent->updateShape(shape);
        //    }
        //}
        //catch (...)
        //{
        //    printLog(tr("Failed to make face!"), 3);
        //    return false;
        //}

        return true;
    }

    FITKOCCModelRevolSurface::FITKOCCModelRevolSurface() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }
    bool FITKOCCModelRevolSurface::update()
    {
        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        gp_Pnt start(m_RotateAxisPoint1[0], m_RotateAxisPoint1[1], m_RotateAxisPoint1[2]);
        gp_Pnt end(m_RotateAxisPoint2[0], m_RotateAxisPoint2[1], m_RotateAxisPoint2[2]);
        if (start.Distance(end) <= Precision::Confusion()) {
            printLog(tr("The two ends of the axis of rotation can not coincide!"), 3);
            return false;
        }

        gp_Ax1 ax1(start, gp_Dir(end.XYZ() - start.XYZ()));

        auto cmd = geoCmdList->getDataByID(m_SourceCurve.CmdId);
        if (cmd == nullptr) return false;

        TopoDS_Shape topoShape;

        // 草图。
        if (cmd->getGeometryCommandType() == Interface::FITKGeoEnum::FITKGeometryComType::FGTSketch2D)
        {
            // 合并草图。
            cmd->update();

            FITKAbstractOCCModel* occModel = cmd->getTShapeAgent<FITKAbstractOCCModel>();
            if (!occModel)
            {
                return false;
            }

            TopoDS_Shape* sketchShape = occModel->getShape();
            if (sketchShape->IsNull())
            {
                return false;
            }

            QList<TopoDS_Wire> wires;
            FITKOCCComandCommon::MakeWires(*sketchShape, wires, 1e-5);

            TopoDS_Builder builder;
            TopoDS_Compound compound;
            builder.MakeCompound(compound);

            for (const TopoDS_Wire & wire : wires)
            {
                if (wire.IsNull())
                {
                    continue;
                }

                try
                {
                    const TopoDS_Shape & shape = BRepPrimAPI_MakeRevol(wire, ax1, m_Angle  * M_PI / 180);
                    builder.Add(compound, shape);
                }
                catch (...)
                {
                    return false;
                }
            }

            topoShape = compound;
        }
        // 几何边。
        else
        {
            auto shape = cmd->getShapeT<FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSEdge, m_SourceCurve.VirtualTopoId);
            if (shape == nullptr) return false;

            try
            {
                topoShape = BRepPrimAPI_MakeRevol(shape->getTopoShape(), ax1, m_Angle  * M_PI / 180);
            }
            catch (...)
            {
                return false;
            }
        }

        if (topoShape.IsNull())
        {
            return false;
        }

        FITKOCCModelPart* part = _occShapeAgent->getPart();

        if (part && part->getAutoBool())
        {
            TopoDS_Shape* partShape{ nullptr };
            FITKAbstractOCCModel* partAgent = part->getTShapeAgent<FITKAbstractOCCModel>();
            if (!partAgent)
            {
                return false;
            }

            partShape = partAgent->getShape();

            QList<TopoDS_Shape> partSubShapes, partSubShapes2D, partSubShapes3D;
            FITKOCCComandCommon::FlattenCompShape2D3D(*partShape, partSubShapes, partSubShapes2D, partSubShapes3D);

            // 合并布尔结果。
            BRep_Builder builder;
            TopoDS_Compound compound, compoundResult, compoundInter, compoundInterEdge;
            builder.MakeCompound(compound);
            builder.MakeCompound(compoundResult);
            builder.MakeCompound(compoundInter);
            builder.MakeCompound(compoundInterEdge);

            int nShells = 0;
            for (const TopoDS_Shape & partSubShape : partSubShapes2D)
            {
                builder.Add(compound, partSubShape);
                nShells++;
            }

            for (const TopoDS_Shape & partSubShape : partSubShapes)
            {
                // 处理一维模型与壳面相交的两种情况：存在交点、存在交线。
                if (partSubShape.ShapeType() == TopAbs_ShapeEnum::TopAbs_VERTEX)
                {
                    builder.Add(compoundResult, partSubShape);
                }
                else
                {
                    BRepExtrema_DistShapeShape distSS;
                    distSS.LoadS1(partSubShape);
                    distSS.LoadS2(topoShape);
                    distSS.Perform();

                    if (distSS.IsDone() && distSS.NbSolution() > 0)
                    {
                        TopoDS_Compound compoundVerts;
                        builder.MakeCompound(compoundVerts);

                        for (int i = 1; i <= distSS.NbSolution(); i++)
                        {
                            const gp_Pnt & cPt = distSS.PointOnShape1(i);

                            const TopoDS_Vertex & vert = BRepBuilderAPI_MakeVertex(cPt);
                            builder.Add(compoundVerts, vert);
                        }

                        TopoDS_Shape newEdge;

                        BRepAlgoAPI_Section intSecLine(topoShape, partSubShape);
                        intSecLine.Build();
                        if (!intSecLine.IsDone())
                        {
                            return false;
                        }

                        const TopoDS_Shape & sec = intSecLine.Shape();

                        BRepAlgoAPI_Cut edgeCutter(partSubShape, sec);
                        if (edgeCutter.IsDone())
                        {
                            newEdge = edgeCutter.Shape();
                        }
                        else
                        {
                            BOPAlgo_Splitter splitter;
                            splitter.AddArgument(partSubShape);
                            splitter.AddTool(compoundVerts);
                            splitter.Perform();

                            newEdge = splitter.Shape();
                        }

                        if (newEdge.IsNull())
                        {
                            return false;
                        }

                        builder.Add(compoundResult, newEdge);

                        builder.Add(compoundInterEdge, compoundVerts);
                    }
                    else
                    {
                        builder.Add(compoundResult, partSubShape);
                    }
                }
            }

            for (const TopoDS_Shape & partSubShape : partSubShapes3D)
            {
                BRepAlgoAPI_Section intSecLine(topoShape, partSubShape);
                intSecLine.Build();
                if (!intSecLine.IsDone())
                {
                    return false;
                }

                BRepAlgoAPI_Common intSec(topoShape, partSubShape);
                intSec.Build();
                if (!intSec.IsDone())
                {
                    return false;
                }

                const TopoDS_Shape & sec = intSec.Shape();
                if (FITKOCCExtendTool::IsEmptyShape(intSecLine))
                {
                    builder.Add(compoundResult, partSubShape);
                }
                else
                {
                    BOPAlgo_Splitter splitter;
                    splitter.AddArgument(partSubShape);
                    splitter.AddTool(intSecLine);
                    splitter.Perform();

                    const TopoDS_Shape & newShape = splitter.Shape();
                    if (newShape.IsNull())
                    {
                        return false;
                    }

                    builder.Add(compoundResult, newShape);
                    builder.Add(compoundInter, sec);
                }
            }

            try
            {
                // 分割模型。
                if (!FITKOCCExtendTool::IsEmptyShape(compoundInter))
                {
                    BOPAlgo_Splitter splitter;
                    splitter.AddArgument(topoShape);
                    splitter.AddTool(compoundInter);
                    splitter.Perform();

                    const TopoDS_Shape & newShape = splitter.Shape();
                    if (!newShape.IsNull())
                    {
                        topoShape = newShape;
                    }
                }

                if (!FITKOCCExtendTool::IsEmptyShape(compoundInterEdge))
                {
                    BOPAlgo_Splitter splitter;
                    splitter.AddArgument(topoShape);
                    splitter.AddTool(compoundInterEdge);
                    splitter.Perform();

                    const TopoDS_Shape & newShape = splitter.Shape();
                    if (!newShape.IsNull())
                    {
                        topoShape = newShape;
                    }
                }

                // 新版本，移除新壳面与原始实体交面部分。
                if (!FITKOCCExtendTool::IsEmptyShape(compoundInter))
                {
                    BRepAlgoAPI_Cut faceCutter(topoShape, compoundInter);
                    if (!faceCutter.IsDone())
                    {
                        return false;
                    }

                    const TopoDS_Shape & newShape = faceCutter.Shape();
                    if (newShape.IsNull())
                    {
                        return false;
                    }

                    topoShape = newShape;
                }

                // 源模型无二维形状则直接进行拼接组合对象。
                if (nShells == 0)
                {
                    builder.Add(compoundResult, topoShape);
                }
                else
                {
                    BRepAlgoAPI_Fuse add = BRepAlgoAPI_Fuse(compound, topoShape);
                    if (!add.IsDone())
                    {
                        return false;
                    }

                    const TopoDS_Shape & shellsShape = add.Shape();
                    builder.Add(compoundResult, shellsShape);
                }
            }
            catch (...)
            {
                return false;
            }

            part->updatePartShape(compoundResult, false);
            _occShapeAgent->updateShape(part->getCopiedPartShape(), false);
        }
        else if (part && !part->getAutoBool())
        {
            TopoDS_Shape* partShape = part->getPartShape();
            TopoDS_Builder builder;
            TopoDS_Compound compound;
            builder.MakeCompound(compound);

            if (!FITKOCCExtendTool::IsEmptyShape(*partShape))
            {
                builder.Add(compound, *partShape);
            }

            builder.Add(compound, topoShape);
            part->updatePartShape(compound, false);
            _occShapeAgent->updateShape(part->getCopiedPartShape(), false);
        }
        else
        {
            _occShapeAgent->updateShape(topoShape);
        }

        //m_OriginId;

        //// 如果是草绘则返回的是线的组合体，需要合并面
        //if (topoShape.ShapeType() == TopAbs_ShapeEnum::TopAbs_COMPOUND)
        //{
        //    BRep_Builder builder;
        //    TopoDS_Compound compound;
        //    builder.MakeCompound(compound);
        //    TopExp_Explorer exp;

        //    // 按封闭曲线分组
        //    for (exp.Init(topoShape, TopAbs_EDGE); exp.More(); exp.Next())
        //    {
        //        TopoDS_Edge edge = TopoDS::Edge(exp.Current());
        //        if (edge.IsNull()) continue;

        //        builder.Add(compound, edge);
        //    }
        //    topoShape = compound;
        //}
        //try {
        //    TopoDS_Shape shape = BRepPrimAPI_MakeRevol(topoShape, ax1, m_Angle  * M_PI / 180).Shape();

        //    if (part)
        //    {
        //        TopoDS_Shape* partShape{ nullptr };
        //        FITKAbstractOCCModel* partAgent = part->getTShapeAgent<FITKAbstractOCCModel>();
        //        if (!partAgent)
        //        {
        //            return false;
        //        }

        //        partShape = partAgent->getShape();

        //        QList<TopoDS_Shape> partSubShapes, partSubShapes2D;
        //        FITKOCCComandCommon::FlattenCompShape2D(*partShape, partSubShapes, partSubShapes2D);

        //        // 合并布尔结果。
        //        BRep_Builder builder;
        //        TopoDS_Compound compound, compoundResult;
        //        builder.MakeCompound(compound);
        //        builder.MakeCompound(compoundResult);

        //        int nShells = 0;
        //        for (const TopoDS_Shape & partSubShape : partSubShapes2D)
        //        {
        //            builder.Add(compound, partSubShape);
        //            nShells++;
        //        }

        //        for (const TopoDS_Shape & partSubShape : partSubShapes)
        //        {
        //            builder.Add(compoundResult, partSubShape);
        //        }

        //        try
        //        {
        //            // 源模型无二维形状则直接进行拼接组合对象。
        //            if (nShells == 0)
        //            {
        //                builder.Add(compoundResult, shape);
        //            }
        //            else
        //            {
        //                BRepAlgoAPI_Fuse add = BRepAlgoAPI_Fuse(compound, shape);
        //                if (!add.IsDone())
        //                {
        //                    return false;
        //                }

        //                const TopoDS_Shape & shellsShape = add.Shape();
        //                builder.Add(compoundResult, shellsShape);
        //            }
        //        }
        //        catch (...)
        //        {
        //            return false;
        //        }

        //        part->updatePartShape(compoundResult, false);
        //        _occShapeAgent->updateShape(part->getCopiedPartShape(), false);
        //    }
        //    else
        //    {
        //        _occShapeAgent->updateShape(shape);
        //    } 
        //}
        //catch (...)
        //{
        //    printLog(tr("Failed to make face!"), 3);
        //    return false;
        //}

        return true;
    }
    FITKOCCModelSweepSurface::FITKOCCModelSweepSurface() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }
    bool FITKOCCModelSweepSurface::update()
    {
        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        auto profileCmd = geoCmdList->getDataByID(m_Profiles.at(0).CmdId);
        auto curveCmd = geoCmdList->getDataByID(m_Curves.at(0).CmdId);
        if (profileCmd == nullptr || curveCmd == nullptr) return false;

        //Interface::FITKGeoEnum::VTopoShapeType curveType = curveCmd->getGeometryCommandType() == Interface::FITKGeoEnum::FITKGeometryComType::FGTSketch2D ? Interface::FITKGeoEnum::VTopoShapeType::VSAssembly : Interface::FITKGeoEnum::VTopoShapeType::VSEdge;
        //auto curveShape = curveCmd->getShapeT<FITKOCCTopoShape>(curveType, m_Curves.at(0).VirtualTopoId);
        //if (curveShape == nullptr) return false;

        //auto curveTopoShape = curveShape->getTopoShape();

        TopoDS_Shape curveTopoShape;

        // 草图。
        TopoDS_Builder builder;
        QList<TopoDS_Wire> wires;
        if (curveCmd->getGeometryCommandType() == Interface::FITKGeoEnum::FITKGeometryComType::FGTSketch2D)
        {
            // 合并草图。
            curveCmd->update();

            FITKAbstractOCCModel* occModel = curveCmd->getTShapeAgent<FITKAbstractOCCModel>();
            if (!occModel)
            {
                return false;
            }

            TopoDS_Shape* sketchShape = occModel->getShape();
            if (sketchShape->IsNull())
            {
                return false;
            }

            FITKOCCComandCommon::MakeWires(*sketchShape, wires, 1e-5);
        }
        // 几何边。
        else
        {
            TopoDS_Compound compoundEdges;
            builder.MakeCompound(compoundEdges);

            for (const Interface::VirtualShape & shape : m_Curves)
            {
                auto occShape = curveCmd->getShapeT<FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSEdge, shape.VirtualTopoId);
                if (!occShape)
                {
                    return false;
                }

                builder.Add(compoundEdges, occShape->getTopoShape());
            }

            FITKOCCComandCommon::MakeWires(compoundEdges, wires, 1e-5);
        }

        if (wires.count() != 1)
        {
            return false;
        }

        curveTopoShape = wires.first();

        TopoDS_Wire sweepWire;

        try
        {
            auto type = curveTopoShape.ShapeType();
            if (type == TopAbs_ShapeEnum::TopAbs_WIRE)
            {
                sweepWire = TopoDS::Wire(curveTopoShape);
            }
            else if (type == TopAbs_ShapeEnum::TopAbs_EDGE) {
                sweepWire = BRepBuilderAPI_MakeWire(TopoDS::Edge(curveTopoShape));
            }
            else if (type == TopAbs_ShapeEnum::TopAbs_COMPOUND) {
                TopoDS_Compound compound = TopoDS::Compound(curveTopoShape);
                TopExp_Explorer exp;
                BRepBuilderAPI_MakeWire makeWire;
                for (exp.Init(compound, TopAbs_EDGE); exp.More(); exp.Next())
                {
                    TopoDS_Edge edge = TopoDS::Edge(exp.Current());
                    if (edge.IsNull()) continue;
                    makeWire.Add(edge);
                }
                makeWire.Build();
                if (!makeWire.IsDone()) return false;
                sweepWire = TopoDS::Wire(makeWire.Shape());
            }
            else return false;
        }
        catch (...)
        {
            return false;
        }

        TopoDS_Shape topoShape;

        // 草图。
        if (profileCmd->getGeometryCommandType() == Interface::FITKGeoEnum::FITKGeometryComType::FGTSketch2D)
        {
            // 合并草图。
            profileCmd->update();

            FITKAbstractOCCModel* occModel = profileCmd->getTShapeAgent<FITKAbstractOCCModel>();
            if (!occModel)
            {
                return false;
            }

            TopoDS_Shape* sketchShape = occModel->getShape();
            if (sketchShape->IsNull())
            {
                return false;
            }

            QList<TopoDS_Wire> wires;
            FITKOCCComandCommon::MakeWires(*sketchShape, wires, 1e-5);

            TopoDS_Builder builder;
            TopoDS_Compound compound;
            builder.MakeCompound(compound);

            for (const TopoDS_Wire & wire : wires)
            {
                if (wire.IsNull())
                {
                    continue;
                }

                BRepOffsetAPI_MakePipeShell mp(sweepWire);
                mp.SetTransitionMode(BRepBuilderAPI_TransitionMode::BRepBuilderAPI_RightCorner);
                mp.SetDiscreteMode();
                mp.Add(wire);
                mp.Build();
                if (!mp.IsDone())
                {
                    return false;
                }

                const TopoDS_Shape & shape = mp.Shape();

                // const TopoDS_Shape & shape = BRepOffsetAPI_MakePipe(sweepWire, wire);
                builder.Add(compound, shape);
            }

            topoShape = compound;
        }
        // 几何边。
        else
        {
            auto shape = profileCmd->getShapeT<FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSEdge, m_Profiles.at(0).VirtualTopoId);
            if (shape == nullptr) return false;

            BRepOffsetAPI_MakePipeShell mp(sweepWire);
            mp.SetTransitionMode(BRepBuilderAPI_TransitionMode::BRepBuilderAPI_RightCorner);
            mp.SetDiscreteMode();
            mp.Add(shape->getTopoShape());
            mp.Build();
            if (!mp.IsDone())
            {
                return false;
            }

            topoShape = mp.Shape();
            //topoShape = BRepOffsetAPI_MakePipeShell(sweepWire, shape->getTopoShape());
        }

        if (topoShape.IsNull())
        {
            return false;
        }

        FITKOCCModelPart* part = _occShapeAgent->getPart();

        try
        {
            if (part && part->getAutoBool())
            {
                TopoDS_Shape* partShape{ nullptr };
                FITKAbstractOCCModel* partAgent = part->getTShapeAgent<FITKAbstractOCCModel>();
                if (!partAgent)
                {
                    return false;
                }

                partShape = partAgent->getShape();

                QList<TopoDS_Shape> partSubShapes, partSubShapes2D, partSubShapes3D;
                FITKOCCComandCommon::FlattenCompShape2D3D(*partShape, partSubShapes, partSubShapes2D, partSubShapes3D);

                // 合并布尔结果。
                BRep_Builder builder;
                TopoDS_Compound compound, compoundResult, compoundInter, compoundInterEdge;
                builder.MakeCompound(compound);
                builder.MakeCompound(compoundResult);
                builder.MakeCompound(compoundInter);
                builder.MakeCompound(compoundInterEdge);

                int nShells = 0;
                for (const TopoDS_Shape & partSubShape : partSubShapes2D)
                {
                    builder.Add(compound, partSubShape);
                    nShells++;
                }

                for (const TopoDS_Shape & partSubShape : partSubShapes)
                {
                    // 处理一维模型与壳面相交的两种情况：存在交点、存在交线。
                    if (partSubShape.ShapeType() == TopAbs_ShapeEnum::TopAbs_VERTEX)
                    {
                        builder.Add(compoundResult, partSubShape);
                    }
                    else
                    {
                        BRepExtrema_DistShapeShape distSS;
                        distSS.LoadS1(partSubShape);
                        distSS.LoadS2(topoShape);
                        distSS.Perform();

                        if (distSS.IsDone() && distSS.NbSolution() > 0)
                        {
                            TopoDS_Compound compoundVerts;
                            builder.MakeCompound(compoundVerts);

                            for (int i = 1; i <= distSS.NbSolution(); i++)
                            {
                                const gp_Pnt & cPt = distSS.PointOnShape1(i);

                                const TopoDS_Vertex & vert = BRepBuilderAPI_MakeVertex(cPt);
                                builder.Add(compoundVerts, vert);
                            }

                            TopoDS_Shape newEdge;

                            BRepAlgoAPI_Section intSecLine(topoShape, partSubShape);
                            intSecLine.Build();
                            if (!intSecLine.IsDone())
                            {
                                return false;
                            }

                            const TopoDS_Shape & sec = intSecLine.Shape();

                            BRepAlgoAPI_Cut edgeCutter(partSubShape, sec);
                            if (edgeCutter.IsDone())
                            {
                                newEdge = edgeCutter.Shape();
                            }
                            else
                            {
                                BOPAlgo_Splitter splitter;
                                splitter.AddArgument(partSubShape);
                                splitter.AddTool(compoundVerts);
                                splitter.Perform();

                                newEdge = splitter.Shape();
                            }

                            if (newEdge.IsNull())
                            {
                                return false;
                            }

                            builder.Add(compoundResult, newEdge);

                            builder.Add(compoundInterEdge, compoundVerts);
                        }
                        else
                        {
                            builder.Add(compoundResult, partSubShape);
                        }
                    }
                }

                for (const TopoDS_Shape & partSubShape : partSubShapes3D)
                {
                    BRepAlgoAPI_Section intSecLine(topoShape, partSubShape);
                    intSecLine.Build();
                    if (!intSecLine.IsDone())
                    {
                        return false;
                    }

                    BRepAlgoAPI_Common intSec(topoShape, partSubShape);
                    intSec.Build();
                    if (!intSec.IsDone())
                    {
                        return false;
                    }

                    const TopoDS_Shape & sec = intSec.Shape();
                    if (FITKOCCExtendTool::IsEmptyShape(intSecLine))
                    {
                        builder.Add(compoundResult, partSubShape);
                    }
                    else
                    {
                        BOPAlgo_Splitter splitter;
                        splitter.AddArgument(partSubShape);
                        splitter.AddTool(intSecLine);
                        splitter.Perform();

                        const TopoDS_Shape & newShape = splitter.Shape();
                        if (newShape.IsNull())
                        {
                            return false;
                        }

                        builder.Add(compoundResult, newShape);
                        builder.Add(compoundInter, sec);
                    }
                }

                try
                {
                    // 分割模型。
                    if (!FITKOCCExtendTool::IsEmptyShape(compoundInter))
                    {
                        BOPAlgo_Splitter splitter;
                        splitter.AddArgument(topoShape);
                        splitter.AddTool(compoundInter);
                        splitter.Perform();

                        const TopoDS_Shape & newShape = splitter.Shape();
                        if (!newShape.IsNull())
                        {
                            topoShape = newShape;
                        }
                    }

                    if (!FITKOCCExtendTool::IsEmptyShape(compoundInterEdge))
                    {
                        BOPAlgo_Splitter splitter;
                        splitter.AddArgument(topoShape);
                        splitter.AddTool(compoundInterEdge);
                        splitter.Perform();

                        const TopoDS_Shape & newShape = splitter.Shape();
                        if (!newShape.IsNull())
                        {
                            topoShape = newShape;
                        }
                    }

                    // 新版本，移除新壳面与原始实体交面部分。
                    if (!FITKOCCExtendTool::IsEmptyShape(compoundInter))
                    {
                        BRepAlgoAPI_Cut faceCutter(topoShape, compoundInter);
                        if (!faceCutter.IsDone())
                        {
                            return false;
                        }

                        const TopoDS_Shape & newShape = faceCutter.Shape();
                        if (newShape.IsNull())
                        {
                            return false;
                        }

                        topoShape = newShape;
                    }

                    // 没有体则直接进行拼接组合对象。
                    if (nShells == 0)
                    {
                        builder.Add(compoundResult, topoShape);
                    }
                    else
                    {
                        BRepAlgoAPI_Fuse add = BRepAlgoAPI_Fuse(compound, topoShape);
                        if (!add.IsDone())
                        {
                            return false;
                        }

                        const TopoDS_Shape & solidsShape = add.Shape();
                        builder.Add(compoundResult, solidsShape);
                    }

                    part->updatePartShape(compoundResult, false);
                    _occShapeAgent->updateShape(part->getCopiedPartShape(), false);
                }
                catch (...)
                {
                    return false;
                }
            }
            else if (part && !part->getAutoBool())
            {
                TopoDS_Shape* partShape = part->getPartShape();
                TopoDS_Builder builder;
                TopoDS_Compound compound;
                builder.MakeCompound(compound);

                if (!FITKOCCExtendTool::IsEmptyShape(*partShape))
                {
                    builder.Add(compound, *partShape);
                }

                builder.Add(compound, topoShape);
                part->updatePartShape(compound, false);
                _occShapeAgent->updateShape(part->getCopiedPartShape(), false);
            }
            else
            {
                _occShapeAgent->updateShape(topoShape);
            }
        }
        catch (...)
        {
            printLog(tr("Failed to make solid!"), 3);
            return false;
        }

        //Interface::FITKGeoEnum::VTopoShapeType curveType = curveCmd->getGeometryCommandType() == Interface::FITKGeoEnum::FITKGeometryComType::FGTSketch2D ? Interface::FITKGeoEnum::VTopoShapeType::VSAssembly : Interface::FITKGeoEnum::VTopoShapeType::VSEdge;
        //auto curveShape = curveCmd->getShapeT<FITKOCCTopoShape>(curveType, m_Curves.at(0).VirtualTopoId);
        //if (curveShape == nullptr) return false;
        //auto curveTopoShape = curveShape->getTopoShape();

        //TopoDS_Wire sweepWire;
        //auto type = curveTopoShape.ShapeType();
        //if (type == TopAbs_ShapeEnum::TopAbs_WIRE)
        //{
        //    sweepWire = TopoDS::Wire(curveTopoShape);
        //}
        //else if (type == TopAbs_ShapeEnum::TopAbs_EDGE) {
        //    sweepWire = BRepBuilderAPI_MakeWire(TopoDS::Edge(curveTopoShape));
        //}
        //else if (type == TopAbs_ShapeEnum::TopAbs_COMPOUND) {
        //    TopoDS_Compound compound = TopoDS::Compound(curveTopoShape);
        //    TopExp_Explorer exp;
        //    BRepBuilderAPI_MakeWire makeWire;
        //    for (exp.Init(compound, TopAbs_EDGE); exp.More(); exp.Next())
        //    {
        //        TopoDS_Edge edge = TopoDS::Edge(exp.Current());
        //        if (edge.IsNull()) continue;
        //        makeWire.Add(edge);
        //    }
        //    makeWire.Build();
        //    if (!makeWire.IsDone()) return false;
        //    sweepWire = TopoDS::Wire(makeWire.Shape());
        //}
        //else return false;

        //// 如果是草绘则返回的是线的组合体，需要合并面
        //if (profileShape.ShapeType() == TopAbs_ShapeEnum::TopAbs_COMPOUND)
        //{
        //    BRep_Builder builder;
        //    TopoDS_Compound compound;
        //    builder.MakeCompound(compound);
        //    TopExp_Explorer exp;

        //    // 按封闭曲线分组
        //    for (exp.Init(profileShape, TopAbs_EDGE); exp.More(); exp.Next())
        //    {
        //        TopoDS_Edge edge = TopoDS::Edge(exp.Current());
        //        if (edge.IsNull()) continue;

        //        builder.Add(compound, edge);
        //    }
        //    profileShape = compound;
        //}
        //try {

        //    BRepOffsetAPI_MakePipe pipe(sweepWire, profileShape);
        //    pipe.Build();
        //    if (!pipe.IsDone()) return false;

        //    TopoDS_Shape shape = pipe.Shape();

        //    if (part)
        //    {
        //        TopoDS_Shape* partShape{ nullptr };
        //        FITKAbstractOCCModel* partAgent = part->getTShapeAgent<FITKAbstractOCCModel>();
        //        if (!partAgent)
        //        {
        //            return false;
        //        }

        //        partShape = partAgent->getShape();

        //        QList<TopoDS_Shape> partSubShapes, partSubShapes2D;
        //        FITKOCCComandCommon::FlattenCompShape2D(*partShape, partSubShapes, partSubShapes2D);

        //        // 合并布尔结果。
        //        BRep_Builder builder;
        //        TopoDS_Compound compound, compoundResult;
        //        builder.MakeCompound(compound);
        //        builder.MakeCompound(compoundResult);

        //        int nShells = 0;
        //        for (const TopoDS_Shape & partSubShape : partSubShapes2D)
        //        {
        //            builder.Add(compound, partSubShape);
        //            nShells++;
        //        }

        //        for (const TopoDS_Shape & partSubShape : partSubShapes)
        //        {
        //            builder.Add(compoundResult, partSubShape);
        //        }

        //        try
        //        {
        //            // 源模型无二维形状则直接进行拼接组合对象。
        //            if (nShells == 0)
        //            {
        //                builder.Add(compoundResult, shape);
        //            }
        //            else
        //            {
        //                BRepAlgoAPI_Fuse add = BRepAlgoAPI_Fuse(compound, shape);
        //                if (!add.IsDone())
        //                {
        //                    return false;
        //                }

        //                const TopoDS_Shape & shellsShape = add.Shape();
        //                builder.Add(compoundResult, shellsShape);
        //            }
        //        }
        //        catch (...)
        //        {
        //            return false;
        //        }

        //        part->updatePartShape(compoundResult, false);
        //        _occShapeAgent->updateShape(part->getCopiedPartShape(), false);
        //    }
        //    else 
        //    {
        //        _occShapeAgent->updateShape(shape);
        //    }
        //}
        //catch (...)
        //{
        //    printLog(tr("Failed to make face!"), 3);
        //    return false;
        //}

        return true;
    }

    FITKOCCModelMultiSectionSurface::FITKOCCModelMultiSectionSurface() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelMultiSectionSurface::update()
    {

        if (m_Sections.size() < 2) return false;

        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        FITKOCCModelPart* part = _occShapeAgent->getPart();

        BRepOffsetAPI_ThruSections thruSection(false);

        for (auto section : m_Sections) 
        {
            auto cmd = geoCmdList->getDataByID(section.CmdId);
            if (cmd == nullptr) return false;

            TopoDS_Shape topoShape;

            // 草图。
            if (cmd->getGeometryCommandType() == Interface::FITKGeoEnum::FITKGeometryComType::FGTSketch2D)
            {
                // 合并草图。
                cmd->update();

                FITKAbstractOCCModel* occModel = cmd->getTShapeAgent<FITKAbstractOCCModel>();
                if (!occModel)
                {
                    return false;
                }

                TopoDS_Shape* sketchShape = occModel->getShape();
                if (sketchShape->IsNull())
                {
                    return false;
                }

                topoShape = BRepBuilderAPI_Copy(*sketchShape);
            }
            // 几何边。
            else
            {
                auto shape = cmd->getShapeT<FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSEdge, section.VirtualTopoId);
                if (shape == nullptr) return false;
                topoShape = shape->getTopoShape();
            }

            if (topoShape.IsNull())
            {
                return false;
            }

            if (topoShape.ShapeType() == TopAbs_ShapeEnum::TopAbs_EDGE) {
                BRepLib_MakeWire wire(TopoDS::Edge(topoShape));
                thruSection.AddWire(wire.Wire());
            }
            else if (topoShape.ShapeType() == TopAbs_ShapeEnum::TopAbs_WIRE) {
                thruSection.AddWire(TopoDS::Wire(topoShape));
            }
            else if (topoShape.ShapeType() == TopAbs_ShapeEnum::TopAbs_COMPOUND) {
                TopExp_Explorer exp;
                BRepLib_MakeWire wire;
                // 按封闭曲线分组
                for (exp.Init(topoShape, TopAbs_EDGE); exp.More(); exp.Next())
                {
                    TopoDS_Edge edge = TopoDS::Edge(exp.Current());
                    if (edge.IsNull()) continue;
                    wire.Add(edge);
                }
                try
                {
                    wire.Build();
                    if (!wire.IsDone()) return false;
                    thruSection.AddWire(wire.Wire());
                }
                catch (...)
                {
                    printLog(tr("Failed to make wire!"), 3);
                    return false;
                }
            }
        }

        try {
            thruSection.Build();
            if (!thruSection.IsDone()) return false;

            TopoDS_Shape shape = thruSection.Shape();

            if (part && part->getAutoBool())
            {
                TopoDS_Shape* partShape{ nullptr };
                FITKAbstractOCCModel* partAgent = part->getTShapeAgent<FITKAbstractOCCModel>();
                if (!partAgent)
                {
                    return false;
                }

                partShape = partAgent->getShape();

                QList<TopoDS_Shape> partSubShapes, partSubShapes2D, partSubShapes3D;
                FITKOCCComandCommon::FlattenCompShape2D3D(*partShape, partSubShapes, partSubShapes2D, partSubShapes3D);

                // 合并布尔结果。
                BRep_Builder builder;
                TopoDS_Compound compound, compoundResult, compoundInter, compoundInterEdge;
                builder.MakeCompound(compound);
                builder.MakeCompound(compoundResult);
                builder.MakeCompound(compoundInter);
                builder.MakeCompound(compoundInterEdge);

                int nShells = 0;
                for (const TopoDS_Shape & partSubShape : partSubShapes2D)
                {
                    builder.Add(compound, partSubShape);
                    nShells++;
                }

                for (const TopoDS_Shape & partSubShape : partSubShapes)
                {
                    // 处理一维模型与壳面相交的两种情况：存在交点、存在交线。
                    if (partSubShape.ShapeType() == TopAbs_ShapeEnum::TopAbs_VERTEX)
                    {
                        builder.Add(compoundResult, partSubShape);
                    }
                    else
                    {
                        BRepExtrema_DistShapeShape distSS;
                        distSS.LoadS1(partSubShape);
                        distSS.LoadS2(shape);
                        distSS.Perform();

                        if (distSS.IsDone() && distSS.NbSolution() > 0)
                        {
                            TopoDS_Compound compoundVerts;
                            builder.MakeCompound(compoundVerts);

                            for (int i = 1; i <= distSS.NbSolution(); i++)
                            {
                                const gp_Pnt & cPt = distSS.PointOnShape1(i);

                                const TopoDS_Vertex & vert = BRepBuilderAPI_MakeVertex(cPt);
                                builder.Add(compoundVerts, vert);
                            }

                            TopoDS_Shape newEdge;

                            BRepAlgoAPI_Section intSecLine(shape, partSubShape);
                            intSecLine.Build();
                            if (!intSecLine.IsDone())
                            {
                                return false;
                            }

                            const TopoDS_Shape & sec = intSecLine.Shape();

                            BRepAlgoAPI_Cut edgeCutter(partSubShape, sec);
                            if (edgeCutter.IsDone())
                            {
                                newEdge = edgeCutter.Shape();
                            }
                            else
                            {
                                BOPAlgo_Splitter splitter;
                                splitter.AddArgument(partSubShape);
                                splitter.AddTool(compoundVerts);
                                splitter.Perform();

                                newEdge = splitter.Shape();
                            }

                            if (newEdge.IsNull())
                            {
                                return false;
                            }

                            builder.Add(compoundResult, newEdge);

                            builder.Add(compoundInterEdge, compoundVerts);
                        }
                        else
                        {
                            builder.Add(compoundResult, partSubShape);
                        }
                    }
                }

                for (const TopoDS_Shape & partSubShape : partSubShapes3D)
                {
                    BRepAlgoAPI_Section intSecLine(shape, partSubShape);
                    intSecLine.Build();
                    if (!intSecLine.IsDone())
                    {
                        return false;
                    }

                    BRepAlgoAPI_Common intSec(shape, partSubShape);
                    intSec.Build();
                    if (!intSec.IsDone())
                    {
                        return false;
                    }

                    const TopoDS_Shape & sec = intSec.Shape();
                    if (FITKOCCExtendTool::IsEmptyShape(intSecLine))
                    {
                        builder.Add(compoundResult, partSubShape);
                    }
                    else
                    {
                        BOPAlgo_Splitter splitter;
                        splitter.AddArgument(partSubShape);
                        splitter.AddTool(intSecLine);
                        splitter.Perform();

                        const TopoDS_Shape & newShape = splitter.Shape();
                        if (newShape.IsNull())
                        {
                            return false;
                        }

                        builder.Add(compoundResult, newShape);
                        builder.Add(compoundInter, sec);
                    }
                }

                try
                {
                    // 分割模型。
                    if (!FITKOCCExtendTool::IsEmptyShape(compoundInter))
                    {
                        BOPAlgo_Splitter splitter;
                        splitter.AddArgument(shape);
                        splitter.AddTool(compoundInter);
                        splitter.Perform();

                        const TopoDS_Shape & newShape = splitter.Shape();
                        if (!newShape.IsNull())
                        {
                            shape = newShape;
                        }
                    }

                    if (!FITKOCCExtendTool::IsEmptyShape(compoundInterEdge))
                    {
                        BOPAlgo_Splitter splitter;
                        splitter.AddArgument(shape);
                        splitter.AddTool(compoundInterEdge);
                        splitter.Perform();

                        const TopoDS_Shape & newShape = splitter.Shape();
                        if (!newShape.IsNull())
                        {
                            shape = newShape;
                        }
                    }

                    // 新版本，移除新壳面与原始实体交面部分。
                    if (!FITKOCCExtendTool::IsEmptyShape(compoundInter))
                    {
                        BRepAlgoAPI_Cut faceCutter(shape, compoundInter);
                        if (!faceCutter.IsDone())
                        {
                            return false;
                        }

                        const TopoDS_Shape & newShape = faceCutter.Shape();
                        if (newShape.IsNull())
                        {
                            return false;
                        }

                        shape = newShape;
                    }

                    // 源模型无二维形状则直接进行拼接组合对象。
                    if (nShells == 0)
                    {
                        builder.Add(compoundResult, shape);
                    }
                    else
                    {
                        BRepAlgoAPI_Fuse add = BRepAlgoAPI_Fuse(compound, shape);
                        if (!add.IsDone())
                        {
                            return false;
                        }

                        const TopoDS_Shape & shellsShape = add.Shape();
                        builder.Add(compoundResult, shellsShape);
                    }
                }
                catch (...)
                {
                    return false;
                }

                part->updatePartShape(compoundResult, false);
                _occShapeAgent->updateShape(part->getCopiedPartShape(), false);
            }
            else if (part && !part->getAutoBool())
            {
                TopoDS_Shape* partShape = part->getPartShape();
                TopoDS_Builder builder;
                TopoDS_Compound compound;
                builder.MakeCompound(compound);

                if (!FITKOCCExtendTool::IsEmptyShape(*partShape))
                {
                    builder.Add(compound, *partShape);
                }

                builder.Add(compound, shape);
                part->updatePartShape(compound, false);
                _occShapeAgent->updateShape(part->getCopiedPartShape(), false);
            }
            else
            {
                _occShapeAgent->updateShape(shape);
            }
        }
        catch (...)
        {
            printLog(tr("Failed to make face!"), 3);
            return false;
        }
        return true;
    }

    FITKOCCModelBridgeSurface::FITKOCCModelBridgeSurface() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelBridgeSurface::update()
    {
        if (m_SourceSurface1.isNull() || m_SourceSurface2.isNull()) return false;

        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        /** 第一条曲面端点 */
        /*@{*/
        auto surfaceCmd1 = geoCmdList->getDataByID(m_SourceSurface1.CmdId);
        if (surfaceCmd1 == nullptr) return false;
        auto faceVshape1 = surfaceCmd1->getShapeT<FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSFace, m_SourceSurface1.VirtualTopoId);
        auto edgeVShape1 = surfaceCmd1->getShapeT<FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSEdge, m_EdgeVirtualTopoId1);
        if (faceVshape1 == nullptr || edgeVShape1 == nullptr) return false;
        TopoDS_Shape faceShape1 = faceVshape1->getTopoShape();
        if (faceShape1.ShapeType() != TopAbs_ShapeEnum::TopAbs_FACE) return false;
        TopoDS_Face face1 = TopoDS::Face(faceShape1);
        if (face1.IsNull()) return false;

        TopoDS_Shape edgeShape1 = edgeVShape1->getTopoShape();
        if (edgeShape1.ShapeType() != TopAbs_ShapeEnum::TopAbs_EDGE) return false;
        TopoDS_Edge edge1 = TopoDS::Edge(edgeShape1);
        if (edge1.IsNull()) return false;

        Standard_Real first1, last1;
        Handle(Geom_Curve) curve1 = BRep_Tool::Curve(edge1, first1, last1);

        gp_Pnt pnt1 = curve1->Value(first1);
        gp_Pnt pnt2 = curve1->Value(last1);

        TopoDS_Edge adjacentEdge11;
        TopoDS_Edge adjacentEdge12;
        // 邻接边
        TopExp_Explorer exp1;
        for (exp1.Init(face1, TopAbs_ShapeEnum::TopAbs_EDGE); exp1.More(); exp1.Next()) {
            TopoDS_Edge edge = TopoDS::Edge(exp1.Current());
            if (edge.IsNull()) return false;


            Standard_Real first, last;
            Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, first, last);

            gp_Pnt firstPnt = curve->Value(first);
            gp_Pnt lastPnt = curve->Value(last);
            // 忽略要桥接的边
            if (
                (firstPnt.Distance(pnt1) < Precision::Confusion() && lastPnt.Distance(pnt2) < Precision::Confusion())
                || (firstPnt.Distance(pnt2) < Precision::Confusion() && lastPnt.Distance(pnt1) < Precision::Confusion())) {
                continue;
            }


            if (firstPnt.Distance(pnt1) < Precision::Confusion() || lastPnt.Distance(pnt1) < Precision::Confusion()) {
                adjacentEdge11 = edge;
            }
            else if (firstPnt.Distance(pnt2) < Precision::Confusion() || lastPnt.Distance(pnt2) < Precision::Confusion()) {
                adjacentEdge12 = edge;
            }
        }
        if (adjacentEdge11.IsNull() || adjacentEdge12.IsNull()) return false;

        /*@}*/

        /** 第二条曲面端点 */
        /*@{*/
        auto surfaceCmd2 = geoCmdList->getDataByID(m_SourceSurface2.CmdId);
        if (surfaceCmd2 == nullptr) return false;
        auto faceVshape2 = surfaceCmd2->getShapeT<FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSFace, m_SourceSurface2.VirtualTopoId);
        auto edgeVShape2 = surfaceCmd2->getShapeT<FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSEdge, m_EdgeVirtualTopoId2);
        if (faceVshape2 == nullptr || edgeVShape2 == nullptr) return false;
        TopoDS_Shape faceShape2 = faceVshape2->getTopoShape();
        if (faceShape2.ShapeType() != TopAbs_ShapeEnum::TopAbs_FACE) return false;
        TopoDS_Face face2 = TopoDS::Face(faceShape2);
        if (face2.IsNull()) return false;

        TopoDS_Shape edgeShape2 = edgeVShape2->getTopoShape();
        if (edgeShape2.ShapeType() != TopAbs_ShapeEnum::TopAbs_EDGE) return false;
        TopoDS_Edge edge2 = TopoDS::Edge(edgeShape2);
        if (edge2.IsNull()) return false;

        Standard_Real first2, last2;
        Handle(Geom_Curve) curve2 = BRep_Tool::Curve(edge2, first2, last2);

        gp_Pnt pnt3 = curve2->Value(first2);
        gp_Pnt pnt4 = curve2->Value(last2);

        TopoDS_Edge adjacentEdge21;
        TopoDS_Edge adjacentEdge22;
        // 邻接边
        TopExp_Explorer exp2;
        for (exp2.Init(face2, TopAbs_ShapeEnum::TopAbs_EDGE); exp2.More(); exp2.Next()) {
            TopoDS_Edge edge = TopoDS::Edge(exp2.Current());
            if (edge.IsNull()) return false;
            Standard_Real first, last;
            Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, first, last);

            gp_Pnt firstPnt = curve->Value(first);
            gp_Pnt lastPnt = curve->Value(last);

            // 忽略要桥接的边
            if (
                (firstPnt.Distance(pnt3) < Precision::Confusion() && lastPnt.Distance(pnt4) < Precision::Confusion())
                || (firstPnt.Distance(pnt4) < Precision::Confusion() && lastPnt.Distance(pnt3) < Precision::Confusion())) {
                continue;
            }

            if (firstPnt.Distance(pnt3) < Precision::Confusion() || lastPnt.Distance(pnt3) < Precision::Confusion()) {
                adjacentEdge21 = edge;
            }
            else if (firstPnt.Distance(pnt4) < Precision::Confusion() || lastPnt.Distance(pnt4) < Precision::Confusion()) {
                adjacentEdge22 = edge;
            }
        }
        if (adjacentEdge21.IsNull() || adjacentEdge22.IsNull()) return false;
        /*@}*/

        /* 桥接Pnt1和Pnt3的样条曲线 */
        /*@{*/
        // 取Pnt1附近的点
        Standard_Real first, last;
        Handle(Geom_Curve) curve11 = BRep_Tool::Curve(adjacentEdge11, first, last);
        bool pnt1IsStart = curve11->Value(first).Distance(pnt1) < Precision::Confusion();
        gp_Vec vec11 = pnt1IsStart ? curve11->DN(first, 1) : curve11->DN(last, 1);
        //vec11.Scale(m_Parameter2 / last11.Magnitude());
        gp_Pnt adjustPnt1;
        if (pnt1IsStart)
            adjustPnt1 = gp_Pnt(pnt1.X() - vec11.X(), pnt1.Y() - vec11.Y(), pnt1.Z() - vec11.Z());
        else
            adjustPnt1 = gp_Pnt(pnt1.X() + vec11.X(), pnt1.Y() + vec11.Y(), pnt1.Z() + vec11.Z());
        // 取Pnt3附近的点
        Handle(Geom_Curve) curve22 = BRep_Tool::Curve(adjacentEdge21, first, last);
        bool pnt3IsStart = curve22->Value(first).Distance(pnt1) < Precision::Confusion();
        gp_Vec vec22 = pnt3IsStart ? curve22->DN(first, 1) : curve11->DN(last, 1);
        //vec11.Scale(m_Parameter2 / last11.Magnitude());
        gp_Pnt adjustPnt3;
        if (pnt3IsStart)
            adjustPnt3 = gp_Pnt(pnt3.X() - vec11.X(), pnt3.Y() - vec11.Y(), pnt3.Z() - vec11.Z());
        else
            adjustPnt3 = gp_Pnt(pnt3.X() + vec11.X(), pnt3.Y() + vec11.Y(), pnt3.Z() + vec11.Z());
        // 构造样条
        TColgp_Array1OfPnt pnts13(1, 4);
        pnts13.SetValue(1, pnt1);
        pnts13.SetValue(2, adjustPnt1);
        pnts13.SetValue(3, adjustPnt3);
        pnts13.SetValue(4, pnt3);

        Standard_Integer KNum13 = 3;
        TColStd_Array1OfReal knots13(1, KNum13);
        for (int i = 0; i < KNum13; ++i)
        {
            knots13.SetValue(i + 1, i);
        }
        TColStd_Array1OfInteger mults13(1, KNum13);
        for (int i = 0; i < KNum13; ++i)
        {
            if (i == 0 || i == KNum13 - 1)
            {
                mults13.SetValue(i + 1, 3);
            }
            else
            {
                mults13.SetValue(i + 1, 1);
            }
        }

        Handle(Geom_BSplineCurve) hCurve13 = new Geom_BSplineCurve(pnts13, knots13, mults13, 2);
        GeomLib_Tool parameterTool;
        Standard_Real p1, p3;
        parameterTool.Parameter(hCurve13, pnt1, Precision::Confusion(), p1);
        parameterTool.Parameter(hCurve13, pnt3, Precision::Confusion(), p3);
        BRepBuilderAPI_MakeEdge anEdge13(hCurve13, p1, p3);
        if (!anEdge13.IsDone()) return false;
        /*@}*/


        /* 桥接Pnt2和Pnt4的样条曲线 */
        /*@{*/
        // 取Pnt2附近的点
        curve11 = BRep_Tool::Curve(adjacentEdge12, first, last);
        bool pnt2IsStart = curve11->Value(first).Distance(pnt2) < Precision::Confusion();
        vec11 = pnt2IsStart ? curve11->DN(first, 1) : curve11->DN(last, 1);
        //vec11.Scale(m_Parameter2 / last11.Magnitude());
        gp_Pnt adjustPnt2;
        if (pnt2IsStart)
            adjustPnt2 = gp_Pnt(pnt2.X() - vec11.X(), pnt2.Y() - vec11.Y(), pnt2.Z() - vec11.Z());
        else
            adjustPnt2 = gp_Pnt(pnt2.X() + vec11.X(), pnt2.Y() + vec11.Y(), pnt2.Z() + vec11.Z());
        // 取Pnt4附近的点
        curve22 = BRep_Tool::Curve(adjacentEdge22, first, last);
        bool pnt4IsStart = curve22->Value(first).Distance(pnt4) < Precision::Confusion();
        vec22 = pnt4IsStart ? curve22->DN(first, 1) : curve11->DN(last, 1);
        //vec11.Scale(m_Parameter2 / last11.Magnitude());
        gp_Pnt adjustPnt4;
        if (pnt4IsStart)
            adjustPnt4 = gp_Pnt(pnt4.X() - vec11.X(), pnt4.Y() - vec11.Y(), pnt4.Z() - vec11.Z());
        else
            adjustPnt4 = gp_Pnt(pnt4.X() + vec11.X(), pnt4.Y() + vec11.Y(), pnt4.Z() + vec11.Z());
        // 构造样条
        TColgp_Array1OfPnt pnts24(1, 4);
        pnts24.SetValue(1, pnt2);
        pnts24.SetValue(2, adjustPnt2);
        pnts24.SetValue(3, adjustPnt4);
        pnts24.SetValue(4, pnt4);

        Standard_Integer KNum24 = 3;
        TColStd_Array1OfReal knots24(1, KNum24);
        for (int i = 0; i < KNum24; ++i)
        {
            knots24.SetValue(i + 1, i);
        }
        TColStd_Array1OfInteger mults24(1, KNum24);
        for (int i = 0; i < KNum24; ++i)
        {
            if (i == 0 || i == KNum24 - 1)
            {
                mults24.SetValue(i + 1, 3);
            }
            else
            {
                mults24.SetValue(i + 1, 1);
            }
        }

        Handle(Geom_BSplineCurve) hCurve24 = new Geom_BSplineCurve(pnts24, knots24, mults24, 2);
        Standard_Real p2, p4;
        parameterTool.Parameter(hCurve24, pnt2, Precision::Confusion(), p2);
        parameterTool.Parameter(hCurve24, pnt4, Precision::Confusion(), p4);
        BRepBuilderAPI_MakeEdge anEdge24(hCurve24, p2, p4);
        if (!anEdge24.IsDone()) return false;
        /*@}*/

        BRepFill_Filling fillFace;
        fillFace.Add(edge1, GeomAbs_Shape::GeomAbs_C0, true);
        fillFace.Add(anEdge13, GeomAbs_Shape::GeomAbs_C0, true);
        fillFace.Add(edge2, GeomAbs_Shape::GeomAbs_C0, true);
        fillFace.Add(anEdge24, GeomAbs_Shape::GeomAbs_C0, true);

        try
        {
            fillFace.Build();
            if (!fillFace.IsDone()) return false;
            _occShapeAgent->updateShape(fillFace.Face());
            return true;
        }
        catch (...)
        {
            printLog("Failed to make face.", 3);
            return false;
        }


        /*Handle(GeomAdaptor_HCurve) SPL1Adaptor = new GeomAdaptor_HCurve(curve1);
        Handle(GeomFill_SimpleBound) B1 = new GeomFill_SimpleBound(SPL1Adaptor, Precision::Approximation(), Precision::Angular());

        Handle(GeomAdaptor_HCurve) SPL2Adaptor = new GeomAdaptor_HCurve(hCurve13);
        Handle(GeomFill_SimpleBound) B2 = new GeomFill_SimpleBound(SPL2Adaptor, Precision::Approximation(), Precision::Angular());

        Handle(GeomAdaptor_HCurve) SPL3Adaptor = new GeomAdaptor_HCurve(curve2);
        Handle(GeomFill_SimpleBound) B3 = new GeomFill_SimpleBound(SPL3Adaptor, Precision::Approximation(), Precision::Angular());

        Handle(GeomAdaptor_HCurve) SPL4Adaptor = new GeomAdaptor_HCurve(hCurve24);
        Handle(GeomFill_SimpleBound) B4 = new GeomFill_SimpleBound(SPL4Adaptor, Precision::Approximation(), Precision::Angular());

        int MaxDeg = 5;
        int MaxSeg = 2;
        GeomFill_ConstrainedFilling aConstrainedFilling(MaxDeg, MaxSeg);
        aConstrainedFilling.Init(B1, B2, B3, B4, false);

        Handle(Geom_BSplineSurface) aBSplineSurface = aConstrainedFilling.Surface();
        auto face = BRepBuilderAPI_MakeFace(aBSplineSurface, Precision::Approximation());



        _occShapeAgent->updateShape(face.Shape());

        _occShapeAgent->writeToFile("d:\\123.stp");
        _occShapeAgent->writeSTLFile("d:\\123.stl");

        return false;*/

        /*TColgp_Array2OfPnt pnts(1, 2, 1, 2);
        pnts.SetValue(1, 1, pnt1);
        pnts.SetValue(2, 1, pnt2);
        pnts.SetValue(1, 2, pnt3);
        pnts.SetValue(2, 2, pnt4);

        try {
            GeomAPI_PointsToBSplineSurface splineSurface(pnts);
            if (splineSurface.IsDone())
            {
                Handle(Geom_BSplineSurface) hSurface = splineSurface.Surface();

                BRepBuilderAPI_MakeFace anFace(hSurface, first1, last1, first2, last2, Precision::Confusion());

                _occShapeAgent->updateShape(anFace.Shape());
                return true;
            }
            else return false;
        }
        catch (...)
        {
            printLog(tr("Failed to make face!"), 3);
            return false;
        }*/
        /*Handle(Geom_BezierCurve) hCurve = new Geom_BezierCurve(pnts);
        GeomLib_Tool parameterTool;
        Standard_Real p1, p4;
        parameterTool.Parameter(hCurve, pnt1, Precision::Confusion(), p1);
        parameterTool.Parameter(hCurve, pnt4, Precision::Confusion(), p4);
        BRepBuilderAPI_MakeEdge anEdge(hCurve, p1, p4);
        _occShapeAgent->updateShape(anEdge.Shape());
        return true;*/
    }

    FITKOCCModelSolidSurface::FITKOCCModelSolidSurface() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelSolidSurface::update()
    {
        if (m_SourceSolid.isNull() || m_FaceVirtualTopoId < 0) return false;
        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        /** 获取原始体形状 */
         /*@{*/
        auto solidCmd = geoCmdList->getDataByID(m_SourceSolid.CmdId);
        if (solidCmd == nullptr) return false;
        auto faceVShape = solidCmd->getShapeT<FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSFace, m_FaceVirtualTopoId);
        if (faceVShape == nullptr) return false;
        TopoDS_Shape faceShape = faceVShape->getTopoShape();
        if (faceShape.ShapeType() != TopAbs_ShapeEnum::TopAbs_FACE) return false;
        TopoDS_Face face = TopoDS::Face(faceShape);
        if (face.IsNull()) return false;
        /*@}*/

        /*if (face.IsNull()) return false;

        Handle(Geom_Surface) surface = BRep_Tool::Surface(face);
        if (surface.IsNull()) return false;

        BRepBuilderAPI_MakeFace anFace(surface, BRep_Tool::Tolerance(face));


        if (!anFace.IsDone()) {
            switch (anFace.Error())
            {
            case BRepBuilderAPI_FaceDone:
                printLog("Face Done", 3);
                break;
            case BRepBuilderAPI_NoFace:
                printLog("No Face", 3);
                break;
            case BRepBuilderAPI_NotPlanar:
                printLog("Not Planar", 3);
                break;
            case BRepBuilderAPI_CurveProjectionFailed:
                printLog("Curve Projection Failed", 3);
                break;
            case BRepBuilderAPI_ParametersOutOfRange:
                printLog("Parameters Out Of Range", 3);
                break;
            default:
                break;
            }
        }
        if (anFace.Shape().IsNull()) {
            printLog("Failed to make face.", 3);
            return false;
        }*/
        _occShapeAgent->updateShape(face);
        return true;
    }



}
