/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef __FITK_MATERIAL_PLAS_TAB_H__
#define __FITK_MATERIAL_PLAS_TAB_H__

#include "FITKAbstractRadMaterial.h"

namespace Radioss
{
    struct FITKMaterialPlasTabData
    {
        // PlasTab材料参数示例（请根据实际需求补充/修改）
        double m_density = 0.0;           //!< 材料密度 [Kg/m3]
        double m_youngsModulus = 0.0;     //!< 杨氏模量 [Pa]
        double m_poissonsRatio = 0.0;     //!< 泊松比
        double m_failureStrain = 1e30;    //!< 失效应变
        double m_tensileFailureStrain = 1e20; //!< 拉伸失效应变
        double m_maxTensileFailureStrain = 2e20; //!< 最大拉伸失效应力 [Pa]
        int    m_numberOfFunctions = 0; //!< 函数数量 范围[0，100],若为0，将没有最后三个参数
        int    m_smoothStrainRateOptFlag = 0; //!< 平滑应变率选项标志
        double m_Chard = 0; //!< 硬化系数 
        double m_Fcut = 1e20; //!< 应变率平滑截止频率  [Hz]
        int    m_vpFlag = 0; //!<  strain rate choice flag
        double m_tensileStrainForElementDeletion = 3e20; //!< 元素删除的拉伸应变
        int    m_fctIDp  = 0; //!<  Yield factor versus pressure function
        double m_fscaleFactor = 1.0; //!< m_fctIDp 缩放因子 [Pa]
        int    m_fctIODE = 0; //!<  Function identifier for the scale factor of Young's modulus,when Young's modulus is function of the plastic strain.
        double m_Eint = 0.0; //!< Saturated Young's modulus for infinitive plastic strain.
        double m_Ce = 0.0; //!< parameter for the evolution of Young's modulus 
        QList<int>  m_fctID{}; //!< Yield stress function identifiers corresponding to strain rate。size()与m_numberOfFunctions相同
        QList<double> m_fctScale{}; //!< Scale factor for ordinate(stress)in fct_ID₁. [Pa]。size()与m_numberOfFunctions相同
        QList<double> m_epsilon{}; //!< If VP=0 total strain rate for fct_ID.If VP=1 plastic strain rate for fct_ID  [1/s]。size()与m_numberOfFunctions相同
    };

    /**
     * @brief PlasTab塑性材料模型类
     * @details 此类实现了PlasTab塑性材料模型，用于描述材料的应力-应变表格行为。
     * @see FITKAbstractMaterial, FITKAbstractRadiossData
     */
    class FITKRadiossDataAPI FITKMaterialPlasTab : public FITKAbstractRadMaterial
    {
        Q_OBJECT
      
        RadiossKeyWord(FITKMaterialPlasTab,/MAT/LAW36); //!< Radioss关键字标识
        FITKCLASS(Radioss, FITKMaterialPlasTab); //!< FITK类标识

    public:     
        explicit FITKMaterialPlasTab() = default;
        virtual ~FITKMaterialPlasTab() = default;

        virtual FITKAbstractRadMaterial::RadMatType getRadMatType() {
            return FITKAbstractRadMaterial::RadMatType::LAW36;
        };
        const FITKMaterialPlasTabData& getMaterialDataReadOnly() const;
        FITKMaterialPlasTabData& getMaterialData();

    private: 
        FITKMaterialPlasTabData m_data;
    };
}

#endif
