/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossDataSolverSettingsAdaptor.h
 * @brief  求解设置数据适配器
 * @author guqingtao (15598887859@163.com)
 * @date   2025-10-14
 *********************************************************************/
#ifndef FITKRadiossDataSolverSettingsAdaptor_H
#define FITKRadiossDataSolverSettingsAdaptor_H

#include "FITKRadiossDataHDF5Adaptor.h"
#include "FITKRadiossDataIOHDF5API.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

namespace Radioss
{
    class FITKRadiossSolverSettings;
}
namespace Interface
{
    /**
     * @brief  求解设置数据适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-10-14
     */
    class FITKRadiossDataIOHDF5API FITKRadiossDataSolverSettingsAdaptor : public FITKRadiossDataHDF5Adaptor
    {
    public:
        explicit FITKRadiossDataSolverSettingsAdaptor() = default;
        ~FITKRadiossDataSolverSettingsAdaptor() = default;

        virtual bool adaptR() override;
        virtual bool adaptW() override;
    private:

    private:
        /**
         * @brief  求解设置数据
         * @author guqingtao (15598887859@163.com)
         * @date   2025-10-14
         */
        Radioss::FITKRadiossSolverSettings* _settingData{};
    };
    Register2FITKIOAdaptorFactory(HDF5, Radioss::FITKRadiossSolverSettings, FITKRadiossDataSolverSettingsAdaptor)
}

#endif 

