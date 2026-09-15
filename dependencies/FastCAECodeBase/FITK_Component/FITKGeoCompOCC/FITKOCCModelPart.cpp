/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCModelPart.h"

// OCC
#include <TopoDS_Shape.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepBuilderAPI_Copy.hxx>
#include <BRep_Builder.hxx>
#include <TopoDS_Compound.hxx>
#include <ShapeFix_Shape.hxx>

#include "FITKOCCComandCommon.h"

namespace OCC
{
    FITKOCCModelPart::FITKOCCModelPart() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    TopoDS_Shape* FITKOCCModelPart::getPartShape()
    {
        if (!_occShapeAgent)
        {
            return nullptr;
        }

        return _occShapeAgent->getShape();
    }

    TopoDS_Shape FITKOCCModelPart::getCopiedPartShape()
    {
        TopoDS_Shape* partShape = getPartShape();
        if (!partShape)
        {
            return TopoDS_Shape();
        }

        // 拷贝形状。
        BRepBuilderAPI_Copy copy(*partShape);
        return copy.Shape();
    }

    void FITKOCCModelPart::updatePartShape(const TopoDS_Shape & shape, bool unite, bool fixShape)
    {
        if (!_occShapeAgent)
        {
            return;
        }

        BRep_Builder builder;
        TopoDS_Shape* orgShape = _occShapeAgent->getShape();

        if (orgShape)
        {
            if (orgShape->IsNull())
            {
                TopoDS_Shape newShape;
                reUnite(shape, newShape, fixShape);
                _occShapeAgent->updateShape(newShape);
            }
            else
            {
                // 求和。
                if (unite)
                {
                    // 融合模型。
                    TopoDS_Shape newShape;
                    FITKOCCComandCommon::MergeShapes(shape, *orgShape, newShape);
                    _occShapeAgent->updateShape(newShape);
                }
                // 替换。
                else
                {
                    // 重组对象。
                    TopoDS_Shape newShape;
                    reUnite(shape, newShape, fixShape);
                    _occShapeAgent->updateShape(newShape);
                }
            }
        }
        // 替换形状。
        else
        {
            // 重组对象。
            TopoDS_Shape newShape;
            reUnite(shape, newShape, fixShape);
            _occShapeAgent->updateShape(newShape);
        }
    }

    bool FITKOCCModelPart::backtrackingShape()
    {
        // 获取最后一条几何命令的形状。
        Interface::FITKAbsGeoCommand* lastCmd = this->getDataByIndex(this->getDataCount() - 1);

        // 不存在命令则重置为空形状。
        if (!lastCmd)
        {
            _occShapeAgent->updateShape(TopoDS_Shape());
            return true;
        }

        // 否则取出当前最后命令维护的形状刷新拓扑。
        FITKAbstractOCCModel* occModel = lastCmd->getTShapeAgent<FITKAbstractOCCModel>();
        if (!occModel)
        {
            _occShapeAgent->updateShape(TopoDS_Shape());
            return false;
        }

        _occShapeAgent->updateShape(*occModel->getShape());
        return true;
    }

    bool FITKOCCModelPart::update()
    {
        if (!_occShapeAgent)
        {
            return false;
        }

        // 重构虚拓扑。
        _occShapeAgent->buildVirtualTopo();
        _occShapeAgent->triangulation();
        return true;
    }

    void FITKOCCModelPart::appendDataObj(Core::FITKAbstractDataObject* obj)
    {
        Interface::FITKAbsGeoCommand* subCmd = dynamic_cast<Interface::FITKAbsGeoCommand*>(obj);
        if (!subCmd)
        {
            return;
        }

        // 类型转换，获取子数据形状代理器。
        FITKAbstractOCCModel* shapeAgentNew = subCmd->getTShapeAgent<FITKAbstractOCCModel>();
        if (!shapeAgentNew)
        {
            return;
        }

        //// 同步第一条子命令形状。
        //if (this->getDataCount() == 0)
        //{
        //    updatePartShape(*shapeAgentNew->getShape());
        //}

        Interface::FITKAbsGeoPart::appendDataObj(subCmd);

        // 设置部件。
        shapeAgentNew->setPart(this);
    }

    void FITKOCCModelPart::reUnite(const TopoDS_Shape & iShape, TopoDS_Shape & oShape, bool fixShape)
    {
        if (iShape.IsNull())
        {
            oShape = TopoDS_Shape();
            return;
        }

        // 修复异常形状。
        TopoDS_Shape shape;
        if (fixShape)
        {
            ShapeFix_Shape fix(iShape);
            fix.Perform();

            shape = fix.Shape();
        }
        else
        {
            shape = iShape;
        }

        BRep_Builder builder;

        // 重组对象。
        QList<TopoDS_Shape> subShapes;
        FITKOCCComandCommon::FlattenCompShape(shape, subShapes);

        //if (subShapes.count() == 1)
        //{
        //    oShape = subShapes[0];
        //}
        //else
        //{
            TopoDS_Compound compoundReUnite;
            builder.MakeCompound(compoundReUnite);

            for (const TopoDS_Shape & shape : subShapes)
            {
                builder.Add(compoundReUnite, shape);
            }

            oShape = compoundReUnite;
        //}
    }
}