/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossDataMaterialElasticAdaptor.h
 * @brief  Elastic材料数据适配器
 * @author guqingtao (15598887859@163.com)
 * @date   2025-09-29
 *********************************************************************/
#ifndef FITKRadiossDataMaterialElasticAdaptor_H
#define FITKRadiossDataMaterialElasticAdaptor_H

#include "FITKRadiossDataHDF5Adaptor.h"
#include "FITKRadiossDataIOHDF5API.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

namespace Radioss
{
    class FITKMaterialElastic;
}
namespace Interface
{
    /**
     * @brief  Elastic材料数据适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-09-29
     */
    class FITKRadiossDataIOHDF5API FITKRadiossDataMaterialElasticAdaptor : public FITKRadiossDataHDF5Adaptor
    {
    public:
        explicit FITKRadiossDataMaterialElasticAdaptor() = default;
        ~FITKRadiossDataMaterialElasticAdaptor() = default;

        virtual bool adaptR() override;
        virtual bool adaptW() override;
    private:

    private:
        /**
         * @brief  Elastic材料数据
         * @author guqingtao (15598887859@163.com)
         * @date   2025-09-29
         */
        Radioss::FITKMaterialElastic* _elasticData{};
    };
    Register2FITKIOAdaptorFactory(HDF5, Radioss::FITKMaterialElastic, FITKRadiossDataMaterialElasticAdaptor)
}

#endif 

