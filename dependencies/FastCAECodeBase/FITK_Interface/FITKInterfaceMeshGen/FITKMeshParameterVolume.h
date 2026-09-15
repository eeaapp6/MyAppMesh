/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   FITKMeshParameterVolume.h
 * @brief 几何网格区域尺寸生成器
 * @author fulipeng (fulipengqd@yeah.net)
 * @date 2024-12-17
 */
#ifndef _FITKMeshParameterVolume_H___
#define _FITKMeshParameterVolume_H___

#include "FITKInterfaceMeshGenAPI.h"
#include "FITKMeshParameterBase.h"

#include <QObject>
#include <QMetaEnum>

namespace Interface
{
    /**
     * @brief  扫略体网格参数
     * @author BaGuijun (baguijun@163.com)
     * @date   2025-07-08
     */
    struct FITKVMeshSweepParameter{
        //源面Id列表
        QList<int> _sweepSourceFaceIDs = {};
        //目标面id
        int _sweepTargetFaceID = -1;
        //路径id
        int _sweepPathID = -1;
        //是否启动扫掠层数指定
        bool _isSweepLayer = false;
        //扫掠层数
        int _sweepLayer = -1;
    };
    /**
     * @brief 体网格划分参数抽象类
     * @author fulipeng (fulipengqd@yeah.net)
     * @date 2024-12-17
     */
    class FITKInterfaceMeshGenAPI FITKMeshParameterVolume : public FITKMeshParameterBase
    {
        FITKCLASS(Interface, FITKMeshParameterVolume);
        Q_OBJECT;
    public:
        enum ElementType{
            Hex,
            Hex_dominated,
            Tet,
            Wedge,
        };
        Q_ENUM(ElementType);
        /**
         * @brief  单元生成方式
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-06-30
         */
        enum ElementGenType {
            AsIs,               //保持原状
            Free,               //自由
            Structured,         //结构
            Sweep,              //扫掠
            BottomUp,           //自底向上
            Multiple,           //重复
        };
        Q_ENUM(ElementGenType);
    public:
        /**
         * @brief Construct a new FITKMeshParameterVolume object
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-12-17
         */
        explicit FITKMeshParameterVolume() = default;
        /**
         * @brief Destroy the FITKMeshParameterVolume object
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-12-17
         */
        virtual ~FITKMeshParameterVolume() = default;
        /**
         * @brief    获取网格参数类型
         * @return   FITKMeshParamType          网格参数类型
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-01-25
         */
        virtual FITKMeshParamType getMeshParamType() override;
        /**
         * @brief   参数拷贝
         * @param   parameter 输入的拷贝参数
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2025-04-11
         */
        void parameterCopy(FITKMeshParameterVolume& parameter);

        /**
         * @brief       设置全局网格尺寸
         * @param gSize 全局网格尺寸
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-12-17
         */
        void setGlobalMeshSize(double gSize);
        /**
         * @brief   获取全局网格尺寸
         * @return  全局网格尺寸
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-12-17
         */
        double getGlobalMeshSize();

        /**
         * @brief       设置最大网格尺寸
         * @param size  全局网格尺寸
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-12-17
         */
        void setMaxMeshSize(double size);
        /**
         * @brief   获取最大网格尺寸
         * @return  全局网格尺寸
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-12-17
         */
        double getMaxMeshSize();

        /**
         * @brief       设置最小网格尺寸
         * @param size  最小网格尺寸
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-12-17
         */
        void setMinMeshSize(double size);
        /**
         * @brief   获取最小网格尺寸
         * @return  最小网格尺寸
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-12-17
         */
        double getMinMeshSize();

        /**
         * @brief   设置最大线程数
         * @param   numberThreads   最大线程数
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-12-17
         */
        void setMaxNumberOfThreads(int numberThreads);
        /**
         * @brief   获取最大线程数
         * @return  最大线程数
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-12-17
         */
        int getMaxNumberOfThreads();

        /**
         * @brief       设置单元类型
         @param eleType 单元类型
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-12-17
         */
        bool setElementType(ElementType eleType);
        /**
         * @brief   获取单元类型
         * @return  单元类型
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-12-17
         */
        ElementType getElementType();
        /**
         * @brief    设置单元生成方式
         * @param[i] type                       方式
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-06-30
         */
        void setElementGenType(ElementGenType type);
        /**
         * @brief    获取单元生成方式
         * @return   ElementGenType             方式
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-06-30
         */
        FITKMeshParameterVolume::ElementGenType getElementGenType();
        /**
         * @brief    获取扫掠参数
         * @return   FITKVMeshSweepParameter    参数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-07-08
         */
        FITKVMeshSweepParameter getSweepPara();
        /**
         * @brief    设置扫掠参数
         * @param[i] para                       参数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-07-08
         */
        void setSweepPara(FITKVMeshSweepParameter para);
        /**
         * @brief    获取是否需要计算扫掠
         * @return   bool                       是否计算
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-07-08
         */
        bool getIsCalculateOfSweep();
    protected:
        /**
         * @brief   单元类型 默认四面体
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-12-17
         */
        ElementType _eleType = ElementType::Tet;
        /**
         * @brief  单元生成方式，默认自由
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-06-30
         */
        ElementGenType _eleGenType = ElementGenType::Free;
        /**
         * @brief  扫掠参数
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-07-08
         */
        FITKVMeshSweepParameter _sweepPara;
        /**
         * @brief   全局网格尺寸
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-12-17
         */
        double _globalMeshSize = 0;

        /**
         * @brief   网格最大尺寸
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-12-17
         */
        double _maxMeshSize = 0;

        /**
         * @brief   网格最小尺寸
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-12-17
         */
        double _minMeshSize = 0;

        /**
         * @brief   最大线程数
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-12-17
         */
        int _maxNumberThreads = 1;
    };
}
#endif
