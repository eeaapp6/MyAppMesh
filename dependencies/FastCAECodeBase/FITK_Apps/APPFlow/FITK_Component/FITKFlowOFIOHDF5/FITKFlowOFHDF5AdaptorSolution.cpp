/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKFlowOFHDF5AdaptorSolution.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFSolution.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFSolutionSolver.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKFlowPhysicsHandlerFactory.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITKFlowOFHDF5Reader.h"
#include "FITKFlowOFHDF5Writer.h"

//数据层
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"



#include <QDebug>
namespace IO
{
    QString FITKFlowOFHDF5AdaptorSolution::getAdaptorClass()
    {
        return "FITKFlowOFHDF5AdaptorSolution";
    }

    bool FITKFlowOFHDF5AdaptorSolution::adaptR()
    {
        _solution = dynamic_cast<Interface::FITKOFSolution*>(_dataObj);
        if (!_reader || !_solution) return false;
        if (!readNDataObject(_solution, *_h5Group)) return false;
        bool isR = true;

        isR &= solversR();
        isR &= speciallyDataR();
        isR &= residualsR();
        isR &= relaxationR();
        isR &= limitsR();

        return isR;
    }

    bool FITKFlowOFHDF5AdaptorSolution::adaptW()
    {
        _solution = dynamic_cast<Interface::FITKOFSolution*>(_dataObj);
        if (!_writer || !_solution) return false;
        if (!writeNDataObject(_solution, *_h5Group)) return false;
        bool isW = true;

        isW &= solversW();
        isW &= speciallyDataW();
        isW &= residualsW();
        isW &= relaxationW();
        isW &= limitsW();

        return isW;
    }

    bool FITKFlowOFHDF5AdaptorSolution::solversW()
    {
        if (!_writer || !_solution)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        if (!tool) return false;
        H5::Group h5Group = _h5Group->createGroup("Solvers");
        int vCount = _solution->getSolversCount();
        tool->writeGroupAttrInt(h5Group, "SolversCount", vCount);
        for (int i = 0; i < vCount; ++i)
        {
            std::string name = std::to_string(i);
            Interface::FITKOFAlgebraicEquationsPara* equationPara = _solution->getSolverVariablePara(i);
            if (!equationPara) return false;
            H5::Group equationH5Group = h5Group.createGroup(name);
            //名称
            QString vName = equationPara->getVariableName();
            tool->writeGroupAttrStr(equationH5Group, "vName", vName.toStdString());
            //其他数据
            H5::Group addH5Group = equationH5Group.createGroup("AdditionalData");
            Core::FITKParameter* solverAddition = equationPara->getSolverAdditionalPara();
            if (solverAddition) parameterToolW(addH5Group, solverAddition);
            //求解器数据
            Interface::FITKOFAbsSolutionSolver* solverData = equationPara->getSolutionSolver();
            if (solverData)
            {
                tool->writeGroupAttrStr(equationH5Group, "SolverName", solverData->getDataObjectName().toStdString());
                H5::Group solverH5Group = equationH5Group.createGroup("SolverData");
                writeNDataObject(solverData, solverH5Group);
                parameterToolW(solverH5Group, solverData->getSolverSolutionPara());
            }
        }
        return true;
    }

    bool FITKFlowOFHDF5AdaptorSolution::solversR()
    {
        if (!_reader || !_solution)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        if (!_h5Group->nameExists("Solvers"))return false;
        Interface::FITKFlowPhysicsHandlerFactory* factoryData = FITKAPP->getComponents()->getComponentTByName<Interface::FITKFlowPhysicsHandlerFactory>("FITKFlowPhysicsHandlerFactory");
        if (!factoryData) return false;
        H5::Group h5Group = _h5Group->openGroup("Solvers");
        int count = 0;
        tool->readGroupint(h5Group, "SolversCount", &count);
        if (_solution->getSolversCount() != count) return false;
        for (int i = 0; i < count; ++i)
        {
            std::string name = std::to_string(i);
            if (!h5Group.nameExists(name))continue;
            Interface::FITKOFAlgebraicEquationsPara* equationPara = _solution->getSolverVariablePara(i);
            if (!equationPara) return false;
            H5::Group equationH5Group = h5Group.openGroup(name);
            //名称
            std::string v;
            tool->readGroupStr(equationH5Group, "vName", v);
            QString vname = QString::fromStdString(v);
            equationPara->setVariableName(vname);
            //其他数据
            if (!equationH5Group.nameExists("AdditionalData"))return false;
            H5::Group addH5Group = equationH5Group.openGroup("AdditionalData");
            if (addH5Group.getNumAttrs() > 0)
            {
                Core::FITKParameter* additionData = new Core::FITKParameter;
                parameterToolR(addH5Group, additionData);
                equationPara->setSolverAdditionalPara(additionData);
            }
            //模式数据
            if (equationH5Group.attrExists("SolverName"))
            {
                tool->readGroupStr(equationH5Group, "SolverName", v);
                QString solverName = QString::fromStdString(v);
                factoryData->setSolutionSolver(i, solverName);
                H5::Group solverH5Group = equationH5Group.openGroup("SolverData");
                Interface::FITKOFAbsSolutionSolver* solverData = equationPara->getSolutionSolver();
                readNDataObject(solverData, solverH5Group);
                solverData->getSolverSolutionPara()->clear();
                parameterToolR(solverH5Group, solverData->getSolverSolutionPara());
            }
        }
        return true;
    }

    bool FITKFlowOFHDF5AdaptorSolution::speciallyDataW()
    {
        if (!_writer || !_solution)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        if (!tool) return false;
        H5::Group h5Group = _h5Group->createGroup("SpeciallyData");
        if (_solution->getSolverSpeciallyDataPara())
        {
            std::string name = _solution->getSolverSpeciallyDataName().toStdString();
            tool->writeGroupAttrStr(h5Group, "SpeciallyDataName", name);
            parameterToolW(h5Group, _solution->getSolverSpeciallyDataPara());
        }
        return true;
    }

    bool FITKFlowOFHDF5AdaptorSolution::speciallyDataR()
    {
        if (!_reader || !_solution)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        if (!_h5Group->nameExists("SpeciallyData"))return false;
        H5::Group h5Group = _h5Group->openGroup("SpeciallyData");
        if (h5Group.getNumAttrs() > 0)
        {
            Core::FITKParameter* para = new Core::FITKParameter;
            parameterToolR(h5Group, para);
            std::string v;
            tool->readGroupStr(h5Group, "SpeciallyDataName", v);
            QString name = QString::fromStdString(v);
            _solution->setSpeciallyData(name, para);
        }
        return true;
    }

    bool FITKFlowOFHDF5AdaptorSolution::residualsW()
    {
        if (!_writer || !_solution)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        if (!tool) return false;
        H5::Group h5Group = _h5Group->createGroup("Residuals");
        int count = _solution->getResidualsCount();
        tool->writeGroupAttrInt(h5Group, "ResidualsCount", count);
        for (int i = 0; i < count; ++i)
        {
            std::string name = std::to_string(i);
            H5::Group residualH5Group = h5Group.createGroup(name);
            QString vName = _solution->getResidualVariableName(i);
            Core::FITKParameter* vPara = _solution->getResidualVariablePara(i);
            tool->writeGroupAttrStr(h5Group, name.c_str(), vName.toStdString());
            parameterToolW(residualH5Group, vPara);
        }
        return true;
    }

    bool FITKFlowOFHDF5AdaptorSolution::residualsR()
    {
        if (!_reader || !_solution)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        if (!_h5Group->nameExists("Residuals"))return false;
        H5::Group h5Group = _h5Group->openGroup("Residuals");
        int count = 0;
        tool->readGroupint(h5Group, "ResidualsCount", &count);
        for (int i = 0; i < count; ++i)
        {
            std::string name = std::to_string(i);
            if (!h5Group.nameExists(name))continue;
            H5::Group convectionH5Group = h5Group.openGroup(name);
            std::string v;
            tool->readGroupStr(h5Group, name.c_str(), v);
            QString vName = QString::fromStdString(v);
            Core::FITKParameter* vPara = new Core::FITKParameter;
            parameterToolR(convectionH5Group, vPara);
            _solution->setResidualData(vName, vPara);
        }
        return true;
    }

    bool FITKFlowOFHDF5AdaptorSolution::relaxationW()
    {
        if (!_writer || !_solution)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        if (!tool) return false;
        H5::Group h5Group = _h5Group->createGroup("Relaxation");
        if (_solution->getRelaxation())
            parameterToolW(h5Group, _solution->getRelaxation());
        return true;
    }

    bool FITKFlowOFHDF5AdaptorSolution::relaxationR()
    {
        if (!_reader || !_solution)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        if (!_h5Group->nameExists("Relaxation"))return false;
        H5::Group h5Group = _h5Group->openGroup("Relaxation");
        if (h5Group.getNumAttrs() > 0)
        {
            Core::FITKParameter* relaxation = _solution->getRelaxation();
            if (!relaxation) return false;
            relaxation->clear();
            parameterToolR(h5Group, relaxation);
        }
        return true;
    }

    bool FITKFlowOFHDF5AdaptorSolution::limitsW()
    {
        if (!_writer || !_solution)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        if (!tool) return false;
        H5::Group h5Group = _h5Group->createGroup("Limits");
        if (_solution->getLimits())
            parameterToolW(h5Group, _solution->getLimits());
        return true;
    }

    bool FITKFlowOFHDF5AdaptorSolution::limitsR()
    {
        if (!_reader || !_solution)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        if (!_h5Group->nameExists("Limits"))return false;
        H5::Group h5Group = _h5Group->openGroup("Limits");
        if (h5Group.getNumAttrs() > 0)
        {
            Core::FITKParameter* limits = _solution->getLimits();
            if (!limits) return false;
            limits->clear();
            parameterToolR(h5Group, limits);
        }
        return true;
    }

}
