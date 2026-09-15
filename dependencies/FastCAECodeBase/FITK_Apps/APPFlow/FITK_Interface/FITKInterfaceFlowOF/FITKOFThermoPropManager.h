/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKOFThermoPropManager.h
 * @brief  Thermo 参数管理器
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-06-19
 *********************************************************************/
#ifndef _FITK___OF__THERMO__PARA_MANAGER__H___
#define _FITK___OF__THERMO__PARA_MANAGER__H___

#include "FITKInterfaceFlowOFAPI.h"
#include "FITKOFEnum.hpp"
#include <functional>
#include <QHash>
#include <QStringList>

namespace Interface
{
    class FITKOFAbsThermoPhysicalEquationOfState;
    class FITKOFAbsThermoPhysicalThermodynamics;
    class FITKOFAbsThermoPhysicalTransport;
}

typedef std::function<Interface::FITKOFAbsThermoPhysicalEquationOfState*()>  CREATETHERMOEQUATIONFUN;
typedef std::function<Interface::FITKOFAbsThermoPhysicalThermodynamics*()>  CREATETHERMODYNAMICSFUN;
typedef std::function<Interface::FITKOFAbsThermoPhysicalTransport*()>  CREATETHERMOTRANSPORTFUN;

namespace Interface
{
    /**
     * @brief  EquationOfState 信息 结构体
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-06-20
     */
    struct FITKOFThermoPhysicalEquationOfStateInfo
    {
        QString _equationOfStateName{};
        QList<FITKOFSolverTypeEnum::FITKOFSolverType> _enableType;//SolverType
        CREATETHERMOEQUATIONFUN _createFuns{};
    };
    /**
     * @brief  Thermodynamics 信息结构体
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-06-20
     */
    struct FITKOFThermodynamicsInfo
    {
        QString _thermodynamicsName{};
        QHash<FITKOFSolverTypeEnum::FITKOFSolverType, QList<QString>> _enableType;//SolverType, _equationOfStateName
        CREATETHERMODYNAMICSFUN _createFuns{};
    };
    /**
     * @brief  Transport 信息结构体
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-06-20
     */
    struct FITKOFThermoPhysicalTransportInfo
    {
        QString _transportName{};
        QList<FITKOFSolverTypeEnum::FITKOFSolverType> _enableType;//SolverType
        QHash<QString, QList<QString>> _enableTypeOther;//_equationOfStateName, _thermodynamicsName
        CREATETHERMOTRANSPORTFUN _createFuns{};
    };
    /**
     * @brief  Thermo Para管理器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-21
     */
    class FITKInterfaceFlowOFAPI FITKOFThermoPropManager
    {
    public:
        explicit FITKOFThermoPropManager();
        virtual ~FITKOFThermoPropManager();
        /**
         * @brief    过滤Equation Of State类型
         * @param[i] solverType 
         * @return   QStringList
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-20
         */
        QStringList filterEquationOfState(FITKOFSolverTypeEnum::FITKOFSolverType solverType, Interface::FITKOFSolverTypeEnum::FITKOFRegionMeshType regionMeshType);
        /**
         * @brief    获取Equation Of State信息
         * @param[i] equationOfStateName 
         * @return   FITKOFThermoPhysicalEquationOfStateInfo
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-20
         */
        FITKOFThermoPhysicalEquationOfStateInfo getEquationOfStateInfo(QString equationOfStateName, Interface::FITKOFSolverTypeEnum::FITKOFRegionMeshType regionMeshType);
        /**
         * @brief    过滤Thermodynamics类型
         * @param[i] equationOfStateName 
         * @param[i] solverType 
         * @return   QStringList
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-20
         */
        QStringList filterThermodynamics(QString equationOfStateName, FITKOFSolverTypeEnum::FITKOFSolverType solverType, Interface::FITKOFSolverTypeEnum::FITKOFRegionMeshType regionMeshType);
        /**
         * @brief    获取Thermodynamics信息
         * @param[i] thermodynamicsName 
         * @return   FITKOFThermodynamicsInfo
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-20
         */
        FITKOFThermodynamicsInfo getThermodynamicsInfo(QString thermodynamicsName, Interface::FITKOFSolverTypeEnum::FITKOFRegionMeshType regionMeshType);
        /**
         * @brief    过滤Transport类型
         * @param[i] equationOfStateName 
         * @param[i] thermodynamicsName 
         * @param[i] solverType 
         * @return   QStringList
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-20
         */
        QStringList filterTransport(QString equationOfStateName, QString thermodynamicsName, FITKOFSolverTypeEnum::FITKOFSolverType solverType, Interface::FITKOFSolverTypeEnum::FITKOFRegionMeshType regionMeshType);
        /**
         * @brief    获取Transport信息
         * @param[i] transportName 
         * @return   FITKOFThermoPhysicalTransportInfo
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-20
         */
        FITKOFThermoPhysicalTransportInfo getTransportInfo(QString transportName, Interface::FITKOFSolverTypeEnum::FITKOFRegionMeshType regionMeshType);

    private:
        /**
         * @brief    初始化Equation Of State数据信息
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-20
         */
        void initEquationOfStateFluid();
        /**
         * @brief    初始化Thermodynamics数据信息
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-20
         */
        void initThermodynamicsFluid();
        /**
         * @brief    初始化Transport数据信息
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-20
         */
        void initTransportFluid();
        /**
         * @brief    初始化Equation Of State数据信息
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-20
         */
        void initEquationOfStateSolid();
        /**
         * @brief    初始化Thermodynamics数据信息
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-20
         */
        void initThermodynamicsSolid();
        /**
         * @brief    初始化Transport数据信息
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-20
         */
        void initTransportSolid();

    private:
        /**
         * @brief  Equation Of State数据信息
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-06-20
         */
        QList<FITKOFThermoPhysicalEquationOfStateInfo> m_thermoEquationOfStateFluid{};
        /**
         * @brief  Thermodynamics数据信息
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-06-20
         */
        QList<FITKOFThermodynamicsInfo> m_thermodynamicsFluid{};
        /**
         * @brief  Transport数据信息
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-06-20
         */
        QList<FITKOFThermoPhysicalTransportInfo> m_thermoTransportFluid{};
        /**
         * @brief  Equation Of State数据信息
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-06-20
         */
        QList<FITKOFThermoPhysicalEquationOfStateInfo> m_thermoEquationOfStateSolid{};
        /**
         * @brief  Thermodynamics数据信息
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-06-20
         */
        QList<FITKOFThermodynamicsInfo> m_thermodynamicsSolid{};
        /**
         * @brief  Transport数据信息
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-06-20
         */
        QList<FITKOFThermoPhysicalTransportInfo> m_thermoTransportSolid{};
    };
}


#endif
