/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCGeoSketchDeleter.h
 * @brief       草图移除边命令。
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2025-09-16
 *********************************************************************/

#ifndef  __FITKOCCGEOSKETCHDELETER_H__
#define  __FITKOCCGEOSKETCHDELETER_H__

#include "FITKAbstractOCCModel.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoSketchDeleter.h"

namespace OCC
{
    /**
     * @brief       草图移除边命令。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-09-16
     */
    class FITKGeoCompOCCAPI FITKOCCGeoSketchDeleter :
        public Interface::FITKAbsGeoSketchDeleter, public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-16
         */
        FITKOCCGeoSketchDeleter();

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-16
         */
        virtual ~FITKOCCGeoSketchDeleter() = default;

        /**
         * @brief       执行命令。[重写]
         * @return      是否执行成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-16
         */
        bool update() override;
    };
}

#endif // !__FITKOCCGEOSKETCHDELETER_H__
