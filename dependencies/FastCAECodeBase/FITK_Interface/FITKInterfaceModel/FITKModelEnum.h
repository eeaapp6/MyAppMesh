/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKModelEnum.h
 * @brief 模型相关枚举值
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-03-07
 *
 */
#ifndef _FITKINTERFACE_MODERL_ENUM_H__
#define _FITKINTERFACE_MODERL_ENUM_H__

#include <QObject>
#include "FITKInterfaceModelAPI.h"


namespace Interface
{
    /**
     * @brief 模型相关枚举
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-04-13
     */
    class FITKInterfaceModelAPI FITKModelEnum : public QObject
    {
        Q_OBJECT
    public:

        /**
         * @brief 单元类型枚举
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-29
         */
        enum FITKEleType
        {
            EleNone,
            Vertex1,
            Line2,
            Line3,
            Tri3,
            Tri6,
            Quad4,
            Quad8,
            Tet4,
            Tet10,
            Wedge6,
            Wedge15,
            Hex8,
            Hex20,
            Polygon,
        };
        Q_ENUM(FITKEleType);

        /**
         * @brief 集合类型
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-30
         */
        enum FITKModelSetType
        {
            FMSNone,     ///< 空
            FMSNode,     ///< 网格点
            FMSElem,     ///< 网格单元
            FMSMIX,      ///< 复合集合
            FMSPoint,    ///< 几何点
            FMSEdge,     ///< 几何线
            FMSSurface,  ///< 几何面
            FMSSolid,    ///< 几何实体
            FMSComb,     ///< 几何装配
        };
        Q_ENUM(FITKModelSetType);

        /**
         * @brief 集合维度类型
         * @author Yanzhihui (chanyuantiandao@126.com)
         * @date 2025-09-25
         */
        enum FITKModelSetDim {
            FMSDimNone = 0,
            FMSDimD0 = 1, 
            FMSDimD1 = 2,
            FMSDimD2 = 4,
            FMSDimD3 = 8,
            FMSDimMix = 16,
        };
        Q_ENUM(FITKModelSetDim);
        /**
         * @brief 网格模型维度
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-03
         */
        enum FITKMeshDim
        {
            FMDimNone = 0,
            FMDimD0 = 1,
            FMDimD1 = 2,
            FMDimD2 = 4,
            FMDimD3 = 8,
            FMDimMix = 16,
        };
        Q_ENUM(FITKMeshDim);

        /**
         * @brief 模型维度
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-11
         */
        enum FITKModelDim
        {
            FMDNone,
            FMD0, //点
            FMD1, //线，梁
            FMD2, //面，壳
            FMD3, //体，solid
        };
        Q_ENUM(FITKModelDim);

        /**
         * @brief 模型类型
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-27
         */
        enum AbsModelType
        {
            AMTNone,                  ///< 错误值
            AMTMesh = 100,            ///< 网格类型
            AMTPartical,              ///< 粒子网格类型
            AMTunstructuredMesh,      ///< 非结构网格类型
            AMTStructuredMesh,        ///< 结构网格类型
            AMTunstructuredMeshvtk,   ///< 非结构网格类型
            AMTGeometry = 200,            ///< 几何类型
            AMTOCC,                   ///< OCC几何类型
            AssInstance = 300,        ///< 装配实例
            Assembly,                 ///< 装配
            AMTMIX = 1000,
        };
        Q_ENUM(AbsModelType);

    public:
        /**
         * @brief 根据网格类型获取网格维度
         * @param[i]  eletype        单元类型
         * @return FITKMeshDim
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-03
         */
        static FITKMeshDim GetElementDim(FITKEleType eletype);

        /**
         * @brief 判断维度标志中是否包含指定的维度
         * @param[in] flags 要检查的维度标志（整数）
         * @param[in] dim   要检查的目标维度
         * @return bool     如果包含指定维度返回true，否则返回false
         * @author LiBaojun
         * @date 2025-04-11
         */
        static bool HasMeshDimFlag(const unsigned int flags, const FITKMeshDim & dim);

    private:
        FITKModelEnum() = default;
        FITKModelEnum(const FITKModelEnum&) = delete;
        ~FITKModelEnum() = default;



    };


}


#endif

