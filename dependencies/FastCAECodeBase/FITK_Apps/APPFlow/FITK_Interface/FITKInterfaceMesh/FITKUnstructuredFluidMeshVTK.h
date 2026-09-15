/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKUnstructuredFulidMeshVTK.h
 * @brief 非结构化流体网格声明，包含边界网格
 * @author libaojun (libaojunqd@foxmail.com)
 * @date 2024-07-24
 *
 */
#ifndef _FITKUNSTRUCTMESH_FLUID_H___
#define _FITKUNSTRUCTMESH_FLUID_H___

#include "FITKInterfaceMeshAPI.h"
#include "FITK_Interface/FITKInterfaceModel/FITKElementList.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include <QList>

class vtkUnstructuredGrid;

namespace Interface
{
    class FITKUnstructuredMeshVTK;
    class FITKBoundaryMeshVTKManager;

    class FITKInterfaceMeshAPI FITKFluidRegionsMesh : public Core::FITKAbstractNDataObject
    {
    public:
        /**
         * @brief 构造函数
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-07-24
         */
        explicit FITKFluidRegionsMesh();
        /**
         * @brief Destroy the FITKUnstructuredFluidMeshVTK object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-07-24
         */
        virtual ~FITKFluidRegionsMesh();

        /**
         * @brief 获取流体域网格
         * @return FITKUnstructuredMeshVTK*
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-07-24
         */
        FITKUnstructuredMeshVTK* getFieldMesh();

        /**
         * @brief 获取边界网格管理器
         * @return FITKBoundaryMeshVTKManager*
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-07-24
         */
        FITKBoundaryMeshVTKManager* getBoundaryMeshManager();

    private:
        /**
         * @brief 流体域网格
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-07-24
         */
        FITKUnstructuredMeshVTK* _fieldMesh{};
        /**
         * @brief 边界网格
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-07-24
         */
        FITKBoundaryMeshVTKManager* _boundaryMeshManager{};
    };

    /**
     * @brief 非结构化流体网格，包含边界网格
     * @author libaojun (libaojunqd@foxmail.com)
     * @date 2024-07-24
     */
    class FITKInterfaceMeshAPI FITKUnstructuredFluidMeshVTK : public Core::FITKAbstractNDataObject, public Core::FITKAbstractDataManager<FITKFluidRegionsMesh>
    {
    public:
        /**
         * @brief 构造函数
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-07-24
         */
        explicit FITKUnstructuredFluidMeshVTK();
        /**
         * @brief Destroy the FITKUnstructuredFluidMeshVTK object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-07-24
         */
        virtual ~FITKUnstructuredFluidMeshVTK();

        /**
         * @brief 获取流体域网格
         * @return FITKUnstructuredMeshVTK*
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-07-24
         */
        FITKUnstructuredMeshVTK* getFieldMesh(int index = 0);

        /**
         * @brief 获取边界网格管理器
         * @return FITKBoundaryMeshVTKManager*
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-07-24
         */
        FITKBoundaryMeshVTKManager* getBoundaryMeshManager(int index = 0);

        /**
         * @brief 清空网格 
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-07-26
         */
        void  clearMesh();

    };

    /**
     * @brief 边界网格
     * @author libaojun (libaojunqd@foxmail.com)
     * @date 2024-07-24
     */
    class FITKInterfaceMeshAPI FITKBoundaryMeshVTK :public Core::FITKAbstractNDataObject,
        public FITKElementList
    {
        FITKCLASS(Interface, FITKBoundaryMeshVTK);
    public:
        /**
         * @brief Construct a new FITKBoundaryMeshVTK object
         * @param[i]  fieldmesh      流体域网格，使用其节点
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-07-24
         */
        explicit FITKBoundaryMeshVTK(FITKUnstructuredMeshVTK* fieldmesh);
        /**
         * @brief Destroy the FITKBoundaryMeshVTK object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-07-24
         */
        virtual ~FITKBoundaryMeshVTK();

        /**
         * @brief 追加单元
         * @param[i]  element        单元指针
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-24
         */
        virtual void appendElement(FITKAbstractElement* element) override;

        /**
         * @brief       获取边界网格vtk数据集。
         * @return      数据集
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-07-24
         */
        vtkUnstructuredGrid* getGrid();

    private:
        /**
        * @brief 流体域网格
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-07-24
        */
        FITKUnstructuredMeshVTK* _fieldMesh{};
        /**
        * @brief VTK表示的网格
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-07-24
        */
        vtkUnstructuredGrid* _vtkDataSet{};
    };

     /**
     * @brief 边界网格管理器
     * @author libaojun (libaojunqd@foxmail.com)
     * @date 2024-07-24
     */
    class FITKInterfaceMeshAPI FITKBoundaryMeshVTKManager
        : public Core::FITKAbstractDataManager<FITKBoundaryMeshVTK>
    {
    public:
        /**
         * @brief Construct a new FITKBoundaryMeshVTKManager object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-07-24
         */
        explicit  FITKBoundaryMeshVTKManager() = default;
        /**
         * @brief Destroy the FITKBoundaryMeshVTKManager object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-07-24
         */
        virtual ~FITKBoundaryMeshVTKManager() = default;

    };
}


#endif
