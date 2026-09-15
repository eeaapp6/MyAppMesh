/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCModelImportedPart.h"

// OCC
#include <TopoDS_Shape.hxx>

namespace OCC
{
    FITKOCCModelImportedPart::FITKOCCModelImportedPart() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelImportedPart::update()
    {
        // 本命令数据来自文件读取，无需更新。
        return true;
    }

    void FITKOCCModelImportedPart::setShape(TopoDS_Shape & shape)
    {
        // 自身无需构建虚拓扑结构，外层部件进行模型数据合并与拆分。
        _occShapeAgent->updateShape(shape, false);
    }
}
