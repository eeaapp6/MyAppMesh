/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file FITKAbaMaterialConductivity.h
 * @brief 材料导热系数属性管理类头文件
 * @details 该文件定义了用于管理材料导热系数属性的类，支持温度相关的各向同性导热系数数据管理
 * @author libaojun
 * @date 2025-04-24
 */

#ifndef __FITKABAMATERAL_CONDUCTIVITY_H_
#define __FITKABAMATERAL_CONDUCTIVITY_H_

#include "FITKAbaMaterialBehavior.h"
#include "FITKAbaMaterialConductivityData.h"
#include <QVector>

namespace Interface
{ 
    /**
     * @brief 材料导热系数属性行为类
     * @details 该类用于管理材料的导热系数属性，包括各向同性的导热系数数据
     * @author libaojun
     * @date 2025-04-24
     */
    class FITKInterfaceStructuralAPI FITKAbaMaterialConductivity : public FITKAbaMaterialBehavior
    {
    public:
        explicit FITKAbaMaterialConductivity() = default;
        virtual ~FITKAbaMaterialConductivity() = default;

        /** 
         * @brief 获取材料行为类型
         * @return 返回导热系数类型枚举值
         * @author libaojun
         * @date 2025-04-24
         */
        virtual FITKAbaMaterialBehaviorEnum::FITKAbaMaterialBehaviorType getMaterialBehaviorType() override;
        /**
         * @brief  获取属性行为类名
         * @param[in]  void
         * @return 属性行为类名
         * @author libaojun
         * @date   2025-08-08
         */
        virtual QString getBehaviorClassName() override { 
            return "Interface::FITKAbaMaterialConductivity"; }
        /** 
         * @brief 设置是否使用温度相关的数据
         * @param use true表示使用温度相关数据，false表示不使用
         * @author libaojun
         * @date 2025-04-24
         */
        void setUseTemperatureDependentData(bool use);
        
        /** 
         * @brief 获取是否使用温度相关的数据
         * @return 返回是否使用温度相关数据的状态
         * @author libaojun
         * @date 2025-04-24
         */
        bool getUseTemperatureDependentData() const;

        /** 
         * @brief 获取各向同性数据的行数
         * @return 返回数据行数
         * @author libaojun
         * @date 2025-04-24
         */
        int getIsotropicNumberOfRows() const;
       
        /** 
         * @brief 设置各向同性数据的行数
         * @param rows 要设置的行数
         * @author libaojun
         * @date 2025-04-24
         */
        void setIsotropicNumberOfRows(int rows);
        
        /** 
         * @brief 在指定位置插入一行各向同性数据
         * @param index 插入位置的索引
         * @author libaojun
         * @date 2025-04-24
         */
        void insertIsotropicRow(int index);

        /** 
         * @brief 删除指定位置的各向同性数据行
         * @param index 要删除的行索引
         * @author libaojun
         * @date 2025-04-24
         */
        void removeIsotropicRow(int index);
        
        /** 
         * @brief 获取指定位置的导热系数值
         * @param index 数据行索引，默认为0
         * @return 返回导热系数值(W/(m·K))
         * @author libaojun
         * @date 2025-04-24
         */
        double getIsotropicConductivity(int index = 0) const;
        
        /** 
         * @brief 设置指定位置的导热系数值
         * @param value 要设置的导热系数值(W/(m·K))
         * @param index 数据行索引，默认为0
         * @author libaojun
         * @date 2025-04-24
         */
        void setIsoTropicConductivity(double value, int index = 0);
        
        /** 
         * @brief 获取指定位置的温度值
         * @param index 数据行索引，默认为0
         * @return 返回温度值(K)
         * @author libaojun
         * @date 2025-04-24
         */
        double getIsotropicTemperature(int index = 0) const;
        
        /** 
         * @brief 设置指定位置的温度值
         * @param value 要设置的温度值(K)
         * @param index 数据行索引，默认为0
         * @author libaojun
         * @date 2025-04-24
         */
        void setIsotropicTemperature(double value, int index = 0);

    private:
        /** @brief 是否使用温度相关数据 */
        bool m_useTemperatureDependentData{false};
        
        /** @brief 各向同性导热系数数据数组 */
        QVector<ConductivityIsotropicData> m_isotropicDatas{ConductivityIsotropicData()};
    };
}

#endif