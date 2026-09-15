/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCModelFilledPolygon.h"

#include "FITKOCCModelPart.h"
#include "FITKOCCExtendTool.h"
#include "FITKOCCComandCommon.h"

// OCC
#include <gp_Pnt.hxx>
#include <BRepBuilderAPI_MakePolygon.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <TopoDS_Builder.hxx>
#include <TopoDS_Compound.hxx>
#include <TopoDS.hxx>

namespace OCC
{
    FITKOCCModelFilledPolygon::FITKOCCModelFilledPolygon() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelFilledPolygon::update()
    {
        FITKOCCModelPart* part = _occShapeAgent->getPart();

        // 参数验证：至少需要3个点
        if (m_Points.size() < 3)
        {
            printLog(tr("Invalid parameters ! At least 3 points are required."), 3);
            return false;
        }

        try
        {
            // 检查点是否共面
            // 使用前三个点计算平面法向，然后检查其他点到该平面的距离
            auto p0 = m_Points.at(0);
            auto p1 = m_Points.at(1);
            auto p2 = m_Points.at(2);

            // 计算两个向量
            double v1x = p1[0] - p0[0];
            double v1y = p1[1] - p0[1];
            double v1z = p1[2] - p0[2];

            double v2x = p2[0] - p0[0];
            double v2y = p2[1] - p0[1];
            double v2z = p2[2] - p0[2];

            // 计算平面法向：N = V1 × V2
            double nx = v1y * v2z - v1z * v2y;
            double ny = v1z * v2x - v1x * v2z;
            double nz = v1x * v2y - v1y * v2x;

            double normalLen = std::sqrt(nx * nx + ny * ny + nz * nz);
            if (normalLen < 1e-9)
            {
                printLog(tr("Invalid parameters ! Points are collinear."), 3);
                return false;
            }

            // 归一化法向
            nx /= normalLen;
            ny /= normalLen;
            nz /= normalLen;

            // 检查其他点到平面的距离
            for (int i = 3; i < m_Points.size(); ++i)
            {
                auto pi = m_Points.at(i);
                double vix = pi[0] - p0[0];
                double viy = pi[1] - p0[1];
                double viz = pi[2] - p0[2];

                // 点到平面的距离 = |V_i · N|
                double dist = std::abs(vix * nx + viy * ny + viz * nz);
                if (dist > 1e-6)
                {
                    printLog(tr("Invalid parameters ! Points are not coplanar."), 3);
                    return false;
                }
            }

            // 创建多边形
            BRepBuilderAPI_MakePolygon polygon;
            for (const auto& point : m_Points)
            {
                polygon.Add(gp_Pnt(point[0], point[1], point[2]));
            }
            polygon.Close();

            if (!polygon.IsDone())
            {
                printLog(tr("Invalid parameters !"), 3);
                return false;
            }

            // 根据是否填充决定创建面还是线框
            TopoDS_Shape output;
            if (m_IsFilled)
            {
                // 创建填充面
                BRepBuilderAPI_MakeFace faceMaker(polygon.Wire());
                if (!faceMaker.IsDone())
                {
                    printLog(tr("Invalid parameters !"), 3);
                    return false;
                }

                output = faceMaker.Shape();
            }
            else
            {
                // 仅创建线框
                output = polygon.Wire();
            }

            if (output.IsNull())
            {
                printLog(tr("Invalid parameters !"), 3);
                return false;
            }

            if (part && part->getAutoBool())
            {
                TopoDS_Shape* partShape{ nullptr };
                FITKAbstractOCCModel* partAgent = part->getTShapeAgent<FITKAbstractOCCModel>();
                if (!partAgent)
                {
                    return false;
                }

                partShape = partAgent->getShape();

                TopoDS_Shape result;
                bool flag = FITKOCCComandCommon::MergeShapes(*partShape, output, result, !part->getAutoMergeSolid());
                if (!flag)
                {
                    return false;
                }

                part->updatePartShape(result, false);
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

                builder.Add(compound, output);
                part->updatePartShape(compound, false);
                _occShapeAgent->updateShape(part->getCopiedPartShape(), false);
            }
            else
            {
                _occShapeAgent->updateShape(output);
            }
        }
        catch (...)
        {
            printLog(tr("Invalid parameters !"), 3);
            return false;
        }

        return true;
    }
}
