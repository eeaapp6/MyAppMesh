/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef _FITKOCC_SAMPLE_CURVE_H___
#define _FITKOCC_SAMPLE_CURVE_H___

#include "FITKAbstractOCCModel.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoSampleCurve.h"
#include <Geom_Curve.hxx>
#include <vector>

namespace OCC
{
    /**
     * @brief 使用OCC引擎进行曲线采样的类
     * @details 该类继承自FITKAbsGeoSampleCurve，提供了等参采样、等弧长采样和等弦长采样的方法
     * @author libaojun
     */
    class FITKGeoCompOCCAPI FITKOCCSampleCurve : public Interface::FITKAbsGeoSampleCurve,
        public OCCShapeAgent
    {
    public:
        /**
         * @brief 构造函数
         * @author libaojun
         */
        explicit FITKOCCSampleCurve();

        /**
         * @brief 析构函数
         * @author libaojun
         */
        virtual ~FITKOCCSampleCurve() = default;

        /**
         * @brief 更新模型
         * @return true 更新成功
         * @return false 更新失败
         * @author libaojun
         */
        virtual bool update() override;

    private:
        /**
         * @brief 等参采样
         * @param curve 要采样的曲线
         * @param start 曲线的起始参数
         * @param end 曲线的结束参数
         * @param numPoints 采样点数量
         * @return 采样点的坐标列表
         * @author libaojun
         */
        std::vector<gp_Pnt> sampleParametric(const Handle(Geom_Curve)& curve, double start, double end, int numPoints);

        /**
         * @brief 等弧长采样
         * @param curve 要采样的曲线
         * @param start 曲线的起始参数
         * @param end 曲线的结束参数
         * @param numPoints 采样点数量
         * @return 采样点的坐标列表
         * @author libaojun
         */
        std::vector<gp_Pnt> sampleArcLength(const Handle(Geom_Curve)& curve, double start, double end, int numPoints);

        /**
         * @brief 等弦长采样
         * @param curve 要采样的曲线
         * @param start 曲线的起始参数
         * @param end 曲线的结束参数
         * @param numPoints 采样点数量
         * @return 采样点的坐标列表
         * @author libaojun
         */
        std::vector<gp_Pnt> sampleChordLength(const Handle(Geom_Curve)& curve, double start, double end, int numPoints);
    };
}

#endif


