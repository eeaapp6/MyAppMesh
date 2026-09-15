/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKInterfaceHDF5AdaptorAssembly.h
 * @brief 读写HDF5文件的ASSembly类
 * @author fulipeng (fulipengqd@yeah.net)
 * @date 2024-05-31
 *
 */
#ifndef  _FITKINTERFACEHDF5ADAPTORASSEMBLY_H__
#define  _FITKINTERFACEHDF5ADAPTORASSEMBLY_H__

#include "FITKInterfaceIOHDF5API.h"
#include "FITKAbstractHDF5Adaptor.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

 //前置声明
ForwardDeclarNS(Interface, FITKAssembly)
ForwardDeclarNS(Interface, FITKModelSet)
ForwardDeclarNS(Interface, FITKMeshSurface)
ForwardDeclarNS(Interface, FITKModelSurface)
ForwardDeclarNS(Interface, FITKComponentManager)
ForwardDeclarNS(Interface, FITKAbstractAssInstance)
ForwardDeclarNS(Interface, FITKComponentManager)

namespace IO
{
    /**
     * @brief HDF5 Assembly 读写适配器
     * @author fulipeng (fulipengqd@yeah.net)
     * @date 2024-05-31
     */
    class FITKINTERFACEHDF5IOAPI FITKInterfaceHDF5AdaptorAssembly : public FITKAbstractHDF5Adaptor
    {
    public:
        explicit FITKInterfaceHDF5AdaptorAssembly() = default;
        virtual ~FITKInterfaceHDF5AdaptorAssembly() = default;

        /**
         * @brief 获取适配器数据类型名
         * @return 适配器数据类型名
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-05-31
         */
        QString getAdaptorClass() override;

        /**
         * @brief 适配器读取
         * @return 状态 true成功， false失败
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-05-31
         */
        bool adaptR() override;

        /**
         * @brief 适配器写出
         * @return 状态 true成功， false失败
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-05-31
         */
        bool adaptW() override;

    protected:

        /**
         * @brief 读取装配
         * @param assembly 装配
         * @param h5Group 数据块
         * @return 状态 true成功， false失败
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-06-19
         */
        virtual bool readAssembly(Interface::FITKAssembly* assembly, H5::Group& h5Group);

        /**
         * @brief 写出装配
         * @param assembly 装配
         * @param h5Group 数据块
         * @return 状态 true成功， false失败
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-06-19
         */
        virtual bool writeAssembly(Interface::FITKAssembly* assembly, H5::Group& h5Group);

    private:
        /**
         * @brief 读取集合与表面
         * @param setSurfManager 集合与表面管理器
         * @param h5Group 数据块
         * @return 状态 true成功， false失败
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-06-19
         */
        bool readSetSurf(Interface::FITKComponentManager* setSurfManager, H5::Group& h5Group);

        /**
         * @brief 读取集合
         * @param set 集合
         * @param h5Group 数据块
         * @return 状态 true成功， false失败
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-06-19
         */
        bool readSet(Interface::FITKModelSet * set, H5::Group& h5Group);

        /**
         * @brief 读取表面
         * @param surface 表面
         * @param h5Group 数据块
         * @return 状态 true成功， false失败
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-06-19
         */
        bool readSurface(Interface::FITKModelSurface* surface, H5::Group& h5Group);

        /**
         * @brief 读取实例
         * @param h5Group 数据块
         * @return 状态 true成功， false失败
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-06-19
         */
        bool readInstance(H5::Group& h5Group);

        /**
         * @brief 读取实例
         * @param surface 表面
         * @param h5Group 数据块
         * @return 状态 true成功， false失败
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-06-19
         */
        bool readInstanceTR(Interface::FITKAbstractAssInstance* assInstance, H5::Group& h5Group);
        bool readInstanceM4(Interface::FITKAbstractAssInstance* assInstance, H5::Group& h5Group);

        bool writeSet(Interface::FITKModelSet * set, H5::Group& h5Group);

        /**
         * @brief 写出集合
         * @param surface 表面
         * @param h5Group 数据块
         * @return 状态 true成功， false失败
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-06-19
         */
        bool writeSurface(Interface::FITKModelSurface* surface, H5::Group& h5Group);

        /**
         * @brief 写出集合与表面
         * @param setSurfManager 集合与表面管理器
         * @param h5Group 数据块
         * @return 状态 true成功， false失败
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-06-19
         */
        bool writeSetSurf(Interface::FITKComponentManager* setSurfManager, H5::Group& h5Group);

        /**
         * @brief 写出实例
         * @param assInstance 实例
         * @param h5Group 数据块
         * @return 状态 true成功， false失败
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-06-19
         */
        bool writeInstance(Interface::FITKAbstractAssInstance* assInstance, H5::Group& h5Group);

        /**
         * @brief 写出实例
         * @param assInstance 实例
         * @param h5Group 数据块
         * @return 状态 true成功， false失败
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-06-19
         */
        bool writeInstanceTR(Interface::FITKAbstractAssInstance* assInstance, H5::Group& h5Group);

        ///**
        // * @brief 写出实例
        // * @param assInstance 实例
        // * @param h5Group 数据块
        // * @return 状态 true成功， false失败
        // * @author fulipeng (fulipengqd@yeah.net)
        // * @date 2024-06-19
        // */
        //bool writeInstanceM4(Interface::FITKAbstractAssInstance* assInstance, H5::Group& h5Group);

    };

    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAssembly, FITKInterfaceHDF5AdaptorAssembly)
}

#endif
