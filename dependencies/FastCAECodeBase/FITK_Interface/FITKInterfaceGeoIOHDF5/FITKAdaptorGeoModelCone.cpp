/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAdaptorGeoModelCone.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelCone.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"


namespace Interface
{
    bool  FITKAdaptorGeoModelCone::readParameters()
    {
        Interface::FITKAbsGeoModelCone* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelCone>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        
        double location[3] = {0.0, 0.0, 0.0};
        double direction[3] = {0.0, 0.0, 1.0};
        double bottomRadius = 0.0;
        double topRadius = 0.0;
        double length = 0.0;
        
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Location, location, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Direction, direction, 1, 3)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_BottomRadius, &bottomRadius)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_TopRadius, &topRadius)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_Length, &length)) return false;
        
        cmd->setLocation(location);
        cmd->setDirection(direction);
        cmd->setBottomRadius(bottomRadius);
        cmd->setTopRadius(topRadius);
        cmd->setLength(length);
        
        return true;
    }
    
    bool FITKAdaptorGeoModelCone::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelCone* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelCone>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        
        double location[3] = {0.0, 0.0, 0.0};
        double direction[3] = {0.0, 0.0, 1.0};
        
        cmd->getLocation(location);
        cmd->getDirection(direction);
        
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Location, location, 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Direction, direction, 1, 3);
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_BottomRadius, cmd->getBottomRadius());
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_TopRadius, cmd->getTopRadius());
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_Length, cmd->getLength());
        
        return true;
    }
}
