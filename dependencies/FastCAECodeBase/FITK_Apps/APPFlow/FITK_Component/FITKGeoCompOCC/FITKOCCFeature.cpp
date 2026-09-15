/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCFeature.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoEnum.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKVirtualTopoManager.h"
#include "FITKOCCVirtualTopoCreator.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelCurve.h"

#include <TopoDS_Shape.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <TopoDS_Wire.hxx>
#include <Geom_Plane.hxx>
#include <BRepFeat_MakeLinearForm.hxx>
#include <TopoDS.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepPrimAPI_MakePrism.hxx>

#include <TopExp_Explorer.hxx>
#include <GeomAPI_IntCS.hxx>
#include <GeomLib_Tool.hxx>
#include <BRepAlgoAPI_Section.hxx>
#include <Geom_Line.hxx>
#include <gp_Lin.hxx>

namespace OCC {

    OCC::FITKOCCStiffener::FITKOCCStiffener() :OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }



    bool OCC::FITKOCCStiffener::update()
    {
        if (m_SourceShape.isNull() || m_ProfileShape.isNull()) return false;

        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        // 源形状
        auto sourceCmd = geoCmdList->getDataByID(m_SourceShape.CmdId);
        if (sourceCmd == nullptr) return false;
        auto sourceShape = sourceCmd->getShapeT<FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSSolid, m_SourceShape.VirtualTopoId);
        if (sourceShape == nullptr) return false;
        TopoDS_Shape sourceTopoShape = sourceShape->getTopoShape();
        if (sourceTopoShape.IsNull()) return false;

        // 截面形状
        auto profileCmd = geoCmdList->getDataByID(m_ProfileShape.CmdId);
        if (profileCmd == nullptr) return false;
        if (profileCmd->getGeometryCommandType() != Interface::FITKGeoEnum::FITKGeometryComType::FGTSketch2D) return false;
        auto sketchCmd = dynamic_cast<Interface::FITKAbsGeoSketch2D*>(profileCmd);
        if (sketchCmd == nullptr || sketchCmd->getDataCount() != 1) return false;
        auto childCmd = sketchCmd->getDataByIndex(0);
        auto segCmd = dynamic_cast<Interface::FITKAbsGeoModelSegment*>(childCmd);
        if (segCmd == nullptr) return false;

        auto start = segCmd->startPoint();
        auto end = segCmd->endPoint();
        BRepBuilderAPI_MakeEdge edge(gp_Pnt(start[0], start[1], start[2]), gp_Pnt(end[0], end[1], end[2]));
        auto profileTopoShape = edge.Shape();
        if (profileTopoShape.IsNull()) return false;

        // Added by ChengHaotian 2024/10/09
        // 求线段与模型的全部交点，计算全部交点U范围将其作为肋板基准线。
        //@{
        double first, last;
        bool flag = getUByCrossPoint(sourceTopoShape, profileTopoShape, first, last);
        if (!flag)
        {
            printLog(tr("Failed to make stiffener."), 3);
            return false;
        }
        //@}

        // 草图
        auto sketch = dynamic_cast<Interface::FITKAbsGeoSketch2D*>(profileCmd);
        if (sketch == nullptr) return false;

        try
        {
            TopoDS_Shape S = sourceTopoShape;
            TopoDS_Wire wire = BRepBuilderAPI_MakeWire(TopoDS::Edge(profileTopoShape));
            double pos[3]{}, normal[3]{};
            sketch->getPosition(pos);
            sketch->getNormal(normal);
            Handle(Geom_Plane) aplane = new Geom_Plane(gp_Pnt(pos[0], pos[1], pos[2]), gp_Vec(normal[0], normal[1], normal[2]));
            gp_Dir dir(normal[0], normal[1], normal[2]);
            gp_Vec vec1 = gp_Vec(dir) * m_Thickness1;
            gp_Vec vec2 = gp_Vec(dir.Reversed()) * m_Thickness2;
            BRepFeat_MakeLinearForm aform(sourceTopoShape, wire, aplane, vec1, vec2, 1, Standard_True);
            aform.Perform();
            _occShapeAgent->updateShape(aform.Shape());
            return true;
        }
        catch (...)
        {
            printLog(tr("Failed to make stiffener."), 3);
            return false;
        }

    }

    bool FITKOCCStiffener::getUByCrossPoint(TopoDS_Shape & shape, TopoDS_Shape & edge, double & u1, double & u2)
    {
        // 求线段与模型的全部交点，计算全部交点U范围。
        //@{
        double first, last;
        const TopoDS_Edge & edgeShape = TopoDS::Edge(edge);
        Handle(Geom_Line) lineFin = Handle(Geom_Line)::DownCast(BRep_Tool::Curve(edgeShape, first, last));
        if (lineFin.IsNull())
        {
            return false;
        }

        const gp_Lin lin = lineFin->Lin();

        gp_Pnt pt = lin.Location();
        gp_Dir dir = lin.Direction();
        const TopoDS_Edge & edgeInf = BRepBuilderAPI_MakeEdge(gp_Lin(pt, dir));
        // Handle(Geom_Curve) lineInf = BRep_Tool::Curve(edgeInf, first, last);

        try
        {
            // 获取体与拾取到有限线段交点。
            int nIntPtsFin = 0;
            const TopoDS_Shape & pointsEdgeFin = BRepAlgoAPI_Section(shape, edge);
            for (TopExp_Explorer iter(pointsEdgeFin, TopAbs_ShapeEnum::TopAbs_VERTEX); iter.More(); iter.Next())
            {
                // 交点计数。
                nIntPtsFin++;
            }
       
            // 有两个交点则不处理。
            //else if (nIntPtsFin == 2)
            //{
            //    return true;
            //}

            // 获取体与无限直线交点。
            int nIntPtsInf = 0;
            double newFirst, newLast;
            const TopoDS_Shape & pointsEdgeInf = BRepAlgoAPI_Section(shape, edgeInf);
            for (TopExp_Explorer iter(pointsEdgeInf, TopAbs_ShapeEnum::TopAbs_VERTEX); iter.More(); iter.Next())
            {
                // 获取交点坐标。
                const TopoDS_Vertex & vertShape = TopoDS::Vertex(iter.Value());
                gp_Pnt crossPt = BRep_Tool::Pnt(vertShape);

                // 获取点在线上的参数值。
                double pu;
                bool flag = GeomLib_Tool::Parameter(lineFin, crossPt, Precision::Confusion(), pu);
                if (!flag)
                {
                    continue;
                }

                if (nIntPtsInf)
                {
                    // 更新范围。
                    newFirst = qMin(newFirst, pu);
                    newLast = qMax(newLast, pu);
                }
                else
                {
                    // 初始化极值范围。
                    newFirst = pu;
                    newLast = pu;
                }

                // 交点计数。
                nIntPtsInf++;
            }

            // 无交点则返回。
            if (nIntPtsInf == 0)
            {
                return false;
            }

            // 重新获取起始与终止点坐标。
            gp_Pnt startPt, endPt;
            lineFin->D0(newFirst + Precision::Confusion(), startPt);
            lineFin->D0(newLast - Precision::Confusion(), endPt);

            // 重新创建线。
            edge = BRepBuilderAPI_MakeEdge(startPt, endPt);

            // 保存起始与终止点参数。
            u1 = newFirst;
            u2 = newLast;
        }
        catch (...)
        {
            return false;
        }
        //@}

        return true;
    }
}