/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossDataPropShellAdaptor.h
 * @brief  Radioss壳单元属性数据适配器
 * @author guqingtao (15598887859@163.com)
 * @date   2025-09-30
 *********************************************************************/
#ifndef FITKRadiossDataPropShellAdaptor_H
#define FITKRadiossDataPropShellAdaptor_H

#include "FITKRadiossDataHDF5Adaptor.h"
#include "FITKRadiossDataIOHDF5API.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

namespace Radioss
{
    class FITKPropShell;
}
namespace Interface
{
    /**
     * @brief  Radioss壳单元属性数据适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-09-30
     */
    class FITKRadiossDataIOHDF5API FITKRadiossDataPropShellAdaptor : public FITKRadiossDataHDF5Adaptor
    {
    public:
        explicit FITKRadiossDataPropShellAdaptor() = default;
        ~FITKRadiossDataPropShellAdaptor() = default;

        virtual bool adaptR() override;
        virtual bool adaptW() override;
    private:

    private:
        /**
         * @brief  Radioss壳单元属性数据适配器
         * @author guqingtao (15598887859@163.com)
         * @date   2025-09-30
         */
        Radioss::FITKPropShell* _shellData{};
    };
    Register2FITKIOAdaptorFactory(HDF5, Radioss::FITKPropShell, FITKRadiossDataPropShellAdaptor)
}

#endif 

