/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKOFRadiationSunLoad.h
 * @brief  Radiation SunLoad数据接口
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-06-18
 *********************************************************************/
#ifndef _FITK_OF_RADIATION_SUNLOAD__H___
#define _FITK_OF_RADIATION_SUNLOAD__H___
 
#include "FITKInterfaceFlowOFAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"

namespace Core
{
    class FITKParameter;
}

namespace Interface
{
    /**
     * @brief  Radiation SunLoad数据基类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-22
     */
    class FITKInterfaceFlowOFAPI FITKOFAbsRadiationSunLoad : public Core::FITKAbstractNDataObject
    {
    public:
        explicit FITKOFAbsRadiationSunLoad();
        virtual ~FITKOFAbsRadiationSunLoad();

        /**
         * @brief    获取参数
         * @return   Core::FITKParameter *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        Core::FITKParameter* getRadiationSunLoadPara();

    protected:
        /**
         * @brief    初始化太阳辐射载荷的参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initRadiationSunLoadParam() = 0;

    protected:
        /**
         * @brief  Radiation SunLoad的参数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-08-22
         */
        Core::FITKParameter* _radiationSunLoadParams{};
    };

    /**
     * @brief  Constant Radiation SunLoad数据基类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-22
     */
    class FITKInterfaceFlowOFAPI FITKOFRadiationSunLoadConstant : public FITKOFAbsRadiationSunLoad
    {
    public:
        explicit FITKOFRadiationSunLoadConstant();
        virtual ~FITKOFRadiationSunLoadConstant();

    protected:
        /**
         * @brief    初始化太阳辐射载荷的参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initRadiationSunLoadParam() override;
    };
}


#endif
