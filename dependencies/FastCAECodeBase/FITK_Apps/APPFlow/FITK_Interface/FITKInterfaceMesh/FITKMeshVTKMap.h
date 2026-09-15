/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef _FITKMESH_VTK_MAP_H___
#define _FITKMESH_VTK_MAP_H___
 
#include "FITK_Interface/FITKInterfaceModel/FITKModelEnum.h"
#include <vtkCellType.h>

namespace Interface
{
    //单元类型映射
    const QHash<Interface::FITKModelEnum::FITKEleType, int> eleTypeHash = {
        {Interface::FITKModelEnum::FITKEleType::Vertex1,   VTK_VERTEX},
        {Interface::FITKModelEnum::FITKEleType::Line2,     VTK_LINE},
        {Interface::FITKModelEnum::FITKEleType::Line3,     VTK_QUADRATIC_EDGE},
        {Interface::FITKModelEnum::FITKEleType::Tri3,      VTK_TRIANGLE},
        {Interface::FITKModelEnum::FITKEleType::Tri6,      VTK_QUADRATIC_TRIANGLE},
        {Interface::FITKModelEnum::FITKEleType::Quad4,     VTK_QUAD},
        {Interface::FITKModelEnum::FITKEleType::Quad8,     VTK_QUADRATIC_QUAD},
        {Interface::FITKModelEnum::FITKEleType::Tet4,      VTK_TETRA},
        {Interface::FITKModelEnum::FITKEleType::Tet10,     VTK_QUADRATIC_TETRA},
        {Interface::FITKModelEnum::FITKEleType::Wedge6,    VTK_WEDGE},
        {Interface::FITKModelEnum::FITKEleType::Hex8,      VTK_HEXAHEDRON},
        {Interface::FITKModelEnum::FITKEleType::Hex20,     VTK_QUADRATIC_HEXAHEDRON},
        {Interface::FITKModelEnum::FITKEleType::Polygon,   VTK_POLYGON},

    };
}


#endif
