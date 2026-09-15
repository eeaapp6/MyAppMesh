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
#ifndef __FITKMESH__SURFACE_H___
#define __FITKMESH__SURFACE_H___

#include "FITKModelSurface.h"
#include <QList>
#include <QPair>

namespace Interface
{
    class FITKAbstractModel;
    class FITKModelSet;

    /**
     * @brief 网格表面类，基于set的表示
     * @author  LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-26
     */
    class FITKInterfaceModelAPI FITKMeshSurface : public FITKModelSurface
    {
        Q_OBJECT
        FITKCLASS(Interface, FITKMeshSurface);
    public:
        /**
     * @brief 表面类型
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-26
     */
        enum MeshSurfaceType
        {
            SurNone,  ///< 错误值
            SurEle,   ///< 单元表面
            SurNode,  ///< 节点表面
        };
        Q_ENUM(MeshSurfaceType);
    public:
        /**
         * @brief Construct a new FITKMeshSurface object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-26
         */
        explicit FITKMeshSurface() = default;
        /**
         * @brief Destroy the FITKMeshSurface object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-26
         */
        virtual ~FITKMeshSurface() = 0;

        /**
         * @brief 获取表面类型
         * @return MeshSurfaceType
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-26
         */
        virtual MeshSurfaceType getMeshSurfaceType() = 0;

        /**
         * @brief 获取包含集合的数量
         * @return int
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-26
         */
        int getMeshSetCount() const;
        /**
         * @brief 获取第index个集合
         * @param[i]  index         索引
         * @return FITKModelSet*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-26
         */
        FITKModelSet* getMeshSetAt(const int index) const;

    };
}


#endif
