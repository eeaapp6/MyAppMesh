/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCModelPartitionFace.h
 * @brief       OCC分割实体。

 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2025-09-01
 *********************************************************************/

#ifndef  __FITKOCCMODELPARTITIONSOLID_H__
#define  __FITKOCCMODELPARTITIONSOLID_H__

#include "FITKAbstractOCCModel.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelPartitionSolid.h"

class TopoDS_Shape;

namespace OCC
{
    /**
     * @brief       OCC平面分割实体。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-09-01
     */
    class FITKGeoCompOCCAPI FITKOCCModelPartitionSolidWithPlane : public Interface::FITKAbsGeoModelPartitionSolidWithPlane, public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-01
         */
        FITKOCCModelPartitionSolidWithPlane();

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-01
         */
        ~FITKOCCModelPartitionSolidWithPlane() = default;

        /**
         * @brief       更新数据。[重写]
         * @return      是否成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-01
         */
        bool update() override;

    };

    /**
     * @brief       OCC延伸面分割实体。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-09-01
     */
    class FITKGeoCompOCCAPI FITKOCCModelPartitionSolidWithExtendFace : public Interface::FITKAbsGeoModelPartitionSolidWithExtendFace, public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-01
         */
        FITKOCCModelPartitionSolidWithExtendFace();

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-01
         */
        ~FITKOCCModelPartitionSolidWithExtendFace() = default;

        /**
         * @brief       更新数据。[重写]
         * @return      是否成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-01
         */
        bool update() override;

    };

    /**
     * @brief       OCC扫掠线分割实体。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-09-03
     */
    class FITKGeoCompOCCAPI FITKOCCModelPartitionSolidWithSweepEdge : public Interface::FITKAbsGeoModelPartitionSolidWithSweepEdge, public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-03
         */
        FITKOCCModelPartitionSolidWithSweepEdge();

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-03
         */
        ~FITKOCCModelPartitionSolidWithSweepEdge() = default;

        /**
         * @brief       更新数据。[重写]
         * @return      是否成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-03
         */
        bool update() override;

    };

    /**
     * @brief       OCC草图面分割实体。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-09-03
     */
    class FITKGeoCompOCCAPI FITKOCCModelPartitionSolidWithSketchPlanar : public Interface::FITKAbsGeoModelPartitionSolidWithSketchPlanar, public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-03
         */
        FITKOCCModelPartitionSolidWithSketchPlanar();

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-03
         */
        ~FITKOCCModelPartitionSolidWithSketchPlanar() = default;

        /**
         * @brief       更新数据。[重写]
         * @return      是否成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-03
         */
        bool update() override;

    };
}

#endif // !__FITKOCCMODELPARTITIONSOLID_H__
