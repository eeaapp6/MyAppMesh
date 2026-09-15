/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file FITKAbaMaterialSpecificHeatData.h
 * @brief 材料比热容数据管理类头文件
 * @details 该文件定义了用于管理材料比热容数据的类，包括温度相关的比热容属性设置与获取
 * @author libaojun
 * @date 2025-04-24
 
 * @todo 待添加其他热学相关属性
 * @note 该类主要用于材料热学性质的数据管理
 */

#ifndef __FITKABAMATERAL_SPECIFICHEAT_DATA_H__
#define __FITKABAMATERAL_SPECIFICHEAT_DATA_H__

namespace Interface
{
    /**
     * @brief 各向同性材料比热容数据类
     * @details 用于存储和管理材料在不同温度下的比热容数据
     * @author libaojun
     * @date 2025-04-24
     */
    class SpecificHeatIsotropicData 
    {
        public:
            /** 
             * @brief 默认构造函数
             * @author libaojun
             * @date 2025-04-24 
             */
            SpecificHeatIsotropicData() = default;
            
            /** 
             * @brief 虚析构函数 
             * @author libaojun
             * @date 2025-04-24
             */
            virtual ~SpecificHeatIsotropicData() = default;

            /** 
             * @brief 获取比热容值
             * @return 返回比热容值
             * @author libaojun
             * @date 2025-04-24
             */
            double getSpecificHeat() const;

            /** 
             * @brief 获取温度值
             * @return 返回温度值(K)
             * @author libaojun
             * @date 2025-04-24
             */
            double getTemperature() const;

            /** 
             * @brief 设置比热容值
             * @param value 比热容值(J/(kg·K))
             * @author libaojun
             * @date 2025-04-24
             */
            void setSpecificHeat(double value);

            /** 
             * @brief 设置温度值
             * @param value 温度值
             * @author libaojun
             * @date 2025-04-24
             */
            void setTemperature(double value);    

        private:
            /**
             * @brief  比热容值
             * @details 单位为J/(kg·K)，表示单位质量的物质在单位温度变化下所吸收或释放的热量
             * @author libaojun
             * @date 2025-04-24
             */
            double m_specificHeat{0}; 
            /**
             * @brief  温度值
             * @details 单位为K，表示材料在该温度下的比热容值
             */
            double m_temperature{0};
    };
}

#endif