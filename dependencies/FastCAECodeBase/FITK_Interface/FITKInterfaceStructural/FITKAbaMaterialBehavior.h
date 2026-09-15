/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaMaterialBehavior.h
 * @brief  结构模板材料属性行为抽象类
 * @author LiJin (1003680421@qq.com)
 * @date   2024-03-20
 * 
 */
#ifndef FITKABAMATERIALBEHAVIOR_H
#define FITKABAMATERIALBEHAVIOR_H

#include "FITKInterfaceStructuralAPI.h"
#include <QObject>


namespace Interface 
{
    class FITKInterfaceStructuralAPI FITKAbaMaterialBehaviorEnum :
        public QObject
    {
        Q_OBJECT
    public:
        /**
        * @brief  材料属性行为枚举类。包括密度、弹性、塑性等
        * @author LiJin (1003680421@qq.com)
        * @date   2024-03-20
        */
        enum FITKAbaMaterialBehaviorType
        {
            None,
            Density,
            Elastic,
            Plastic,
            Expansion,
            Hyperelastic,
            Damping,
            JohnsonCookDamage,
            Conductivity,
            SpecificHeat,

            NasMAT1,
        };
        Q_ENUM(FITKAbaMaterialBehaviorType);
        /**
        * @brief  材料弹性属性类型枚举类。包括各向同性、各向异性等
        * @author LiJin (1003680421@qq.com)
        * @date   2024-03-20
        */
        enum FITKAbaMaterialElasticType
        {
            IsoTropic,
            EngineeringConstants,
            Lamina,
            Orthotropics,
            AnIsotopic,
            Traction,
            CoupledTraction,
            Shear,
        };
        Q_ENUM(FITKAbaMaterialElasticType);
        /**
         * @brief  热膨胀属性类型枚举类
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        enum FITKAbaMaterialExpansionType
        {
            Isotropic,
            Orthotropic,
            Anisotropic,
        };
        Q_ENUM(FITKAbaMaterialExpansionType);

        /**
         * @brief  超弹性行为材料类型，包括各向同性、各向异性
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        enum HyperelasticMaterialType
        {
            HyereIsotropic,
            HyereAnisotropic
        };
        Q_ENUM(HyperelasticMaterialType);
        /**
         * @brief  超弹性各向同性应变势能枚举类
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        enum HyperelasticIsotropicStrainEnergyPotential
        {
            Unknown,
            ArrudaBoyce,
            Marlow,
            MooneyRivlin,
            NeoHooke,
            Ogden,
            Polynomial,
            ReducePolynomial,
            UsrDefined,
            VanderWaals,
            Yeoh,
        };
        Q_ENUM(HyperelasticIsotropicStrainEnergyPotential);
        /**
         * @brief  超弹性各向异性应变势能枚举类
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        enum HyperelasticAnisotropicStrainEnergyPotential
        {
            FungAnisotropic,
            FungOrthotropic,
            Holzapfel,
            User
        };
        Q_ENUM(HyperelasticAnisotropicStrainEnergyPotential);
        /**
         * @brief  超弹性各向同性输入源
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        enum HyperelasticIsotropicInputSource
        {
            TestData,
            Coeffcients,
        };
        Q_ENUM(HyperelasticIsotropicInputSource);
        /**
         * @brief  各向同性模量时间尺度
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        enum HyperelasticIsoModuliTimeScale
        {
            LongTerm,
            Instantaneous
        };
        Q_ENUM(HyperelasticIsoModuliTimeScale);
        /**
         * @brief  塑性属性类型枚举类。包括各向同性、jhnsonCook、各向异性等
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        enum FITKAbaMaterialPlasticType
        {
            PlasticIsoTropic,
            PlasticKinematic,
            PlasticMultiLinearKinematic,
            PlasticJohnsonCook,
        };
        Q_ENUM(FITKAbaMaterialPlasticType);
         
        /**
         * @brief 比热容数据类型
         */
        enum SpecificHeatDataType
        {
            ConstantVolume,   // 定容比热容
            ConstantPressure, // 定压比热容
        }; 
        Q_ENUM(SpecificHeatDataType);
    };

   
    /**
     * @brief  材料属性行为抽象类
     * @author LiJin (1003680421@qq.com)
     * @date   2024-03-20
     */
    class FITKInterfaceStructuralAPI FITKAbaMaterialBehavior  
    {
    public:
        /**
         * @brief  构造函数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        FITKAbaMaterialBehavior() = default;
        /**
         * @brief  析构函数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        virtual ~FITKAbaMaterialBehavior() = default;
        /**
         * @brief  获取属性行为类型，便于对对象的动态类型转换
         * @param[in]  void
         * @return 属性行为枚举类型
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        virtual FITKAbaMaterialBehaviorEnum::FITKAbaMaterialBehaviorType getMaterialBehaviorType() = 0;
        /**
         * @brief  获取属性行为类名
         * @param[in]  void
         * @return 属性行为类名
         * @author libaojun
         * @date   2025-08-08
         */
        virtual QString getBehaviorClassName() = 0;
   };
}


#endif // FITKABAMATERIALBEHAVIOR_H
