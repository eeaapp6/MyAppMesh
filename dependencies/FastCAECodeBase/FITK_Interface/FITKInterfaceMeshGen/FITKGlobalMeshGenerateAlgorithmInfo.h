/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKGlobalMeshGenerateAlgorithmInfo.h
 * @brief  全局网格生成的算法信息
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-10-15
 *********************************************************************/
#ifndef __FITK_GLOBAL_MeshGenerate_AlgorithmInfo_H__
#define __FITK_GLOBAL_MeshGenerate_AlgorithmInfo_H__

#include "FITKInterfaceMeshGenAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITK_Kernel/FITKCore/FITKVarientParams.h"

namespace Interface
{
    /**
     * @brief 全局网格生成的算法信息
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-07-17
     */
    class FITKInterfaceMeshGenAPI FITKGlobalMeshGenerateAlgorithmInfo :
        public Core::FITKAbstractNDataObject, public Core::FITKVarientParams
    {
        Q_OBJECT
    public:
        /**
        * @brief  算法2D
        * @author liuzhonghua (liuzhonghuaszch@163.com)
        * @date   2024-09-02
        */
        enum Algorithm2D
        {
            Automatic = 0,
            MeshAdapt = 1,
            Delaunay2D = 2,
            FrontalDelaunay = 3,
            BAMG = 4,
            FrontalDelaunayForQuads = 5,
            PackingOfParallelograms = 6,
            Algorithm2DUserDef1,
            Algorithm2DUserDef2,
        };
        Q_ENUM(Algorithm2D);
        /**
         * @brief  算法3D
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-02
         */
        enum Algorithm3D
        {
            Delaunay3D = 0,
            Frontal = 1,
            HXT = 2,
            MMG3D = 3,
            Algorithm3DUserDef1,
            Algorithm3DUserDef2,
        };
        Q_ENUM(Algorithm3D);
        /**
         * @brief  重组算法2D
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-10-14
         */
        enum ReAlgorithm2D
        {
            Simple = 0,
            Blossom = 1,
            SimpleFullQuad = 2,
            BlossomFullQuad = 3,
            ReAlgorithm2DUserDef1,
            ReAlgorithm2DUserDef2,
        };
        Q_ENUM(ReAlgorithm2D);
        /**
         * @brief  细化算法
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-10-14
         */
        enum SubdivisionAlgorithm
        {
            None = 0,
            AllQuads = 1,
            AllHexas = 2,
            SAUserDef1,
            SAUserDef2,
        };
        Q_ENUM(SubdivisionAlgorithm);
        /**
         * @brief Construct a new FITKGlobalMeshSizeInfo object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-17
         */
        explicit FITKGlobalMeshGenerateAlgorithmInfo();
        /**
         * @brief Destroy the FITKGlobalMeshSizeInfo object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-17
         */
        virtual ~FITKGlobalMeshGenerateAlgorithmInfo() = default;
        /**
         * @brief    设置网格生成的最高维度
         * @param[i] dim 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-10-15
         */
        void setMeshGenerateDimension(int dim);
        /**
         * @brief    获取网格生成的最高维度
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-10-15
         */
        int getMeshGenerateDimension();
        /**
         * @brief    设置2DAlgorithm
         * @param[i] algorithm2D 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-10-14
         */
        void set2DAlgorithm(Algorithm2D algorithm2D);
        /**
         * @brief    获取2DAlgorithm
         * @return   Algorithm2D
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-10-14
         */
        Algorithm2D get2DAlgorithm();
        /**
         * @brief    设置3DAlgorithm
         * @param[i] algorithm3D 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-10-14
         */
        void set3DAlgorithm(Algorithm3D algorithm3D);
        /**
         * @brief    获取3DAlgorithm
         * @return   Algorithm3D
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-10-14
         */
        Algorithm3D get3DAlgorithm();
        /**
         * @brief    设置2d重组算法
         * @param[i] reAlgorithm2D 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-10-14
         */
        void set2DRecombinationAlgorithm(ReAlgorithm2D reAlgorithm2D);
        /**
         * @brief    获取2d重组算法
         * @return   ReAlgorithm2D
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-10-14
         */
        ReAlgorithm2D get2DRecombinationAlgorithm();
        /**
         * @brief    设置重新组合所有三角形网格
         * @param[i] reTriMesh 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-10-14
         */
        void setRecombineAllTriMeshes(bool reTriMesh);
        /**
         * @brief    是否重新组合所有三角形网格
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-10-14
         */
        bool getRecombineAllTriMeshes();
        /**
         * @brief    设置细分算法
         * @param[i] subdivisionAlgorithm 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-10-14
         */
        void setSubdivisionAlgorithm(SubdivisionAlgorithm subdivisionAlgorithm);
        /**
         * @brief    获取细分算法
         * @return   SubdivisionAlgorithm
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-10-14
         */
        SubdivisionAlgorithm getSubdivisionAlgorithm();
        /**
         * @brief    设置Smoothing Steps
         * @param[i] smoothStep 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-10-14
         */
        void setSmoothStep(int smoothStep);
        /**
         * @brief    获取Smoothing Steps
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-10-14
         */
        int getSmoothStep();
        /**
         * @brief    设置单元阶数
         * @param[i] eleOrader 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-10-14
         */
        void setElementOrder(int eleOrader);
        /**
         * @brief    获取单元阶数
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-10-14
         */
        int getElementOrder();
        /**
         * @brief    设置不完整的单元
         * @param[i] useIncElem 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-10-14
         */
        void setUseIncElem(bool useIncElem);
        /**
         * @brief    是否设置不完整的单元
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-10-14
         */
        bool getUseIncElem();
        /**
         * @brief    设置几何缝合（这是在导出几何文件之前，软件进行几何缝合之后，在进行网格划分）
         * @param[i] isStitch 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-11
         */
        void setGeometryStitch(bool isStitch);
        /**
         * @brief    是否设置几何缝合
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-11
         */
        bool getGeometryStitch();
        /**
         * @brief    设置几何缝合容差
         * @param[i] tol 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-11
         */
        void setGeometryStitchTolerance(double tol);
        /**
         * @brief    获取几何缝合容差
         * @return   double
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-11
         */
        double getGeometryStitchTolerance();
        /**
         * @brief    设置几何连贯
         * @param[i] isCoherence
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-10-14
         */
        void setGeometryCoherence(bool isCoherence);
        /**
         * @brief    是否设置几何连贯
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-10-14
         */
        bool getGeometryCoherence();

    protected:

    };
}


#endif
