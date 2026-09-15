/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKFlowOFHDF5AdaptorBoundary.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"

#include "FITK_Interface/FITKInterfaceFlowOF/FITKAbstractOFSolver.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFBoundary.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFPhysicsData.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFBoundaryTypeManager.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFBoundaryType.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFPhysicsManager.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKFlowPhysicsHandlerFactory.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITKFlowOFHDF5Reader.h"
#include "FITKFlowOFHDF5Writer.h"

//数据层
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"



#include <QDebug>
namespace IO
{
    QString FITKFlowOFHDF5AdaptorBoundary::getAdaptorClass()
    {
        return "FITKFlowOFHDF5AdaptorBoundary";
    }

    bool FITKFlowOFHDF5AdaptorBoundary::adaptR()
    {
        _boundary = dynamic_cast<Interface::FITKOFBoundary*>(_dataObj);
        if (!_reader || !_boundary) return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        if (!readNDataObject(_boundary, *_h5Group)) return false;
        bool isR = true;
        //读取边界与区域间关联数据
        int meshBoundaryId = 0, meshRegionId = 0;
        tool->readGroupint(*_h5Group, "meshBoundaryId", &meshBoundaryId);
        tool->readGroupint(*_h5Group, "meshRegionId", &meshRegionId);
        _boundary->setMeshBoundaryID(meshRegionId, meshBoundaryId);

        //读取边界属性数据
        isR &= flowBoundaryR();
        isR &= turbulenceBoundaryR();
        isR &= phasesBoundaryR();
        isR &= thermalBoundaryR();
        return isR;
    }

    bool FITKFlowOFHDF5AdaptorBoundary::adaptW()
    {
        _boundary = dynamic_cast<Interface::FITKOFBoundary*>(_dataObj);
        if (!_writer || !_boundary) return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        if (!tool) return false;
        if (!writeNDataObject(_boundary, *_h5Group)) return false;
        bool isW = true;
        //写出边界与区域间关联数据
        int meshBoundaryId = _boundary->getMeshBoundaryID();
        tool->writeGroupAttrInt(*_h5Group, "meshBoundaryId", meshBoundaryId);
        int meshRegionId = _boundary->getMeshRegionID();
        tool->writeGroupAttrInt(*_h5Group, "meshRegionId", meshRegionId);
        //写出边界属性数据
        isW &= flowBoundaryW();
        isW &= turbulenceBoundaryW();
        isW &= phasesBoundaryW();
        isW &= thermalBoundaryW();
        return isW;
    }

    bool FITKFlowOFHDF5AdaptorBoundary::flowBoundaryW()
    {
        if (!_writer || !_boundary)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        if (!tool) return false;
        H5::Group h5Group = _h5Group->createGroup("Flow");
        int count = _boundary->getFlowCount();
        tool->writeGroupAttrInt(h5Group, "FlowCount", count);
        //写出Flow属性数据
        for (int i = 0; i < count; ++i)
        {
            Interface::FITKOFAbsBoundaryType* boundaryT = _boundary->getFlowVBType(i);
            if (!boundaryT)continue;
            QString vname = _boundary->getFlowVariableName(i);
            std::string name = std::to_string(i);
            tool->writeGroupAttrStr(h5Group, name.c_str(), vname.toStdString());
            H5::Group boundaryH5Group = h5Group.createGroup(name);
            tool->writeGroupAttrStr(boundaryH5Group, "boundaryTypeName", boundaryT->getDataObjectName().toStdString());
            writeNDataObject(boundaryT, boundaryH5Group);
            parameterToolW(boundaryH5Group, boundaryT->getBoundaryTypePara());
        }
        return true;
    }
    bool FITKFlowOFHDF5AdaptorBoundary::flowBoundaryR()
    {
        if (!_reader || !_boundary)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        if (!_h5Group->nameExists("Flow"))return false;
        Interface::FITKOFPhysicsData* physicsData = FITKAPP->getGlobalData()->getPhysicsData<Interface::FITKOFPhysicsData>();
        Interface::FITKOFPhysicsManager* physicsManager = Interface::FITKOFPhysicsManager::getInstance();
        if (!physicsData || !physicsManager)return false;
        Interface::FITKOFBoundaryTypeManager* boundaryTypeMgr = physicsManager->getBoundaryTypeManager();
        if (!boundaryTypeMgr)return false;
        //获取求解器
        Interface::FITKAbstractOFSolver* solver = physicsData->getSolver();
        if (!solver)return false;
        H5::Group h5Group = _h5Group->openGroup("Flow");
        int count = 0;
        tool->readGroupint(h5Group, "FlowCount", &count);
        //读取Flow属性数据
        for (int i = 0; i < count; ++i)
        {
            std::string name = std::to_string(i);
            if (!h5Group.attrExists(name))continue;
            std::string v;
            tool->readGroupStr(h5Group, name.c_str(), v);
            QString vname = QString::fromStdString(v);
            if (!h5Group.nameExists(name))return false;
            H5::Group boundaryH5Group = h5Group.openGroup(name);
            tool->readGroupStr(boundaryH5Group, "boundaryTypeName", v);
            QString boundaryType = QString::fromStdString(v);
            //获取边界信息
            Interface::FITKOFBoundaryTypeInfo info = boundaryTypeMgr->getBoundaryTypeInfo(boundaryType, solver->getSolverType(), vname, _boundary->getBoundaryType());
            if (info._boundaryTypeName.isEmpty())return false;
            Interface::FITKOFAbsBoundaryType* boundaryT = info._createFuns();
            if (!boundaryT)return false;
            readNDataObject(boundaryT, boundaryH5Group);
            parameterToolR(boundaryH5Group, boundaryT->getBoundaryTypePara());
            _boundary->setFlowBoundary(vname, boundaryT);
        }
        return true;
    }

    bool FITKFlowOFHDF5AdaptorBoundary::turbulenceBoundaryW()
    {
        if (!_writer || !_boundary)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        if (!tool) return false;
        H5::Group h5Group = _h5Group->createGroup("Turbulence");
        int count = _boundary->getTurbulenceCount();
        tool->writeGroupAttrInt(h5Group, "TurbulenceCount", count);
        //写出Turbulence属性数据
        for (int i = 0; i < count; ++i)
        {
            Interface::FITKOFAbsBoundaryType* boundaryT = _boundary->getTurbulenceVBType(i);
            if (!boundaryT)continue;
            QString vname = _boundary->getTurbulenceVariableName(i);
            std::string name = std::to_string(i);
            tool->writeGroupAttrStr(h5Group, name.c_str(), vname.toStdString());
            H5::Group boundaryH5Group = h5Group.createGroup(name);
            tool->writeGroupAttrStr(boundaryH5Group, "boundaryTypeName", boundaryT->getDataObjectName().toStdString());
            writeNDataObject(boundaryT, boundaryH5Group);
            parameterToolW(boundaryH5Group, boundaryT->getBoundaryTypePara());
        }
        return true;
    }
    bool FITKFlowOFHDF5AdaptorBoundary::turbulenceBoundaryR()
    {
        if (!_reader || !_boundary)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        if (!_h5Group->nameExists("Turbulence"))return false;
        Interface::FITKOFPhysicsData* physicsData = FITKAPP->getGlobalData()->getPhysicsData<Interface::FITKOFPhysicsData>();
        Interface::FITKOFPhysicsManager* physicsManager = Interface::FITKOFPhysicsManager::getInstance();
        if (!physicsData || !physicsManager)return false;
        Interface::FITKOFBoundaryTypeManager* boundaryTypeMgr = physicsManager->getBoundaryTypeManager();
        if (!boundaryTypeMgr)return false;
        //获取求解器
        Interface::FITKAbstractOFSolver* solver = physicsData->getSolver();
        if (!solver)return false;
        H5::Group h5Group = _h5Group->openGroup("Turbulence");
        int count = 0;
        tool->readGroupint(h5Group, "TurbulenceCount", &count);
        //读取Turbulence属性数据
        for (int i = 0; i < count; ++i)
        {
            std::string name = std::to_string(i);
            if (!h5Group.attrExists(name))continue;
            std::string v;
            tool->readGroupStr(h5Group, name.c_str(), v);
            QString vname = QString::fromStdString(v);
            if (!h5Group.nameExists(name))return false;
            H5::Group boundaryH5Group = h5Group.openGroup(name);
            tool->readGroupStr(boundaryH5Group, "boundaryTypeName", v);
            QString boundaryType = QString::fromStdString(v);
            //获取边界信息
            Interface::FITKOFBoundaryTypeInfo info = boundaryTypeMgr->getBoundaryTypeInfo(boundaryType, solver->getSolverType(), vname, _boundary->getBoundaryType());
            if (info._boundaryTypeName.isEmpty())return false;
            Interface::FITKOFAbsBoundaryType* boundaryT = info._createFuns();
            if (!boundaryT)return false;
            readNDataObject(boundaryT, boundaryH5Group);
            parameterToolR(boundaryH5Group, boundaryT->getBoundaryTypePara());
            _boundary->setTurbulenceBoundary(vname, boundaryT);
        }
        return true;
    }

    bool FITKFlowOFHDF5AdaptorBoundary::phasesBoundaryW()
    {
        if (!_writer || !_boundary)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        if (!tool) return false;
        H5::Group h5Group = _h5Group->createGroup("Phases");
        int count = _boundary->getPhasesCount();
        tool->writeGroupAttrInt(h5Group, "PhasesCount", count);
        //写出Phases属性数据
        for (int i = 0; i < count; ++i)
        {
            Interface::FITKOFAbsBoundaryType* boundaryT = _boundary->getPhasesVBType(i);
            if (!boundaryT)continue;
            QString vname = _boundary->getPhasesVariableName(i);
            std::string name = std::to_string(i);
            tool->writeGroupAttrStr(h5Group, name.c_str(), vname.toStdString());
            H5::Group boundaryH5Group = h5Group.createGroup(name);
            tool->writeGroupAttrStr(boundaryH5Group, "boundaryTypeName", boundaryT->getDataObjectName().toStdString());
            writeNDataObject(boundaryT, boundaryH5Group);
            parameterToolW(boundaryH5Group, boundaryT->getBoundaryTypePara());
        }
        return true;
    }
    bool FITKFlowOFHDF5AdaptorBoundary::phasesBoundaryR()
    {
        if (!_reader || !_boundary)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        if (!_h5Group->nameExists("Phases"))return false;
        Interface::FITKOFPhysicsData* physicsData = FITKAPP->getGlobalData()->getPhysicsData<Interface::FITKOFPhysicsData>();
        Interface::FITKOFPhysicsManager* physicsManager = Interface::FITKOFPhysicsManager::getInstance();
        if (!physicsData || !physicsManager)return false;
        Interface::FITKOFBoundaryTypeManager* boundaryTypeMgr = physicsManager->getBoundaryTypeManager();
        if (!boundaryTypeMgr)return false;
        //获取求解器
        Interface::FITKAbstractOFSolver* solver = physicsData->getSolver();
        if (!solver)return false;
        H5::Group h5Group = _h5Group->openGroup("Phases");
        int count = 0;
        tool->readGroupint(h5Group, "PhasesCount", &count);
        //读取Phases属性数据
        for (int i = 0; i < count; ++i)
        {
            std::string name = std::to_string(i);
            if (!h5Group.attrExists(name))continue;
            std::string v;
            tool->readGroupStr(h5Group, name.c_str(), v);
            QString vname = QString::fromStdString(v);
            if (!h5Group.nameExists(name))return false;
            H5::Group boundaryH5Group = h5Group.openGroup(name);
            tool->readGroupStr(boundaryH5Group, "boundaryTypeName", v);
            QString boundaryType = QString::fromStdString(v);
            //获取边界信息
            Interface::FITKOFBoundaryTypeInfo info = boundaryTypeMgr->getBoundaryTypeInfo(boundaryType, solver->getSolverType(), vname, _boundary->getBoundaryType());
            if (info._boundaryTypeName.isEmpty())return false;
            Interface::FITKOFAbsBoundaryType* boundaryT = info._createFuns();
            if (!boundaryT)return false;
            readNDataObject(boundaryT, boundaryH5Group);
            parameterToolR(boundaryH5Group, boundaryT->getBoundaryTypePara());
            _boundary->setPhasesBoundary(vname, boundaryT);
        }
        return true;
    }
    bool FITKFlowOFHDF5AdaptorBoundary::thermalBoundaryW()
    {
        if (!_writer || !_boundary)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        if (!tool) return false;
        H5::Group h5Group = _h5Group->createGroup("Thermal");
        int count = _boundary->getThermalCount();
        tool->writeGroupAttrInt(h5Group, "ThermalCount", count);
        //写出Thermal属性数据
        for (int i = 0; i < count; ++i)
        {
            Interface::FITKOFAbsBoundaryType* boundaryT = _boundary->getThermalVBType(i);
            if (!boundaryT)continue;
            QString vname = _boundary->getThermalVariableName(i);
            std::string name = std::to_string(i);
            tool->writeGroupAttrStr(h5Group, name.c_str(), vname.toStdString());
            H5::Group boundaryH5Group = h5Group.createGroup(name);
            tool->writeGroupAttrStr(boundaryH5Group, "boundaryTypeName", boundaryT->getDataObjectName().toStdString());
            writeNDataObject(boundaryT, boundaryH5Group);
            parameterToolW(boundaryH5Group, boundaryT->getBoundaryTypePara());
        }
        return true;
    }
    bool FITKFlowOFHDF5AdaptorBoundary::thermalBoundaryR()
    {
        if (!_reader || !_boundary)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        if (!_h5Group->nameExists("Thermal"))return false;
        Interface::FITKOFPhysicsData* physicsData = FITKAPP->getGlobalData()->getPhysicsData<Interface::FITKOFPhysicsData>();
        Interface::FITKOFPhysicsManager* physicsManager = Interface::FITKOFPhysicsManager::getInstance();
        if (!physicsData || !physicsManager)return false;
        Interface::FITKOFBoundaryTypeManager* boundaryTypeMgr = physicsManager->getBoundaryTypeManager();
        if (!boundaryTypeMgr)return false;
        //获取求解器
        Interface::FITKAbstractOFSolver* solver = physicsData->getSolver();
        if (!solver)return false;
        H5::Group h5Group = _h5Group->openGroup("Thermal");
        int count = 0;
        tool->readGroupint(h5Group, "ThermalCount", &count);
        //读取Thermal属性数据
        for (int i = 0; i < count; ++i)
        {
            std::string name = std::to_string(i);
            if (!h5Group.attrExists(name))continue;
            std::string v;
            tool->readGroupStr(h5Group, name.c_str(), v);
            QString vname = QString::fromStdString(v);
            if (!h5Group.nameExists(name))return false;
            H5::Group boundaryH5Group = h5Group.openGroup(name);
            tool->readGroupStr(boundaryH5Group, "boundaryTypeName", v);
            QString boundaryType = QString::fromStdString(v);
            //获取边界信息
            Interface::FITKOFBoundaryTypeInfo info = boundaryTypeMgr->getBoundaryTypeInfo(boundaryType, solver->getSolverType(), vname, _boundary->getBoundaryType());
            if (info._boundaryTypeName.isEmpty())return false;
            Interface::FITKOFAbsBoundaryType* boundaryT = info._createFuns();
            if (!boundaryT)return false;
            readNDataObject(boundaryT, boundaryH5Group);
            parameterToolR(boundaryH5Group, boundaryT->getBoundaryTypePara());
            _boundary->setThermalBoundary(vname, boundaryT);
        }
        return true;
    }
}
