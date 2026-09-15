/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKFlowOFHDF5AdaptorPhysicsData.h
 * @brief  hdf5求解物理数据
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-09-11
 *********************************************************************/
#ifndef __FITKFLOWOFHDF5ADAPTORPHYSICSDATA_R_H__
#define __FITKFLOWOFHDF5ADAPTORPHYSICSDATA_R_H__

#include "FITKFlowOFIOHDF5API.h"
#include "FITKFlowOFHDF5AbstractAdaptor.h"
#include "FITK_Kernel/FITKCore/FITKCoreMacros.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

 //前置声明
ForwardDeclarNS(Interface, FITKOFPhysicsData)

namespace IO
{
    /**
     * @brief HDF5 Geometry 读写适配器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date 2024-05-30
     */
    class FITKFLOWOFIOHDF5API FITKFlowOFHDF5AdaptorPhysicsData : public FITKFlowOFHDF5AbstractAdaptor
    {
    public:
        explicit FITKFlowOFHDF5AdaptorPhysicsData() = default;
        ~FITKFlowOFHDF5AdaptorPhysicsData() = default;

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

    protected:
        /**
         * @brief    求解器数据写出
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-08
         */
        bool physicsSolverW();
        /**
         * @brief    求解器数据读取
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-08
         */
        bool physicsSolverR();
        /**
         * @brief    区域网格类型写出
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-08
         */
        bool physicsRegionMeshTypeW();
        /**
         * @brief    区域网格类型读取
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-08
         */
        bool physicsRegionMeshTypeR();
        /**
         * @brief    辐射数据写出
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-08
         */
        bool physicsRadiationW();
        /**
         * @brief    辐射数据读取
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-08
         */
        bool physicsRadiationR();
        /**
         * @brief    热物理属性数据写出
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-08
         */
        bool physicsThermoW();
        /**
         * @brief    热物理属性数据读取
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-08
         */
        bool physicsThermoR();
        /**
         * @brief    湍流数据写出
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-08
         */
        bool physicsTurbulenceW();
        /**
         * @brief    湍流数据读取
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-08
         */
        bool physicsTurbulenceR();
        /**
         * @brief    材料数据写出
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-08
         */
        bool physicsTransportPropW();
        /**
         * @brief    材料数据读取
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-08
         */
        bool physicsTransportPropR();
        /**
         * @brief    离散数据写出
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-08
         */
        bool physicsDiscretizationW();
        /**
         * @brief    离散数据读取
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-08
         */
        bool physicsDiscretizationR();
        /**
         * @brief    解决方案数据写出
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-08
         */
        bool physicsSolutionW();
        /**
         * @brief    解决方案数据读取
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-08
         */
        bool physicsSolutionR();
        /**
         * @brief    初始化参数数据写出
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-08
         */
        bool physicsInitialConditionsW();
        /**
         * @brief    初始化参数数据读取
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-08
         */
        bool physicsInitialConditionsR();
        /**
         * @brief    边界数据写出
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-08
         */
        bool physicsBoundaryW();
        /**
         * @brief    边界数据读取
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-08
         */
        bool physicsBoundaryR();
        /**
         * @brief    运行配置数据写出
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-08
         */
        bool physicsRunControlW();
        /**
         * @brief    运行配置数据读取
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-08
         */
        bool physicsRunControlR();
        /**
         * @brief    工作条件数据写出
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-08
         */
        bool physicsOperatingConditionsW();
        /**
         * @brief    工作条件数据读取
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-08
         */
        bool physicsOperatingConditionsR();

    private:
        /**
         * @brief  物理数据对象
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-09
         */
        Interface::FITKOFPhysicsData* _physicsData{};
    };

    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKOFPhysicsData, FITKFlowOFHDF5AdaptorPhysicsData)
}


#endif // !1

