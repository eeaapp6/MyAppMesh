/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCModelWireEdgeFromEdge.h
 * @brief       OCC根据边创建线边功能实现。
 *
 * @author      FastCAE
 * @date        2026-07-07
 *********************************************************************/

#ifndef __FITKOCCMODELWIREEDGEFROMEDGE_H__
#define __FITKOCCMODELWIREEDGEFROMEDGE_H__

#include "FITKAbstractOCCModel.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoWireEdgeFromEdge.h"

namespace OCC
{
    /**
     * @brief       OCC根据边创建线边类。
     * @author      FastCAE
     * @date        2026-07-07
     */
    class FITKGeoCompOCCAPI FITKOCCModelWireEdgeFromEdge : public Interface::FITKAbsGeoWireEdgeFromEdge, public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      FastCAE
         * @date        2026-07-07
         */
        FITKOCCModelWireEdgeFromEdge();

        /**
         * @brief       析构函数。
         * @author      FastCAE
         * @date        2026-07-07
         */
        ~FITKOCCModelWireEdgeFromEdge() = default;

        /**
         * @brief       更新数据。[重写]
         * @return      是否成功
         * @author      FastCAE
         * @date        2026-07-07
         */
        bool update() override;
    };
}

#endif // !__FITKOCCMODELWIREEDGEFROMEDGE_H__
