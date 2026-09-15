/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAdaptorGeoModelFilledEllipse.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelFilledEllipse.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"


namespace Interface
{
    bool  FITKAdaptorGeoModelFilledEllipse::readParameters()
    {
        Interface::FITKAbsGeoModelFilledEllipse* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelFilledEllipse>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        
        double origin[3] = {0.0, 0.0, 0.0};
        double xDirection[3] = {1.0, 0.0, 0.0};
        double yDirection[3] = {0.0, 1.0, 0.0};
        double xRadius = 0.0;
        double yRadius = 0.0;
        int isFilled = 1;
        
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Origin, origin, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_XDirection, xDirection, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_YDirection, yDirection, 1, 3)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_XRadius, &xRadius)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_YRadius, &yRadius)) return false;
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_IsFilled, &isFilled)) return false;
        
        cmd->setOrigin(origin[0], origin[1], origin[2]);
        cmd->setXDirection(xDirection[0], xDirection[1], xDirection[2]);
        cmd->setYDirection(yDirection[0], yDirection[1], yDirection[2]);
        cmd->setXRadius(xRadius);
        cmd->setYRadius(yRadius);
        cmd->setFilled(isFilled != 0);
        
        return true;
    }
    
    bool FITKAdaptorGeoModelFilledEllipse::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelFilledEllipse* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelFilledEllipse>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        
        std::array<double, 3> origin = cmd->origin();
        std::array<double, 3> xDir = cmd->xDirection();
        std::array<double, 3> yDir = cmd->yDirection();
        
        double originArr[3] = {origin[0], origin[1], origin[2]};
        double xDirArr[3] = {xDir[0], xDir[1], xDir[2]};
        double yDirArr[3] = {yDir[0], yDir[1], yDir[2]};
        
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Origin, originArr, 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_XDirection, xDirArr, 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_YDirection, yDirArr, 1, 3);
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_XRadius, cmd->xRadius());
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_YRadius, cmd->yRadius());
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_IsFilled, cmd->isFilled() ? 1 : 0);
        
        return true;
    }
}
