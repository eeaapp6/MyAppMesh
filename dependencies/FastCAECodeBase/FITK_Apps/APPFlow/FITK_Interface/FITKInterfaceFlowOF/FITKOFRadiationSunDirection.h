/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKOFRadiationSunDirection.h
 * @brief  Radiation SunDirection 数据接口
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-06-18
 *********************************************************************/
#ifndef _FITK_OF_RADIATION_SUNDIRECTION__H___
#define _FITK_OF_RADIATION_SUNDIRECTION__H___
 
#include "FITKInterfaceFlowOFAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"

namespace Core
{
    class FITKParameter;
}

namespace Interface
{
    /**
     * @brief  Radiation SunDirection数据基类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-22
     */
    class FITKInterfaceFlowOFAPI FITKOFAbsRadiationSunDirection : public Core::FITKAbstractNDataObject
    {
    public:
        explicit FITKOFAbsRadiationSunDirection();
        virtual ~FITKOFAbsRadiationSunDirection();

        /**
         * @brief    获取参数
         * @return   Core::FITKParameter *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        Core::FITKParameter* getRadiationSunDirectionPara();

    protected:
        /**
         * @brief    初始化太阳辐射方向的参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initRadiationSunDirectionParam() = 0;

    protected:
        /**
         * @brief  Radiation SunDirection的参数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-08-22
         */
        Core::FITKParameter* _radiationSunDirectionParams{};
    };

    /**
     * @brief  Constant Radiation SunDirection数据基类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-22
     */
    class FITKInterfaceFlowOFAPI FITKOFRadiationSunDirectionConstant : public FITKOFAbsRadiationSunDirection
    {
    public:
        explicit FITKOFRadiationSunDirectionConstant();
        virtual ~FITKOFRadiationSunDirectionConstant();

    protected:
        /**
         * @brief    初始化太阳辐射方向的参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initRadiationSunDirectionParam() override;
    };
    /**
     * @brief  SunTracking Radiation SunDirection数据类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-22
     */
    class FITKInterfaceFlowOFAPI FITKOFRadiationSunDirectionSunTracking : public FITKOFAbsRadiationSunDirection
    {
    public:
        explicit FITKOFRadiationSunDirectionSunTracking();
        virtual ~FITKOFRadiationSunDirectionSunTracking();

    protected:
        /**
         * @brief    初始化太阳辐射方向的参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initRadiationSunDirectionParam() override;
    };
}


#endif
