/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKOFTransportModel.h
 * @brief  TransportModel数据接口
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-08-23
 *********************************************************************/
#ifndef _FITK_OF_TRANSPORT_MODEL__H___
#define _FITK_OF_TRANSPORT_MODEL__H___
 
#include "FITKInterfaceFlowOFAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"

namespace Core
{
    class FITKParameter;
}

namespace Interface
{
    /**
     * @brief  传输模型类型基类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-06-17
     */
    class FITKInterfaceFlowOFAPI FITKAbsOFTransportModel : public Core::FITKAbstractNDataObject
    {
    public:
        explicit FITKAbsOFTransportModel();
        virtual ~FITKAbsOFTransportModel();

        /**
         * @brief    获取参数
         * @return   Core::FITKParameter *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-23
         */
        Core::FITKParameter* getTransportModelPara();

    protected:
        /**
         * @brief    初始化数据基类
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-17
         */
        virtual void initTransportModelParam() = 0;

    protected:
        /**
         * @brief  传输模型参数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-06-17
         */
        Core::FITKParameter* _transModelPara{};
    };
    /**
     * @brief  Newtonian传输模型类型
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-06-17
     */
    class FITKInterfaceFlowOFAPI FITKOFTransModelNewtonian : public FITKAbsOFTransportModel
    {
    public:
        explicit FITKOFTransModelNewtonian();
        virtual ~FITKOFTransModelNewtonian();

    protected:
        /**
         * @brief    初始化数据
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-17
         */
        virtual void initTransportModelParam() override;
    };
}


#endif
