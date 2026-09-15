/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossDataPropSPHAdaptor.h
 * @brief  SPH平滑粒子流体动力属性适配器
 * @author guqingtao (15598887859@163.com)
 * @date   2025-10-31
 *********************************************************************/
#ifndef FITKRadiossDataPropSPHAdaptor_H
#define FITKRadiossDataPropSPHAdaptor_H

#include "FITKRadiossDataHDF5Adaptor.h"
#include "FITKRadiossDataIOHDF5API.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

namespace Radioss
{
    class FITKPropSPH;
}
namespace Interface
{
    /**
     * @brief  SPH属性数据适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-10-31
     */
    class FITKRadiossDataIOHDF5API FITKRadiossDataPropSPHAdaptor : public FITKRadiossDataHDF5Adaptor
    {
    public:
        explicit FITKRadiossDataPropSPHAdaptor() = default;
        ~FITKRadiossDataPropSPHAdaptor() = default;

        virtual bool adaptR() override;
        virtual bool adaptW() override;
    private:

    private:
        /**
         * @brief  SPH平滑粒子流体动力属性数据
         * @author guqingtao (15598887859@163.com)
         * @date   2025-10-31
         */
        Radioss::FITKPropSPH* _data{};
    };
    Register2FITKIOAdaptorFactory(HDF5, Radioss::FITKPropSPH, FITKRadiossDataPropSPHAdaptor)
}

#endif 

