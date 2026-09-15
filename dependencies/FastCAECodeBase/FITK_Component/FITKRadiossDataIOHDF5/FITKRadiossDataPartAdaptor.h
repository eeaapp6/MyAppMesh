/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossDataPartAdaptor.h
 * @brief  Part数据HDF5适配器
 * @author guqingtao (15598887859@163.com)
 * @date   2025-09-28
 *********************************************************************/
#ifndef FITKRadiossDataPartAdaptor_H
#define FITKRadiossDataPartAdaptor_H

#include "FITKRadiossDataHDF5Adaptor.h"
#include "FITKRadiossDataIOHDF5API.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractElement.h"
#include "FITK_Interface/FITKInterfaceModel/FITKModelEnum.h"

namespace Radioss
{
    class FITKRadiossPart;
}
namespace Interface
{
    /**
     * @brief  Part数据HDF5适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-09-28
     */
    class FITKRadiossDataIOHDF5API FITKRadiossDataPartAdaptor : public FITKRadiossDataHDF5Adaptor
    {
    public:
        explicit FITKRadiossDataPartAdaptor() = default;
        ~FITKRadiossDataPartAdaptor() = default;

        virtual bool adaptR() override;
        virtual bool adaptW() override;
    private:
        bool getDataSetDim(H5::Group& h5group, std::string name, int& n, int& m);
    private:
        /**
         * @brief  part数据
         * @author guqingtao (15598887859@163.com)
         * @date   2025-09-28
         */
        Radioss::FITKRadiossPart* _partData{};
    };
    Register2FITKIOAdaptorFactory(HDF5, Radioss::FITKRadiossPart, FITKRadiossDataPartAdaptor)
}

#endif 

