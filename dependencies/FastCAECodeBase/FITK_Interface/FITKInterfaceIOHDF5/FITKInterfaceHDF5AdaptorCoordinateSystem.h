/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKInterfaceHDF5AdaptorCoordinateSystem.h
 * @brief 读写HDF5文件的CoordinateSystem类
 * @author fulipeng (fulipengqd@yeah.net)
 * @date 2024-05-31
 *
 */
#ifndef  _FITKINTERFACEHDF5ADAPTORCOORDINATESYSTEM_H__
#define  _FITKINTERFACEHDF5ADAPTORCOORDINATESYSTEM_H__

#include "FITKInterfaceIOHDF5API.h"
#include "FITKAbstractHDF5Adaptor.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

 //前置声明
ForwardDeclarNS(Interface, FITKCoordinateSystem)

namespace IO
{
    /**
     * @brief HDF5 CoordinateSystem 读写适配器
     * @author fulipeng (fulipengqd@yeah.net)
     * @date 2024-05-31
     */
    class FITKINTERFACEHDF5IOAPI FITKInterfaceHDF5AdaptorCoordinateSystem : public FITKAbstractHDF5Adaptor
    {
    public:
        explicit FITKInterfaceHDF5AdaptorCoordinateSystem() = default;
        virtual ~FITKInterfaceHDF5AdaptorCoordinateSystem() = default;

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



        /**
         * @brief 读取坐标系
         * @param coor 坐标系抽象结构
         * @param h5Group 数据块
         * @return 状态 true成功， false失败
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-06-19
         */
        bool readCoordinateSystem(Interface::FITKCoordinateSystem* coor, H5::Group & h5Group);

        /**
         * @brief 写出坐标系
         * @param coor 坐标系抽象结构
         * @param h5Group 数据块
         * @return 状态 true成功， false失败
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-06-19
         */
        bool writeCoordinateSystem(Interface::FITKCoordinateSystem* coor, H5::Group & h5Group);

    };

    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKCoordinateSystem, FITKInterfaceHDF5AdaptorCoordinateSystem)

}

#endif
