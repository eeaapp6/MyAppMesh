/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKElementFactory.h"
#include "FITKElementLine.h"
#include "FITKElementTri.h"
#include "FITKElementQuad.h"
#include "FITKElementTet.h"
#include "FITKElementWedge.h"
#include "FITKElementHex.h"
#include "FITKElementPolygon.h"
#include "FITKElementVertex.h"

namespace Interface
{
    /**
     * @brief 根据类型创建对象
     * @param[i]  t            类型
     * @return FITKAbstractElement*
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-30
     */
    FITKAbstractElement* FITKElementFactory::createElement(FITKModelEnum::FITKEleType t)
    {
        switch (t)
        {
        case FITKModelEnum::FITKEleType::Vertex1:
            return new FITKElementVertex1;
        case FITKModelEnum::FITKEleType::Line2:
            return new FITKElementLine2;
        case FITKModelEnum::FITKEleType::Line3:
            return new FITKElementLine3;
        case FITKModelEnum::FITKEleType::Tri3:
            return new FITKElementTri3;
        case FITKModelEnum::FITKEleType::Tri6:
            return new FITKElementTri6;
        case FITKModelEnum::FITKEleType::Quad4:
            return new FITKElementQuad4;
        case FITKModelEnum::FITKEleType::Quad8:
            return new FITKElementQuad8;
        case FITKModelEnum::FITKEleType::Tet4:
            return new FITKElementTet4;
        case FITKModelEnum::FITKEleType::Tet10:
            return new FITKElementTet10;
        case FITKModelEnum::FITKEleType::Wedge6:
            return new FITKElementWedge6;
        case FITKModelEnum::FITKEleType::Wedge15:
            return new FITKElementWedge15;
        case FITKModelEnum::FITKEleType::Hex8:
            return new FITKElementHex8;
        case FITKModelEnum::FITKEleType::Hex20:
            return new FITKElementHex20;
        case FITKModelEnum::FITKEleType::Polygon:
            return new FITKElementPolygon;
//错误值
        default:
            return nullptr;
        }
        //预防异常
        return nullptr;
    }

}



