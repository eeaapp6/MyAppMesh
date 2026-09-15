/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCModelSplitPart.h
 * @brief       OCC部件分割命令实现类。
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2026-04-13
 *********************************************************************/

#ifndef __FITKOCCMODELSPLITPART_H__
#define __FITKOCCMODELSPLITPART_H__

#include "FITKAbstractOCCModel.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelSplitPart.h"

namespace OCC
{
    /**
     * @brief       OCC部件分割命令实现类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2026-04-13
     */
    class FITKGeoCompOCCAPI FITKOCCModelSplitPart : public Interface::FITKAbsGeoModelSplitPart,
        public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        explicit FITKOCCModelSplitPart();

        /**
         * @brief       析构函数。[虚函数]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        virtual ~FITKOCCModelSplitPart() = default;

        /**
         * @brief       更新模型数据。[重写]
         * @return      更新是否成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        bool update() override;

    };
}

#endif // !__FITKOCCMODELSPLITPART_H__
