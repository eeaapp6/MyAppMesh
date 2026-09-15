/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCModelTransformPart.h
 * @brief       OCC部件变换命令实现类。
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2026-04-13
 *********************************************************************/

#ifndef __FITKOCCMODELTRANFORMPART_H__
#define __FITKOCCMODELTRANFORMPART_H__

#include "FITKAbstractOCCModel.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelTransformPart.h"

namespace OCC
{
    /**
     * @brief       OCC部件平移命令实现类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2026-04-13
     */
    class FITKGeoCompOCCAPI FITKOCCModelTranslatePart : public Interface::FITKAbsGeoModelTranslatePart,
        public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        explicit FITKOCCModelTranslatePart();

        /**
         * @brief       析构函数。[虚函数]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        virtual ~FITKOCCModelTranslatePart() = default;

        /**
         * @brief       更新模型数据。[重写]
         * @return      更新是否成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        bool update() override;

    };

    /**
     * @brief       OCC部件旋转命令实现类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2026-04-13
     */
    class FITKGeoCompOCCAPI FITKOCCModelRotatePart : public Interface::FITKAbsGeoModelRotatePart,
        public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        explicit FITKOCCModelRotatePart();

        /**
         * @brief       析构函数。[虚函数]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        virtual ~FITKOCCModelRotatePart() = default;

        /**
         * @brief       更新模型数据。[重写]
         * @return      更新是否成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        bool update() override;

    };

    /**
     * @brief       OCC部件缩放命令实现类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2026-04-13
     */
    class FITKGeoCompOCCAPI FITKOCCModelScalePart : public Interface::FITKAbsGeoModelScalePart,
        public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        explicit FITKOCCModelScalePart();

        /**
         * @brief       析构函数。[虚函数]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        virtual ~FITKOCCModelScalePart() = default;

        /**
         * @brief       更新模型数据。[重写]
         * @return      更新是否成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        bool update() override;

    };

    /**
     * @brief       OCC部件镜像命令实现类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2026-04-13
     */
    class FITKGeoCompOCCAPI FITKOCCModelMirrorPart : public Interface::FITKAbsGeoModelMirrorPart,
        public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        explicit FITKOCCModelMirrorPart();

        /**
         * @brief       析构函数。[虚函数]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        virtual ~FITKOCCModelMirrorPart() = default;

        /**
         * @brief       更新模型数据。[重写]
         * @return      更新是否成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        bool update() override;

    };

    /**
     * @brief       OCC部件线性阵列命令实现类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2026-06-23
     */
    class FITKGeoCompOCCAPI FITKOCCModelRectangularPatternPart : public Interface::FITKAbsGeoModelRectangularPatternPart,
        public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        explicit FITKOCCModelRectangularPatternPart();

        /**
         * @brief       析构函数。[虚函数]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        virtual ~FITKOCCModelRectangularPatternPart() = default;

        /**
         * @brief       更新模型数据。[重写]
         * @return      更新是否成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        bool update() override;

    };

    /**
     * @brief       OCC部件环形阵列命令实现类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2026-06-23
     */
    class FITKGeoCompOCCAPI FITKOCCModelCircularPatternPart : public Interface::FITKAbsGeoModelCircularPatternPart,
        public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        explicit FITKOCCModelCircularPatternPart();

        /**
         * @brief       析构函数。[虚函数]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        virtual ~FITKOCCModelCircularPatternPart() = default;

        /**
         * @brief       更新模型数据。[重写]
         * @return      更新是否成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        bool update() override;

    };
}

#endif // !__FITKOCCMODELTRANFORMPART_H__
