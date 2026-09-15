/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef  FITKOCCMODELCURVE_H
#define  FITKOCCMODELCURVE_H

#include "FITKAbstractOCCModel.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelCurve.h"

namespace OCC
{
    /**
     * @brief  无限直线OCC实现类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-13
     */
    class FITKGeoCompOCCAPI FITKOCCModelLine : public Interface::FITKAbsGeoModelLine
        , public OCCShapeAgent
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        FITKOCCModelLine();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        ~FITKOCCModelLine() override = default;
        /**
         * @brief  更新模型.
         * @return 是否成功
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        virtual bool update() override;
    };

    /**
     * @brief  有限线段抽象类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-13
     */
    class FITKGeoCompOCCAPI FITKOCCModelSegment : public Interface::FITKAbsGeoModelSegment, public OCCShapeAgent
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        FITKOCCModelSegment();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        ~FITKOCCModelSegment() override = default;
        /**
         * @brief  更新模型.
         * @return 是否成功
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        virtual bool update() override;
    };
    /**
     * @brief  多段线.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-13
     */
    class FITKGeoCompOCCAPI FITKOCCModelWire : public Interface::FITKAbsGeoModelWire, public OCCShapeAgent
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        FITKOCCModelWire();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        ~FITKOCCModelWire() override = default;
        /**
         * @brief  更新模型.
         * @return 是否成功
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        virtual bool update() override;

    };
    /**
     * @brief  圆.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-13
     */
    class FITKGeoCompOCCAPI FITKOCCModelCircle : public Interface::FITKAbsGeoModelCircle, public OCCShapeAgent
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        FITKOCCModelCircle();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        ~FITKOCCModelCircle() override = default;
        /**
         * @brief  更新模型.
         * @return 是否成功
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        virtual bool update() override;
    };


    /**
     * @brief  三点圆.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-13
     */
    class FITKGeoCompOCCAPI FITKOCCModelThreePointsCircle : public Interface::FITKAbsGeoModelThreePointsCircle, public OCCShapeAgent
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        FITKOCCModelThreePointsCircle();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        ~FITKOCCModelThreePointsCircle() override = default;
        /**
         * @brief  更新模型.
         * @return 是否成功
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        virtual bool update() override;
    };

    /**
     * @brief  圆弧.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-13
     */
    class FITKGeoCompOCCAPI FITKOCCModelCircleArc : public Interface::FITKAbsGeoModelCircleArc, public OCCShapeAgent
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        FITKOCCModelCircleArc();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        ~FITKOCCModelCircleArc() override = default;
        /**
         * @brief  更新模型.
         * @return 是否成功
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        virtual bool update() override;
    };

    /**
     * @brief  椭圆.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-13
     */
    class FITKGeoCompOCCAPI FITKOCCModelEllipse : public Interface::FITKAbsGeoModelEllipse, public OCCShapeAgent
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        FITKOCCModelEllipse();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        ~FITKOCCModelEllipse() override = default;
        /**
         * @brief  更新模型.
         * @return 是否成功
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        virtual bool update() override;
    };
    /**
     * @brief  三点椭圆.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-13
     */
    class FITKGeoCompOCCAPI FITKOCCModelThreePointsEllipse : public Interface::FITKAbsGeoModelThreePointsEllipse, public OCCShapeAgent
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        FITKOCCModelThreePointsEllipse();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        ~FITKOCCModelThreePointsEllipse() override = default;
        /**
         * @brief  更新模型.
         * @return 是否成功
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        virtual bool update() override;
    };

    /**
     * @brief  椭圆弧.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-13
     */
    class FITKGeoCompOCCAPI FITKOCCModelEllipseArc : public Interface::FITKAbsGeoModelEllipseArc, public OCCShapeAgent
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        FITKOCCModelEllipseArc();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        ~FITKOCCModelEllipseArc() override = default;
        /**
         * @brief  更新模型.
         * @return 是否成功
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        virtual bool update() override;
    };
    /**
     * @brief  双曲线.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-13
     */
    class FITKGeoCompOCCAPI FITKOCCModelHyperbola : public Interface::FITKAbsGeoModelHyperbola, public OCCShapeAgent
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        FITKOCCModelHyperbola();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        ~FITKOCCModelHyperbola() override = default;
        /**
         * @brief  更新模型.
         * @return 是否成功
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        virtual bool update() override;
    };
    /**
     * @brief  三点双曲线.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-13
     */
    class FITKGeoCompOCCAPI FITKOCCModelThreePointsHyperbola : public Interface::FITKAbsGeoModelThreePointsHyperbola, public OCCShapeAgent
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        FITKOCCModelThreePointsHyperbola();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        ~FITKOCCModelThreePointsHyperbola() override = default;
        /**
         * @brief  更新模型.
         * @return 是否成功
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        virtual bool update() override;
    };

    /**
     * @brief  双曲线弧.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-13
     */
    class FITKGeoCompOCCAPI FITKOCCModelHyperbolaArc : public Interface::FITKAbsGeoModelHyperbolaArc, public OCCShapeAgent
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        FITKOCCModelHyperbolaArc();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        ~FITKOCCModelHyperbolaArc() override = default;
        /**
         * @brief  更新模型.
         * @return 是否成功
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        virtual bool update() override;
    };

    /**
     * @brief  抛物线.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-13
     */
    class FITKGeoCompOCCAPI FITKOCCModelParabola : public Interface::FITKAbsGeoModelParabola, public OCCShapeAgent
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        FITKOCCModelParabola();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        ~FITKOCCModelParabola() override = default;
        /**
         * @brief  更新模型.
         * @return 是否成功
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        virtual bool update() override;
    };
    /**
     * @brief  三点抛物线.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-13
     */
    class FITKGeoCompOCCAPI FITKOCCModelThreePointsParabola : public Interface::FITKAbsGeoModelThreePointsParabola, public OCCShapeAgent
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        FITKOCCModelThreePointsParabola();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        ~FITKOCCModelThreePointsParabola() override = default;
        /**
         * @brief  更新模型.
         * @return 是否成功
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        virtual bool update() override;
    };

    /**
     * @brief  抛物线弧.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-13
     */
    class FITKGeoCompOCCAPI FITKOCCModelParabolaArc : public Interface::FITKAbsGeoModelParabolaArc, public OCCShapeAgent
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        FITKOCCModelParabolaArc();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        ~FITKOCCModelParabolaArc() override = default;
        /**
         * @brief  更新模型.
         * @return 是否成功
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        virtual bool update() override;
    };

    /**
     * @brief  通过控制点创建贝塞尔曲线.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-09-07
     */
    class FITKGeoCompOCCAPI FITKOCCModelBeizeByControlPoints : public Interface::FITKAbsGeoModelBeizeByControlPoints, public OCCShapeAgent {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        FITKOCCModelBeizeByControlPoints();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        ~FITKOCCModelBeizeByControlPoints() override = default;
        /**
         * @brief  更新模型.
         * @return 是否成功
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        virtual bool update() override;
    };
    /**
     * @brief  通过线上的点创建B样条曲线.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-09-07
     */
    class FITKGeoCompOCCAPI FITKOCCModelBsplineByThroughPoints : public Interface::FITKAbsGeoModelBsplineByThroughPoints, public OCCShapeAgent {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        FITKOCCModelBsplineByThroughPoints();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        ~FITKOCCModelBsplineByThroughPoints() override = default;
        /**
         * @brief  更新模型.
         * @return 是否成功
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        virtual bool update() override;
    };
    /**
     * @brief  裁剪曲线OCC实现类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-13
     */
    class FITKGeoCompOCCAPI FITKOCCModelTrimmedCurve : public Interface::FITKAbsGeoModelTrimmedCurve, public OCCShapeAgent
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        FITKOCCModelTrimmedCurve();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        ~FITKOCCModelTrimmedCurve() override = default;
        /**
         * @brief  更新模型.
         * @return 是否成功
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        virtual bool update() override;
    };

    /**
    * @brief  偏移曲线.
    * @author YanZhiHui (chanyuantiandao@126.com)
    * @date   2024-08-13
    */
    class FITKGeoCompOCCAPI FITKOCCModelOffsetCurve :public Interface::FITKAbsGeoModelOffsetCurve, public OCCShapeAgent
    {
    public:
        /**
          * @brief  构造函数.
          * @author YanZhiHui (chanyuantiandao@126.com)
          * @date   2024-08-13
          */
        FITKOCCModelOffsetCurve();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        ~FITKOCCModelOffsetCurve() override = default;
        /**
         * @brief  更新模型.
         * @return 是否成功
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        virtual bool update() override;
    };

    /**
     * @brief  线在面上的投影.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-27
     */
    class FITKGeoCompOCCAPI FITKOCCModelCurveProjectionOnSurface :
        public Interface::FITKAbsGeoModelCurveProjectionOnSurface, public OCCShapeAgent
    {
    public:
        /**
          * @brief  构造函数.
          * @author YanZhiHui (chanyuantiandao@126.com)
          * @date   2024-08-13
          */
        FITKOCCModelCurveProjectionOnSurface();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        ~FITKOCCModelCurveProjectionOnSurface() override = default;
        /**
         * @brief  更新模型.
         * @return 是否成功
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        virtual bool update() override;
    };
    /**
     * @brief  面和面的交线.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-27
     */
    class FITKGeoCompOCCAPI FITKOCCModelSurfaceIntersectionSurface :
        public Interface::FITKAbsGeoModelSurfaceIntersectionSurface, public OCCShapeAgent
    {
    public:
        /**
          * @brief  构造函数.
          * @author YanZhiHui (chanyuantiandao@126.com)
          * @date   2024-08-13
          */
        FITKOCCModelSurfaceIntersectionSurface();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        ~FITKOCCModelSurfaceIntersectionSurface() override = default;
        /**
         * @brief  更新模型.
         * @return 是否成功
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        virtual bool update() override;
    };

    /**
     * @brief  桥接曲线.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-27
     */
    class FITKGeoCompOCCAPI FITKOCCModelBridgeCurve :
        public Interface::FITKAbsGeoModelBridgeCurve, public OCCShapeAgent
    {
    public:
        /**
          * @brief  构造函数.
          * @author YanZhiHui (chanyuantiandao@126.com)
          * @date   2024-08-13
          */
        FITKOCCModelBridgeCurve();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        ~FITKOCCModelBridgeCurve() override = default;
        /**
         * @brief  更新模型.
         * @return 是否成功
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        virtual bool update() override;
    };

    /**
     * @brief  面的边线.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-27
     */
    class FITKGeoCompOCCAPI FITKOCCModelSurfaceEdge :
        public Interface::FITKAbsGeoModelSurfaceEdge, public OCCShapeAgent
    {
    public:
        /**
          * @brief  构造函数.
          * @author YanZhiHui (chanyuantiandao@126.com)
          * @date   2024-08-13
          */
        FITKOCCModelSurfaceEdge();
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        ~FITKOCCModelSurfaceEdge() override = default;
        /**
         * @brief  更新模型.
         * @return 是否成功
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        virtual bool update() override;
    };

}

#endif // !FITKOCCMODELCURVE_H
