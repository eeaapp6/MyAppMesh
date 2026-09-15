/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCModelFilledEllipse.h"

#include "FITKOCCModelPart.h"
#include "FITKOCCExtendTool.h"
#include "FITKOCCComandCommon.h"
#include <array>

// OCC
#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>
#include <gp_Elips.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <TopoDS_Builder.hxx>
#include <TopoDS_Compound.hxx>
#include <TopoDS.hxx>

namespace OCC
{
    FITKOCCModelFilledEllipse::FITKOCCModelFilledEllipse() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelFilledEllipse::update()
    {
        FITKOCCModelPart* part = _occShapeAgent->getPart();

        // 参数验证。
        if (m_XRadius < 1e-9 || m_YRadius < 1e-9)
        {
            printLog(tr("Invalid radius !"), 3);
            return false;
        }

        // 计算两个方向向量的长度
        double len1 = std::sqrt(m_XDirection[0] * m_XDirection[0] +
            m_XDirection[1] * m_XDirection[1] +
            m_XDirection[2] * m_XDirection[2]);
        double len2 = std::sqrt(m_YDirection[0] * m_YDirection[0] +
            m_YDirection[1] * m_YDirection[1] +
            m_YDirection[2] * m_YDirection[2]);

        try
        {
            double dir1Norm[3]{ 1., 0., 0. };
            double dir2Norm[3]{ 0., 1., 0. };

            // 如果两个方向都是非零向量
            if (len1 > 1e-9 && len2 > 1e-9)
            {
                // 归一化方向向量
                dir1Norm[0] = m_XDirection[0] / len1;
                dir1Norm[1] = m_XDirection[1] / len1;
                dir1Norm[2] = m_XDirection[2] / len1;

                dir2Norm[0] = m_YDirection[0] / len2;
                dir2Norm[1] = m_YDirection[1] / len2;
                dir2Norm[2] = m_YDirection[2] / len2;
            }

            // 通过XY轴计算Z轴法向：Z = X × Y
            std::array<double, 3> zDir = {
                dir1Norm[1] * dir2Norm[2] - dir2Norm[1] * dir1Norm[2],
                dir1Norm[2] * dir2Norm[0] - dir2Norm[2] * dir1Norm[0],
                dir1Norm[0] * dir2Norm[1] - dir2Norm[0] * dir1Norm[1]
            };

            // 通过Z轴和X轴反推Y轴：Y = Z × X
            std::array<double, 3> correctedYDir = {
                zDir[1] * dir1Norm[2] - dir1Norm[1] * zDir[2],
                zDir[2] * dir1Norm[0] - dir1Norm[2] * zDir[0],
                zDir[0] * dir1Norm[1] - dir1Norm[0] * zDir[1]
            };

            // 创建椭圆。
            gp_Pnt center(m_Origin[0], m_Origin[1], m_Origin[2]);
            gp_Ax2 ax2(center,
                gp_Dir(zDir[0], zDir[1], zDir[2]),
                gp_Dir(dir1Norm[0], dir1Norm[1], dir1Norm[2]));
            gp_Elips ellipse(ax2, qMax(m_XRadius, m_YRadius), qMin(m_XRadius, m_YRadius));

            // 创建椭圆线框。
            BRepBuilderAPI_MakeEdge edgeMaker(ellipse);
            if (!edgeMaker.IsDone())
            {
                printLog(tr("Invalid parameters !"), 3);
                return false;
            }

            BRepBuilderAPI_MakeWire wireMaker(edgeMaker.Edge());
            if (!wireMaker.IsDone())
            {
                printLog(tr("Invalid parameters !"), 3);
                return false;
            }

            // 根据是否填充决定创建面还是线框。
            TopoDS_Shape output;
            if (m_IsFilled)
            {
                // 创建填充面。
                BRepBuilderAPI_MakeFace faceMaker(wireMaker.Wire());
                if (!faceMaker.IsDone())
                {
                    printLog(tr("Invalid parameters !"), 3);
                    return false;
                }

                output = faceMaker.Shape();
            }
            else
            {
                output = wireMaker.Shape();
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
