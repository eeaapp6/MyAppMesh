/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossAdaptorMaterialHydro.h
 * @brief  流体材料读取适配器
 * @author   wangning
 * @date     2025-10-24
 *********************************************************************/
#ifndef _FITKRadiossAdaptorMaterialHydro_H_
#define _FITKRadiossAdaptorMaterialHydro_H_
#include "FITKRadiossRadIOAPI.h"
#include "FITKRadiossAbstractAdaptor.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

namespace Radioss
{
    class FITKMaterialHydro;
    /**
     * @brief  流体材料读取适配器
     * @author   wangning
     * @date     2025-10-24
     */
    class FITKRadiossRadIOAPI FITKRadiossAdaptorMaterialHydro : public FITKRadiossAbstractAdaptor
    {
    public:
        explicit FITKRadiossAdaptorMaterialHydro() = default;
        ~FITKRadiossAdaptorMaterialHydro() = default;
        /**
         * @brief    获取适配器数据类型名
         * @return   QString
         * @author   wangning
         * @date     2025-10-24
         */
        QString getAdaptorClass() override;
        /**
         * @brief    适配器读取
         * @return   bool true成功，false失败
         * @author   wangning
         * @date     2025-10-24
         */
        bool adaptR() override;
        /**
         * @brief    适配器写出
         * @return   bool true成功，false失败
         * @author   wangning
         * @date     2025-10-24
         */
        bool adaptW() override;
    private:
        /**
         * @brief    读取材料
         * @param[i] mat
         * @return   bool
         * @author   wangning
         * @date     2025-10-24
         */
        bool readMaterial(FITKMaterialHydro* mat);
        /**
         * @brief    读取参数
         * @param[i] lines
         * @param[i] mat
         * @return   bool
         * @author   wangning
         * @date     2025-10-24
         */
        bool readParam(const QStringList& lines, FITKMaterialHydro* mat);
    };
    /**
     * @brief 注册适配器
     * @date   2025-07-07
     */
    Register2FITKIOAdaptorFactory(rad, Radioss::FITKMaterialHydro, FITKRadiossAdaptorMaterialHydro);
    RegRadiossRadAdaptor(/MAT/LAW6, FITKRadiossAdaptorMaterialHydro);
    RegRadiossRadAdaptor(/MAT/HYDRO, FITKRadiossAdaptorMaterialHydro, 1);
    RegRadiossRadAdaptor(/MAT/HYD_VISC, FITKRadiossAdaptorMaterialHydro, 2);
}
#endif 