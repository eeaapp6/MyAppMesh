/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKOFThermo.h
 * @brief  定义材料的热物理属性
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-06-16
 *********************************************************************/
#ifndef _FITK_OF_Thermo__H___
#define _FITK_OF_Thermo__H___
 
#include "FITKInterfaceFlowOFAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITKOFEnum.hpp"

namespace Core
{
    class FITKParameter;
}

namespace Interface
{
    class FITKOFAbsThermoPhysicalEquationOfState;
    class FITKOFAbsThermoPhysicalThermodynamics;
    class FITKOFAbsThermoPhysicalTransport;
    class FITKFluidRegionsMesh;
    /**
     * @brief  ThermoPhysical Prop数据
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-06-19
     */
    class FITKInterfaceFlowOFAPI FITKOFThermoPhysicalProp
    {
    public:
        explicit FITKOFThermoPhysicalProp();
        virtual ~FITKOFThermoPhysicalProp();
        /**
         * @brief    获取区域类型
         * @return   FITKOFSolverTypeEnum::FITKOFRegionMeshType
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-19
         */
        FITKOFSolverTypeEnum::FITKOFRegionMeshType getRegionType();
        /**
         * @brief    设置区域网格类型
         * @param[i] type
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-03
         */
        void setRegionMeshType(FITKOFSolverTypeEnum::FITKOFRegionMeshType type);
        /**
         * @brief    获取Specie参数
         * @return   Core::FITKParameter *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-19
         */
        Core::FITKParameter* getSpeciePara();
        /**
         * @brief    获取状态方程参数
         * @return   FITKOFAbsThermoPhysicalEquationOfState *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-19
         */
        FITKOFAbsThermoPhysicalEquationOfState* getEquationOfStatePara();
        /**
         * @brief    获取热力学属性参数
         * @return   FITKOFAbsThermoPhysicalThermodynamics *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-19
         */
        FITKOFAbsThermoPhysicalThermodynamics* getThermodynamicsPara();
        /**
         * @brief    获取材料属性参数
         * @return   FITKOFAbsThermoPhysicalTransport *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-19
         */
        FITKOFAbsThermoPhysicalTransport* getTransportPara();
        /**
         * @brief    设置状态方程参数
         * @param[i]  equationOfStatePara
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-19
         */
        void setEquationOfStatePara(FITKOFAbsThermoPhysicalEquationOfState* equationOfStatePara);
        /**
         * @brief    设置热力学属性参数
         * @param[i]  thermodynamicsPara
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-19
         */
        void setThermodynamicsPara(FITKOFAbsThermoPhysicalThermodynamics* thermodynamicsPara);
        /**
         * @brief    设置材料属性参数
         * @param[i]  transportPara
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-19
         */
        void setTransportPara(FITKOFAbsThermoPhysicalTransport* transportPara);
        /**
         * @brief    设置区域网格ID
         * @param[i] regionID                    网格区域id
         * @param[i] meshId                      网格边界id
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-14
         */
        void setRegionMeshID(int regionID);
        /**
         * @brief    获取区域网格ID
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-26
         */
        int getRegionMeshID();
        /**
         * @brief    获取区域网格数据
         * @return   FITKFluidRegionsMesh *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-03
         */
        FITKFluidRegionsMesh* getRegionMeshObj();
        
    private:
        /**
         * @brief  网格区域ID
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-07-03
         */
        int _regionMeshID{ -1 };
        /**
         * @brief  区域类型
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-06-19
         */
        FITKOFSolverTypeEnum::FITKOFRegionMeshType _regionType{ FITKOFSolverTypeEnum::FITKOFRegionMeshType::Fluid };
        /**
         * @brief  Specie参数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-06-19
         */
        Core::FITKParameter* _speciePara{};
        /**
         * @brief  状态方程参数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-06-19
         */
        FITKOFAbsThermoPhysicalEquationOfState* _equationOfStatePara{};
        /**
         * @brief  热力学属性参数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-06-19
         */
        FITKOFAbsThermoPhysicalThermodynamics* _thermodynamicsPara{};
        /**
         * @brief  材料属性参数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-06-19
         */
        FITKOFAbsThermoPhysicalTransport* _transportPara{};
    };

    /**
     * @brief  定义材料的热物理属性
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-09-03
     */
    class FITKInterfaceFlowOFAPI FITKOFThermo : public Core::FITKAbstractNDataObject
    {
        Q_OBJECT
            FITKCLASS(Interface, FITKOFThermo);
    public:
        explicit FITKOFThermo();
        virtual ~FITKOFThermo();

        /**
         * @brief    获取热物理区域属性个数
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-03
         */
        int getThermoPropRegionCount();
        /**
         * @brief    通过index获取热物理区域属性
         * @param[i] index
         * @return   FITKOFInitPropRegions *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-17
         */
        FITKOFThermoPhysicalProp* getThermoPropRegionByIndex(int index);
        /**
         * @brief    通过区域类型获取热物理数据
         * @param[i] type
         * @return   QList<FITKOFInitPropRegions*>
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-03
         */
        QList<FITKOFThermoPhysicalProp*> getThermoPropRegionByType(FITKOFSolverTypeEnum::FITKOFRegionMeshType type);
        /**
         * @brief    通过区域网格ID获取热物理区域属性
         * @param[i] regionMeshID
         * @return   FITKOFInitPropRegions *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-03
         */
        FITKOFThermoPhysicalProp* getThermoPropRegionByRegionMeshID(int regionMeshID);
        /**
         * @brief    添加热物理属性
         * @param[i] thermoPhysicalProp 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-19
         */
        void addThermoPropRegion(FITKOFThermoPhysicalProp* thermoPhysicalProp);
        /**
         * @brief    移除热物理属性
         * @param[i] regionMeshID 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-03
         */
        void removeThermoPropRegion(int regionMeshID);
        /**
         * @brief    清理全部数据
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-08
         */
        void clearAllThermoPropRegion();

    protected:
    private:
        /**
         * @brief  热物理属性区域
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-06-19
         */
        QList<FITKOFThermoPhysicalProp*> _regionThermoProp{};
    };
}


#endif
