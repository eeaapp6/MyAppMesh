/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossDataInteractionMultiUsageImpactAdaptor.h
 * @brief  多用途冲击适配器
 * @author guqingtao (15598887859@163.com)
 * @date   2025-11-03
 *********************************************************************/
#ifndef FITKRadiossDataInteractionMultiUsageImpactAdaptor_H
#define FITKRadiossDataInteractionMultiUsageImpactAdaptor_H

#include "FITKRadiossDataHDF5Adaptor.h"
#include "FITKRadiossDataIOHDF5API.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

namespace Radioss
{
    class FITKInteractionMultiUsageImpact;
}
namespace Interface
{
    /**
     * @brief  多用途冲击适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-11-03
     */
    class FITKRadiossDataIOHDF5API FITKRadiossDataInteractionMultiUsageImpactAdaptor : public FITKRadiossDataHDF5Adaptor
    {
    public:
        explicit FITKRadiossDataInteractionMultiUsageImpactAdaptor() = default;
        ~FITKRadiossDataInteractionMultiUsageImpactAdaptor() = default;

        virtual bool adaptR() override;
        virtual bool adaptW() override;
    private:

    private:
        /**
         * @brief  多用途冲击数据
         * @author guqingtao (15598887859@163.com)
         * @date   2025-11-03
         */
        Radioss::FITKInteractionMultiUsageImpact* _data{};
    };
    Register2FITKIOAdaptorFactory(HDF5, Radioss::FITKInteractionMultiUsageImpact, FITKRadiossDataInteractionMultiUsageImpactAdaptor)
}

#endif 

