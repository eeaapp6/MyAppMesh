/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossDataFailureModelBIQUADAdaptor.h
 * @brief  失效模型BIQUAD适配器
 * @author guqingtao (15598887859@163.com)
 * @date   2025-11-11
 *********************************************************************/
#ifndef FITKRadiossDataFailureModelBIQUADAdaptor_H
#define FITKRadiossDataFailureModelBIQUADAdaptor_H

#include "FITKRadiossDataHDF5Adaptor.h"
#include "FITKRadiossDataIOHDF5API.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

namespace Radioss
{
    class FITKFailureModelBIQUAD;
}
namespace Interface
{
    /**
     * @brief  失效模型BIQUAD适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-11-11
     */
    class FITKRadiossDataIOHDF5API FITKRadiossDataFailureModelBIQUADAdaptor : public FITKRadiossDataHDF5Adaptor
    {
    public:
        explicit FITKRadiossDataFailureModelBIQUADAdaptor() = default;
        ~FITKRadiossDataFailureModelBIQUADAdaptor() = default;

        virtual bool adaptR() override;
        virtual bool adaptW() override;
    private:

    private:
        /**
         * @brief  失效模型BIQUAD数据
         * @author guqingtao (15598887859@163.com)
         * @date   2025-11-11
         */
        Radioss::FITKFailureModelBIQUAD* _data{};
    };
    Register2FITKIOAdaptorFactory(HDF5, Radioss::FITKFailureModelBIQUAD, FITKRadiossDataFailureModelBIQUADAdaptor)
}

#endif 

