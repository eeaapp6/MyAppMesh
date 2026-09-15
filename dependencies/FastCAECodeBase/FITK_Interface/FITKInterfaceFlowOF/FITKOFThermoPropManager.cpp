/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFThermoPropManager.h"
#include "FITKOFThermoPhysicalEquationOfState.h"
#include "FITKOFThermoPhysicalTransport.h"
#include "FITKOFThermoPhysicalThermodynamics.h"

#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"

namespace Interface
{
    FITKOFThermoPropManager::FITKOFThermoPropManager()
    {
        this->initEquationOfStateFluid();
        this->initThermodynamicsFluid();
        this->initTransportFluid();
        this->initEquationOfStateSolid();
        this->initThermodynamicsSolid();
        this->initTransportSolid();
    }

    FITKOFThermoPropManager::~FITKOFThermoPropManager()
    {
    }

    QStringList FITKOFThermoPropManager::filterEquationOfState(FITKOFSolverTypeEnum::FITKOFSolverType solverType, Interface::FITKOFSolverTypeEnum::FITKOFRegionMeshType regionMeshType)
    {
        QStringList listType;
        if (regionMeshType == Interface::FITKOFSolverTypeEnum::FITKOFRegionMeshType::Fluid)
        {
            //求解器遍历
            for (QList<FITKOFThermoPhysicalEquationOfStateInfo>::iterator iter = m_thermoEquationOfStateFluid.begin(); iter != m_thermoEquationOfStateFluid.end(); ++iter)
            {
                //进行过滤
                if (iter->_enableType.contains(solverType))
                    listType.append(iter->_equationOfStateName);
            }
        }
        else if (regionMeshType == Interface::FITKOFSolverTypeEnum::FITKOFRegionMeshType::Solid)
        {
            //求解器遍历
            for (QList<FITKOFThermoPhysicalEquationOfStateInfo>::iterator iter = m_thermoEquationOfStateSolid.begin(); iter != m_thermoEquationOfStateSolid.end(); ++iter)
            {
                //进行过滤
                if (iter->_enableType.contains(solverType))
                    listType.append(iter->_equationOfStateName);
            }
        }
        return listType;
    }

    FITKOFThermoPhysicalEquationOfStateInfo FITKOFThermoPropManager::getEquationOfStateInfo(QString equationOfStateName, Interface::FITKOFSolverTypeEnum::FITKOFRegionMeshType regionMeshType)
    {
        if (regionMeshType == Interface::FITKOFSolverTypeEnum::FITKOFRegionMeshType::Fluid)
        {
            for (QList<FITKOFThermoPhysicalEquationOfStateInfo>::iterator iter = m_thermoEquationOfStateFluid.begin(); iter != m_thermoEquationOfStateFluid.end(); ++iter)
            {
                if (iter->_equationOfStateName == equationOfStateName) return *iter;
            }
        }
        else if (regionMeshType == Interface::FITKOFSolverTypeEnum::FITKOFRegionMeshType::Solid)
        {
            for (QList<FITKOFThermoPhysicalEquationOfStateInfo>::iterator iter = m_thermoEquationOfStateSolid.begin(); iter != m_thermoEquationOfStateSolid.end(); ++iter)
            {
                if (iter->_equationOfStateName == equationOfStateName) return *iter;
            }
        }
        return FITKOFThermoPhysicalEquationOfStateInfo();
    }

    QStringList FITKOFThermoPropManager::filterThermodynamics(QString equationOfStateName, FITKOFSolverTypeEnum::FITKOFSolverType solverType, Interface::FITKOFSolverTypeEnum::FITKOFRegionMeshType regionMeshType)
    {
        QStringList listType;
        if (regionMeshType == Interface::FITKOFSolverTypeEnum::FITKOFRegionMeshType::Fluid)
        {
            //求解器遍历
            for (QList<FITKOFThermodynamicsInfo>::iterator iter = m_thermodynamicsFluid.begin(); iter != m_thermodynamicsFluid.end(); ++iter)
            {
                //进行过滤
                if (iter->_enableType.contains(solverType) && iter->_enableType[solverType].contains(equationOfStateName))
                    listType.append(iter->_thermodynamicsName);
            }
        }
        else if (regionMeshType == Interface::FITKOFSolverTypeEnum::FITKOFRegionMeshType::Solid)
        {
            //求解器遍历
            for (QList<FITKOFThermodynamicsInfo>::iterator iter = m_thermodynamicsSolid.begin(); iter != m_thermodynamicsSolid.end(); ++iter)
            {
                //进行过滤
                if (iter->_enableType.contains(solverType) && iter->_enableType[solverType].contains(equationOfStateName))
                    listType.append(iter->_thermodynamicsName);
            }
        }
        return listType;
    }

    FITKOFThermodynamicsInfo FITKOFThermoPropManager::getThermodynamicsInfo(QString thermodynamicsName, Interface::FITKOFSolverTypeEnum::FITKOFRegionMeshType regionMeshType)
    {
        if (regionMeshType == Interface::FITKOFSolverTypeEnum::FITKOFRegionMeshType::Fluid)
        {
            for (QList<FITKOFThermodynamicsInfo>::iterator iter = m_thermodynamicsFluid.begin(); iter != m_thermodynamicsFluid.end(); ++iter)
            {
                if (iter->_thermodynamicsName == thermodynamicsName) return *iter;
            }
        }
        else if (regionMeshType == Interface::FITKOFSolverTypeEnum::FITKOFRegionMeshType::Solid)
        {
            for (QList<FITKOFThermodynamicsInfo>::iterator iter = m_thermodynamicsSolid.begin(); iter != m_thermodynamicsSolid.end(); ++iter)
            {
                if (iter->_thermodynamicsName == thermodynamicsName) return *iter;
            }
        }
        return FITKOFThermodynamicsInfo();
    }

    QStringList FITKOFThermoPropManager::filterTransport(QString equationOfStateName, QString thermodynamicsName, FITKOFSolverTypeEnum::FITKOFSolverType solverType, Interface::FITKOFSolverTypeEnum::FITKOFRegionMeshType regionMeshType)
    {
        QStringList listType;
        if (regionMeshType == Interface::FITKOFSolverTypeEnum::FITKOFRegionMeshType::Fluid)
        {
            //求解器遍历
            for (QList<FITKOFThermoPhysicalTransportInfo>::iterator iter = m_thermoTransportFluid.begin(); iter != m_thermoTransportFluid.end(); ++iter)
            {
                //进行过滤
                if (!iter->_enableType.contains(solverType)) continue;
                if (iter->_enableTypeOther.contains(equationOfStateName) && iter->_enableTypeOther[equationOfStateName].contains(thermodynamicsName))
                    listType.append(iter->_transportName);
            }
        }
        else if (regionMeshType == Interface::FITKOFSolverTypeEnum::FITKOFRegionMeshType::Solid)
        {
            //求解器遍历
            for (QList<FITKOFThermoPhysicalTransportInfo>::iterator iter = m_thermoTransportSolid.begin(); iter != m_thermoTransportSolid.end(); ++iter)
            {
                //进行过滤
                if (!iter->_enableType.contains(solverType)) continue;
                if (iter->_enableTypeOther.contains(equationOfStateName) && iter->_enableTypeOther[equationOfStateName].contains(thermodynamicsName))
                    listType.append(iter->_transportName);
            }
        }
        return listType;
    }

    FITKOFThermoPhysicalTransportInfo FITKOFThermoPropManager::getTransportInfo(QString transportName, Interface::FITKOFSolverTypeEnum::FITKOFRegionMeshType regionMeshType)
    {
        //获取Transport信息
        if (regionMeshType == Interface::FITKOFSolverTypeEnum::FITKOFRegionMeshType::Fluid)
        {
            for (QList<FITKOFThermoPhysicalTransportInfo>::iterator iter = m_thermoTransportFluid.begin(); iter != m_thermoTransportFluid.end(); ++iter)
            {
                if (iter->_transportName == transportName) return *iter;
            }
        }
        else if (regionMeshType == Interface::FITKOFSolverTypeEnum::FITKOFRegionMeshType::Solid)
        {
            for (QList<FITKOFThermoPhysicalTransportInfo>::iterator iter = m_thermoTransportSolid.begin(); iter != m_thermoTransportSolid.end(); ++iter)
            {
                if (iter->_transportName == transportName) return *iter;
            }
        }
        return FITKOFThermoPhysicalTransportInfo();
    }

    void FITKOFThermoPropManager::initEquationOfStateFluid()
    {
        //Perfect Gas
        FITKOFThermoPhysicalEquationOfStateInfo infoPerfectGas;
        infoPerfectGas._equationOfStateName = "Perfect Gas";
        infoPerfectGas._enableType.append(FITKOFSolverTypeEnum::FITKOFSolverType::CHT_MULTI_REGION);
        infoPerfectGas._createFuns = [&] { return new FITKOFThermoPhysicalEquationOfStatePerfectGas; };
        m_thermoEquationOfStateFluid.append(infoPerfectGas);
        //Boussinesq
        FITKOFThermoPhysicalEquationOfStateInfo infoBoussinesq;
        infoBoussinesq._equationOfStateName = "Boussinesq";
        infoBoussinesq._enableType.append(FITKOFSolverTypeEnum::FITKOFSolverType::CHT_MULTI_REGION);
        infoBoussinesq._createFuns = [&] { return new FITKOFThermoPhysicalEquationOfStateBoussinesq; };
        m_thermoEquationOfStateFluid.append(infoBoussinesq);
        //Incompressible Perfect Gas
        FITKOFThermoPhysicalEquationOfStateInfo infoIncompressiblePerfectGas;
        infoIncompressiblePerfectGas._equationOfStateName = "Incompressible Perfect Gas";
        infoIncompressiblePerfectGas._enableType.append(FITKOFSolverTypeEnum::FITKOFSolverType::CHT_MULTI_REGION);
        infoIncompressiblePerfectGas._createFuns = [&] { return new FITKOFThermoPhysicalEquationOfStateIncompressiblePerfectGas; };
        m_thermoEquationOfStateFluid.append(infoIncompressiblePerfectGas);
        //Constant Density
        FITKOFThermoPhysicalEquationOfStateInfo infoConstantDensity;
        infoConstantDensity._equationOfStateName = "Constant Density";
        infoConstantDensity._enableType.append(FITKOFSolverTypeEnum::FITKOFSolverType::CHT_MULTI_REGION);
        infoConstantDensity._createFuns = [&] { return new FITKOFThermoPhysicalEquationOfStateConstantDensity; };
        m_thermoEquationOfStateFluid.append(infoConstantDensity);
        //Perfect Fluid
        FITKOFThermoPhysicalEquationOfStateInfo infoPerfectFluid;
        infoPerfectFluid._equationOfStateName = "Perfect Fluid";
        infoPerfectFluid._enableType.append(FITKOFSolverTypeEnum::FITKOFSolverType::CHT_MULTI_REGION);
        infoPerfectFluid._createFuns = [&] { return new FITKOFThermoPhysicalEquationOfStatePerfectFluid; };
        m_thermoEquationOfStateFluid.append(infoPerfectFluid);
        //Adiabatic Perfect Fluid
        FITKOFThermoPhysicalEquationOfStateInfo infoAdiabaticPerfectFluid;
        infoAdiabaticPerfectFluid._equationOfStateName = "Adiabatic Perfect Fluid";
        infoAdiabaticPerfectFluid._enableType.append(FITKOFSolverTypeEnum::FITKOFSolverType::CHT_MULTI_REGION);
        infoAdiabaticPerfectFluid._createFuns = [&] { return new FITKOFThermoPhysicalEquationOfStateAdiabaticPerfectFluid; };
        m_thermoEquationOfStateFluid.append(infoAdiabaticPerfectFluid);
        //Peng-Robinson
        //FITKOFThermoPhysicalEquationOfStateInfo infoPengRobinson;
        //infoPengRobinson._equationOfStateName = "Peng-Robinson";
        //infoPengRobinson._enableType.append(FITKOFSolverTypeEnum::FITKOFSolverType::CHT_MULTI_REGION);
        //infoPengRobinson._createFuns = [&] { return new FITKOFThermoPhysicalEquationOfStatePengRobinson; };
        //m_thermoEquationOfStateFluid.append(infoPengRobinson);
    }

    void FITKOFThermoPropManager::initThermodynamicsFluid()
    {
        //Constant
        FITKOFThermodynamicsInfo infoConstant;
        infoConstant._thermodynamicsName = "Constant";
        infoConstant._enableType.insert(FITKOFSolverTypeEnum::FITKOFSolverType::CHT_MULTI_REGION, QStringList() << "Perfect Gas" << "Boussinesq" << "Incompressible Perfect Gas" << "Constant Density"
        << "Perfect Fluid" << "Adiabatic Perfect Fluid" << "Peng-Robinson");
        infoConstant._createFuns = [&] { return new FITKOFThermoPhysicalThermodynamicsConstant; };
        m_thermodynamicsFluid.append(infoConstant);
        
    }

    void FITKOFThermoPropManager::initTransportFluid()
    {
        //Constant
        FITKOFThermoPhysicalTransportInfo infoConstant;
        infoConstant._transportName = "Constant";
        infoConstant._enableType.append(FITKOFSolverTypeEnum::FITKOFSolverType::CHT_MULTI_REGION);
        infoConstant._enableTypeOther.insert("Perfect Gas", QStringList() << "Constant");
        infoConstant._enableTypeOther.insert("Boussinesq", QStringList() << "Constant");
        infoConstant._enableTypeOther.insert("Incompressible Perfect Gas", QStringList() << "Constant");
        infoConstant._enableTypeOther.insert("Constant Density", QStringList() << "Constant");
        infoConstant._enableTypeOther.insert("Perfect Fluid", QStringList() << "Constant");
        infoConstant._enableTypeOther.insert("Adiabatic Perfect Fluid", QStringList() << "Constant");
        infoConstant._createFuns = [&] { return new FITKOFThermoPhysicalTransportConstant; };
        m_thermoTransportFluid.append(infoConstant);

        //Sutherland
        //FITKOFThermoPhysicalTransportInfo infoSutherland;
        //infoSutherland._transportName = "Sutherland";
        //infoSutherland._enableType.append(FITKOFSolverTypeEnum::FITKOFSolverType::CHT_MULTI_REGION);
        //infoSutherland._enableTypeOther.insert("Perfect Gas", QStringList() << "Constant");
        //infoSutherland._enableTypeOther.insert("Boussinesq", QStringList() << "Constant");
        //infoSutherland._enableTypeOther.insert("Incompressible Perfect Gas", QStringList() << "Constant");
        //infoSutherland._enableTypeOther.insert("Peng-Robinson", QStringList() << "Constant");
        //infoSutherland._createFuns = [&] { return new FITKOFThermoPhysicalTransportConstant; };
        //m_thermoTransport.append(infoSutherland);
    }

    void FITKOFThermoPropManager::initEquationOfStateSolid()
    {
        //Constant Density
        FITKOFThermoPhysicalEquationOfStateInfo infoConstantDensity;
        infoConstantDensity._equationOfStateName = "Constant Density";
        infoConstantDensity._enableType.append(FITKOFSolverTypeEnum::FITKOFSolverType::CHT_MULTI_REGION);
        infoConstantDensity._createFuns = [&] { return new FITKOFThermoPhysicalEquationOfStateConstantDensity; };
        m_thermoEquationOfStateSolid.append(infoConstantDensity);
    }

    void FITKOFThermoPropManager::initThermodynamicsSolid()
    {
        //Constant
        FITKOFThermodynamicsInfo infoConstant;
        infoConstant._thermodynamicsName = "eConstant";
        infoConstant._enableType.insert(FITKOFSolverTypeEnum::FITKOFSolverType::CHT_MULTI_REGION, QStringList() << "Constant Density");
        infoConstant._createFuns = [&] { return new FITKOFThermoPhysicalThermodynamicsEConstant; };
        m_thermodynamicsSolid.append(infoConstant);
    }

    void FITKOFThermoPropManager::initTransportSolid()
    {
        //Constant Isotropic
        FITKOFThermoPhysicalTransportInfo infoConstantIsotropic;
        infoConstantIsotropic._transportName = "Constant Isotropic";
        infoConstantIsotropic._enableType.append(FITKOFSolverTypeEnum::FITKOFSolverType::CHT_MULTI_REGION);
        infoConstantIsotropic._enableTypeOther.insert("Constant Density", QStringList() << "eConstant");
        infoConstantIsotropic._createFuns = [&] { return new FITKOFThermoPhysicalTransportConstantIsotropic; };
        m_thermoTransportSolid.append(infoConstantIsotropic);
    }

}