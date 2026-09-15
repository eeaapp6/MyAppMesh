/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCModelPartitionSolid.h"

// OCC base
#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_Compound.hxx>
#include <TopoDS_Builder.hxx>
#include <BRepTools_ReShape.hxx>
#include <BOPAlgo_Splitter.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <gp_Pln.hxx>
#include <BRep_Tool.hxx>
#include <Geom_Surface.hxx>
#include <BRepOffsetAPI_MakePipe.hxx>
#include <Geom_Curve.hxx>
#include <Geom_Line.hxx>
#include <BRep_Tool.hxx>

// App
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

// Geometry
#include "FITKOCCVirtualTopoCreator.h"
#include "FITKOCCModelPart.h"
#include "FITKOCCComandCommon.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoEnum.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKVirtualTopoManager.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoSketch2D.h"

#include <QDebug>

namespace OCC
{
    // 平面分割实体。
    //@{
    FITKOCCModelPartitionSolidWithPlane::FITKOCCModelPartitionSolidWithPlane() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelPartitionSolidWithPlane::update()
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

        if (m_Solids.isEmpty())
        {
            return false;
        }

        // 从原始形状移除实体。
        TopoDS_Builder builder;
        TopoDS_Compound compound;
        builder.MakeCompound(compound);

        Handle(BRepTools_ReShape) reShape = new BRepTools_ReShape;

        QList<const TopoDS_Shape*> fullShapes;
        QList<TopoDS_Shape> solidShapes;
        for (const Interface::VirtualShape & solid : m_Solids)
        {
            Interface::FITKAbsGeoCommand* cmd = geoCmdList->getDataByID(solid.CmdId);
            if (!cmd)
            {
                return false;
            }

            FITKAbstractOCCModel* occModel = cmd->getTShapeAgent<FITKAbstractOCCModel>();
            if (!occModel)
            {
                return false;
            }

            const TopoDS_Shape* topoShape = occModel->getShape();

            FITKOCCTopoShape* occSolid = cmd->getShapeTopoByIndexT<FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSSolid, solid.VirtualTopoIndex);
            if (!occSolid)
            {
                return false;
            }

            const TopoDS_Shape & solidShape = occSolid->getTopoShape();
            reShape->Remove(solidShape);

            if (!fullShapes.contains(topoShape))
            {
                fullShapes.push_back(topoShape);
            }

            solidShapes.push_back(solidShape);
        }

        if (solidShapes.isEmpty())
        {
            return false;
        }

        TopoDS_Shape resultShape;

        try
        {
            // 创建平面。
            gp_Pln pln(gp_Pnt(m_Origin[0], m_Origin[1], m_Origin[2]), gp_Dir(m_Normal[0], m_Normal[1], m_Normal[2]));
            const TopoDS_Face & face = BRepBuilderAPI_MakeFace(pln);
            if (face.IsNull())
            {
                return false;
            }

            // 获取移除体后的形状进行拼接。
            int nComps = 0;
            for (const TopoDS_Shape* topoShape : fullShapes)
            {
                //TopAbs_ShapeEnum sType = topoShape->ShapeType();
                TopoDS_Shape newShape = reShape->Apply(*topoShape);

                if (newShape.IsNull())
                {
                    continue;

                    //// 排除自身为体的情况。
                    //if (sType == TopAbs_ShapeEnum::TopAbs_SOLID)
                    //{
                    //    continue;
                    //}
                    //else
                    //{
                    //    continue;
                    //    // return false;
                    //}
                }

                builder.Add(compound, newShape);
                nComps++;
            }

            // 分割体。
            for (const TopoDS_Shape & shape : solidShapes)
            {
                BOPAlgo_Splitter splitter;
                splitter.AddArgument(shape);
                splitter.AddTool(face);
                splitter.Perform();

                resultShape = splitter.Shape();
                if (resultShape.IsNull())
                {
                    return false;
                }

                builder.Add(compound, resultShape);
                nComps++;
            }

            if (nComps != 1)
            {
                resultShape = compound;
            }
        }
        catch (...)
        {
            return false;
        }

        part->updatePartShape(resultShape, false);
        _occShapeAgent->updateShape(part->getCopiedPartShape(), false);

        return true;
    }
    //@}

    // 延伸面分割实体。
    //@{
    FITKOCCModelPartitionSolidWithExtendFace::FITKOCCModelPartitionSolidWithExtendFace() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelPartitionSolidWithExtendFace::update()
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

        if (m_Solids.isEmpty())
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

        // 从原始形状移除实体。
        TopoDS_Builder builder;
        TopoDS_Compound compound;
        builder.MakeCompound(compound);

        Handle(BRepTools_ReShape) reShape = new BRepTools_ReShape;

        QList<const TopoDS_Shape*> fullShapes;
        QList<TopoDS_Shape> solidShapes;
        for (const Interface::VirtualShape & solid : m_Solids)
        {
            Interface::FITKAbsGeoCommand* cmd = geoCmdList->getDataByID(solid.CmdId);
            if (!cmd)
            {
                return false;
            }

            FITKAbstractOCCModel* occModel = cmd->getTShapeAgent<FITKAbstractOCCModel>();
            if (!occModel)
            {
                return false;
            }

            const TopoDS_Shape* topoShape = occModel->getShape();

            FITKOCCTopoShape* occSolid = cmd->getShapeTopoByIndexT<FITKOCCTopoShape>
                (Interface::FITKGeoEnum::VTopoShapeType::VSSolid, solid.VirtualTopoIndex);
            if (!occSolid)
            {
                return false;
            }

            const TopoDS_Shape & solidShape = occSolid->getTopoShape();
            reShape->Remove(solidShape);

            if (!fullShapes.contains(topoShape))
            {
                fullShapes.push_back(topoShape);
            }

            solidShapes.push_back(solidShape);
        }

        if (solidShapes.isEmpty())
        {
            return false;
        }

        TopoDS_Shape resultShape;

        try
        {
            // 创建面。
            const TopoDS_Face & face = BRepBuilderAPI_MakeFace(surface, 1e-6);
            if (face.IsNull())
            {
                return false;
            }

            // 获取移除体后的形状进行拼接。
            int nComps = 0;
            for (const TopoDS_Shape* topoShape : fullShapes)
            {
                //TopAbs_ShapeEnum sType = topoShape->ShapeType();
                TopoDS_Shape newShape = reShape->Apply(*topoShape);

                if (newShape.IsNull())
                {
                    continue;

                    //// 排除自身为体的情况。
                    //if (sType == TopAbs_ShapeEnum::TopAbs_SOLID)
                    //{
                    //    continue;
                    //}
                    //else
                    //{
                    //    continue;
                    //    // return false;
                    //}
                }

                builder.Add(compound, newShape);
                nComps++;
            }

            // 分割体。
            for (const TopoDS_Shape & shape : solidShapes)
            {
                BOPAlgo_Splitter splitter;
                splitter.AddArgument(shape);
                splitter.AddTool(face);
                splitter.Perform();

                resultShape = splitter.Shape();
                if (resultShape.IsNull())
                {
                    return false;
                }

                builder.Add(compound, resultShape);
                nComps++;
            }

            if (nComps != 1)
            {
                resultShape = compound;
            }
        }
        catch (...)
        {
            return false;
        }

        part->updatePartShape(resultShape, false);
        _occShapeAgent->updateShape(part->getCopiedPartShape(), false);

        return true;
    }
    //@}

    // 扫掠线分割实体。
    //@{
    FITKOCCModelPartitionSolidWithSweepEdge::FITKOCCModelPartitionSolidWithSweepEdge() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelPartitionSolidWithSweepEdge::update()
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

        // 获取包围盒计算扫掠距离。
        double min3[3], max3[3];
        bool flag = partShapeAgent->getBoundaryBox(min3, max3);
        if (!flag)
        {
            return false;
        }

        double len = sqrt(pow(max3[0] - min3[0], 2) + pow(max3[1] - min3[1], 2) + pow(max3[2] - min3[2], 2));
        if (qFuzzyIsNull(len))
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

        if (m_Solids.isEmpty())
        {
            return false;
        }

        // 处理扫掠方向。
        double dir3[3]{ 0., 0., 0. };
        if (!m_SweepPath.isNull())
        {
            Interface::FITKAbsGeoCommand* cmd = geoCmdList->getDataByID(m_SweepPath.CmdId);
            if (!cmd)
            {
                return false;
            }

            FITKOCCTopoShape* occEdge = cmd->getShapeTopoByIndexT<FITKOCCTopoShape>
                (Interface::FITKGeoEnum::VTopoShapeType::VSEdge, m_SweepPath.VirtualTopoIndex);
            if (!occEdge)
            {
                return false;
            }

            // 获取曲线方向。
            const TopoDS_Edge & topoEdge = TopoDS::Edge(occEdge->getTopoShape());

            Standard_Real first, last;
            Handle(Geom_Line) cLine = Handle(Geom_Line)::DownCast(BRep_Tool::Curve(topoEdge, first, last));
            if (cLine.IsNull())
            {
                return false;
            }

            const gp_Dir & dir = cLine->Lin().Direction();
            for (int i = 0; i < 3; i++)
            {
                dir3[i] = dir.Coord(i + 1);
            }
        }
        else if (m_SweepDirection.count() == 3)
        {
            for (int i = 0; i < 3; i++)
            {
                dir3[i] = m_SweepDirection[i];
            }
        }
        else
        {
            return false;
        }

        QList<TopoDS_Shape> edges;
        for (const Interface::VirtualShape & edge : m_Edges)
        {
            Interface::FITKAbsGeoCommand* cmd = geoCmdList->getDataByID(edge.CmdId);
            if (!cmd)
            {
                return false;
            }

            FITKOCCTopoShape* occEdge = cmd->getShapeTopoByIndexT<FITKOCCTopoShape>
                (Interface::FITKGeoEnum::VTopoShapeType::VSEdge, edge.VirtualTopoIndex);
            if (!occEdge)
            {
                return false;
            }

            const TopoDS_Shape & topoEdge = occEdge->getTopoShape();
            edges.push_back(topoEdge);
        }

        // 从原始形状移除实体。
        TopoDS_Builder builder;
        TopoDS_Compound compound;
        builder.MakeCompound(compound);

        Handle(BRepTools_ReShape) reShape = new BRepTools_ReShape;

        QList<const TopoDS_Shape*> fullShapes;
        QList<TopoDS_Shape> solidShapes;
        for (const Interface::VirtualShape & solid : m_Solids)
        {
            Interface::FITKAbsGeoCommand* cmd = geoCmdList->getDataByID(solid.CmdId);
            if (!cmd)
            {
                return false;
            }

            FITKAbstractOCCModel* occModel = cmd->getTShapeAgent<FITKAbstractOCCModel>();
            if (!occModel)
            {
                return false;
            }

            const TopoDS_Shape* topoShape = occModel->getShape();

            FITKOCCTopoShape* occSolid = cmd->getShapeTopoByIndexT<FITKOCCTopoShape>
                (Interface::FITKGeoEnum::VTopoShapeType::VSSolid, solid.VirtualTopoIndex);
            if (!occSolid)
            {
                return false;
            }

            const TopoDS_Shape & solidShape = occSolid->getTopoShape();
            reShape->Remove(solidShape);

            if (!fullShapes.contains(topoShape))
            {
                fullShapes.push_back(topoShape);
            }

            solidShapes.push_back(solidShape);
        }

        if (solidShapes.isEmpty())
        {
            return false;
        }

        TopoDS_Shape resultShape;

        try
        {
            // 获取移除体后的形状进行拼接。
            int nComps = 0;
            for (const TopoDS_Shape* topoShape : fullShapes)
            {
                //TopAbs_ShapeEnum sType = topoShape->ShapeType();
                TopoDS_Shape newShape = reShape->Apply(*topoShape);

                if (newShape.IsNull())
                {
                    continue;

                    //// 排除自身为体的情况。
                    //if (sType == TopAbs_ShapeEnum::TopAbs_SOLID)
                    //{
                    //    continue;
                    //}
                    //else
                    //{
                    //    continue;
                    //    // return false;
                    //}
                }

                builder.Add(compound, newShape);
                nComps++;
            }

            // 创建扫掠面。
            QList<TopoDS_Shape> faces;
            for (const TopoDS_Shape & edge : edges)
            {
                const TopoDS_Edge & e = TopoDS::Edge(edge);
                if (e.IsNull())
                {
                    continue;
                }

                // 获取线上一点，根据方向与距离计算令一点坐标作为扫掠线。
                TopoDS_Vertex vert = TopExp::FirstVertex(e);
                gp_Pnt pos = BRep_Tool::Pnt(vert);
                gp_Pnt pos2(pos.Coord(1) + dir3[0] * len, pos.Coord(2) + dir3[1] * len, pos.Coord(3) + dir3[2] * len);

                const TopoDS_Edge & sweepEdge = BRepBuilderAPI_MakeEdge(pos, pos2);
                if (sweepEdge.IsNull())
                {
                    continue;
                }

                const TopoDS_Wire & sweepWire = BRepBuilderAPI_MakeWire(sweepEdge);
                if (sweepWire.IsNull())
                {
                    continue;
                }

                const TopoDS_Shape & face = BRepOffsetAPI_MakePipe(sweepWire, edge);               
                if (face.IsNull())
                {
                    continue;
                }

                faces.push_back(face);
            }

            // 分割体。
            for (const TopoDS_Shape & shape : solidShapes)
            {
                BOPAlgo_Splitter splitter;
                splitter.AddArgument(shape);

                for (const TopoDS_Shape & face : faces)
                {
                    splitter.AddTool(face);
                }

                splitter.Perform();

                resultShape = splitter.Shape();
                if (resultShape.IsNull())
                {
                    return false;
                }

                builder.Add(compound, resultShape);
                nComps++;
            }

            if (nComps != 1)
            {
                resultShape = compound;
            }
        }
        catch (...)
        {
            return false;
        }

        part->updatePartShape(resultShape, false);
        _occShapeAgent->updateShape(part->getCopiedPartShape(), false);

        return true;
    }
    //@}

    // 草图面分割实体。
    //@{
    FITKOCCModelPartitionSolidWithSketchPlanar::FITKOCCModelPartitionSolidWithSketchPlanar() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelPartitionSolidWithSketchPlanar::update()
    {
        FITKOCCModelPart* part = _occShapeAgent->getPart();
        if (!part || m_Sketch.isNull())
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

        if (m_Solids.isEmpty())
        {
            return false;
        }

        Interface::FITKAbsGeoSketch2D* cmd = dynamic_cast<Interface::FITKAbsGeoSketch2D*>(geoCmdList->getDataByID(m_Sketch.CmdId));
        if (!cmd)
        {
            return false;
        }

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

        double pos3[3], nor3[3], up3[3];
        cmd->getPlane(pos3, nor3, up3);
        gp_Pln pln(gp_Pnt(pos3[0], pos3[1], pos3[2]), gp_Dir(nor3[0], nor3[1], nor3[2]));

        QList<EncloseWire> wiresList;
        QList<TopoDS_Face> faces;
        FITKOCCComandCommon::MakeWires(*sketchShape, wiresList, pln, 1e-5);
        FITKOCCComandCommon::MakeFaces(wiresList, faces);

        // 从原始形状移除实体。
        TopoDS_Builder builder;
        TopoDS_Compound compound;
        builder.MakeCompound(compound);

        Handle(BRepTools_ReShape) reShape = new BRepTools_ReShape;

        QList<const TopoDS_Shape*> fullShapes;
        QList<TopoDS_Shape> solidShapes;
        for (const Interface::VirtualShape & solid : m_Solids)
        {
            Interface::FITKAbsGeoCommand* cmd = geoCmdList->getDataByID(solid.CmdId);
            if (!cmd)
            {
                return false;
            }

            FITKAbstractOCCModel* occModel = cmd->getTShapeAgent<FITKAbstractOCCModel>();
            if (!occModel)
            {
                return false;
            }

            const TopoDS_Shape* topoShape = occModel->getShape();

            FITKOCCTopoShape* occSolid = cmd->getShapeTopoByIndexT<FITKOCCTopoShape>
                (Interface::FITKGeoEnum::VTopoShapeType::VSSolid, solid.VirtualTopoIndex);
            if (!occSolid)
            {
                return false;
            }

            const TopoDS_Shape & solidShape = occSolid->getTopoShape();
            reShape->Remove(solidShape);

            if (!fullShapes.contains(topoShape))
            {
                fullShapes.push_back(topoShape);
            }

            solidShapes.push_back(solidShape);
        }

        if (solidShapes.isEmpty())
        {
            return false;
        }

        TopoDS_Shape resultShape;

        try
        {
            // 获取移除体后的形状进行拼接。
            int nComps = 0;
            for (const TopoDS_Shape* topoShape : fullShapes)
            {
                //TopAbs_ShapeEnum sType = topoShape->ShapeType();
                TopoDS_Shape newShape = reShape->Apply(*topoShape);

                if (newShape.IsNull())
                {
                    continue;

                    //// 排除自身为体的情况。
                    //if (sType == TopAbs_ShapeEnum::TopAbs_SOLID)
                    //{
                    //    continue;
                    //}
                    //else
                    //{
                    //    continue;
                    //    // return false;
                    //}
                }

                builder.Add(compound, newShape);
                nComps++;
            }

            // 分割体。
            for (const TopoDS_Shape & shape : solidShapes)
            {
                BOPAlgo_Splitter splitter;
                splitter.AddArgument(shape);

                for (const TopoDS_Shape & face : faces)
                {
                    splitter.AddTool(face);
                }

                splitter.Perform();

                resultShape = splitter.Shape();
                if (resultShape.IsNull())
                {
                    return false;
                }

                builder.Add(compound, resultShape);
                nComps++;
            }

            if (nComps != 1)
            {
                resultShape = compound;
            }
        }
        catch (...)
        {
            return false;
        }

        part->updatePartShape(resultShape, false);
        _occShapeAgent->updateShape(part->getCopiedPartShape(), false);

        return true;
    }
    //@}
}
