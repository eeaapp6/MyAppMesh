/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossDataTimeHistorySECTIOAdaptor.h
 * @brief  输出请求时间历程截面力适配器
 * @author guqingtao (15598887859@163.com)
 * @date   2025-11-11
 *********************************************************************/
#ifndef FITKRadiossDataTimeHistorySECTIOAdaptor_H
#define FITKRadiossDataTimeHistorySECTIOAdaptor_H

#include "FITKRadiossDataHDF5Adaptor.h"
#include "FITKRadiossDataIOHDF5API.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

namespace Radioss
{
    class FITKTimeHistorySECTIO;
}
namespace Interface
{
    /**
     * @brief  输出请求时间历程截面力适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-11-11
     */
    class FITKRadiossDataIOHDF5API FITKRadiossDataTimeHistorySECTIOAdaptor : public FITKRadiossDataHDF5Adaptor
    {
    public:
        explicit FITKRadiossDataTimeHistorySECTIOAdaptor() = default;
        ~FITKRadiossDataTimeHistorySECTIOAdaptor() = default;

        virtual bool adaptR() override;
        virtual bool adaptW() override;
    private:

    private:
        /**
         * @brief  输出请求时间历程截面力数据
         * @author guqingtao (15598887859@163.com)
         * @date   2025-11-11
         */
        Radioss::FITKTimeHistorySECTIO* _data{};
    };
    Register2FITKIOAdaptorFactory(HDF5, Radioss::FITKTimeHistorySECTIO, FITKRadiossDataTimeHistorySECTIOAdaptor)
}

#endif 

