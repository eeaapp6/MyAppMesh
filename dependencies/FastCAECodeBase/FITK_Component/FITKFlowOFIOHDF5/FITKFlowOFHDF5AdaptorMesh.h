/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKFlowOFHDF5AdaptorMesh.h
 * @brief  hdf5 网格数据读写接口
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-09-13
 *********************************************************************/
#ifndef __FITKFLOWOFHDF5ADAPTORMESH_R_H__
#define __FITKFLOWOFHDF5ADAPTORMESH_R_H__

#include "FITKFlowOFIOHDF5API.h"
#include "FITKFlowOFHDF5AbstractAdaptor.h"
#include "FITK_Kernel/FITKCore/FITKCoreMacros.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

 //前置声明
ForwardDeclarNS(Interface, FITKUnstructuredFluidMeshVTK)
ForwardDeclarNS(Interface, FITKBoundaryMeshVTKManager)
ForwardDeclarNS(Interface, FITKUnstructuredMesh)
ForwardDeclarNS(Interface, FITKUnstructuredMeshVTK)
ForwardDeclarNS(Interface, FITKElementList)
ForwardDeclarNS(Interface, FITKNodeList)
ForwardDeclarNS(Interface, FITKFluidRegionsMesh)
ForwardDeclarNS(H5, Group)

namespace IO
{
    /**
     * @brief HDF5 Geometry 读写适配器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date 2024-05-30
     */
    class FITKFLOWOFIOHDF5API FITKFlowOFHDF5AdaptorMesh : public FITKFlowOFHDF5AbstractAdaptor
    {
    public:
        explicit FITKFlowOFHDF5AdaptorMesh() = default;
        ~FITKFlowOFHDF5AdaptorMesh() = default;

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
         * @brief  区域网格写出
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-06-09
         */
        bool regionMeshW(H5::Group& h5Group, Interface::FITKFluidRegionsMesh* regionMesh);
        /**
         * @brief  区域网格读取
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-06-09
         */
        bool regionMeshR(H5::Group& h5Group, Interface::FITKFluidRegionsMesh* regionMesh);
        /**
         * @brief  流体网格写出
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-06-09
         */
        bool fieldMeshW(H5::Group& h5Group, Interface::FITKUnstructuredMeshVTK* fieldMesh);
        /**
         * @brief  流体网格读取
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-06-09
         */
        bool fieldMeshR(H5::Group& h5Group, Interface::FITKUnstructuredMeshVTK* fieldMesh);
        /**
         * @brief  边界网格写出
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-06-09
         */
        bool boundaryMeshW(H5::Group& h5Group, Interface::FITKBoundaryMeshVTKManager* boundaryMeshMgr);
        /**
         * @brief  边界网格读取
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-06-09
         */
        bool boundaryMeshR(H5::Group& h5Group, Interface::FITKBoundaryMeshVTKManager* boundaryMeshMgr, Interface::FITKUnstructuredMeshVTK* fieldMesh);

        /**
         * @brief 读取网格节点
         * @param mesh 网格数据结构
         * @param h5Group HDF5块结构
         * @return 是否写出成功
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-06-09
         */
        bool readNode(Interface::FITKNodeList* mesh, H5::Group& h5Group);

        /**
         * @brief 读取单元节点
         * @param mesh 网格数据结构
         * @param h5Group HDF5块结构
         * @return 是否写出成功
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-06-09
         */
        bool readElement(Interface::FITKElementList* mesh, H5::Group& h5Group);


        /**
         * @brief 写出网格节点
         * @param mesh 网格数据结构
         * @param h5Group HDF5块结构
         * @return 是否写出成功
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-06-09
         */
        bool writeNode(Interface::FITKNodeList* mesh, H5::Group& h5Group);

        /**
         * @brief 写出单元节点
         * @param mesh 网格数据结构
         * @param h5Group HDF5块结构
         * @return 是否写出成功
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-06-09
         */
        bool writeElement(Interface::FITKElementList* mesh, H5::Group& h5Group);

        /**
         * @brief 获取数据集的行列数
         * @return true，读取成功；false，读取失败。
         * @param h5group 要读取的group
         * @param name 属性名称
         * @param n 属性行数
         * @param m 属性列数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-06-09
         */
        bool getDataSetDim(H5::Group& h5group, std::string name, int& n, int& m);

    private:
        /**
         * @brief  网格数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-09
         */
        Interface::FITKUnstructuredFluidMeshVTK* _unstructuredMesh{};
    };

    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKUnstructuredFluidMeshVTK, FITKFlowOFHDF5AdaptorMesh)
}


#endif // !1

