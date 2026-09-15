/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossDataSetAdaptor.h
 * @brief  Radioss集合数据适配器
 * @author guqingtao (15598887859@163.com)
 * @date   2025-09-29
 *********************************************************************/
#ifndef FITKRadiossDataSetAdaptor_H
#define FITKRadiossDataSetAdaptor_H

#include "FITKRadiossDataHDF5Adaptor.h"
#include "FITKRadiossDataIOHDF5API.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

namespace Radioss
{
    class FITKRadiossNodeGroup;
}
namespace Interface
{
    /**
     * @brief  Radioss集合数据适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-09-29
     */
    class FITKRadiossDataIOHDF5API FITKRadiossDataSetAdaptor : public FITKRadiossDataHDF5Adaptor
    {
    public:
        explicit FITKRadiossDataSetAdaptor() = default;
        ~FITKRadiossDataSetAdaptor() = default;

        virtual bool adaptR() override;
        virtual bool adaptW() override;
    private:

    private:
        /**
         * @brief  集合数据
         * @author guqingtao (15598887859@163.com)
         * @date   2025-09-29
         */
        Radioss::FITKRadiossNodeGroup* _setData{};
    };
    Register2FITKIOAdaptorFactory(HDF5, Radioss::FITKRadiossNodeGroup, FITKRadiossDataSetAdaptor)
}

#endif 

