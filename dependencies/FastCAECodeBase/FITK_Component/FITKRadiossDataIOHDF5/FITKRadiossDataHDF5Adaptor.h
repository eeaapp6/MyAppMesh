/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossDataHDF5Adaptor.h
 * @brief  Radioss数据HDF5IO适配器基类声明
 * @author guqingtao (15598887859@163.com)
 * @date   2025-09-28
 *********************************************************************/
#ifndef __FITKRadiossDataHDF5Adaptor_H___
#define __FITKRadiossDataHDF5Adaptor_H___

#include "FITK_Kernel/FITKAdaptor/FITKAbstractIOAdaptor.h"
#include "FITKRadiossDataIOHDF5API.h"
 
namespace H5
{
	class Group;
}
namespace Core
{
    class FITKAbstractNDataObject;
}
namespace Radioss
{
    class FITKAbstractRadiossData;
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
     * @brief  Radioss数据HDF5IO适配器基类声明
     * @author guqingtao (15598887859@163.com)
     * @date   2025-09-28
     */
    class FITKRadiossDataIOHDF5API FITKRadiossDataHDF5Adaptor : public Adaptor:: FITKAbstractIOAdaptor
    {
        FITKCLASS(Interface, FITKRadiossDataHDF5Adaptor);
    public:
        /**
         * @brief    构造函数
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-28
         */
        explicit FITKRadiossDataHDF5Adaptor() = default;
        /**
         * @brief    析构函数，纯虚函数
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-28
         */
        virtual ~FITKRadiossDataHDF5Adaptor() override = 0;

        /**
         * @brief    设置HDF5读取器
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-28
         */
        void setReader(IO::FITKAbstractHDF5Reader* reader);
        /**
         * @brief    设置HDF5写出器
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-28
         */
        void setWriter(IO::FITKAbstractHDF5Writer* writer);
        /**
         * @brief    设置HDF5节点组
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-28
         */
        void setH5Group(H5::Group* g);
        bool readDataObject(Core::FITKAbstractNDataObject * obj, H5::Group & h5Group);
        bool writeDataObject(Core::FITKAbstractNDataObject * obj, H5::Group & h5Group);

        bool readRadiossData(Radioss::FITKAbstractRadiossData * obj, H5::Group & h5Group);
        bool writeRadiossData(Radioss::FITKAbstractRadiossData * obj, H5::Group & h5Group);

        bool writeVariantToHDF5(const QString &datasetName, const QVariant &value, H5::Group& h5Group);
        bool readVariantFromHDF5(const QString &datasetName, QVariant &value, H5::Group& h5Group);

    protected:

        IO::FITKAbstractHDF5Reader* _reader = nullptr;

        IO::FITKAbstractHDF5Writer* _writer = nullptr;

        H5::Group* _h5Group = nullptr;
    };
}

#endif
