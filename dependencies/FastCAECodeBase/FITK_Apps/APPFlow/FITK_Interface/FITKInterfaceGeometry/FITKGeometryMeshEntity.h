/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKGeometryMeshEntity.h
 * @brief  几何显示对象实体声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-08-04
 *
 */
#ifndef _FITKGEOMETRY_MESHVS_ENTITY_H___
#define _FITKGEOMETRY_MESHVS_ENTITY_H___

#include "FITKInterfaceGeometryAPI.h"
#include <QList>

namespace Interface
{
    /**
     * @brief 三维点坐标
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-08-04
     */
    class FITKInterfaceGeometryAPI FITKGeoMeshVSPt
    {
    public:
        /**
         * @brief Construct a new FITKGeoMeshVSPt object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-04
         */
        FITKGeoMeshVSPt() = default;
        /**
         * @brief Construct a new FITKGeoMeshVSPt object
         * @param[i]  x y z             坐标
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-04
         */
        FITKGeoMeshVSPt(const float& x, const float& y, const float& z);
        /**
         * @brief Construct a new FITKGeoMeshVSPt object
         * @param[i]  coor           坐标
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-04
         */
        FITKGeoMeshVSPt(double* coor);
        /**
         * @brief Destroy the FITKGeoMeshVSPt object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-04
         */
        ~FITKGeoMeshVSPt() = default;
        /**
         * @brief 设置xyz坐标
         * @param[i]  x y z         坐标
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-04
         */
        void setXYZ(const float& x, const float& y, const float& z);
        /**
         * @brief x坐标
         * @return float
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-04
         */
        float x();
        /**
         * @brief y坐标
         * @return float
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-04
         */
        float y();
        /**
         * @brief z坐标
         * @return float
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-04
         */
        float z();

    private:
        /**
         * @brief 坐标值
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-04
         */
        float _coor[3] = { 0,0,0 };

    };

    typedef FITKGeoMeshVSPt FITKGeoMeshTriNor;
    /**
     * @brief 三角形
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-08-04
     */
    class FITKInterfaceGeometryAPI FITKGeoMeshVSTri
    {
    public:
        /**
         * @brief Construct a new FITKGeoMeshVSTri object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-04
         */
        FITKGeoMeshVSTri(const int p1, const int p2, const int p3);
        /**
         * @brief Destroy the FITKGeoMeshVSTri object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-04
         */
        ~FITKGeoMeshVSTri() = default;
        /**
         * @brief 获取节点
         * @param[i]  index   节点索引 有效值0-2
         * @return int
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-04
         */
        int getNode(const int index);
        /**
         * @brief 获取节点
         * @return int*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-04
         */
        const int* getNode() const;

    private:
        /**
         * @brief 三个节点
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-04
         */
        int _p[3] = { -1,-1,-1 };

    };
    /**
     * @brief 几何面三角形
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-08-04
     */

     /**
      * @brief 几何边
      * @author LiBaojun (libaojunqd@foxmail.com)
      * @date 2024-08-04
      */
    class FITKInterfaceGeometryAPI FITKGeoMeshVSEdgeEntity
    {
    public:
        /**
         * @brief Construct a new FITKGeoMeshVSEdgeEntity object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-04
         */
        explicit FITKGeoMeshVSEdgeEntity() = default;
        /**
         * @brief Destroy the FITKGeoMeshVSEdgeEntity object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-04
         */
        virtual ~FITKGeoMeshVSEdgeEntity();
        /**
         * @brief 清空边
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-04
         */
        void clear();
        /**
         * @brief 添加点
         * @param[i]  pt     点
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-04
         */
        void appendPoint(FITKGeoMeshVSPt* pt);
        /**
         * @brief 获取点数量
         * @return int
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-04
         */
        int getPointCount();
        /**
         * @brief 获取点列表
         * @return const QList<FITKGeoMeshVSPt*>&
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-04
         */
        const QList<FITKGeoMeshVSPt*>& getPoints() const;

    private:
        /**
         * @brief 点列表
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-04
         */
        QList<FITKGeoMeshVSPt*> _ptList{};

    };

    class FITKInterfaceGeometryAPI FITKGeoMeshVSFaceEntity :
        public FITKGeoMeshVSEdgeEntity
    {
    public:
        /**
         * @brief Construct a new FITKGeoMeshVSFaceEntity object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-04
         */
        explicit FITKGeoMeshVSFaceEntity() = default;
        /**
         * @brief Destroy the FITKGeoMeshVSFaceEntity object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-04
         */
        virtual ~FITKGeoMeshVSFaceEntity();
        /**
         * @brief 清空三角形
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-04
         */
        void clear();
        /**
         * @brief 添加三角形
         * @param[i]  tri    三角形
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-04
         */
        void appendMeshTri(FITKGeoMeshVSTri* tri);
        /**
         * @brief 获取三角形数量
         * @return int
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-04
         */
        int getTriangleCount();
        /**
         * @brief 获取三角形列表
         * @return FITKGeoMeshVSTri*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-04
         */
        const QList<FITKGeoMeshVSTri*>& getTriangles() const;

    private:
        /**
         * @brief 三角形列表
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-04
         */
        QList<FITKGeoMeshVSTri*> _triList{};

    };
}


#endif
