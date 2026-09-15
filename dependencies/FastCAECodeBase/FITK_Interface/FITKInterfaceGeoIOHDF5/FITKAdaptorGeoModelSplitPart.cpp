/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAdaptorGeoModelSplitPart.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelSplitPart.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoEnum.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"


namespace Interface
{
    bool  FITKAdaptorGeoModelSplitPart::readParameters()
    {
        Interface::FITKAbsGeoModelSplitPart* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelSplitPart>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        
        int sourcePartId = -1;
        double origin[3] = {0.0, 0.0, 0.0};
        std::string planeType = {};
        double rotateX = 0.0;
        double rotateY = 0.0;
        double rotateZ = 0.0;
        
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_SourcePartId, &sourcePartId)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Origin, origin, 1, 3)) return false;
        if (!h5tools->readGroupStr(std::get<1>(paramGroup), FGKW_PlaneType, planeType)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_RotateX, &rotateX)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_RotateY, &rotateY)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_RotateZ, &rotateZ)) return false;
        
        QMetaEnum metaEnum = QMetaEnum::fromType<Interface::FITKGeoEnum::PlaneType>();
        Interface::FITKGeoEnum::PlaneType type = 
            (Interface::FITKGeoEnum::PlaneType)metaEnum.keyToValue(planeType.c_str());
        
        cmd->setSourcePartId(sourcePartId);
        cmd->setSplitOrigin(origin[0], origin[1], origin[2]);
        cmd->setSplitPlaneType(type);
        cmd->setRotateX(rotateX);
        cmd->setRotateY(rotateY);
        cmd->setRotateZ(rotateZ);
        
        return true;
    }
    
    bool FITKAdaptorGeoModelSplitPart::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelSplitPart* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelSplitPart>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        
        double origin[3] = {0.0, 0.0, 0.0};
        cmd->getSplitOrigin(origin[0], origin[1], origin[2]);
        
        Core::FITKEnumTransfer<Interface::FITKGeoEnum::PlaneType> enumTransfer;
        bool ok = false;
        QString planeTypeStr = enumTransfer.toString(cmd->getSplitPlaneType(), ok);
        if (!ok) return false;
        
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_SourcePartId, cmd->getSourcePartId());
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Origin, origin, 1, 3);
        h5tools->writeGroupAttrStr(std::get<1>(paramGroup), FGKW_PlaneType, planeTypeStr.toStdString());
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_RotateX, cmd->getRotateX());
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_RotateY, cmd->getRotateY());
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_RotateZ, cmd->getRotateZ());
        
        return true;
    }
}
