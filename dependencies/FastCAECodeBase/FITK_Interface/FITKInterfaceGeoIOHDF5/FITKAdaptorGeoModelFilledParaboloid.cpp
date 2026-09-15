/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAdaptorGeoModelFilledParaboloid.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelFilledParaboloid.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"


namespace Interface
{
    bool  FITKAdaptorGeoModelFilledParaboloid::readParameters()
    {
        Interface::FITKAbsGeoModelFilledParaboloid* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelFilledParaboloid>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        
        double origin[3] = {0.0, 0.0, 0.0};
        double axisDirection[3] = {0.0, 0.0, 1.0};
        double xDirection[3] = {1.0, 0.0, 0.0};
        double startRadius = 0.0;
        double focalDepth = 0.0;
        
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Origin, origin, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Direction, axisDirection, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_XDirection, xDirection, 1, 3)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_StartRadius, &startRadius)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_FocalDepth, &focalDepth)) return false;
        
        cmd->setOrigin(origin[0], origin[1], origin[2]);
        cmd->setAxisDirection(axisDirection[0], axisDirection[1], axisDirection[2]);
        cmd->setXDirection(xDirection[0], xDirection[1], xDirection[2]);
        cmd->setStartRadius(startRadius);
        cmd->setFocalDepth(focalDepth);
        
        return true;
    }
    
    bool FITKAdaptorGeoModelFilledParaboloid::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelFilledParaboloid* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelFilledParaboloid>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        
        std::array<double, 3> origin = cmd->origin();
        std::array<double, 3> axisDir = cmd->axisDirection();
        std::array<double, 3> xDir = cmd->xDirection();
        
        double originArr[3] = {origin[0], origin[1], origin[2]};
        double axisDirArr[3] = {axisDir[0], axisDir[1], axisDir[2]};
        double xDirArr[3] = {xDir[0], xDir[1], xDir[2]};
        
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Origin, originArr, 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Direction, axisDirArr, 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_XDirection, xDirArr, 1, 3);
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_StartRadius, cmd->startRadius());
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_FocalDepth, cmd->focalDepth());
        
        return true;
    }
}
