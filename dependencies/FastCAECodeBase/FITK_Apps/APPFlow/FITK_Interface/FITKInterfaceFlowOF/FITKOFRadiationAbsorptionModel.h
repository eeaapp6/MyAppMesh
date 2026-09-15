/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKOFRadiationAbsorptionModel.h
 * @brief  Radiation Absorption Model数据接口
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-06-18
 *********************************************************************/
#ifndef _FITK_OF_RADIATION_ABSORPTION_MODEL__H___
#define _FITK_OF_RADIATION_ABSORPTION_MODEL__H___
 
#include "FITKInterfaceFlowOFAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"

namespace Core
{
    class FITKParameter;
}

namespace Interface
{
    /**
     * @brief  Radiation Absorption Model数据基类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-22
     */
    class FITKInterfaceFlowOFAPI FITKOFAbsRadiationAbsorptionModel : public Core::FITKAbstractNDataObject
    {
    public:
        explicit FITKOFAbsRadiationAbsorptionModel();
        virtual ~FITKOFAbsRadiationAbsorptionModel();

        /**
         * @brief    获取参数
         * @return   Core::FITKParameter *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        Core::FITKParameter* getRadiationAbsorptionModelPara();

    protected:
        /**
         * @brief    初始化辐射模型的参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initRadiationAbsorptionModelParam() = 0;

    protected:
        /**
         * @brief  求解器的参数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-08-22
         */
        Core::FITKParameter* _radiationAbsorptionModelParams{};
    };

    /**
     * @brief  none Radiation Absorption Model数据基类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-22
     */
    class FITKInterfaceFlowOFAPI FITKOFRadiationAbsorptionModelNone : public FITKOFAbsRadiationAbsorptionModel
    {
    public:
        explicit FITKOFRadiationAbsorptionModelNone();
        virtual ~FITKOFRadiationAbsorptionModelNone();

    protected:
        /**
         * @brief    初始化求解器的参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initRadiationAbsorptionModelParam() override;
    };
    /**
     * @brief  Constant Radiation Model数据类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-22
     */
    class FITKInterfaceFlowOFAPI FITKOFRadiationAbsorptionModelConstant : public FITKOFAbsRadiationAbsorptionModel
    {
    public:
        explicit FITKOFRadiationAbsorptionModelConstant();
        virtual ~FITKOFRadiationAbsorptionModelConstant();

    protected:
        /**
         * @brief    初始化求解器的参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initRadiationAbsorptionModelParam() override;
    };
}


#endif
