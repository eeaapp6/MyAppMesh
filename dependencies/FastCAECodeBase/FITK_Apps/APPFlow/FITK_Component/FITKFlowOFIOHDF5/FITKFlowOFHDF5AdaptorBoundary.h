/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKFlowOFHDF5AdaptorBoundary.h
 * @brief  hdf5 物理数据边界读写接口
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-09-13
 *********************************************************************/
#ifndef __FITKFLOWOFHDF5ADAPTORBOUNDARY_R_H__
#define __FITKFLOWOFHDF5ADAPTORBOUNDARY_R_H__

#include "FITKFlowOFIOHDF5API.h"
#include "FITKFlowOFHDF5AbstractAdaptor.h"
#include "FITK_Kernel/FITKCore/FITKCoreMacros.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

 //前置声明
ForwardDeclarNS(Interface, FITKOFBoundary)

namespace IO
{
    /**
     * @brief HDF5 Geometry 读写适配器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date 2024-05-30
     */
    class FITKFLOWOFIOHDF5API FITKFlowOFHDF5AdaptorBoundary : public FITKFlowOFHDF5AbstractAdaptor
    {
    public:
        explicit FITKFlowOFHDF5AdaptorBoundary() = default;
        ~FITKFlowOFHDF5AdaptorBoundary() = default;

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
         * @brief    写出flow
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-10
         */
        bool flowBoundaryW();
        /**
         * @brief    读取flow
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-10
         */
        bool flowBoundaryR();
        /**
         * @brief    写出turbulence
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-10
         */
        bool turbulenceBoundaryW();
        /**
         * @brief    读取turbulence
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-10
         */
        bool turbulenceBoundaryR();
        /**
         * @brief    写出phases
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-10
         */
        bool phasesBoundaryW();
        /**
         * @brief    读取phases
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-10
         */
        bool phasesBoundaryR();
        /**
         * @brief    写出thermal
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-10
         */
        bool thermalBoundaryW();
        /**
         * @brief    读取thermal
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-10
         */
        bool thermalBoundaryR();

    private:
        /**
         * @brief  物理场材料数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-09
         */
        Interface::FITKOFBoundary* _boundary{};
    };

    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKOFBoundary, FITKFlowOFHDF5AdaptorBoundary)
}


#endif // !1

