/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossDataPropBeamSPRAdaptor.h
 * @brief  梁单元属性数据适配器
 * @author guqingtao (15598887859@163.com)
 * @date   2025-09-30
 *********************************************************************/
#ifndef FITKRadiossDataPropBeamSPRAdaptor_H
#define FITKRadiossDataPropBeamSPRAdaptor_H

#include "FITKRadiossDataHDF5Adaptor.h"
#include "FITKRadiossDataIOHDF5API.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

#include "FITK_Component/FITKRadiossData/FITKPropBeamSPR.h"

namespace Radioss
{
    class FITKPropBeamSPR;
}
namespace Interface
{
    /**
     * @brief  梁单元属性数据适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-09-30
     */
    class FITKRadiossDataIOHDF5API FITKRadiossDataPropBeamSPRAdaptor : public FITKRadiossDataHDF5Adaptor
    {
    public:
        explicit FITKRadiossDataPropBeamSPRAdaptor() = default;
        ~FITKRadiossDataPropBeamSPRAdaptor() = default;

        virtual bool adaptR() override;
        virtual bool adaptW() override;
    private:
        bool writeValueDOF(const Radioss::ValueDOF& valueDOF,QString name);
        bool readValueDOF(Radioss::ValueDOF& valueDOF,QString name);
    private:
        /**
         * @brief  梁单元属性数据
         * @author guqingtao (15598887859@163.com)
         * @date   2025-09-30
         */
        Radioss::FITKPropBeamSPR* _beamSPRData{};
    };
    Register2FITKIOAdaptorFactory(HDF5, Radioss::FITKPropBeamSPR, FITKRadiossDataPropBeamSPRAdaptor)
}

#endif 

