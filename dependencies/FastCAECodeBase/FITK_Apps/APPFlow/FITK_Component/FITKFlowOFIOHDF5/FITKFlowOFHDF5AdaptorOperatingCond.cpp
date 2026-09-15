/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKFlowOFHDF5AdaptorOperatingCond.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFOperatingConditions.h"

#include "FITK_Interface/FITKInterfaceFlowOF/FITKFlowPhysicsHandlerFactory.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITKFlowOFHDF5Reader.h"
#include "FITKFlowOFHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
//数据层
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"



#include <QDebug>
namespace IO
{
    QString FITKFlowOFHDF5AdaptorOperatingCond::getAdaptorClass()
    {
        return "FITKFlowOFHDF5AdaptorOperatingCond";
    }

    bool FITKFlowOFHDF5AdaptorOperatingCond::adaptR()
    {
        _operatingCond = dynamic_cast<Interface::FITKOFOperatingConditions*>(_dataObj);
        if (!_reader || !_operatingCond) return false;
        if (!readNDataObject(_operatingCond, *_h5Group)) return false;
        bool isR = true;

        isR &= gravitationalAccelerationR();
        isR &= referencePressureR();

        return isR;
    }

    bool FITKFlowOFHDF5AdaptorOperatingCond::adaptW()
    {
        _operatingCond = dynamic_cast<Interface::FITKOFOperatingConditions*>(_dataObj);
        if (!_writer || !_operatingCond) return false;
        if (!writeNDataObject(_operatingCond, *_h5Group)) return false;
        bool isW = true;

        isW &= gravitationalAccelerationW();
        isW &= referencePressureW();

        return isW;
    }

    bool FITKFlowOFHDF5AdaptorOperatingCond::gravitationalAccelerationW()
    {
        if (!_writer || !_operatingCond)return false;
        H5::Group h5Group = _h5Group->createGroup("GravitationalAcceleration");
        if (_operatingCond->getGravitationalAcceleration())
            parameterToolW(h5Group, _operatingCond->getGravitationalAcceleration());
        return true;
    }
    bool FITKFlowOFHDF5AdaptorOperatingCond::gravitationalAccelerationR()
    {
        if (!_reader || !_operatingCond)return false;
        if (!_h5Group->nameExists("GravitationalAcceleration"))return false;
        H5::Group h5Group = _h5Group->openGroup("GravitationalAcceleration");
        if (h5Group.getNumAttrs() > 0)
        {
            Core::FITKParameter* ga = _operatingCond->getGravitationalAcceleration();
            if (!ga) return false;
            ga->clear();
            parameterToolR(h5Group, ga);
        }
        return true;
    }
    bool FITKFlowOFHDF5AdaptorOperatingCond::referencePressureW()
    {
        if (!_writer || !_operatingCond)return false;
        H5::Group h5Group = _h5Group->createGroup("ReferencePressure");
        if (_operatingCond->getReferencePressure())
            parameterToolW(h5Group, _operatingCond->getReferencePressure());
        return true;
    }
    bool FITKFlowOFHDF5AdaptorOperatingCond::referencePressureR()
    {
        if (!_reader || !_operatingCond)return false;
        if (!_h5Group->nameExists("ReferencePressure"))return false;
        H5::Group h5Group = _h5Group->openGroup("ReferencePressure");
        if (h5Group.getNumAttrs() > 0)
        {
            Core::FITKParameter* press = _operatingCond->getReferencePressure();
            if (!press) return false;
            press->clear();
            parameterToolR(h5Group, press);
        }
        return true;
    }

}
