/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaInteractionContactTangentialBehavior.h
 * @brief  相互左右接触切向属性行为
 * @author LiJin (1003680421@qq.com)
 * @date   2024-04-15
 * 
 */
#ifndef FITKABAINTERACTIONCONTACTTANGENTIALBEHAVIOR_H
#define FITKABAINTERACTIONCONTACTTANGENTIALBEHAVIOR_H
#include "FITKAbaInteractionContactBehavior.h"
#include "FITKInterfaceStructuralAPI.h"
#include <QVector>
#include <QObject>


namespace Interface {
    
    /**
     * @brief  Penalty摩擦方程数据类，表示一行数据
     * @author LiJin (1003680421@qq.com)
     * @date   2024-04-16
     */
    class PenaltyData
    {
    public:
        /**
         * @brief  获取各向同性摩擦系数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-16
         */
        double getFrictionCoeff() const;
        /**
         * @brief  设置各向同性摩擦系数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-16
         */
        void setFrictionCoeff(double value);
        /**
         * @brief  获取各向异性摩擦系数1
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-16
         */
        double getFrictionCoeff1() const;
        /**
         * @brief  设置各向异性摩擦系数1
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-16
         */
        void setFrictionCoeff1(double value);
        /**
         * @brief  获取各向异性摩擦系数2
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-16
         */
        double getFrictionCoeff2() const;
        /**
         * @brief  设置各向异性摩擦系数2
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-16
         */
        void setFrictionCoeff2(double value);
        /**
         * @brief  获取滑移速率
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-16
         */
        double getSlipRate() const;
        /**
         * @brief  设置滑移速率
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-16
         */
        void setSlipRate(double value);
        /**
         * @brief  获取接触压力
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-16
         */
        double getContactPressure() const;
        /**
         * @brief  设置接触压力
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-16
         */
        void setContactPressure(double value);
        /**
         * @brief  获取温度
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-16
         */
        double getTemperature() const;
        /**
         * @brief  设置温度
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-16
         */
        void setTemperature(double value);
    private:
        /**
         * @brief  各向同性摩擦系数
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-16
         */
        double m_fricCoeff{0.0};
        /**
         * @brief  各向异性摩擦系数1
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-16
         */
        double m_fricCoeff1{0.0};
        /**
         * @brief  各项异性摩擦系数2
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-16
         */
        double m_fricCoeff2{0.0};
        /**
         * @brief  滑移速率
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-16
         */
        double m_slipRate{0.0};
        /**
         * @brief  接触压力
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-16
         */
        double m_contactPressure{0.0};
        /**
         * @brief  温度
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-16
         */
        double m_temperature{0.0};
    };

    class FITKInterfaceStructuralAPI FrictionDiretionalityEnum : public QObject
    {
        Q_OBJECT
    public:
        /**
         * @brief  摩擦方向枚举
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-16
         */
        enum FrictionDiretionality
        {
            Isotropic,
            Anisotropic,
        };
        Q_ENUM(FrictionDiretionality);
  
    };

    /**
     * @brief  相互作用接触切向属性行为
     * @author LiJin (1003680421@qq.com)
     * @date   2024-04-15
     */
    class FITKInterfaceStructuralAPI FITKAbaInteractionContactTangentialBehavior :
         public FITKAbaInteractionContactBehavior
    {
      

    public:
        /**
         * @brief  构造函数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        FITKAbaInteractionContactTangentialBehavior() = default;
        /**
         * @brief  析构函数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        ~FITKAbaInteractionContactTangentialBehavior() = default;
        /**
         * @brief  获取接触行为类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        FITKAbaInteractionContactBehaviorEnum::FITKAbaContactBehaviorType getBehaviorType() const override;
        /**
         * @brief  获取摩擦方程
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        FITKAbaInteractionContactBehaviorEnum::FrictionFormulation getFrictionFormulation() const;
        /**
         * @brief  设置摩擦方程
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setFrictionFormulation(FITKAbaInteractionContactBehaviorEnum::FrictionFormulation type);
        /**
         * @brief  获取摩擦方向类型，各向同性或各向异性
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-16
         */
        FrictionDiretionalityEnum::FrictionDiretionality getFrictionDirectionality() const;
        /**
         * @brief  设置摩擦方向类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-16
         */
        void setFrictionDirectionality(FrictionDiretionalityEnum::FrictionDiretionality type);
        /**
         * @brief  是否使用滑移速率相关数据
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-16
         */
        bool getUseSlipRateDependentData() const;
        /**
         * @brief  设置是否使用滑移速率相关数据
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-16
         */
        void setUseSlipRateDependentData(bool state);
        /**
         * @brief  是否使用接触压力相关数据
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-16
         */
        bool getUseContactPressureDependentData() const;
        /**
         * @brief  设置是否使用接触压力相关数据
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-16
         */
        void setUseContactPressureDependentData(bool state);
        /**
         * @brief  是否使用温度相关数据
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-16
         */
        bool getUseTemperatureDependentData() const;
        /**
         * @brief  设置是否使用温度相关数据
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-16
         */
        void setUseTemperatureDependentData(bool state);
        /**
         * @brief  获取各向同性摩擦系数
         * @param[in]  index 数据行索引
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        double getFrictionCoeff(int index = 0) const;
        /**
         * @brief  设置各向同性摩擦系数
         * @param[in]  index 数据行索引
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        void setFrictionCoeff(double value, int index = 0);
        /**
         * @brief  获取各向异性摩擦系数1
         * @param[in]  index 数据行索引
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-16
         */
        double getFrictionCoeff1(int index = 0) const;
        /**
         * @brief  设置各向异性摩擦系数1
         * @param[in]  index 数据行索引
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-16
         */
        void setFrictioinCoeff1(double value, int index = 0);
        /**
         * @brief  获取各向异性摩擦系数2
         * @param[in]  index 数据行索引
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-16
         */
        double getFrictionCoeff2(int index = 0) const;
        /**
         * @brief  设置各向异性摩擦系数2
         * @param[in]  index 数据行索引
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-16
         */
        void setFrictionCoeff2(double value, int index = 0);
        /**
         * @brief  获取滑移速率
         * @param[in]  index 数据行索引
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-16
         */
        double getSlipRate(int index = 0) const;
        /**
         * @brief  设置滑移速率
         * @param[in]  index 数据行索引
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-16
         */
        void setSlipRate(double value, int index = 0);
        /**
         * @brief  获取接触压力
         * @param[in]  index 数据行索引
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-16
         */
        double getContactPressure(int index = 0) const;
        /**
         * @brief  设置接触压力
         * @param[in]  index 数据行索引
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-16
         */
        void setContactPressure(double value, int index = 0);
        /**
         * @brief  获取温度
         * @param[in]  index 数据行索引
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-16
         */
        double getTemperature(int index = 0) const;
        /**
         * @brief  设置温度
         * @param[in]  index 数据行索引
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-16
         */
        void setTemperature(double value,int index = 0);
        /**
         * @brief  获取数据行数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-16
         */
        int getRowCount() const;
        /**
         * @brief  设置数据行数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-16
         */
        void setRowCount(int row);
        /**
         * @brief  移除数据行
         * @param[in]  index要移除的数据行索引
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-16
         */
        void removeRow(int index);
        /**
         * @brief  插入数据行
         * @param[in]  index要插入的数据行索引
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-16
         */
        void insetRow(int index);
    private:
        /**
         * @brief  摩擦方程
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        FITKAbaInteractionContactBehaviorEnum::FrictionFormulation m_formulation{ FITKAbaInteractionContactBehaviorEnum::FrictionFormulation::Frictionless};
        /**
         * @brief  摩擦方向
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-16
         */
        FrictionDiretionalityEnum::FrictionDiretionality m_directionality{ FrictionDiretionalityEnum::FrictionDiretionality::Isotropic};
        /**
         * @brief  是否使用滑移速率相关数据
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-16
         */
        bool m_useSlipRateDependent{false};
        /**
         * @brief  是否使用接触压力相关数据
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-16
         */
        bool m_useContactPressureDependent{false};
        /**
         * @brief  是否使用温度相关数据
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-16
         */
        bool m_useTemperatureDependent{false};
        /**
         * @brief  多行数据
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-16
         */
        QVector<PenaltyData> m_PenaltyDatas{PenaltyData(),};
    };
}


#endif // FITKABAINTERACTIONCONTACTTANGENTIALBEHAVIOR_H
