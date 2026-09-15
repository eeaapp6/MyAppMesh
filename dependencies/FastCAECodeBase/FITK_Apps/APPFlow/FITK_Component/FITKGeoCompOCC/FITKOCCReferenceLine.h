/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef FITKOCCREFERENCELINE_H
#define FITKOCCREFERENCELINE_H

#include "FITKAbstractOCCModel.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoReferenceLine.h"

namespace OCC
{
    /**
     * @brief  坐标点.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-27
     */
    class FITKGeoCompOCCAPI FITKOCCReferenceLine :public Interface::FITKAbsGeoReferenceLine, public OCCShapeAgent
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-04
         */
        FITKOCCReferenceLine();
        /**
         * @brief  .
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-04
         */
        ~FITKOCCReferenceLine() override = default;
        /**
         * @brief  更新数据对象.
         * @return 是否更新成功
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-04
         */
        virtual bool update() override;
    };   

    /**
     * @brief       OCC面面交线创建基准线。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-09-01
     */
    class FITKGeoCompOCCAPI FITKOCCReferenceLineByIntersectPlanes :public Interface::FITKAbsGeoReferenceLineByIntersectPlanes, public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-01
         */
        FITKOCCReferenceLineByIntersectPlanes();

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-01
         */
        ~FITKOCCReferenceLineByIntersectPlanes() override = default;

        /**
         * @brief       更新数据。[重写]
         * @return      是否成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-01
         */
        bool update() override;

    };

    /**
     * @brief       OCC旋转轴创建基准线。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-09-01
     */
    class FITKGeoCompOCCAPI FITKOCCReferenceLineByRotateLine :public Interface::FITKAbsGeoReferenceLineByRotateLine, public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-01
         */
        FITKOCCReferenceLineByRotateLine();

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-01
         */
        ~FITKOCCReferenceLineByRotateLine() override = default;

        /**
         * @brief       更新数据。[重写]
         * @return      是否成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-01
         */
        bool update() override;

    };
}
#endif // FITKOCCREFERENCELINE_H
