/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCComandCommon.h"

#include <BRepBuilderAPI_MakeWire.hxx>
#include <ShapeFix_Wire.hxx>
#include <BRepBndLib.hxx>
#include <Bnd_Box.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepCheck_Analyzer.hxx>
#include <ShapeAnalysis.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <ShapeFix_Shape.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <IntTools_FClass2d.hxx>
#include <ShapeAnalysis_Surface.hxx>
#include <BRep_Tool.hxx>
#include <BRep_Builder.hxx>
#include <vector>
#include <algorithm>
#include <BRepBuilderAPI_MakeSolid.hxx>
#include <BRepTools_ReShape.hxx>
#include <BRepBuilderAPI_Copy.hxx>
#include "FITKOCCModelRefine.h"
#include <QDebug> 
#include <Geom_TrimmedCurve.hxx>
#include <GeomConvert.hxx>
#include <GeomAdaptor_Curve.hxx>
#include <GCPnts_UniformAbscissa.hxx>
#include <CPnts_AbscissaPoint.hxx>
#include <BRepGProp.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <TopTools_HSequenceOfShape.hxx>
#include <ShapeAnalysis_FreeBounds.hxx>
#include <TopoDS_Builder.hxx>
#include <Geom_Plane.hxx>
#include <GeomLib_Tool.hxx>
#include <GProp_GProps.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepAlgoAPI_Section.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include <BOPAlgo_Splitter.hxx>
#include <ShapeUpgrade_UnifySameDomain.hxx>
#include <ShapeCustom_Surface.hxx>
#include <ShapeFix.hxx>

#include "FITKOCCExtendTool.h"

namespace OCC
{
    void FITKOCCComandCommon::FlattenCompShape(const TopoDS_Shape & shape, QList<TopoDS_Shape> & shapes)
    {
        if (shape.IsNull())
        {
            return;
        }

        TopAbs_ShapeEnum sType = shape.ShapeType();
        switch (sType)
        {
        case TopAbs_COMPOUND:
        case TopAbs_COMPSOLID:
        {
            // 遍历子形状。
            TopoDS_Iterator iter(shape);
            for (; iter.More(); iter.Next())
            {
                // 获取子形状递归拆分形状及信息。
                const TopoDS_Shape & subShape = iter.Value();
                FlattenCompShape(subShape, shapes);
            }

            break;
        }
        case TopAbs_SOLID:
        case TopAbs_SHELL:
        case TopAbs_FACE:
        case TopAbs_WIRE:
        case TopAbs_EDGE:
        case TopAbs_VERTEX:
        {
            shapes.push_back(shape);
            break;
        }
        default:
            break;
        }
    }

    void FITKOCCComandCommon::FlattenCompShape2D(const TopoDS_Shape & shape, QList<TopoDS_Shape> & shapes, QList<TopoDS_Shape> & shapes2D)
    {
        if (shape.IsNull())
        {
            return;
        }

        TopAbs_ShapeEnum sType = shape.ShapeType();
        switch (sType)
        {
        case TopAbs_COMPOUND:
        case TopAbs_COMPSOLID:
        {
            // 遍历子形状。
            TopoDS_Iterator iter(shape);
            for (; iter.More(); iter.Next())
            {
                // 获取子形状递归拆分形状及信息。
                const TopoDS_Shape & subShape = iter.Value();
                FlattenCompShape2D(subShape, shapes, shapes2D);
            }

            break;
        }
        case TopAbs_SHELL:
        case TopAbs_FACE:
        {
            shapes2D.push_back(shape);
            break;
        }
        case TopAbs_SOLID:
        case TopAbs_WIRE:
        case TopAbs_EDGE:
        case TopAbs_VERTEX:
        {
            shapes.push_back(shape);
            break;
        }
        default:
            break;
        }
    }

    void FITKOCCComandCommon::FlattenCompShape3D(const TopoDS_Shape & shape, QList<TopoDS_Shape> & shapes, QList<TopoDS_Shape> & shapes3D)
    {
        if (shape.IsNull())
        {
            return;
        }

        TopAbs_ShapeEnum sType = shape.ShapeType();
        switch (sType)
        {
        case TopAbs_COMPOUND:
        case TopAbs_COMPSOLID:
        {
            // 遍历子形状。
            TopoDS_Iterator iter(shape);
            for (; iter.More(); iter.Next())
            {
                // 获取子形状递归拆分形状及信息。
                const TopoDS_Shape & subShape = iter.Value();
                FlattenCompShape3D(subShape, shapes, shapes3D);
            }

            break;
        }
        case TopAbs_SOLID:
        {
            shapes3D.push_back(shape);
            break;
        }
        case TopAbs_SHELL:
        case TopAbs_FACE:
        case TopAbs_WIRE:
        case TopAbs_EDGE:
        case TopAbs_VERTEX:
        {
            shapes.push_back(shape);
            break;
        }
        default:
            break;
        }
    }

    void FITKOCCComandCommon::FlattenCompShape2D3D(const TopoDS_Shape & shape, QList<TopoDS_Shape> & shapes,
        QList<TopoDS_Shape> & shapes2D, QList<TopoDS_Shape> & shapes3D)
    {
        if (shape.IsNull())
        {
            return;
        }

        TopAbs_ShapeEnum sType = shape.ShapeType();
        switch (sType)
        {
        case TopAbs_COMPOUND:
        case TopAbs_COMPSOLID:
        {
            // 遍历子形状。
            TopoDS_Iterator iter(shape);
            for (; iter.More(); iter.Next())
            {
                // 获取子形状递归拆分形状及信息。
                const TopoDS_Shape & subShape = iter.Value();
                FlattenCompShape2D3D(subShape, shapes, shapes2D, shapes3D);
            }

            break;
        }
        case TopAbs_SOLID:
        {
            shapes3D.push_back(shape);
            break;
        }
        case TopAbs_SHELL:
        case TopAbs_FACE:
        {
            shapes2D.push_back(shape);
            break;
        }
        case TopAbs_WIRE:
        case TopAbs_EDGE:
        case TopAbs_VERTEX:
        {
            shapes.push_back(shape);
            break;
        }
        default:
            break;
        }
    }

    void FITKOCCComandCommon::FlattenCompShapeBaseTopo(const TopoDS_Shape & shape, QList<TopoDS_Shape> & shapes, QList<TopoDS_Shape> & shapesBase)
    {
        if (shape.IsNull())
        {
            return;
        }

        TopAbs_ShapeEnum sType = shape.ShapeType();
        switch (sType)
        {
        case TopAbs_COMPOUND:
        case TopAbs_COMPSOLID:
        {
            // 遍历子形状。
            TopoDS_Iterator iter(shape);
            for (; iter.More(); iter.Next())
            {
                // 获取子形状递归拆分形状及信息。
                const TopoDS_Shape & subShape = iter.Value();
                FlattenCompShapeBaseTopo(subShape, shapes, shapesBase);
            }

            break;
        }
        case TopAbs_SOLID:
        case TopAbs_SHELL:
        {
            shapes.push_back(shape);
            break;
        }
        case TopAbs_FACE:
        case TopAbs_WIRE:
        case TopAbs_EDGE:
        case TopAbs_VERTEX:
        {
            shapesBase.push_back(shape);
            break;
        }
        default:
            break;
        }
    }

    bool FITKOCCComandCommon::MergeShapes(const TopoDS_Shape & shape1, const TopoDS_Shape & shape2,
        TopoDS_Shape & result, bool noNerg)
    {
        QList<TopoDS_Shape> shapes;
        shapes.push_back(shape1);
        shapes.push_back(shape2);
        return MergeShapes(shapes, result, noNerg);
    }

    bool FITKOCCComandCommon::MergeShapes(QList<TopoDS_Shape>& shapes, TopoDS_Shape& result, bool noNerg)
    {
        QList<TopoDS_Shape> shapesChecked;
        for (const TopoDS_Shape& shape : shapes)
        {
            if (!FITKOCCExtendTool::IsEmptyShape(shape))
            {
                shapesChecked.append(shape);
            }
        }

        if (shapesChecked.isEmpty())
        {
            result = TopoDS_Shape();
            return false;
        }
        else if (shapesChecked.count() == 1)
        {
            result = shapesChecked.first();
            return true;
        }

        BRep_Builder builder;
        TopoDS_Compound compoundInter1D, compoundInter2D, compoundInter3D;
        builder.MakeCompound(compoundInter1D);
        builder.MakeCompound(compoundInter2D);
        builder.MakeCompound(compoundInter3D);

        std::function<TopoDS_Shape(QList<TopoDS_Shape>&)> fuseShapes = [](
            QList<TopoDS_Shape>& shapes)
        {
            TopoDS_Shape fused;
            for (int i = 0; i < shapes.size(); ++i)
            {
                const TopoDS_Shape& s = shapes.at(i);
                if (FITKOCCExtendTool::IsEmptyShape(s))
                {
                    continue;
                }

                if (i == 0 || FITKOCCExtendTool::IsEmptyShape(fused))
                {
                    fused = s;
                }
                else
                {
                    try
                    {
                        BRepAlgoAPI_Fuse fuse(fused, s);
                        fuse.Build();
                        if (fuse.IsDone() && !fuse.Shape().IsNull())
                        {
                            fused = fuse.Shape();
                        }
                    }
                    catch (...) {}
                }
            }
            return fused;
        };

        QList<TopoDS_Shape> fused1Ds, fused2Ds, fused3Ds;

        for (const TopoDS_Shape& shape : shapesChecked)
        {
            // 将部件模型平铺后添加到各维度数组中。
            QList<TopoDS_Shape> shapes1D, shapes2D, shapes3D;
            FITKOCCComandCommon::FlattenCompShape2D3D(shape, shapes1D, shapes2D, shapes3D);

            // 融合模型。
            TopoDS_Shape fused1D = fuseShapes(shapes1D);
            TopoDS_Shape fused2D = fuseShapes(shapes2D);
            TopoDS_Shape fused3D = fuseShapes(shapes3D);

            if (!FITKOCCExtendTool::IsEmptyShape(fused1D))
            {
                fused1Ds.push_back(fused1D);
            }

            if (!FITKOCCExtendTool::IsEmptyShape(fused2D))
            {
                fused2Ds.push_back(fused2D);
            }

            if (!FITKOCCExtendTool::IsEmptyShape(fused3D))
            {
                fused3Ds.push_back(fused3D);
            }
        }

        // 融合所有一维边。
        TopoDS_Shape fused1D = fuseShapes(fused1Ds);

        // 融合所有二维面。
        TopoDS_Shape fused2D = fuseShapes(fused2Ds);

        // 融合所有三维实体。
        TopoDS_Shape fused3D = fuseShapes(fused3Ds);

        // 非正则布尔需计算模型相交。
        if (noNerg)
        {
            std::function<void(TopoDS_Shape&, TopoDS_Shape&, TopoDS_Compound&)> getSection = [](
                TopoDS_Shape& topoShape, TopoDS_Shape& partSubShape, TopoDS_Compound& compoundResult)
            {
                BRepAlgoAPI_Section intSec(topoShape, partSubShape);
                intSec.Build();
                if (!intSec.IsDone())
                {
                    return;
                }

                const TopoDS_Shape& s = intSec.Shape();
                if (!FITKOCCExtendTool::IsEmptyShape(s))
                {
                    BRep_Builder builder;
                    builder.Add(compoundResult, s);
                }
            };

            // 握手计算交线。
            //for (int i = 0; i < fused1Ds.count(); i++)
            //{
            //    for (int j = i + 1; j < fused1Ds.count(); j++)
            //    {
            //        getSection(fused1Ds[i], fused1Ds[j], compoundInter1D);
            //    }
            //}

            for (int i = 0; i < fused2Ds.count(); i++)
            {
                for (int j = i + 1; j < fused2Ds.count(); j++)
                {
                    getSection(fused2Ds[i], fused2Ds[j], compoundInter2D);
                }
            }

            for (int i = 0; i < fused3Ds.count(); i++)
            {
                for (int j = i + 1; j < fused3Ds.count(); j++)
                {
                    getSection(fused3Ds[i], fused3Ds[j], compoundInter3D);
                }
            }
        }

        // 计算维度间的交线与交面。
        // 3D与2D之间的交面/交线。
        TopoDS_Compound inter3D2D;
        builder.MakeCompound(inter3D2D);

        if (!FITKOCCExtendTool::IsEmptyShape(fused3D) &&
            !FITKOCCExtendTool::IsEmptyShape(fused2D))
        {
            try
            {
                BRepAlgoAPI_Section intSecLine3D2D(fused3D, fused2D);
                intSecLine3D2D.Build();
                if (intSecLine3D2D.IsDone())
                {
                    BRepAlgoAPI_Common intSec3D2D(fused3D, fused2D);
                    intSec3D2D.Build();

                    const TopoDS_Shape& s = intSec3D2D.Shape();
                    if (intSec3D2D.IsDone() && !FITKOCCExtendTool::IsEmptyShape(s))
                    {
                        builder.Add(inter3D2D, s);
                    }
                }
            }
            catch (...) {}
        }

        // 1D与3D之间的交线/交点。
        TopoDS_Compound inter1D3D;
        builder.MakeCompound(inter1D3D);

        if (!FITKOCCExtendTool::IsEmptyShape(fused1D) &&
            !FITKOCCExtendTool::IsEmptyShape(fused3D))
        {
            try
            {
                BRepAlgoAPI_Section intSecLine1D3D(fused1D, fused3D);
                intSecLine1D3D.Build();

                if (intSecLine1D3D.IsDone())
                {
                    BRepAlgoAPI_Common intSec1D3D(fused1D, fused3D);
                    intSec1D3D.Build();

                    const TopoDS_Shape& s = intSec1D3D.Shape();
                    if (intSec1D3D.IsDone() && !FITKOCCExtendTool::IsEmptyShape(s))
                    {
                        builder.Add(inter1D3D, intSec1D3D.Shape());
                    }
                }
            }
            catch (...) {}
        }

        // 2D与1D之间的交线/交点。
        TopoDS_Compound inter2D1D;
        builder.MakeCompound(inter2D1D);

        if (!FITKOCCExtendTool::IsEmptyShape(fused2D) &&
            !FITKOCCExtendTool::IsEmptyShape(fused1D))
        {
            try
            {
                BRepAlgoAPI_Section intSecLine2D1D(fused2D, fused1D);
                intSecLine2D1D.Build();
                if (intSecLine2D1D.IsDone())
                {
                    BRepAlgoAPI_Common intSec2D1D(fused2D, fused1D);
                    intSec2D1D.Build();

                    const TopoDS_Shape& s = intSec2D1D.Shape();
                    if (intSec2D1D.IsDone() && !FITKOCCExtendTool::IsEmptyShape(s))
                    {
                        builder.Add(inter2D1D, s);
                    }
                }
            }
            catch (...) {}
        }

        TopoDS_Shape result1D = fused1D;
        TopoDS_Shape result2D = fused2D;
        TopoDS_Shape result3D = fused3D;

        // 使用2D与3D的交线/交点进行互相分割。
        if (!FITKOCCExtendTool::IsEmptyShape(result2D) &&
            !FITKOCCExtendTool::IsEmptyShape(inter3D2D))
        {
            // 用3D与2D的交线分割2D。
            try
            {
                result2D = BRepAlgoAPI_Cut(result2D, inter3D2D);
            }
            catch (...) {}

            //// 用3D与2D的交线分割3D。
            //try
            //{
            //    BOPAlgo_Splitter splitter3D;
            //    splitter3D.AddArgument(result3D);
            //    splitter3D.AddTool(inter3D2D);
            //    splitter3D.Perform();
            //    if (!splitter3D.Shape().IsNull())
            //    {
            //        result3D = splitter3D.Shape();
            //    }
            //}
            //catch (...) {}
        }

        // 使用1D与3D的交线/交点进行互相分割。
        if (!FITKOCCExtendTool::IsEmptyShape(result1D) &&
            !FITKOCCExtendTool::IsEmptyShape(inter1D3D))
        {
            // 用1D与3D的交线分割1D。
            try
            {
                result1D = BRepAlgoAPI_Cut(result1D, inter1D3D);
            }
            catch (...) {}
        }

        // 使用2D与1D的交线/交点进行互相分割。
        if (!FITKOCCExtendTool::IsEmptyShape(result1D) &&
            !FITKOCCExtendTool::IsEmptyShape(inter2D1D))
        {
            // 用2D与1D的交线分割1D。
            try
            {
                result1D = BRepAlgoAPI_Cut(result1D, inter2D1D);
            }
            catch (...) {}
        }

        // 非正则布尔需使用模型相交部分进行分割。
        if (noNerg)
        {
            std::function<void(TopoDS_Shape&, TopoDS_Shape&, TopoDS_Shape&, TopAbs_ShapeEnum)> splitWithSectionCommon = [](
                TopoDS_Shape& interSec, TopoDS_Shape& shape, TopoDS_Shape& result, TopAbs_ShapeEnum subShapeType)
            {
                // 获取相交部分。
                BRepAlgoAPI_Common comm(interSec, shape);
                comm.Build();
                if (!comm.IsDone())
                {
                    return false;
                }

                BRep_Builder builder;
                TopoDS_Compound retComp;
                builder.MakeCompound(retComp);

                TopExp_Explorer iterComm;
                for (iterComm.Init(comm, subShapeType); iterComm.More(); iterComm.Next())
                {
                    const TopoDS_Shape& commShape = iterComm.Value();
                    if (commShape.IsNull())
                    {
                        continue;
                    }

                    builder.Add(retComp, commShape);
                }

                // 分割模型。
                BOPAlgo_Splitter splitter;
                splitter.AddArgument(shape);
                splitter.AddTool(retComp);
                splitter.Perform();

                result = splitter.Shape();
                return true;
            };

            // 分割模型。
            //if (!FITKOCCExtendTool::IsEmptyShape(compoundInter1D) &&
            //    !FITKOCCExtendTool::IsEmptyShape(result1D))
            //{
            //    splitWithSectionCommon(compoundInter1D, result1D, result1D, TopAbs_ShapeEnum::TopAbs_EDGE);
            //}

            if (!FITKOCCExtendTool::IsEmptyShape(compoundInter2D) &&
                !FITKOCCExtendTool::IsEmptyShape(result2D))
            {
                splitWithSectionCommon(compoundInter2D, result2D, result2D, TopAbs_ShapeEnum::TopAbs_EDGE);
            }

            if (!FITKOCCExtendTool::IsEmptyShape(compoundInter3D) &&
                !FITKOCCExtendTool::IsEmptyShape(result3D))
            {
                splitWithSectionCommon(compoundInter3D, result3D, result3D, TopAbs_ShapeEnum::TopAbs_FACE);
            }

            if (!FITKOCCExtendTool::IsEmptyShape(inter3D2D) &&
                !FITKOCCExtendTool::IsEmptyShape(result3D))
            {
                splitWithSectionCommon(inter3D2D, result3D, result3D, TopAbs_ShapeEnum::TopAbs_FACE);
            }
        }
        else
        {
            // 尝试合并面与边。
            try
            {
                ShapeUpgrade_UnifySameDomain unify1d(result1D);
                unify1d.Build();
                result1D = unify1d.Shape();
            }
            catch (...) {}

            try
            {
                ShapeUpgrade_UnifySameDomain unify2d(result2D);
                unify2d.Build();
                result2D = unify2d.Shape();
            }
            catch (...) {}

            // 去除实体间缝合线。
            try
            {
                if (!FITKOCCExtendTool::IsEmptyShape(result3D))
                {
                    result3D = FITKOCCComandCommon::removeSplitter(result3D);
                }
            }
            catch (...) {}
        }

        // 用1D2D交线中的点分割2D模型边。
        if (!FITKOCCExtendTool::IsEmptyShape(result2D) &&
            !FITKOCCExtendTool::IsEmptyShape(inter2D1D))
        {
            TopoDS_Compound verts;
            builder.MakeCompound(verts);

            TopExp_Explorer exp(inter2D1D, TopAbs_ShapeEnum::TopAbs_VERTEX);
            while (exp.More())
            {
                // 保存顶点。
                builder.Add(verts, exp.Current());
                exp.Next();
            }

            try
            {
                BOPAlgo_Splitter splitter2D;
                splitter2D.AddArgument(result2D);
                splitter2D.AddTool(verts);
                splitter2D.Perform();

                const TopoDS_Shape& s = splitter2D.Shape();
                if (!FITKOCCExtendTool::IsEmptyShape(s))
                {
                    result2D = s;
                }
            }
            catch (...) {}
        }

        // 用1D3D交线中的点分割3D模型边。
        if (!FITKOCCExtendTool::IsEmptyShape(result3D) &&
            !FITKOCCExtendTool::IsEmptyShape(inter1D3D))
        {
            TopoDS_Compound verts;
            builder.MakeCompound(verts);

            TopExp_Explorer exp(inter1D3D, TopAbs_ShapeEnum::TopAbs_VERTEX);
            while (exp.More())
            {
                // 保存顶点。
                builder.Add(verts, exp.Current());
                exp.Next();
            }

            try
            {
                BOPAlgo_Splitter splitter3D;
                splitter3D.AddArgument(result3D);
                splitter3D.AddTool(verts);
                splitter3D.Perform();

                const TopoDS_Shape& s = splitter3D.Shape();
                if (!FITKOCCExtendTool::IsEmptyShape(s))
                {
                    result3D = s;
                }
            }
            catch (...) {}
        }

        //// 用1D结果分割2D。
        //if (!FITKOCCExtendTool::IsEmptyShape(result1D) &&
        //    !FITKOCCExtendTool::IsEmptyShape(result2D))
        //{
        //    try
        //    {
        //        BOPAlgo_Splitter splitter2D;
        //        splitter2D.AddArgument(result2D);
        //        splitter2D.AddTool(result1D);
        //        splitter2D.Perform();

        //        const TopoDS_Shape& s = splitter2D.Shape();
        //        if (!FITKOCCExtendTool::IsEmptyShape(s))
        //        {
        //            result2D = s;
        //        }
        //    }
        //    catch (...) {}
        //}

        //// 用1D结果分割3D。
        //if (!FITKOCCExtendTool::IsEmptyShape(result1D) &&
        //    !FITKOCCExtendTool::IsEmptyShape(result3D))
        //{
        //    try
        //    {
        //        BOPAlgo_Splitter splitter3D;
        //        splitter3D.AddArgument(result3D);
        //        splitter3D.AddTool(result1D);
        //        splitter3D.Perform();

        //        const TopoDS_Shape& s = splitter3D.Shape();
        //        if (!FITKOCCExtendTool::IsEmptyShape(s))
        //        {
        //            result3D = s;
        //        }
        //    }
        //    catch (...) {}
        //}

        // 用2D结果分割3D。
        if (!FITKOCCExtendTool::IsEmptyShape(result2D) &&
            !FITKOCCExtendTool::IsEmptyShape(result3D))
        {
            try
            {
                BOPAlgo_Splitter splitter3D;
                splitter3D.AddArgument(result3D);
                splitter3D.AddTool(result2D);
                splitter3D.Perform();

                const TopoDS_Shape& s = splitter3D.Shape();
                if (!FITKOCCExtendTool::IsEmptyShape(s))
                {
                    result3D = s;
                }
            }
            catch (...) {}
        }

        // 合并所有维度的结果。
        TopoDS_Compound resultComp;
        builder.MakeCompound(resultComp);
        if (!FITKOCCExtendTool::IsEmptyShape(result3D))
        {
            builder.Add(resultComp, result3D);
        }

        if (!FITKOCCExtendTool::IsEmptyShape(result2D))
        {
            builder.Add(resultComp, result2D);
        }

        if (!FITKOCCExtendTool::IsEmptyShape(result1D))
        {
            builder.Add(resultComp, result1D);
        }

        result = resultComp;

        // 尝试合并边。
        //try
        //{
        //    ShapeUpgrade_UnifySameDomain unify(result);
        //    unify.Build();
        //    result = unify.Shape();
        //}
        //catch (...) {}

        //ShapeCustom_Surface customSurface;

        //BRepTools_ReShape reShape;
        //TopExp_Explorer exp(result, TopAbs_ShapeEnum::TopAbs_FACE);
        //while (exp.More())
        //{
        //    Handle(Geom_Surface) surf = BRep_Tool::Surface(TopoDS::Face(exp.Current()));
        //    exp.Next();

        //    if (!surf)
        //    {
        //        continue;
        //    }

        //    // 替换表面。
        //    customSurface.Init(surf);
        //    Handle(Geom_Surface) newSurf = customSurface.ConvertToAnalytical(1e-1, true);
        //    if (!newSurf)
        //    {
        //        continue;
        //    }

        //    const TopoDS_Face & newFace = BRepBuilderAPI_MakeFace(newSurf, 1e-6);
        //    if (newFace.IsNull())
        //    {
        //        continue;
        //    }

        //    reShape.Replace(exp.Current(), newFace);
        //}

        //result = reShape.Apply(result);

        return true;
    }

    void FITKOCCComandCommon::MakeWires(const TopoDS_Shape & shape, QList<TopoDS_Wire> & wires, double tol)
    {
        wires.clear();

        if (shape.IsNull())
        {
            return;
        }

        Handle(TopTools_HSequenceOfShape) edges = new TopTools_HSequenceOfShape;

        // 获取全部边
        int nEdges = 0;
        TopExp_Explorer exp(shape, TopAbs_ShapeEnum::TopAbs_EDGE);
        while (exp.More())
        {
            const TopoDS_Edge & edge = TopoDS::Edge(exp.Current());
            if (edge.IsNull())
            {
                continue;
            }

            // 保存边。
            edges->Append(edge);
            exp.Next();

            nEdges++;
        }

        if (nEdges == 0)
        {
            return;
        }

        try
        {
            Handle(TopTools_HSequenceOfShape) ws = new TopTools_HSequenceOfShape;
            ShapeAnalysis_FreeBounds::ConnectEdgesToWires(edges, tol, false, ws);
            int nWires = ws->Size();
            if (nWires == 0)
            {
                return;
            }

            for (int i = 1; i <= nWires; i++)
            {
                const TopoDS_Wire & wire = TopoDS::Wire(ws->Value(i));
                if (wire.IsNull())
                {
                    continue;
                }

                wires.push_back(wire);
            }
        }
        catch (...)
        {
            return;
        }
    }

    void FITKOCCComandCommon::MakeWires(const TopoDS_Shape & shape, QList<EncloseWire> & wiresList, gp_Pln pln, double tol)
    {
        wiresList.clear();

        if (shape.IsNull())
        {
            return;
        }

        QList<TopoDS_Wire> ws;
        MakeWires(shape, ws, tol);
        if (ws.isEmpty())
        {
            return;
        }

        // 根据线框面积从大到小排序。
        auto getWireArea = [&](const TopoDS_Wire & w)
        {
            if (!w.Closed())
            {
                return -1.;
            }

            const TopoDS_Face & f = BRepBuilderAPI_MakeFace(w);
            if (f.IsNull())
            {
                return -1.;
            }

            GProp_GProps fProp;
            BRepGProp::SurfaceProperties(f, fProp);
            double area = fProp.Mass();
            return area;
        };

        QList<WireWithArea> was;
        for (TopoDS_Wire & w : ws)
        {
            was.push_back(WireWithArea(w, getWireArea(w)));
        }

        std::sort(was.begin(), was.end(), WireWithArea());

        ws.clear();
        for (WireWithArea & wa : was)
        {
            ws.push_back(wa.Wire);
        }

        Handle(Geom_Plane) plane = new Geom_Plane(pln);

        // 根据包围盒分组。
        while (!ws.isEmpty())
        {
            TopoDS_Wire w = ws.takeFirst();
            if (!w.Closed() || wiresList.isEmpty())
            {
                EncloseWire ew;
                ew.Wire = w;
                wiresList.push_back(ew);
                continue;
            }

            const TopoDS_Face & f = BRepBuilderAPI_MakeFace(plane, w);
            if (f.IsNull())
            {
                EncloseWire ew;
                ew.Wire = w;
                wiresList.push_back(ew);
                continue;
            }

            Standard_Real uFirst, uLast, vFirst, vLast;
            BRepAdaptor_Surface brepFace(f);
            uFirst = brepFace.FirstUParameter();
            uLast = brepFace.LastUParameter();
            vFirst = brepFace.FirstVParameter();
            vLast = brepFace.LastVParameter();
            
            bool contains = false;
            for (EncloseWire & ewGroup : wiresList)
            {
                // 首先比较外层整体线框。
                const TopoDS_Wire & outWire = ewGroup.Wire;
                const TopoDS_Face & fOut = BRepBuilderAPI_MakeFace(plane, outWire);
                if (fOut.IsNull())
                {
                    break;
                }

                Standard_Real uFirstOut, uLastOut, vFirstOut, vLastOut;
                BRepAdaptor_Surface brepFaceOut(fOut);
                uFirstOut = brepFaceOut.FirstUParameter();
                uLastOut = brepFaceOut.LastUParameter();
                vFirstOut = brepFaceOut.FirstVParameter();
                vLastOut = brepFaceOut.LastVParameter();

                bool smallerThanOut = true;
                for (TopExp_Explorer iter(w, TopAbs_ShapeEnum::TopAbs_VERTEX); iter.More(); iter.Next())
                {
                    const TopoDS_Vertex & vert = TopoDS::Vertex(iter.Value());

                    Standard_Real u, v;
                    bool isOnSurf = GeomLib_Tool::Parameters(plane, BRep_Tool::Pnt(vert), Precision::Confusion(), u, v);
                    if (!isOnSurf)
                    {
                        break;
                    }

                    bool onFace = u < uLastOut && u > uFirstOut && v < vLastOut && v > vFirstOut;
                    if (!onFace)
                    {
                        smallerThanOut = false;
                        break;
                    }
                }

                if (!smallerThanOut)
                {
                    continue;
                }

                EncloseWire emptyEWire;
                contains = addToChildren(emptyEWire, ewGroup, w, plane, uFirst, uLast, vFirst, vLast);
            }

            if (contains)
            {
                continue;
            }

            wiresList.push_back(EncloseWire(w));
        }
    }

    bool FITKOCCComandCommon::addToChildren(EncloseWire & pew, EncloseWire & ew, TopoDS_Wire & wire, Handle(Geom_Plane) & plane,
        Standard_Real uFirst, Standard_Real uLast, Standard_Real vFirst, Standard_Real vLast)
    {
        Q_UNUSED(pew);

        if (ew.Children.isEmpty())
        {
            ew.Children.push_back(EncloseWire(wire));
            return true;
        }
        else
        {
            // 比较子线框最外层。
            for (int i = 0; i < ew.Children.count(); i++)
            {
                EncloseWire & cew = ew.Children[i];
                const TopoDS_Wire w2 = cew.Wire;
                const TopoDS_Face f2 = BRepBuilderAPI_MakeFace(plane, w2);
                if (f2.IsNull())
                {
                    break;
                }

                Standard_Real uFirst2, uLast2, vFirst2, vLast2;
                BRepAdaptor_Surface brepFace2(f2);
                uFirst2 = brepFace2.FirstUParameter();
                uLast2 = brepFace2.LastUParameter();
                vFirst2 = brepFace2.FirstVParameter();
                vLast2 = brepFace2.LastVParameter();

                bool wireInChild = true;
                for (TopExp_Explorer iter(wire, TopAbs_ShapeEnum::TopAbs_VERTEX); iter.More(); iter.Next())
                {
                    const TopoDS_Vertex & vert = TopoDS::Vertex(iter.Value());

                    Standard_Real u, v;
                    bool isOnSurf = GeomLib_Tool::Parameters(plane, BRep_Tool::Pnt(vert), Precision::Confusion(), u, v);
                    if (!isOnSurf)
                    {
                        wireInChild = false;
                        break;
                    }

                    bool onFace = u < uLast2 && u > uFirst2 && v < vLast2 && v > vFirst2;
                    if (!onFace)
                    {
                        wireInChild = false;
                        break;
                    }
                }

                bool wireChildIn = true;
                for (TopExp_Explorer iter(w2, TopAbs_ShapeEnum::TopAbs_VERTEX); iter.More(); iter.Next())
                {
                    const TopoDS_Vertex & vert = TopoDS::Vertex(iter.Value());

                    Standard_Real u, v;
                    bool isOnSurf = GeomLib_Tool::Parameters(plane, BRep_Tool::Pnt(vert), Precision::Confusion(), u, v);
                    if (!isOnSurf)
                    {
                        wireChildIn = false;
                        break;
                    }

                    bool onFace = u < uLast && u > uFirst && v < vLast && v > vFirst;
                    if (!onFace)
                    {
                        wireChildIn = false;
                        break;
                    }
                }

                // 当前线框在内部则递归判断该分支线框子线框，否则尝试插入同级。
                if (wireInChild)
                {
                    return addToChildren(ew, cew, wire, plane, uFirst, uLast, vFirst, vLast);
                }
                // 当前线框包含内部则将被包含线框从子节点移除，挂载至本线框下。
                else if (wireChildIn)
                {
                    EncloseWire newEw(wire);
                    newEw.Children.push_back(ew.Children.takeAt(i));
                    ew.Children.insert(i, newEw);
                    return true;
                }
            }

            // 与全部子线框互不包含则说明为同级，追加至父线框子节点。
            ew.Children.push_back(EncloseWire(wire));
            return true;
        }

        return false;
    }

    void FITKOCCComandCommon::FlattenWires(QList<EncloseWire> & iWiresList, QList<EncloseWire> & oWiresList)
    {
        while (!iWiresList.isEmpty())
        {
            EncloseWire outEW = iWiresList.takeFirst();

            // 添加内部线框。
            EncloseWire newEW(outEW.Wire);
            for (EncloseWire & child : outEW.Children)
            {
                newEW.Children.push_back(EncloseWire(child.Wire));
            }

            oWiresList.push_back(newEW);

            // 尝试平铺内部子线框。
            for (EncloseWire & child : outEW.Children)
            {
                // 递归。
                FlattenWires(child.Children, oWiresList);
            }
        }
    }

    void FITKOCCComandCommon::MakeFaces(QList<EncloseWire> & wiresList, QList<TopoDS_Face> & faces)
    {
        while (!wiresList.isEmpty())
        {
            EncloseWire outEW = wiresList.takeFirst();

            BRepBuilderAPI_MakeFace mkFace(outEW.Wire);

            try
            {
                // 添加内部孔面。
                for (EncloseWire & child : outEW.Children)
                {
                    mkFace.Add(child.Wire);
                }

                mkFace.Build();

                if (!mkFace.IsDone())
                {
                    continue;
                }

                const TopoDS_Face & face = mkFace.Face();
                if (face.IsNull())
                {
                    continue;
                }

                faces.push_back(face);
            }
            catch (...)
            {
                
            }

            // 尝试创建内部子面。
            for (EncloseWire & child : outEW.Children)
            {
                // 递归。
                MakeFaces(child.Children, faces);
            }
        }
    }

    void FITKOCCComandCommon::MakeWires(const TopoDS_Shape & shape, TopoDS_Shape & wires, double tol)
    {
        QList<TopoDS_Wire> ws;
        MakeWires(shape, ws, tol);

        if (ws.isEmpty())
        {
            return;
        }

        TopoDS_Builder builder;
        TopoDS_Compound compound;
        builder.MakeCompound(compound);

        for (const TopoDS_Wire & wire : ws)
        {
            builder.Add(compound, wire);
        }

        wires = compound;
    }

    BoxCompare FITKOCCComandCommon::CompareBox(const Bnd_Box & box1, const Bnd_Box & box2, double tol)
    {
        // 获取盒子信息。
        double box1Min[3], box1Max[3];
        box1.Get(box1Min[0], box1Min[1], box1Min[2], box1Max[0], box1Max[1], box1Max[2]);

        double box2Min[3], box2Max[3];
        box2.Get(box2Min[0], box2Min[1], box2Min[2], box2Max[0], box2Max[1], box2Max[2]);

        // 检查所有维度是否在范围内。
        BoxCompare ret = BoxCompare::BC_None;

        int n1In2 = 0, n2In1 = 0, nSame = 0;
        for (int i = 0; i < 3; i++) 
        {
            if (box1Min[i] < box2Min[i] - tol)
            {
                n1In2++;
            }
            else if (box2Min[i] < box1Min[i] - tol)
            {
                n2In1++;
            }
            else if (FuzzyCompare(box1Min[i], box2Min[i], tol))
            {
                nSame++;
            }

            if (box1Max[i] > box2Max[i] + tol)
            {
                n1In2++;
            }
            else if (box2Max[i] > box1Max[i] + tol)
            {
                n2In1++;
            }
            else if (FuzzyCompare(box1Max[i], box2Max[i], tol))
            {
                nSame++;
            }
        }

        if (n1In2 == 6)
        {
            ret = BoxCompare::BC_1_in_2;
        }
        else if (n2In1 == 6)
        {
            ret = BoxCompare::BC_2_in_1;
        }
        else if (nSame == 6)
        {
            ret = BoxCompare::BC_Same;
        }
        else
        {
            if (n1In2 == 3 && n2In1 == 3)
            {
                ret = BoxCompare::BC_NonIntersect;
            }
            else
            {
                ret = BoxCompare::BC_Intersect;
            }
        }

        return ret;
    }

    bool FITKOCCComandCommon::FuzzyCompare(double p1, double p2, double tol)
    {
        return (qAbs(p1 - p2) * (1 / (qFuzzyIsNull(tol) ? 1e-6 : tol)) <= qMin(qAbs(p1), qAbs(p2)));
    }

    std::vector<TopoDS_Wire> FITKOCCComandCommon::bulidWire(std::list<TopoDS_Edge> &edgeList)
    {
        std::vector<TopoDS_Wire> wires;

        while (edgeList.size() > 0)
        {
            BRepBuilderAPI_MakeWire mkWire;
            mkWire.Add(edgeList.front());
            edgeList.pop_front();

            TopoDS_Wire new_wire = mkWire.Wire(); // current new wire

            bool found = false;
            do
            {
                found = false;
                for (std::list<TopoDS_Edge>::iterator pE = edgeList.begin(); pE != edgeList.end(); ++pE)
                {
                    mkWire.Add(*pE);
                    if (mkWire.Error() != BRepBuilderAPI_DisconnectedWire)
                    {
                        found = true;
                        edgeList.erase(pE);
                        new_wire = mkWire.Wire();
                        break;
                    }
                }
            } while (found);

            // Fix any topological issues of the wire
            ShapeFix_Wire aFix;
            aFix.SetPrecision(Precision::Confusion());
            aFix.Load(new_wire);
            aFix.FixReorder();
            aFix.FixConnected();
            aFix.FixClosed();
            wires.push_back(aFix.Wire());
        }

        return wires;
    }

    TopoDS_Shape FITKOCCComandCommon::makeFace(std::vector<TopoDS_Wire> &w)
    {
        if (w.empty())
            return TopoDS_Shape();

        std::vector<TopoDS_Wire> wires = w;
        std::sort(wires.begin(), wires.end(), Wire_Compare());
        std::list<TopoDS_Wire> wire_list;
        wire_list.insert(wire_list.begin(), wires.rbegin(), wires.rend());

        // separate the wires into several independent faces
        std::list<std::list<TopoDS_Wire>> sep_wire_list;
        while (!wire_list.empty())
        {
            std::list<TopoDS_Wire> sep_list;
            TopoDS_Wire wire = wire_list.front();
            wire_list.pop_front();
            sep_list.push_back(wire);

            std::list<TopoDS_Wire>::iterator it = wire_list.begin();
            while (it != wire_list.end())
            {
                if (isInside(wire, *it))
                {
                    sep_list.push_back(*it);
                    it = wire_list.erase(it);
                }
                else
                {
                    ++it;
                }
            }

            sep_wire_list.push_back(sep_list);
        }

        if (sep_wire_list.size() == 1)
        {
            std::list<TopoDS_Wire> &wires = sep_wire_list.front();
            return makeFace(wires);
        }
        else if (sep_wire_list.size() > 1)
        {
            TopoDS_Compound comp;
            BRep_Builder builder;
            builder.MakeCompound(comp);
            for (std::list<std::list<TopoDS_Wire>>::iterator it = sep_wire_list.begin(); it != sep_wire_list.end(); ++it)
            {
                TopoDS_Shape aFace = makeFace(*it);
                if (!aFace.IsNull())
                    builder.Add(comp, aFace);
            }
            if (comp.Closed() == false)
            {
                return TopoDS_Shape(); // no closed.
            }
            return comp;
        }
        else
        {
            return TopoDS_Shape(); // error
        }
    }

    TopoDS_Shape FITKOCCComandCommon::makeFace(std::list<TopoDS_Wire> &wires)
    {
        if (wires.size() < 1)
        {
            return TopoDS_Shape();
        }
        BRepBuilderAPI_MakeFace mkFace(wires.front());
        const TopoDS_Face &face = mkFace.Face();
        if (face.IsNull())
            return face;
        gp_Dir axis(0, 0, 1);
        BRepAdaptor_Surface adapt(face);
        if (adapt.GetType() == GeomAbs_Plane)
        {
            axis = adapt.Plane().Axis().Direction();
        }

        wires.pop_front();
        for (std::list<TopoDS_Wire>::iterator it = wires.begin(); it != wires.end(); ++it)
        {
            BRepBuilderAPI_MakeFace mkInnerFace(*it);
            const TopoDS_Face &inner_face = mkInnerFace.Face();
            if (inner_face.IsNull())
                return inner_face; // failure
            gp_Dir inner_axis(0, 0, 1);
            BRepAdaptor_Surface adapt(inner_face);
            if (adapt.GetType() == GeomAbs_Plane)
            {
                inner_axis = adapt.Plane().Axis().Direction();
            }
            // It seems that orientation is always 'Forward' and we only have to reverse
            // if the underlying plane have opposite normals.
            if (axis.Dot(inner_axis) < 0)
                it->Reverse();
            mkFace.Add(*it);
        }
        return validateFace(mkFace.Face());
    }

    bool FITKOCCComandCommon::isInside(const TopoDS_Wire &wire1, const TopoDS_Wire &wire2)
    {
        Bnd_Box box1;
        BRepBndLib::Add(wire1, box1);
        box1.SetGap(0.0);

        Bnd_Box box2;
        BRepBndLib::Add(wire2, box2);
        box2.SetGap(0.0);

        if (box1.IsOut(box2))
            return false;

        double prec = Precision::Confusion();
        prec = 1E-6;

        BRepBuilderAPI_MakeFace mkFace(wire1);
        if (!mkFace.IsDone())
            Standard_Failure::Raise("Failed to create a face from wire in sketch");
        TopoDS_Face face = validateFace(mkFace.Face());
        BRepAdaptor_Surface adapt(face);
        IntTools_FClass2d class2d(face, prec);
        Handle_Geom_Surface surf = new Geom_Plane(adapt.Plane());
        ShapeAnalysis_Surface as(surf);

        TopExp_Explorer xp(wire2, TopAbs_VERTEX);
        while (xp.More())
        {
            TopoDS_Vertex v = TopoDS::Vertex(xp.Current());
            gp_Pnt p = BRep_Tool::Pnt(v);
            gp_Pnt2d uv = as.ValueOfUV(p, prec);
            if (class2d.Perform(uv) == TopAbs_IN)
                return true;
            // TODO: We can make a check to see if all points are inside or all outside
            // because otherwise we have some intersections which is not allowed
            else
                return false;
            xp.Next();
        }

        return false;
    }

    TopoDS_Face FITKOCCComandCommon::validateFace(const TopoDS_Face &face)
    {
        BRepCheck_Analyzer aChecker(face);
        if (!aChecker.IsValid())
        {
            TopoDS_Wire outerwire = ShapeAnalysis::OuterWire(face);
            TopTools_IndexedMapOfShape myMap;
            myMap.Add(outerwire);

            TopExp_Explorer xp(face, TopAbs_WIRE);
            ShapeFix_Wire fix;
            fix.SetFace(face);
            fix.Load(outerwire);
            fix.Perform();
            BRepBuilderAPI_MakeFace mkFace(fix.WireAPIMake());
            while (xp.More())
            {
                if (!myMap.Contains(xp.Current()))
                {
                    fix.Load(TopoDS::Wire(xp.Current()));
                    fix.Perform();
                    mkFace.Add(fix.WireAPIMake());
                }
                xp.Next();
            }

            aChecker.Init(mkFace.Face());
            if (!aChecker.IsValid())
            {
                ShapeFix_Shape fix(mkFace.Face());
                fix.SetPrecision(Precision::Confusion());
                fix.SetMaxTolerance(Precision::Confusion());
                fix.SetMaxTolerance(Precision::Confusion());
                fix.Perform();
                fix.FixWireTool()->Perform();
                fix.FixFaceTool()->Perform();
                TopoDS_Face fixedFace = TopoDS::Face(fix.Shape());
                aChecker.Init(fixedFace);
                if (!aChecker.IsValid())
                    return TopoDS_Face();
                // Standard_Failure::Raise("Failed to validate broken face");
                return fixedFace;
            }
            return mkFace.Face();
        }

        return face;
    }

    TopoDS_Shape FITKOCCComandCommon::removeSplitter(const TopoDS_Shape &shape)
    {
        if (shape.IsNull())
            Standard_Failure::Raise("Cannot remove splitter from empty shape");

        if (shape.ShapeType() == TopAbs_SOLID)
        {
            const TopoDS_Solid &solid = TopoDS::Solid(shape);
            BRepBuilderAPI_MakeSolid mkSolid;
            TopExp_Explorer it;
            for (it.Init(solid, TopAbs_SHELL); it.More(); it.Next())
            {
                const TopoDS_Shell &currentShell = TopoDS::Shell(it.Current());
                ModelRefine::FaceUniter uniter(currentShell);
                if (uniter.process())
                {
                    if (uniter.isModified())
                    {
                        const TopoDS_Shell &newShell = uniter.getShell();
                        mkSolid.Add(newShell);
                    }
                    else
                    {
                        mkSolid.Add(currentShell);
                    }
                }
                else
                {
                    Standard_Failure::Raise("Removing splitter failed");
                    return shape;
                }
            }
            return mkSolid.Solid();
        }
        else if (shape.ShapeType() == TopAbs_SHELL)
        {
            const TopoDS_Shell &shell = TopoDS::Shell(shape);
            ModelRefine::FaceUniter uniter(shell);
            if (uniter.process())
            {
                return uniter.getShell();
            }
            else
            {
                Standard_Failure::Raise("Removing splitter failed");
            }
        }
        else if (shape.ShapeType() == TopAbs_COMPOUND)
        {
            BRep_Builder builder;
            TopoDS_Compound comp;
            builder.MakeCompound(comp);

            TopExp_Explorer xp;
            // solids
            for (xp.Init(shape, TopAbs_SOLID); xp.More(); xp.Next())
            {
                const TopoDS_Solid &solid = TopoDS::Solid(xp.Current());
                BRepTools_ReShape reshape;
                TopExp_Explorer it;
                for (it.Init(solid, TopAbs_SHELL); it.More(); it.Next())
                {
                    const TopoDS_Shell &currentShell = TopoDS::Shell(it.Current());
                    ModelRefine::FaceUniter uniter(currentShell);
                    if (uniter.process())
                    {
                        if (uniter.isModified())
                        {
                            const TopoDS_Shell &newShell = uniter.getShell();
                            reshape.Replace(currentShell, newShell);
                        }
                    }
                }
                builder.Add(comp, reshape.Apply(solid));
            }
            // free shells
            for (xp.Init(shape, TopAbs_SHELL, TopAbs_SOLID); xp.More(); xp.Next())
            {
                const TopoDS_Shell &shell = TopoDS::Shell(xp.Current());
                ModelRefine::FaceUniter uniter(shell);
                if (uniter.process())
                {
                    builder.Add(comp, uniter.getShell());
                }
            }
            // the rest
            for (xp.Init(shape, TopAbs_FACE, TopAbs_SHELL); xp.More(); xp.Next())
            {
                if (!xp.Current().IsNull())
                    builder.Add(comp, xp.Current());
            }
            for (xp.Init(shape, TopAbs_WIRE, TopAbs_FACE); xp.More(); xp.Next())
            {
                if (!xp.Current().IsNull())
                    builder.Add(comp, xp.Current());
            }
            for (xp.Init(shape, TopAbs_EDGE, TopAbs_WIRE); xp.More(); xp.Next())
            {
                if (!xp.Current().IsNull())
                    builder.Add(comp, xp.Current());
            }
            for (xp.Init(shape, TopAbs_VERTEX, TopAbs_EDGE); xp.More(); xp.Next())
            {
                if (!xp.Current().IsNull())
                    builder.Add(comp, xp.Current());
            }

            return comp;
        }

        return shape;
    }

    bool FITKOCCComandCommon::isEmpty(const TopoDS_Shape &shape)
    {
        TopExp_Explorer ptExp(shape, TopAbs_VERTEX);
        for (; ptExp.More(); ptExp.Next())
            return false;

        TopExp_Explorer edgeExp(shape, TopAbs_EDGE);
        for (; edgeExp.More(); edgeExp.Next())
            return false;

        TopExp_Explorer faceExp(shape, TopAbs_FACE);
        for (; faceExp.More(); faceExp.Next())
            return false;

        return true;
    }

    TopoDS_Shape FITKOCCComandCommon::removeShape(TopoDS_Shape *inputShape, TopoDS_Shape *component)
    {

        TopoDS_Shape copyshape = BRepBuilderAPI_Copy(*inputShape);

        Handle(TopoDS_TShape) hand1 = inputShape->TShape();
        Handle(TopoDS_TShape) hand2 = component->TShape();
        if (hand1 == hand2)
        {
            *inputShape = TopoDS_Shape();
            return copyshape;
        }
        const TopoDS_Shape &aCuttedShape = BRepAlgoAPI_Cut(*inputShape, *component);
        *inputShape = aCuttedShape;
        //      BRep_Builder builder;
        //      builder.Remove(*inputShape, *component);
        return copyshape;
    }

    /*
        gp_Ax2 FITKOCCComandCommon::getEdgeAxis(Geometry::GeometrySet* set, int edgeindex)
        {
            TopoDS_Shape* shape = set->getShape();
            TopExp_Explorer edgeExp(*shape, TopAbs_EDGE);
            for (int index = 0; index < edgeindex && edgeExp.More(); edgeExp.Next(), ++index);

            const TopoDS_Shape& edgeShape = edgeExp.Current();
            if (edgeShape.IsNull()) return;
            const TopoDS_Edge &edgeone = TopoDS::Edge(edgeShape);
            if (edgeone.IsNull()) return;


            Standard_Real first, last;
            Handle(Geom_Curve) &curve = BRep_Tool::Curve(edgeone, first, last);
            Handle(Geom_TrimmedCurve) myTrimmed = new Geom_TrimmedCurve(curve, first, last);
            Handle(Geom_BSplineCurve) NurbsCurve = GeomConvert::CurveToBSplineCurve(myTrimmed);

            GeomAdaptor_Curve GAC(NurbsCurve);
            GCPnts_UniformAbscissa UA(GAC, 200);
            if (!UA.IsDone()) return;
            Standard_Real n = UA.NbPoints();
            for (int i = 0; i < n; ++i)
            {
                Standard_Real parami = UA.Parameter(i + 1);
                gp_Pnt tpnt;
                gp_Vec vpnt;
                curve->D1(parami, tpnt, vpnt);
            }
        }*/
        /*
        gp_Ax2 FITKOCCComandCommon::getFaceAxis(Geometry::GeometrySet* set, int index)
        {
            return gp_Ax2;
        }*/

}