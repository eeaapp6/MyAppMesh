/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCModelFilledParaboloid.h
 * @brief       填充抛物面建模命令OCC实现类。
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2026-04-09
 *********************************************************************/

#ifndef __FITKOCCMODELFILLEDPARABOLOID_H__
#define __FITKOCCMODELFILLEDPARABOLOID_H__

#include "FITKAbstractOCCModel.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelFilledParaboloid.h"

namespace OCC
{
    /**
     * @brief       填充抛物面建模命令OCC实现类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2026-04-09
     */
    class FITKGeoCompOCCAPI FITKOCCModelFilledParaboloid :
        public Interface::FITKAbsGeoModelFilledParaboloid, public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-09
         */
        FITKOCCModelFilledParaboloid();

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-09
         */
        ~FITKOCCModelFilledParaboloid() override = default;

        /**
         * @brief       更新模型。
         * @return      true: 更新成功
         * @return      false: 更新失败
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-09
         */
        bool update() override;
    };
}

#endif // !__FITKOCCMODELFILLEDPARABOLOID_H__
