/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossBCS.h
 * @brief  Radioss边界条件声明：定义节点组在平移和旋转运动中的边界条件。
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date   2025-10-24
 *********************************************************************/
#ifndef FITK_RADIOSS_BCS_H
#define FITK_RADIOSS_BCS_H

#include "FITKRadiossDataAPI.h"
#include "FITKAbstractBCS.h"

namespace Radioss
{
    /**
     * @brief   Radioss边界条件类
     * @details 此类继承自FITKAbstractBCS，表示Radioss中的边界条件（BCS）。
     *          它用于定义节点的约束
     * @author LiBaojun (libaojunqd@foxmail.com)
     */
    class FITKRadiossDataAPI FITKRadiossBCS : public FITKAbstractBCS
    {
        Q_OBJECT
        FITKCLASS(Radioss, FITKRadiossBCS);
        RadiossKeyWord(FITKRadiossBCS, /BCS);

    public:
        explicit FITKRadiossBCS() = default;
        virtual ~FITKRadiossBCS() = default;

        /**
         * @brief 获取边界条件类型
         * @return BCSType 返回边界条件类型
         * @details 此方法返回当前对象的边界条件类型。
         *          默认返回BCS类型，可以在派生类中重写以提供具体类型。
         */
        virtual BCSType getBCSType() const override;
        
        /**
         * @brief 设置自由度是否固定
         * @param index 自由度索引（1-6）
         * @param fixed 是否固定该自由度
         * @return void
         * @details 此方法用于设置指定自由度的固定状态。
         */
        void setFreedomFixed(int index, bool fixed);
     
        /**
         * @brief 获取自由度是否固定
         * @param index 自由度索引（1-6）
         * @return bool 返回该自由度是否被固定
         * @details 此方法用于获取指定自由度的固定状态。
         */
        bool isFreedomFixed(int index) const;
    
    private:
        bool _freedomFixed[6] = {true, true, true, true, true, true}; // 6自由度全部约束
    
    };
    /**
     * @brief Radioss周期边界条件类
     * @details 此类继承自FITKRadiossBCS，表示Radioss中的周期边界条件（BSSCyclic）。
     *          它用于定义节点的周期性约束。
     * @author LiBaojun (libaojunqd@foxmail.com)
     */
    class FITKRadiossDataAPI FITKRadiossBCSCyclic : public FITKAbstractBCS
    {
        Q_OBJECT
        FITKCLASS(Radioss, FITKRadiossBCSCyclic);
        RadiossKeyWord(FITKRadiossBCSCyclic, /BCS/CYCLIC);
    public:
        explicit FITKRadiossBCSCyclic() = default;
        virtual ~FITKRadiossBCSCyclic() = default;
        /**
         * @brief 获取边界条件类型
         * @return BCSType 返回边界条件类型
         * @details 此方法返回当前对象的边界条件类型。
         *          默认返回BSSCyclic类型，可以在派生类中重写以提供具体类型。
         */
        virtual BCSType getBCSType() const override;
    };

    /**
     * @brief Radioss拉格朗日乘子边界条件类
     * @details 此类继承自FITKRadiossBCS，表示Radioss中的拉格朗日乘子边界条件（BCSLagMul）。
     *          它用于定义节点的拉格朗日乘子约束。
     * @author LiBaojun (libaojunqd@foxmail.com)
     */
    class FITKRadiossDataAPI FITKRadiossBCSLagMul : public FITKRadiossBCS
    {
        Q_OBJECT
        FITKCLASS(Radioss, FITKRadiossBCSLagMul);
        RadiossKeyWord(FITKRadiossBCSLagMul, /BCS/LAGMUL);
    public:
        explicit FITKRadiossBCSLagMul() = default;
        virtual ~FITKRadiossBCSLagMul() = default;
        /**
         * @brief 获取边界条件类型
         * @return BCSType 返回边界条件类型
         * @details 此方法返回当前对象的边界条件类型。
         *          默认返回BCSLAGMUL类型，可以在派生类中重写以提供具体类型。
         */
        virtual BCSType getBCSType() const override;

    };

}

#endif // FITK_RADIOSS_BCS_H
