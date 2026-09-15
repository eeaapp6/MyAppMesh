/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   FITKMeshParameterSurface.h
 * @brief 面网格划分参数
 * @author fulipeng (fulipengqd@yeah.net)
 * @date 2024-12-17
 */
#ifndef _FITKMeshParameterSurface_H___
#define _FITKMeshParameterSurface_H___

#include "FITKInterfaceMeshGenAPI.h"
#include "FITKMeshParameterBase.h"

#include <QObject>
#include <QMetaEnum>

namespace Interface
{
    /**
     * @brief 面网格划分参数抽象类
     * @author fulipeng (fulipengqd@yeah.net)
     * @date 2024-12-17
     */
    class FITKInterfaceMeshGenAPI FITKMeshParameterSurface : public FITKMeshParameterBase
    {
        FITKCLASS(Interface, FITKMeshParameterSurface);
        Q_OBJECT;
    public:
        /**
         * @brief   单元类型
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-12-25
         */
        enum ElementType
        {
            FullQuad,       //四边形
            QuadDominant,   //混合 四边形主导
            Triangle,       //三角形
        };
        Q_ENUM(ElementType);

        /**
         * @brief   网格划分方式
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-12-25
         */
        enum TechniqueType
        {
            Asis,           //轴
            Free,           //自由
            Structured,     //结构
            Sweep,          //扫掠
            Multiple        //混合
        };
        Q_ENUM(TechniqueType);
    public:
        /**
         * @brief   Construct a new FITKMeshParameterSurface object
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-12-17
         */
        explicit FITKMeshParameterSurface() = default;
        /**
         * @brief   Destroy the FITKMeshParameterSurface object
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-12-17
         */
        virtual ~FITKMeshParameterSurface() = default;
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
        void parameterCopy(FITKMeshParameterSurface& parameter);
        /**
         * @brief   初始化控制默认参数
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-12-25
         */
        void initControlDefault();

        /**
         * @brief   设置各向异性网格边界生成参数
         * @param   true    需要生成边界层
         * @param   false   不需要生成边界层
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-12-17
         */
        bool setAnisotropicMeshing(bool toggle);
        /**
         * @brief   获取各向异性网格边界生成参数
         * @return  是否生成边界层
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-12-17
         */
        bool getAnisotropicMeshing();

        /**
         * @brief   设置最大各向异性比
         * @param   ratio   最大各向异性比
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-12-17
         */
        bool setAnisotropicRatio(double ratio);
        /**
         * @brief   获取最大各向异性比
         * @return  最大各向异性比
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-12-17
         */
        double getAnisotropicRatio();

        /**
         * @brief   设置是否优化不良几何形状
         * @param   toggle  是否优化不良几何形状
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-12-17
         */
        void setClosedGeometry(bool toggle);
        /**
         * @brief   设置是否优化不良几何形状
         * @return  是否优化不良几何形状
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-12-17
         */
        bool getClosedGeometry();

        /**
         * @brief   设置网格元素阶次
         * @param   order   网格阶次
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-12-17
         */
        void setElementOrder(int order);
        /**
         * @brief   获取网格元素阶次
         * @return  网格阶次
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-12-17
         */
        int getElementOrder();

        /**
         * @brief   设置全局网格边长尺寸
         * @param   gSize 全局网格边长尺寸
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-12-17
         */
        void setGlobalMeshSize(double gSize);
        /**
         * @brief   获取全局网格边长尺寸
         * @return  全局网格边长尺寸
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-12-17
         */
        double getGlobalMeshSize();

        /**
         * @brief   设置最大网格边长尺寸
         * @param   size  最大网格边长尺寸
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-12-17
         */
        void setMaxMeshSize(double size);
        /**
         * @brief   获取最大网格边长尺寸
         * @return  最大网格边长尺寸
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-12-17
         */
        double getMaxMeshSize();

        /**
         * @brief   设置最小网格边长尺寸
         * @param   size  最小网格边长尺寸
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-12-17
         */
        void setMinMeshSize(double size);
        /**
         * @brief   获取最小网格边长尺寸
         * @return  最小网格边长尺寸
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-12-17
         */
        double getMinMeshSize();

        /**
         * @brief   设置单元类型
         * @param   eleType 单元类型
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-12-17
         */
        bool setElementType(ElementType eleType);
        /**
         * @brief   获取单元类型
         * @return  单元类型
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-12-17
         */
        QString getElementTypeForString();
        /**
         * @brief   获取单元类型
         * @return  单元类型
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-12-17
         */
        ElementType getElementType();
        /**
         * @brief   设置网格划分方式
         * @param   TechniqueType 网格划分方式
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-12-24
         */
        void setTechnique(TechniqueType tType);

        /**
         * @brief   设获取网格划分方式
         * @return  网格划分方式
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-12-24
         */
        TechniqueType getTechnique();

        /**
         * @brief   设置最大线程数
         * @param   numberThreads   最大线程数
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-12-17
         */
        void setMaxNumberOfThreads(int numberThreads);
        /**
         * @brief   获取最大线程数
         * @return  最大线程数
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-12-17
         */
        int getMaxNumberOfThreads();

    protected:
        /**
         * @brief   单元类型
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-12-17
         */
        ElementType _eleType = ElementType::Triangle;

        /**
         * @brief   网格划分方式
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-12-24
         */
        TechniqueType _techniqueType = TechniqueType::Free;
        /**
         * @brief   全局网格尺寸
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-12-17
         */
        double _globalMeshSize = 3;

        /**
         * @brief   网格最大尺寸
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-12-17
         */
        double _maxMeshSize = 3;

        /**
         * @brief   网格最小尺寸
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-12-17
         */
        double _minMeshSize = 3;

        /**
         * @brief   网格阶次
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-12-17
         */
        int _elementOrder = 1;

        /**
         * @brief   最大线程数
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-12-17
         */
        int _maxNumberThreads = 4;
    };
}
#endif
