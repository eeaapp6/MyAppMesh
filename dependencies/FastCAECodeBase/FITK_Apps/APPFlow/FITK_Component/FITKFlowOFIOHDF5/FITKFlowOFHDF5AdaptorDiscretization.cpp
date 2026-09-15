/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKFlowOFHDF5AdaptorDiscretization.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFDiscretization.h"
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
    QString FITKFlowOFHDF5AdaptorDiscretization::getAdaptorClass()
    {
        return "FITKFlowOFHDF5AdaptorDiscretization";
    }

    bool FITKFlowOFHDF5AdaptorDiscretization::adaptR()
    {
        _discretization = dynamic_cast<Interface::FITKOFDiscretization*>(_dataObj);
        if (!_reader || !_discretization) return false;
        if (!readNDataObject(_discretization, *_h5Group)) return false;
        bool isR = true;
        isR &= timeOptionR();
        isR &= convectionR();
        isR &= gradientsR();
        isR &= interpolationR();
        return isR;
    }

    bool FITKFlowOFHDF5AdaptorDiscretization::adaptW()
    {
        _discretization = dynamic_cast<Interface::FITKOFDiscretization*>(_dataObj);
        if (!_writer || !_discretization) return false;
        if (!writeNDataObject(_discretization, *_h5Group)) return false;
        bool isW = true;
        isW &= timeOptionW();
        isW &= convectionW();
        isW &= gradientsW();
        isW &= interpolationW();
        return isW;
    }

    bool FITKFlowOFHDF5AdaptorDiscretization::timeOptionW()
    {
        if (!_writer || !_discretization)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        if (!tool) return false;
        H5::Group h5Group = _h5Group->createGroup("TimeOption");
        if (_discretization->getTimeOption())
            parameterToolW(h5Group, _discretization->getTimeOption());
        return true;
    }

    bool FITKFlowOFHDF5AdaptorDiscretization::timeOptionR()
    {
        if (!_reader || !_discretization)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        if (!_h5Group->nameExists("TimeOption"))return false;
        H5::Group h5Group = _h5Group->openGroup("TimeOption");
        if (h5Group.getNumAttrs() > 0)
        {
            Core::FITKParameter* timeOption = _discretization->getTimeOption();
            if (!timeOption) return false;
            timeOption->clear();
            parameterToolR(h5Group, timeOption);
        }
        return true;
    }

    bool FITKFlowOFHDF5AdaptorDiscretization::convectionW()
    {
        if (!_writer || !_discretization)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        if (!tool) return false;
        H5::Group h5Group = _h5Group->createGroup("Convection");
        int convectionCount = _discretization->getConvectionCount();
        tool->writeGroupAttrInt(h5Group, "convectionCount", convectionCount);
        for (int i = 0; i < convectionCount; ++i)
        {
            std::string name = std::to_string(i);
            H5::Group convectionH5Group = h5Group.createGroup(name);
            QString vName = _discretization->getConvectionVName(i);
            Core::FITKParameter* vPara = _discretization->getConvectionVPara(i);
            tool->writeGroupAttrStr(h5Group, name.c_str(), vName.toStdString());
            parameterToolW(convectionH5Group, vPara);
        }
        return true;
    }

    bool FITKFlowOFHDF5AdaptorDiscretization::convectionR()
    {
        if (!_reader || !_discretization)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        if (!_h5Group->nameExists("Convection"))return false;
        _discretization->clearConvection();
        H5::Group h5Group = _h5Group->openGroup("Convection");
        int convectionCount = 0;
        tool->readGroupint(h5Group, "convectionCount", &convectionCount);
        for (int i = 0; i < convectionCount; ++i)
        {
            std::string name = std::to_string(i);
            if (!h5Group.nameExists(name))continue;
            H5::Group convectionH5Group = h5Group.openGroup(name);
            std::string v;
            tool->readGroupStr(h5Group, name.c_str(), v);
            QString vName = QString::fromStdString(v);
            Core::FITKParameter* vPara = new Core::FITKParameter;
            parameterToolR(convectionH5Group, vPara);
            _discretization->insertConvection(vName, vPara);
        }
        return true;
    }

    bool FITKFlowOFHDF5AdaptorDiscretization::gradientsW()
    {
        if (!_writer || !_discretization)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        if (!tool) return false;
        H5::Group h5Group = _h5Group->createGroup("Gradients");
        if (_discretization->getGradients())
            parameterToolW(h5Group, _discretization->getGradients());
        return true;
    }

    bool FITKFlowOFHDF5AdaptorDiscretization::gradientsR()
    {
        if (!_reader || !_discretization)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        if (!_h5Group->nameExists("Gradients"))return false;
        H5::Group h5Group = _h5Group->openGroup("Gradients");
        if (h5Group.getNumAttrs() > 0)
        {
            Core::FITKParameter* gradients = _discretization->getGradients();
            if (!gradients) return false;
            gradients->clear();
            parameterToolR(h5Group, gradients);
        }
        return true;
    }

    bool FITKFlowOFHDF5AdaptorDiscretization::interpolationW()
    {
        if (!_writer || !_discretization)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        if (!tool) return false;
        H5::Group h5Group = _h5Group->createGroup("Interpolation");
        if (_discretization->getInterpolation())
            parameterToolW(h5Group, _discretization->getInterpolation());
        return true;
    }

    bool FITKFlowOFHDF5AdaptorDiscretization::interpolationR()
    {
        if (!_reader || !_discretization)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        if (!_h5Group->nameExists("Interpolation"))return false;
        H5::Group h5Group = _h5Group->openGroup("Interpolation");
        if (h5Group.getNumAttrs() > 0)
        {
            Core::FITKParameter* interpolation = _discretization->getInterpolation();
            if (!interpolation) return false;
            interpolation->clear();
            parameterToolR(h5Group, interpolation);
        }
        return true;
    }
}
