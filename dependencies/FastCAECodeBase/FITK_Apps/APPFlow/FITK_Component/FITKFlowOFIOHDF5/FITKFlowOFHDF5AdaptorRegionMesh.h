/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKFlowOFHDF5AdaptorRegionMesh.h
 * @brief  hdf5区域网格读写接口
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-09-10
 *********************************************************************/
#ifndef __FITKFLOWOFHDF5ADAPTORREGIONMESH_R_H__
#define __FITKFLOWOFHDF5ADAPTORREGIONMESH_R_H__

#include "FITKFlowOFIOHDF5API.h"
#include "FITKFlowOFHDF5AbstractAdaptor.h"
#include "FITK_Kernel/FITKCore/FITKCoreMacros.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

 //前置声明
ForwardDeclarNS(Interface, FITKAbstractRegionMeshSize)

namespace IO
{
    /**
     * @brief HDF5 RegionMesh读写适配器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date 2024-05-30
     */
    class FITKFLOWOFIOHDF5API FITKFlowOFHDF5AdaptorRegionMesh : public FITKFlowOFHDF5AbstractAdaptor
    {
    public:
        explicit FITKFlowOFHDF5AdaptorRegionMesh() = default;
        ~FITKFlowOFHDF5AdaptorRegionMesh() = default;

        /**
         * @brief 获取适配器数据类型名
         * @return 适配器数据类型名
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-05-30
         */
        QString getAdaptorClass() override;

        /**
         * @brief 适配器读取
         * @return 状态 true成功， false失败
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-05-30
         */
        bool adaptR() override;

        /**
         * @brief 适配器写出
         * @return 状态 true成功， false失败
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-05-30
         */
        bool adaptW() override;

    private:
        /**
         * @brief    写出区域网格基类数据
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-09
         */
        bool regionMeshSizeAbstractW();
        /**
         * @brief    读取区域网格基类数据
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-09
         */
        bool regionMeshSizeAbstractR();
        /**
         * @brief    写出区域几何
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-09
         */
        bool regionGeometryW();
        /**
         * @brief    读取区域几何
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-09
         */
        bool regionGeometryR();
        /**
         * @brief    写出盒区域
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-09
         */
        bool regionBoxW();
        /**
         * @brief    读取盒区域
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-09
         */
        bool regionBoxR();
        /**
         * @brief    写出圆柱区域
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-09
         */
        bool regionCylinderW();
        /**
         * @brief    读取圆柱区域
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-09
         */
        bool regionCylinderR();
        /**
         * @brief    写出球区域
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-09
         */
        bool regionSphereW();
        /**
         * @brief    读取球区域
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-09
         */
        bool regionSphereR();


    private:
        /**
         * @brief  几何数据对象
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-09
         */
        Interface::FITKAbstractRegionMeshSize* _regionMeshData{};
    };

    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbstractRegionMeshSize, FITKFlowOFHDF5AdaptorRegionMesh)
}


#endif // !1

