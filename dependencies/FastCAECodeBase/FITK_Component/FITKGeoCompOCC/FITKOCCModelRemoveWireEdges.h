/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCModelRemoveWireEdges.h
 * @brief       OCC移除线边功能实现。
 *
 * @author      FastCAE
 * @date        2026-07-01
 *********************************************************************/

#ifndef __FITKOCCMODELREMOVEWIREDGES_H__
#define __FITKOCCMODELREMOVEWIREDGES_H__

#include "FITKAbstractOCCModel.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoRemoveWireEdges.h"

namespace OCC
{
    /**
     * @brief       OCC移除线边类。
     * @author      FastCAE
     * @date        2026-07-01
     */
    class FITKGeoCompOCCAPI FITKOCCModelRemoveWireEdges : public Interface::FITKAbsGeoRemoveWireEdges, public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      FastCAE
         * @date        2026-07-01
         */
        FITKOCCModelRemoveWireEdges();

        /**
         * @brief       析构函数。
         * @author      FastCAE
         * @date        2026-07-01
         */
        ~FITKOCCModelRemoveWireEdges() = default;

        /**
         * @brief       更新数据。[重写]
         * @return      是否成功
         * @author      FastCAE
         * @date        2026-07-01
         */
        bool update() override;
    };
}

#endif // !__FITKOCCMODELREMOVEWIREDGES_H__
