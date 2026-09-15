/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKFlowOFHDF5AdaptorOperatingCond.h
 * @brief  hdf5 物理场运行配置读写接口
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-09-13
 *********************************************************************/
#ifndef __FITKFLOWOFHDF5ADAPTOROPERATIONCONDITION_R_H__
#define __FITKFLOWOFHDF5ADAPTOROPERATIONCONDITION_R_H__

#include "FITKFlowOFIOHDF5API.h"
#include "FITKFlowOFHDF5AbstractAdaptor.h"
#include "FITK_Kernel/FITKCore/FITKCoreMacros.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

 //前置声明
ForwardDeclarNS(Interface, FITKOFOperatingConditions)

namespace IO
{
    /**
     * @brief HDF5 Geometry 读写适配器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date 2024-05-30
     */
    class FITKFLOWOFIOHDF5API FITKFlowOFHDF5AdaptorOperatingCond : public FITKFlowOFHDF5AbstractAdaptor
    {
    public:
        explicit FITKFlowOFHDF5AdaptorOperatingCond() = default;
        ~FITKFlowOFHDF5AdaptorOperatingCond() = default;

        /**
         * @brief 获取适配器数据类型名
         * @return 适配器数据类型名
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-05-30
         */
        QString getAdaptorClass() override;

        /**
         * @brief 适配器读取
         * @return 状态 true成功， false失败
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-05-30
         */
        bool adaptR() override;

        /**
         * @brief 适配器写出
         * @return 状态 true成功， false失败
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-05-30
         */
        bool adaptW() override;

    private:
        /**
         * @brief    gravitationalAcceleration写出
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-10
         */
        bool gravitationalAccelerationW();
        /**
         * @brief    gravitationalAcceleration读取
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-10
         */
        bool gravitationalAccelerationR();
        /**
         * @brief    压力写出
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-10
         */
        bool referencePressureW();
        /**
         * @brief    压力读取
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-10
         */
        bool referencePressureR();

    private:
        /**
         * @brief  物理场材料数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-09
         */
        Interface::FITKOFOperatingConditions* _operatingCond{};
    };

    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKOFOperatingConditions, FITKFlowOFHDF5AdaptorOperatingCond)
}


#endif // !1

