/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef __FITK_MATERIAL_VOID_ELASTIC__H__
#define __FITK_MATERIAL_VOID_ELASTIC__H__

#include "FITKAbstractRadMaterial.h"

namespace Radioss
{
    /**
     * @brief 空材料类
     * @details 此类用于描述Radioss中的空材料，通常用于占位或无效材料。
     * @see FITKAbstractMaterial, FITKAbstractRadiossData
     * @author libaojun
     * @date 2025-04-25
     */
    class FITKRadiossDataAPI FITKMaterialVoid : public FITKAbstractRadMaterial
    {
        FITKCLASS(Radioss, FITKMaterialVoid); //!< FITK类标识
        RadiossKeyWord(FITKMaterialVoid, /MAT/LAW0) //!< Radioss关键字标识

    public:
        explicit FITKMaterialVoid() = default;
        virtual ~FITKMaterialVoid() = default;

        virtual FITKAbstractRadMaterial::RadMatType getRadMatType() {
            return FITKAbstractRadMaterial::RadMatType::LAW0; 
        };
        /**
         * @brief 获取材料密度
         * @return 返回材料密度值
         * @author libaojun
         * @date 2025-04-25
         */
        double getDensity() const;

        /**
         * @brief 设置材料密度
         * @param density 密度值
         * @author libaojun
         * @date 2025-04-25
         */
        void setDensity(double density);

        /**
         * @brief 获取杨氏模量
         * @return 返回杨氏模量值
         * @author libaojun
         * @date 2025-04-25
         */
        double getYoungsModulus() const;

        /**
         * @brief 设置杨氏模量
         * @param modulus 杨氏模量值
         * @author libaojun
         * @date 2025-04-25
         */
        void setYoungsModulus(double modulus);

        /**
         * @brief 获取泊松比
         * @return 返回泊松比值
         * @author libaojun
         * @date 2025-04-25
         */
        double getPoissonsRatio() const;

        /**
         * @brief 设置泊松比
         * @param ratio 泊松比值
         * @author libaojun
         * @date 2025-04-25
         */
        void setPoissonsRatio(double ratio);

    private:
        double m_density = 0.0; //!< 材料密度，默认为0.0
        double m_youngsModulus = 0.0; //!< 杨氏模量
        double m_poissonsRatio = 0.0; //!< 泊松比
    };

    /**
     * @brief 弹性材料类
     * @details 此类用于描述Radioss中的弹性材料，通常用于线性弹性材料模型。
     * @see FITKAbstractMaterial, FITKAbstractRadiossData
     * @author libaojun
     * @date 2025-04-25
     */
    class FITKRadiossDataAPI FITKMaterialElastic : public FITKMaterialVoid
    {
        
        RadiossKeyWord(FITKMaterialElastic, /MAT/LAW1) //!< Radioss关键字标识

        FITKCLASS(Radioss, FITKMaterialElastic); //!< FITK类标识
    public:
        explicit FITKMaterialElastic() = default;
        virtual ~FITKMaterialElastic() = default;

        virtual FITKAbstractRadMaterial::RadMatType getRadMatType() {
            return FITKAbstractRadMaterial::RadMatType::LAW1;
        };
    };
    



}



#endif