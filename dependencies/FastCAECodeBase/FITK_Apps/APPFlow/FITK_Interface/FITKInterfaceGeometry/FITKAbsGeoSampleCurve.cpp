/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbsGeoSampleCurve.h"

namespace Interface
{
    FITKAbsGeoSampleCurve::~FITKAbsGeoSampleCurve()
    {
        // 析构函数的具体实现（如果有需要的话）
    }

    void FITKAbsGeoSampleCurve::setSampleMethod(SampleMethod method)
    {
        _sampleMethod = method;
    }

    FITKAbsGeoSampleCurve::SampleMethod FITKAbsGeoSampleCurve::getSampleMethod() const
    {
        return _sampleMethod;
    }

    void FITKAbsGeoSampleCurve::setSamplePointCount(int count)
    {
        _samplePointCount = count;
    }

    int FITKAbsGeoSampleCurve::getSamplePointCount() const
    {
        return _samplePointCount;
    }

    void FITKAbsGeoSampleCurve::setSampleCurve(const VirtualShape& curve)
    {
        _sampleCurve = curve;
    }

    VirtualShape FITKAbsGeoSampleCurve::getSampleCurve() const
    {
        return _sampleCurve;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoSampleCurve::getGeometryCommandType()
    {
        // 返回几何命令类型，具体实现根据实际需求进行修改
        return FITKGeoEnum::FITKGeometryComType::FGTSampleCurve; // 替换为实际的几何命令类型
    }
}



