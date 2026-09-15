/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKOFRadiationModel.h
 * @brief  Radiation Model 数据接口
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-06-18
 *********************************************************************/
#ifndef _FITK_OF_RADIATION_MODEL__H___
#define _FITK_OF_RADIATION_MODEL__H___
 
#include "FITKInterfaceFlowOFAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"

namespace Core
{
    class FITKParameter;
}

namespace Interface
{
    /**
     * @brief  Radiation Model数据基类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-22
     */
    class FITKInterfaceFlowOFAPI FITKOFAbsRadiationModel : public Core::FITKAbstractNDataObject
    {
    public:
        explicit FITKOFAbsRadiationModel();
        virtual ~FITKOFAbsRadiationModel();

        /**
         * @brief    获取参数
         * @return   Core::FITKParameter *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        Core::FITKParameter* getRadiationModelPara();

    protected:
        /**
         * @brief    初始化辐射模型的参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initRadiationModelParam() = 0;

    protected:
        /**
         * @brief  Radiation Model的参数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-08-22
         */
        Core::FITKParameter* _radiationModelParams{};
    };

    /**
     * @brief  none Radiation Model数据基类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-22
     */
    class FITKInterfaceFlowOFAPI FITKOFRadiationModelNone : public FITKOFAbsRadiationModel
    {
    public:
        explicit FITKOFRadiationModelNone();
        virtual ~FITKOFRadiationModelNone();

    protected:
        /**
         * @brief    初始化求解器的参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initRadiationModelParam() override;
    };
    /**
     * @brief  P1 Radiation Model数据类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-22
     */
    class FITKInterfaceFlowOFAPI FITKOFRadiationModelP1 : public FITKOFAbsRadiationModel
    {
    public:
        explicit FITKOFRadiationModelP1();
        virtual ~FITKOFRadiationModelP1();

    protected:
        /**
         * @brief    初始化求解器的参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initRadiationModelParam() override;
    };
    /**
     * @brief  Discrete Ordinates Radiation Model数据类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-06-18
     */
    class FITKInterfaceFlowOFAPI FITKOFRadiationModelDiscreteOrdinates : public FITKOFAbsRadiationModel
    {
    public:
        explicit FITKOFRadiationModelDiscreteOrdinates();
        virtual ~FITKOFRadiationModelDiscreteOrdinates();

    protected:
        /**
         * @brief    初始化求解器的参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initRadiationModelParam() override;
    };
    /**
     * @brief  Surface to Surface Radiation Model数据类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-06-18
     */
    class FITKInterfaceFlowOFAPI FITKOFRadiationModelSurfToSurf : public FITKOFAbsRadiationModel
    {
    public:
        explicit FITKOFRadiationModelSurfToSurf();
        virtual ~FITKOFRadiationModelSurfToSurf();

    protected:
        /**
         * @brief    初始化求解器的参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        virtual void initRadiationModelParam() override;
    };
}


#endif
