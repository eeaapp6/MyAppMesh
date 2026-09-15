/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaMaterialExpansion.h
 * @brief  热膨胀属性行为
 * @author LiJin (1003680421@qq.com)
 * @date   2024-03-20
 * 
 */
#ifndef FITKABAMATERIALEXPANSION_H
#define FITKABAMATERIALEXPANSION_H
#include "FITKInterfaceStructuralAPI.h"
#include "FITKAbaMaterialBehavior.h"
#include "FITKAbaMaterialExpansionData.h"
#include <QVector>


namespace Interface {

    /**
     * @brief  材料热膨胀属性行为类
     * @author LiJin (1003680421@qq.com)
     * @date   2024-03-20
     */
    class FITKInterfaceStructuralAPI FITKAbaMaterialExpansion: public FITKAbaMaterialBehavior
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
         * @brief  获取属性行为类名
         * @param[in]  void
         * @return 属性行为类名
         * @author libaojun
         * @date   2025-08-08
         */
        virtual QString getBehaviorClassName() override {
            return "Interface::FITKAbaMaterialExpansion";
        }
        /**
         * @brief  获取热膨胀属性类型。如各向同性、正交各向异性、各向异性等
         * @param[in]  void
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        FITKAbaMaterialBehaviorEnum::FITKAbaMaterialExpansionType getExpansionType() const;
        /**
         * @brief  设置热膨胀属性类型
         * @param[in] type 热膨胀属性类型枚举类型值 
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        void setExpansionType(FITKAbaMaterialBehaviorEnum::FITKAbaMaterialExpansionType type);
        /**
         * @brief  获取参考温度
         * @param[in]  void
         * @return double类型温度值
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        double getReferenceTemperature() const;
        /**
         * @brief  设置参考温度
         * @param[in] value 温度值
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        void setReferenceTemperature(double value);
        /**
         * @brief  获取属性是否随温度变化
         * @param[in]  void
         * @return bool值
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        bool getUseTemperatureDependentData() const;
        /**
         * @brief  设置属性是否随温度变化。若为false，数据行数将自动置为1
         * @param[in] stat bool值  
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        void setUseTemperatureDependentData(bool stat);
        /**
         * @brief  获取各向同性数据行数
         * @param[in]  void
         * @return int型行数
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        int getIsotropicNumberOfRows() const;
        /**
         * @brief  设置各向同性数据行数
         * @param[in] rows 数据行数
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        void setIsotropicNumberOfRows(int rows);
        /**
         * @brief  插入各向同性数据行
         * @param[in] index 要插入的数据行索引 
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        void insertIsotropicRow(int index);
        /**
         * @brief  移除各向同性数据行
         * @param[in] index 要移除的各向同性数据行索引
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        void removeIsotropicRow(int index);
        /**
         * @brief  获取各向同性膨胀系数
         * @param[in] index 数据行索引
         * @return double型数值
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        double getIsotropicExpansionCoeff(int index = 0) const;
        /**
         * @brief  设置各向同性膨胀系数
         * @param[in] value 膨胀系数
         * @param[in] index 数据行索引
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        void setIsoTropicExpansionCoeff(double value, int index = 0);
        /**
         * @brief  获取各向同性温度值
         * @param[in] index 数据行索引 
         * @return double型温度值
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        double getIsotropicTemperature(int index = 0) const;
        /**
         * @brief  设置各向同性温度值
         * @param[in] value 温度值
         * @param[in] index 数据行索引
         * @return void
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        void setIsotropicTemperature(double value, int index = 0);
    private:
        /**
         * @brief  热膨胀行为类型，如各向同性、各向异性等
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-01
         */
        FITKAbaMaterialBehaviorEnum::FITKAbaMaterialExpansionType m_expansionType{ FITKAbaMaterialBehaviorEnum::FITKAbaMaterialExpansionType::Isotropic};
        /**
         * @brief  参考温度
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-01
         */
        double m_refTemperature{0.0};
        /**
         * @brief  是否使用温度相关的数据
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-01
         */
        bool m_useTemperatureDependentData{false};
        /**
         * @brief  各向同性数据数组。数组长度对应数据行数
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        QVector<ExpansionIsotropicData> m_isotropicDatas{ExpansionIsotropicData(),};
        /**
         * @brief  正交各向异性数据数组。数组长度对应数据行数。
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        QVector<ExpansionOrthotropicData> m_orthotropicDatas{ExpansionOrthotropicData(),};
        /**
         * @brief  各向异性数据数组。数组长度对应数据行数。
         * @author LiJin (1003680421@qq.com)
         * @date   2024-03-20
         */
        QVector<ExpansionAnisotropicData> m_anisotropicDatas{ExpansionAnisotropicData(),};
    };
}


#endif // FITKABAMATERIALEXPANSION_H
