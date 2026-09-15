/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file FITKAbstractBEM.h
 * @brief Radioss 边界元法（Boundary Elements Method）基类声明。
 * @author liuzhonghuaAI
 * @date 2025-04-27
 */
#ifndef FITK_ABSTRACT_BEM_H
#define FITK_ABSTRACT_BEM_H

#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"
#include "FITKAbstractRadiossData.h"

namespace Radioss
{
    class FITKRadiossDataAPI FITKAbstractBEM : public Core::FITKAbstractNDataObject,
        public FITKAbstractRadiossData
    {
        Q_OBJECT
    public:
        /**
         * @brief 边界元法类型。
         */
        enum BEMType
        {
            BEM_DAA = 1,   ///< Doubly Asymptotic Approximation（双重渐近近似）。
            BEM_FLOW,      ///< Incompressible fluid flow（不可压缩流体流动）。
        }; Q_ENUM(BEMType)

    public:
        /**
         * @brief 默认构造函数。
         * @author liuzhonghuaAI
         * @date 2025-04-27
         */
        explicit FITKAbstractBEM() = default;

        /**
         * @brief 虚析构函数。
         * @author liuzhonghuaAI
         * @date 2025-04-27
         */
        virtual ~FITKAbstractBEM() = 0;

        /**
         * @brief 获取边界元法类型。
         * @return BEM 类型。
         * @author liuzhonghuaAI
         * @date 2025-04-27
         */
        virtual BEMType getBEMType() = 0;
    };

    /**
     * @brief BEM 管理器类。
     */
    class FITKRadiossDataAPI FITKRadiossBEMManager :
        public Core::FITKAbstractDataManager<FITKAbstractBEM>
    {
    public:
        explicit FITKRadiossBEMManager() = default;
        virtual ~FITKRadiossBEMManager() = default;
    };
}

#endif // FITK_ABSTRACT_BEM_H