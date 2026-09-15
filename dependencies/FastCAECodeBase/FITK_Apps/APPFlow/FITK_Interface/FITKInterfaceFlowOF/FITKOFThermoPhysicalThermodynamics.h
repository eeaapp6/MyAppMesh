/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKOFThermoPhysicalThermodynamics.h
 * @brief  Thermodynamics 数据接口
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-06-19
 *********************************************************************/
#ifndef _FITK_OF_THERMOPHYSICAL_THERMODYNAMICS__H___
#define _FITK_OF_THERMOPHYSICAL_THERMODYNAMICS__H___
 
#include "FITKInterfaceFlowOFAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"

namespace Core
{
    class FITKParameter;
}

namespace Interface
{
    /**
     * @brief  Thermodynamics 数据基类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-06-19
     */
    class FITKInterfaceFlowOFAPI FITKOFAbsThermoPhysicalThermodynamics : public Core::FITKAbstractNDataObject
    {
    public:
        explicit FITKOFAbsThermoPhysicalThermodynamics();
        virtual ~FITKOFAbsThermoPhysicalThermodynamics();

        /**
         * @brief    获取Thermodynamics参数
         * @return   Core::FITKParameter *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-19
         */
        Core::FITKParameter* getThermodynamicsPara();

    protected:
        /**
         * @brief    初始化Thermodynamics的参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initThermodynamicsParam() = 0;

    protected:
        /**
         * @brief  thermodynamics 参数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-06-19
         */
        Core::FITKParameter* _thermodynamicsPara{};
    };
    /**
     * @brief  Constant - Thermodynamics数据基类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-06-19
     */
    class FITKInterfaceFlowOFAPI FITKOFThermoPhysicalThermodynamicsConstant : public FITKOFAbsThermoPhysicalThermodynamics
    {
    public:
        explicit FITKOFThermoPhysicalThermodynamicsConstant();
        virtual ~FITKOFThermoPhysicalThermodynamicsConstant();

    protected:
        /**
         * @brief    初始化Thermodynamics的参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initThermodynamicsParam() override;
    };
    /**
     * @brief  eConstant - Thermodynamics数据基类-用于固体
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-06-19
     */
    class FITKInterfaceFlowOFAPI FITKOFThermoPhysicalThermodynamicsEConstant : public FITKOFAbsThermoPhysicalThermodynamics
    {
    public:
        explicit FITKOFThermoPhysicalThermodynamicsEConstant();
        virtual ~FITKOFThermoPhysicalThermodynamicsEConstant();

    protected:
        /**
         * @brief    初始化Thermodynamics的参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initThermodynamicsParam() override;
    };
}


#endif
