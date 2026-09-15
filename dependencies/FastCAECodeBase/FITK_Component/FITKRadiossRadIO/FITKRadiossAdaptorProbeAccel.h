/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossAdaptorProbeAccel.h
 * @brief  加速度计读写适配器
 * @author wangning (2185896382@qq.com)
 * @date   2025-09-24
 *********************************************************************/
#ifndef __FITKRadiossAdaptorProbeAccel_H__
#define __FITKRadiossAdaptorProbeAccel_H__
#include "FITKRadiossRadIOAPI.h"
#include "FITKRadiossAbstractAdaptor.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

namespace Radioss
{
    class FITKProbeAcceleration;
    /**
     * @brief  加速度计读写适配器
     * @author wangning (2185896382@qq.com)
     * @date   2025-09-15
     */
    class FITKRadiossRadIOAPI FITKRadiossAdaptorProbeAccel : public FITKRadiossAbstractAdaptor
    {
    public:
        explicit FITKRadiossAdaptorProbeAccel() = default;
        ~FITKRadiossAdaptorProbeAccel() = default;
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
         * @brief    读取
         * @return   bool
         * @author wangning (2185896382@qq.com)
         * @date   2025-09-15
         */
        bool readAccel(FITKProbeAcceleration* accel);
        /**
         * @brief    读取文件参数并赋值给对象
         * @param[i] lines
         * @param[i] accel
         * @return   bool
         * @author wangning (2185896382@qq.com)
         * @date   2025-09-15
         */
        bool readParam(const QStringList& lines, FITKProbeAcceleration* accel);
    };
    /**
     * @brief  注册适配器
     * @author wangning (2185896382@qq.com)
     * @date   2025-09-15
     */
    Register2FITKIOAdaptorFactory(rad, Radioss::FITKProbeAcceleration, FITKRadiossAdaptorProbeAccel);
    RegRadiossRadAdaptor(/ACCEL, FITKRadiossAdaptorProbeAccel);
}
#endif //!__FITKRADIOSSADAPTORPROPSHELLVALUE_H__
