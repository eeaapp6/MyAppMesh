/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossDataFailureModelJOHNSONAdaptor.h
 * @brief  失效模型JOHNSON适配器
 * @author guqingtao (15598887859@163.com)
 * @date   2025-11-11
 *********************************************************************/
#ifndef FITKRadiossDataFailureModelJOHNSONAdaptor_H
#define FITKRadiossDataFailureModelJOHNSONAdaptor_H

#include "FITKRadiossDataHDF5Adaptor.h"
#include "FITKRadiossDataIOHDF5API.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

namespace Radioss
{
    class FITKFailureModelJOHNSON;
}
namespace Interface
{
    /**
     * @brief  失效模型JOHNSON适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-11-11
     */
    class FITKRadiossDataIOHDF5API FITKRadiossDataFailureModelJOHNSONAdaptor : public FITKRadiossDataHDF5Adaptor
    {
    public:
        explicit FITKRadiossDataFailureModelJOHNSONAdaptor() = default;
        ~FITKRadiossDataFailureModelJOHNSONAdaptor() = default;

        virtual bool adaptR() override;
        virtual bool adaptW() override;
    private:

    private:
        /**
         * @brief  失效模型JOHNSON数据
         * @author guqingtao (15598887859@163.com)
         * @date   2025-11-11
         */
        Radioss::FITKFailureModelJOHNSON* _data{};
    };
    Register2FITKIOAdaptorFactory(HDF5, Radioss::FITKFailureModelJOHNSON, FITKRadiossDataFailureModelJOHNSONAdaptor)
}

#endif 

