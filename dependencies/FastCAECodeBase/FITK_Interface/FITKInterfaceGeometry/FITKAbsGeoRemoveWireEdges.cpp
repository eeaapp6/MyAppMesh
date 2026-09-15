/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbsGeoRemoveWireEdges.h"

namespace Interface
{
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoRemoveWireEdges::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTRemoveWireEdges;
    }

    void FITKAbsGeoRemoveWireEdges::add(VirtualShape edge)
    {
        m_tempVShapes.push_back(edge);
    }

    void FITKAbsGeoRemoveWireEdges::add(QList<VirtualShape> edges)
    {
        m_tempVShapes.append(edges);
    }

    void FITKAbsGeoRemoveWireEdges::set(QList<VirtualShape> edges)
    {
        m_tempVShapes = edges;
    }

    QList<VirtualShape> FITKAbsGeoRemoveWireEdges::getVShapes()
    {
        return m_tempVShapes;
    }
}
