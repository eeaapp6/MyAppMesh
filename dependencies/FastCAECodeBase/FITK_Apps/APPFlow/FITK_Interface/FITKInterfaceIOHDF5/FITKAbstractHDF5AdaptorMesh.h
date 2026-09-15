/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKAbstractHDF5AdaptorMesh.h
 * @brief 读取HDF5文件的Mesh 抽象类
 * @author fulipeng (fulipengqd@yeah.net)
 * @date 2024-05-29
 *
 */
#ifndef  _FITKABSTRACTHDF5ADAPTORMESH_H__
#define  _FITKABSTRACTHDF5ADAPTORMESH_H__

#include "FITKInterfaceHDF5IOAPI.h"
#include "FITKAbstractHDF5Adaptor.h"
#include "FITK_Kernal/FITKAdaptor/FITKIOAdaptorFactory.h"

 //前置声明
ForwardDeclarNS(Interface, FITKModelSurface)

namespace IO
{
    /**
     * @brief HDF5 Mesh 读写适配器
     * @author fulipeng (fulipengqd@yeah.net)
     * @date 2024-05-29
     */
    class FITKINTERFACEHDF5IOAPI FITKAbstractHDF5AdaptorMesh : public FITKAbstractHDF5Adaptor
    {
    public:
        explicit FITKAbstractHDF5AdaptorMesh() = default;
        virtual ~FITKAbstractHDF5AdaptorMesh() = default;

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

    };

    //Register2FITKIOAdaptorFactory(HDF5, Interface::FITKModelSet, FITKInterfaceHDF5AdaptorSet)
}

#endif