/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossAdaptorInteractionMUImpactDefault.h
 * @brief  默认MultiUsageImpact读取适配器，设置某些参数的默认值
 * @author wangning (2185896382@qq.com)
 * @date   2025-09-26
 *********************************************************************/
#ifndef __FITKRadiossAdaptorInteractionMUImpactDefault_H__
#define __FITKRadiossAdaptorInteractionMUImpactDefault_H__
#include "FITKRadiossRadIOAPI.h"
#include "FITKRadiossAbstractAdaptor.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

namespace Radioss
{
    class FITKInteractionMultiUsageImpactDefaultValue;
    /**
     * @brief  默认MultiUsageImpact属性读取适配器，设置某些参数的默认值
     * @author wangning (2185896382@qq.com)
     * @date   2025-09-26
     */
    class FITKRadiossRadIOAPI FITKRadiossAdaptorInteractionMUImpactDefault : public FITKRadiossAbstractAdaptor
    {
    public:
        explicit FITKRadiossAdaptorInteractionMUImpactDefault() = default;
        ~FITKRadiossAdaptorInteractionMUImpactDefault() = default;
        /**
         * @brief
         * @return   QString
         * @author wangning (2185896382@qq.com)
         * @date   2025-09-15
         */
        QString getAdaptorClass() override;
        /**
         * @brief    适配器读取
         * @return   bool
         * @author wangning (2185896382@qq.com)
         * @date   2025-09-15
         */
        bool adaptR() override;
        /**
         * @brief    适配器写出
         * @return   bool
         * @author wangning (2185896382@qq.com)
         * @date   2025-09-15
         */
        bool adaptW() override;
    private:
        /**
         * @brief    读取value
         * @return   bool
         * @author wangning (2185896382@qq.com)
         * @date   2025-09-15
         */
        bool readDefMultiUsageImpact(FITKInteractionMultiUsageImpactDefaultValue* MUImpact);
        /**
         * @brief    读取文件参数并赋值给对象
         * @param[i] lines
         * @param[i] MUImpact
         * @return   bool
         * @author wangning (2185896382@qq.com)
         * @date   2025-09-15
         */
        bool readParam(const QStringList& lines, FITKInteractionMultiUsageImpactDefaultValue* MUImpact);
    };
    /**
     * @brief  注册适配器
     * @author wangning (2185896382@qq.com)
     * @date   2025-09-15
     */
    Register2FITKIOAdaptorFactory(rad, Radioss::FITKInteractionMultiUsageImpactDefaultValue, FITKRadiossAdaptorInteractionMUImpactDefault);
    RegRadiossRadAdaptor(/DEFAULT/INTER/TYPE7, FITKRadiossAdaptorInteractionMUImpactDefault);
}
#endif //!__FITKRADIOSSADAPTORPROPSHELLVALUE_H__
