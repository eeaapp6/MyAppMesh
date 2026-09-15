/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossAdaptorBCS.h
 * @brief  边界条件读取适配器
 * @author zhangjingx (hiter25@outlook.com)
 * @date   2025-07-24
 *********************************************************************/
#ifndef __FITKRADIOSSADAPTORBCS_H__
#define __FITKRADIOSSADAPTORBCS_H__
#include "FITKRadiossRadIOAPI.h"
#include "FITKRadiossAbstractAdaptor.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

namespace Radioss
{
    class FITKRadiossBCS;
    /**
     * @brief  边界条件读取适配器
     * @author zhangjingx (hiter25@outlook.com)
     * @date   2025-07-24
     */
    class FITKRadiossRadIOAPI FITKRadiossAdaptorBCS : public FITKRadiossAbstractAdaptor
    {
    public:
        explicit FITKRadiossAdaptorBCS() = default;
        ~FITKRadiossAdaptorBCS() = default;
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
        bool readBCS(FITKRadiossBCS* bcs);
        /**
         * @brief    读取参数
         * @param[i] lines 
         * @param[i] bcs 
         * @return   bool
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-24
         */
        bool readParam(const QStringList& lines, FITKRadiossBCS* bcs);
    };
    /**
     * @brief  注册适配器
     * @author zhangjingx (hiter25@outlook.com)
     * @date   2025-07-09
     */
    Register2FITKIOAdaptorFactory(rad, Radioss::FITKRadiossBCS, FITKRadiossAdaptorBCS);
    RegRadiossRadAdaptor(/BCS, FITKRadiossAdaptorBCS);
}
#endif //!__FITKRADIOSSADAPTORBCS_H__
