/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKGeometryMeshVS.h
 * @brief 几何模型渲染对象
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-08-31
 *
 */
#ifndef _FITK_GEOMETRY_MESH_VS_H___
#define _FITK_GEOMETRY_MESH_VS_H___

#include "FITKInterfaceGeometryAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"
#include <QHashIterator>

namespace Interface
{
    class FITKGeoMeshVSPt;
    class FITKGeoMeshVSEdgeEntity;
    class FITKGeoMeshVSFaceEntity;
    /**
     * @brief 几何模型渲染网格对象
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-08-31
     */
    class FITKInterfaceGeometryAPI FITKGeometryMeshVS :
        public Core::FITKAbstractDataObject
    {
    public:
        /**
         * @brief Construct a new FITKGeometryMeshVS object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-31
         */
        explicit FITKGeometryMeshVS() = default;
        /**
         * @brief Destroy the FITKGeometryMeshVS object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-31
         */
        virtual ~FITKGeometryMeshVS();
        /**
         * @brief 插入点对象
         * @param[i]  id             虚拓扑id
         * @param[i]  pt             点渲染对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-31
         */
        void insertPoint(int id, FITKGeoMeshVSPt* pt);
        /**
         * @brief 插入边对象
         * @param[i]  id             边的虚拓扑id
         * @param[i]  pt             边的网格
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-31
         */
        void insertEdge(int id, FITKGeoMeshVSEdgeEntity* pt);
        /**
         * @brief 插入面对象
         * @param[i]  id             面的虚拓扑id
         * @param[i]  pt             面的网格
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-31
         */
        void insertFace(int id, FITKGeoMeshVSFaceEntity* pt);
        /**
         * @brief 获取点的渲染数据
         * @return const QHash<int, FITKGeoMeshVSPt*>& int 为虚拓扑id
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-31
         */
        const QHash<int, FITKGeoMeshVSPt*> & getPointItems() const;
        /**
         * @brief 获取边的渲染数据
         * @return const QHash<int, FITKGeoMeshVSPt*>& int 为虚拓扑id
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-31
         */
        const QHash<int, FITKGeoMeshVSEdgeEntity*> & getEdgeItems() const;
        /**
         * @brief  获取面的渲染数据
         * @return const QHash<int, FITKGeoMeshVSFaceEntity*>&  int 为虚拓扑id
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-31
         */
        const QHash<int, FITKGeoMeshVSFaceEntity*> & getFaceItems() const;
        /**
         * @brief 清空对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-31
         */
        void clear();
        /**
         * @brief 是否有效，所有对象均为空是无效状态
         * @return true
         * @return false
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-31
         */
        bool isValid();

    private:
        /**
         * @brief 点渲染对象 键为虚拓扑id
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-31
         */
        QHash<int, FITKGeoMeshVSPt*> _points{};
        /**
         * @brief 线渲染对象 键为虚拓扑id
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-31
         */
        QHash<int, FITKGeoMeshVSEdgeEntity*> _edges{};
        /**
         * @brief 面渲染对象 键为虚拓扑id
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-31
         */
        QHash<int, FITKGeoMeshVSFaceEntity*> _faces{};
    };
}


#endif
