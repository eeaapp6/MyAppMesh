/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKInterfaceHDF5AdaptorAssElement.h
 * @brief 读写HDF5文件的AssElement类
 * @author fulipeng (fulipengqd@yeah.net)
 * @date 2024-07-01
 *
 */
#ifndef  _FITKINTERFACEHDF5ADAPTORASSELEMENT_H__
#define  _FITKINTERFACEHDF5ADAPTORASSELEMENT_H__

#include "FITKInterfaceIOHDF5API.h"
#include "FITKAbstractHDF5Adaptor.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

 //前置声明
ForwardDeclarNS(Interface, FITKAbstractAssemblyElement)
ForwardDeclarNS(Interface, FITKAssemblyElementLine)

namespace IO
{
    /**
     * @brief HDF5 AssElement 抽象单元 读写适配器
     * @author fulipeng (fulipengqd@yeah.net)
     * @date 2024-07-01
     */
    class FITKINTERFACEHDF5IOAPI FITKInterfaceHDF5AdaptorAssemblyElementLine : public FITKAbstractHDF5Adaptor
    {
    public:
        explicit FITKInterfaceHDF5AdaptorAssemblyElementLine() = default;
        virtual ~FITKInterfaceHDF5AdaptorAssemblyElementLine() = default;

        /**
         * @brief 获取适配器数据类型名
         * @return 适配器数据类型名
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-07-01
         */
        QString getAdaptorClass() override;

        /**
         * @brief 适配器读取
         * @return 状态 true成功， false失败
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-07-01
         */
        bool adaptR() override;

        /**
         * @brief 适配器写出
         * @return 状态 true成功， false失败
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-07-01
         */
        bool adaptW() override;

    protected:

        virtual bool readAssemblyElementLine(Interface::FITKAbstractAssemblyElement* assElement, H5::Group& h5Group);


        virtual bool writeAssemblyElementLine(Interface::FITKAbstractAssemblyElement* assElement, H5::Group& h5Group);

    };

    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAssemblyElementLine, FITKInterfaceHDF5AdaptorAssemblyElementLine)

}

#endif
