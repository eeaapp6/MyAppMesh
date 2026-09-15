/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file FITKAbaInteractionContactThermalConductanceBehavior.h
 * @brief 接触热传导行为类
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2025-06-04
 * 
 */
#ifndef FITK_ABA_INTERACTION_CONTACT_THERMAL_CONDUCTANCE_BEHAVIOR_H
#define FITK_ABA_INTERACTION_CONTACT_THERMAL_CONDUCTANCE_BEHAVIOR_H

#include "FITKInterfaceStructuralAPI.h"
#include "FITKAbaInteractionContactBehavior.h"
#include <QObject>
#include <QList>

namespace Interface
{
    /**
     * @brief 热传导行为数据行
     * @author libaojun
     * @date 2025-06-04
     */
    struct FITKAbaThermalConductanceBehaviorRowData
    {
        double m_conductance{0.0}; ///< 热传导系数
        double m_clearance{0.0}; ///< 间隙
        double m_pressure{0.0}; ///< 压力
        double m_temperature{0.0}; ///< 温度
        double m_massFlowRate{0.0}; ///< 质量流量
        QList<double> m_fieldValues; ///< 其他字段值
    };
    
    /**
     * @brief 热传导行为数据类
     * @author libaojun
     * @date 2025-06-04
     */
    class FITKInterfaceStructuralAPI FITKAbaThermalConductanceBehaviorData
    {
    public:
        explicit FITKAbaThermalConductanceBehaviorData() = default;
        virtual ~FITKAbaThermalConductanceBehaviorData() = default;

        /**
         * @brief 获取热传导行为数据行
         * @return QList<FITKAbaThermalConductanceBehaviorRowData>
         */
        const QList<FITKAbaThermalConductanceBehaviorRowData>& getRowData() const;

        /**
         * @brief 添加一行数据
         * @param row 行数据
         */
        void addRowData(const FITKAbaThermalConductanceBehaviorRowData& row);

        /**
         * @brief 根据索引删除一行数据
         * @param index 行索引
         * @return 是否删除成功
         */
        bool removeRowData(int index);
        /**
         * @brief 获取指定索引的数据
         * @param index 行索引
         * @return 指定行的数据指针，若索引无效返回nullptr
         */
        FITKAbaThermalConductanceBehaviorRowData* getRowDataAt(int index);

        /**
         * @brief 清空所有数据
         */
        void clearRowData();

        /**
         * @brief 是否使用温度相关的热传导
         * @return true 如果使用，false 如果不使用
         */
        bool isTempDependent() const { return m_useTempDependent; }
        /**
         * @brief 设置是否使用温度相关的热传导
         * @param use 是否使用
         */
        void setTempDependent(bool use) { m_useTempDependent = use; }

        /**
         * @brief 是否使用质量流量相关的热传导
         * @return true 如果使用，false 如果不使用
         */
        bool isMassFlowRateDependent() const { return m_useMassFlowRateDependent; }
        /**
         * @brief 设置是否使用质量流量相关的热传导
         * @param use 是否使用
         */
        void setMassFlowRateDependent(bool use) { m_useMassFlowRateDependent = use; }
    private:
        bool m_useTempDependent{false}; ///< 是否使用温度相关的热传导
        bool m_useMassFlowRateDependent{false}; ///< 是否使用质量流量相关的热传导
        QList<FITKAbaThermalConductanceBehaviorRowData> m_rowData; ///< 热传导行为数据行

    };
    /**
     * @brief 接触热传导行为类
     * @author libaojun
     * @date 2025-06-04
     */
    class FITKInterfaceStructuralAPI FITKAbaInteractionContactThermalConductanceBehavior
      : public QObject, public FITKAbaInteractionContactBehavior
    {
        Q_OBJECT
        
        public:
        /**
         * @brief 热传导类型枚举
         * @author libaojun
         * @date 2025-06-04
         */
        enum ThermalConductanceType
        {
            DependentOnClearance,  ///< 依赖于间隙
            DependentOnPressure,   ///< 依赖于压力
            DependentOnBoth,      ///< 依赖于间隙和压力
        };
        Q_ENUM(ThermalConductanceType);
    

    public:
        explicit FITKAbaInteractionContactThermalConductanceBehavior();
        virtual ~FITKAbaInteractionContactThermalConductanceBehavior();
        /**
         * @brief 获取热传导类型
         * @return ThermalConductanceType
         */
        ThermalConductanceType getConductanceType() const;
        /**
         * @brief 设置热传导类型
         * @param type 热传导类型
         */
        void setConductanceType(ThermalConductanceType type);
        /**
         * @brief 获取接触行为类型
         * @return FITKAbaInteractionContactBehaviorEnum::FITKAbaContactBehaviorType
         */
        FITKAbaInteractionContactBehaviorEnum::FITKAbaContactBehaviorType getBehaviorType() const override;
        /**
         * @brief 获取间隙相关的热传导数据
         * @return FITKAbaThermalConductanceBehaviorData*
         */
        FITKAbaThermalConductanceBehaviorData* getClearanceDependentData() const;
        /**
         * @brief 获取压力相关的热传导数据
         * @return FITKAbaThermalConductanceBehaviorData*
         */
        FITKAbaThermalConductanceBehaviorData* getPressureDependentData() const;

        /**
         * @brief 重载=运算符
         * @param other 另一个热传导行为对象
         * @param data FITKAbaThermalConductanceBehaviorData*
         */
        FITKAbaInteractionContactThermalConductanceBehavior& operator=(const FITKAbaInteractionContactThermalConductanceBehavior& other);

    private:
        ThermalConductanceType m_conductanceType{DependentOnClearance}; ///< 热传导类型
        FITKAbaThermalConductanceBehaviorData* m_clearanceDependentData{nullptr}; ///< 间隙相关数据
        FITKAbaThermalConductanceBehaviorData* m_pressureDependentData{nullptr}; ///< 压力相关数据
    };
}

#endif // FITK_ABA_INTERACTION_CONTACT_THERMAL_CONDUCTANCE_BEHAVIOR_H
