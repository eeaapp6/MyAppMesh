/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file FITKBoundaryConditionTemperature.h
 * @brief 结构分析的温度边界条件定义
 * @author libaojun
 * @date 2025年9月1日
 */

#ifndef __FITKBOUNDARY_CONDITION_TEMPERATURE_H__
#define __FITKBOUNDARY_CONDITION_TEMPERATURE_H__

#include "FITKInterfaceStructuralAPI.h"
#include "FITKAbaAbstractBoundaryCondition.h"
#include <QMap>

/**
 * @namespace Interface
 * @brief 包含结构分析接口类的命名空间
 */
namespace Interface
{
    /**
     * @class BCTemperatureData
     * @brief 用于存储温度边界条件数据的类
     * 
     * 该类存储边界条件的温度和幅值信息
     */
    class BCTemperatureData
    {
    public:
        /** @brief 默认构造函数 */
        explicit BCTemperatureData() = default;
        
        /** @brief 虚拟析构函数 */
        virtual ~BCTemperatureData() = default;

        /**
         * @brief 设置温度值
         * @param temperature 要设置的温度值
         */
        void setTemperature(double temperature);
        
        /**
         * @brief 获取当前温度值
         * @return 当前温度值
         */
        double getTemperature() const;

        /**
         * @brief 获取幅值ID
         * @return 当前幅值ID
         */
        int getAmplitudeId() const;
        
        /**
         * @brief 设置幅值ID
         * @param id 要设置的幅值ID
         */
        void setAmplitudeId(int id);

        /**
         * @brief 启用或禁用指定方向的位移约束
         * @param dir 方向索引 (0=X, 1=Y, 2=Z)
         * @param en 是否启用约束
         */
        void enableDirectionU(int dir, bool en);
        
        /**
         * @brief 检查指定方向的位移约束是否启用
         * @param dir 方向索引 (0=X, 1=Y, 2=Z)
         * @return 如果启用则返回true，否则返回false
         */
        bool isDirectionUEnabled(int dir) const;
        
        /**
         * @brief 设置指定方向的位移值
         * @param dir 方向索引 (0=X, 1=Y, 2=Z)
         * @param val 位移值
         */
        void setValueU(int dir, double val);
        
        /**
         * @brief 获取指定方向的位移值
         * @param dir 方向索引 (0=X, 1=Y, 2=Z)
         * @return 位移值
         */
        double getValueU(int dir) const;

    private:
        double m_temperature{0};  ///< 温度值
        int m_amplitudeId{0};     ///< 幅值ID
        //Abaqus中不存在这两个选项，为IGA项目特殊添加
        bool m_directionU[3]{true,true,true}; ///< 方向约束标志 U
        double m_valueU[3]{0,0,0};            ///< 方向约束值
    };

  
    /**
     * @class FITKBoundaryConditionTemperature
     * @brief 温度边界条件类
     * 
     * 该类表示可应用于模型的温度边界条件，
     * 继承自FITKAbaAbstractBoundaryCondition
     */
    class FITKInterfaceStructuralAPI FITKAbaBoundaryConditionTemperature : public FITKAbaAbstractBoundaryCondition
    {
    public:
        /** @brief 默认构造函数 */
        explicit FITKAbaBoundaryConditionTemperature() = default;
        
        /** @brief 虚拟析构函数 */
        virtual ~FITKAbaBoundaryConditionTemperature() = default;

        /**
         * @brief 从另一个对象复制数据
         * @param obj 源对象的指针
         * @return 如果复制成功返回true，否则返回false
         */
        bool copy(Core::FITKAbstractDataObject* obj) override;
     
        /**
         * @brief 获取边界条件类型
         * @return 边界条件类型
         */
        virtual FITKAbaBoundaryConditionType getBoundaryConditionType() const override;

        /**
         * @brief 检查此条件在特定步骤中是否被修改
         * @param stepId 要检查的步骤ID
         * @return 如果在步骤中被修改则返回true，否则返回false
         */
        bool isModifiedInStep(int stepId) const override;

        /**
         * @brief 处理活动状态的变化
         */
        void activeStateChanged() override;

        /**
         * @brief 为特定步骤设置幅值ID
         * @param id 要设置的幅值ID
         * @param stepId 步骤ID（默认：0）
         */
        void setAmplitudeId(int id, int stepId = 0);
        
        /**
         * @brief 获取特定步骤的幅值ID
         * @param stepId 步骤ID（默认：0）
         * @return 幅值ID
         */
        int getAmplitudeId(int stepId = 0);

        /**
         * @brief 为特定步骤设置温度
         * @param temperature 要设置的温度值
         * @param stepId 步骤ID（默认：0）
         */
        void setTemperature(double temperature, int stepId = 0);
        
        /**
         * @brief 获取特定步骤的温度
         * @param stepId 步骤ID（默认：0）
         * @return 温度值
         */
        double getTemperature(int stepId = 0);

        /**
         * @brief 为特定步骤启用或禁用指定方向的位移约束
         * @param dir 方向索引 (0=X, 1=Y, 2=Z)
         * @param en 是否启用约束
         * @param stepId 步骤ID（默认：0）
         */
        void enableDirectionU(int dir, bool en, int stepId = 0);
        
        /**
         * @brief 检查特定步骤指定方向的位移约束是否启用
         * @param dir 方向索引 (0=X, 1=Y, 2=Z)
         * @param stepId 步骤ID（默认：0）
         * @return 如果启用则返回true，否则返回false
         */
        bool isDirectionUEnabled(int dir, int stepId = 0);

        /**
         * @brief 为特定步骤设置指定方向的位移值
         * @param dir 方向索引 (0=X, 1=Y, 2=Z)
         * @param val 位移值
         * @param stepId 步骤ID（默认：0）
         */
        void setValueU(int dir, double val, int stepId = 0);
        
        /**
         * @brief 获取特定步骤指定方向的位移值
         * @param dir 方向索引 (0=X, 1=Y, 2=Z)
         * @param stepId 步骤ID（默认：0）
         * @return 位移值
         */
        double getValueU(int dir, int stepId = 0);

    private:
        /**
         * @brief 获取特定步骤的数据
         * @param stepId 步骤ID
         * @return 指定步骤的温度数据
         */
        BCTemperatureData getDataInStep(int stepId);

    private:
        BCTemperatureData m_data{};                    ///< 默认温度数据
        QMap<int, BCTemperatureData> m_modifiedDatas{}; ///< 每个步骤ID的修改数据映射
    };
}

#endif // __FITKBOUNDARY_CONDITION_TEMPERATURE_H__


