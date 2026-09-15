/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file FITKNasSectionInfoPSHELL.h
 * @brief  BDF中的PShell 属性信息
 * @author libaojun (libaojunqd@foxmail.com)
 * @date 2025-02-27
 * 
 */
#ifndef _FITK_NASTRAN_SECTION_INFO_PSHELL_H___
#define _FITK_NASTRAN_SECTION_INFO_PSHELL_H___
 
#include "FITKNasAbstractSectionInfo.h"
#include "FITK_Kernel/FITKCore/FITKAbstractParam.hpp"

namespace Interface
{
    /**
     * @brief PSHELL信息
     * @author libaojun (libaojunqd@foxmail.com)
     * @date 2025-02-27
     */
    class FITKInterfaceStructuralAPI FITKNasSectionInfoPSHELL :
        public FITKNasAbstractSectionInfo
    {
        FITKCLASS(Interface, FITKNasSectionInfoPSHELL);
    public:
        /**
         * @brief Construct a new FITKNasSectionInfoPSHELL object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        explicit FITKNasSectionInfoPSHELL();
        /**
         * @brief Destroy the FITKNasSectionInfoPSHELL object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        virtual ~FITKNasSectionInfoPSHELL() = default;
        /**
         * @brief 获取类型
         * @return FITKNasAbstractSectionInfo::NasSectionType 
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        virtual FITKNasAbstractSectionInfo::NasSectionType getNasSectionType() override;
        /**
         * @brief Set the Bending Moment 
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        void setBendingMoment(const double & value);
        double getBendingMoment() const;
        /**
         * @brief Set the Transverse Shear
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        void setTransverseShear(const double & value);
        double getTransverseShear() const;
        /**
         * @brief Set the Non Structural Mass 
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        void setNonStructuralMass(const double & value);
        double getNonStructuralMass() const;
        /**
         * @brief Set the Fiber Distance Z1
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        void setFiberDistanceZ1(const double & value);
        double getFiberDistanceZ1() const;
        /**
         * @brief Set the Fiber Distance Z2
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        void setFiberDistanceZ2(const double & value);
        double getFiberDistanceZ2() const;
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
         * @brief 弯矩
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        Core::FITKAbstractParamValueDouble _bendingMoment{};
        /**
         * @brief transverseShear
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        Core::FITKAbstractParamValueDouble _transverseShear{};
        /**
         * @brief 质量
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        Core::FITKAbstractParamValueDouble _nonstructualMass{};
        /**
         * @brief Fiber Distance Z1
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        Core::FITKAbstractParamValueDouble _z1{};
        /**
         * @brief Fiber Distance Z2
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-02-27
         */
        Core::FITKAbstractParamValueDouble _z2{};

        
     
    };
}


#endif
