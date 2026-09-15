/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbsGeoWireEdgeFromEdge.h"

namespace Interface
{
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoWireEdgeFromEdge::getGeometryCommandType()
    {
        // 根据边创建线边类型。
        return FITKGeoEnum::FITKGeometryComType::FGTWireEdgeFromEdge;
    }

    QList<VirtualShape> FITKAbsGeoWireEdgeFromEdge::edges() const
    {
        return m_Edges;
    }

    void FITKAbsGeoWireEdgeFromEdge::setEdges(QList<VirtualShape> edges)
    {
        m_Edges = edges;
    }
}
