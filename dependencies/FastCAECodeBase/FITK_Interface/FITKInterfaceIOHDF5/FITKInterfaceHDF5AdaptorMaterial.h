/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKInterfaceHDF5AdaptorMaterial.h
 * @brief 读写HDF5文件的Material类
 * @author fulipeng (fulipengqd@yeah.net)
 * @date 2024-05-31
 *
 */
#ifndef  _FITKINTERFACEHDF5ADAPTORMATERIAL_H__
#define  _FITKINTERFACEHDF5ADAPTORMATERIAL_H__

#include "FITKInterfaceIOHDF5API.h"
#include "FITKAbstractHDF5Adaptor.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

 //前置声明
ForwardDeclarNS(Interface, FITKAbstractMaterial)

namespace IO
{
    /**
     * @brief HDF5 Material 读写适配器
     * @author fulipeng (fulipengqd@yeah.net)
     * @date 2024-05-31
     */
    class FITKINTERFACEHDF5IOAPI FITKInterfaceHDF5AdaptorMaterial : public FITKAbstractHDF5Adaptor
    {
    public:
        explicit FITKInterfaceHDF5AdaptorMaterial() = default;
        virtual ~FITKInterfaceHDF5AdaptorMaterial() = default;

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
         * @brief 读取材料信息 需要设置groupName 为材料名称
         * @param material 抽象材料
         * @param h5Group 数据块
         * @return
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-06-03
         */
        virtual bool readMaterial(Interface::FITKAbstractMaterial* material, H5::Group & h5Group);

        /**
         * @brief 写出材料信息
         * @param material 抽象材料
         * @param h5Group 数据块
         * @return
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-06-03
         */
        virtual bool writeMaterial(Interface::FITKAbstractMaterial* material, H5::Group & h5Group);

    };

    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKAbstractMaterial, FITKInterfaceHDF5AdaptorMaterial)

}

#endif
