/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKFlowOFHDF5AdaptorPhysicsData.h"
#include "FITKFlowOFHDF5AdaptorTurbulence.h"
#include "FITKFlowOFHDF5AdaptorThermo.h"
#include "FITKFlowOFHDF5AdaptorTransportProp.h"
#include "FITKFlowOFHDF5AdaptorDiscretization.h"
#include "FITKFlowOFHDF5AdaptorSolution.h"
#include "FITKFlowOFHDF5AdaptorInitialCondition.h"
#include "FITKFlowOFHDF5AdaptorRunControl.h"
#include "FITKFlowOFHDF5AdaptorBoundary.h"
#include "FITKFlowOFHDF5AdaptorRadiation.h"
#include "FITKFlowOFHDF5AdaptorOperatingCond.h"

#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFPhysicsData.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKAbstractOFSolver.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKAbstractOFTurbulence.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFTransportProp.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFDiscretization.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFBoundary.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFSolution.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFInitialConditions.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFRunControl.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFOperatingConditions.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFRadiation.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFThermo.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKFlowPhysicsHandlerFactory.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
//求解器
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFSolverSIMPLE.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFSolverInter.h"

#include "FITKFlowOFHDF5Reader.h"
#include "FITKFlowOFHDF5Writer.h"


#include <QDebug>
namespace IO
{
    QString FITKFlowOFHDF5AdaptorPhysicsData::getAdaptorClass()
    {
        return "FITKFlowOFHDF5AdaptorPhysicsData";
    }

    bool FITKFlowOFHDF5AdaptorPhysicsData::adaptR()
    {
        _physicsData = dynamic_cast<Interface::FITKOFPhysicsData*>(_dataObj);
        if (!_reader || !_physicsData) return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        if (!readDataObject(_physicsData, *_h5Group))return false;
        //是否启用湍流数据
        bool enableTurb = false;
        tool->readGroupBool(*_h5Group, "enableTurb", &enableTurb);
        _physicsData->setEnableTurbulenceEqu(enableTurb);

        bool isR = true;
        //必须先求解器后湍流
        isR &= this->physicsSolverR();
        isR &= this->physicsRegionMeshTypeR();
        isR &= this->physicsTurbulenceR();
        isR &= this->physicsRadiationR();
        isR &= this->physicsThermoR();
        isR &= this->physicsTransportPropR();
        isR &= this->physicsDiscretizationR();
        isR &= this->physicsSolutionR();
        isR &= this->physicsInitialConditionsR();
        isR &= this->physicsBoundaryR();
        isR &= this->physicsRunControlR();
        isR &= this->physicsOperatingConditionsR();

        return isR;
    }

    bool FITKFlowOFHDF5AdaptorPhysicsData::adaptW()
    {
        _physicsData = dynamic_cast<Interface::FITKOFPhysicsData*>(_dataObj);
        if (!_writer || !_physicsData) return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        if (!tool) return false;
        bool isW = true;
        if (!writeDataObject(_physicsData, *_h5Group))return false;
        //是否启用湍流数据
        bool enableTurb = _physicsData->isEnableTurbulenceEqu();
        tool->writeGroupAttrBool(*_h5Group, "enableTurb", enableTurb);
        //写出湍流数据
        if (_physicsData->getSolver())
            isW &= this->physicsSolverW();
        isW &= this->physicsRegionMeshTypeW();
        if (_physicsData->getTurbulence())
            isW &= this->physicsTurbulenceW();
        if (_physicsData->getRadiation())
            isW &= this->physicsRadiationW();
        if (_physicsData->getThermo())
            isW &= this->physicsThermoW();
        if (_physicsData->getTransportProp())
            isW &= this->physicsTransportPropW();
        if (_physicsData->getDiscretization())
            isW &= this->physicsDiscretizationW();
        if (_physicsData->getSolution())
            isW &= this->physicsSolutionW();
        if (_physicsData->getInitialConditions())
            isW &= this->physicsInitialConditionsW();
        if (_physicsData->getBoundaryManager())
            isW &= this->physicsBoundaryW();
        if (_physicsData->getRunControl())
            isW &= this->physicsRunControlW();
        if (_physicsData->getOperatingConditions())
            isW &= this->physicsOperatingConditionsW();

        return isW;
    }

    bool FITKFlowOFHDF5AdaptorPhysicsData::physicsSolverW()
    {
        if (!_physicsData || !_writer)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        if (!tool) return false;
        std::tuple<bool, H5::Group> subH5Group = tool->createGp(*_h5Group, "Solver");
        if (std::get<0>(subH5Group) == false) return false;
        Interface::FITKAbstractOFSolver* solverData = _physicsData->getSolver();
        if (!solverData)return false;
        //写出求解器
        if (!writeNDataObject(solverData, std::get<1>(subH5Group))) return false;
        std::string solverName = solverData->getDataObjectName().toStdString();
        if (solverName.empty())return false;
        tool->writeGroupAttrStr(std::get<1>(subH5Group), "solverName", solverName);
        return true;
    }

    bool FITKFlowOFHDF5AdaptorPhysicsData::physicsSolverR()
    {
        if (!_physicsData || !_reader)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        std::tuple<bool, H5::Group> subH5Group = tool->openGp(*_h5Group, "Solver");
        if (std::get<0>(subH5Group) == false) return true;
        Interface::FITKFlowPhysicsHandlerFactory* factoryData = FITKAPP->getComponents()->getComponentTByName<Interface::FITKFlowPhysicsHandlerFactory>("FITKFlowPhysicsHandlerFactory");
        if (!factoryData) return false;
        std::string solverName;
        if (!tool->readGroupStr(std::get<1>(subH5Group), "solverName", solverName))
            return false;
        factoryData->setSolver(QString::fromStdString(solverName));
        Interface::FITKAbstractOFSolver* solverData = _physicsData->getSolver();
        if (!solverData)return false;
        if (!readNDataObject(solverData, std::get<1>(subH5Group)))return false;
        return true;
    }

    bool FITKFlowOFHDF5AdaptorPhysicsData::physicsRegionMeshTypeW()
    {
        if (!_physicsData || !_writer)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        if (!tool) return false;
        //写出区域类型
        QHash<int, Interface::FITKOFSolverTypeEnum::FITKOFRegionMeshType> regionMeshType = _physicsData->getAllRegionMeshType();
        std::vector<std::vector<int>> regionMeshTypeData;
        for (QHash<int, Interface::FITKOFSolverTypeEnum::FITKOFRegionMeshType>::iterator iter = regionMeshType.begin(); iter != regionMeshType.end(); ++iter)
        {
            std::vector<int> data;
            data.push_back(iter.key());
            data.push_back(int(iter.value()));
            regionMeshTypeData.push_back(data);
        }
        tool->CreateTwoArrayDataSetInt(*_h5Group, "MappedBoundary", regionMeshTypeData);
        return true;
    }

    bool FITKFlowOFHDF5AdaptorPhysicsData::physicsRegionMeshTypeR()
    {
        if (!_physicsData || !_reader)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        Interface::FITKFlowPhysicsHandlerFactory* factoryData = FITKAPP->getComponents()->getComponentTByName<Interface::FITKFlowPhysicsHandlerFactory>("FITKFlowPhysicsHandlerFactory");
        if (!factoryData) return false;
        //读取区域类型
        std::vector<std::vector<int>> regionMeshTypeData;
        tool->ReadTwoArrayDataSetInt(*_h5Group, "MappedBoundary", regionMeshTypeData);
        for (std::vector<std::vector<int>>::iterator iter = regionMeshTypeData.begin(); iter != regionMeshTypeData.end(); ++iter)
        {
            factoryData->setRegionMeshType((*iter)[0], Interface::FITKOFSolverTypeEnum::FITKOFRegionMeshType((*iter)[1]));
        }
        return true;
    }

    bool FITKFlowOFHDF5AdaptorPhysicsData::physicsRadiationW()
    {
        if (!_physicsData || !_writer)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        if (!tool) return false;
        H5::Group h5Group = _h5Group->createGroup("Radiation");
        Interface::FITKOFRadiation* radiation = _physicsData->getRadiation();
        if (!radiation)return false;
        //写出辐射类型
        auto adaptor = FITKIOADAPTORFACTORY->createT<FITKFlowOFHDF5AdaptorRadiation>("HDF5", "Interface::FITKOFRadiation");
        if (adaptor == nullptr) return false;
        adaptor->setH5Group(&h5Group);
        adaptor->setDataObject(radiation);
        adaptor->setWriter(_writer);
        bool isW = adaptor->adaptW();
        if (!isW) consoleMessage(3, QString("Write Radiation Errors ! "));
        delete adaptor;
        return isW;
    }

    bool FITKFlowOFHDF5AdaptorPhysicsData::physicsRadiationR()
    {
        if (!_physicsData || !_reader)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        std::tuple<bool, H5::Group> subH5Group = tool->openGp(*_h5Group, "Radiation");
        if (std::get<0>(subH5Group) == false) return true;
        Interface::FITKOFRadiation* radiation = _physicsData->getRadiation();
        if (!radiation)return false;
        //读取辐射类型
        auto adaptor = FITKIOADAPTORFACTORY->createT<FITKFlowOFHDF5AdaptorRadiation>("HDF5", "Interface::FITKOFRadiation");
        if (adaptor == nullptr) return false;
        adaptor->setH5Group(&std::get<1>(subH5Group));
        adaptor->setDataObject(radiation);
        adaptor->setReader(_reader);
        bool isR = adaptor->adaptR();
        if (!isR) consoleMessage(3, QString("Reader Radiation Errors ! "));
        delete adaptor;
        return isR;
    }

    bool FITKFlowOFHDF5AdaptorPhysicsData::physicsThermoW()
    {
        if (!_physicsData || !_writer)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        if (!tool) return false;
        H5::Group h5Group = _h5Group->createGroup("Thermo");
        Interface::FITKOFThermo* thermo = _physicsData->getThermo();
        if (!thermo)return false;
        //写出辐射类型
        auto adaptor = FITKIOADAPTORFACTORY->createT<FITKFlowOFHDF5AdaptorThermo>("HDF5", "Interface::FITKOFThermo");
        if (adaptor == nullptr) return false;
        adaptor->setH5Group(&h5Group);
        adaptor->setDataObject(thermo);
        adaptor->setWriter(_writer);
        bool isW = adaptor->adaptW();
        if (!isW) consoleMessage(3, QString("Write Thermo Errors ! "));
        delete adaptor;
        return isW;
    }

    bool FITKFlowOFHDF5AdaptorPhysicsData::physicsThermoR()
    {
        if (!_physicsData || !_reader)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        std::tuple<bool, H5::Group> subH5Group = tool->openGp(*_h5Group, "Thermo");
        if (std::get<0>(subH5Group) == false) return true;
        Interface::FITKOFThermo* thermo = _physicsData->getThermo();
        if (!thermo)return false;
        //读取辐射类型
        auto adaptor = FITKIOADAPTORFACTORY->createT<FITKFlowOFHDF5AdaptorThermo>("HDF5", "Interface::FITKOFThermo");
        if (adaptor == nullptr) return false;
        adaptor->setH5Group(&std::get<1>(subH5Group));
        adaptor->setDataObject(thermo);
        adaptor->setReader(_reader);
        bool isR = adaptor->adaptR();
        if (!isR) consoleMessage(3, QString("Reader Radiation Errors ! "));
        delete adaptor;
        return isR;
    }

    bool FITKFlowOFHDF5AdaptorPhysicsData::physicsTurbulenceW()
    {
        if (!_physicsData || !_writer)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        if (!tool) return false;
        H5::Group h5Group = _h5Group->createGroup("Turbulence");
        Interface::FITKAbstractOFTurbulence* turbulence = _physicsData->getTurbulence();
        if (!turbulence)return false;
        //写出湍流类型
        std::string turbulenceType = turbulence->getDataObjectName().toStdString();
        tool->writeGroupAttrStr(h5Group, "turbulenceType", turbulenceType);
        auto adaptor = FITKIOADAPTORFACTORY->createT<FITKFlowOFHDF5AdaptorTurbulence>("HDF5", "Interface::FITKAbstractOFTurbulence");
        if (adaptor == nullptr) return false;
        adaptor->setH5Group(&h5Group);
        adaptor->setDataObject(turbulence);
        adaptor->setWriter(_writer);
        bool isW = adaptor->adaptW();
        if (!isW) consoleMessage(3, QString("Write Turbulence Errors ! "));
        delete adaptor;
        return isW;
    }
    bool FITKFlowOFHDF5AdaptorPhysicsData::physicsTurbulenceR()
    {
        if (!_physicsData || !_reader)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        if (!_h5Group->nameExists("Turbulence"))return true;
        H5::Group h5Group = _h5Group->openGroup("Turbulence");
        //读取湍流类型
        std::string turbulenceType;
        if (!tool->readGroupStr(h5Group, "turbulenceType", turbulenceType))
            return false;
        Interface::FITKFlowPhysicsHandlerFactory* factoryData = FITKAPP->getComponents()->getComponentTByName<Interface::FITKFlowPhysicsHandlerFactory>("FITKFlowPhysicsHandlerFactory");
        if (!factoryData) return false;
        factoryData->setTurbence(QString::fromStdString(turbulenceType));
        Interface::FITKAbstractOFTurbulence* turbulence = _physicsData->getTurbulence();
        if (!turbulence)return false;
        auto adaptor = FITKIOADAPTORFACTORY->createT<FITKFlowOFHDF5AdaptorTurbulence>("HDF5", "Interface::FITKAbstractOFTurbulence");
        if (adaptor == nullptr) return false;
        adaptor->setH5Group(&h5Group);
        adaptor->setDataObject(turbulence);
        adaptor->setReader(_reader);
        bool isR = adaptor->adaptR();
        if (!isR) consoleMessage(3, QString("Reader Turbulence Errors ! "));
        delete adaptor;
        return isR;
    }
    bool FITKFlowOFHDF5AdaptorPhysicsData::physicsTransportPropW()
    {
        if (!_physicsData || !_writer)return false;
        H5::Group h5Group = _h5Group->createGroup("TransportProp");
        Interface::FITKOFTransportProp* transportProp = _physicsData->getTransportProp();
        if (!transportProp) return false;
        auto adaptor = FITKIOADAPTORFACTORY->createT<FITKFlowOFHDF5AdaptorTransportProp>("HDF5", transportProp);
        if (adaptor == nullptr) return false;
        adaptor->setH5Group(&h5Group);
        adaptor->setDataObject(transportProp);
        adaptor->setWriter(_writer);
        bool isW = adaptor->adaptW();
        if (!isW) consoleMessage(3, QString("Write transportProp Errors ! "));
        delete adaptor;
        return isW;
    }
    bool FITKFlowOFHDF5AdaptorPhysicsData::physicsTransportPropR()
    {
        if (!_physicsData || !_reader)return false;
        if (!_h5Group->nameExists("TransportProp")) return true;
        H5::Group h5Group = _h5Group->openGroup("TransportProp");
        Interface::FITKOFTransportProp* transportProp = _physicsData->getTransportProp();
        if (!transportProp) return false;
        auto adaptor = FITKIOADAPTORFACTORY->createT<FITKFlowOFHDF5AdaptorTransportProp>("HDF5", transportProp);
        if (adaptor == nullptr) return false;
        adaptor->setH5Group(&h5Group);
        adaptor->setDataObject(transportProp);
        adaptor->setReader(_reader);
        bool isR = adaptor->adaptR();
        if (!isR) consoleMessage(3, QString("Read transportProp Errors ! "));
        delete adaptor;
        return isR;
    }
    bool FITKFlowOFHDF5AdaptorPhysicsData::physicsDiscretizationW()
    {
        if (!_physicsData || !_writer)return false;
        H5::Group h5Group = _h5Group->createGroup("Discretization");
        Interface::FITKOFDiscretization* discretization = _physicsData->getDiscretization();
        if (!discretization) return false;
        auto adaptor = FITKIOADAPTORFACTORY->createT<FITKFlowOFHDF5AdaptorDiscretization>("HDF5", discretization);
        if (adaptor == nullptr) return false;
        adaptor->setH5Group(&h5Group);
        adaptor->setDataObject(discretization);
        adaptor->setWriter(_writer);
        bool isW = adaptor->adaptW();
        if (!isW) consoleMessage(3, QString("Write Discretization Errors ! "));
        delete adaptor;
        return isW;
    }
    bool FITKFlowOFHDF5AdaptorPhysicsData::physicsDiscretizationR()
    {
        if (!_physicsData || !_reader)return false;
        if (!_h5Group->nameExists("Discretization")) return true;
        H5::Group h5Group = _h5Group->openGroup("Discretization");
        Interface::FITKOFDiscretization* discretization = _physicsData->getDiscretization();
        if (!discretization) return false;
        auto adaptor = FITKIOADAPTORFACTORY->createT<FITKFlowOFHDF5AdaptorDiscretization>("HDF5", discretization);
        if (adaptor == nullptr) return false;
        adaptor->setH5Group(&h5Group);
        adaptor->setDataObject(discretization);
        adaptor->setReader(_reader);
        bool isR = adaptor->adaptR();
        if (!isR) consoleMessage(3, QString("Read Discretization Errors ! "));
        delete adaptor;
        return isR;
    }
    bool FITKFlowOFHDF5AdaptorPhysicsData::physicsSolutionW()
    {
        if (!_physicsData || !_writer)return false;
        H5::Group h5Group = _h5Group->createGroup("Solution");
        Interface::FITKOFSolution* solution = _physicsData->getSolution();
        if (!solution) return false;
        auto adaptor = FITKIOADAPTORFACTORY->createT<FITKFlowOFHDF5AdaptorSolution>("HDF5", solution);
        if (adaptor == nullptr) return false;
        adaptor->setH5Group(&h5Group);
        adaptor->setDataObject(solution);
        adaptor->setWriter(_writer);
        bool isW = adaptor->adaptW();
        if (!isW) consoleMessage(3, QString("Write Solution Errors ! "));
        delete adaptor;
        return isW;
    }
    bool FITKFlowOFHDF5AdaptorPhysicsData::physicsSolutionR()
    {
        if (!_physicsData || !_reader)return false;
        if (!_h5Group->nameExists("Solution")) return true;
        H5::Group h5Group = _h5Group->openGroup("Solution");
        Interface::FITKOFSolution* solution = _physicsData->getSolution();
        if (!solution) return false;
        auto adaptor = FITKIOADAPTORFACTORY->createT<FITKFlowOFHDF5AdaptorSolution>("HDF5", solution);
        if (adaptor == nullptr) return false;
        adaptor->setH5Group(&h5Group);
        adaptor->setDataObject(solution);
        adaptor->setReader(_reader);
        bool isR = adaptor->adaptR();
        if (!isR) consoleMessage(3, QString("Read Solution Errors ! "));
        delete adaptor;
        return isR;
    }
    bool FITKFlowOFHDF5AdaptorPhysicsData::physicsInitialConditionsW()
    {
        if (!_physicsData || !_writer)return false;
        H5::Group h5Group = _h5Group->createGroup("InitialConditions");
        Interface::FITKOFInitialConditions* initialCond = _physicsData->getInitialConditions();
        if (!initialCond) return false;
        auto adaptor = FITKIOADAPTORFACTORY->createT<FITKFlowOFHDF5AdaptorInitialCondition>("HDF5", initialCond);
        if (adaptor == nullptr) return false;
        adaptor->setH5Group(&h5Group);
        adaptor->setDataObject(initialCond);
        adaptor->setWriter(_writer);
        bool isW = adaptor->adaptW();
        if (!isW) consoleMessage(3, QString("Write InitialCondition Errors ! "));
        delete adaptor;
        return isW;
    }
    bool FITKFlowOFHDF5AdaptorPhysicsData::physicsInitialConditionsR()
    {
        if (!_physicsData || !_reader)return false;
        if (!_h5Group->nameExists("InitialConditions")) return true;
        H5::Group h5Group = _h5Group->openGroup("InitialConditions");
        Interface::FITKOFInitialConditions* initialCond = _physicsData->getInitialConditions();
        if (!initialCond) return false;
        auto adaptor = FITKIOADAPTORFACTORY->createT<FITKFlowOFHDF5AdaptorInitialCondition>("HDF5", initialCond);
        if (adaptor == nullptr) return false;
        adaptor->setH5Group(&h5Group);
        adaptor->setDataObject(initialCond);
        adaptor->setReader(_reader);
        bool isR = adaptor->adaptR();
        if (!isR) consoleMessage(3, QString("Read InitialCondition Errors ! "));
        delete adaptor;
        return isR;
    }
    bool FITKFlowOFHDF5AdaptorPhysicsData::physicsBoundaryW()
    {
        if (!_physicsData || !_writer)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        if (!tool) return false;
        bool isW = true;
        H5::Group h5Group = _h5Group->createGroup("Bounadry");
        Interface::FITKOFBoundaryManager* boundaryMgr = _physicsData->getBoundaryManager();
        if (!boundaryMgr)return false;
        //写出映射
        QHash<int, int> mappedBoundary = boundaryMgr->getMappedWallBoundary();
        std::vector<std::vector<int>> mappedBoundaryData;
        for (QHash<int, int>::iterator iter = mappedBoundary.begin(); iter != mappedBoundary.end(); ++iter)
        {
            std::vector<int> data;
            data.push_back(iter.key());
            data.push_back(iter.value());
            mappedBoundaryData.push_back(data);
        }
        tool->CreateTwoArrayDataSetInt(h5Group, "MappedBoundary", mappedBoundaryData);
        //写出边界数据
        int count = boundaryMgr->getDataCount();
        tool->writeGroupAttrInt(h5Group, "BoundaryCount", count);
        for (int i = 0; i < count; ++i)
        {
            Interface::FITKOFBoundary* boundary = boundaryMgr->getDataByIndex(i);
            if (!boundary) continue;
            std::string name = std::to_string(i);
            H5::Group boundaryH5Group = h5Group.createGroup(name);
            int boundaryType = boundary->getBoundaryType();
            tool->writeGroupAttrInt(h5Group, name.c_str(), boundaryType);
            auto adaptor = FITKIOADAPTORFACTORY->createT<FITKFlowOFHDF5AdaptorBoundary>("HDF5", boundary);
            if (adaptor == nullptr) return false;
            adaptor->setH5Group(&boundaryH5Group);
            adaptor->setDataObject(boundary);
            adaptor->setWriter(_writer);
            bool w = adaptor->adaptW();
            if (!w) consoleMessage(3, QString("Write boundary Errors : %1 ! ").arg(boundary->getDataObjectName()));
            delete adaptor;
            isW &= w;
        }
        return isW;
    }
    bool FITKFlowOFHDF5AdaptorPhysicsData::physicsBoundaryR()
    {
        if (!_physicsData || !_reader)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        if (!_h5Group->nameExists("Bounadry")) return true;
        bool isR = true;
        H5::Group h5Group = _h5Group->openGroup("Bounadry");
        Interface::FITKOFBoundaryManager* boundaryMgr = _physicsData->getBoundaryManager();
        if (!boundaryMgr) return false;
        //读取映射
        std::vector<std::vector<int>> mappedBoundaryData;
        tool->ReadTwoArrayDataSetInt(h5Group, "MappedBoundary", mappedBoundaryData);
        for (std::vector<std::vector<int>>::iterator iter = mappedBoundaryData.begin(); iter != mappedBoundaryData.end(); ++iter)
        {
            boundaryMgr->setMappedWallBoundary((*iter)[0], (*iter)[1]);
        }
        //读取边界数据
        boundaryMgr->clear();
        int count = 0;
        if (!tool->readGroupint(h5Group, "BoundaryCount", &count)) return false;
        for (int i = 0; i < count; ++i)
        {
            std::string name = std::to_string(i);
            if (!h5Group.nameExists(name))continue;
            H5::Group boundaryH5Group = h5Group.openGroup(name);
            int boundaryType = 0;
            if (!tool->readGroupint(h5Group, name.c_str(), &boundaryType)) continue;
            Interface::FITKOFBoundary* boundary = new Interface::FITKOFBoundary(Interface::FITKOFSolverTypeEnum::FITKOFBoundaryType(boundaryType));
            boundaryMgr->appendDataObj(boundary);
            auto adaptor = FITKIOADAPTORFACTORY->createT<FITKFlowOFHDF5AdaptorBoundary>("HDF5", boundary);
            if (adaptor == nullptr) return false;
            adaptor->setH5Group(&boundaryH5Group);
            adaptor->setDataObject(boundary);
            adaptor->setReader(_reader);
            bool r = adaptor->adaptR();
            if (!r) consoleMessage(3, QString("Read boundary Errors : %1 ! ").arg(boundary->getDataObjectName()));
            delete adaptor;
            isR &= r;
        }
        return isR;
    }
    bool FITKFlowOFHDF5AdaptorPhysicsData::physicsRunControlW()
    {
        if (!_physicsData || !_writer)return false;
        H5::Group h5Group = _h5Group->createGroup("RunControl");
        Interface::FITKOFRunControl* runControl = _physicsData->getRunControl();
        if (!runControl) return false;
        auto adaptor = FITKIOADAPTORFACTORY->createT<FITKFlowOFHDF5AdaptorRunControl>("HDF5", runControl);
        if (adaptor == nullptr) return false;
        adaptor->setH5Group(&h5Group);
        adaptor->setDataObject(runControl);
        adaptor->setWriter(_writer);
        bool isW = adaptor->adaptW();
        if (!isW) consoleMessage(3, QString("Write RunControl Errors ! "));
        delete adaptor;
        return isW;
    }
    bool FITKFlowOFHDF5AdaptorPhysicsData::physicsRunControlR()
    {
        if (!_physicsData || !_reader)return false;
        if (!_h5Group->nameExists("RunControl")) return true;
        H5::Group h5Group = _h5Group->openGroup("RunControl");
        Interface::FITKOFRunControl* runControl = _physicsData->getRunControl();
        if (!runControl) return false;
        auto adaptor = FITKIOADAPTORFACTORY->createT<FITKFlowOFHDF5AdaptorRunControl>("HDF5", runControl);
        if (adaptor == nullptr) return false;
        adaptor->setH5Group(&h5Group);
        adaptor->setDataObject(runControl);
        adaptor->setReader(_reader);
        bool isR = adaptor->adaptR();
        if (!isR) consoleMessage(3, QString("Read RunControl Errors ! "));
        delete adaptor;
        return isR;
    }
    bool FITKFlowOFHDF5AdaptorPhysicsData::physicsOperatingConditionsW()
    {
        if (!_physicsData || !_writer)return false;
        H5::Group h5Group = _h5Group->createGroup("OperatingConditions");
        Interface::FITKOFOperatingConditions* operatingCond = _physicsData->getOperatingConditions();
        if (!operatingCond) return false;
        auto adaptor = FITKIOADAPTORFACTORY->createT<FITKFlowOFHDF5AdaptorOperatingCond>("HDF5", operatingCond);
        if (adaptor == nullptr) return false;
        adaptor->setH5Group(&h5Group);
        adaptor->setDataObject(operatingCond);
        adaptor->setWriter(_writer);
        bool isW = adaptor->adaptW();
        if (!isW) consoleMessage(3, QString("Write OperatingCondition Errors ! "));
        delete adaptor;
        return isW;
    }
    bool FITKFlowOFHDF5AdaptorPhysicsData::physicsOperatingConditionsR()
    {
        if (!_physicsData || !_reader)return false;
        if (!_h5Group->nameExists("OperatingConditions")) return true;
        H5::Group h5Group = _h5Group->openGroup("OperatingConditions");
        Interface::FITKOFOperatingConditions* operatingCond = _physicsData->getOperatingConditions();
        if (!operatingCond) return false;
        auto adaptor = FITKIOADAPTORFACTORY->createT<FITKFlowOFHDF5AdaptorOperatingCond>("HDF5", operatingCond);
        if (adaptor == nullptr) return false;
        adaptor->setH5Group(&h5Group);
        adaptor->setDataObject(operatingCond);
        adaptor->setReader(_reader);
        bool isR = adaptor->adaptR();
        if (!isR) consoleMessage(3, QString("Read OperatingCondition Errors ! "));
        delete adaptor;
        return isR;
    }

}
