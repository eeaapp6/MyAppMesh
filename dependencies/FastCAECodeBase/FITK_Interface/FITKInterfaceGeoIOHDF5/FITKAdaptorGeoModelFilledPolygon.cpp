/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAdaptorGeoModelFilledPolygon.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelFilledPolygon.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"


namespace Interface
{
    bool  FITKAdaptorGeoModelFilledPolygon::readParameters()
    {
        Interface::FITKAbsGeoModelFilledPolygon* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelFilledPolygon>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        
        int count = 0;
        int isFilled = 1;
        
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_Count, &count)) return false;
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_IsFilled, &isFilled)) return false;
        
        cmd->clearPoints();
        for (int i = 0; i < count; i++)
        {
            double point[3] = {0.0, 0.0, 0.0};
            std::string pointKey = "Point_" + std::to_string(i);
            if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), pointKey.c_str(), point, 1, 3)) return false;
            cmd->addPoint(point[0], point[1], point[2]);
        }
        
        cmd->setFilled(isFilled != 0);
        
        return true;
    }
    
    bool FITKAdaptorGeoModelFilledPolygon::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelFilledPolygon* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelFilledPolygon>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        
        QList<std::array<double, 3>> points = cmd->points();
        int count = points.count();
        
        for (int i = 0; i < count; i++)
        {
            std::array<double, 3> point = points[i];
            double pointArr[3] = {point[0], point[1], point[2]};
            std::string pointKey = "Point_" + std::to_string(i);
            h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), pointKey.c_str(), pointArr, 1, 3);
        }
        
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_Count, count);
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_IsFilled, cmd->isFilled() ? 1 : 0);
        
        return true;
    }
}
