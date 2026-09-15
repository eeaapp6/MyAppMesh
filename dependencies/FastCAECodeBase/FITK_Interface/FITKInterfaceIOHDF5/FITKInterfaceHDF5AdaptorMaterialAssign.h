/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKAbaqusHDF5AdaptorMaterialAssign.h
 * @brief 读写HDF5文件的SectionAssign类
 * @author fulipeng (fulipengqd@yeah.net)
 * @date 2024-06-05
 *
 */
#ifndef  _FITKINTERFACEHDF5ADAPTORMATERIALASSIGN_H__
#define  _FITKINTERFACEHDF5ADAPTORMATERIALASSIGN_H__

#include "FITKInterfaceIOHDF5API.h"
#include "FITKAbstractHDF5Adaptor.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

 //前置声明
ForwardDeclarNS(Interface, FITKAbstractSection)
ForwardDeclarNS(Interface, FITKMaterialAssign)
ForwardDeclarNS(Interface, FITKAbstractMaterial)

namespace IO
{
    /**
     * @brief HDF5 MaterialAssign 读写适配器
     * @author fulipeng (fulipengqd@yeah.net)
     * @date 2024-06-05
     */
    class FITKINTERFACEHDF5IOAPI FITKInterfaceHDF5AdaptorMaterialAssign : public FITKAbstractHDF5Adaptor
    {
    public:
        explicit FITKInterfaceHDF5AdaptorMaterialAssign() = default;
        virtual ~FITKInterfaceHDF5AdaptorMaterialAssign() = default;

        /**
         * @brief 获取适配器数据类型名
         * @return 适配器数据类型名
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-06-05
         */
        QString getAdaptorClass() override;

        /**
         * @brief 适配器读取
         * @return 状态 true成功， false失败
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-06-05
         */
        bool adaptR() override;

        /**
         * @brief 适配器写出
         * @return 状态 true成功， false失败
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-06-05
         */
        bool adaptW() override;

    protected:

        virtual bool readMaterialAssign(Interface::FITKMaterialAssign* assign, H5::Group& h5Group);

        virtual bool writeMaterialAssign(Interface::FITKMaterialAssign* assign, H5::Group& h5Group);

    };

    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKMaterialAssign, FITKInterfaceHDF5AdaptorMaterialAssign)

}

#endif
