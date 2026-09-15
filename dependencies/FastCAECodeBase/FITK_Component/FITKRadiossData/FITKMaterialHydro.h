/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKMaterialHydro.h
 * @brief  HYDRO流体材料
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-10-23
 *********************************************************************/
#ifndef __FITK_MATERIAL_HYDRO_H__
#define __FITK_MATERIAL_HYDRO_H__

#include "FITKAbstractRadMaterial.h"

namespace Radioss
{
    struct FITKMaterialHydroData
    {
        // PlasTab材料参数示例（请根据实际需求补充/修改）
        double m_density = 0.0;           //!< 材料密度 [Kg/m3] RHO_I
        double m_R_density = 0.0;               //!< 参考密度 [Kg/m3] RHO_0
        double m_kinematicViscosity = 0.0;//!< 运动粘度 [m3/s]  Knu
        double m_pressureCut = -1e20;     //!< 剪切压力 [Pa]    Pmin
    };

    /**
     * @brief   HYDRO流体材料
     * @details 此类实现了HYDRO流体材料模型，用于描述流体材料
     * @see FITKAbstractMaterial, FITKAbstractRadiossData
     */
    class FITKRadiossDataAPI FITKMaterialHydro : public FITKAbstractRadMaterial
    {
        Q_OBJECT
      
        RadiossKeyWord(FITKMaterialHydro,/MAT/LAW6); //!< Radioss关键字标识
        FITKCLASS(Radioss, FITKMaterialHydro); //!< FITK类标识

    public:
        explicit FITKMaterialHydro() = default;
        virtual ~FITKMaterialHydro() = default;
 
        virtual FITKAbstractRadMaterial::RadMatType getRadMatType() {
            return FITKAbstractRadMaterial::RadMatType::LAW6;
        };
        const FITKMaterialHydroData& getMaterialDataReadOnly() const;
        FITKMaterialHydroData& getMaterialData();

        /**
        * @brief  状态方程ID
        * @author wangning (2185896382@qq.com)
        * @date   2025-11-11
        */
        void setEOSID(int id);
        int getEOSID();

    private: 
        FITKMaterialHydroData m_data;
        int _eosID = -1;
    };
}

#endif
