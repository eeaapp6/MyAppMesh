/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKFlowOFHDF5AdaptorRunControl.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFRunControl.h"
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
    QString FITKFlowOFHDF5AdaptorRunControl::getAdaptorClass()
    {
        return "FITKFlowOFHDF5AdaptorRunControl";
    }

    bool FITKFlowOFHDF5AdaptorRunControl::adaptR()
    {
        _runControl = dynamic_cast<Interface::FITKOFRunControl*>(_dataObj);
        if (!_reader || !_runControl) return false;
        if (!readNDataObject(_runControl, *_h5Group)) return false;
        bool isR = true;

        isR &= timeControlR();
        isR &= outputControlR();

        return isR;
    }

    bool FITKFlowOFHDF5AdaptorRunControl::adaptW()
    {
        _runControl = dynamic_cast<Interface::FITKOFRunControl*>(_dataObj);
        if (!_writer || !_runControl) return false;
        if (!writeNDataObject(_runControl, *_h5Group)) return false;
        bool isW = true;

        isW &= timeControlW();
        isW &= outputControlW();

        return isW;
    }

    bool FITKFlowOFHDF5AdaptorRunControl::timeControlW()
    {
        if (!_writer || !_runControl)return false;
        H5::Group h5Group = _h5Group->createGroup("TimeControl");
        if (_runControl->getTimeControl())
            parameterToolW(h5Group, _runControl->getTimeControl());
        return true;
    }
    bool FITKFlowOFHDF5AdaptorRunControl::timeControlR()
    {
        if (!_reader || !_runControl)return false;
        if (!_h5Group->nameExists("TimeControl"))return false;
        H5::Group h5Group = _h5Group->openGroup("TimeControl");
        if (h5Group.getNumAttrs() > 0)
        {
            Core::FITKParameter* timeControl = _runControl->getTimeControl();
            if (!timeControl) return false;
            timeControl->clear();
            parameterToolR(h5Group, timeControl);
        }
        return true;
    }
    bool FITKFlowOFHDF5AdaptorRunControl::outputControlW()
    {
        if (!_writer || !_runControl)return false;
        H5::Group h5Group = _h5Group->createGroup("OutputControl");
        if (_runControl->getOutputControl())
            parameterToolW(h5Group, _runControl->getOutputControl());
        return true;
    }
    bool FITKFlowOFHDF5AdaptorRunControl::outputControlR()
    {
        if (!_reader || !_runControl)return false;
        if (!_h5Group->nameExists("OutputControl"))return false;
        H5::Group h5Group = _h5Group->openGroup("OutputControl");
        if (h5Group.getNumAttrs() > 0)
        {
            Core::FITKParameter* outputControl = _runControl->getOutputControl();
            if (!outputControl) return false;
            outputControl->clear();
            parameterToolR(h5Group, outputControl);
        }
        return true;
    }
}
