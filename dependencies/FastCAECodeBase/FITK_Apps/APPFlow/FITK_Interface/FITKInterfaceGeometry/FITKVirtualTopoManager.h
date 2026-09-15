/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKVirtualTopoManager.h
 * @brief 虚拓扑管理器
 * @author libaojun (libaojunqd@foxmail.com)
 * @date 2024-08-12
 *
 */
#ifndef _FITK_VIRTUAL_TOPO_MANAGER_H___
#define _FITK_VIRTUAL_TOPO_MANAGER_H___

#include "FITKInterfaceGeometryAPI.h"
#include "FITKAbsVirtualTopo.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"
#include <QHash>
#include <QMutex>

namespace Interface
{
    /**
     * @brief 几何虚拟拓扑管理器
     * @author libaojun (libaojunqd@foxmail.com)
     * @date 2024-08-09
     */
    class FITKInterfaceGeometryAPI FITKShapeVirtualTopoManager
        : public Core::FITKAbstractDataManager<FITKAbsVirtualTopo>
    {
    public:
        /**
         * @brief Construct a new FITKShapeVirtualTopoManager object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-12
         */
        explicit FITKShapeVirtualTopoManager() = default;
        /**
         * @brief Destroy the FITKShapeVirtualTopoManager object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-12
         */
        virtual ~FITKShapeVirtualTopoManager() = default;

    };

    /**
     * @brief 拓扑管理器
     * @author libaojun (libaojunqd@foxmail.com)
     * @date 2024-08-12
     */
    class FITKInterfaceGeometryAPI FITKVirtualTopoManager : public Core::FITKAbstractDataObject
    {
    public:
        /**
         * @brief Construct a new FITKVirtualTopoManager object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-12
         */
        explicit FITKVirtualTopoManager();
        /**
         * @brief Destroy the FITKVirtualTopoManager object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-12
         */
        virtual ~FITKVirtualTopoManager();
        /**
         * @brief 清空全部联系
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-12
         */
        void clear();

        /**
         * @brief       清空根节点数据。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-10-20
         */
        void clearRootTopos();

        /**
         * @brief 设置根对象（清除历史根节点数据）
         * @param[]  topoObj        My Param doc
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-12
         */
        void setRootObj(FITKAbsVirtualTopo* topoObj);
        /**
         * @brief       追加根节点
         * @param[in]   topoObj
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-11-12
         */
        void addRootObj(FITKAbsVirtualTopo* topoObj);
        /**
         * @brief 获取根对象
         * @param   index: 根节点索引（默认第0个）[缺省]
         * @return FITKAbsVirtualTopo*
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-12
         */
        FITKAbsVirtualTopo* getRootObj(int index = 0);

        /**
         * @brief       获取根节点形状数量。
         * @return      根节点数量
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-11-12
         */
        int getNumberOfRootObjs();

        /**
         * @brief 添加虚拟拓扑对象
         * @param[i]  tobj           虚拓扑对象
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-12
         */
        void appendVirtualTopoObj(FITKAbsVirtualTopo* tobj);
        /**
         * @brief 添加虚拟拓扑对象
         * @param[i]  tobj           虚拓扑对象
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-12
         */
        void appendVirtualTopoObj(FITKAbsVirtualTopo* tobj, Interface::FITKGeoEnum::VTopoShapeType st);

        /**
         * @brief 添加虚拟拓扑对象
         * @param[i]  tobj           虚拓扑对象
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-12
         */
        void appendVirtualTopoObjs(const QList< FITKAbsVirtualTopo*>& tobj);
        /**
         * @brief 添加虚拟拓扑对象
         * @param[i]  st             对象类型
         * @param[i]  tobj           虚拓扑对象
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-12
         */
        void appendVirtualTopoObjs(Interface::FITKGeoEnum::VTopoShapeType st, const QList<Core::FITKAbstractDataObject*>& tobj);

        /**
        * @brief 根据真实的几何形状获取虚拟拓扑
        * @param[i]  shape          真实的几何形状，例如OCC为TopoDS_Shape
        * @param[i]  type           类型
        * @return FITKAbsVirtualTopo*
        * @author libaojun (libaojunqd@foxmail.com)
        * @date 2024-08-09
        */
        FITKAbsVirtualTopo* getVirtualTopo(void* shape, FITKGeoEnum::VTopoShapeType type);
        /**
         * @brief 获取type类型的虚拟拓扑管理器
         * @param[i]  type           虚拓扑类型
         * @return FITKShapeVirtualTopoManager*
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-12
         */
        FITKShapeVirtualTopoManager* getShapeVirtualTopoManager(FITKGeoEnum::VTopoShapeType type);
        /**
         * @brief 获取类型为type的虚拟拓扑数量
         * @param[i]  type           虚拓扑类型
         * @return int
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-12
         */
        int getShapeVirtualTopoCount(FITKGeoEnum::VTopoShapeType type);
        /**
         * @brief 获取全部类型的子对象数量
         * @return int
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-12
         */
        int getAllShapeVirtualTopoCount();

        /**
         * @brief       获取当前形状所有形状类型。
         * @return      形状类型列表
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-10
         */
        QList<FITKGeoEnum::VTopoShapeType> getTopoTypes();

        /**
         * @brief       内部方法 - 给定需要查询的虚拓扑数据ID以及虚拓扑类型向下查找虚拓扑对象。
         * @param[in]   destId：需要查询的数据ID
         * @param[in]   srcType：类型阈值
         * @return      查询结果
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-11
         */
        FITKAbsVirtualTopo* findDownInternal(int destId, FITKGeoEnum::VTopoShapeType srcType);

        /**
         * @brief       根据ID查询虚拓扑对象。
         * @param[in]   destId：需要查询的数据ID
         * @return      查询结果
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-03
         */
        FITKAbsVirtualTopo* findTopo(int destId);

    private:
        /**
         * @brief 线程锁
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-12
         */
        QMutex _mutex;
        /**
         * @brief 根节点id列表
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-12
         */
        QList<int> _rootTopoObjs{};
        /**
         * @brief 子对象管理器
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-12
         */
        QHash<FITKGeoEnum::VTopoShapeType, FITKShapeVirtualTopoManager*> _shapeVirtualTopos{};
    };
}


#endif
