/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKInterfaceHDF5AdaptorSet.h
 * @brief 读写HDF5文件的Set类
 * @author fulipeng (fulipengqd@yeah.net)
 * @date 2024-05-29
 *
 */
#ifndef  _FITKINTERFACEHDF5ADAPTORSET_H__
#define  _FITKINTERFACEHDF5ADAPTORSET_H__

#include "FITKInterfaceIOHDF5API.h"
#include "FITKAbstractHDF5Adaptor.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

 //前置声明
ForwardDeclarNS(Interface, FITKModelSet)
ForwardDeclarNS(Interface, FITKComponentManager)

namespace IO
{
    /**
     * @brief HDF5 Set 读写适配器
     * @author fulipeng (fulipengqd@yeah.net)
     * @date 2024-05-29
     */
    class FITKINTERFACEHDF5IOAPI FITKInterfaceHDF5AdaptorSet : public FITKAbstractHDF5Adaptor
    {
    public:
        explicit FITKInterfaceHDF5AdaptorSet() = default;
        virtual ~FITKInterfaceHDF5AdaptorSet() = default;

        /**
         * @brief 获取适配器数据类型名
         * @return 适配器数据类型名
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-05-29
         */
        QString getAdaptorClass() override;

        /**
         * @brief 设置set管理器
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-06-18
         */
        void setCompomentManager(Interface::FITKComponentManager* compManager);

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


    protected:

        /**
         * @brief 读取集合
         * @param set
         * @param h5Group
         * @return 状态 true成功， false失败
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-05-30
         */
        bool readSet(Interface::FITKModelSet* set, H5::Group& h5Group);

        /**
         * @brief 写出集合
         * @param set 
         * @param h5Group 
         * @return 状态 true成功， false失败
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-05-30
         */
        bool writeSet(Interface::FITKModelSet* set, H5::Group& h5Group);

    protected:

        /**
         * @brief 集合管理器
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-05-29
         */
        Interface::FITKComponentManager* _compManager{};

        /**
         * @brief 集合（Set）数据  NodeSet\ElementSet
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-05-29
         */
        Interface::FITKModelSet* _setData{};

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

    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKModelSet, FITKInterfaceHDF5AdaptorSet)

} // namespace IO
#endif // _FITKABSTRACTHDF5ADAPTORSET_H__