/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKOFBoundaryType.h
 * @brief  边界类型
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-08-26
 *********************************************************************/
#ifndef _FITK_OF_BOUNDARY_TYPE_H___
#define _FITK_OF_BOUNDARY_TYPE_H___
 
#include "FITKInterfaceFlowOFAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITKOFEnum.hpp"

namespace Core
{
    class FITKParameter;
}

namespace Interface
{
    /**
     * @brief  边界类型基类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-26
     */
    class FITKInterfaceFlowOFAPI FITKOFAbsBoundaryType : public Core::FITKAbstractNDataObject
    {
    public:
        explicit FITKOFAbsBoundaryType();
        virtual ~FITKOFAbsBoundaryType();

        /**
         * @brief    获取参数
         * @return   Core::FITKParameter *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        Core::FITKParameter* getBoundaryTypePara();
        
    protected:
        /**
         * @brief    初始化求解器的参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initBoundaryTypeParam() = 0;
    protected:
        /**
         * @brief  边界类型的参数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-08-22
         */
        Core::FITKParameter* _boundaryTypeParams{};
    };

    /**
     * @brief  Symmetry 边界类型
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-26
     */
    class FITKInterfaceFlowOFAPI FITKOFBTySymmetry : public FITKOFAbsBoundaryType
    {
    public:
        explicit FITKOFBTySymmetry();
        virtual ~FITKOFBTySymmetry();

    protected:
        /**
         * @brief    初始化求解器的参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initBoundaryTypeParam() override;
    };

    /**
     * @brief  Wedge 边界类型
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-26
     */
    class FITKInterfaceFlowOFAPI FITKOFBTyWedge : public FITKOFAbsBoundaryType
    {
    public:
        explicit FITKOFBTyWedge();
        virtual ~FITKOFBTyWedge();

    protected:
        /**
         * @brief    初始化求解器的参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initBoundaryTypeParam() override;
    };

    /**
     * @brief  Zero Gradient边界类型
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-26
     */
    class FITKInterfaceFlowOFAPI FITKOFBTyZeroGradient : public FITKOFAbsBoundaryType
    {
    public:
        explicit FITKOFBTyZeroGradient();
        virtual ~FITKOFBTyZeroGradient();

    protected:
        /**
         * @brief    初始化求解器的参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initBoundaryTypeParam() override;
    };

    /**
    * @brief  Fixed Flux Pressure边界类型
    * @author liuzhonghua (liuzhonghuaszch@163.com)
    * @date   2024-08-26
    */
    class FITKInterfaceFlowOFAPI FITKOFBTyFixedFluxPressure : public FITKOFAbsBoundaryType
    {
    public:
        explicit FITKOFBTyFixedFluxPressure();
        virtual ~FITKOFBTyFixedFluxPressure();

    protected:
        /**
         * @brief    初始化求解器的参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initBoundaryTypeParam() override;
    };

    /**
     * @brief  湍流动能Fixed Value边界类型
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-26
     */
    class FITKInterfaceFlowOFAPI FITKOFBTyTKEFixedValue : public FITKOFAbsBoundaryType
    {
    public:
        explicit FITKOFBTyTKEFixedValue();
        virtual ~FITKOFBTyTKEFixedValue();

    protected:
        /**
         * @brief    初始化求解器的参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initBoundaryTypeParam() override;
    };

    /**
     * @brief  速度Fixed Value边界类型
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-26
     */
    class FITKInterfaceFlowOFAPI FITKOFBTyVFixedValue : public FITKOFAbsBoundaryType
    {
    public:
        explicit FITKOFBTyVFixedValue();
        virtual ~FITKOFBTyVFixedValue();

    protected:
        /**
         * @brief    初始化求解器的参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initBoundaryTypeParam() override;
    };

    /**
     * @brief  Fixed Value 无单位 边界类型
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-26
     */
    class FITKInterfaceFlowOFAPI FITKOFBTyFixedValue : public FITKOFAbsBoundaryType
    {
    public:
        explicit FITKOFBTyFixedValue();
        virtual ~FITKOFBTyFixedValue();

    protected:
        /**
         * @brief    初始化求解器的参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initBoundaryTypeParam() override;
    };

    /**
     * @brief  Fixed Value 压力 边界类型
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-26
     */
    class FITKInterfaceFlowOFAPI FITKOFBTyPFixedValue : public FITKOFAbsBoundaryType
    {
    public:
        explicit FITKOFBTyPFixedValue();
        virtual ~FITKOFBTyPFixedValue();

    protected:
        /**
         * @brief    初始化求解器的参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initBoundaryTypeParam() override;
    };

    /**
     * @brief  Standard Wall Function边界类型
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-26
     */
    class FITKInterfaceFlowOFAPI FITKOFBTyStandardWallFunction : public FITKOFAbsBoundaryType
    {
    public:
        explicit FITKOFBTyStandardWallFunction();
        virtual ~FITKOFBTyStandardWallFunction();

    protected:
        /**
         * @brief    初始化求解器的参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initBoundaryTypeParam() override;
    };

    /**
     * @brief  Low Re Wall Function边界类型
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-26
     */
    class FITKInterfaceFlowOFAPI FITKOFBTyVLowReWallFunction : public FITKOFAbsBoundaryType
    {
    public:
        explicit FITKOFBTyVLowReWallFunction();
        virtual ~FITKOFBTyVLowReWallFunction();

    protected:
        /**
         * @brief    初始化求解器的参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initBoundaryTypeParam() override;
    };

    /**
    * @brief  No-Slip边界类型
    * @author liuzhonghua (liuzhonghuaszch@163.com)
    * @date   2024-08-26
    */
    class FITKInterfaceFlowOFAPI FITKOFBTyNoSlip : public FITKOFAbsBoundaryType
    {
    public:
        explicit FITKOFBTyNoSlip();
        virtual ~FITKOFBTyNoSlip();

    protected:
        /**
         * @brief    初始化求解器的参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initBoundaryTypeParam() override;
    };

    /**
     * @brief  Turbulent Intensity Inlet边界类型
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-26
     */
    class FITKInterfaceFlowOFAPI FITKOFBTyTurbulentIntensityInlet : public FITKOFAbsBoundaryType
    {
    public:
        explicit FITKOFBTyTurbulentIntensityInlet();
        virtual ~FITKOFBTyTurbulentIntensityInlet();

    protected:
        /**
         * @brief    初始化求解器的参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initBoundaryTypeParam() override;
    };

    /**
     * @brief  Turbulent Mixing Length Inlet边界类型
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-26
     */
    class FITKInterfaceFlowOFAPI FITKOFBTyTurbulentMixingLengthInlet : public FITKOFAbsBoundaryType
    {
    public:
        explicit FITKOFBTyTurbulentMixingLengthInlet();
        virtual ~FITKOFBTyTurbulentMixingLengthInlet();

    protected:
        /**
         * @brief    初始化求解器的参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initBoundaryTypeParam() override;
    };

    /**
     * @brief  Total Pressure边界类型
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-26
     */
    class FITKInterfaceFlowOFAPI FITKOFBTyTKETotalPressure : public FITKOFAbsBoundaryType
    {
    public:
        explicit FITKOFBTyTKETotalPressure();
        virtual ~FITKOFBTyTKETotalPressure();

    protected:
        /**
         * @brief    初始化求解器的参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initBoundaryTypeParam() override;
    };

    /**
    * @brief  Total Pressure边界类型
    * @author liuzhonghua (liuzhonghuaszch@163.com)
    * @date   2024-08-26
    */
    class FITKInterfaceFlowOFAPI FITKOFBTyPTotalPressure : public FITKOFAbsBoundaryType
    {
    public:
        explicit FITKOFBTyPTotalPressure();
        virtual ~FITKOFBTyPTotalPressure();

    protected:
        /**
         * @brief    初始化求解器的参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initBoundaryTypeParam() override;
    };

    /**
     * @brief  Pressure Inlet-Outlet Velocity边界类型
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-26
     */
    class FITKInterfaceFlowOFAPI FITKOFBTyPressureIOVelocity : public FITKOFAbsBoundaryType
    {
    public:
        explicit FITKOFBTyPressureIOVelocity();
        virtual ~FITKOFBTyPressureIOVelocity();

    protected:
        /**
         * @brief    初始化求解器的参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initBoundaryTypeParam() override;
    };

    /**
     * @brief  Surface Normal Fixed Value边界类型
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-26
     */
    class FITKInterfaceFlowOFAPI FITKOFBTySurNorFixedValue : public FITKOFAbsBoundaryType
    {
    public:
        explicit FITKOFBTySurNorFixedValue();
        virtual ~FITKOFBTySurNorFixedValue();

    protected:
        /**
         * @brief    初始化求解器的参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initBoundaryTypeParam() override;
    };

    /**
     * @brief  Inlet-Outlet边界类型
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-26
     */
    class FITKInterfaceFlowOFAPI FITKOFBTyInletOutlet : public FITKOFAbsBoundaryType
    {
    public:
        explicit FITKOFBTyInletOutlet();
        virtual ~FITKOFBTyInletOutlet();

    protected:
        /**
         * @brief    初始化求解器的参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initBoundaryTypeParam() override;
    };

    /**
     * @brief  Outlet Phase Mean Velocity边界类型
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-26
     */
    class FITKInterfaceFlowOFAPI FITKOFBTyOutletPhaseMeanVelocity : public FITKOFAbsBoundaryType
    {
    public:
        explicit FITKOFBTyOutletPhaseMeanVelocity();
        virtual ~FITKOFBTyOutletPhaseMeanVelocity();

    protected:
        /**
         * @brief    初始化求解器的参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initBoundaryTypeParam() override;
    };

    /**
     * @brief  Variable Height边界类型
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-26
     */
    class FITKInterfaceFlowOFAPI FITKOFBTyVariableHeight : public FITKOFAbsBoundaryType
    {
    public:
        explicit FITKOFBTyVariableHeight();
        virtual ~FITKOFBTyVariableHeight();

    protected:
        /**
         * @brief    初始化求解器的参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initBoundaryTypeParam() override;
    };

    /**
     * @brief  Coupled Temperature and Radiation边界类型
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-26
     */
    class FITKInterfaceFlowOFAPI FITKOFBTyCoupledTemperatureAndRadiation : public FITKOFAbsBoundaryType
    {
    public:
        explicit FITKOFBTyCoupledTemperatureAndRadiation();
        virtual ~FITKOFBTyCoupledTemperatureAndRadiation();

    protected:
        /**
         * @brief    初始化求解器的参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initBoundaryTypeParam() override;
    };

    /**
     * @brief  Empty边界类型
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-26
     */
    class FITKInterfaceFlowOFAPI FITKOFBTyEmpty : public FITKOFAbsBoundaryType
    {
    public:
        explicit FITKOFBTyEmpty();
        virtual ~FITKOFBTyEmpty();

    protected:
        /**
         * @brief    初始化求解器的参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initBoundaryTypeParam() override;
    };

    /**
     * @brief  Total Temperature边界类型
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-26
     */
    class FITKInterfaceFlowOFAPI FITKOFBTyTotalTemperature : public FITKOFAbsBoundaryType
    {
    public:
        explicit FITKOFBTyTotalTemperature();
        virtual ~FITKOFBTyTotalTemperature();

    protected:
        /**
         * @brief    初始化求解器的参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initBoundaryTypeParam() override;
    };
}


#endif
