/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKInterfaceHDF5AdaptorSection.h
 * @brief 读写HDF5文件的Section类
 * @author fulipeng (fulipengqd@yeah.net)
 * @date 2024-05-31
 *
 */
#ifndef  _FITKINTERFACEHDF5ADAPTORSECTION_H__
#define  _FITKINTERFACEHDF5ADAPTORSECTION_H__

#include "FITKInterfaceIOHDF5API.h"
#include "FITKAbstractHDF5Adaptor.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

 //前置声明
ForwardDeclarNS(Interface, FITKAbstractSection)
ForwardDeclarNS(Interface, FITKAbstractMaterial)

namespace IO
{
    /**
     * @brief HDF5 Assembly 读写适配器
     * @author fulipeng (fulipengqd@yeah.net)
     * @date 2024-05-31
     */
    class FITKINTERFACEHDF5IOAPI FITKInterfaceHDF5AdaptorSection : public FITKAbstractHDF5Adaptor
    {
    public:
        explicit FITKInterfaceHDF5AdaptorSection() = default;
        virtual ~FITKInterfaceHDF5AdaptorSection() = default;

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
         * @brief 读取截面属性
         * @param section 截面
         * @param h5Group
         * @return 状态 true成功， false失败
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-06-19
         */
        virtual bool readSection(Interface::FITKAbstractSection* section, H5::Group& h5Group);

        /**
         * @brief 写出截面属性
         * @param section 截面
         * @param h5Group
         * @return 状态 true成功， false失败
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-06-19
         */
        virtual bool writeSection(Interface::FITKAbstractSection* section, H5::Group& h5Group);

    };

    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbstractSection, FITKInterfaceHDF5AdaptorSection)

}

#endif
