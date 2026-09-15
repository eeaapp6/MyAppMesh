/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKInterfaceHDF5AdaptorMesh.h
 * @brief 读写HDF5文件的Mesh类
 * @author fulipeng (fulipengqd@yeah.net)
 * @date 2024-05-29
 *
 */
#ifndef  _FITKINTERFACEHDF5ADAPTORMESH_H__
#define  _FITKINTERFACEHDF5ADAPTORMESH_H__

#include "FITKInterfaceIOHDF5API.h"
#include "FITKAbstractHDF5Adaptor.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

 //前置声明
ForwardDeclarNS(Interface, FITKModelSurface)
ForwardDeclarNS(Interface, FITKUnstructuredMesh)
ForwardDeclarNS(Interface, FITKUnstructuredMeshVTK)

namespace IO
{
    /**
     * @brief HDF5 Mesh 读写适配器
     * @author fulipeng (fulipengqd@yeah.net)
     * @date 2024-05-29
     */
    class FITKINTERFACEHDF5IOAPI FITKInterfaceHDF5AdaptorMesh : public FITKAbstractHDF5Adaptor
    {
    public:
        explicit FITKInterfaceHDF5AdaptorMesh() = default;
        virtual ~FITKInterfaceHDF5AdaptorMesh() = default;

        /**
         * @brief 获取适配器数据类型名
         * @return 适配器数据类型名
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-05-29
         */
        QString getAdaptorClass() override;

        /**
         * @brief 适配器读取
         * @return 状态 true成功， false失败
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-05-29
         */
        bool adaptR() override;

        /**
         * @brief 适配器写出
         * @return 状态 true成功， false失败
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-05-29
         */
        bool adaptW() override;

    protected:

        /**
         * @brief 读取网格节点
         * @param mesh 网格数据结构
         * @param h5Group HDF5块结构
         * @return 是否写出成功
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-05-29
         */
        virtual bool readNode(Interface::FITKUnstructuredMesh* mesh, H5::Group& h5Group);

        /**
         * @brief 读取单元节点
         * @param mesh 网格数据结构
         * @param h5Group HDF5块结构
         * @return 是否写出成功
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-05-29
         */
        virtual bool readElement(Interface::FITKUnstructuredMesh* mesh, H5::Group& h5Group);


        /**
         * @brief 写出网格节点
         * @param mesh 网格数据结构
         * @param h5Group HDF5块结构
         * @return 是否写出成功
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-05-29
         */
        virtual bool writeNode(Interface::FITKUnstructuredMesh* mesh, H5::Group& h5Group);

        /**
         * @brief 写出单元节点
         * @param mesh 网格数据结构
         * @param h5Group HDF5块结构
         * @return 是否写出成功
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-05-29
         */
        virtual bool writeElement(Interface::FITKUnstructuredMesh* mesh, H5::Group& h5Group);

        /**
         * @brief 提取单元信息 网格类型有变化需要在子类中重写
         * @param mesh 网格数据
         * @return 提取后的信息
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-06-03
         */
        virtual QMultiHash<QString, int> extractElementInformation(Interface::FITKUnstructuredMesh* mesh);

    };

    //Register2FITKIOAdaptorFactory(HDF5, Interface::FITKModelSet, FITKInterfaceHDF5AdaptorSet)
}

#endif