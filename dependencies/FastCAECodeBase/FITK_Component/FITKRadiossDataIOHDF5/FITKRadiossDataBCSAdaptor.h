/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossDataBCSAdaptor.h
 * @brief  边界条件适配器
 * @author guqingtao (15598887859@163.com)
 * @date   2025-11-10
 *********************************************************************/
#ifndef FITKRadiossDataBCSAdaptor_H
#define FITKRadiossDataBCSAdaptor_H

#include "FITKRadiossDataHDF5Adaptor.h"
#include "FITKRadiossDataIOHDF5API.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

namespace Radioss
{
    class FITKRadiossBCS;
}
namespace Interface
{
    /**
     * @brief  边界条件适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-11-10
     */
    class FITKRadiossDataIOHDF5API FITKRadiossDataBCSAdaptor : public FITKRadiossDataHDF5Adaptor
    {
    public:
        explicit FITKRadiossDataBCSAdaptor() = default;
        ~FITKRadiossDataBCSAdaptor() = default;

        virtual bool adaptR() override;
        virtual bool adaptW() override;
    private:

    private:
        /**
         * @brief  边界条件数据
         * @author guqingtao (15598887859@163.com)
         * @date   2025-11-10
         */
        Radioss::FITKRadiossBCS* _data{};
    };
    Register2FITKIOAdaptorFactory(HDF5, Radioss::FITKRadiossBCS, FITKRadiossDataBCSAdaptor)
}

#endif 

