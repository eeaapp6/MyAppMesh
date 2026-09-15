/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file FITKAbaMaterialSpecificHeat.h
 * @brief 材料比热容属性管理类头文件
 * @details 该文件定义了用于管理材料比热容属性的类，支持温度相关的各向同性比热容数据管理
 * @author libaojun
 * @date 2025-04-24
 */

#ifndef ___FITKABAMATERAL_SPECIFICHEAT__H__
#define ___FITKABAMATERAL_SPECIFICHEAT__H__

#include "FITKAbaMaterialBehavior.h"
#include "FITKAbaMaterialSpecificHeatData.h"
#include <QVector>

namespace Interface
{
    /**
     * @brief 材料比热容属性行为类
     * @details 该类用于管理材料的比热容属性
     * @author libaojun
     * @date 2025-04-24
     */
    class FITKInterfaceStructuralAPI FITKAbaMaterialSpecificHeat : public FITKAbaMaterialBehavior
    {
        
       

    public:
        explicit FITKAbaMaterialSpecificHeat() = default;
        virtual ~FITKAbaMaterialSpecificHeat() = default;   

        /** 
         * @brief 获取材料行为类型
         * @return 返回比热容类型枚举值
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
            return "Interface::FITKAbaMaterialSpecificHeat";
        }

        /**
         * @brief 获取各向同性比热容数据类型
         * @return 返回各向同性比热容数据类型
         * @author libaojun
         * @date 2025-04-24
         */
        FITKAbaMaterialBehaviorEnum::SpecificHeatDataType getSpecificHeatDataType() const;
        /**
         * @brief 设置各向同性比热容数据类型
         * @param type 要设置的各向同性比热容数据类型
         * @author libaojun
         * @date 2025-04-24
         */
        void setSpecificHeatDataType(FITKAbaMaterialBehaviorEnum::SpecificHeatDataType type);

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
         * @brief 获取数据的行数
         * @return 返回数据行数
         * @author libaojun
         * @date 2025-04-24
         */
        int getNumberOfDataRows() const;

        /** 
         * @brief 设置各向同性数据的行数
         * @param rows 要设置的行数
         * @author libaojun
         * @date 2025-04-24
         */
        void setNumberOfDataRows(int rows);

        /** 
         * @brief 在指定位置插入一行各向同性数据
         * @param index 插入位置的索引
         * @author libaojun
         * @date 2025-04-24
         */
        void insertDataRow(int index);

        /** 
         * @brief 删除指定位置的各向同性数据行
         * @param index 要删除的行索引
         * @author libaojun
         * @date 2025-04-24
         */
        void removeDataRow(int index);

        /** 
         * @brief 获取指定位置的比热容值
         * @param index 数据行索引，默认为0
         * @return 返回比热容值(J/(kg·K))
         * @author libaojun
         * @date 2025-04-24
         */
        double getSpecificHeat(int index = 0) const;

        /** 
         * @brief 设置指定位置的比热容值
         * @param value 要设置的比热容值(J/(kg·K))
         * @param index 数据行索引，默认为0
         * @author libaojun
         * @date 2025-04-24
         */
        void setSpecificHeat(double value, int index = 0);

        /** 
         * @brief 获取指定位置的温度值
         * @param index 数据行索引，默认为0
         * @return 返回温度值(K)
         * @author libaojun
         * @date 2025-04-24
         */
        double getTemperature(int index = 0) const;

        /** 
         * @brief 设置指定位置的温度值
         * @param value 要设置的温度值(K)
         * @param index 数据行索引，默认为0
         * @author libaojun
         * @date 2025-04-24
         */
        void setTemperature(double value, int index = 0);
    private:
        /** @brief 各向同性比热容数据类型 */
        FITKAbaMaterialBehaviorEnum:: SpecificHeatDataType m_specificHeatDataType{FITKAbaMaterialBehaviorEnum::
            SpecificHeatDataType::ConstantVolume};
        /** @brief 是否使用温度相关数据 */
        bool m_useTemperatureDependentData{false};
        /** @brief 各向同性比热容数据数组 */
        QVector<SpecificHeatIsotropicData> m_isotropicDatas{SpecificHeatIsotropicData()};

    };

}

#endif