/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCModelFromSketch.h
 * @brief       OCC模型从草图创建几何命令类。
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2026-06-26
 *********************************************************************/

#ifndef  __FITKOCCMODELFROMSKETCH_H__
#define  __FITKOCCMODELFROMSKETCH_H__

#include "FITKAbstractOCCModel.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelFromSketch.h"

namespace OCC
{
    /**
     * @brief       OCC模型从草图创建几何命令类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2026-06-26
     */
    class FITKGeoCompOCCAPI FITKOCCModelFromSketch :
        public Interface::FITKAbsGeoModelFromSketch, public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-26
         */
        FITKOCCModelFromSketch();

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-26
         */
        virtual ~FITKOCCModelFromSketch() = default;
        
        /**
         * @brief       更新模型。[重写]
         * @return      是否成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-26
         */
        bool update() override;

    };
}

#endif // !__FITKOCCMODELFROMSKETCH_H__
