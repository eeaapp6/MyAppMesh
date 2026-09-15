/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKRegionMeshSize.h
 * @brief 局部区域网格尺寸以及管理器声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-07-17
 *
 */
#ifndef _FITK_REGION_MESH_SIZE_H___
#define _FITK_REGION_MESH_SIZE_H___

#include "FITKInterfaceMeshGenAPI.h"
#include "FITKGlobalMeshSizeInfo.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"
#include <QHash>
#include <QList>

namespace Interface
{
    /**
     * @brief 局部区域尺寸抽象类
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-07-17
     */
    class FITKInterfaceMeshGenAPI FITKAbstractRegionMeshSize
        :public FITKGlobalMeshSizeInfo
    {
        Q_OBJECT
            FITKCLASS(Interface, FITKAbstractRegionMeshSize);
    public:
        /**
         * @brief 局部区域类型
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-17
         */
        enum RegionType
        {
            RegionNone,
            RigonFromFile,
            RigonGeom,
            RegionBox,
            RegionCylinder,
            RegionSphere,
            RegionUserDef1,
            RegionUserDef2,
            RegionUserDef3,
            RegionUserDef4,
            RegionUserDef5,
        };
        Q_ENUM(RegionType);

        /**
         * @brief 局部区域边界类型
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-17
         */
        enum BoundaryType
        {
            BTEmpty,
            BTPatch,
            BTWall,
            BTSymmetry,
            BTUDF1,
            BTUDF2,
            BTUDF3,
            BTUDF4,
            BTUDF5,
        };
        Q_ENUM(BoundaryType);

    public:
        /**
         * @brief Construct a new FITKAbstractRegionMeshSize object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-17
         */
        explicit FITKAbstractRegionMeshSize() = default;
        /**
         * @brief Destroy the FITKAbstractRegionMeshSize object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-17
         */
        virtual ~FITKAbstractRegionMeshSize() = 0;
        /**
         * @brief 获取区域类型
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-17
         * @return RegionType
         */
        virtual RegionType getRegionType() = 0;
        /**
         * @brief 设置离散数量
         * @param[i]  dim            维度 0-2
         * @param[i]  divNum         离散数量
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-17
         */
        void setDivision(int dim, int divNum);
        /**
         * @brief 设置离散比例
         * @param[i]  dim            维度 0-2
         * @param[i]  grad           离散比例
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-17
         */
        void setGrading(int dim, double grad);
        /**
         * @brief 获取离散数量
         * @param[i]  dim            维度 0-2
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-17
         * @return int
         */
        int getDivision(int dim);
        /**
         * @brief 获取离散比例
         * @param[i]  dim            维度 0-2
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-17
         * @return double
         */
        double getGrading(int dim);
        /**
         * @brief 设置边界类型
         * @param[i]  faceID         面ID
         * @param[i]  bt             边界类型
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-17
         */
        void insertBoundary(int faceID, BoundaryType bt);
        /**
         * @brief 获取边界类型
         * @param[i]  faceID         面ID
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-17
         * @return BoundaryType
         */
        BoundaryType getBoundary(int faceID);
        /**
         * @brief    设置面名称
         * @param[i] faceID 
         * @param[i] bn 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-09-03
         */
        void insertFaceName(int faceID, QString bn);
        /**
         * @brief    获取面名称
         * @param[i] faceID 
         * @return   QString
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-09-03
         */
        QString getFaceName(int faceID);
        /**
         * @brief    获取内部尺寸
         * @return   double
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-10-16
         */
        double getInternalSize();
        /**
         * @brief    设置内部尺寸
         * @param[i] vIn 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-10-16
         */
        void setInternalSize(double vIn);
        /**
         * @brief    获取外部尺寸
         * @return   double
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-10-16
         */
        double getExternalSize();
        /**
         * @brief    设置外部尺寸
         * @param[i] vOut 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-10-16
         */
        void setExternalSize(double vOut);
        /**
         * @brief    获取厚度
         * @return   double
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-10-16
         */
        double getThickness();
        /**
         * @brief    设置厚度
         * @param[i] thickness 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-10-16
         */
        void setThickness(double thickness);

    protected:
        /**
         * @brief 局部区域离散数量
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-17
         */
        int _division[3] = { 10,10,10 };
        /**
         * @brief 局部区域离散比例
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-17
         */
        double _grading[3] = { 1,1,1 };
        /**
         * @brief 局部区域边界类型，面ID-边界类型
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-17
         */
        QHash<int, BoundaryType> _boundarys{};
        /**
         * @brief  局部区域名称，用于划分网格边界组
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-03
         */
        QHash<int, QString> _facesName{};
    };

     /**
     * @brief 局部区域尺寸管理器
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-07-17
     */
    class FITKInterfaceMeshGenAPI FITKRegionMeshSizeManager :
        public Core::FITKAbstractDataManager<FITKAbstractRegionMeshSize>
    {
    public:
        /**
         * @brief Construct a new FITKRegionMeshSizeManager object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-17
         */
        explicit FITKRegionMeshSizeManager() = default;
        /**
         * @brief Destroy the FITKRegionMeshSizeManager object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-17
         */
        virtual ~FITKRegionMeshSizeManager() = default;
        /**
         * @brief 通过类型获取区域
         * @param[i]  rt             区域类型
         * @return QList<FITKAbstractRegionMeshSize*>
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-17
         */
        QList<FITKAbstractRegionMeshSize*> getRigonByType(FITKAbstractRegionMeshSize::RegionType rt);
    };
}


#endif
