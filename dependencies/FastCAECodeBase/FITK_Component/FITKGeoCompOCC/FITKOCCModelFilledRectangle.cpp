/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCModelFilledRectangle.h"

#include "FITKOCCModelPart.h"
#include "FITKOCCExtendTool.h"
#include "FITKOCCComandCommon.h"
#include <array>

// OCC
#include <gp_Pnt.hxx>
#include <BRepBuilderAPI_MakePolygon.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <TopoDS_Builder.hxx>
#include <TopoDS_Compound.hxx>
#include <TopoDS.hxx>

namespace OCC
{
    FITKOCCModelFilledRectangle::FITKOCCModelFilledRectangle() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelFilledRectangle::update()
    {
        FITKOCCModelPart* part = _occShapeAgent->getPart();

        // 参数验证。
        if (m_XLength < 1e-9 || m_YLength < 1e-9)
        {
            printLog(tr("Invalid length !"), 3);
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

            // 计算四个顶点。
            gp_Pnt p1(m_Origin[0], m_Origin[1], m_Origin[2]);
            gp_Pnt p2(m_Origin[0] + dir1Norm[0] * m_XLength,
                     m_Origin[1] + dir1Norm[1] * m_XLength,
                     m_Origin[2] + dir1Norm[2] * m_XLength);
            gp_Pnt p3(m_Origin[0] + dir1Norm[0] * m_XLength + correctedYDir[0] * m_YLength,
                     m_Origin[1] + dir1Norm[1] * m_XLength + correctedYDir[1] * m_YLength,
                     m_Origin[2] + dir1Norm[2] * m_XLength + correctedYDir[2] * m_YLength);
            gp_Pnt p4(m_Origin[0] + correctedYDir[0] * m_YLength,
                     m_Origin[1] + correctedYDir[1] * m_YLength,
                     m_Origin[2] + correctedYDir[2] * m_YLength);

            // 创建矩形多边形。
            BRepBuilderAPI_MakePolygon polygon;
            polygon.Add(p1);
            polygon.Add(p2);
            polygon.Add(p3);
            polygon.Add(p4);
            polygon.Close();
            if (!polygon.IsDone())
            {
                printLog(tr("Invalid parameters !"), 3);
                return false;
            }

            // 根据是否填充决定创建面还是线框。
            TopoDS_Shape output;
            if (m_IsFilled)
            {
                // 创建填充面。
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
                // 仅创建线框。
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
