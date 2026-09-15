/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCModelStitch.h
 * @brief       OCC缝合功能。

 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2025-09-01
 *********************************************************************/

#ifndef  __FITKOCCMODELSTITCH_H__
#define  __FITKOCCMODELSTITCH_H__

#include "FITKAbstractOCCModel.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoStitch.h"

class TopoDS_Shape;

namespace OCC
{
    /**
     * @brief       OCC缝合功能。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-09-01
     */
    class FITKGeoCompOCCAPI FITKOCCModelStitch : public Interface::FITKAbsGeoStitch, public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-01
         */
        FITKOCCModelStitch();

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-01
         */
        ~FITKOCCModelStitch() = default;

        /**
         * @brief       更新数据。[重写]
         * @return      是否成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-01
         */
        bool update() override;

    };
}

#endif // !__FITKOCCMODELSTITCH_H__
