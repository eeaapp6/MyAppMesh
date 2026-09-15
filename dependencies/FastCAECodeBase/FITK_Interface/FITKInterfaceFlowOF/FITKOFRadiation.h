/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKOFRadiation.h
 * @brief  辐射建模属性
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-06-17
 *********************************************************************/
#ifndef _FITK_OF_Radiation__H___
#define _FITK_OF_Radiation__H___
 
#include "FITKInterfaceFlowOFAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITKOFEnum.hpp"

namespace Core
{
    class FITKParameter;
}

namespace Interface
{
    class FITKOFAbsRadiationModel;
    class FITKOFAbsRadiationAbsorptionModel;
    class FITKOFAbsRadiationScatterModel;
    class FITKOFAbsRadiationSunLoad;
    class FITKOFAbsRadiationSunDirection;

    class FITKInterfaceFlowOFAPI FITKOFRadiationSolarLoad
    {
    public:
        explicit FITKOFRadiationSolarLoad();
        virtual ~FITKOFRadiationSolarLoad();
        /**
         * @brief    获取太阳能负载参数
         * @return   Core::FITKParameter *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-18
         */
        Core::FITKParameter*  getRadiationSolarLoadParams();
        /**
         * @brief    获取太阳辐射方向
         * @return   FITKOFAbsRadiationSunDirection *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-18
         */
        FITKOFAbsRadiationSunDirection* getRadiationSunDirection();
        /**
         * @brief    获取太阳辐射载荷参数
         * @return   FITKOFAbsRadiationSunLoad *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-18
         */
        FITKOFAbsRadiationSunLoad* getRadiationSunLoad();
        /**
         * @brief    设置太阳辐射方向
         * @param[i] sunDirection 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-18
         */
        void setRadiationSunDirection(FITKOFAbsRadiationSunDirection* sunDirection);
        /**
         * @brief    设置太阳辐射载荷参数
         * @param[i] sunLoad 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-18
         */
        void setRadiationSunLoad(FITKOFAbsRadiationSunLoad* sunLoad);

    private:
        /**
         * @brief  太阳能负载参数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-06-18
         */
        Core::FITKParameter* _radiationSolarLoadParams{};
        /**
         * @brief  太阳辐射载荷参数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-06-18
         */
        FITKOFAbsRadiationSunLoad* _sunLoad{};
        /**
         * @brief  太阳辐射方向
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-06-18
         */
        FITKOFAbsRadiationSunDirection* _sunDirection{};
    };
    /**
     * @brief  辐射建模属性
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-09-03
     */
    class FITKInterfaceFlowOFAPI FITKOFRadiation : public Core::FITKAbstractNDataObject
    {
        Q_OBJECT
            FITKCLASS(Interface, FITKOFRadiation);
    public:
        explicit FITKOFRadiation();
        virtual ~FITKOFRadiation();
        /**
         * @brief    获取辐射参数
         * @return   Core::FITKParameter *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-18
         */
        Core::FITKParameter* getRadiationParam();
        /**
         * @brief    获取辐射模型
         * @return   FITKOFAbsRadiationModel *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-18
         */
        FITKOFAbsRadiationModel* getRadiationModel();
        /**
         * @brief    获取吸收系数模型
         * @return   FITKOFAbsRadiationAbsorptionModel *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-18
         */
        FITKOFAbsRadiationAbsorptionModel* getAbsorptionModel();
        /**
         * @brief    获取散射模型
         * @return   FITKOFAbsRadiationScatterModel *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-18
         */
        FITKOFAbsRadiationScatterModel* getScatterModel();
        /**
         * @brief    获取太阳辐射数据
         * @return   FITKOFRadiationSolarLoad *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-18
         */
        FITKOFRadiationSolarLoad* getSolarLoad();
        /**
         * @brief    设置辐射模型
         * @param[i] radiationModel 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-18
         */
        void setRadiationModel(FITKOFAbsRadiationModel* radiationModel);
        /**
         * @brief    设置吸收系数模型
         * @param[i] absorptionModel
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-18
         */
        void setAbsorptionModel(FITKOFAbsRadiationAbsorptionModel* absorptionModel);
        /**
         * @brief    设置散射模型
         * @param[i] scatterModel 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-18
         */
        void setScatterModel(FITKOFAbsRadiationScatterModel* scatterModel);

    protected:
    private:
        /**
         * @brief  辐射参数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-06-18
         */
        Core::FITKParameter* _radiationParams{};
        /**
         * @brief  辐射模型
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-06-18
         */
        FITKOFAbsRadiationModel* _radiationModel{};
        /**
         * @brief  吸收系数模型
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-06-18
         */
        FITKOFAbsRadiationAbsorptionModel* _absorptionModel{};
        /**
         * @brief  散射系数模型
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-06-18
         */
        FITKOFAbsRadiationScatterModel* _scatterModel{};
        /**
         * @brief  太阳辐射源
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-06-18
         */
        FITKOFRadiationSolarLoad* _solarLoad{};
    };
}


#endif
