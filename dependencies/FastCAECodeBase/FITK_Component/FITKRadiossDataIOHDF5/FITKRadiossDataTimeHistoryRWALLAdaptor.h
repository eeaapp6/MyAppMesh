/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossDataTimeHistoryRWALLAdaptor.h
 * @brief  输出请求时间历程刚性墙适配器
 * @author guqingtao (15598887859@163.com)
 * @date   2025-11-11
 *********************************************************************/
#ifndef FITKRadiossDataTimeHistoryRWALLAdaptor_H
#define FITKRadiossDataTimeHistoryRWALLAdaptor_H

#include "FITKRadiossDataHDF5Adaptor.h"
#include "FITKRadiossDataIOHDF5API.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

namespace Radioss
{
    class FITKTimeHistoryRWALL;
}
namespace Interface
{
    /**
     * @brief  输出请求时间历程刚性墙适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-11-11
     */
    class FITKRadiossDataIOHDF5API FITKRadiossDataTimeHistoryRWALLAdaptor : public FITKRadiossDataHDF5Adaptor
    {
    public:
        explicit FITKRadiossDataTimeHistoryRWALLAdaptor() = default;
        ~FITKRadiossDataTimeHistoryRWALLAdaptor() = default;

        virtual bool adaptR() override;
        virtual bool adaptW() override;
    private:

    private:
        /**
         * @brief  输出请求时间历程刚性墙数据
         * @author guqingtao (15598887859@163.com)
         * @date   2025-11-11
         */
        Radioss::FITKTimeHistoryRWALL* _data{};
    };
    Register2FITKIOAdaptorFactory(HDF5, Radioss::FITKTimeHistoryRWALL, FITKRadiossDataTimeHistoryRWALLAdaptor)
}

#endif 

