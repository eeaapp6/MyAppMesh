/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaMaterialHyperElastic.h
 * @brief  材料超弹性行为
 * @author LiJin (1003680421@qq.com)
 * @date   2024-03-25
 * 
 */
#ifndef FITKABAMATERIALHYPERELASTIC_H
#define FITKABAMATERIALHYPERELASTIC_H
#include "FITKInterfaceStructuralAPI.h"
#include "FITKAbaMaterialBehavior.h"
#include "FITKAbaMaterialHyperelasticData.h"
#include <QVector>


namespace Interface {

    /**
     * @brief  材料超弹性行为类
     * @author LiJin (1003680421@qq.com)
     * @date   2024-03-25
     */
    class FITKInterfaceStructuralAPI FITKAbaMaterialHyperElastic: public FITKAbaMaterialBehavior
    {
    public:
        /**
         * @brief  获取材料属性行为类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-01
         */
        FITKAbaMaterialBehaviorEnum::FITKAbaMaterialBehaviorType getMaterialBehaviorType() override;
        /**
         * @brief  获取材料类型，各向同性或各向异性
         * @param[in]  void
         * @return HyperelasticMaterialType枚举类型值
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        FITKAbaMaterialBehaviorEnum::HyperelasticMaterialType getMaterialType() const;
        /**
         * @brief  获取属性行为类名
         * @param[in]  void
         * @return 属性行为类名
         * @author libaojun
         * @date   2025-08-08
         */
        virtual QString getBehaviorClassName() override {
            return "Interface::FITKAbaMaterialHyperElastic";
        }
        /**
         * @brief  设置材料类型
         * @param[in]  type 材料类型
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        void setMaterialType(FITKAbaMaterialBehaviorEnum::HyperelasticMaterialType type);
        /**
         * @brief  获取参数是否随温度变化
         * @param[in]  void
         * @return bool值
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        bool getUseTemperatureDependentData() const;
        /**
         * @brief  设置参数是否随温度变化
         * @param[in]  stat bool值
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        void setUseTemperatureDependentData(bool stat);
        /**
         * @brief  获取各向同性应变势能类型
         * @param[in]  void
         * @return 各向同性应变势能枚举类型值
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        FITKAbaMaterialBehaviorEnum::HyperelasticIsotropicStrainEnergyPotential getIsoStrainEnergyPotential() const;
        /**
         * @brief  设置各向同性应变势能类型
         * @param[in]  type 各向同性应变势能类型
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        void setIsoStrainEnergyPotential(FITKAbaMaterialBehaviorEnum::HyperelasticIsotropicStrainEnergyPotential type);
        /**
         * @brief  获取各向同性参数输入源
         * @param[in]  void
         * @return 各向同性参数输入源类型
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        FITKAbaMaterialBehaviorEnum::HyperelasticIsotropicInputSource getIsoInputSource() const;
        /**
         * @brief  设置各向同性参数输入源类型
         * @param[in]  type 输入源类型
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        void setIsoInputSource(FITKAbaMaterialBehaviorEnum::HyperelasticIsotropicInputSource type);
        /**
         * @brief  获取各向同性Moduli Time Scale
         * @param[in]  void
         * @return 各向同性Moduli Time Scale
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        FITKAbaMaterialBehaviorEnum::HyperelasticIsoModuliTimeScale getIsoModuliTimeScale() const;
        /**
         * @brief  设置各向同性Moduli Time Scale
         * @param[in]  Moduli Time Scale类型
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        void setIsoModuliTimeScale(FITKAbaMaterialBehaviorEnum::HyperelasticIsoModuliTimeScale type);
        /**
         * @brief  获取各向同性MooneyRivlin数据行数
         * @param[in]  void
         * @return 数据行数
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        int getIsoMooneyRivlinDataRows() const;
        /**
         * @brief  设置各向同性MooneyRivlin数据行数
         * @param[in]  rows 数据行数
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        void setIsoMooneyRivlinDataRows(int rows);
        /**
         * @brief  插入各向同性MooneyRivlin数据行
         * @param[in]  index 要插入的数据行索引
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        void insertIsoMooneyRivlinDataRow(int index);
        /**
         * @brief  移除各向同性MooneyRivlin数据行
         * @param[in]  index 要移除的数据行索引
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        void removeIsoMooneyRivlinDataRow(int index);
        /**
         * @brief  获取各向同性MooneyRivlin参数C10
         * @param[in]  index 数据行索引
         * @return 参数值
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        double getIsoMooneyRivlinC10(int index = 0) const;
        /**
         * @brief  获取各向同性MooneyRivlin参数C01
         * @param[in]  index 数据行索引
         * @return 参数值
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        double getIsoMooneyRivlinC01(int index = 0) const;
        /**
         * @brief  获取各向同性MoonneyRivlin参数D1
         * @param[in]  index 数据行索引
         * @return 参数值
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        double getIsoMooneyRivlinD1(int index = 0) const;
        /**
         * @brief  获取各向同性MoonneyRivlin参数温度
         * @param[in]  index 数据行索引
         * @return 参数值
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        double getIsoMooneyRivlinTemperature(int index = 0) const;
        /**
         * @brief  设置各向同性MoonneyRivlin参数C10
         * @param[in]  value 参数值
         * @param[in]  index 数据行索引
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        void setIsoMooneyRivlinC10(double value, int index = 0);
        /**
         * @brief  设置各向同性MoonneyRivlin参数C01
         * @param[in]  value 参数值
         * @param[in]  index 数据行索引
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        void setIsoMooneyRivlinC01(double value, int index = 0);
        /**
         * @brief  设置各向同性MoonneyRivlin参数D1
         * @param[in]  value 参数值
         * @param[in]  index 数据行索引
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        void setIsoMooneyRivlinD1(double value, int index = 0);
        /**
         * @brief  设置各向同性MoonneyRivlin参数温度
         * @param[in]  value 参数值
         * @param[in]  index 数据行索引
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-25
         */
        void setIosMooneyRivlinTemperature(double value, int index = 0);

    private:
        /**
         * @brief  超弹性行为类型，各向同性或各向异性
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-01
         */
        FITKAbaMaterialBehaviorEnum::HyperelasticMaterialType m_materialType{ FITKAbaMaterialBehaviorEnum::HyperelasticMaterialType::HyereIsotropic};
        /**
         * @brief  是否使用温度相关的数据
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-01
         */
        bool m_useTemperatureDependentData{false};
        /**
         * @brief  超弹性各向同性应变能类型
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-01
         */
        FITKAbaMaterialBehaviorEnum::HyperelasticIsotropicStrainEnergyPotential m_isoStrainEnergyPotential{ FITKAbaMaterialBehaviorEnum::HyperelasticIsotropicStrainEnergyPotential::Unknown};
        /**
         * @brief  超弹性各向同性输入类型，测试数据或参数
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-01
         */
        FITKAbaMaterialBehaviorEnum::HyperelasticIsotropicInputSource m_isoInputSource{ FITKAbaMaterialBehaviorEnum::HyperelasticIsotropicInputSource::TestData};
        /**
         * @brief  各向同性模量时间尺度
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-01
         */
        FITKAbaMaterialBehaviorEnum::HyperelasticIsoModuliTimeScale m_isoModuliTimeScale{ FITKAbaMaterialBehaviorEnum::HyperelasticIsoModuliTimeScale::LongTerm};
        /**
         * @brief  各向同性MooneyRivilin参数数组，当InputSource为Coeffcients时，用于定义参数。当使用温度相关参数时，
         * 数据为多行。
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-01
         */
        QVector<MooneyRivlinData> m_isoMooneyRivlinDatas{MooneyRivlinData(),};
    };
}


#endif // FITKABAMATERIALHYPERELASTIC_H
