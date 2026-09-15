/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCModelCircularHole.h
 * @brief       OCC打圆孔功能。

 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2025-09-01
 *********************************************************************/

#ifndef  FITKOCCModelCircularHole_H
#define  FITKOCCModelCircularHole_H

#include "FITKAbstractOCCModel.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelOperSolid.h"

class TopoDS_Shape;

namespace OCC
{
    /**
     * @brief       OCC打圆孔功能。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-09-01
     */
    class FITKGeoCompOCCAPI FITKOCCModelCircularHole : public Interface::FITKAbsGeoModelCircularHoleSolid, public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-01
         */
        FITKOCCModelCircularHole();

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-01
         */
        ~FITKOCCModelCircularHole() = default;

        /**
         * @brief       更新数据。[重写]
         * @return      是否成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-01
         */
        bool update() override;

    };
}

#endif // !FITKOCCModelCircularHole_H
