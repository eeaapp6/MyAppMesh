/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef __FITK_MATERIAL_PLAS_JOHNS_H__
#define __FITK_MATERIAL_PLAS_JOHNS_H__

#include "FITKAbstractRadMaterial.h"

namespace Radioss
{
    struct FITKMaterialPlasJohnsData
     {
             
        double m_density = 0.0; //!< 材料密度             [Kg/m3]
        double m_youngsModulus = 0.0; //!< 杨氏模量       [Pa]
        double m_poissonsRatio = 0.0; //!< 泊松比         
        double m_yieldStress = 0.0; //!< 屈服应力(a)       [Pa] 
        double m_plasticHardening = 0.0; //!< 塑性硬化参数(b)  [Pa]
        double m_plasticHardeningExponent = 1.0; //!< 塑性硬化指数(c)

        double m_failureStrain = 1e30; //!< 断裂应变
        double m_maxStress = 1e30; //!< 最大应力           [Pa]
//        double m_yeldStressSigmaY = 0.0; //!< 屈服应力(Sigma y) [Pa]，注释掉后使用m_yieldStress
        double m_UTS = 0.0; //!< 极限拉伸应力(UTS)  [Pa]
        double m_EpsilonUTS = 1.0; //!< 极限拉伸应力对应的工程应变
        double m_c = 0.0 ; //!< 应变率系数(c)
        double m_Epsilon0 = 0.0; //!< 参考应变率(ε0)  [1/s]
        int    m_ICC = 1; //!< 应变率计算标记   
        int    m_Fsmooth = 1; //!< 应变率平滑标记
        double m_Fcut = 1e30; //!< 应变率平滑截止频率  [Hz]
        double m_Chard = 0; //!< 硬化系数(未施加载荷)  
        double m_tempExponent = 0.0; //!< 温度指数(m)
        double m_meltingTemperature = 1e30; //!< 熔化温度(Tmelt)  [K]
        double m_rhoCp = 0.0; //!< 比热容(ρCp)           [J/m3.K]
        double m_Tref = 0.0; //!< 参考温度(Tref)        [K]
     };


    /**
     * @brief Johnson-Cook塑性材料模型类
     * @details 此类实现了Johnson-Cook塑性材料模型，用于描述材料在大应变、高应变率和
     *          高温下的塑性变形行为。模型考虑了应变强化、应变率效应和温度软化效应。
     * @see FITKAbstractMaterial, FITKAbstractRadiossData
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2025-04-23
     */
    class FITKRadiossDataAPI FITKMaterialPlasJohns : public FITKAbstractRadMaterial
    {
        Q_OBJECT
        RadiossKeyWord(FITKMaterialPlasJohns,/MAT/LAW2); //!< Radioss关键字标识
        FITKCLASS(Radioss, FITKMaterialPlasJohns); //!< FITK类标识
         
    public:
        enum MaterialPlasJohnsType
        {
            MATERIAL_PLAS_JOHNS_TYPE_CLASSIC = 0, 
            MATERIAL_PLAS_JOHNS_TYPE_SIMPLE = 1,  
        };Q_ENUM(MaterialPlasJohnsType)
        

    public:
        /**
         * @brief 构造函数
         * @details 初始化Johnson-Cook材料模型对象
         */
        explicit FITKMaterialPlasJohns() = default;

        /**
         * @brief 虚析构函数
         * @details 确保正确释放派生类对象
         */
        virtual ~FITKMaterialPlasJohns() = default; 

        virtual FITKAbstractRadMaterial::RadMatType getRadMatType() {
            return FITKAbstractRadMaterial::RadMatType::LAW2;
        };
        /**
         * @brief 设置材料类型
         * @param type Johnson-Cook材料模型类型 
         */
        void setType(MaterialPlasJohnsType type);

        /**
         * @brief 获取材料类型
         * @return MaterialPlasJohnsType 返回当前设置的材料类型
         */
        MaterialPlasJohnsType getType() const;

        /**
         * @brief 获取材料数据
         * @return FITKMaterialPlasJohnsData 返回当前材料的参数数据
         */
        const FITKMaterialPlasJohnsData & getMaterialDataReadOnly() const;
          /**
         * @brief 获取材料数据
         * @return FITKMaterialPlasJohnsData 返回当前材料的参数数据
         */
        FITKMaterialPlasJohnsData & getMaterialData();

    private:
            MaterialPlasJohnsType m_type = MATERIAL_PLAS_JOHNS_TYPE_CLASSIC; //!< 材料类型 Iflag
            FITKMaterialPlasJohnsData m_data; //!< 材料数据

    };

     
}

#endif