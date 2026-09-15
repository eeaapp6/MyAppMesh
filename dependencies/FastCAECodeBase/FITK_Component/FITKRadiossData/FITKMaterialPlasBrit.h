/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKMaterialPlasBrit.h
 * @brief  PLAS_BRIT各向异性脆性失效材料
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-10-23
 *********************************************************************/
#ifndef __FITK_MATERIAL_PLAS_BRIT_H__
#define __FITK_MATERIAL_PLAS_BRIT_H__

#include "FITKAbstractRadMaterial.h"

namespace Radioss
{
    struct FITKMaterialPlasBritData
    {
        // PlasTab材料参数示例（请根据实际需求补充/修改）
        double m_density = 0.0;                      //!< 材料密度 [Kg/m3]
        double m_youngsModulus = 0.0;                //!< 杨氏模量 [Pa]         E
        double m_poissonsRatio = 0.0;                //!< 泊松比                v
        double m_plasticityYieldStress = 0.0;        //!< 塑性屈服应力          a
        double m_plasticityHardeningParameter = 0.0; //!< 塑性硬化参数          b
        double m_plasticityHardeningExponent = 0.0;  //!< 塑性硬化指数          n
        double m_plasticityMaximumStress = 0.0;      //!< 塑性最大应力          SIG_max0
        double m_strainRateCoefficient = 0.0;        //!< 应变率系数            c
        double m_referenceStrainRate = 0.0;          //!< 参考应变速率          EPS_DOT_0
        int m_strainRateComputation = 0;             //!< 应变率计算标志        ICC
        int m_strainRateSmoothing = 0;               //!< 应变率平滑标志        Fsmooth
        double m_cutoffFreq = 1e30;                   //!< 应变率平滑的截止频率  Fcut
        double m_tensileFilureStrain1 = 1e30;         //!< 方向1拉伸破坏应变     EPS_t1
        double m_maximumTensileFilureStrain1 = 1.1e30;//!< 方向1最大拉伸破坏应变 EPS_m1
        double m_maximumDamageFactor1 = 0.999;        //!< 方向1最大损伤因子     d_max1
        double m_maximumTensileStrain1 = 1.2e30;      //!< 方向1最大拉伸应变     EPS_f1
        double m_tensileFilureStrain2 = 1e30;         //!< 方向2最大拉伸破坏应变 EPS_t2
        double m_maximumTensileFilureStrain2 = 1.1e30;//!< 方向2最大拉伸破坏应变 EPS_m2
        double m_maximumDamageFactor2 = 0.999;        //!< 方向2最大损伤因子     d_max2
        double m_maximumTensileStrain2 = 1.2e30;      //!< 方向2最大拉伸破坏应变 EPS_f2
    };

    /**
     * @brief   PLAS_BRIT各向异性脆性失效材料
     * @details 此类实现了PLAS_BRIT各向异性脆性失效材料模型，用于描述模型在失效之前考虑了材料损伤。失效和损伤仅发生在拉伸状态下。
     * @see FITKAbstractMaterial, FITKAbstractRadiossData
     */
    class FITKRadiossDataAPI FITKMaterialPlasBrit : public FITKAbstractRadMaterial
    {
        Q_OBJECT
      
        RadiossKeyWord(FITKMaterialPlasBrit,/MAT/LAW27); //!< Radioss关键字标识
        FITKCLASS(Radioss, FITKMaterialPlasBrit); //!< FITK类标识

    public:       
        explicit FITKMaterialPlasBrit() = default;
        virtual ~FITKMaterialPlasBrit() = default;

        virtual FITKAbstractRadMaterial::RadMatType getRadMatType() {
            return FITKAbstractRadMaterial::RadMatType::LAW27;
        };
        const FITKMaterialPlasBritData& getMaterialDataReadOnly() const;
        FITKMaterialPlasBritData& getMaterialData();

    private: 
        FITKMaterialPlasBritData m_data;
    };
}

#endif
