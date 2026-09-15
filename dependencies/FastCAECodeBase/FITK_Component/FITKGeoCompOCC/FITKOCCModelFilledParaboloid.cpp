/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCModelFilledParaboloid.h"

#include "FITKOCCModelPart.h"
#include "FITKOCCExtendTool.h"
#include "FITKOCCComandCommon.h"

// OCC
#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>
#include <gp_Ax1.hxx>
#include <gp_Ax2.hxx>
#include <gp_Parab.hxx>
#include <gp_Vec.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepPrimAPI_MakeRevol.hxx>
#include <TopoDS_Builder.hxx>
#include <TopoDS_Compound.hxx>
#include <TopoDS.hxx>
#include <TopLoc_Location.hxx>

namespace OCC
{
    FITKOCCModelFilledParaboloid::FITKOCCModelFilledParaboloid() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelFilledParaboloid::update()
    {
        FITKOCCModelPart* part = _occShapeAgent->getPart();

        if (m_FocalDepth < 1e-9 || m_StartRadius < 1e-9)
        {
            printLog(tr("Invalid parameters ! Focal depth and start radius must be positive."), 3);
            return false;
        }

        // 计算两个方向向量的长度
        double len1 = std::sqrt(m_XDirection[0] * m_XDirection[0] +
            m_XDirection[1] * m_XDirection[1] +
            m_XDirection[2] * m_XDirection[2]);
        double len2 = std::sqrt(m_AxisDirection[0] * m_AxisDirection[0] +
            m_AxisDirection[1] * m_AxisDirection[1] +
            m_AxisDirection[2] * m_AxisDirection[2]);

        try
        {
            double dir1Norm[3]{ 1., 0., 0. };
            double dir2Norm[3]{ 0., 0., 1. };

            // 如果两个方向都是非零向量
            if (len1 > 1e-9 && len2 > 1e-9)
            {
                // 归一化方向向量
                dir1Norm[0] = m_XDirection[0] / len1;
                dir1Norm[1] = m_XDirection[1] / len1;
                dir1Norm[2] = m_XDirection[2] / len1;

                dir2Norm[0] = m_AxisDirection[0] / len2;
                dir2Norm[1] = m_AxisDirection[1] / len2;
                dir2Norm[2] = m_AxisDirection[2] / len2;
            }

            // 参数验证
            if (m_StartRadius < 1e-9)
            {
                printLog(tr("Invalid parameters ! Start radius must be positive."), 3);
                return false;
            }

            // 创建顶点坐标
            gp_Pnt vertex(m_Origin[0], m_Origin[1], m_Origin[2]);

            // 创建旋转轴（使用Z轴方向）
            gp_Dir zAxis(dir2Norm[0], dir2Norm[1], dir2Norm[2]);

            // 计算真正的X轴方向：X = Z × XRef，然后投影到垂直于Z的平面
            gp_Dir xAxis(dir1Norm[0], dir1Norm[1], dir1Norm[2]);

            // 创建局部坐标系
            gp_Ax2 localAxis(vertex, xAxis, zAxis);

            // 创建旋转轴
            gp_Ax1 revolAx(vertex, zAxis);

            // 创建抛物线
            // gp_Parab方程: Y² = 4pX，其中p = Focal/2
            // 在localAxis坐标系中：
            // - X轴是抛物线的对称轴
            // - Y轴垂直于对称轴和旋转轴
            // - 抛物线沿X轴正方向开口
            gp_Parab parab(localAxis, m_FocalDepth);

            // 创建抛物线边缘，使用参数范围
            // 抛物线的参数U对应径向距离Y坐标
            // U=0 时是顶点，U=startRadius 时是指定半径位置
            BRepBuilderAPI_MakeEdge edgeMaker(parab, 0.0, m_StartRadius);
            if (!edgeMaker.IsDone())
            {
                printLog(tr("Failed to create parabola edge!"), 3);
                return false;
            }

            // 绕轴旋转360度创建抛物面
            BRepPrimAPI_MakeRevol revolMaker(edgeMaker.Edge(), revolAx, 2 * M_PI);
            if (!revolMaker.IsDone())
            {
                printLog(tr("Failed to create revolution!"), 3);
                return false;
            }

            TopoDS_Shape output = revolMaker.Shape();
            if (output.IsNull())
            {
                printLog(tr("Failed to create paraboloid!"), 3);
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
            printLog(tr("Failed to create paraboloid!"), 3);
            return false;
        }

        return true;
    }
}
