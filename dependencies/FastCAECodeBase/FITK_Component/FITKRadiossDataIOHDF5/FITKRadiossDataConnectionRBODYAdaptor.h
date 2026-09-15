/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossDataConnectionRBODYAdaptor.h
 * @brief  刚体连接适配器
 * @author guqingtao (15598887859@163.com)
 * @date   2025-11-10
 *********************************************************************/
#ifndef FITKRadiossDataConnectionRBODYAdaptor_H
#define FITKRadiossDataConnectionRBODYAdaptor_H

#include "FITKRadiossDataHDF5Adaptor.h"
#include "FITKRadiossDataIOHDF5API.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

namespace Radioss
{
    class FITKConnectionRBODY;
}
namespace Interface
{
    /**
     * @brief  刚体连接适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-11-10
     */
    class FITKRadiossDataIOHDF5API FITKRadiossDataConnectionRBODYAdaptor : public FITKRadiossDataHDF5Adaptor
    {
    public:
        explicit FITKRadiossDataConnectionRBODYAdaptor() = default;
        ~FITKRadiossDataConnectionRBODYAdaptor() = default;

        virtual bool adaptR() override;
        virtual bool adaptW() override;
    private:

    private:
        /**
         * @brief  刚体连接数据
         * @author guqingtao (15598887859@163.com)
         * @date   2025-11-10
         */
        Radioss::FITKConnectionRBODY* _data{};
    };
    Register2FITKIOAdaptorFactory(HDF5, Radioss::FITKConnectionRBODY, FITKRadiossDataConnectionRBODYAdaptor)
}

#endif 

