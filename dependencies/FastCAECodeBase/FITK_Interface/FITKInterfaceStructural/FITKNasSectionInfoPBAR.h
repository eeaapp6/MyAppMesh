/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file FITKNasSectionInfoPBAR.h
 * @brief  BDF中定义的PBAR信息
 * @author libaojun (libaojunqd@foxmail.com)
 * @date 2025-02-27
 * 
 */
#ifndef _FITK_NASTRAN_SECTION_INFO_PBAR_H___
#define _FITK_NASTRAN_SECTION_INFO_PBAR_H___

#include <QString>
#include "FITKNasAbstractSectionInfo.h"
#include "FITK_Kernel/FITKCore/FITKAbstractParam.hpp"

namespace Interface
{
    /**
     * @brief BDF中定义的PBAR信息
     * @author libaojun (libaojunqd@foxmail.com)
     * @date 2025-02-27
     */
    class FITKInterfaceStructuralAPI FITKNasSectionInfoPBAR :
        public FITKNasAbstractSectionInfo
    {
        FITKCLASS(Interface, FITKNasSectionInfoPBAR);
    public:
        /**
         * @brief Construct a new FITKNasSectionInfoPBAR object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        explicit FITKNasSectionInfoPBAR() = default;
        /**
         * @brief Destroy the FITKNasSectionInfoPBAR object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        virtual ~FITKNasSectionInfoPBAR() = default;
        /**
         * @brief Get the Nas Section Type object
         * @return FITKNasAbstractSectionInfo::NasSectionType 
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        virtual FITKNasAbstractSectionInfo::NasSectionType getNasSectionType() override;
        /**
         * @brief Set the Area 
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        void setArea(const double & value);
        double getArea()const;
        /**
         * @brief Set the Inertia Area Moments I1
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        void setInertiaAreaMomentsI1(const double & value);
        double getInertiaAreaMomentsI1() const;
        /**
         * @brief Set the Inertia Area Moments I2
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        void setInertiaAreaMomentsI2(const double & value);
        double getInertiaAreaMomentsI2() const;
        /**
         * @brief Set the Inertia Area Moments I12
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        void setInertiaAreaMomentsI12(const double & value);
        double getInertiaAreaMomentsI12() const;
        /**
         * @brief Set the Torsional Constant
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        void setTorsionalConstant(const double & value);
        double getTorsionalConstant() const;
        /**
         * @brief Set the Nonstructural Mass
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        void setNonstructuralMass(const double & value);
        double getNonstructuralMass() const;
        /**
         * @brief Set the Stress Recovery Coeff C1
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        void setStressRecoveryCoeffC1(const double & value);
        double getStressRecoveryCoeffC1() const;
        /**
         * @brief Set the Stress Recovery Coeff C2
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        void setStressRecoveryCoeffC2(const double & value);
        double getStressRecoveryCoeffC2() const;
        /**
         * @brief Set the Stress Recovery Coeff D1
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        void setStressRecoveryCoeffD1(const double & value);
        double getStressRecoveryCoeffD1() const;
        /**
         * @brief Set the Stress Recovery Coeff D2
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        void setStressRecoveryCoeffD2(const double & value);
        double getStressRecoveryCoeffD2() const;
        /**
         * @brief Set the Stress Recovery Coeff E1
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        void setStressRecoveryCoeffE1(const double & value);
        double getStressRecoveryCoeffE1() const;
        /**
         * @brief Set the Stress Recovery Coeff E2
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        void setStressRecoveryCoeffE2(const double & value);
        double getStressRecoveryCoeffE2() const;
        /**
         * @brief Set the Stress Recovery Coeff F1
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        void setStressRecoveryCoeffF1(const double & value);
        double getStressRecoveryCoeffF1() const;
         /**
         * @brief Set the Stress Recovery Coeff F2
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        void setStressRecoveryCoeffF2(const double & value);
        double getStressRecoveryCoeffF2() const;
        /**
         * @brief Set the Area Shear Factor K1
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        void setAreaShearFactorK1(const double & value);
        double getAreaShearFactorK1() const;
        /**
         * @brief Set the Area Shear Factor K2
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        void setAreaShearFactorK2(const double & value);
        double getAreaShearFactorK2() const;
        /**
         * @brief 获取参数
         * @param[in]  param 参数名称 上面函数中set后面的字符串
         * @param[out] res   是否存在
         * @return 参数值
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */ 
        const Core::FITKAbstractParamValueDouble& getParam(const QString & param, bool& res);

    private:
        /**
         * @brief 面积
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        Core::FITKAbstractParamValueDouble _area{};
        /**
         * @brief 惯性矩
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        Core::FITKAbstractParamValueDouble _inertiaAreaMomentsI1{};
        Core::FITKAbstractParamValueDouble _inertiaAreaMomentsI2{};
        Core::FITKAbstractParamValueDouble _inertiaAreaMomentsI12{};
        /**
         * @brief torsional Constant
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        Core::FITKAbstractParamValueDouble _torsionalConstant{};
        /**
         * @brief 单位面积质量
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        Core::FITKAbstractParamValueDouble _nonstructuralMass{};
        /**
         * @brief stressRecoveryCoeff
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        Core::FITKAbstractParamValueDouble _stressRecoveryCoeff[8];
        /**
         * @brief areaShearFactor
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        Core::FITKAbstractParamValueDouble _areaShearFactor[2];


    };
}


#endif
