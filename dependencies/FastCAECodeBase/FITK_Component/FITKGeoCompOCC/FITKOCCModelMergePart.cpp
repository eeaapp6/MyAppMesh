/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCModelMergePart.h"

// OCC
#include <TopoDS_Shape.hxx>
#include <TopoDS_Compound.hxx>
#include <BRep_Builder.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepAlgoAPI_Section.hxx>
#include <BOPAlgo_Splitter.hxx>
#include <ShapeFix_Shape.hxx>
#include <TopExp_Explorer.hxx>
#include <TopAbs.hxx>

#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITKOCCExtendTool.h"
#include "FITKOCCComandCommon.h"
#include "FITKOCCModelPart.h"

namespace OCC
{
    FITKOCCModelMergePart::FITKOCCModelMergePart() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelMergePart::update()
    {
        Interface::FITKGeoCommandList* geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (!geoCmdList)
        {
            return false;
        }

        // 还原上一次保存的部件ID列表，清除之前的引用关系。
        for (const int & cmdId : m_lastSavedCmdIds)
        {
            Interface::FITKAbsGeoCommand* cmd = geoCmdList->getDataByID(cmdId);
            if (!cmd)
            {
                continue;
            }

            // 移除引用关系。
            this->removeReferenceCmdObj(cmd);
        }

        bool flag = false;
        switch (m_boolOperType)
        {
        case GBT_Common:
        {
            flag = updateBoolCommon();
            break;
        }
        case GBT_Cut:
        {
            flag = updateBoolCut();
            break;
        }  
        case GBT_Add:
        {
            flag = updateBoolAdd();
            break;
        }
        default:
        {
            printLog(tr("Invalid Boolean operation type."), 3);
            return false;
        }
        }

        if (flag)
        {
            for (const int & cmdId : m_cmdIds)
            {
                Interface::FITKAbsGeoCommand* cmd = geoCmdList->getDataByID(cmdId);
                if (!cmd)
                {
                    return false;
                }

                // 添加引用关系。
                this->addReferenceCmdObj(cmd);
            }

            // 保存部件ID列表。
            m_lastSavedCmdIds = m_cmdIds;
        }

        return flag;
    }

    bool FITKOCCModelMergePart::updateBoolCommon()
    {
        if (m_cmdIds.size() < 2)
        {
            printLog(tr("The command ID list must contain at least two commands."), 3);
            return false;
        }

        FITKOCCModelPart* part = _occShapeAgent->getPart();
        if (!part)
        {
            return false;
        }

        Interface::FITKGeoCommandList* geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (!geoCmdList)
        {
            return false;
        }

        // 第一阶段：收集所有部件模型到各维度数组中。
        QList<TopoDS_Shape> all1D;
        QList<TopoDS_Shape> all2D;
        QList<TopoDS_Shape> all3D;

        for (int i = 0; i < m_cmdIds.size(); ++i)
        {
            Interface::FITKAbsGeoCommand* cmd = geoCmdList->getDataByID(m_cmdIds.at(i));
            if (!cmd)
            {
                return false;
            }

            FITKAbstractOCCModel* occModel = cmd->getTShapeAgent<FITKAbstractOCCModel>();
            if (!occModel)
            {
                return false;
            }

            TopoDS_Shape* shapePtr = occModel->getShape();
            if (!shapePtr || shapePtr->IsNull())
            {
                return false;
            }

            // 将部件模型平铺后添加到各维度数组中。
            QList<TopoDS_Shape> shapes1D, shapes2D, shapes3D;
            FITKOCCComandCommon::FlattenCompShape2D3D(*shapePtr, shapes1D, shapes2D, shapes3D);

            // 按维度分别进行融合。
            TopoDS_Shape fused1D, fused2D, fused3D;
            bool hasFused1D = false, hasFused2D = false, hasFused3D = false;

            // 融合所有一维边。
            for (const TopoDS_Shape& s : shapes1D)
            {
                if (s.IsNull() || FITKOCCExtendTool::IsEmptyShape(s))
                {
                    continue;
                }
                if (!hasFused1D)
                {
                    fused1D = s;
                    hasFused1D = true;
                }
                else
                {
                    try
                    {
                        BRepAlgoAPI_Fuse fuse(fused1D, s);
                        if (fuse.IsDone() && !fuse.Shape().IsNull())
                        {
                            fused1D = fuse.Shape();
                        }
                    }
                    catch (...) {}
                }
            }

            // 融合所有二维面。
            for (const TopoDS_Shape& s : shapes2D)
            {
                if (s.IsNull() || FITKOCCExtendTool::IsEmptyShape(s))
                {
                    continue;
                }
                if (!hasFused2D)
                {
                    fused2D = s;
                    hasFused2D = true;
                }
                else
                {
                    try
                    {
                        BRepAlgoAPI_Fuse fuse(fused2D, s);
                        if (fuse.IsDone() && !fuse.Shape().IsNull())
                        {
                            fused2D = fuse.Shape();
                        }
                    }
                    catch (...) {}
                }
            }

            // 融合所有三维实体。
            for (const TopoDS_Shape& s : shapes3D)
            {
                if (s.IsNull() || FITKOCCExtendTool::IsEmptyShape(s))
                {
                    continue;
                }
                if (!hasFused3D)
                {
                    fused3D = s;
                    hasFused3D = true;
                }
                else
                {
                    try
                    {
                        BRepAlgoAPI_Fuse fuse(fused3D, s);
                        if (fuse.IsDone() && !fuse.Shape().IsNull())
                        {
                            fused3D = fuse.Shape();
                        }
                    }
                    catch (...) {}
                }
            }

            // 将融合后的结果添加到各维度列表中。
            if (hasFused1D)
            {
                all1D.append(fused1D);
            }
            if (hasFused2D)
            {
                all2D.append(fused2D);
            }
            if (hasFused3D)
            {
                all3D.append(fused3D);
            }
        }

        // 第二阶段：各维度内部进行布尔交集运算（所有部件的交集）。
        BRep_Builder builder;
        TopoDS_Compound result3D, result2D, result1D;
        builder.MakeCompound(result3D);
        builder.MakeCompound(result2D);
        builder.MakeCompound(result1D);

        // 三维实体内部交集：先由第一个实体与其他所有实体依次求交，再将所有求交结果进行求交。
        if (all3D.size() >= 2)
        {
            // 首轮：第一个实体分别与其他所有实体求交。
            QList<TopoDS_Shape> firstRound3D;
            const TopoDS_Shape& firstShape3D = all3D.first();
            for (int i = 1; i < all3D.size(); ++i)
            {
                try
                {
                    BRepAlgoAPI_Common common(firstShape3D, all3D.at(i));
                    if (common.IsDone() && !common.Shape().IsNull() &&
                        !FITKOCCExtendTool::IsEmptyShape(common.Shape()))
                    {
                        firstRound3D.append(common.Shape());
                    }
                }
                catch (...) {}
            }

            // 将所有首轮求交结果进行求交。
            if (!firstRound3D.isEmpty())
            {
                TopoDS_Shape finalResult3D = firstRound3D.first();
                for (int i = 1; i < firstRound3D.size(); ++i)
                {
                    if (FITKOCCExtendTool::IsEmptyShape(finalResult3D))
                    {
                        break;
                    }
                    try
                    {
                        BRepAlgoAPI_Common common(finalResult3D, firstRound3D.at(i));
                        if (common.IsDone() && !common.Shape().IsNull() &&
                            !FITKOCCExtendTool::IsEmptyShape(common.Shape()))
                        {
                            finalResult3D = common.Shape();
                        }
                        else
                        {
                            finalResult3D.Nullify();
                            break;
                        }
                    }
                    catch (...) {}
                }
                if (!finalResult3D.IsNull() && !FITKOCCExtendTool::IsEmptyShape(finalResult3D))
                {
                    builder.Add(result3D, finalResult3D);
                }
            }
        }
        else if (all3D.size() == 1)
        {
            // 只有一个三维实体时，直接添加。
            if (!all3D.first().IsNull() && !FITKOCCExtendTool::IsEmptyShape(all3D.first()))
            {
                builder.Add(result3D, all3D.first());
            }
        }

        // 二维面内部交集：先由第一个面与其他所有面依次求交，再将所有求交结果进行求交。
        if (all2D.size() >= 2)
        {
            // 首轮：第一个面分别与其他所有面求交。
            QList<TopoDS_Shape> firstRound2D;
            const TopoDS_Shape& firstShape2D = all2D.first();
            for (int i = 1; i < all2D.size(); ++i)
            {
                try
                {
                    BRepAlgoAPI_Common common(firstShape2D, all2D.at(i));
                    if (common.IsDone() && !common.Shape().IsNull() &&
                        !FITKOCCExtendTool::IsEmptyShape(common.Shape()))
                    {
                        firstRound2D.append(common.Shape());
                    }
                }
                catch (...) {}
            }

            // 将所有首轮求交结果进行求交。
            if (!firstRound2D.isEmpty())
            {
                TopoDS_Shape finalResult2D = firstRound2D.first();
                for (int i = 1; i < firstRound2D.size(); ++i)
                {
                    if (FITKOCCExtendTool::IsEmptyShape(finalResult2D))
                    {
                        break;
                    }
                    try
                    {
                        BRepAlgoAPI_Common common(finalResult2D, firstRound2D.at(i));
                        if (common.IsDone() && !common.Shape().IsNull() &&
                            !FITKOCCExtendTool::IsEmptyShape(common.Shape()))
                        {
                            finalResult2D = common.Shape();
                        }
                        else
                        {
                            finalResult2D.Nullify();
                            break;
                        }
                    }
                    catch (...) {}
                }
                if (!finalResult2D.IsNull() && !FITKOCCExtendTool::IsEmptyShape(finalResult2D))
                {
                    builder.Add(result2D, finalResult2D);
                }
            }
        }
        else if (all2D.size() == 1)
        {
            // 只有一个二维面时，直接添加。
            if (!all2D.first().IsNull() && !FITKOCCExtendTool::IsEmptyShape(all2D.first()))
            {
                builder.Add(result2D, all2D.first());
            }
        }

        // 一维边内部交集：先由第一条边与其他所有边依次求交，再将所有求交结果进行求交。
        if (all1D.size() >= 2)
        {
            // 首轮：第一条边分别与其他所有边求交。
            QList<TopoDS_Shape> firstRound1D;
            const TopoDS_Shape& firstShape1D = all1D.first();
            for (int i = 1; i < all1D.size(); ++i)
            {
                try
                {
                    BRepAlgoAPI_Common common(firstShape1D, all1D.at(i));
                    if (common.IsDone() && !common.Shape().IsNull() &&
                        !FITKOCCExtendTool::IsEmptyShape(common.Shape()))
                    {
                        firstRound1D.append(common.Shape());
                    }
                }
                catch (...) {}
            }

            // 将所有首轮求交结果进行求交。
            if (!firstRound1D.isEmpty())
            {
                TopoDS_Shape finalResult1D = firstRound1D.first();
                for (int i = 1; i < firstRound1D.size(); ++i)
                {
                    if (FITKOCCExtendTool::IsEmptyShape(finalResult1D))
                    {
                        break;
                    }
                    try
                    {
                        BRepAlgoAPI_Common common(finalResult1D, firstRound1D.at(i));
                        if (common.IsDone() && !common.Shape().IsNull() &&
                            !FITKOCCExtendTool::IsEmptyShape(common.Shape()))
                        {
                            finalResult1D = common.Shape();
                        }
                        else
                        {
                            finalResult1D.Nullify();
                            break;
                        }
                    }
                    catch (...) {}
                }
                if (!finalResult1D.IsNull() && !FITKOCCExtendTool::IsEmptyShape(finalResult1D))
                {
                    builder.Add(result1D, finalResult1D);
                }
            }
        }
        else if (all1D.size() == 1)
        {
            // 只有一条一维边时，直接添加。
            if (!all1D.first().IsNull() && !FITKOCCExtendTool::IsEmptyShape(all1D.first()))
            {
                builder.Add(result1D, all1D.first());
            }
        }

        // 第三阶段：计算维度间的交线与交面。
        // 3D与2D之间的交面：3D与2D进行Section和Common计算，再进行Cut求差。
        // 只有当3D和2D同时存在时才计算。
        BRep_Builder builderInter3D2D;
        TopoDS_Compound inter3D2D;
        builderInter3D2D.MakeCompound(inter3D2D);
        bool hasInter3D2D = false;

        BRep_Builder builderResult3D2D;
        TopoDS_Compound result3D2D;
        builderResult3D2D.MakeCompound(result3D2D);

        // 从result3D中提取所有3D实体。
        QList<TopoDS_Shape> result3DShapes;
        TopExp_Explorer expResult3D(result3D, TopAbs_SOLID);
        for (; expResult3D.More(); expResult3D.Next())
        {
            result3DShapes.append(expResult3D.Current());
        }

        if (!result3DShapes.isEmpty() && !all2D.isEmpty())
        {
            for (const TopoDS_Shape& s3d : result3DShapes)
            {
                if (s3d.IsNull() || FITKOCCExtendTool::IsEmptyShape(s3d))
                {
                    continue;
                }

                // 将提取的壳或面与2D进行Section和Common计算。
                for (const TopoDS_Shape& s2d : all2D)
                {
                    if (s2d.IsNull() || FITKOCCExtendTool::IsEmptyShape(s2d))
                    {
                        continue;
                    }
                    try
                    {
                        BRepAlgoAPI_Section intSecLine(s3d, s2d);
                        intSecLine.Build();
                        if (!intSecLine.IsDone())
                        {
                            continue;
                        }

                        BRepAlgoAPI_Common intSec(s3d, s2d);
                        intSec.Build();
                        if (!intSec.IsDone())
                        {
                            continue;
                        }

                        const TopoDS_Shape& sec = intSec.Shape();
                        if (FITKOCCExtendTool::IsEmptyShape(sec))
                        {
                            continue;
                        }

                        builderResult3D2D.Add(result3D2D, sec);
                        builderInter3D2D.Add(inter3D2D, intSecLine.Shape());
                        hasInter3D2D = true;
                    }
                    catch (...) {}
                }
            }
        }

        // 1D与3D之间的交线/交点。只有当1D和3D同时存在时才计算。
        BRep_Builder builderInter1D3D;
        TopoDS_Compound inter1D3D;
        builderInter1D3D.MakeCompound(inter1D3D);
        bool hasInter1D3D = false;

        BRep_Builder builderResult1D;
        TopoDS_Compound result1DNew;
        builderResult1D.MakeCompound(result1DNew);

        // 从result1D中提取所有1D边。
        QList<TopoDS_Shape> result1DEdges;
        TopExp_Explorer expResult1D(result1D, TopAbs_EDGE);
        for (; expResult1D.More(); expResult1D.Next())
        {
            result1DEdges.append(expResult1D.Current());
        }

        if (!result1DEdges.isEmpty() && !result3DShapes.isEmpty())
        {
            for (const TopoDS_Shape& s1d : result1DEdges)
            {
                for (const TopoDS_Shape& s3d : result3DShapes)
                {
                    if (s1d.IsNull() || s3d.IsNull() ||
                        FITKOCCExtendTool::IsEmptyShape(s1d) || FITKOCCExtendTool::IsEmptyShape(s3d))
                    {
                        continue;
                    }
                    try
                    {
                        BRepAlgoAPI_Section intSecLine1D3D(s1d, s3d);
                        intSecLine1D3D.Build();
                        if (!intSecLine1D3D.IsDone())
                        {
                            continue;
                        }

                        BRepAlgoAPI_Common intSec1D3D(s1d, s3d);
                        intSec1D3D.Build();
                        if (!intSec1D3D.IsDone())
                        {
                            continue;
                        }

                        const TopoDS_Shape& sec = intSec1D3D.Shape();
                        if (FITKOCCExtendTool::IsEmptyShape(sec))
                        {
                            continue;
                        }

                        builderResult1D.Add(result1DNew, sec);
                        builderInter1D3D.Add(inter1D3D, intSecLine1D3D.Shape());
                        hasInter1D3D = true;
                    }
                    catch (...) {}
                }
            }
        }

        // 2D与1D之间的交线/交点。只有当2D和1D同时存在时才计算。
        BRep_Builder builderInter2D1D;
        TopoDS_Compound inter2D1D;
        builderInter2D1D.MakeCompound(inter2D1D);
        bool hasInter2D1D = false;

        BRep_Builder builderResult2D1D;
        TopoDS_Compound result2D1D;
        builderResult2D1D.MakeCompound(result2D1D);

        // 从result2D中提取所有2D面。
        QList<TopoDS_Shape> result2DFaces;
        TopExp_Explorer expResult2D(result2D, TopAbs_FACE);
        for (; expResult2D.More(); expResult2D.Next())
        {
            result2DFaces.append(expResult2D.Current());
        }

        if (!result2DFaces.isEmpty() && !result1DEdges.isEmpty())
        {
            for (const TopoDS_Shape& s2d : result2DFaces)
            {
                for (const TopoDS_Shape& s1d : result1DEdges)
                {
                    if (s2d.IsNull() || s1d.IsNull() ||
                        FITKOCCExtendTool::IsEmptyShape(s2d) || FITKOCCExtendTool::IsEmptyShape(s1d))
                    {
                        continue;
                    }
                    try
                    {
                        BRepAlgoAPI_Section intSecLine2D1D(s2d, s1d);
                        intSecLine2D1D.Build();
                        if (!intSecLine2D1D.IsDone())
                        {
                            continue;
                        }

                        BRepAlgoAPI_Common intSec2D1D(s2d, s1d);
                        intSec2D1D.Build();
                        if (!intSec2D1D.IsDone())
                        {
                            continue;
                        }

                        const TopoDS_Shape& sec = intSec2D1D.Shape();
                        if (FITKOCCExtendTool::IsEmptyShape(sec))
                        {
                            continue;
                        }

                        builderResult2D1D.Add(result2D1D, sec);
                        builderInter2D1D.Add(inter2D1D, intSecLine2D1D.Shape());
                        hasInter2D1D = true;
                    }
                    catch (...) {}
                }
            }
        }

        // 第四阶段：合并所有维度的结果。
        // 由于是求交操作，根据各维度模型存在与否决定如何添加结果。
        BRep_Builder resultBuilder;
        TopoDS_Compound resultCompound;
        resultBuilder.MakeCompound(resultCompound);

        // 判断各维度是否有模型。
        const bool has3D = !result3D.IsNull() && !FITKOCCExtendTool::IsEmptyShape(result3D);
        const bool has2D = !result2D.IsNull() && !FITKOCCExtendTool::IsEmptyShape(result2D);
        const bool has1D = !result1D.IsNull() && !FITKOCCExtendTool::IsEmptyShape(result1D);
        const bool hasInter3D2DResult = !result3D2D.IsNull() && !FITKOCCExtendTool::IsEmptyShape(result3D2D);
        const bool hasInter1D3DResult = !result1DNew.IsNull() && !FITKOCCExtendTool::IsEmptyShape(result1DNew);
        const bool hasInter2D1DResult = !result2D1D.IsNull() && !FITKOCCExtendTool::IsEmptyShape(result2D1D);

        // 如果只有3D存在（2D和1D都不存在），则直接添加result3D。
        if (has3D && !has2D && !has1D)
        {
            resultBuilder.Add(resultCompound, result3D);
        }
        // 如果只有2D存在（3D和1D都不存在），则直接添加result2D。
        else if (!has3D && has2D && !has1D)
        {
            resultBuilder.Add(resultCompound, result2D);
        }
        // 如果只有1D存在（3D和2D都不存在），则直接添加result1D。
        else if (!has3D && !has2D && has1D)
        {
            resultBuilder.Add(resultCompound, result1D);
        }
        // 否则（存在多个维度的模型），需要添加维度间的交线。
        else
        {
            // 添加3D与2D求差后的结果和交线。
            if (hasInter3D2DResult)
            {
                resultBuilder.Add(resultCompound, result3D2D);
            }
            if (hasInter3D2D)
            {
                resultBuilder.Add(resultCompound, inter3D2D);
            }

            // 添加1D与3D的交线。
            if (hasInter1D3DResult)
            {
                resultBuilder.Add(resultCompound, result1DNew);
            }
            if (hasInter1D3D)
            {
                resultBuilder.Add(resultCompound, inter1D3D);
            }

            // 添加2D与1D的交线。
            if (hasInter2D1DResult)
            {
                resultBuilder.Add(resultCompound, result2D1D);
            }
            if (hasInter2D1D)
            {
                resultBuilder.Add(resultCompound, inter2D1D);
            }
        }

        if (FITKOCCExtendTool::IsEmptyShape(resultCompound))
        {
            return false;
        }

        if (part)
        {
            part->updatePartShape(resultCompound, false);
            _occShapeAgent->updateShape(part->getCopiedPartShape());
        }
        else
        {
            _occShapeAgent->updateShape(resultCompound);
        }

        return true;
    }

    bool FITKOCCModelMergePart::updateBoolCut()
    {
        if (m_cmdIds.size() < 2)
        {
            printLog(tr("The command ID list must contain at least two commands."), 3);
            return false;
        }

        FITKOCCModelPart* part = _occShapeAgent->getPart();
        if (!part)
        {
            return false;
        }

        Interface::FITKGeoCommandList* geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (!geoCmdList)
        {
            return false;
        }

        Interface::FITKAbsGeoCommand* command = geoCmdList->getDataByID(m_cmdIds.first());
        if (!command)
        {
            return false;
        }

        FITKAbstractOCCModel* occModel = command->getTShapeAgent<FITKAbstractOCCModel>();
        if (!occModel)
        {
            return false;
        }

        TopoDS_Shape* firstShapePtr = occModel->getShape();
        if (!firstShapePtr || firstShapePtr->IsNull())
        {
            return false;
        }

        // 第一阶段：收集所有部件模型到各维度数组中。
        QList<TopoDS_Shape> all1D;
        QList<TopoDS_Shape> all2D;
        QList<TopoDS_Shape> all3D;

        for (int i = 0; i < m_cmdIds.size(); ++i)
        {
            Interface::FITKAbsGeoCommand* cmd = geoCmdList->getDataByID(m_cmdIds.at(i));
            if (!cmd)
            {
                return false;
            }

            FITKAbstractOCCModel* occModel = cmd->getTShapeAgent<FITKAbstractOCCModel>();
            if (!occModel)
            {
                return false;
            }

            TopoDS_Shape* shapePtr = occModel->getShape();
            if (!shapePtr || shapePtr->IsNull())
            {
                return false;
            }

            // 将部件模型平铺后添加到各维度数组中。
            QList<TopoDS_Shape> shapes1D, shapes2D, shapes3D;
            FITKOCCComandCommon::FlattenCompShape2D3D(*shapePtr, shapes1D, shapes2D, shapes3D);

            // 按维度分别进行融合。
            TopoDS_Shape fused1D, fused2D, fused3D;
            bool hasFused1D = false, hasFused2D = false, hasFused3D = false;

            // 融合所有一维边。
            for (const TopoDS_Shape& s : shapes1D)
            {
                if (s.IsNull() || FITKOCCExtendTool::IsEmptyShape(s))
                {
                    continue;
                }
                if (!hasFused1D)
                {
                    fused1D = s;
                    hasFused1D = true;
                }
                else
                {
                    try
                    {
                        BRepAlgoAPI_Fuse fuse(fused1D, s);
                        if (fuse.IsDone() && !fuse.Shape().IsNull())
                        {
                            fused1D = fuse.Shape();
                        }
                    }
                    catch (...) {}
                }
            }

            // 融合所有二维面。
            for (const TopoDS_Shape& s : shapes2D)
            {
                if (s.IsNull() || FITKOCCExtendTool::IsEmptyShape(s))
                {
                    continue;
                }
                if (!hasFused2D)
                {
                    fused2D = s;
                    hasFused2D = true;
                }
                else
                {
                    try
                    {
                        BRepAlgoAPI_Fuse fuse(fused2D, s);
                        if (fuse.IsDone() && !fuse.Shape().IsNull())
                        {
                            fused2D = fuse.Shape();
                        }
                    }
                    catch (...) {}
                }
            }

            // 融合所有三维实体。
            for (const TopoDS_Shape& s : shapes3D)
            {
                if (s.IsNull() || FITKOCCExtendTool::IsEmptyShape(s))
                {
                    continue;
                }
                if (!hasFused3D)
                {
                    fused3D = s;
                    hasFused3D = true;
                }
                else
                {
                    try
                    {
                        BRepAlgoAPI_Fuse fuse(fused3D, s);
                        if (fuse.IsDone() && !fuse.Shape().IsNull())
                        {
                            fused3D = fuse.Shape();
                        }
                    }
                    catch (...) {}
                }
            }

            // 将融合后的结果添加到各维度列表中。
            if (hasFused1D)
            {
                all1D.append(fused1D);
            }
            if (hasFused2D)
            {
                all2D.append(fused2D);
            }
            if (hasFused3D)
            {
                all3D.append(fused3D);
            }
        }

        // 第二阶段：各维度内部进行布尔差集运算（第一个命令的部件减去其他命令的部件）。
        // 第一条命令的部件先融合，后续命令的部件直接做差集。
        TopoDS_Shape result3D, result2D, result1D;
        bool hasResult3D = false, hasResult2D = false, hasResult1D = false;

        // 遍历所有部件。
        for (int i = 0; i < m_cmdIds.size(); ++i)
        {
            Interface::FITKAbsGeoCommand* cmd = geoCmdList->getDataByID(m_cmdIds.at(i));
            if (!cmd)
            {
                continue;
            }

            FITKAbstractOCCModel* occModel = cmd->getTShapeAgent<FITKAbstractOCCModel>();
            if (!occModel)
            {
                continue;
            }

            TopoDS_Shape* shapePtr = occModel->getShape();
            if (!shapePtr || shapePtr->IsNull())
            {
                continue;
            }

            QList<TopoDS_Shape> shapes1D, shapes2D, shapes3D;
            FITKOCCComandCommon::FlattenCompShape2D3D(*shapePtr, shapes1D, shapes2D, shapes3D);

            // 处理第一条命令：先融合。
            if (i == 0)
            {
                // 融合第一条命令的所有一维边。
                for (const TopoDS_Shape& s : shapes1D)
                {
                    if (s.IsNull() || FITKOCCExtendTool::IsEmptyShape(s))
                    {
                        continue;
                    }
                    if (!hasResult1D)
                    {
                        result1D = s;
                        hasResult1D = true;
                    }
                    else
                    {
                        try
                        {
                            BRepAlgoAPI_Fuse fuse(result1D, s);
                            if (fuse.IsDone() && !fuse.Shape().IsNull())
                            {
                                result1D = fuse.Shape();
                            }
                        }
                        catch (...) {}
                    }
                }

                // 融合第一条命令的所有二维面。
                for (const TopoDS_Shape& s : shapes2D)
                {
                    if (s.IsNull() || FITKOCCExtendTool::IsEmptyShape(s))
                    {
                        continue;
                    }
                    if (!hasResult2D)
                    {
                        result2D = s;
                        hasResult2D = true;
                    }
                    else
                    {
                        try
                        {
                            BRepAlgoAPI_Fuse fuse(result2D, s);
                            if (fuse.IsDone() && !fuse.Shape().IsNull())
                            {
                                result2D = fuse.Shape();
                            }
                        }
                        catch (...) {}
                    }
                }

                // 融合第一条命令的所有三维实体。
                for (const TopoDS_Shape& s : shapes3D)
                {
                    if (s.IsNull() || FITKOCCExtendTool::IsEmptyShape(s))
                    {
                        continue;
                    }
                    if (!hasResult3D)
                    {
                        result3D = s;
                        hasResult3D = true;
                    }
                    else
                    {
                        try
                        {
                            BRepAlgoAPI_Fuse fuse(result3D, s);
                            if (fuse.IsDone() && !fuse.Shape().IsNull())
                            {
                                result3D = fuse.Shape();
                            }
                        }
                        catch (...) {}
                    }
                }
            }
            // 处理后续命令：直接做差集。
            else
            {
                // 处理一维边：先融合当前命令的一维边，然后与结果做差集。
                TopoDS_Shape rest1D;
                bool hasRest1D = false;
                for (const TopoDS_Shape& s : shapes1D)
                {
                    if (s.IsNull() || FITKOCCExtendTool::IsEmptyShape(s))
                    {
                        continue;
                    }
                    if (!hasRest1D)
                    {
                        rest1D = s;
                        hasRest1D = true;
                    }
                    else
                    {
                        try
                        {
                            BRepAlgoAPI_Fuse fuse(rest1D, s);
                            if (fuse.IsDone() && !fuse.Shape().IsNull())
                            {
                                rest1D = fuse.Shape();
                            }
                        }
                        catch (...) {}
                    }
                }

                if (hasRest1D && hasResult1D)
                {
                    try
                    {
                        BRepAlgoAPI_Section secLine(result1D, rest1D);
                        secLine.Build();
                        if (secLine.IsDone() && !FITKOCCExtendTool::IsEmptyShape(secLine.Shape()))
                        {
                            BRepAlgoAPI_Common intSec(result1D, rest1D);
                            if (intSec.IsDone() && !intSec.Shape().IsNull())
                            {
                                BRepAlgoAPI_Cut cut(result1D, intSec.Shape());
                                if (cut.IsDone() && !cut.Shape().IsNull())
                                {
                                    result1D = cut.Shape();
                                }
                            }
                        }
                    }
                    catch (...) {}
                }

                // 处理二维面：先融合当前命令的二维面，然后与结果做差集。
                TopoDS_Shape rest2D;
                bool hasRest2D = false;
                for (const TopoDS_Shape& s : shapes2D)
                {
                    if (s.IsNull() || FITKOCCExtendTool::IsEmptyShape(s))
                    {
                        continue;
                    }
                    if (!hasRest2D)
                    {
                        rest2D = s;
                        hasRest2D = true;
                    }
                    else
                    {
                        try
                        {
                            BRepAlgoAPI_Fuse fuse(rest2D, s);
                            if (fuse.IsDone() && !fuse.Shape().IsNull())
                            {
                                rest2D = fuse.Shape();
                            }
                        }
                        catch (...) {}
                    }
                }

                if (hasRest2D && hasResult2D)
                {
                    try
                    {
                        BRepAlgoAPI_Cut cut(result2D, rest2D);
                        if (cut.IsDone() && !cut.Shape().IsNull())
                        {
                            result2D = cut.Shape();
                        }
                    }
                    catch (...) {}
                }

                // 处理三维实体：先融合当前命令的三维实体，然后与结果做差集。
                TopoDS_Shape rest3D;
                bool hasRest3D = false;
                for (const TopoDS_Shape& s : shapes3D)
                {
                    if (s.IsNull() || FITKOCCExtendTool::IsEmptyShape(s))
                    {
                        continue;
                    }
                    if (!hasRest3D)
                    {
                        rest3D = s;
                        hasRest3D = true;
                    }
                    else
                    {
                        try
                        {
                            BRepAlgoAPI_Fuse fuse(rest3D, s);
                            if (fuse.IsDone() && !fuse.Shape().IsNull())
                            {
                                rest3D = fuse.Shape();
                            }
                        }
                        catch (...) {}
                    }
                }

                if (hasRest3D && hasResult3D)
                {
                    try
                    {
                        BRepAlgoAPI_Cut cut(result3D, rest3D);
                        if (cut.IsDone() && !cut.Shape().IsNull())
                        {
                            result3D = cut.Shape();
                        }
                    }
                    catch (...) {}
                }

                // 跨维度差集处理：用1D边去切3D实体。
                if (hasRest1D && hasResult3D)
                {
                    try
                    {
                        BRepAlgoAPI_Section secLine(rest1D, result3D);
                        secLine.Build();
                        if (secLine.IsDone() && !FITKOCCExtendTool::IsEmptyShape(secLine.Shape()))
                        {
                            BRepAlgoAPI_Common intSec(rest1D, result3D);
                            if (intSec.IsDone() && !intSec.Shape().IsNull())
                            {
                                BRepAlgoAPI_Cut cut(result3D, intSec.Shape());
                                if (cut.IsDone() && !cut.Shape().IsNull())
                                {
                                    result3D = cut.Shape();
                                }
                            }
                        }
                    }
                    catch (...) {}
                }

                // 跨维度差集处理：用2D面去切3D实体。
                if (hasRest2D && hasResult3D)
                {
                    try
                    {
                        BRepAlgoAPI_Section secLine(rest2D, result3D);
                        secLine.Build();
                        if (secLine.IsDone() && !FITKOCCExtendTool::IsEmptyShape(secLine.Shape()))
                        {
                            BRepAlgoAPI_Common intSec(rest2D, result3D);
                            if (intSec.IsDone() && !intSec.Shape().IsNull())
                            {
                                BRepAlgoAPI_Cut cut(result3D, intSec.Shape());
                                if (cut.IsDone() && !cut.Shape().IsNull())
                                {
                                    result3D = cut.Shape();
                                }
                            }
                        }
                    }
                    catch (...) {}
                }

                // 跨维度差集处理：用1D边去切2D面。
                if (hasRest1D && hasResult2D)
                {
                    try
                    {
                        BRepAlgoAPI_Section secLine(rest1D, result2D);
                        secLine.Build();
                        if (secLine.IsDone() && !FITKOCCExtendTool::IsEmptyShape(secLine.Shape()))
                        {
                            BRepAlgoAPI_Common intSec(rest1D, result2D);
                            if (intSec.IsDone() && !intSec.Shape().IsNull())
                            {
                                BRepAlgoAPI_Cut cut(result2D, intSec.Shape());
                                if (cut.IsDone() && !cut.Shape().IsNull())
                                {
                                    result2D = cut.Shape();
                                }
                            }
                        }
                    }
                    catch (...) {}
                }
            }
        }

        // 第三阶段：计算维度间的交线与交面（使用差集后的各维度结果）。
        // 3D与2D之间的交面/交线。
        BRep_Builder builderInter3D2D;
        TopoDS_Compound inter3D2D;
        builderInter3D2D.MakeCompound(inter3D2D);

        BRep_Builder builderResult3D2D;
        TopoDS_Compound result3D2D;
        builderResult3D2D.MakeCompound(result3D2D);

        // 从result3D中提取所有3D实体。
        QList<TopoDS_Shape> result3DShapes;
        TopExp_Explorer expResult3D(result3D, TopAbs_SOLID);
        for (; expResult3D.More(); expResult3D.Next())
        {
            result3DShapes.append(expResult3D.Current());
        }

        // 从result2D中提取所有2D面。
        QList<TopoDS_Shape> result2DFaces;
        TopExp_Explorer expResult2D(result2D, TopAbs_FACE);
        for (; expResult2D.More(); expResult2D.Next())
        {
            result2DFaces.append(expResult2D.Current());
        }

        if (!result3DShapes.isEmpty() && !result2DFaces.isEmpty())
        {
            for (const TopoDS_Shape& s3d : result3DShapes)
            {
                for (const TopoDS_Shape& s2d : result2DFaces)
                {
                    if (s3d.IsNull() || s2d.IsNull() ||
                        FITKOCCExtendTool::IsEmptyShape(s3d) || FITKOCCExtendTool::IsEmptyShape(s2d))
                    {
                        continue;
                    }
                    try
                    {
                        BRepAlgoAPI_Section intSecLine3D2D(s3d, s2d);
                        intSecLine3D2D.Build();
                        if (!intSecLine3D2D.IsDone())
                        {
                            continue;
                        }

                        BRepAlgoAPI_Common intSec3D2D(s3d, s2d);
                        intSec3D2D.Build();
                        if (!intSec3D2D.IsDone())
                        {
                            continue;
                        }

                        const TopoDS_Shape& sec = intSec3D2D.Shape();
                        if (FITKOCCExtendTool::IsEmptyShape(sec))
                        {
                            continue;
                        }

                        builderResult3D2D.Add(result3D2D, sec);
                        builderInter3D2D.Add(inter3D2D, intSecLine3D2D.Shape());
                    }
                    catch (...) {}
                }
            }
        }

        // 1D与3D之间的交线/交点。
        BRep_Builder builderInter1D3D;
        TopoDS_Compound inter1D3D;
        builderInter1D3D.MakeCompound(inter1D3D);

        BRep_Builder builderResult1D3D;
        TopoDS_Compound result1D3D;
        builderResult1D3D.MakeCompound(result1D3D);

        // 从result1D中提取所有1D边。
        QList<TopoDS_Shape> result1DEdges;
        TopExp_Explorer expResult1D(result1D, TopAbs_EDGE);
        for (; expResult1D.More(); expResult1D.Next())
        {
            result1DEdges.append(expResult1D.Current());
        }

        if (!result1DEdges.isEmpty() && !result3DShapes.isEmpty())
        {
            for (const TopoDS_Shape& s1d : result1DEdges)
            {
                for (const TopoDS_Shape& s3d : result3DShapes)
                {
                    if (s1d.IsNull() || s3d.IsNull() ||
                        FITKOCCExtendTool::IsEmptyShape(s1d) || FITKOCCExtendTool::IsEmptyShape(s3d))
                    {
                        continue;
                    }
                    try
                    {
                        BRepAlgoAPI_Section intSecLine1D3D(s1d, s3d);
                        intSecLine1D3D.Build();
                        if (!intSecLine1D3D.IsDone())
                        {
                            continue;
                        }

                        BRepAlgoAPI_Common intSec1D3D(s1d, s3d);
                        intSec1D3D.Build();
                        if (!intSec1D3D.IsDone())
                        {
                            continue;
                        }

                        const TopoDS_Shape& sec = intSec1D3D.Shape();
                        if (FITKOCCExtendTool::IsEmptyShape(sec))
                        {
                            continue;
                        }

                        builderResult1D3D.Add(result1D3D, sec);
                        builderInter1D3D.Add(inter1D3D, intSecLine1D3D.Shape());
                    }
                    catch (...) {}
                }
            }
        }

        // 2D与1D之间的交线/交点。
        BRep_Builder builderInter2D1D;
        TopoDS_Compound inter2D1D;
        builderInter2D1D.MakeCompound(inter2D1D);

        BRep_Builder builderResult2D1D;
        TopoDS_Compound result2D1D;
        builderResult2D1D.MakeCompound(result2D1D);

        if (!result2DFaces.isEmpty() && !result1DEdges.isEmpty())
        {
            for (const TopoDS_Shape& s2d : result2DFaces)
            {
                for (const TopoDS_Shape& s1d : result1DEdges)
                {
                    if (s2d.IsNull() || s1d.IsNull() ||
                        FITKOCCExtendTool::IsEmptyShape(s2d) || FITKOCCExtendTool::IsEmptyShape(s1d))
                    {
                        continue;
                    }
                    try
                    {
                        BRepAlgoAPI_Section intSecLine2D1D(s2d, s1d);
                        intSecLine2D1D.Build();
                        if (!intSecLine2D1D.IsDone())
                        {
                            continue;
                        }

                        BRepAlgoAPI_Common intSec2D1D(s2d, s1d);
                        intSec2D1D.Build();
                        if (!intSec2D1D.IsDone())
                        {
                            continue;
                        }

                        const TopoDS_Shape& sec = intSec2D1D.Shape();
                        if (FITKOCCExtendTool::IsEmptyShape(sec))
                        {
                            continue;
                        }

                        builderResult2D1D.Add(result2D1D, sec);
                        builderInter2D1D.Add(inter2D1D, intSecLine2D1D.Shape());
                    }
                    catch (...) {}
                }
            }
        }

        // 第四阶段：使用交线与交面对各维度结果进行互相分割。
        TopoDS_Shape final3D = result3D;
        TopoDS_Shape final2D = result2D;
        TopoDS_Shape final1D = result1D;

        // 使用3D与2D的交面/交线进行互相分割。
        if (!FITKOCCExtendTool::IsEmptyShape(inter3D2D))
        {
            // 用3D与2D的交线分割2D。
            try
            {
                BOPAlgo_Splitter splitter2D;
                splitter2D.AddArgument(result2D);
                splitter2D.AddTool(inter3D2D);
                splitter2D.Perform();
                if (!splitter2D.Shape().IsNull())
                {
                    final2D = splitter2D.Shape();
                }
            }
            catch (...) {}

            // 用3D与2D的交线分割3D。
            try
            {
                BOPAlgo_Splitter splitter3D;
                splitter3D.AddArgument(result3D);
                splitter3D.AddTool(inter3D2D);
                splitter3D.Perform();
                if (!splitter3D.Shape().IsNull())
                {
                    final3D = splitter3D.Shape();
                }
            }
            catch (...) {}
        }

        // 使用1D与3D的交线/交点进行互相分割。
        if (!FITKOCCExtendTool::IsEmptyShape(inter1D3D))
        {
            // 用1D与3D的交线分割1D。
            try
            {
                BOPAlgo_Splitter splitter1D;
                splitter1D.AddArgument(result1D);
                splitter1D.AddTool(inter1D3D);
                splitter1D.Perform();
                if (!splitter1D.Shape().IsNull())
                {
                    final1D = splitter1D.Shape();
                }
            }
            catch (...) {}

            // 用1D与3D的交线分割3D。
            try
            {
                BOPAlgo_Splitter splitter3D;
                splitter3D.AddArgument(final3D);
                splitter3D.AddTool(inter1D3D);
                splitter3D.Perform();
                if (!splitter3D.Shape().IsNull())
                {
                    final3D = splitter3D.Shape();
                }
            }
            catch (...) {}
        }

        // 使用2D与1D的交线/交点进行互相分割。
        if (!FITKOCCExtendTool::IsEmptyShape(inter2D1D))
        {
            // 用2D与1D的交线分割1D。
            try
            {
                result1D = BRepAlgoAPI_Cut(result1D, inter2D1D);
            }
            catch (...) {}

            // 用2D与1D的结果分割2D。
            try
            {
                BOPAlgo_Splitter splitter2D;
                splitter2D.AddArgument(result2D);
                splitter2D.AddTool(result1D);
                splitter2D.Perform();
                if (!splitter2D.Shape().IsNull())
                {
                    result2D = splitter2D.Shape();
                }
            }
            catch (...) {}
        }

        // 合并所有维度的结果。
        BRep_Builder resultBuilder;
        TopoDS_Compound resultCompound;
        resultBuilder.MakeCompound(resultCompound);
        if (!final3D.IsNull() && !FITKOCCExtendTool::IsEmptyShape(final3D))
        {
            resultBuilder.Add(resultCompound, final3D);
        }
        if (!final2D.IsNull() && !FITKOCCExtendTool::IsEmptyShape(final2D))
        {
            resultBuilder.Add(resultCompound, final2D);
        }
        if (!final1D.IsNull() && !FITKOCCExtendTool::IsEmptyShape(final1D))
        {
            resultBuilder.Add(resultCompound, final1D);
        }

        //if (FITKOCCExtendTool::IsEmptyShape(resultCompound))
        //{
        //    return false;
        //}

        if (part)
        {
            part->updatePartShape(resultCompound, false);
            _occShapeAgent->updateShape(part->getCopiedPartShape());
        }
        else
        {
            _occShapeAgent->updateShape(resultCompound);
        }

        return true;
    }

    bool FITKOCCModelMergePart::updateBoolAdd()
    {
        if (m_cmdIds.size() < 2)
        {
            printLog(tr("The command ID list must contain at least two commands."), 3);
            return false;
        }

        FITKOCCModelPart* part = _occShapeAgent->getPart();
        if (!part)
        {
            return false;
        }

        Interface::FITKGeoCommandList* geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        if (!geoCmdList)
        {
            return false;
        }

        // 收集所有部件模型。
        QList<TopoDS_Shape> shapes;
        for (int i = 0; i < m_cmdIds.size(); ++i)
        {
            Interface::FITKAbsGeoCommand* cmd = geoCmdList->getDataByID(m_cmdIds.at(i));
            if (!cmd)
            {
                return false;
            }

            FITKAbstractOCCModel* occModel = cmd->getTShapeAgent<FITKAbstractOCCModel>();
            if (!occModel)
            {
                return false;
            }

            TopoDS_Shape* shapePtr = occModel->getShape();
            if (!shapePtr)
            {
                return false;
            }

            if (!FITKOCCExtendTool::IsEmptyShape(*shapePtr))
            {
                shapes.push_back(*shapePtr);
            }
        }

        // 融合模型。
        TopoDS_Shape result;

        bool merge = true;
        if (part)
        {
            merge = part->getAutoMergeSolid();
        }

        bool flag = FITKOCCComandCommon::MergeShapes(shapes, result, !merge);
        if (!flag)
        {
            return false;
        }

        if (part)
        {
            part->updatePartShape(result, false);
            _occShapeAgent->updateShape(part->getCopiedPartShape());
        }
        else
        {
            _occShapeAgent->updateShape(result);
        }

        return true;
    }
}
