/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossDataTimeHistoryACCELAdaptor.h
 * @brief  输出请求时间历程加速度计适配器
 * @author guqingtao (15598887859@163.com)
 * @date   2025-11-11
 *********************************************************************/
#ifndef FITKRadiossDataTimeHistoryACCELAdaptor_H
#define FITKRadiossDataTimeHistoryACCELAdaptor_H

#include "FITKRadiossDataHDF5Adaptor.h"
#include "FITKRadiossDataIOHDF5API.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

namespace Radioss
{
    class FITKTimeHistoryACCEL;
}
namespace Interface
{
    /**
     * @brief  输出请求时间历程加速度计适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-11-11
     */
    class FITKRadiossDataIOHDF5API FITKRadiossDataTimeHistoryACCELAdaptor : public FITKRadiossDataHDF5Adaptor
    {
    public:
        explicit FITKRadiossDataTimeHistoryACCELAdaptor() = default;
        ~FITKRadiossDataTimeHistoryACCELAdaptor() = default;

        virtual bool adaptR() override;
        virtual bool adaptW() override;
    private:

    private:
        /**
         * @brief  输出请求时间历程加速度计数据
         * @author guqingtao (15598887859@163.com)
         * @date   2025-11-11
         */
        Radioss::FITKTimeHistoryACCEL* _data{};
    };
    Register2FITKIOAdaptorFactory(HDF5, Radioss::FITKTimeHistoryACCEL, FITKRadiossDataTimeHistoryACCELAdaptor)
}

#endif 

