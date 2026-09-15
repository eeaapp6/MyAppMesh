/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKFlowOFHDF5AdaptorTurbulence.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKAbstractOFTurbulence.h"
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
    QString FITKFlowOFHDF5AdaptorTurbulence::getAdaptorClass()
    {
        return "FITKFlowOFHDF5AdaptorTurbulence";
    }

    bool FITKFlowOFHDF5AdaptorTurbulence::adaptR()
    {
        _turbulence = dynamic_cast<Interface::FITKAbstractOFTurbulence*>(_dataObj);
        if (!_reader || !_turbulence) return false;
        if (!readNDataObject(_turbulence, *_h5Group)) return false;
        bool isR = true;
        //读取数据
        isR &= turbulenceParaR();
        isR &= deltaR();

        return isR;
    }

    bool FITKFlowOFHDF5AdaptorTurbulence::adaptW()
    {
        _turbulence = dynamic_cast<Interface::FITKAbstractOFTurbulence*>(_dataObj);
        if (!_writer || !_turbulence) return false;
        if (!writeNDataObject(_turbulence, *_h5Group)) return false;
        bool isW = true;
        //写出数据
        isW &= turbulenceParaW();
        isW &= deltaW();

        return isW;
    }

    bool FITKFlowOFHDF5AdaptorTurbulence::turbulenceParaW()
    {
        if (!_writer || !_turbulence)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        if (!tool) return false;
        std::tuple<bool, H5::Group> subH5Group = tool->createGp(*_h5Group, "TurbulencePara");
        if (std::get<0>(subH5Group) == false) return false;
        if (_turbulence->getTurbulenceParams())
            parameterToolW(std::get<1>(subH5Group), _turbulence->getTurbulenceParams());
        return true;
    }
    bool FITKFlowOFHDF5AdaptorTurbulence::turbulenceParaR()
    {
        if (!_reader || !_turbulence)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        if (!_h5Group->nameExists("TurbulencePara"))return false;
        H5::Group h5Group = _h5Group->openGroup("TurbulencePara");
        if (h5Group.getNumAttrs() > 0)
        {
            Core::FITKParameter* turbPara = _turbulence->getTurbulenceParams();
            if (!turbPara) return false;
            turbPara->clear();
            parameterToolR(h5Group, turbPara);
        }
        return true;
    }
    bool FITKFlowOFHDF5AdaptorTurbulence::deltaW()
    {
        if (!_writer || !_turbulence)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        if (!tool) return false;
        H5::Group h5Group = _h5Group->createGroup("Delta");
        if (_turbulence->getDeltaParams())
            parameterToolW(h5Group, _turbulence->getDeltaParams());
        return true;
    }
    bool FITKFlowOFHDF5AdaptorTurbulence::deltaR()
    {
        if (!_reader || !_turbulence)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        if (!_h5Group->nameExists("Delta"))return false;
        H5::Group h5Group = _h5Group->openGroup("Delta");
        if (h5Group.getNumAttrs() > 0)
        {
            Core::FITKParameter* delta = _turbulence->getDeltaParams();
            if (!delta) return false;
            delta->clear();
            parameterToolR(h5Group, delta);
        }
        return true;
    }
}
