/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKMeshSurface.h
 * @brief  网格表面类声明，基于elementset表示
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-03-26
 *
 */
#ifndef __FITKMESH__SURFACE_NODE_H___
#define __FITKMESH__SURFACE_NODE_H___

#include "FITKMeshSurface.h"

namespace Interface
{
    
    /**
     * @brief 网格节点表面类，基于set的表示
     * @author  LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-26
     */
    class FITKInterfaceModelAPI FITKMeshSurfaceNode : public FITKMeshSurface
    {
        FITKCLASS(Interface, FITKMeshSurfaceNode);
    public:
        /**
         * @brief Construct a new FITKMeshSurface object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-26
         */
        explicit FITKMeshSurfaceNode() = default;
        /**
         * @brief Destroy the FITKMeshSurface object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-26
         */
        virtual ~FITKMeshSurfaceNode() = default;
         
        /**
         * @brief 获取表面类型
         * @return MeshSurfaceType
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-26
         */
        FITKMeshSurface::MeshSurfaceType getMeshSurfaceType() override;

        /**
         * @brief       获取模型表面类型。[虚函数][重写]
         * @return      模型表面类型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-28
         */
        virtual ModelSurfaceType getModelSurfaceType() override;

        /**
         * @brief 追加表面
         * @param[i]  modelID        set的父对象id，可以是装配、部件 装配实例
         * @param[i]  setID          集合ID
         * @param[i]  value          权重参数
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-26
         */
        void addMeshSet(int modelID,int setID, double value);
        /**
         * @brief 追加表面
         * @param[i]  model          set的父对象，可以是装配、部件 装配实例
         * @param[i]  set            集合
         * @param[i]  value          权重参数
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-26
         */
        void addMeshSet(FITKAbstractModel* model, FITKModelSet* set, double value);
        /**
         * @brief  清除表面集合.
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-11-14
         */
        void clearMeshSet();
        /**
         * @brief 获取第index个值
         * @param[i]  index          索引
         * @return double
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-26
         */
        double getValueAt(const int index) const;

        /**
         * @brief 获取集合信息，将复合集合拆分，返回值不会包含复合集合
         * @return QList<SurfaceAbsSetInfo>
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-26
         */
        virtual QList<SurfaceAbsSetInfo> getAbsoluteSetInfo() override;

    protected:
 
        /**
         * @brief 权重列表
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-26
         */
        QList<double> _values{};
    };
}


#endif
