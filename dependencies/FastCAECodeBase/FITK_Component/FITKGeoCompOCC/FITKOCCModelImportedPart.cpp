/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCModelImportedPart.h"

#include "FITKOCCExtendTool.h"
#include "FITKOCCModelPart.h"

// OCC
#include <TopoDS_Shape.hxx>
#include <BRepBuilderAPI_Copy.hxx>

namespace OCC
{
    FITKOCCModelImportedPart::FITKOCCModelImportedPart() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelImportedPart::update()
    {
        if (!_occShapeAgent)
        {
            return false;
        }

        TopoDS_Shape* shape = _occShapeAgent->getShape();
        if (!shape)
        {
            return false;
        }

        FITKOCCModelPart* part = _occShapeAgent->getPart();
        if (!part)
        {
            return false;
        }

        // part->setColor(getColor());
        part->updatePartShape(BRepBuilderAPI_Copy(*shape), false, false);

        return true;
    }

    void FITKOCCModelImportedPart::setShape(TopoDS_Shape & shape)
    {
        // 自身无需构建虚拓扑结构，外层部件进行模型数据合并与拆分。
        _occShapeAgent->updateShape(shape, false);
    }

    Interface::FITKAbsGeoImportedPart* FITKOCCModelImportedPart::getCopy()
    {
        if (!_occShapeAgent)
        {
            return nullptr;
        }

        TopoDS_Shape* shape = _occShapeAgent->getShape();
        if (!shape)
        {
            return nullptr;
        }

        if (FITKOCCExtendTool::IsEmptyShape(*shape))
        {
            return nullptr;
        }

        // 拷贝数据。
        FITKOCCModelImportedPart* copy = new FITKOCCModelImportedPart;
        TopoDS_Shape newShape = BRepBuilderAPI_Copy(*shape);
        copy->setShape(newShape);
        copy->setDataObjectName(this->getDataObjectName());

        return copy;
    }
}
