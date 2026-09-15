/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCModelPartitionFace.h
 * @brief       OCC分割面。

 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2025-08-29
 *********************************************************************/

#ifndef  __FITKOCCMODELPARTITIONFACE_H__
#define  __FITKOCCMODELPARTITIONFACE_H__

#include "FITKAbstractOCCModel.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelPartitionFace.h"

class TopoDS_Shape;

namespace OCC
{
    /**
     * @brief       OCC草图分割面。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-08-29
     */
    class FITKGeoCompOCCAPI FITKOCCModelPartitionFaceWithSketch : 
        public Interface::FITKAbsGeoModelPartitionFaceWithSketch, public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-29
         */
        FITKOCCModelPartitionFaceWithSketch();

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-29
         */
        ~FITKOCCModelPartitionFaceWithSketch() = default;

        /**
         * @brief       更新数据。[重写]
         * @return      是否成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-29
         */
        bool update() override;

    };
    
    /**
     * @brief       OCC两点分割面。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-08-29
     */
    class FITKGeoCompOCCAPI FITKOCCModelPartitionFaceWithTwoPoints :
        public Interface::FITKAbsGeoModelPartitionFaceWithTwoPoints, public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-29
         */
        FITKOCCModelPartitionFaceWithTwoPoints();

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-29
         */
        ~FITKOCCModelPartitionFaceWithTwoPoints() = default;

        /**
         * @brief       更新数据。[重写]
         * @return      是否成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-29
         */
        bool update() override;

    };

    /**
     * @brief       OCC基准面分割面。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-08-29
     */
    class FITKGeoCompOCCAPI FITKOCCModelPartitionFaceWithDatumPlane :
        public Interface::FITKAbsGeoModelPartitionFaceWithDatumPlane, public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-29
         */
        FITKOCCModelPartitionFaceWithDatumPlane();

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-29
         */
        ~FITKOCCModelPartitionFaceWithDatumPlane() = default;

        /**
         * @brief       更新数据。[重写]
         * @return      是否成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-29
         */
        bool update() override;

    };

    /**
     * @brief       OCC延长线路径分割面。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-09-26
     */
    class FITKGeoCompOCCAPI FITKOCCModelPartitionFaceWithCurvedPath :
        public Interface::FITKAbsGeoModelPartitionFaceWithCurvedPath, public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-26
         */
        FITKOCCModelPartitionFaceWithCurvedPath();

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-26
         */
        ~FITKOCCModelPartitionFaceWithCurvedPath() = default;

        /**
         * @brief       更新数据。[重写]
         * @return      是否成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-26
         */
        bool update() override;

    };

    /**
     * @brief       OCC延伸面分割面。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-09-01
     */
    class FITKGeoCompOCCAPI FITKOCCModelPartitionFaceWithExtendFace :
        public Interface::FITKAbsGeoModelPartitionFaceWithExtendFace, public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-01
         */
        FITKOCCModelPartitionFaceWithExtendFace();

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-01
         */
        ~FITKOCCModelPartitionFaceWithExtendFace() = default;

        /**
         * @brief       更新数据。[重写]
         * @return      是否成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-01
         */
        bool update() override;

    };

    /**
     * @brief       OCC相交面分割面。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-09-08
     */
    class FITKGeoCompOCCAPI FITKOCCModelPartitionFaceWithIntersectFace :
        public Interface::FITKAbsGeoModelPartitionFaceWithIntersectFace, public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-08
         */
        FITKOCCModelPartitionFaceWithIntersectFace();

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-08
         */
        ~FITKOCCModelPartitionFaceWithIntersectFace() = default;

        /**
         * @brief       更新数据。[重写]
         * @return      是否成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-08
         */
        bool update() override;

    };

    /**
     * @brief       OCC投影边分割面。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-09-01
     */
    class FITKGeoCompOCCAPI FITKOCCModelPartitionFaceWithProjectEdges :
        public Interface::FITKAbsGeoModelPartitionFaceWithProjectEdges, public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-01
         */
        FITKOCCModelPartitionFaceWithProjectEdges();

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-01
         */
        ~FITKOCCModelPartitionFaceWithProjectEdges() = default;

        /**
         * @brief       更新数据。[重写]
         * @return      是否成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-01
         */
        bool update() override;

    };
}

#endif // !__FITKOCCMODELPARTITIONFACE_H__
