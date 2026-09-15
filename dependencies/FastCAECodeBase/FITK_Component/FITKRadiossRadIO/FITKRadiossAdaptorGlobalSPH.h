/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossAdaptorGlobalSPH.h
 * @brief  全局SPH读取适配器，设置某些参数的默认值
 * @author wangning (2185896382@qq.com)
 * @date   2025-09-15
 *********************************************************************/
#ifndef __FITKRadiossAdaptorGlobalSPH_H__
#define __FITKRadiossAdaptorGlobalSPH_H__

#include "FITKRadiossRadIOAPI.h"
#include "FITKRadiossAbstractAdaptor.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

namespace Radioss
{
    class FITKRadiossSolverSPHGlobal;
    /**
     * @brief  全局SPH读取适配器，设置某些参数的默认值
     * @author wangning (2185896382@qq.com)
     * @date   2025-09-15
     */
    class FITKRadiossRadIOAPI FITKRadiossAdaptorGlobalSPH : public FITKRadiossAbstractAdaptor
    {
    public:
        explicit FITKRadiossAdaptorGlobalSPH() = default;
        ~FITKRadiossAdaptorGlobalSPH() = default;
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
        bool readGlobalSPH(FITKRadiossSolverSPHGlobal* sph);
        /**
         * @brief    读取文件参数并赋值给对象
         * @param[i] lines
         * @param[i] sph
         * @return   bool
         * @author wangning (2185896382@qq.com)
         * @date   2025-09-15
         */
        bool readParam(const QStringList& lines, FITKRadiossSolverSPHGlobal* sph);
    };
    /**
     * @brief  注册适配器
     * @author wangning (2185896382@qq.com)
     * @date   2025-09-15
     */
    Register2FITKIOAdaptorFactory(rad, Radioss::FITKRadiossSolverSPHGlobal, FITKRadiossAdaptorGlobalSPH);
    RegRadiossRadAdaptor(/SPHGLO, FITKRadiossAdaptorGlobalSPH);
}
#endif //!__FITKRADIOSSADAPTORPROPSHELLVALUE_H__
