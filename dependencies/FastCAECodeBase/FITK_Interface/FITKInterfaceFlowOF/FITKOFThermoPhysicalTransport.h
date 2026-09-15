/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKOFThermoPhysicalTransport.h
 * @brief  Transport 数据接口
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-06-19
 *********************************************************************/
#ifndef _FITK_OF_THERMOPHYSICAL_TRANSPORT__H___
#define _FITK_OF_THERMOPHYSICAL_TRANSPORT__H___
 
#include "FITKInterfaceFlowOFAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"

namespace Core
{
    class FITKParameter;
}

namespace Interface
{
    /**
     * @brief  Transport 数据基类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-06-19
     */
    class FITKInterfaceFlowOFAPI FITKOFAbsThermoPhysicalTransport : public Core::FITKAbstractNDataObject
    {
    public:
        explicit FITKOFAbsThermoPhysicalTransport();
        virtual ~FITKOFAbsThermoPhysicalTransport();
        
        /**
         * @brief    获取Transport参数
         * @return   Core::FITKParameter *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-19
         */
        Core::FITKParameter* getTransportPara();

    protected:
        /**
         * @brief    初始化Transport的参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initTransportParam() = 0;

    protected:
        /**
         * @brief  transport 参数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-06-19
         */
        Core::FITKParameter* _transportPara{};
    };
    /**
     * @brief  Constant - Transport 数据基类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-06-19
     */
    class FITKInterfaceFlowOFAPI FITKOFThermoPhysicalTransportConstant : public FITKOFAbsThermoPhysicalTransport
    {
    public:
        explicit FITKOFThermoPhysicalTransportConstant();
        virtual ~FITKOFThermoPhysicalTransportConstant();

    protected:
        /**
         * @brief    初始化Transport的参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initTransportParam() override;

    };

    /**
     * @brief  Sutherland - Transport 数据基类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-06-19
     */
    class FITKInterfaceFlowOFAPI FITKOFThermoPhysicalTransportSutherland : public FITKOFAbsThermoPhysicalTransport
    {
    public:
        explicit FITKOFThermoPhysicalTransportSutherland();
        virtual ~FITKOFThermoPhysicalTransportSutherland();

    protected:
        /**
         * @brief    初始化Transport的参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initTransportParam() override;

    };

    /**
     * @brief  Constant Isotropic - Transport 数据基类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-06-19
     */
    class FITKInterfaceFlowOFAPI FITKOFThermoPhysicalTransportConstantIsotropic : public FITKOFAbsThermoPhysicalTransport
    {
    public:
        explicit FITKOFThermoPhysicalTransportConstantIsotropic();
        virtual ~FITKOFThermoPhysicalTransportConstantIsotropic();

    protected:
        /**
         * @brief    初始化Transport的参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initTransportParam() override;

    };
}


#endif
