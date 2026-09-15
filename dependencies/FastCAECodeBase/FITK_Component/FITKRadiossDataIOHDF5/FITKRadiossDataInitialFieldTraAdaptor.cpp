/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossDataInitialFieldTraAdaptor.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
//RadiossData
#include "FITK_Component/FITKRadiossData/FITKInitialField.h"

namespace Interface
{
    bool FITKRadiossDataInitialFieldTraAdaptor::adaptR()
    {
        _traData = dynamic_cast<Radioss::FITKInitialFieldTra*>(_dataObj);
        if (_reader == nullptr || _traData == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!readRadiossData(_traData, *_h5Group)) return false;
        if (!readDataObject(_traData, *_h5Group)) return false;
        int setID = 0;
        if (!h5tools->readGroupint(*_h5Group, "SetID", &setID)) return false;
        _traData->setSetID(setID);

        int systemID = 0;
        if (!h5tools->readGroupint(*_h5Group, "SystemID", &systemID)) return false;
        _traData->setSystemID(systemID);

        double velocity[3] = { 0.0, 0.0, 0.0 }; 
        if (!h5tools->readGroupDouble(*_h5Group, "Vx", &velocity[0])) return false; 
        if (!h5tools->readGroupDouble(*_h5Group, "Vy", &velocity[1])) return false; 
        if (!h5tools->readGroupDouble(*_h5Group, "Vz", &velocity[2])) return false; 
        _traData->setValue(velocity); 

        std::string fieldTypeStr{};
        if (!h5tools->readGroupStr(*_h5Group, "InitialFieldType", fieldTypeStr)) return false;

        Core::FITKEnumTransfer<Radioss::FITKAbstractInitialField::InitialFieldType> fieldTypeTransfer;
        bool isTypeValid = false;
        Radioss::FITKAbstractInitialField::InitialFieldType fieldType
            = fieldTypeTransfer.fromString(QString::fromStdString(fieldTypeStr), isTypeValid);
        if (!isTypeValid || fieldType != Radioss::FITKAbstractInitialField::InitialFieldType::TRA) return false;
        return true;
    }

    bool FITKRadiossDataInitialFieldTraAdaptor::adaptW()
    {
        _traData = dynamic_cast<Radioss::FITKInitialFieldTra*>(_dataObj);
        if (_writer == nullptr || _traData == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!writeRadiossData(_traData, *_h5Group))return false;
        if (!writeDataObject(_traData, *_h5Group))return false;
        //获取数据
        int setID = _traData->getSetID();                 
        int systemID = _traData->getSystemID();       
        double vx = _traData->getValue(0);          
        double vy = _traData->getValue(1);          
        double vz = _traData->getValue(2);     
        Radioss::FITKAbstractInitialField::InitialFieldType fieldType = _traData->getType();

        h5tools->writeGroupAttrInt(*_h5Group, "SetID", setID);
        h5tools->writeGroupAttrInt(*_h5Group, "SystemID", systemID);
        h5tools->writeGroupAttrDouble(*_h5Group, "Vx", vx);
        h5tools->writeGroupAttrDouble(*_h5Group, "Vy", vy);
        h5tools->writeGroupAttrDouble(*_h5Group, "Vz", vz);

        Core::FITKEnumTransfer<Radioss::FITKAbstractInitialField::InitialFieldType> fieldTypeTransfer;
        bool isTypeValid = false;
        QString typeStr = fieldTypeTransfer.toString(fieldType, isTypeValid);
        if (!isTypeValid) return false; 
        h5tools->writeGroupAttrStr(*_h5Group, "InitialFieldType", typeStr.toStdString());
        //关键字
        QString fieldKeyWord = Radioss::FITKInitialFieldTra::GetFITKInitialFieldTraRadiossKeyWord();
        h5tools->writeGroupAttrStr(*_h5Group, "InitialFieldKeyWord", fieldKeyWord.toStdString()); 
        return true;
    }
}
