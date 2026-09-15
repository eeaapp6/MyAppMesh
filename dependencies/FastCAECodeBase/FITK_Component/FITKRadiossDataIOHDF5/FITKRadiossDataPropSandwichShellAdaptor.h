/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossDataPropSandwichShellAdaptor.h
 * @brief  夹层壳属性数据适配器
 * @author guqingtao (15598887859@163.com)
 * @date   2025-10-31
 *********************************************************************/
#ifndef FITKRadiossDataPropSandwichShellAdaptor_H
#define FITKRadiossDataPropSandwichShellAdaptor_H

#include "FITKRadiossDataHDF5Adaptor.h"
#include "FITKRadiossDataIOHDF5API.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

namespace Radioss
{
    class FITKPropSandwichShell;
}
namespace Interface
{
    /**
     * @brief  夹层壳属性数据适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-10-31
     */
    class FITKRadiossDataIOHDF5API FITKRadiossDataPropSandwichShellAdaptor : public FITKRadiossDataHDF5Adaptor
    {
    public:
        explicit FITKRadiossDataPropSandwichShellAdaptor() = default;
        ~FITKRadiossDataPropSandwichShellAdaptor() = default;

        virtual bool adaptR() override;
        virtual bool adaptW() override;
    private:

    private:
        /**
         * @brief  夹层壳属性数据
         * @author guqingtao (15598887859@163.com)
         * @date   2025-10-31
         */
        Radioss::FITKPropSandwichShell* _data{};
    };
    Register2FITKIOAdaptorFactory(HDF5, Radioss::FITKPropSandwichShell, FITKRadiossDataPropSandwichShellAdaptor)
}

#endif 

