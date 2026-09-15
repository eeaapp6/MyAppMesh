/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKFlowOFHDF5AdaptorThermo.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFThermo.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFThermoPhysicalEquationOfState.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFThermoPhysicalTransport.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFThermoPhysicalThermodynamics.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKFlowPhysicsHandlerFactory.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFThermoPropManager.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFPhysicsManager.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITKFlowOFHDF5Reader.h"
#include "FITKFlowOFHDF5Writer.h"

//数据层
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"



#include <QDebug>
namespace IO
{
    QString FITKFlowOFHDF5AdaptorThermo::getAdaptorClass()
    {
        return "FITKFlowOFHDF5AdaptorThermo";
    }

    bool FITKFlowOFHDF5AdaptorThermo::adaptR()
    {
        _thermo = dynamic_cast<Interface::FITKOFThermo*>(_dataObj);
        if (!_reader || !_thermo) return false;
        _thermo->clearAllThermoPropRegion();
        //获取工具
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        if (!readNDataObject(_thermo, *_h5Group)) return false;
        bool isR = true;
        //读取数据
        int count = 0;
        tool->readGroupint(*_h5Group, "ThermoPropRegionCount", &count);
        for (int i = 0; i < count; ++i)
        {
            std::string name = std::to_string(i);
            std::tuple<bool, H5::Group> subH5Group = tool->openGp(*_h5Group, name.c_str());
            if (std::get<0>(subH5Group) == false) continue;
            Interface::FITKOFThermoPhysicalProp* thermoPhyProp = new Interface::FITKOFThermoPhysicalProp;
            _thermo->addThermoPropRegion(thermoPhyProp);
            this->thermoPhysicalPropR(thermoPhyProp, std::get<1>(subH5Group));
            this->equationOfStateR(thermoPhyProp, std::get<1>(subH5Group));
            this->thermodynamicsR(thermoPhyProp, std::get<1>(subH5Group));
            this->transportR(thermoPhyProp, std::get<1>(subH5Group));
        }
        return isR;
    }

    bool FITKFlowOFHDF5AdaptorThermo::adaptW()
    {
        _thermo = dynamic_cast<Interface::FITKOFThermo*>(_dataObj);
        if (!_writer || !_thermo) return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        if (!tool) return false;
        if (!writeNDataObject(_thermo, *_h5Group)) return false;
        bool isW = true;
        //写出数据
        int count = _thermo->getThermoPropRegionCount();
        tool->writeGroupAttrInt(*_h5Group, "ThermoPropRegionCount", count);
        for (int i = 0; i < count; ++i)
        {
            Interface::FITKOFThermoPhysicalProp* thermoPhyProp = _thermo->getThermoPropRegionByIndex(i);
            if (!thermoPhyProp) continue;
            std::string name = std::to_string(i);
            std::tuple<bool, H5::Group> subH5Group = tool->createGp(*_h5Group, QString::fromStdString(name));
            if (std::get<0>(subH5Group) == false) continue;
            this->thermoPhysicalPropW(thermoPhyProp, std::get<1>(subH5Group));
            this->equationOfStateW(thermoPhyProp, std::get<1>(subH5Group));
            this->thermodynamicsW(thermoPhyProp, std::get<1>(subH5Group));
            this->transportW(thermoPhyProp, std::get<1>(subH5Group));
        }

        return isW;
    }

    bool FITKFlowOFHDF5AdaptorThermo::thermoPhysicalPropW(Interface::FITKOFThermoPhysicalProp * thermoPhyProp, H5::Group & h5Group)
    {
        if (!thermoPhyProp || !_writer) return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        if (!tool) return false;
        Interface::FITKOFSolverTypeEnum::FITKOFRegionMeshType regionMeshType = thermoPhyProp->getRegionType();
        int regionMeshID = thermoPhyProp->getRegionMeshID();
        tool->writeGroupAttrInt(h5Group, "RegionMeshID", regionMeshID);
        tool->writeGroupAttrInt(h5Group, "RegionMeshType", int(regionMeshType));
        //写出Specie参数
        Core::FITKParameter * speciePara = thermoPhyProp->getSpeciePara();
        if (!speciePara) return false;
        std::tuple<bool, H5::Group> specieParaGroup = tool->createGp(h5Group, "SpeciePara");
        if (std::get<0>(specieParaGroup) == false) return false;
        parameterToolW(std::get<1>(specieParaGroup), speciePara);
        return true;
    }

    bool FITKFlowOFHDF5AdaptorThermo::thermoPhysicalPropR(Interface::FITKOFThermoPhysicalProp * thermoPhyProp, H5::Group & h5Group)
    {
        if (!thermoPhyProp || !_reader) return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        int regionMeshID = 0, regionMeshType = 0;
        tool->readGroupint(h5Group, "RegionMeshID", &regionMeshID);
        tool->readGroupint(h5Group, "RegionMeshType", &regionMeshType);
        thermoPhyProp->setRegionMeshID(regionMeshID);
        thermoPhyProp->setRegionMeshType(Interface::FITKOFSolverTypeEnum::FITKOFRegionMeshType(regionMeshType));
        //读取Specie参数
        std::tuple<bool, H5::Group> specieParaGroup = tool->openGp(h5Group, "SpeciePara");
        if (std::get<0>(specieParaGroup) == false) return false;
        Core::FITKParameter * speciePara = thermoPhyProp->getSpeciePara();
        if (!speciePara) return false;
        parameterToolR(std::get<1>(specieParaGroup), speciePara);
        return true;
    }

    bool FITKFlowOFHDF5AdaptorThermo::equationOfStateW(Interface::FITKOFThermoPhysicalProp * thermoPhyProp, H5::Group & h5Group)
    {
        if (!thermoPhyProp || !_writer) return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        if (!tool) return false;
        //写出状态方程参数
        Interface::FITKOFAbsThermoPhysicalEquationOfState* equationOfState = thermoPhyProp->getEquationOfStatePara();
        if (!equationOfState) return false;
        std::tuple<bool, H5::Group> equationOfStateGroup = tool->createGp(h5Group, "EquationOfState");
        if (std::get<0>(equationOfStateGroup) == false) return false;
        this->writeNDataObject(equationOfState, std::get<1>(equationOfStateGroup));
        tool->writeGroupAttrStr(std::get<1>(equationOfStateGroup), "ModelType", equationOfState->getDataObjectName().toStdString());
        parameterToolW(std::get<1>(equationOfStateGroup), equationOfState->getEquationOfStatePara());
        return true;
    }

    bool FITKFlowOFHDF5AdaptorThermo::equationOfStateR(Interface::FITKOFThermoPhysicalProp * thermoPhyProp, H5::Group & h5Group)
    {
        if (!thermoPhyProp || !_reader) return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        //获取创建工具管理器
        Interface::FITKOFPhysicsManager* physicsManager = Interface::FITKOFPhysicsManager::getInstance();
        if (!physicsManager) return false;
        Interface::FITKOFThermoPropManager* thermoPropMgr = physicsManager->getThermoPropManager();
        if (!thermoPropMgr) return false;
        //读取状态方程参数
        std::tuple<bool, H5::Group> equationOfStateGroup = tool->openGp(h5Group, "EquationOfState");
        if (std::get<0>(equationOfStateGroup) == false) return false;
        std::string modelTypeName;
        tool->readGroupStr(std::get<1>(equationOfStateGroup), "ModelType", modelTypeName);
        Interface::FITKOFThermoPhysicalEquationOfStateInfo info = thermoPropMgr->getEquationOfStateInfo(QString::fromStdString(modelTypeName), thermoPhyProp->getRegionType());
        if (info._equationOfStateName.isEmpty()) return false;
        Interface::FITKOFAbsThermoPhysicalEquationOfState* equationOfState = info._createFuns();
        this->readNDataObject(equationOfState, std::get<1>(equationOfStateGroup));
        parameterToolR(std::get<1>(equationOfStateGroup), equationOfState->getEquationOfStatePara());
        thermoPhyProp->setEquationOfStatePara(equationOfState);
        return true;
    }

    bool FITKFlowOFHDF5AdaptorThermo::thermodynamicsW(Interface::FITKOFThermoPhysicalProp * thermoPhyProp, H5::Group & h5Group)
    {
        if (!thermoPhyProp || !_writer) return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        if (!tool) return false;
        //写出热力学属性参数
        Interface::FITKOFAbsThermoPhysicalThermodynamics* thermodynamics = thermoPhyProp->getThermodynamicsPara();
        if (!thermodynamics) return false;
        std::tuple<bool, H5::Group> thermodynamicsGroup = tool->createGp(h5Group, "Thermodynamics");
        if (std::get<0>(thermodynamicsGroup) == false) return false;
        this->writeNDataObject(thermodynamics, std::get<1>(thermodynamicsGroup));
        tool->writeGroupAttrStr(std::get<1>(thermodynamicsGroup), "ModelType", thermodynamics->getDataObjectName().toStdString());
        parameterToolW(std::get<1>(thermodynamicsGroup), thermodynamics->getThermodynamicsPara());
        return true;
    }

    bool FITKFlowOFHDF5AdaptorThermo::thermodynamicsR(Interface::FITKOFThermoPhysicalProp * thermoPhyProp, H5::Group & h5Group)
    {
        if (!thermoPhyProp || !_reader) return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        //获取创建工具管理器
        Interface::FITKOFPhysicsManager* physicsManager = Interface::FITKOFPhysicsManager::getInstance();
        if (!physicsManager) return false;
        Interface::FITKOFThermoPropManager* thermoPropMgr = physicsManager->getThermoPropManager();
        if (!thermoPropMgr) return false;
        //读取状态方程参数
        std::tuple<bool, H5::Group> thermodynamicsGroup = tool->openGp(h5Group, "Thermodynamics");
        if (std::get<0>(thermodynamicsGroup) == false) return false;
        std::string modelTypeName;
        tool->readGroupStr(std::get<1>(thermodynamicsGroup), "ModelType", modelTypeName);
        Interface::FITKOFThermodynamicsInfo info = thermoPropMgr->getThermodynamicsInfo(QString::fromStdString(modelTypeName), thermoPhyProp->getRegionType());
        if (info._thermodynamicsName.isEmpty()) return false;
        Interface::FITKOFAbsThermoPhysicalThermodynamics* thermodynamics = info._createFuns();
        this->readNDataObject(thermodynamics, std::get<1>(thermodynamicsGroup));
        parameterToolR(std::get<1>(thermodynamicsGroup), thermodynamics->getThermodynamicsPara());
        thermoPhyProp->setThermodynamicsPara(thermodynamics);
        return true;
    }

    bool FITKFlowOFHDF5AdaptorThermo::transportW(Interface::FITKOFThermoPhysicalProp * thermoPhyProp, H5::Group & h5Group)
    {
        if (!thermoPhyProp || !_writer) return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        if (!tool) return false;
        //写出热力学属性参数
        Interface::FITKOFAbsThermoPhysicalTransport* transport = thermoPhyProp->getTransportPara();
        if (!transport) return false;
        std::tuple<bool, H5::Group> transportGroup = tool->createGp(h5Group, "Transport");
        if (std::get<0>(transportGroup) == false) return false;
        this->writeNDataObject(transport, std::get<1>(transportGroup));
        tool->writeGroupAttrStr(std::get<1>(transportGroup), "ModelType", transport->getDataObjectName().toStdString());
        parameterToolW(std::get<1>(transportGroup), transport->getTransportPara());
        return true;
    }

    bool FITKFlowOFHDF5AdaptorThermo::transportR(Interface::FITKOFThermoPhysicalProp * thermoPhyProp, H5::Group & h5Group)
    {
        if (!thermoPhyProp || !_reader) return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        //获取创建工具管理器
        Interface::FITKOFPhysicsManager* physicsManager = Interface::FITKOFPhysicsManager::getInstance();
        if (!physicsManager) return false;
        Interface::FITKOFThermoPropManager* thermoPropMgr = physicsManager->getThermoPropManager();
        if (!thermoPropMgr) return false;
        //读取状态方程参数
        std::tuple<bool, H5::Group> transportGroup = tool->openGp(h5Group, "Transport");
        if (std::get<0>(transportGroup) == false) return false;
        std::string modelTypeName;
        tool->readGroupStr(std::get<1>(transportGroup), "ModelType", modelTypeName);
        Interface::FITKOFThermoPhysicalTransportInfo info = thermoPropMgr->getTransportInfo(QString::fromStdString(modelTypeName), thermoPhyProp->getRegionType());
        if (info._transportName.isEmpty()) return false;
        Interface::FITKOFAbsThermoPhysicalTransport* transport = info._createFuns();
        this->readNDataObject(transport, std::get<1>(transportGroup));
        parameterToolR(std::get<1>(transportGroup), transport->getTransportPara());
        thermoPhyProp->setTransportPara(transport);
        return true;
    }

}
