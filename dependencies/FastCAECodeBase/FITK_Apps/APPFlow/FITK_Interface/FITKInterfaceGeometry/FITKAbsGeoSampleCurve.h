/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef _FITK_ABS_GEO_SAMPLE_CURVE_H___
#define _FITK_ABS_GEO_SAMPLE_CURVE_H___

#include "FITKInterfaceGeometryAPI.h"
#include "FITKAbsGeoCommand.h"

namespace Interface
{
    /**
     * @brief 曲线采样类
     * @details 该类用于对曲线进行采样，支持等参采样、等弧长采样和等弦长采样三种方式
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoSampleCurve :
        public FITKAbsGeoCommand
    {
        FITKCLASS(Interface, FITKAbsGeoSampleCurve);
        Q_OBJECT
 //           FITKCLASS(Interface, FITKAbsGeoSampleCurve)
    public:
        /**
         * @brief 采样方法枚举
         */
        enum SampleMethod
        {
            Parametric,  ///< 等参采样
            ArcLength,   ///< 等弧长采样
            ChordLength  ///< 等弦长采样
        };
        Q_ENUM(SampleMethod)

    public:
        explicit FITKAbsGeoSampleCurve() = default;
        virtual ~FITKAbsGeoSampleCurve() = 0;

        /**
         * @brief 设置采样方法
         * @param method 采样方法
         */
        void setSampleMethod(SampleMethod method);

        /**
         * @brief 获取采样方法
         * @return 采样方法
         */
        SampleMethod getSampleMethod() const;

        /**
         * @brief 设置采样点数量
         * @param count 采样点数量
         */
        void setSamplePointCount(int count);

        /**
         * @brief 获取采样点数量
         * @return 采样点数量
         */
        int getSamplePointCount() const;

        /**
         * @brief 设置采样的曲线对象
         * @param curve 采样的曲线对象
         */
        void setSampleCurve(const VirtualShape& curve);

        /**
         * @brief 获取采样的曲线对象
         * @return 采样的曲线对象
         */
        VirtualShape getSampleCurve() const;

        /**
         * @brief 获取几何命令类型
         * @return FITKGeoEnum::FITKGeometryComType
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

    protected:
        SampleMethod _sampleMethod{ Parametric }; ///< 采样方法
        int _samplePointCount{ 0 }; ///< 采样点数量
        VirtualShape _sampleCurve{}; ///< 采样的曲线对象
    };
}

#endif

