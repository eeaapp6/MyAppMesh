/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAdaptorGeoModelBox.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelBox.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"


namespace Interface
{
    bool  FITKAdaptorGeoModelBox::readParameters()
    {
        Interface::FITKAbsGeoModelBox* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelBox>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        double length[3] = { 0.0, 0.0, 0.0 };
        double p1[3] = { 0.0, 0.0, 0.0 };
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Length, length, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Point, p1, 1, 3))return false;
        cmd->setPoint1(p1);
        cmd->setLength(length);
        return true;
    }
    bool FITKAdaptorGeoModelBox::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelBox* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelBox>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool,H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if(!std::get<0>(paramGroup)) return false;
        
        double length[3] = {0.0, 0.0, 0.0};
        double p1[3] = {0.0, 0.0, 0.0};
        cmd->getLength(length);
        cmd->getPoint1(p1);

        //写出参数
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Length, length,1,3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Point, p1, 1, 3);

        return true;
    }

}



