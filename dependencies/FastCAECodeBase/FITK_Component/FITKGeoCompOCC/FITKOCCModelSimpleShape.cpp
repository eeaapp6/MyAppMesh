/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCModelSimpleShape.h"

#include "FITKOCCModelPart.h"
#include "FITKOCCComandCommon.h"
#include "FITKOCCExtendTool.h"

#include <BRepPrimAPI_MakeBox.hxx>
#include <gp_Ax2.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRepBuilderAPI_MakePolygon.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <gp_Pnt.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Compound.hxx>
#include <TopExp_Explorer.hxx>
#include <TopAbs_ShapeEnum.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeSolid.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <Geom_Plane.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_Transform.hxx>

namespace OCC
{

    FITKOCCModelBox::FITKOCCModelBox() :OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelBox::update()
    {
        FITKOCCModelPart* part = _occShapeAgent->getPart();

        gp_Pnt pt1(_point1[0], _point1[1], _point1[2]);

        //错误判断 - 长度参数必须为正
        if (_lengthParam[0] <= 1e-9 || _lengthParam[1] <= 1e-9 || _lengthParam[2] <= 1e-9)
        {
            printLog(tr("Invalid length !"), 3);
            return false;
        }

        // 异常处理。
        try
        {
            // 检查两个方向是否有效(非零向量)且平行或反平行
            bool hasValidDirections = true;
            gp_Ax2 boxAxes;

            // 计算两个方向向量的长度
            double len1 = std::sqrt(_direction1[0] * _direction1[0] +
                _direction1[1] * _direction1[1] +
                _direction1[2] * _direction1[2]);
            double len2 = std::sqrt(_direction2[0] * _direction2[0] +
                _direction2[1] * _direction2[1] +
                _direction2[2] * _direction2[2]);

            // 如果两个方向都是非零向量
            if (len1 > 1e-9 && len2 > 1e-9)
            {
                // 归一化方向向量
                double dir1Norm[3] = { _direction1[0] / len1, _direction1[1] / len1, _direction1[2] / len1 };
                double dir2Norm[3] = { _direction2[0] / len2, _direction2[1] / len2, _direction2[2] / len2 };

                // 计算叉积,判断是否平行或反平行
                double cross[3];
                cross[0] = dir1Norm[1] * dir2Norm[2] - dir1Norm[2] * dir2Norm[1];
                cross[1] = dir1Norm[2] * dir2Norm[0] - dir1Norm[0] * dir2Norm[2];
                cross[2] = dir1Norm[0] * dir2Norm[1] - dir1Norm[1] * dir2Norm[0];

                double crossLen = std::sqrt(cross[0] * cross[0] + cross[1] * cross[1] + cross[2] * cross[2]);

                // 如果叉积接近零,说明两个方向平行或反平行
                if (crossLen < 1e-9)
                {
                    hasValidDirections = false;
                }
                else
                {
                    hasValidDirections = true;

                    // 使用第一个方向作为X轴,第二个方向与第一个的叉积计算Z轴
                    gp_Dir xDir(dir1Norm[0], dir1Norm[1], dir1Norm[2]);

                    // 计算Z轴方向(底面法向) = 叉积
                    // 第三个方向应该是两个方向的叉积的归一化
                    gp_Dir zDir(cross[0], cross[1], cross[2]);

                    // Y轴方向 = Z轴 × X轴(右手坐标系)
                    gp_Dir yDir = zDir.Crossed(xDir);

                    // 创建局部坐标系gp_Ax2
                    boxAxes.SetLocation(pt1);
                    boxAxes.SetDirection(zDir);
                    boxAxes.SetXDirection(xDir);
                    boxAxes.SetYDirection(yDir);
                }
            }

            // 根据是否有有效方向选择不同的创建方式
            if (hasValidDirections)
            {
                // 使用带方向的gp_Ax2构造函数
                BRepPrimAPI_MakeBox cmd(boxAxes, _lengthParam[0], _lengthParam[1], _lengthParam[2]);
                cmd.Build();
                if (!cmd.IsDone())
                {
                    printLog(tr("Invalid parameters !"), 3);
                    return false;
                }

                const TopoDS_Shape& output = cmd.Shape();
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
            else
            {
                //// 使用原来的两点方式创建Box
                //gp_Pnt pt2(_point1[0] + _lengthParam[0], _point1[1] + _lengthParam[1], _point1[2] + _lengthParam[2]);
                //
                //if (pt1.Distance(pt2) < 1e-9)
                //{
                //    printLog(tr("Invalid length !"), 3);
                //    return false;
                //}

                //BRepPrimAPI_MakeBox cmd(pt1, pt2);
                //cmd.Build();
                //if (!cmd.IsDone())
                //{
                //    printLog(tr("Invalid parameters !"), 3);
                //    return false;
                //}

                //const TopoDS_Shape & output = cmd.Shape();
                //if (output.IsNull())
                //{
                //    printLog(tr("Invalid parameters !"), 3);
                //    return false;
                //}

                //if (part)
                //{
                //    TopoDS_Shape* partShape = part->getPartShape();
                //    TopoDS_Builder builder;
                //    TopoDS_Compound compound;
                //    builder.MakeCompound(compound);

                //    if (!FITKOCCExtendTool::IsEmptyShape(*partShape))
                //    {
                //        builder.Add(compound, *partShape);
                //    }

                //    builder.Add(compound, cmd.Shape());
                //    part->updatePartShape(compound, false);
                //    _occShapeAgent->updateShape(part->getCopiedPartShape(), false);
                //}
                //else
                //{
                //    _occShapeAgent->updateShape(cmd.Shape());
                //}

                return false;
            }
        }
        catch (...)
        {
            printLog(tr("Invalid parameters !"), 3);
            return false;
        }

        return true;
    }

    FITKOCCModelCylinder::FITKOCCModelCylinder() :OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;

    }

    bool FITKOCCModelCylinder::update()
    {
        FITKOCCModelPart* part = _occShapeAgent->getPart();

        // 异常处理。
        try
        {
            //创建轴线
            gp_Ax2 anAxis;
            anAxis.SetLocation(gp_Pnt(_location[0], _location[1], _location[2]));
            anAxis.SetDirection(gp_Dir(_direction[0], _direction[1], _direction[2]));
            //创建圆柱
            if (_length < 1e-9)
            {
                printLog(tr("Invalid length !"), 3);
                return false;
            }

            if (_radius < 1e-9)
            {
                printLog(tr("Invalid radius !"), 3);
                return false;
            }

            BRepPrimAPI_MakeCylinder cmd(anAxis, _radius, _length);
            cmd.Build();
            if (!cmd.IsDone())
            {
                printLog(tr("Invalid parameters !"), 3);
                return false;
            }

            const TopoDS_Shape& output = cmd.Shape();
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

    FITKOCCModelCone::FITKOCCModelCone() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelCone::update()
    {
        FITKOCCModelPart* part = _occShapeAgent->getPart();

        // 异常处理。
        try
        {
            // 创建轴线。
            gp_Ax2 anAxis;
            anAxis.SetLocation(gp_Pnt(_location[0], _location[1], _location[2]));
            anAxis.SetDirection(gp_Dir(_direction[0], _direction[1], _direction[2]));

            // 创建圆锥。
            if (_length < 1e-9)
            {
                printLog(tr("Invalid length !"), 3);
                return false;
            }

            if (_bottomRadius < 1e-9 && _topRadius < 1e-9)
            {
                printLog(tr("Invalid radius !"), 3);
                return false;
            }

            TopoDS_Shape output;
            if (qFuzzyCompare(_bottomRadius, _topRadius))
            {
                BRepPrimAPI_MakeCylinder cmd(anAxis, _bottomRadius, _length);
                cmd.Build();
                if (!cmd.IsDone())
                {
                    printLog(tr("Invalid parameters !"), 3);
                    return false;
                }

                output = cmd.Shape();
            }
            else
            {
                BRepPrimAPI_MakeCone cmd(anAxis, _bottomRadius, _topRadius, _length);
                cmd.Build();
                if (!cmd.IsDone())
                {
                    printLog(tr("Invalid parameters !"), 3);
                    return false;
                }

                output = cmd.Shape();
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

    FITKOCCModelFrustum::FITKOCCModelFrustum() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelFrustum::update()
    {
        FITKOCCModelPart* part = _occShapeAgent->getPart();

        // 异常处理。
        try
        {
            // 检查参数有效性。
            if (_length < 1e-9)
            {
                printLog(tr("Invalid length !"), 3);
                return false;
            }

            if (_bottomLength < 1e-9)
            {
                printLog(tr("Bottom length must be greater than 0!"), 3);
                return false;
            }

            if (_bottomWidth < 1e-9)
            {
                printLog(tr("Bottom width must be greater than 0!"), 3);
                return false;
            }

            if (_topLength < 0 || _topWidth < 0)
            {
                printLog(tr("Invalid top dimensions !"), 3);
                return false;
            }

            // 检查方向是否有效(非零向量)且不平行。
            double len1 = std::sqrt(_direction1[0] * _direction1[0] +
                _direction1[1] * _direction1[1] +
                _direction1[2] * _direction1[2]);
            double len2 = std::sqrt(_direction2[0] * _direction2[0] +
                _direction2[1] * _direction2[1] +
                _direction2[2] * _direction2[2]);

            if (len1 < 1e-9 || len2 < 1e-9)
            {
                printLog(tr("Invalid direction !"), 3);
                return false;
            }

            // 归一化方向向量。
            double dir1Norm[3] = { _direction1[0] / len1, _direction1[1] / len1, _direction1[2] / len1 };
            double dir2Norm[3] = { _direction2[0] / len2, _direction2[1] / len2, _direction2[2] / len2 };

            // 计算叉积,判断是否平行或反平行。
            double cross[3];
            cross[0] = dir1Norm[1] * dir2Norm[2] - dir1Norm[2] * dir2Norm[1];
            cross[1] = dir1Norm[2] * dir2Norm[0] - dir1Norm[0] * dir2Norm[2];
            cross[2] = dir1Norm[0] * dir2Norm[1] - dir1Norm[1] * dir2Norm[0];

            double crossLen = std::sqrt(cross[0] * cross[0] + cross[1] * cross[1] + cross[2] * cross[2]);

            // 如果叉积接近零,说明两个方向平行或反平行，无效。
            if (crossLen < 1e-9)
            {
                printLog(tr("Invalid direction !"), 3);
                return false;
            }

            // 计算局部坐标系。
            gp_Dir xDir(dir1Norm[0], dir1Norm[1], dir1Norm[2]);
            gp_Dir zDir(cross[0], cross[1], cross[2]);
            gp_Dir yDir = zDir.Crossed(xDir);

            // 底面中心点。
            gp_Pnt center(_location[0], _location[1], _location[2]);

            // 计算底面四个顶点（相对于中心点）。
            // ACIS顶点映射：v0(-X,-Y), v1(+X,-Y), v2(+X,+Y), v3(-X,+Y)
            // OCC底面：p1(+X,-Y)=v1, p2(-X,-Y)=v0, p3(-X,+Y)=v3, p4(+X,+Y)=v2
            double halfBottomL = _bottomLength / 2.0;
            double halfBottomW = _bottomWidth / 2.0;

            gp_Pnt p1 = center.Translated(xDir.XYZ() * halfBottomL + yDir.XYZ() * (-halfBottomW));
            gp_Pnt p2 = center.Translated(xDir.XYZ() * (-halfBottomL) + yDir.XYZ() * (-halfBottomW));
            gp_Pnt p3 = center.Translated(xDir.XYZ() * (-halfBottomL) + yDir.XYZ() * halfBottomW);
            gp_Pnt p4 = center.Translated(xDir.XYZ() * halfBottomL + yDir.XYZ() * halfBottomW);

            // 计算顶面四个顶点。
            // ACIS顶点映射：v4(-X,-Y,H), v5(+X,-Y,H), v6(+X,+Y,H), v7(-X,+Y,H)
            // OCC顶面：tp1(+X,-Y,H)=v5, tp2(-X,-Y,H)=v4, tp3(-X,+Y,H)=v7, tp4(+X,+Y,H)=v6
            double halfTopL = _topLength / 2.0;
            double halfTopW = _topWidth / 2.0;
            gp_Pnt tp1 = center.Translated(xDir.XYZ() * halfTopL + yDir.XYZ() * (-halfTopW) + zDir.XYZ() * _length);
            gp_Pnt tp2 = center.Translated(xDir.XYZ() * (-halfTopL) + yDir.XYZ() * (-halfTopW) + zDir.XYZ() * _length);
            gp_Pnt tp3 = center.Translated(xDir.XYZ() * (-halfTopL) + yDir.XYZ() * halfTopW + zDir.XYZ() * _length);
            gp_Pnt tp4 = center.Translated(xDir.XYZ() * halfTopL + yDir.XYZ() * halfTopW + zDir.XYZ() * _length);

            // 判断创建哪种形状。
            bool topLengthZero = (_topLength < 1e-9);
            bool topWidthZero = (_topWidth < 1e-9);

            TopoDS_Shape output;

            if (topLengthZero && topWidthZero)
            {
                // 三角形棱锥：顶面退化成一个点（顶部顶点）。
                gp_Pnt apex = center.Translated(zDir.XYZ() * _length);

                // 创建底面四边形Wire: p1→p2→p3→p4→p1
                BRepBuilderAPI_MakeWire bottomWire;
                bottomWire.Add(BRepBuilderAPI_MakeEdge(p1, p2).Edge());
                bottomWire.Add(BRepBuilderAPI_MakeEdge(p2, p3).Edge());
                bottomWire.Add(BRepBuilderAPI_MakeEdge(p3, p4).Edge());
                bottomWire.Add(BRepBuilderAPI_MakeEdge(p4, p1).Edge());
                if (!bottomWire.IsDone())
                {
                    printLog(tr("Failed to create bottom wire!"), 3);
                    return false;
                }

                // 创建4个三角形侧面。
                BRepBuilderAPI_MakeFace face1(BRepBuilderAPI_MakeWire(BRepBuilderAPI_MakeEdge(p1, p2).Edge(),
                    BRepBuilderAPI_MakeEdge(p2, apex).Edge(),
                    BRepBuilderAPI_MakeEdge(apex, p1).Edge()).Wire());
                BRepBuilderAPI_MakeFace face2(BRepBuilderAPI_MakeWire(BRepBuilderAPI_MakeEdge(p2, p3).Edge(),
                    BRepBuilderAPI_MakeEdge(p3, apex).Edge(),
                    BRepBuilderAPI_MakeEdge(apex, p2).Edge()).Wire());
                BRepBuilderAPI_MakeFace face3(BRepBuilderAPI_MakeWire(BRepBuilderAPI_MakeEdge(p3, p4).Edge(),
                    BRepBuilderAPI_MakeEdge(p4, apex).Edge(),
                    BRepBuilderAPI_MakeEdge(apex, p3).Edge()).Wire());
                BRepBuilderAPI_MakeFace face4(BRepBuilderAPI_MakeWire(BRepBuilderAPI_MakeEdge(p4, p1).Edge(),
                    BRepBuilderAPI_MakeEdge(p1, apex).Edge(),
                    BRepBuilderAPI_MakeEdge(apex, p4).Edge()).Wire());

                if (!face1.IsDone() || !face2.IsDone() || !face3.IsDone() || !face4.IsDone())
                {
                    printLog(tr("Failed to create pyramid faces!"), 3);
                    return false;
                }

                // 创建底面
                BRepBuilderAPI_MakeFace bottomFace(bottomWire.Wire());
                if (!bottomFace.IsDone())
                {
                    printLog(tr("Failed to create bottom face!"), 3);
                    return false;
                }

                // 融合所有面
                BRepAlgoAPI_Fuse fuse1(bottomFace.Face(), face1.Face());
                if (!fuse1.IsDone())
                {
                    printLog(tr("Failed to fuse bottom and face1!"), 3);
                    return false;
                }

                BRepAlgoAPI_Fuse fuse2(fuse1.Shape(), face2.Face());
                if (!fuse2.IsDone())
                {
                    printLog(tr("Failed to fuse with face2!"), 3);
                    return false;
                }

                BRepAlgoAPI_Fuse fuse3(fuse2.Shape(), face3.Face());
                if (!fuse3.IsDone())
                {
                    printLog(tr("Failed to fuse with face3!"), 3);
                    return false;
                }

                BRepAlgoAPI_Fuse fuse4(fuse3.Shape(), face4.Face());
                if (!fuse4.IsDone())
                {
                    printLog(tr("Failed to fuse with face4!"), 3);
                    return false;
                }

                // 从融合结果创建Shell和Solid
                TopoDS_Builder shellBuilder;
                TopoDS_Shell newShell;
                shellBuilder.MakeShell(newShell);

                TopExp_Explorer faceExplorer(fuse4.Shape(), TopAbs_ShapeEnum::TopAbs_FACE);
                int faceCount = 0;
                for (; faceExplorer.More(); faceExplorer.Next())
                {
                    shellBuilder.Add(newShell, TopoDS::Face(faceExplorer.Current()));
                    faceCount++;
                }

                if (faceCount == 0)
                {
                    printLog(tr("No faces found!"), 3);
                    return false;
                }

                BRepBuilderAPI_MakeSolid solidMaker(newShell);
                if (!solidMaker.IsDone())
                {
                    printLog(tr("Failed to create solid!"), 3);
                    return false;
                }

                output = solidMaker.Solid();
            }
            else if (topLengthZero || topWidthZero)
            {
                // 梯形棱锥：顶面退化成一条线。
                // 底面是四边形，顶面是一条线，3个侧面是四边形，1个侧面是三角形。

                // 创建底面四边形Wire: p1→p2→p3→p4→p1
                BRepBuilderAPI_MakeWire bottomWire;
                bottomWire.Add(BRepBuilderAPI_MakeEdge(p1, p2).Edge());
                bottomWire.Add(BRepBuilderAPI_MakeEdge(p2, p3).Edge());
                bottomWire.Add(BRepBuilderAPI_MakeEdge(p3, p4).Edge());
                bottomWire.Add(BRepBuilderAPI_MakeEdge(p4, p1).Edge());
                if (!bottomWire.IsDone())
                {
                    printLog(tr("Failed to create bottom wire!"), 3);
                    return false;
                }

                // 创建梯形棱锥的侧面：2个三角形和2个四边形
                if (topLengthZero)
                {
                    // topLength=0, topWidth>0: 顶面线段沿y方向（tp1=tp2, tp3=tp4）
                    // 根据ACIS建模逻辑，顶面位于x=-X处
                    // 修正顶面顶点的x坐标为-xDir方向
                    tp1 = center.Translated(xDir.XYZ() * (-halfTopL) + yDir.XYZ() * (-halfTopW) + zDir.XYZ() * _length);
                    tp2 = tp1;
                    tp3 = center.Translated(xDir.XYZ() * (-halfTopL) + yDir.XYZ() * halfTopW + zDir.XYZ() * _length);
                    tp4 = tp3;

                    // 根据ACIS建模逻辑：2个三角形 + 2个四边形
                    // 底面：p1(+X,-Y), p2(-X,-Y), p3(-X,+Y), p4(+X,+Y)
                    // 顶面线段：tp2=tp1(-X,-Y,H), tp3=tp4(-X,+Y,H)
                    // - 前侧面(Y负方向): p1, p2, tp2 (三角形)
                    // - 右侧面(X正方向): p1, p4, tp3, tp2 (四边形)
                    // - 后侧面(Y正方向): p4, p3, tp3 (三角形)
                    // - 左侧面(X负方向): p2, p3, tp3, tp2 (四边形)

                    // 侧面1(前侧面): p1-p2-tp2 (三角形)
                    BRepBuilderAPI_MakeFace side1Face(BRepBuilderAPI_MakeWire(BRepBuilderAPI_MakeEdge(p1, p2).Edge(),
                        BRepBuilderAPI_MakeEdge(p2, tp2).Edge(),
                        BRepBuilderAPI_MakeEdge(tp2, p1).Edge()).Wire());

                    // 侧面2(右侧面): p1-p4-tp3-tp2 (四边形)
                    BRepBuilderAPI_MakeWire side2Wire;
                    side2Wire.Add(BRepBuilderAPI_MakeEdge(p1, p4).Edge());
                    side2Wire.Add(BRepBuilderAPI_MakeEdge(p4, tp3).Edge());
                    side2Wire.Add(BRepBuilderAPI_MakeEdge(tp3, tp2).Edge());
                    side2Wire.Add(BRepBuilderAPI_MakeEdge(tp2, p1).Edge());
                    BRepBuilderAPI_MakeFace side2Face(side2Wire.Wire());

                    // 侧面3(后侧面): p4-p3-tp3 (三角形)
                    BRepBuilderAPI_MakeFace side3Face(BRepBuilderAPI_MakeWire(BRepBuilderAPI_MakeEdge(p4, p3).Edge(),
                        BRepBuilderAPI_MakeEdge(p3, tp3).Edge(),
                        BRepBuilderAPI_MakeEdge(tp3, p4).Edge()).Wire());

                    // 侧面4(左侧面): p2-p3-tp3-tp2 (四边形)
                    BRepBuilderAPI_MakeWire side4Wire;
                    side4Wire.Add(BRepBuilderAPI_MakeEdge(p2, p3).Edge());
                    side4Wire.Add(BRepBuilderAPI_MakeEdge(p3, tp3).Edge());
                    side4Wire.Add(BRepBuilderAPI_MakeEdge(tp3, tp2).Edge());
                    side4Wire.Add(BRepBuilderAPI_MakeEdge(tp2, p2).Edge());
                    BRepBuilderAPI_MakeFace side4Face(side4Wire.Wire());

                    if (!side1Face.IsDone() || !side2Face.IsDone() || !side3Face.IsDone() || !side4Face.IsDone())
                    {
                        printLog(tr("Failed to create side faces!"), 3);
                        return false;
                    }

                    // 创建底面
                    BRepBuilderAPI_MakeFace bottomFace(bottomWire.Wire());
                    if (!bottomFace.IsDone())
                    {
                        printLog(tr("Failed to create bottom face!"), 3);
                        return false;
                    }

                    // 融合所有面
                    BRepAlgoAPI_Fuse fuse1(bottomFace.Face(), side1Face.Face());
                    if (!fuse1.IsDone())
                    {
                        printLog(tr("Failed to fuse bottom and side1!"), 3);
                        return false;
                    }

                    BRepAlgoAPI_Fuse fuse2(fuse1.Shape(), side2Face.Face());
                    if (!fuse2.IsDone())
                    {
                        printLog(tr("Failed to fuse with side2!"), 3);
                        return false;
                    }

                    BRepAlgoAPI_Fuse fuse3(fuse2.Shape(), side3Face.Face());
                    if (!fuse3.IsDone())
                    {
                        printLog(tr("Failed to fuse with side3!"), 3);
                        return false;
                    }

                    BRepAlgoAPI_Fuse fuse4(fuse3.Shape(), side4Face.Face());
                    if (!fuse4.IsDone())
                    {
                        printLog(tr("Failed to fuse with side4!"), 3);
                        return false;
                    }

                    // 创建Shell和Solid
                    TopoDS_Builder shellBuilder;
                    TopoDS_Shell newShell;
                    shellBuilder.MakeShell(newShell);

                    TopExp_Explorer faceExplorer(fuse4.Shape(), TopAbs_ShapeEnum::TopAbs_FACE);
                    int faceCount = 0;
                    for (; faceExplorer.More(); faceExplorer.Next())
                    {
                        shellBuilder.Add(newShell, TopoDS::Face(faceExplorer.Current()));
                        faceCount++;
                    }

                    if (faceCount == 0)
                    {
                        printLog(tr("No faces found!"), 3);
                        return false;
                    }

                    BRepBuilderAPI_MakeSolid solidMaker(newShell);
                    if (!solidMaker.IsDone())
                    {
                        printLog(tr("Failed to create solid!"), 3);
                        return false;
                    }

                    output = solidMaker.Solid();
                }
                else
                {
                    // topWidthZero: topWidth=0, topLength>0: 顶面线段沿x方向（tp3=tp4）
                    // 根据ACIS建模逻辑，顶面位于y=+Y处
                    // 修正顶面顶点的y坐标为+yDir方向
                    tp1 = center.Translated(xDir.XYZ() * (-halfTopL) + yDir.XYZ() * halfTopW + zDir.XYZ() * _length);
                    tp2 = center.Translated(xDir.XYZ() * halfTopL + yDir.XYZ() * halfTopW + zDir.XYZ() * _length);
                    tp3 = tp2;
                    tp4 = tp1;

                    // 根据ACIS建模逻辑：2个四边形 + 2个三角形
                    // 底面：p1(+X,-Y), p2(-X,-Y), p3(-X,+Y), p4(+X,+Y)
                    // 顶面线段：tp1=(-X,+Y,H), tp2=tp3=(+X,+Y,H), tp4=tp1
                    // - 前侧面(Y负方向): p2, p1, tp2, tp1 (四边形)
                    // - 右侧面(X正方向): p1, p4, tp2 (三角形)
                    // - 后侧面(Y正方向): p4, p3, tp2, tp1 (四边形)
                    // - 左侧面(X负方向): p3, p2, tp1 (三角形)

                    // 侧面1(前侧面): p2-p1-tp2-p2 (四边形) - 修正边的顺序避免对角线交叉
                    BRepBuilderAPI_MakeWire side1Wire;
                    side1Wire.Add(BRepBuilderAPI_MakeEdge(p2, p1).Edge());
                    side1Wire.Add(BRepBuilderAPI_MakeEdge(p1, tp2).Edge());
                    side1Wire.Add(BRepBuilderAPI_MakeEdge(tp2, tp1).Edge());
                    side1Wire.Add(BRepBuilderAPI_MakeEdge(tp1, p2).Edge());
                    BRepBuilderAPI_MakeFace side1Face(side1Wire.Wire());

                    // 侧面2(右侧面): p1-p4-tp2 (三角形)
                    BRepBuilderAPI_MakeFace side2Face(BRepBuilderAPI_MakeWire(BRepBuilderAPI_MakeEdge(p1, p4).Edge(),
                        BRepBuilderAPI_MakeEdge(p4, tp2).Edge(),
                        BRepBuilderAPI_MakeEdge(tp2, p1).Edge()).Wire());

                    // 侧面3(后侧面): p4-p3-tp1-tp2 (四边形) - 修正边的顺序避免对角线交叉
                    BRepBuilderAPI_MakeWire side3Wire;
                    side3Wire.Add(BRepBuilderAPI_MakeEdge(p4, p3).Edge());
                    side3Wire.Add(BRepBuilderAPI_MakeEdge(p3, tp1).Edge());
                    side3Wire.Add(BRepBuilderAPI_MakeEdge(tp1, tp2).Edge());
                    side3Wire.Add(BRepBuilderAPI_MakeEdge(tp2, p4).Edge());
                    BRepBuilderAPI_MakeFace side3Face(side3Wire.Wire());

                    // 侧面4(左侧面): p3-p2-tp1 (三角形)
                    BRepBuilderAPI_MakeFace side4Face(BRepBuilderAPI_MakeWire(BRepBuilderAPI_MakeEdge(p3, p2).Edge(),
                        BRepBuilderAPI_MakeEdge(p2, tp1).Edge(),
                        BRepBuilderAPI_MakeEdge(tp1, p3).Edge()).Wire());

                    if (!side1Face.IsDone() || !side2Face.IsDone() || !side3Face.IsDone() || !side4Face.IsDone())
                    {
                        printLog(tr("Failed to create side faces!"), 3);
                        return false;
                    }

                    // 创建底面
                    BRepBuilderAPI_MakeFace bottomFace(bottomWire.Wire());
                    if (!bottomFace.IsDone())
                    {
                        printLog(tr("Failed to create bottom face!"), 3);
                        return false;
                    }

                    // 融合所有面
                    BRepAlgoAPI_Fuse fuse1(bottomFace.Face(), side1Face.Face());
                    if (!fuse1.IsDone())
                    {
                        printLog(tr("Failed to fuse bottom and side1!"), 3);
                        return false;
                    }

                    BRepAlgoAPI_Fuse fuse2(fuse1.Shape(), side2Face.Face());
                    if (!fuse2.IsDone())
                    {
                        printLog(tr("Failed to fuse with side2!"), 3);
                        return false;
                    }

                    BRepAlgoAPI_Fuse fuse3(fuse2.Shape(), side3Face.Face());
                    if (!fuse3.IsDone())
                    {
                        printLog(tr("Failed to fuse with side3!"), 3);
                        return false;
                    }

                    BRepAlgoAPI_Fuse fuse4(fuse3.Shape(), side4Face.Face());
                    if (!fuse4.IsDone())
                    {
                        printLog(tr("Failed to fuse with side4!"), 3);
                        return false;
                    }

                    // 创建Shell和Solid
                    TopoDS_Builder shellBuilder;
                    TopoDS_Shell newShell;
                    shellBuilder.MakeShell(newShell);

                    TopExp_Explorer faceExplorer(fuse4.Shape(), TopAbs_ShapeEnum::TopAbs_FACE);
                    int faceCount = 0;
                    for (; faceExplorer.More(); faceExplorer.Next())
                    {
                        shellBuilder.Add(newShell, TopoDS::Face(faceExplorer.Current()));
                        faceCount++;
                    }

                    if (faceCount == 0)
                    {
                        printLog(tr("No faces found!"), 3);
                        return false;
                    }

                    BRepBuilderAPI_MakeSolid solidMaker(newShell);
                    if (!solidMaker.IsDone())
                    {
                        printLog(tr("Failed to create solid!"), 3);
                        return false;
                    }

                    output = solidMaker.Solid();
                }
            }
            else
            {
                // 标准棱台：顶面和底面都是四边形，4个侧面都是四边形。

                // 创建底面Wire: p1→p2→p3→p4→p1
                BRepBuilderAPI_MakeWire bottomWire;
                bottomWire.Add(BRepBuilderAPI_MakeEdge(p1, p2).Edge());   // p1→p2
                bottomWire.Add(BRepBuilderAPI_MakeEdge(p2, p3).Edge());   // p2→p3
                bottomWire.Add(BRepBuilderAPI_MakeEdge(p3, p4).Edge());   // p3→p4
                bottomWire.Add(BRepBuilderAPI_MakeEdge(p4, p1).Edge());   // p4→p1
                if (!bottomWire.IsDone())
                {
                    printLog(tr("Failed to create bottom wire!"), 3);
                    return false;
                }

                // 创建顶面Wire: tp1→tp2→tp3→tp4→tp1
                BRepBuilderAPI_MakeWire topWire;
                topWire.Add(BRepBuilderAPI_MakeEdge(tp1, tp2).Edge());   // tp1→tp2
                topWire.Add(BRepBuilderAPI_MakeEdge(tp2, tp3).Edge());   // tp2→tp3
                topWire.Add(BRepBuilderAPI_MakeEdge(tp3, tp4).Edge());   // tp3→tp4
                topWire.Add(BRepBuilderAPI_MakeEdge(tp4, tp1).Edge());   // tp4→tp1
                if (!topWire.IsDone())
                {
                    printLog(tr("Failed to create top wire!"), 3);
                    return false;
                }

                // 创建侧面1的Wire: p1→p2→tp2→tp1→p1 (沿x正方向)
                BRepBuilderAPI_MakeWire side1Wire;
                side1Wire.Add(BRepBuilderAPI_MakeEdge(p1, p2).Edge());       // p1→p2
                side1Wire.Add(BRepBuilderAPI_MakeEdge(p2, tp2).Edge());      // p2→tp2
                side1Wire.Add(BRepBuilderAPI_MakeEdge(tp2, tp1).Edge());     // tp2→tp1
                side1Wire.Add(BRepBuilderAPI_MakeEdge(tp1, p1).Edge());      // tp1→p1
                if (!side1Wire.IsDone())
                {
                    printLog(tr("Failed to create side1 wire!"), 3);
                    return false;
                }

                // 创建侧面2的Wire: p2→p3→tp3→tp2→p2 (沿y负方向)
                BRepBuilderAPI_MakeWire side2Wire;
                side2Wire.Add(BRepBuilderAPI_MakeEdge(p2, p3).Edge());       // p2→p3
                side2Wire.Add(BRepBuilderAPI_MakeEdge(p3, tp3).Edge());      // p3→tp3
                side2Wire.Add(BRepBuilderAPI_MakeEdge(tp3, tp2).Edge());     // tp3→tp2
                side2Wire.Add(BRepBuilderAPI_MakeEdge(tp2, p2).Edge());      // tp2→p2
                if (!side2Wire.IsDone())
                {
                    printLog(tr("Failed to create side2 wire!"), 3);
                    return false;
                }

                // 创建侧面3的Wire: p3→p4→tp4→tp3→p3 (沿x负方向)
                BRepBuilderAPI_MakeWire side3Wire;
                side3Wire.Add(BRepBuilderAPI_MakeEdge(p3, p4).Edge());       // p3→p4
                side3Wire.Add(BRepBuilderAPI_MakeEdge(p4, tp4).Edge());      // p4→tp4
                side3Wire.Add(BRepBuilderAPI_MakeEdge(tp4, tp3).Edge());     // tp4→tp3
                side3Wire.Add(BRepBuilderAPI_MakeEdge(tp3, p3).Edge());      // tp3→p3
                if (!side3Wire.IsDone())
                {
                    printLog(tr("Failed to create side3 wire!"), 3);
                    return false;
                }

                // 创建侧面4的Wire: p4→p1→tp1→tp4→p4 (沿y正方向)
                BRepBuilderAPI_MakeWire side4Wire;
                side4Wire.Add(BRepBuilderAPI_MakeEdge(p4, p1).Edge());       // p4→p1
                side4Wire.Add(BRepBuilderAPI_MakeEdge(p1, tp1).Edge());      // p1→tp1
                side4Wire.Add(BRepBuilderAPI_MakeEdge(tp1, tp4).Edge());     // tp1→tp4
                side4Wire.Add(BRepBuilderAPI_MakeEdge(tp4, p4).Edge());      // tp4→p4
                if (!side4Wire.IsDone())
                {
                    printLog(tr("Failed to create side4 wire!"), 3);
                    return false;
                }

                // 用Wire创建6个Face。
                BRepBuilderAPI_MakeFace bottomFace(bottomWire.Wire());
                BRepBuilderAPI_MakeFace topFace(topWire.Wire());
                BRepBuilderAPI_MakeFace side1Face(side1Wire.Wire());
                BRepBuilderAPI_MakeFace side2Face(side2Wire.Wire());
                BRepBuilderAPI_MakeFace side3Face(side3Wire.Wire());
                BRepBuilderAPI_MakeFace side4Face(side4Wire.Wire());

                if (!bottomFace.IsDone() || !topFace.IsDone() || !side1Face.IsDone() ||
                    !side2Face.IsDone() || !side3Face.IsDone() || !side4Face.IsDone())
                {
                    printLog(tr("Failed to create faces!"), 3);
                    return false;
                }

                // 使用布尔融合（Fuse）将6个面合并成一个形状
                // 布尔融合会自动处理边的共享问题
                BRepAlgoAPI_Fuse fuse1(bottomFace.Face(), topFace.Face());
                if (!fuse1.IsDone())
                {
                    printLog(tr("Failed to fuse bottom and top faces!"), 3);
                    return false;
                }

                BRepAlgoAPI_Fuse fuse2(fuse1.Shape(), side1Face.Face());
                if (!fuse2.IsDone())
                {
                    printLog(tr("Failed to fuse with side1!"), 3);
                    return false;
                }

                BRepAlgoAPI_Fuse fuse3(fuse2.Shape(), side2Face.Face());
                if (!fuse3.IsDone())
                {
                    printLog(tr("Failed to fuse with side2!"), 3);
                    return false;
                }

                BRepAlgoAPI_Fuse fuse4(fuse3.Shape(), side3Face.Face());
                if (!fuse4.IsDone())
                {
                    printLog(tr("Failed to fuse with side3!"), 3);
                    return false;
                }

                BRepAlgoAPI_Fuse fuse5(fuse4.Shape(), side4Face.Face());
                if (!fuse5.IsDone())
                {
                    printLog(tr("Failed to fuse with side4!"), 3);
                    return false;
                }

                // 从融合结果创建Shell
                // 由于融合结果只有面，需要用Builder创建Shell
                TopoDS_Builder shellBuilder;
                TopoDS_Shell newShell;
                shellBuilder.MakeShell(newShell);

                // 从融合结果中提取所有面并添加到Shell
                TopExp_Explorer faceExplorer(fuse5.Shape(), TopAbs_ShapeEnum::TopAbs_FACE);
                int faceCount = 0;
                for (; faceExplorer.More(); faceExplorer.Next())
                {
                    TopoDS_Face currentFace = TopoDS::Face(faceExplorer.Current());
                    shellBuilder.Add(newShell, currentFace);
                    faceCount++;
                }

                if (faceCount == 0)
                {
                    printLog(tr("No faces found in fused shape!"), 3);
                    return false;
                }

                // 从Shell创建Solid
                BRepBuilderAPI_MakeSolid solidMaker(newShell);
                if (!solidMaker.IsDone())
                {
                    printLog(tr("Failed to create solid from shell!"), 3);
                    return false;
                }

                output = solidMaker.Solid();
                if (output.IsNull())
                {
                    printLog(tr("Failed to create solid!"), 3);
                    return false;
                }
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

    FITKOCCModelSphere::FITKOCCModelSphere() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelSphere::update()
    {
        FITKOCCModelPart* part = _occShapeAgent->getPart();

        // 异常处理。
        try
        {
            //错误判断
            gp_Pnt pt(_location[0], _location[1], _location[2]);
            if (_radius < 1e-6)
            {
                printLog(tr("Invalid radius !"), 3);
                return false;
            }

            //创建球体
            BRepPrimAPI_MakeSphere cmd(pt, _radius);
            cmd.Build();
            if (!cmd.IsDone())
            {
                printLog(tr("Invalid parameters !"), 3);
                return false;
            }

            const TopoDS_Shape& output = cmd.Shape();
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
