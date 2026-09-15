/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   FITKOCCReferencePlane.h
 * @brief  基准面.
 * @author YanZhiHui (chanyuantiandao@126.com)
 * @date   2024-09-04
 */
#ifndef FITKOCCREFERENCEPLANE_H
#define FITKOCCREFERENCEPLANE_H

#include "FITKAbstractOCCModel.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoReferencePlane.h"
#include <array>

namespace OCC
{
    /**
     * @brief  基准面.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-27
     */
    class FITKGeoCompOCCAPI FITKOCCReferencePlane :
        public Interface::FITKAbsGeoReferencePlane, public OCCShapeAgent
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-04
         */
        FITKOCCReferencePlane();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-04
         */
        ~FITKOCCReferencePlane() override = default;
        /**
         * @brief  更新数据对象.
         * @return 是否更新成功
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-04
         */
        virtual bool update() override;
    };

    /**
     * @brief  偏移方式创建基准面.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-27
     */
    class FITKGeoCompOCCAPI FITKOCCReferenceOffsetPlane :
        public Interface::FITKAbsGeoReferenceOffsetPlane, public OCCShapeAgent
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-04
         */
        FITKOCCReferenceOffsetPlane();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-04
         */
        ~FITKOCCReferenceOffsetPlane() override = default;
        /**
         * @brief  更新数据对象.
         * @return 是否更新成功
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-04
         */
        virtual bool update() override;
    };
    /**
     * @brief  三点创建参考面类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-16
     */
    class FITKGeoCompOCCAPI FITKOCCReferenceThreePointsPlane :
        public Interface::FITKAbsGeoReferenceThreePointsPlane, public OCCShapeAgent
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-04
         */
        FITKOCCReferenceThreePointsPlane();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-04
         */
        ~FITKOCCReferenceThreePointsPlane() override = default;
        /**
         * @brief  更新数据对象.
         * @return 是否更新成功
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-04
         */
        virtual bool update() override;
    };

    /**
     * @brief  方程（Ax+By+Cz+D=0）创建参考面类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-16
     */
    class FITKGeoCompOCCAPI FITKOCCReferenceEquationPlane :
        public Interface::FITKAbsGeoReferenceEquationPlane, public OCCShapeAgent
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-04
         */
        FITKOCCReferenceEquationPlane();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-04
         */
        ~FITKOCCReferenceEquationPlane() override = default;
        /**
         * @brief  更新数据对象.
         * @return 是否更新成功
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-04
         */
        virtual bool update() override;
    };

    /**
     * @brief  点和法线创建参考面类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-16
     */
    class FITKGeoCompOCCAPI FITKOCCReferencePointAndDirectionPlane :
        public Interface::FITKAbsGeoReferencePointAndDirectionPlane, public OCCShapeAgent
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-04
         */
        FITKOCCReferencePointAndDirectionPlane();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-04
         */
        ~FITKOCCReferencePointAndDirectionPlane() override = default;
        /**
         * @brief  更新数据对象.
         * @return 是否更新成功
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-04
         */
        virtual bool update() override;
    };
    
    /**
     * @brief       OCC旋转平面创建基准面。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-09-01
     */
    class FITKGeoCompOCCAPI FITKOCCReferenceRotateFromPlanePlane :public Interface::FITKAbsGeoReferenceRotateFromPlanePlane, public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-01
         */
        FITKOCCReferenceRotateFromPlanePlane();

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-01
         */
        ~FITKOCCReferenceRotateFromPlanePlane() override = default;

        /**
         * @brief       更新数据。[重写]
         * @return      是否成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-01
         */
        bool update() override;

    };
}
#endif // FITKOCCREFERENCEPLANE_H
