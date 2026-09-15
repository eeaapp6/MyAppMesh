/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAdaptorGeoModelFrustum.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelFrustum.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"


namespace Interface
{
    bool  FITKAdaptorGeoModelFrustum::readParameters()
    {
        Interface::FITKAbsGeoModelFrustum* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelFrustum>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        
        double location[3] = {0.0, 0.0, 0.0};
        double direction1[3] = {1.0, 0.0, 0.0};
        double direction2[3] = {0.0, 1.0, 0.0};
        double bottomLength = 0.0;
        double bottomWidth = 0.0;
        double topLength = 0.0;
        double topWidth = 0.0;
        double length = 0.0;
        
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Location, location, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_FirstDirection, direction1, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_SecondDirection, direction2, 1, 3)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_BottomLength, &bottomLength)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_BottomWidth, &bottomWidth)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_TopLength, &topLength)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_TopWidth, &topWidth)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_Length, &length)) return false;
        
        cmd->setLocation(location);
        cmd->setDirection1(direction1);
        cmd->setDirection2(direction2);
        cmd->setBottomLength(bottomLength);
        cmd->setBottomWidth(bottomWidth);
        cmd->setTopLength(topLength);
        cmd->setTopWidth(topWidth);
        cmd->setLength(length);
        
        return true;
    }
    
    bool FITKAdaptorGeoModelFrustum::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelFrustum* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelFrustum>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        
        double location[3] = {0.0, 0.0, 0.0};
        double direction1[3] = {1.0, 0.0, 0.0};
        double direction2[3] = {0.0, 1.0, 0.0};
        
        cmd->getLocation(location);
        cmd->getDirection1(direction1);
        cmd->getDirection2(direction2);
        
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Location, location, 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_FirstDirection, direction1, 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_SecondDirection, direction2, 1, 3);
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_BottomLength, cmd->getBottomLength());
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_BottomWidth, cmd->getBottomWidth());
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_TopLength, cmd->getTopLength());
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_TopWidth, cmd->getTopWidth());
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_Length, cmd->getLength());
        
        return true;
    }
}
