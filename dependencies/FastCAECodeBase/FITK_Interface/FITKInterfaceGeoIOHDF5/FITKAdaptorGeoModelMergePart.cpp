/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAdaptorGeoModelMergePart.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelMergePart.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"


namespace Interface
{
    bool  FITKAdaptorGeoModelMergePart::readParameters()
    {
        Interface::FITKAbsGeoModelMergePart* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelMergePart>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        
        std::string type = {};
        int count = 0;
        
        if (!h5tools->readGroupStr(std::get<1>(paramGroup), FGKW_Type, type)) return false;
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_Count, &count)) return false;
        
        QMetaEnum metaEnum = QMetaEnum::fromType<Interface::FITKAbsGeoModelMergePart::GeoBoolOperType>();
        Interface::FITKAbsGeoModelMergePart::GeoBoolOperType cmdType = 
            (Interface::FITKAbsGeoModelMergePart::GeoBoolOperType)metaEnum.keyToValue(type.c_str());
        cmd->setMergeOperType(cmdType);
        
        cmd->clearCommandIds();
        for (int i = 0; i < count; i++)
        {
            int cmdId = 0;
            std::string cmdIdKey = "CmdID_" + std::to_string(i);
            if (!h5tools->readGroupint(std::get<1>(paramGroup), cmdIdKey.c_str(), &cmdId)) return false;
            cmd->addCommandId(cmdId);
        }
        
        return true;
    }
    
    bool FITKAdaptorGeoModelMergePart::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelMergePart* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelMergePart>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        
        FITKAbsGeoModelMergePart::GeoBoolOperType t = cmd->getMergeOperType();
        bool ok = false;
        Core::FITKEnumTransfer<Interface::FITKAbsGeoModelMergePart::GeoBoolOperType> enumTransfer;
        QString cmdTypeStr = enumTransfer.toString(t, ok);
        if (!ok) return false;
        
        QList<int> cmdIds = cmd->getCommandIds();
        int count = cmdIds.count();
        
        for (int i = 0; i < count; i++)
        {
            std::string cmdIdKey = "CmdID_" + std::to_string(i);
            h5tools->writeGroupAttrInt(std::get<1>(paramGroup), cmdIdKey.c_str(), cmdIds[i]);
        }
        
        h5tools->writeGroupAttrStr(std::get<1>(paramGroup), FGKW_Type, cmdTypeStr.toStdString());
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_Count, count);
        
        return true;
    }
}
