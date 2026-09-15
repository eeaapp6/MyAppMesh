/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef __FITK_ABS_GEO_MODEL_FROM_MESH_H__
#define __FITK_ABS_GEO_MODEL_FROM_MESH_H__

/**
 * @file FITKAbsGeoModelFromMesh.h
 * @brief 从网格数据生成几何模型的抽象基类
 * @details 提供从网格节点和单元构建几何模型的基础功能
 * @author libaojun
 * @date 2025-05-09
 */

#include "FITKAbsGeoCommand.h"
#include "FITKGeoEnum.h"
#include <QList>
#include <QHash>
#include <QString>

namespace Core
{
    class FITKNode;
}

namespace Interface
{
    class FITKAbstractElement;
 
    /**
     * @brief 从网格生成几何模型的抽象基类
     * @details 此类提供从网格数据构建几何模型的通用功能
     * @author libaojun
     * @date 2025-05-09
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelFromMesh : public FITKAbsGeoCommand
    {
    public:
    /**
         * @brief 网格节点坐标结构体
         * @details 存储网格节点的三维坐标信息
         */
        struct MeshNodeCoordinates
        {
            double x{0};  ///< X坐标
            double y{0};  ///< Y坐标
            double z{0};  ///< Z坐标
        };
        
        /**
         * @brief 网格三角形结构体
         * @details 存储三角形的节点索引和法向量信息
         */
        struct MeshTri
        {
            int node1{0};         ///< 第一个节点索引
            int node2{0};         ///< 第二个节点索引
            int node3{0};         ///< 第三个节点索引
            double normal[3]={0,0,0}; ///< 法向量
        };

    public:
        /**
         * @brief 默认构造函数
         */
        explicit FITKAbsGeoModelFromMesh() = default;
        
        /**
         * @brief 纯虚析构函数
         */
        virtual ~FITKAbsGeoModelFromMesh() = 0;

        /**
         * @brief 获取几何命令类型
         * @return 几何命令类型枚举值
         * @override
         */
        virtual FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;
        
        /**
         * @brief 设置网格数据
         * @param meshNodes 网格节点列表
         * @param meshElements 网格单元列表
         * @details 从提供的网格节点和单元数据构建几何模型
         */
        void setMesh(const QList<Core::FITKNode*>& meshNodes, const QList<FITKAbstractElement*> & meshElements); 

        /**
         * @brief 将几何模型导出为STL文件
         * @param fileName STL文件的保存路径
         * @return 操作是否成功
         * @details 将构建的几何模型保存为标准STL格式文件
         */
        bool writeSTL(const QString& fileName) const;
     
        /**
         * @brief 获取网格节点坐标
         * @return 网格节点坐标的哈希表
         */
        const QHash<int, MeshNodeCoordinates> & getMeshNodeCoordinates() const;
        /**
         * @brief 获取网格三角形列表
         * @return 网格三角形的列表
         */
        const QList<MeshTri> & getMeshTriangles() const;

    private:
        /**
         * @brief 计算三角形法向量
         * @param tri 待计算的三角形
         * @details 根据三角形的三个顶点计算其法向量
         */
        void calTriNormal(MeshTri& tri);

    protected:
        QHash<int,MeshNodeCoordinates> m_meshNodeCoordinates{}; ///< Mesh节点坐标
        QList<MeshTri> m_meshTriangles{}; ///< Mesh三角形
    };
}

#endif // __FITK_ABS_GEO_MODEL_FROM_MESH_H__