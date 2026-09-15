/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCModelPlanarShell.h
 * @brief       OCC模型从草图创建平面壳几何命令类。
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2026-06-26
 *********************************************************************/

#ifndef  __FITKOCCMODELPLANARSHELL_H__
#define  __FITKOCCMODELPLANARSHELL_H__

#include "FITKAbstractOCCModel.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelPlanarShell.h"

namespace OCC
{
    /**
     * @brief       OCC模型从草图创建平面壳几何命令类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2026-06-26
     */
    class FITKGeoCompOCCAPI FITKOCCModelPlanarShell :
        public Interface::FITKAbsGeoModelPlanarShell, public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-26
         */
        FITKOCCModelPlanarShell();

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-26
         */
        virtual ~FITKOCCModelPlanarShell() = default;
        
        /**
         * @brief       更新模型。[重写]
         * @return      是否成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-26
         */
        bool update() override;

    };
}

#endif // !__FITKOCCMODELPLANARSHELL_H__
