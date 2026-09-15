/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossDataGravityAdaptor.h
 * @brief  重力场数据适配器
 * @author guqingtao (15598887859@163.com)
 * @date   2025-10-09
 *********************************************************************/
#ifndef FITKRadiossDataGravityAdaptor_H
#define FITKRadiossDataGravityAdaptor_H

#include "FITKRadiossDataHDF5Adaptor.h"
#include "FITKRadiossDataIOHDF5API.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

namespace Radioss
{
    class FITKGravity;
}
namespace Interface
{
    /**
     * @brief  重力场数据适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-10-09
     */
    class FITKRadiossDataIOHDF5API FITKRadiossDataGravityAdaptor : public FITKRadiossDataHDF5Adaptor
    {
    public:
        explicit FITKRadiossDataGravityAdaptor() = default;
        ~FITKRadiossDataGravityAdaptor() = default;

        virtual bool adaptR() override;
        virtual bool adaptW() override;
    private:

    private:
        /**
         * @brief  重力场数据
         * @author guqingtao (15598887859@163.com)
         * @date   2025-10-09
         */
        Radioss::FITKGravity* _gravityData{};
    };
    Register2FITKIOAdaptorFactory(HDF5, Radioss::FITKGravity, FITKRadiossDataGravityAdaptor)
}

#endif 

