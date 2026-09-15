/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbsGeoOperNBodyUnite.h"

namespace Interface
{
    const QList<VirtualShape> & FITKAbsGeoOperNBodyUnite::getShapes()
    {
        // 获取形状列表。
        return m_shapes;
    }

    void FITKAbsGeoOperNBodyUnite::addShape(VirtualShape & shape)
    {
        // 判断拓扑类型。
        if (shape.Type != Interface::FITKGeoEnum::VSSolid)
        {
            return;
        }

        // 去除重复。
        for (const VirtualShape & iShape : m_shapes)
        {
            if (iShape.CmdId == shape.CmdId &&
                (iShape.VirtualTopoId == shape.VirtualTopoId || 
                iShape.VirtualTopoIndex == shape.VirtualTopoIndex))
            {
                return;
            }
        }

        m_shapes.push_back(shape);
    }

    void FITKAbsGeoOperNBodyUnite::clearShapes()
    {
        m_shapes.clear();
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoOperNBodyUnite::getGeometryCommandType()
    {
        // 获取几何命令类型。
        return FITKGeoEnum::FGTNBodyUnite;
    }
}