/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossDataNonLinerCurveAdaptor.h
 * @brief  非线性曲线数据适配器
 * @author guqingtao (15598887859@163.com)
 * @date   2025-09-30
 *********************************************************************/
#ifndef FITKRadiossDataNonLinerCurveAdaptor_H
#define FITKRadiossDataNonLinerCurveAdaptor_H

#include "FITKRadiossDataHDF5Adaptor.h"
#include "FITKRadiossDataIOHDF5API.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

namespace Radioss
{
    class FITKNonLinerCurve;
}
namespace Interface
{
    /**
     * @brief  非线性曲线数据适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-09-30
     */
    class FITKRadiossDataIOHDF5API FITKRadiossDataNonLinerCurveAdaptor : public FITKRadiossDataHDF5Adaptor
    {
    public:
        explicit FITKRadiossDataNonLinerCurveAdaptor() = default;
        ~FITKRadiossDataNonLinerCurveAdaptor() = default;

        virtual bool adaptR() override;
        virtual bool adaptW() override;
    private:

    private:
        /**
         * @brief  非线性曲线数据
         * @author guqingtao (15598887859@163.com)
         * @date   2025-09-30
         */
        Radioss::FITKNonLinerCurve* _curveData{};
    };
    Register2FITKIOAdaptorFactory(HDF5, Radioss::FITKNonLinerCurve, FITKRadiossDataNonLinerCurveAdaptor)
}

#endif 

