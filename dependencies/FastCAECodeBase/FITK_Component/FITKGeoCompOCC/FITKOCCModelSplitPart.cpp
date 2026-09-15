/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCModelSplitPart.h"

#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoEnum.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoPart.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITKAbstractOCCModel.h"
#include "FITKOCCModelPart.h"
#include "FITKOCCComandCommon.h"
#include "FITKOCCExtendTool.h"

// OCC
#include <TopoDS.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Compound.hxx>
#include <TopoDS_Builder.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Vertex.hxx>
#include <gp_Trsf.hxx>
#include <gp_Vec.hxx>
#include <gp_Ax1.hxx>
#include <gp_Dir.hxx>
#include <gp_Pnt.hxx>
#include <gp_Pln.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <BRepBuilderAPI_MakeShape.hxx>
#include <BRep_Tool.hxx>
#include <gp_Ax2.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include <BRepAlgoAPI_Section.hxx>
#include <BOPAlgo_Splitter.hxx>
#include <Geom_Curve.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <Geom_Plane.hxx>
#include <GeomAPI_IntCS.hxx>

namespace OCC
{
    FITKOCCModelSplitPart::FITKOCCModelSplitPart() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelSplitPart::update()
    {
        // 检查源部件ID是否有效。
        if (m_sourcePartId < 0)
        {
            return false;
        }

        // 获取几何命令列表。
        Interface::FITKGeoCommandList* geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        // 根据部件ID获取源部件对象。
        Interface::FITKAbsGeoPart* sourcePart = dynamic_cast<Interface::FITKAbsGeoPart*>(geoCmdList->getDataByID(m_sourcePartId));
        if (!sourcePart)
        {
            return false;
        }

        // 获取源部件的OCC模型代理。
        FITKAbstractOCCModel* occModel = sourcePart->getTShapeAgent<FITKAbstractOCCModel>();
        if (!occModel)
        {
            return false;
        }

        // 获取OCC形状数据。
        TopoDS_Shape* shape = occModel->getShape();
        if (!shape || shape->IsNull())
        {
            return false;
        }

        // 获取当前部件对象。
        FITKOCCModelPart* part = _occShapeAgent->getPart();

        try
        {
            // 创建分割平面原点。
            gp_Pnt splitOrigin(m_origin[0], m_origin[1], m_origin[2]);

            // 根据平面类型创建分割平面法向。
            gp_Dir planeNormal;
            if (m_planeType == Interface::FITKGeoEnum::PT_XY)
            {
                // XY平面的法向为Z轴。
                planeNormal = gp_Dir(0, 0, 1);
            }
            else if (m_planeType == Interface::FITKGeoEnum::PT_XZ)
            {
                // XZ平面的法向为Y轴。
                planeNormal = gp_Dir(0, 1, 0);
            }
            else if (m_planeType == Interface::FITKGeoEnum::PT_YZ)
            {
                // YZ平面的法向为X轴。
                planeNormal = gp_Dir(1, 0, 0);
            }

            // 将角度转换为弧度。
            double rotX = m_rotateX * M_PI / 180.0;
            double rotY = m_rotateY * M_PI / 180.0;
            double rotZ = m_rotateZ * M_PI / 180.0;

            // 创建平面。
            gp_Pln splitPlane(splitOrigin, planeNormal);
            TopoDS_Face splitFace = BRepBuilderAPI_MakeFace(splitPlane);
            if (splitFace.IsNull())
            {
                printLog(tr("Failed to create split plane!"), 3);
                return false;
            }

            // 对平面进行旋转。
            TopoDS_Shape finalPlane = splitFace;
            if (m_planeType == Interface::FITKGeoEnum::PT_XY)
            {
                // XY平面可以绕X轴和Y轴旋转。
                if (!qFuzzyIsNull(rotX))
                {
                    gp_Ax1 axisX(splitOrigin, gp_Dir(1, 0, 0));
                    gp_Trsf trsfX;
                    trsfX.SetRotation(axisX, rotX);
                    BRepBuilderAPI_Transform transformX(finalPlane, trsfX, true);
                    finalPlane = transformX.Shape();
                }

                if (!qFuzzyIsNull(rotY))
                {
                    gp_Ax1 axisY(splitOrigin, gp_Dir(0, 1, 0));
                    gp_Trsf trsfY;
                    trsfY.SetRotation(axisY, rotY);
                    BRepBuilderAPI_Transform transformY(finalPlane, trsfY, true);
                    finalPlane = transformY.Shape();
                }
            }
            else if (m_planeType == Interface::FITKGeoEnum::PT_XZ)
            {
                // XZ平面可以绕X轴和Z轴旋转。
                if (!qFuzzyIsNull(rotX))
                {
                    gp_Ax1 axisX(splitOrigin, gp_Dir(1, 0, 0));
                    gp_Trsf trsfX;
                    trsfX.SetRotation(axisX, rotX);
                    BRepBuilderAPI_Transform transformX(finalPlane, trsfX, true);
                    finalPlane = transformX.Shape();
                }

                if (!qFuzzyIsNull(rotZ))
                {
                    gp_Ax1 axisZ(splitOrigin, gp_Dir(0, 0, 1));
                    gp_Trsf trsfZ;
                    trsfZ.SetRotation(axisZ, rotZ);
                    BRepBuilderAPI_Transform transformZ(finalPlane, trsfZ, true);
                    finalPlane = transformZ.Shape();
                }
            }
            else if (m_planeType == Interface::FITKGeoEnum::PT_YZ)
            {
                // YZ平面可以绕Y轴和Z轴旋转。
                if (!qFuzzyIsNull(rotY))
                {
                    gp_Ax1 axisY(splitOrigin, gp_Dir(0, 1, 0));
                    gp_Trsf trsfY;
                    trsfY.SetRotation(axisY, rotY);
                    BRepBuilderAPI_Transform transformY(finalPlane, trsfY, true);
                    finalPlane = transformY.Shape();
                }

                if (!qFuzzyIsNull(rotZ))
                {
                    gp_Ax1 axisZ(splitOrigin, gp_Dir(0, 0, 1));
                    gp_Trsf trsfZ;
                    trsfZ.SetRotation(axisZ, rotZ);
                    BRepBuilderAPI_Transform transformZ(finalPlane, trsfZ, true);
                    finalPlane = transformZ.Shape();
                }
            }

            // 获取分割平面作为工具面。
            TopoDS_Face toolFace = TopoDS::Face(finalPlane);
            if (toolFace.IsNull())
            {
                printLog(tr("Failed to create tool face!"), 3);
                return false;
            }

            // 将模型拆分为多个维度。
            QList<TopoDS_Shape> shapes1D;
            QList<TopoDS_Shape> shapes2D;
            QList<TopoDS_Shape> shapes3D;
            FITKOCCComandCommon::FlattenCompShape2D3D(*shape, shapes1D, shapes2D, shapes3D);

            // 构建结果复合形状。
            TopoDS_Builder builder;
            TopoDS_Compound compound;
            builder.MakeCompound(compound);

            // 使用BOPAlgo_Splitter分割模型，保留分割后的两侧。
            // 处理三维形状（实体）。
            for (const TopoDS_Shape& solidShape : shapes3D)
            {
                if (solidShape.IsNull())
                {
                    continue;
                }

                // 使用BOPAlgo_Splitter进行分割，保留两侧。
                BOPAlgo_Splitter splitter3D;
                splitter3D.AddArgument(solidShape);
                splitter3D.AddTool(toolFace);
                splitter3D.Perform();

                TopoDS_Shape splitResult = splitter3D.Shape();
                if (splitResult.IsNull())
                {
                    builder.Add(compound, solidShape);
                }
                else
                {
                    builder.Add(compound, splitResult);
                }
            }

            // 处理二维形状（面）。
            for (const TopoDS_Shape& faceShape : shapes2D)
            {
                if (faceShape.IsNull())
                {
                    continue;
                }

                // 使用BOPAlgo_Splitter进行分割，保留两侧。
                BOPAlgo_Splitter splitter2D;
                splitter2D.AddArgument(faceShape);
                splitter2D.AddTool(toolFace);
                splitter2D.Perform();

                TopoDS_Shape splitResult = splitter2D.Shape();
                if (splitResult.IsNull())
                {
                    builder.Add(compound, faceShape);
                }
                else
                {
                    builder.Add(compound, splitResult);
                }
            }

            // 处理一维形状（边）。
            for (const TopoDS_Shape& edgeShape : shapes1D)
            {
                if (edgeShape.IsNull())
                {
                    continue;
                }

                // 获取边的曲线信息。
                TopoDS_Edge edge = TopoDS::Edge(edgeShape);
                Standard_Real first, last;
                Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, first, last);
                if (curve.IsNull())
                {
                    // 如果无法获取曲线，保留原边。
                    builder.Add(compound, edgeShape);
                    continue;
                }

                // 创建裁剪曲线。
                Handle(Geom_TrimmedCurve) tCurve = new Geom_TrimmedCurve(curve, first, last);
                if (tCurve.IsNull())
                {
                    builder.Add(compound, edgeShape);
                    continue;
                }

                // 获取平面几何对象。
                Handle(Geom_Plane) planeGeom = Handle(Geom_Plane)::DownCast(BRep_Tool::Surface(toolFace));
                if (planeGeom.IsNull())
                {
                    builder.Add(compound, edgeShape);
                    continue;
                }

                // 计算边与平面的交点。
                GeomAPI_IntCS intCS(tCurve, planeGeom);
                if (!intCS.IsDone())
                {
                    // 如果交点计算失败，保留原边。
                    builder.Add(compound, edgeShape);
                    continue;
                }

                int nPts = intCS.NbPoints();
                if (nPts == 0)
                {
                    // 没有交点，保留原边。
                    builder.Add(compound, edgeShape);
                    continue;
                }

                // 使用BRepAlgoAPI_Section进行边与平面的分割。
                BRepAlgoAPI_Section sectionTool(edge, toolFace, false);
                sectionTool.Approximation(Standard_True);
                sectionTool.Build();

                if (!sectionTool.IsDone())
                {
                    // 分割失败，保留原边。
                    builder.Add(compound, edgeShape);
                    continue;
                }

                TopoDS_Shape sectionResult = sectionTool.Shape();
                if (sectionResult.IsNull())
                {
                    // 分割结果为空，保留原边。
                    builder.Add(compound, edgeShape);
                    continue;
                }

                // 将分割后的边添加到结果中。
                builder.Add(compound, sectionResult);
            }

            // 如果存在部件，则将结果添加到部件中。
            if (part)
            {
                // 添加分割后的新形状。
                part->updatePartShape(compound, false);
                _occShapeAgent->updateShape(part->getCopiedPartShape(), false);
            }
            else
            {
                // 无部件模式，直接更新形状显示。
                _occShapeAgent->updateShape(compound);
            }
        }
        catch (...)
        {
            printLog(tr("Failed to split shape!"), 3);
            return false;
        }

        return true;
    }
}
