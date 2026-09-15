/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCModelPartitionEdge.h
 * @brief       OCC分割边。

 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2025-09-01
 *********************************************************************/

#ifndef  __FITKOCCMODELPARTITIONEDGE_H__
#define  __FITKOCCMODELPARTITIONEDGE_H__

#include "FITKAbstractOCCModel.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelPartitionEdge.h"

class TopoDS_Shape;

namespace OCC
{
    /**
     * @brief       OCC参数值分割边。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-09-01
     */
    class FITKGeoCompOCCAPI FITKOCCModelPartitionEdgeWithParameter : public Interface::FITKAbsGeoModelPartitionEdgeWithParameter, public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-01
         */
        FITKOCCModelPartitionEdgeWithParameter();

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-01
         */
        ~FITKOCCModelPartitionEdgeWithParameter() = default;

        /**
         * @brief       更新数据。[重写]
         * @return      是否成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-01
         */
        bool update() override;

    };    
    
    /**
     * @brief       OCC坐标分割边。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-09-01
     */
    class FITKGeoCompOCCAPI FITKOCCModelPartitionEdgeWithPoint : public Interface::FITKAbsGeoModelPartitionEdgeWithPoint, public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-01
         */
        FITKOCCModelPartitionEdgeWithPoint();

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-01
         */
        ~FITKOCCModelPartitionEdgeWithPoint() = default;

        /**
         * @brief       更新数据。[重写]
         * @return      是否成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-01
         */
        bool update() override;

    };    
    
    /**
     * @brief       OCC基准面分割边。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-09-01
     */
    class FITKGeoCompOCCAPI FITKOCCModelPartitionEdgeWithDatumPlane : public Interface::FITKAbsGeoModelPartitionEdgeWithDatumPlane, public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-01
         */
        FITKOCCModelPartitionEdgeWithDatumPlane();

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-01
         */
        ~FITKOCCModelPartitionEdgeWithDatumPlane() = default;

        /**
         * @brief       更新数据。[重写]
         * @return      是否成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-01
         */
        bool update() override;

    };
}

#endif // !__FITKOCCMODELPARTITIONEDGE_H__
