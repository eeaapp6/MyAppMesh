/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKAbstractGeoHDF5Adaptor.h
 * @brief HDF5几何数据IO适配器基类
 * @author libaojun (libaojunqd@foxmail.com)
 * @date 2025-04-16
 *
 */
#ifndef __FITKABSTRACT_GEO_HDF5_ADAPTOR_H___
#define __FITKABSTRACT_GEO_HDF5_ADAPTOR_H___

#include "FITK_Kernel/FITKAdaptor/FITKAbstractIOAdaptor.h"
#include "FITKGeoHDF5KeyWords.h"
#include "FITKInterfaceGeoIOHDF5API.h"
 
namespace H5
{
	class Group;
}

// 前置声明
namespace IO
{
    class FITKAbstractHDF5Reader;
    class FITKAbstractHDF5Writer;

}

namespace Interface
{
    /**
     * @brief HDF5几何数据IO适配器基类声明
     * @author libaojun (libaojunqd@foxmail.com)
     * @date 2025-04-16
     */
    class FITKINTERFACEGEOIOHDF5API FITKAbstractGeoHDF5Adaptor : public Adaptor:: FITKAbstractIOAdaptor
    {
        FITKCLASS(Interface, FITKAbstractGeoHDF5Adaptor);
    public:
        /**
         * @brief 构造函数
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-04-16
         */
        explicit FITKAbstractGeoHDF5Adaptor() = default;
        /**
         * @brief 析构函数，纯虚函数
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-04-16
         */
        virtual ~FITKAbstractGeoHDF5Adaptor() override = 0;

        /**
         * @brief 设置HDF5读取器
         * @param[in] reader HDF5读取器指针
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-04-16
         */
        void setReader(IO::FITKAbstractHDF5Reader* reader);
        /**
         * @brief 设置HDF5写入器
         * @param[in] writer HDF5写入器指针
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-04-16
         */
        void setWriter(IO::FITKAbstractHDF5Writer* writer);

        /**
         * @brief 设置HDF5组指针
         * @param[in] g HDF5组指针
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-04-16
         */
        void setH5Group(H5::Group* g);


    protected:
        /**
         * @brief HDF5读取器指针
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-04-16
         */
        IO::FITKAbstractHDF5Reader* _reader = nullptr;
        /**
         * @brief HDF5写入器指针
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-04-16
         */
        IO::FITKAbstractHDF5Writer* _writer = nullptr;

        /**
         * @brief HDF5组指针
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2025-04-16
         */
        H5::Group* _h5Group = nullptr;
    };
}

#endif
