/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKFlowOFHDF5AdaptorRadiation.h
 * @brief  hdf5 物理场辐射数据接口
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-07-08
 *********************************************************************/
#ifndef __FITKFLOWOFHDF5ADAPTORRADIATION_R_H__
#define __FITKFLOWOFHDF5ADAPTORRADIATION_R_H__

#include "FITKFlowOFIOHDF5API.h"
#include "FITKFlowOFHDF5AbstractAdaptor.h"
#include "FITK_Kernel/FITKCore/FITKCoreMacros.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

 //前置声明
ForwardDeclarNS(Interface, FITKOFRadiation)

namespace IO
{
    /**
     * @brief  物理场湍流读写接口 hdf5
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-09-18
     */
    class FITKFLOWOFIOHDF5API FITKFlowOFHDF5AdaptorRadiation : public FITKFlowOFHDF5AbstractAdaptor
    {
    public:
        explicit FITKFlowOFHDF5AdaptorRadiation() = default;
        ~FITKFlowOFHDF5AdaptorRadiation() = default;

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
         * @brief    辐射参数写出
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-08
         */
        bool radiationParamW();
        /**
         * @brief    辐射参数读取
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-08
         */
        bool radiationParamR();
        /**
         * @brief    辐射模型写出
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-08
         */
        bool radiationModelW();
        /**
         * @brief    辐射模型读取
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-08
         */
        bool radiationModelR();
        /**
         * @brief    吸收系数写出
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-08
         */
        bool absorptionModelW();
        /**
         * @brief    吸收系数读取
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-08
         */
        bool absorptionModelR();
        /**
         * @brief    散射系数写出
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-08
         */
        bool scatterModelW();
        /**
         * @brief    散射系数读取
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-08
         */
        bool scatterModelR();
        /**
         * @brief    太阳辐射源写出
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-08
         */
        bool solarLoadW();
        /**
         * @brief    太阳辐射源读取
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-08
         */
        bool solarLoadR();

    private:
        /**
         * @brief  物理场辐射数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-09
         */
        Interface::FITKOFRadiation* _radiation{};
    };

    Register2FITKIOAdaptorFactory(HDF5, Interface::FITKOFRadiation, FITKFlowOFHDF5AdaptorRadiation)
}


#endif // !1

