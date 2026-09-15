/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKInterfaceHDF5AdaptorFiled.h
 * @brief 读写HDF5文件的Filed类
 * @author fulipeng (fulipengqd@yeah.net)
 * @date 2024-07-12
 *
 */
#ifndef  _FITKINTERFACEHDF5ADAPTORFILED_H__
#define  _FITKINTERFACEHDF5ADAPTORFILED_H__

#include "FITKInterfaceIOHDF5API.h"
#include "FITKAbstractHDF5Adaptor.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

 //前置声明
ForwardDeclarNS(Interface, FITKAbstractFiled)

namespace IO
{
    /**
     * @brief HDF5 Filed 读写适配器
     * @author fulipeng (fulipengqd@yeah.net)
     * @date 2024-07-12
     */
    class FITKINTERFACEHDF5IOAPI FITKInterfaceHDF5AdaptorFiled : public FITKAbstractHDF5Adaptor
    {
    public:
        explicit FITKInterfaceHDF5AdaptorFiled() = default;
        virtual ~FITKInterfaceHDF5AdaptorFiled() = default;

        /**
         * @brief 获取适配器数据类型名
         * @return 适配器数据类型名
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-07-12
         */
        QString getAdaptorClass() override;

        /**
         * @brief 适配器读取
         * @return 状态 true成功， false失败
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-07-12
         */
        bool adaptR() override;

        /**
         * @brief 适配器写出
         * @return 状态 true成功， false失败
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-07-12
         */
        bool adaptW() override;

        /**
         * @brief 读取场
         * @param filed 离散场或解析场
         * @param h5Group 数据块
         * @return 状态 true成功， false失败
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-07-12
         */
        bool readFiled(Interface::FITKAbstractFiled* filed, H5::Group & h5Group);

        /**
         * @brief 写出场
         * @param filed 离散场或解析场
         * @param h5Group 数据块
         * @return 状态 true成功， false失败
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-07-12
         */
        bool writeFiled(Interface::FITKAbstractFiled* filed, H5::Group & h5Group);

    private:

        /**
         * @brief 读取解析场
         * @param filed 解析场
         * @param h5Group 数据块
         * @return 状态 true成功， false失败
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-07-12
         */
        bool readAnalyticalExpressField(Interface::FITKAbstractFiled* filed, H5::Group & h5Group);

        /**
         * @brief 写出解析场
         * @param filed 解析场
         * @param h5Group 数据块
         * @return 状态 true成功， false失败
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-07-12
         */
        bool writeAnalyticalExpressField(Interface::FITKAbstractFiled* filed, H5::Group & h5Group);

        /**
         * @brief 读取离散场
         * @param filed 离散场
         * @param h5Group 数据块
         * @return 状态 true成功， false失败
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-07-12
         */
        bool readDiscreteFiled(Interface::FITKAbstractFiled* filed, H5::Group & h5Group);

        /**
         * @brief 写出离散场
         * @param filed 离散场
         * @param h5Group 数据块
         * @return 状态 true成功， false失败
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-07-12
         */
        bool writeDiscreteFiled(Interface::FITKAbstractFiled* filed, H5::Group & h5Group);

    };

    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbstractFiled, FITKInterfaceHDF5AdaptorFiled)

}

#endif
