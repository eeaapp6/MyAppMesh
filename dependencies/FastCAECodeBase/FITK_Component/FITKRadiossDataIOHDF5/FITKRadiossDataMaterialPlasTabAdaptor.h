/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossDataMaterialPlasTabAdaptor.h
 * @brief  PlasTab材料数据适配器
 * @author guqingtao (15598887859@163.com)
 * @date   2025-09-29
 *********************************************************************/
#ifndef FITKRadiossDataMaterialPlasTabAdaptor_H
#define FITKRadiossDataMaterialPlasTabAdaptor_H

#include "FITKRadiossDataHDF5Adaptor.h"
#include "FITKRadiossDataIOHDF5API.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

namespace Radioss
{
    class FITKMaterialPlasTab;
}
namespace Interface
{
    /**
     * @brief  PlasTab材料数据
     * @author guqingtao (15598887859@163.com)
     * @date   2025-09-29
     */
    class FITKRadiossDataIOHDF5API FITKRadiossDataMaterialPlasTabAdaptor : public FITKRadiossDataHDF5Adaptor
    {
    public:
        explicit FITKRadiossDataMaterialPlasTabAdaptor() = default;
        ~FITKRadiossDataMaterialPlasTabAdaptor() = default;

        virtual bool adaptR() override;
        virtual bool adaptW() override;
    private:

    private:
        /**
         * @brief  PlasTab材料数据
         * @author guqingtao (15598887859@163.com)
         * @date   2025-09-29
         */
        Radioss::FITKMaterialPlasTab* _plasTabData{};
    };
    Register2FITKIOAdaptorFactory(HDF5, Radioss::FITKMaterialPlasTab, FITKRadiossDataMaterialPlasTabAdaptor)
}

#endif 

