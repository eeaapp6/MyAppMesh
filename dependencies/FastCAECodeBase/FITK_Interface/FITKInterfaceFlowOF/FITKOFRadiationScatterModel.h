/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKOFRadiationScatterModel.h
 * @brief  Radiation Scatter Model 数据接口
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-06-18
 *********************************************************************/
#ifndef _FITK_OF_RADIATION_SCATTER_MODEL__H___
#define _FITK_OF_RADIATION_SCATTER_MODEL__H___
 
#include "FITKInterfaceFlowOFAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"

namespace Core
{
    class FITKParameter;
}

namespace Interface
{
    /**
     * @brief  Radiation Scatter Model数据基类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-22
     */
    class FITKInterfaceFlowOFAPI FITKOFAbsRadiationScatterModel : public Core::FITKAbstractNDataObject
    {
    public:
        explicit FITKOFAbsRadiationScatterModel();
        virtual ~FITKOFAbsRadiationScatterModel();

        /**
         * @brief    获取参数
         * @return   Core::FITKParameter *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        Core::FITKParameter* getRadiationScatterModelPara();

    protected:
        /**
         * @brief    初始化散射系数的参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initRadiationScatterModelParam() = 0;

    protected:
        /**
         * @brief  Radiation Scatter Model的参数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-08-22
         */
        Core::FITKParameter* _radiationScatterModelParams{};
    };

    /**
     * @brief  none Radiation Scatter Model数据基类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-22
     */
    class FITKInterfaceFlowOFAPI FITKOFRadiationScatterModelNone : public FITKOFAbsRadiationScatterModel
    {
    public:
        explicit FITKOFRadiationScatterModelNone();
        virtual ~FITKOFRadiationScatterModelNone();

    protected:
        /**
         * @brief    初始化散射系数的参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initRadiationScatterModelParam() override;
    };
    /**
     * @brief  Constant Radiation Scatter Model数据基类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-22
     */
    class FITKInterfaceFlowOFAPI FITKOFRadiationScatterModelConstant : public FITKOFAbsRadiationScatterModel
    {
    public:
        explicit FITKOFRadiationScatterModelConstant();
        virtual ~FITKOFRadiationScatterModelConstant();

    protected:
        /**
         * @brief    初始化散射系数的参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initRadiationScatterModelParam() override;
    };
}


#endif
