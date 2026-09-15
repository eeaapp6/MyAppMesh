/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   FITKOFEnum.hpp
 * @brief FITKOF枚举文件
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date 2024-06-13
 */
#ifndef _FITKOFENUM_HPP__
#define _FITKOFENUM_HPP__

#include "FITKInterfaceFlowOFAPI.h"
#include <QObject>

namespace Interface
{
    /**
     * @brief  流体枚举类
     * @author BaGuijun (baguijun@163.com)
     * @date   2025-04-14
     */
    class FITKInterfaceFlowOFAPI FITKOFSolverTypeEnum : public QObject
    {
        Q_OBJECT;
    public:
        /**
         * @brief 求解器过滤类型
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-06-12
         */
        enum FITKOFSolverFiltersType
        {
            NoneFiltersType,      /// 无过滤类型
            SteadyState,          /// 稳态问题，流体特性不随时间变化
            Transient,            /// 瞬态问题，流体特性随时间变化
            Incompressible,       /// 不可压缩流体
            Compressible,         /// 可压缩流体
            Turbulences,          /// 湍流问题
            MRF,                  /// 多重参考系（周期面） (Multiple Reference Frame)，用于旋转机械模拟
            Spcies,               /// 物种传输问题（如化学反应中的物质传输）
            HeatTransfer,         /// 热传导或热对流问题
            SRF,                  /// 单一参考系 (Single Reference Frame)，用于旋转系统的简化模拟
            Lagrangian,           /// 拉格朗日方法，通常用于颗粒或离散相模拟
            Radiation,            /// 辐射传热问题
            Porosity,             /// 多孔介质流动
            DynamicMesh,          /// 动态网格，用于模拟随时间变化的几何形状
            Buoyancy,             /// 浮力驱动流动
            Multiphase,           /// 多相流问题（如液体-气体界面）
            UserDefined,          /// 用户自定义过滤类型
        };
        Q_ENUM(FITKOFSolverFiltersType);
        /**
         * @brief 求解器类型
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-06-12
         */
        enum FITKOFSolverType
        {
            SolverNone,
            SIMPLE,                      ///稳态不可压缩求解器
            INTER,                       ///多相流求解器
            CHT_MULTI_REGION,            ///多区域共轭传热求解器
        };
        /**
         * @brief  湍流模式
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-08-13
         */
        enum FITKOFTurbulenceType
        {
            TurbNone,
            RANS,                        ///雷诺平均方法
            LES,                         ///大涡模拟
        };
        Q_ENUM(FITKOFTurbulenceType);
        /**
         * @brief  边界类型
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-04-14
         */
        enum FITKOFBoundaryType
        {
            BNone,
            BWall,                       ///壁面
            BPressureInlet,              ///压力入口
            BVelocityInlet,              ///速度入口
            BPressureOutlet,             ///压力出口
            BOutflow,                    ///流出边界
            BSymmetry,                   ///对称边界
            BWedge,                      ///楔形边界
            BEmpty,                      ///空边界
            BMappedWall,                 ///映射墙
        };
        /**
         * @brief  区域网格类型
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-06-17
         */
        enum FITKOFRegionMeshType
        {
            None,                         ///空
            Fluid,                        ///流体
            Solid,                        ///固体
        };
        Q_ENUM(FITKOFRegionMeshType);
    private:
        /**
         * @brief    构造函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-14
         */
        explicit FITKOFSolverTypeEnum() = default;
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-14
         */
        ~FITKOFSolverTypeEnum() = default;
    };

    /**
     * @brief FITKOF SolverInitial枚举
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date 2024-06-12
     */
    class FITKInterfaceFlowOFAPI FITKOFSolverInitialEnum : public QObject
    {
        Q_OBJECT
    public:

        enum FITKPatchApplyObjectType
        {
            Cells,
            Faces,
            Both,
        };
        Q_ENUM(FITKPatchApplyObjectType);

    private:
        explicit FITKOFSolverInitialEnum() = default;
        ~FITKOFSolverInitialEnum() = default;
    };
}


#endif
