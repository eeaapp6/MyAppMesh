/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossDataMaterialHydroAdaptor.h
 * @brief  流体材料适配器
 * @author guqingtao (15598887859@163.com)
 * @date   2025-10-30
 *********************************************************************/
#ifndef FITKRadiossDataMaterialHydroAdaptor_H
#define FITKRadiossDataMaterialHydroAdaptor_H

#include "FITKRadiossDataHDF5Adaptor.h"
#include "FITKRadiossDataIOHDF5API.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

namespace Radioss
{
    class FITKMaterialHydro;
}
namespace Interface
{
    /**
     * @brief  流体材料适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-10-30
     */
    class FITKRadiossDataIOHDF5API FITKRadiossDataMaterialHydroAdaptor : public FITKRadiossDataHDF5Adaptor
    {
    public:
        explicit FITKRadiossDataMaterialHydroAdaptor() = default;
        ~FITKRadiossDataMaterialHydroAdaptor() = default;

        virtual bool adaptR() override;
        virtual bool adaptW() override;
    private:

    private:
        /**
         * @brief  流体材料
         * @author guqingtao (15598887859@163.com)
         * @date   2025-10-30
         */
        Radioss::FITKMaterialHydro* _data{};
    };
    Register2FITKIOAdaptorFactory(HDF5, Radioss::FITKMaterialHydro, FITKRadiossDataMaterialHydroAdaptor)
}

#endif 

