/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKFlowOFHDF5AdaptorGeometryComponent.h
 * @brief  hdf5 几何组件数据
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-09-19
 *********************************************************************/
#ifndef __FITKFLOWOFHDF5ADAPTORGEOMETRYCOMPONENT_R_H__
#define __FITKFLOWOFHDF5ADAPTORGEOMETRYCOMPONENT_R_H__

#include "FITKFlowOFIOHDF5API.h"
#include "FITKFlowOFHDF5AbstractAdaptor.h"
#include "FITK_Kernel/FITKCore/FITKCoreMacros.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

 //前置声明
ForwardDeclarNS(Interface, FITKGeoComponentManager)
ForwardDeclarNS(Interface, FITKGeoComponent)

namespace IO
{
    /**
     * @brief  hdf5 几何组件数据
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-09-19
     */
    class FITKFLOWOFIOHDF5API FITKFlowOFHDF5AdaptorGeometryComponent : public FITKFlowOFHDF5AbstractAdaptor
    {
    public:
        explicit FITKFlowOFHDF5AdaptorGeometryComponent() = default;
        ~FITKFlowOFHDF5AdaptorGeometryComponent() = default;

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
         * @brief    几何分组写出
         * @param[i] component 
         * @param[i] h5Group 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-06
         */
        bool geoComponentW(Interface::FITKGeoComponent* component, H5::Group& h5Group);
        /**
         * @brief    几何分组读取
         * @param[i] component 
         * @param[i] h5Group 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-06
         */
        bool geoComponentR(Interface::FITKGeoComponent* component, H5::Group& h5Group);

    private:
        /**
         * @brief  物理场材料数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-09
         */
        Interface::FITKGeoComponentManager* _geoComponentMgr{};
    };

    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKGeoComponentManager, FITKFlowOFHDF5AdaptorGeometryComponent)
}


#endif // !1

