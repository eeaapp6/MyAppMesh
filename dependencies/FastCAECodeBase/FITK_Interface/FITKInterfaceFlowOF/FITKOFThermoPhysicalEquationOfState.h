/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKOFThermoPhysicalEquationOfState.h
 * @brief  Equation Of State数据接口
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-06-19
 *********************************************************************/
#ifndef _FITK_OF_THERMOPHYSICAL_EQUATIONOFSTATE__H___
#define _FITK_OF_THERMOPHYSICAL_EQUATIONOFSTATE__H___
 
#include "FITKInterfaceFlowOFAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"

namespace Core
{
    class FITKParameter;
}

namespace Interface
{
    /**
     * @brief  Equation Of State数据基类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-06-19
     */
    class FITKInterfaceFlowOFAPI FITKOFAbsThermoPhysicalEquationOfState : public Core::FITKAbstractNDataObject
    {
    public:
        explicit FITKOFAbsThermoPhysicalEquationOfState();
        virtual ~FITKOFAbsThermoPhysicalEquationOfState();

        /**
         * @brief    获取Equation Of State参数
         * @return   Core::FITKParameter *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-19
         */
        Core::FITKParameter* getEquationOfStatePara();

    protected:
        /**
         * @brief    初始化EquationOfState的参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initEquationOfStateParam() = 0;

    protected:
        /**
         * @brief  Equation Of State 参数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-06-19
         */
        Core::FITKParameter* _equationOfStatePara{};
    };
    /**
     * @brief  Perfect Gas - Equation Of State数据基类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-06-19
     */
    class FITKInterfaceFlowOFAPI FITKOFThermoPhysicalEquationOfStatePerfectGas : public FITKOFAbsThermoPhysicalEquationOfState
    {
    public:
        explicit FITKOFThermoPhysicalEquationOfStatePerfectGas();
        virtual ~FITKOFThermoPhysicalEquationOfStatePerfectGas();

    protected:
        /**
         * @brief    初始化EquationOfState的参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initEquationOfStateParam() override;
    };
    /**
     * @brief  Boussinesq - Equation Of State数据基类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-06-19
     */
    class FITKInterfaceFlowOFAPI FITKOFThermoPhysicalEquationOfStateBoussinesq : public FITKOFAbsThermoPhysicalEquationOfState
    {
    public:
        explicit FITKOFThermoPhysicalEquationOfStateBoussinesq();
        virtual ~FITKOFThermoPhysicalEquationOfStateBoussinesq();

    protected:
        /**
         * @brief    初始化EquationOfState的参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initEquationOfStateParam() override;
    };
    /**
     * @brief  Incompressible Perfect Gas - Equation Of State数据基类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-06-19
     */
    class FITKInterfaceFlowOFAPI FITKOFThermoPhysicalEquationOfStateIncompressiblePerfectGas : public FITKOFAbsThermoPhysicalEquationOfState
    {
    public:
        explicit FITKOFThermoPhysicalEquationOfStateIncompressiblePerfectGas();
        virtual ~FITKOFThermoPhysicalEquationOfStateIncompressiblePerfectGas();

    protected:
        /**
         * @brief    初始化EquationOfState的参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initEquationOfStateParam() override;
    };
    /**
     * @brief  Constant Density - Equation Of State数据基类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-06-19
     */
    class FITKInterfaceFlowOFAPI FITKOFThermoPhysicalEquationOfStateConstantDensity : public FITKOFAbsThermoPhysicalEquationOfState
    {
    public:
        explicit FITKOFThermoPhysicalEquationOfStateConstantDensity();
        virtual ~FITKOFThermoPhysicalEquationOfStateConstantDensity();

    protected:
        /**
         * @brief    初始化EquationOfState的参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initEquationOfStateParam() override;
    };
    /**
     * @brief  Perfect Fluid - Equation Of State数据基类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-06-19
     */
    class FITKInterfaceFlowOFAPI FITKOFThermoPhysicalEquationOfStatePerfectFluid : public FITKOFAbsThermoPhysicalEquationOfState
    {
    public:
        explicit FITKOFThermoPhysicalEquationOfStatePerfectFluid();
        virtual ~FITKOFThermoPhysicalEquationOfStatePerfectFluid();

    protected:
        /**
         * @brief    初始化EquationOfState的参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initEquationOfStateParam() override;
    };
    /**
     * @brief  Adiabatic Perfect Fluid - Equation Of State数据基类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-06-19
     */
    class FITKInterfaceFlowOFAPI FITKOFThermoPhysicalEquationOfStateAdiabaticPerfectFluid : public FITKOFAbsThermoPhysicalEquationOfState
    {
    public:
        explicit FITKOFThermoPhysicalEquationOfStateAdiabaticPerfectFluid();
        virtual ~FITKOFThermoPhysicalEquationOfStateAdiabaticPerfectFluid();

    protected:
        /**
         * @brief    初始化EquationOfState的参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initEquationOfStateParam() override;
    };
    /**
     * @brief  Peng-Robinson - Equation Of State数据基类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-06-19
     */
    class FITKInterfaceFlowOFAPI FITKOFThermoPhysicalEquationOfStatePengRobinson : public FITKOFAbsThermoPhysicalEquationOfState
    {
    public:
        explicit FITKOFThermoPhysicalEquationOfStatePengRobinson();
        virtual ~FITKOFThermoPhysicalEquationOfStatePengRobinson();

    protected:
        /**
         * @brief    初始化EquationOfState的参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initEquationOfStateParam() override;
    };
}


#endif
