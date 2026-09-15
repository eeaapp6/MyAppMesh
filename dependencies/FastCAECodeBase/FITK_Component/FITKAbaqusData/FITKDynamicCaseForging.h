/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file FIITKDynamicCaseForging.h
 * @brief 锻造算例数据类,对Abaqus数据结构的扩展
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2025-10-24
 * 
 */
#ifndef FIITKDynamicCaseForging_H
#define FIITKDynamicCaseForging_H

#include "FITKDataCase.h"
#include "FITKAbaqusDataAPI.h"

namespace Forging 
{
    /**
     * @brief 锻造算例数据类,对Abaqus数据结构的扩展，非Abaqus原生算例数据结构
     * @author libaojun
     * date 2025-10-24
     */
    class FITKAbaqusDataAPI FITKDynamicCaseForging : 
           public AbaqusData::FITKDataCase 
    {
        Q_OBJECT
        FITKCLASS(Forging, FITKDynamicCaseForging);

    public:
        enum ProcessType
        {
            FORGING,          ///< 锻造
            HEATTREATMENT,    ///< 热处理
        };
        Q_ENUM(ProcessType)

        enum SimulationType
        {
            D3,                 ///< 三维
            D2_AXISYMMETRIC,    ///< 轴对称
            D2_PLANAR           ///< 平面
        };
        Q_ENUM(SimulationType)

    public:
        explicit FITKDynamicCaseForging(bool autoCreatePart = true);
        virtual ~FITKDynamicCaseForging()   = default;

        /**
         * @brief 设置工艺类型
         * @param[i]  type          工艺类型
         * @author libaojun
         * @date 2025-10-24
         */
        void setProcessType(ProcessType type);

        /**
         * @brief 设置仿真类型
         * @param[i]  type          仿真类型
         * @author libaojun 
         * @date 2025-10-24
         */
        void setSimulationType(SimulationType type);

        /**
         * @brief 设置模具数量
         * @param[i]  number       模具数量
         * @author libaojun
         * @date 2025-10-24
         */
        void setNumberOfDies(int number);

        /**
         * @brief 获取工艺类型
         * @return ProcessType 工艺类型
         * @author libaojun
         * @date 2025-10-24
         */
        ProcessType getProcessType() const;
        /**
         * @brief 获取仿真类型
         * @return SimulationType 仿真类型
         * @author libaojun 
         * @date 2025-10-24
         */
        SimulationType getSimulationType() const;
        /**
         * @brief 获取模具数量
         * @return int 模具数量
         * @author libaojun
         * @date 2025-10-24
         */
        int getNumberOfDies() const;
        /**
         * @brief 根据工艺类型、仿真类型和模具数量更新部件名称
         * @author libaojun
         * @date 2025-10-24
         */
        void updatePartsName();

    private:
        /**
         * @brief 更新模具部件
         * @author libaojun
         * @date 2025-10-24
         */
        void updateDiesParts();
        /**
         * @brief 移除所有模具部件
         * @author libaojun
         * @date 2025-10-24
         */
        void removeAllDies();


private:
        ProcessType m_processType{ FORGING };          ///< 工艺类型
        SimulationType m_simulationType{ D3 };         ///< 仿真类型
        int m_numberOfDies{ 2 };                     ///< 模具数量

    };

} // namespace Forging



#endif // FIITKDynamicCaseForging_H