/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   FITKOCCModelPoint.h
 * @brief  OCC表示的点.
 * @author YanZhiHui (chanyuantiandao@126.com)
 * @date   2024-08-27
 */
#ifndef  FITKOCCMODELPOINT_H
#define  FITKOCCMODELPOINT_H

#include "FITKAbstractOCCModel.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelPoint.h"

namespace OCC
{
    /**
     * @brief  坐标点.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-27
     */
    class FITKGeoCompOCCAPI FITKOCCModelPoint :public Interface::FITKAbsGeoModelPoint,
        public OCCShapeAgent
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        FITKOCCModelPoint();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        ~FITKOCCModelPoint() override = default;
        /**
         * @brief  更新模型.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        virtual bool update() override;
    };
    /**
     * @brief  曲线的端点.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-27
     */
    class FITKGeoCompOCCAPI FITKOCCModelCurveEnd :
        public Interface::FITKAbsGeoModelCurveEnd,
        public OCCShapeAgent
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        FITKOCCModelCurveEnd();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        ~FITKOCCModelCurveEnd() override = default;
        /**
         * @brief  更新模型.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        virtual bool update() override;
    };
    /**
     * @brief  距离曲线的端点指定距离.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-27
     */
    class FITKGeoCompOCCAPI FITKOCCModelCurveEndDistance :
        public Interface::FITKAbsGeoModelCurveEndDistance,
        public OCCShapeAgent
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        FITKOCCModelCurveEndDistance();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        ~FITKOCCModelCurveEndDistance() override = default;
        /**
         * @brief  更新模型.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        virtual bool update() override;
    };
    /**
     * @brief  指定曲线比例创建点.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-27
     */
    class FITKGeoCompOCCAPI FITKOCCModelCurveRadio :
        public Interface::FITKAbsGeoModelCurveRadio,
        public OCCShapeAgent
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        FITKOCCModelCurveRadio();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        ~FITKOCCModelCurveRadio() override = default;
        /**
         * @brief  更新模型.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        virtual bool update() override;
    };
    class FITKGeoCompOCCAPI FITKOCCModelPointProjectionOnCurve :public Interface::FITKAbsGeoModelPointProjectionOnCurve,
        public OCCShapeAgent
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        FITKOCCModelPointProjectionOnCurve();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        ~FITKOCCModelPointProjectionOnCurve() override = default;
        /**
         * @brief  更新模型.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        virtual bool update() override;
    };

    class FITKGeoCompOCCAPI FITKOCCModelTwoCurveIntersection :
        public Interface::FITKAbsGeoModelTwoCurveIntersection, public OCCShapeAgent
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        FITKOCCModelTwoCurveIntersection();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        ~FITKOCCModelTwoCurveIntersection() override = default;
        /**
         * @brief  更新模型.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        virtual bool update() override;
    };

    /**
     * @brief  点在面上的投影.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-27
     */
    class FITKGeoCompOCCAPI FITKOCCModelPointProjectionOnSurface :
        public Interface::FITKAbsGeoModelPointProjectionOnSurface, public OCCShapeAgent
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        FITKOCCModelPointProjectionOnSurface();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        ~FITKOCCModelPointProjectionOnSurface() override = default;
        /**
         * @brief  更新模型.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        virtual bool update() override;

    };
    /**
     * @brief  线和面的交点.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-27
     */
    class FITKGeoCompOCCAPI FITKOCCModelCurveIntersectionSurface :
        public Interface::FITKAbsGeoModelCurveIntersectionSurface, public OCCShapeAgent
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        FITKOCCModelCurveIntersectionSurface();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        ~FITKOCCModelCurveIntersectionSurface() override = default;
        /**
         * @brief  更新模型.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        virtual bool update() override;
    };

    /**
     * @brief  在线、面上交互生成点.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-27
     */
    class FITKGeoCompOCCAPI FITKOCCModelInteractionPoint :
        public Interface::FITKAbsGeoModelInteractionPoint, public OCCShapeAgent
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        FITKOCCModelInteractionPoint();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        ~FITKOCCModelInteractionPoint() override = default;
        /**
         * @brief  更新模型.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        virtual bool update() override;
    };

}

#endif // !FITKOCCMODELPOINT_H
