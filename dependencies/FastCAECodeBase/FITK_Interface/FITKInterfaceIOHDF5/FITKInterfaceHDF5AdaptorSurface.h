/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKInterfaceHDF5AdaptorSurface.h
 * @brief 读写HDF5文件的Set类
 * @author fulipeng (fulipengqd@yeah.net)
 * @date 2024-05-29
 *
 */
#ifndef  _FITKINTERFACEHDF5ADAPTORSURFACE_H__
#define  _FITKINTERFACEHDF5ADAPTORSURFACE_H__

#include "FITKInterfaceIOHDF5API.h"
#include "FITKAbstractHDF5Adaptor.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

namespace Interface
{
    class FITKComponentManager;
    class FITKMeshSurface;
    class FITKGeometrySurface;
    class FITKModelSurface;
}

namespace IO
{
    /**
     * @brief HDF5 Surface 读写适配器
     * @author fulipeng (fulipengqd@yeah.net)
     * @date 2024-05-29
     */
    class FITKINTERFACEHDF5IOAPI FITKInterfaceHDF5AdaptorSurface : public FITKAbstractHDF5Adaptor
    {
    public:
        explicit FITKInterfaceHDF5AdaptorSurface() = default;
        virtual ~FITKInterfaceHDF5AdaptorSurface() = default;

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

        virtual bool surfR(Interface::FITKMeshSurface* surfaceData, H5::Group & h5Group);
        virtual bool geoSurfR(Interface::FITKGeometrySurface* surfaceData, H5::Group & h5Group);

        bool nodeTypeR(Interface::FITKMeshSurface* surfaceData, H5::Group& h5Group);
        bool elementTypeR(Interface::FITKMeshSurface* surfaceData, H5::Group& h5Group);

        virtual bool surfW(Interface::FITKMeshSurface* surfaceData, H5::Group & h5Group);
        virtual bool geoSurfW(Interface::FITKGeometrySurface* surfaceData, H5::Group & h5Group);

        bool nodeTypeW(Interface::FITKMeshSurface* surfaceData, H5::Group & h5Group);

        bool elementTypeW(Interface::FITKMeshSurface* surfaceData, H5::Group & h5Group);


    protected:

        /**
         * @brief instance名称，如果没有则为空
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-05-29
         */
        QString instanceName;

        /**
         * @brief 是否是局部坐标系
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-05-29
         */
        bool _isTrans{ false };
    };

    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKModelSurface, FITKInterfaceHDF5AdaptorSurface)

} // namespace IO
#endif // _FITKABSTRACTHDF5ADAPTORSET_H__