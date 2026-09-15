/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKFlowOFHDF5AdaptorThermo.h
 * @brief  hdf5 物理场热物理属性数据接口
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-07-08
 *********************************************************************/
#ifndef __FITKFLOWOFHDF5ADAPTORTHERMO_R_H__
#define __FITKFLOWOFHDF5ADAPTORTHERMO_R_H__

#include "FITKFlowOFIOHDF5API.h"
#include "FITKFlowOFHDF5AbstractAdaptor.h"
#include "FITK_Kernel/FITKCore/FITKCoreMacros.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

 //前置声明
ForwardDeclarNS(Interface, FITKOFThermo)
ForwardDeclarNS(Interface, FITKOFThermoPhysicalProp)

namespace IO
{
    /**
     * @brief  物理场热物理属性读写接口 hdf5
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-09-18
     */
    class FITKFLOWOFIOHDF5API FITKFlowOFHDF5AdaptorThermo : public FITKFlowOFHDF5AbstractAdaptor
    {
    public:
        explicit FITKFlowOFHDF5AdaptorThermo() = default;
        ~FITKFlowOFHDF5AdaptorThermo() = default;

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
         * @brief    热物理属性写出
         * @param[i] thermoPhyProp 
         * @param[i] h5Group 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-08
         */
        bool thermoPhysicalPropW(Interface::FITKOFThermoPhysicalProp* thermoPhyProp, H5::Group& h5Group);
        /**
         * @brief    热物理属性读取
         * @param[i] thermoPhyProp 
         * @param[i] h5Group 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-08
         */
        bool thermoPhysicalPropR(Interface::FITKOFThermoPhysicalProp* thermoPhyProp, H5::Group& h5Group);
        /**
         * @brief    状态方程写出
         * @param[i] thermoPhyProp 
         * @param[i] h5Group 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-08
         */
        bool equationOfStateW(Interface::FITKOFThermoPhysicalProp* thermoPhyProp, H5::Group& h5Group);
        /**
         * @brief    状态方程读取
         * @param[i] thermoPhyProp 
         * @param[i] h5Group 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-08
         */
        bool equationOfStateR(Interface::FITKOFThermoPhysicalProp* thermoPhyProp, H5::Group& h5Group);
        /**
         * @brief    热力学属性写出
         * @param[i] thermoPhyProp 
         * @param[i] h5Group 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-08
         */
        bool thermodynamicsW(Interface::FITKOFThermoPhysicalProp* thermoPhyProp, H5::Group& h5Group);
        /**
         * @brief    热力学属性读取
         * @param[i] thermoPhyProp 
         * @param[i] h5Group 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-08
         */
        bool thermodynamicsR(Interface::FITKOFThermoPhysicalProp* thermoPhyProp, H5::Group& h5Group);
        /**
         * @brief    材料属性写出
         * @param[i] thermoPhyProp 
         * @param[i] h5Group 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-08
         */
        bool transportW(Interface::FITKOFThermoPhysicalProp* thermoPhyProp, H5::Group& h5Group);
        /**
         * @brief    材料属性读取
         * @param[i] thermoPhyProp 
         * @param[i] h5Group 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-08
         */
        bool transportR(Interface::FITKOFThermoPhysicalProp* thermoPhyProp, H5::Group& h5Group);

    private:
        /**
         * @brief  物理场热物理属性数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-09
         */
        Interface::FITKOFThermo* _thermo{};
    };

    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKOFThermo, FITKFlowOFHDF5AdaptorThermo)
}


#endif // !1

