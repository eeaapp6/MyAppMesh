/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKComponentCreator.h
 * @brief 模型部件创建器
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-04-24
 *
 */
#ifndef __FITKCOMPONENT_CREATOR_H__
#define __FITKCOMPONENT_CREATOR_H__

#include "FITK_Kernel/FITKCore/FITKAbstractObject.hpp"
#include "FITKInterfaceModelAPI.h"

namespace Interface
{
    class FITKComponentManager;
    class FITKModelSet;
    class FITKMeshSurface;

    /**
     * @brief 模型部件创建器
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-04-24
     *
     */
    class FITKInterfaceModelAPI FITKComponentCreator : public Core::FITKAbstractObject
    {
    public:
        /**
         * @brief Construct a new FITKComponentCreator object
         * @param[i]  manager        管理器，创建的对象将放入管理器
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-24
         */
        explicit FITKComponentCreator(FITKComponentManager* manager);
        /**
         * @brief Destroy the FITKComponentCreator object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-24
         */
        virtual ~FITKComponentCreator() = default;
        /**
         * @brief 设置名称
         * @param[]  name           My Param doc
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-24
         */
        void setName(const QString& name);
        /**
         * @brief 是否为内部集合
         * @param[]  internal       My Param doc
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-24
         */
        void isInternal(const bool internal);
        /**
         * @brief 创建节点集合
         * @param[i]  parentID       所属对象的id
         * @param[i]  member         成员ID
         * @return FITKModelSet*     创建的集合
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-24
         */
        FITKModelSet* createNodeSet(const int &parentID, const QList<int> & member, bool addCompManager = true);
        /**
         * @brief 创建单元集合
         * @param[i]  parentID       所属对象的id
         * @param[i]  member         成员ID
         * @return FITKModelSet*     创建的集合
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-24
         */
        FITKModelSet* createEleSet(const int &parentID, const QList<int> & member, bool addCompManager = true);
        /**
         * @brief 创建几何点集合
         * @param[i]  parentID       所属对象的id
         * @param[i]  member         成员ID
         * @return FITKModelSet*     创建的集合
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date 2025-08-28
         */
        FITKModelSet* createGeometryVertexSet(const int &parentID, const QList<int> & member, bool addCompManager = true);
        /**
         * @brief 创建几何边集合
         * @param[i]  parentID       所属对象的id
         * @param[i]  member         成员ID
         * @return FITKModelSet*     创建的集合
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date 2025-08-28
         */
        FITKModelSet* createGeometryEdgeSet(const int &parentID, const QList<int> & member, bool addCompManager = true);
        /**
         * @brief 创建几何面集合
         * @param[i]  parentID       所属对象的id
         * @param[i]  member         成员ID
         * @return FITKModelSet*     创建的集合
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date 2025-08-28
         */
        FITKModelSet* createGeometryFaceSet(const int &parentID, const QList<int> & member, bool addCompManager = true);
        /**
         * @brief 创建几何体集合
         * @param[i]  parentID       所属对象的id
         * @param[i]  member         成员ID
         * @return FITKModelSet*     创建的集合
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date 2025-08-28
         */
        FITKModelSet* createGeometrySolidSet(const int &parentID, const QList<int> & member, bool addCompManager = true);

        /**
         * @brief 编辑节点集合
         * @param[i]  parentID       所属对象的id
         * @param[i]  member         成员ID
         * @return FITKModelSet*     创建的集合
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-11-14
         */
        FITKModelSet* editNodeSet(const int &parentID, const QList<int> & member, int originId);
        /**
         * @brief 编辑单元集合
         * @param[i]  parentID       所属对象的id
         * @param[i]  member         成员ID
         * @return FITKModelSet*     创建的集合
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-11-14
         */
        FITKModelSet* editEleSet(const int &parentID, const QList<int> & member, int originId);
        /**
         * @brief 创建单元表面集合
         * @param[i]  setSurfaceIndex   集合与表面索引的映射
         * @return FITKMeshSurface*     创建的表面
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-24
         */
        FITKMeshSurface* createEleSurface(QHash<Interface::FITKModelSet*, int >& setSurfaceIndex);
        /**
         * @brief  编辑单元表面集合.
         * @param  setSurfaceIndex 集合与表面索引的映射
         * @param  originId 原始ID
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-11-14
         */
        FITKMeshSurface* editEleSurface(const QHash<Interface::FITKModelSet*, int >& setSurfaceIndex, int originId);
        /**
         * @brief 创建节点表面集合
         * @param[i]  setSurfaceIndex   集合与表面索引的映射
         * @return FITKMeshSurface*     创建的表面
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-24
         */
        FITKMeshSurface* createNodeSurface(QHash<Interface::FITKModelSet*, int >& setSurfaceIndex);
        /**
         * @brief  编辑节点表面集合.
         * @param  setSurfaceIndex 集合与表面索引的映射
         * @param  originId 原始ID
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-11-14
         */
        FITKMeshSurface* editNodeSurface(const QHash<Interface::FITKModelSet*, int >& setSurfaceIndex, int originId);

    private:
        /**
         * @brief 创建集合
         * @param[i]  parentID       所属对象的id
         * @param[i]  member         成员ID
         * @return FITKModelSet*     创建的集合
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-24
         */
        FITKModelSet* createSet(const int &parentID, const QList<int> & member, bool addCompManager = true);

    private:
        /**
         * @brief 组件管理器，构造函数传入
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-24
         */
        FITKComponentManager* _compManager{};
        /**
         * @brief 是否内部集合
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-24
         */
        bool _internal{ true };
        /**
         * @brief 名称
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-24
         */
        QString _name{};

    };

}


#endif
