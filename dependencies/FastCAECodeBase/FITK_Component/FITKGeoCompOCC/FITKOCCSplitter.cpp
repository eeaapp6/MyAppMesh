/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCSplitter.h"
#include <TopoDS.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Edge.hxx>
#include <Precision.hxx>
#include <BOPAlgo_Splitter.hxx>
#include <TopExp_Explorer.hxx>
#include <BRep_Builder.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <gp_Pln.hxx>

#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoEnum.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKVirtualTopoManager.h"
#include "FITKOCCVirtualTopoCreator.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelSurface.h"

namespace OCC {

    // 构造函数
    FITKOCCCurveSplitter::FITKOCCCurveSplitter() :OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    // 更新数据
    bool FITKOCCCurveSplitter::update()
    {
        if (m_SourceShape.isNull() || (m_ToolShape.isNull() && m_ToolPoint.size() != 3)) return false;

        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        // 分割算法
        BOPAlgo_Splitter splitter;

        // 设置被分割的对象
        auto sourceCmd = geoCmdList->getDataByID(m_SourceShape.CmdId);
        if (sourceCmd == nullptr) return false;
        auto sourceShape = sourceCmd->getShapeTopoByIndexT<FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSEdge, m_SourceShape.VirtualTopoIndex);
        if (sourceShape == nullptr) return false;
        TopoDS_Shape sourceTopoShape = sourceShape->getTopoShape();
        if (sourceTopoShape.IsNull()) return false;
        splitter.AddArgument(sourceTopoShape);

        TopoDS_Shape toolTopoShape;
        if (!m_ToolShape.isNull()) {
            // 设置分割的工具对象
            auto toolCmd = geoCmdList->getDataByID(m_ToolShape.CmdId);
            // 实体形状
            if (toolCmd == nullptr) {
                // 基准形状
                toolCmd = geoCmdList->getDatumManager()->getDataByID(m_ToolShape.CmdId);
                if (toolCmd == nullptr) return false;
            }
            auto toolShape = toolCmd->getShapeTopoByIndexT<FITKOCCTopoShape>(m_ToolShape.Type, m_ToolShape.VirtualTopoIndex);
            if (toolShape == nullptr) return false;
            toolTopoShape = toolShape->getTopoShape();
        }
        else if (m_ToolPoint.size() == 3) {
            toolTopoShape = BRepBuilderAPI_MakeVertex(gp_Pnt(m_ToolPoint.at(0), m_ToolPoint.at(1), m_ToolPoint.at(2))).Shape();
        }
        else {
            return false;
        }
        if (toolTopoShape.IsNull()) return false;
        splitter.AddTool(toolTopoShape);

        // 更新分割
        try {
            splitter.Perform();

            BRep_Builder aBuilder;
            TopoDS_Compound aCompound;
            aBuilder.MakeCompound(aCompound);

            TopExp_Explorer exp;
            int index = 0;
            for (exp.Init(splitter.Shape(), TopAbs_EDGE); exp.More(); exp.Next())
            {
                aBuilder.Add(aCompound, exp.Current());
            }
            _occShapeAgent->updateShape(aCompound);
            /*if (toolTopoShape.ShapeType() == TopAbs_ShapeEnum::TopAbs_VERTEX) {
                gp_Pnt pnt = BRep_Tool::Pnt(TopoDS::Vertex(toolTopoShape));
                m_Point = { pnt.X(), pnt.Y(), pnt.Z() };
            }*/
        }
        catch (...)
        {
            printLog(tr("Failed to split!"), 3);
            return false;
        }
        return true;
    }

    // 构造函数
    FITKOCCSurfaceSplitter::FITKOCCSurfaceSplitter() :OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    // 更新数据
    bool FITKOCCSurfaceSplitter::update()
    {
        if (m_SourceShape.isNull() || m_ToolShape.isNull()) return false;

        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        // 分割算法
        BOPAlgo_Splitter splitter;

        // 设置被分割的对象
        auto sourceCmd = geoCmdList->getDataByID(m_SourceShape.CmdId);
        if (sourceCmd == nullptr) return false;
        auto sourceShape = sourceCmd->getShapeTopoByIndexT<FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSFace, m_SourceShape.VirtualTopoIndex);
        if (sourceShape == nullptr) return false;
        TopoDS_Shape sourceTopoShape = sourceShape->getTopoShape();
        if (sourceTopoShape.IsNull()) return false;
        splitter.AddArgument(sourceTopoShape);

        // 设置分割的工具对象
        auto toolCmd = geoCmdList->getDataByID(m_ToolShape.CmdId);
        // 实体形状
        if (toolCmd == nullptr) {
            // 基准形状
            toolCmd = geoCmdList->getDatumManager()->getDataByID(m_ToolShape.CmdId);
            if (toolCmd == nullptr) return false;
        }
        auto toolShape = toolCmd->getShapeTopoByIndexT<FITKOCCTopoShape>(m_ToolShape.Type, m_ToolShape.VirtualTopoIndex);
        if (toolShape == nullptr) return false;
        TopoDS_Shape toolTopoShape = toolShape->getTopoShape();
        if (toolTopoShape.IsNull()) return false;
        splitter.AddTool(toolTopoShape);


        // 更新分割
        try {
            splitter.Perform();

            BRep_Builder aBuilder;
            TopoDS_Compound aCompound;
            aBuilder.MakeCompound(aCompound);

            TopExp_Explorer exp;
            int index = 0;
            for (exp.Init(splitter.Shape(), TopAbs_FACE); exp.More(); exp.Next())
            {
                aBuilder.Add(aCompound, exp.Current());
            }
            _occShapeAgent->updateShape(aCompound);
        }
        catch (...)
        {
            printLog(tr("Failed to split!"), 3);
            return false;
        }
        return true;
    }

    // 构造函数
    FITKOCCSolidSplitter::FITKOCCSolidSplitter() :OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    // 更新数据
    bool FITKOCCSolidSplitter::update()
    {
        if (m_SourceShape.isNull() || m_ToolShape.isNull()) return false;

        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        // 分割算法
        BOPAlgo_Splitter splitter;

        // 设置被分割的对象
        auto sourceCmd = geoCmdList->getDataByID(m_SourceShape.CmdId);
        if (sourceCmd == nullptr) return false;
        auto sourceShape = sourceCmd->getShapeTopoByIndexT<FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSSolid, m_SourceShape.VirtualTopoIndex);
        if (sourceShape == nullptr) return false;
        TopoDS_Shape sourceTopoShape = sourceShape->getTopoShape();
        if (sourceTopoShape.IsNull()) return false;
        splitter.AddArgument(sourceTopoShape);

        // 设置分割的工具对象
        auto toolCmd = geoCmdList->getDataByID(m_ToolShape.CmdId);
        // 实体形状
        if (toolCmd == nullptr) {
            // 基准形状
            toolCmd = geoCmdList->getDatumManager()->getDataByID(m_ToolShape.CmdId);
            if (toolCmd == nullptr) return false;
        }
        auto toolShape = toolCmd->getShapeTopoByIndexT<FITKOCCTopoShape>(m_ToolShape.Type, m_ToolShape.VirtualTopoIndex);
        if (toolShape == nullptr) return false;
        TopoDS_Shape toolTopoShape = toolShape->getTopoShape();
        if (toolTopoShape.IsNull()) return false;
        splitter.AddTool(toolTopoShape);


        // 更新分割
        try {
            splitter.Perform();

            BRep_Builder aBuilder;
            TopoDS_Compound aCompound;
            aBuilder.MakeCompound(aCompound);

            TopExp_Explorer exp;
            int index = 0;
            for (exp.Init(splitter.Shape(), TopAbs_SOLID); exp.More(); exp.Next())
            {
                aBuilder.Add(aCompound, exp.Current());
            }
            _occShapeAgent->updateShape(aCompound);
        }
        catch (...)
        {
            printLog(tr("Failed to split!"), 3);
            return false;
        }
        return true;
    }
}


