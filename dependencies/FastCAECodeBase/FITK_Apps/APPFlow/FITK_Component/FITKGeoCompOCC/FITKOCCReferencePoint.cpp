/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCReferencePoint.h"
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <gp_Pnt.hxx>

namespace OCC {
    FITKOCCReferencePoint::FITKOCCReferencePoint() :OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }
    bool FITKOCCReferencePoint::update()
    {
        BRepBuilderAPI_MakeVertex vertex(gp_Pnt(m_pos[0], m_pos[1], m_pos[2]));
        _occShapeAgent->updateShape(vertex);
        return true;
    }

    Interface::FITKGeoEnum::FITKDatumType FITKOCCReferencePoint::getDatumType()
    {
        return Interface::FITKGeoEnum::FITKDatumType::FDTPoint;
    }
}

