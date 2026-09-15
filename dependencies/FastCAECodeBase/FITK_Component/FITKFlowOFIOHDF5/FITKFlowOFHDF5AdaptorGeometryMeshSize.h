/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKFlowOFHDF5AdaptorGeometryMeshSize.h
 * @brief  HDF5文件几何边界层读写接口适配器 
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-09-09
 *********************************************************************/
#ifndef __FITKFLOWOFHDF5ADAPTORCGEOMETRYMESHSIZE_R_H__
#define __FITKFLOWOFHDF5ADAPTORCGEOMETRYMESHSIZE_R_H__

#include "FITKFlowOFIOHDF5API.h"
#include "FITKFlowOFHDF5AbstractAdaptor.h"
#include "FITK_Kernel/FITKCore/FITKCoreMacros.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

 //前置声明
ForwardDeclarNS(Interface, FITKGeometryMeshSize)

namespace IO
{
    /**
     * @brief HDF5 Geometry 读写适配器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date 2024-05-30
     */
    class FITKFLOWOFIOHDF5API FITKFlowOFHDF5AdaptorGeometryMeshSize : public FITKFlowOFHDF5AbstractAdaptor
    {
    public:
        explicit FITKFlowOFHDF5AdaptorGeometryMeshSize() = default;
        ~FITKFlowOFHDF5AdaptorGeometryMeshSize() = default;

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
         * @brief  几何数据对象
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-09
         */
        Interface::FITKGeometryMeshSize* _geoMeshSizeData{};
    };

    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKGeometryMeshSize, FITKFlowOFHDF5AdaptorGeometryMeshSize)
}


#endif // !1

