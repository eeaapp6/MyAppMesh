/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCModelCut.h
 * @brief       切削功能类。
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2025-08-29
 *********************************************************************/

#ifndef  __FITKOCCMODELCUT_H__
#define  __FITKOCCMODELCUT_H__

#include "FITKAbstractOCCModel.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelCut.h"

namespace OCC
{
    /**
     * @brief       拉伸切削。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-08-29
     */
    class FITKGeoCompOCCAPI FITKOCCModelExtrudeCut :
        public Interface::FITKAbsGeoModelExtrudeCut, public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-29
         */
        FITKOCCModelExtrudeCut();

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-29
         */
        virtual ~FITKOCCModelExtrudeCut() = default;

        /**
         * @brief       刷新命令。[重写]
         * @return      执行结果
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-29
         */
        bool update() override;

    };


    /**
     * @brief       旋转切削。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-08-29
     */
    class FITKGeoCompOCCAPI FITKOCCModelRevolCut :
        public Interface::FITKAbsGeoModelRevolCut, public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-29
         */
        FITKOCCModelRevolCut();
    
        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-29
         */
        virtual ~FITKOCCModelRevolCut() = default;

        /**
         * @brief       刷新命令。[重写]
         * @return      执行结果
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-29
         */
        bool update() override;

    };


    /**
     * @brief       扫掠切削。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-08-29
     */
    class FITKGeoCompOCCAPI FITKOCCModelSweepCut :
        public Interface::FITKAbsGeoModelSweepCut, public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-29
         */
        FITKOCCModelSweepCut();

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-29
         */
        virtual ~FITKOCCModelSweepCut() = default;

        /**
         * @brief       刷新命令。[重写]
         * @return      执行结果
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-29
         */
        bool update() override;

    };

    /**
     * @brief       多截面切削。(TODO)
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-08-29
     */
    class FITKGeoCompOCCAPI FITKOCCModelMultiSectionCut :
        public Interface::FITKAbsGeoModelMultiSectionCut, public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-29
         */
        FITKOCCModelMultiSectionCut();

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-29
         */
        virtual ~FITKOCCModelMultiSectionCut() = default;

        /**
         * @brief       刷新命令。[重写]
         * @return      执行结果
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-29
         */
        bool update() override;

    };
}

#endif // !__FITKOCCMODELCUT_H__
