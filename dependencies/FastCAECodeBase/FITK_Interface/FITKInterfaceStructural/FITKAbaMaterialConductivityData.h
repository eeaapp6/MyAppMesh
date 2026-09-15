/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file FITKAbaMaterialConductivityData.h
 * @brief 定义材料热导率数据类
 * @details 该文件包含了用于存储和管理材料热导率相关数据的类定义
 * @author libaojun
 * @date 2024-04-15 
 */

#ifndef __FITKABAMATERAL_CONDUCTIVITYDATA_H__
#define __FITKABAMATERAL_CONDUCTIVITYDATA_H__

namespace Interface {
    /**
     * @brief 热导率各向同性数据类
     * @details 该类用于存储材料的各向同性热导率数据，包括热导率值和对应温度
        
     */
    class ConductivityIsotropicData
    {
    public:
        /**
         * @brief 默认构造函数
         */
        explicit ConductivityIsotropicData() = default;

        /**
         * @brief 虚析构函数
         */
        virtual ~ConductivityIsotropicData() = default;
        
        /**
         * @brief 获取热导率值
         * @return double 热导率值，单位：W/(m·K)
         */
        double getConductivity() const;

        /**
         * @brief 获取温度值
         * @return double 温度值，单位：K
         */
        double getTemperature() const;

        /**
         * @brief 设置热导率值
         * @param value 热导率值，单位：W/(m·K)
         */
        void setConductivity(double value);

        /**
         * @brief 设置温度值
         * @param value 温度值，单位：K
         */
        void setTemperature(double value);

    private:
        double m_conductivity {0}; /*!< 热导率值 */
        double m_temperature {0}; /*!< 温度值*/
    };
}
#endif