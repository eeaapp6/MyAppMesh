/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossAdaptorBCSCyclic.h
 * @brief  周期边界条件读取适配器
 * @author zhangjingx (hiter25@outlook.com)
 * @date   2025-07-24
 *********************************************************************/
#ifndef __FITKRADIOSSADAPTORBCSCYCLIC_H__
#define __FITKRADIOSSADAPTORBCSCYCLIC_H__
#include "FITKRadiossRadIOAPI.h"
#include "FITKRadiossAbstractAdaptor.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

namespace Radioss
{
    class FITKRadiossBCSCyclic;
    /**
     * @brief  周期边界条件读取适配器
     * @author zhangjingx (hiter25@outlook.com)
     * @date   2025-07-24
     */
    class FITKRadiossRadIOAPI FITKRadiossAdaptorBCSCyclic : public FITKRadiossAbstractAdaptor
    {
    public:
        explicit FITKRadiossAdaptorBCSCyclic() = default;
        ~FITKRadiossAdaptorBCSCyclic() = default;
        /**
         * @brief
         * @return   QString
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-08
         */
        QString getAdaptorClass() override;
        /**
         * @brief    适配器读取
         * @return   bool
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-08
         */
        bool adaptR() override;
        /**
         * @brief    适配器写出
         * @return   bool
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-08
         */
        bool adaptW() override;
    private:
        /**
         * @brief    读取边界条件
         * @param[i] part 
         * @return   bool
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-11
         */
        bool readBCS(FITKRadiossBCSCyclic* bcs);
    };
    /**
     * @brief  注册适配器
     * @author zhangjingx (hiter25@outlook.com)
     * @date   2025-07-09
     */
    Register2FITKIOAdaptorFactory(rad, Radioss::FITKRadiossBCSCyclic, FITKRadiossAdaptorBCSCyclic);
    RegRadiossRadAdaptor(/BCS/CYCLIC, FITKRadiossAdaptorBCSCyclic);
}
#endif //!__FITKRADIOSSADAPTORBCSCYCLIC_H__
