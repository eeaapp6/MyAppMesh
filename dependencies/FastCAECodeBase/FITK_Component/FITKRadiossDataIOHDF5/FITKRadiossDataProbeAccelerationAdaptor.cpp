/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossDataProbeAccelerationAdaptor.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
//RadiossData
#include "FITK_Component/FITKRadiossData/FITKProbeAcceleration.h"

namespace Interface
{
    bool FITKRadiossDataProbeAccelerationAdaptor::adaptR()
    {
        _data = dynamic_cast<Radioss::FITKProbeAcceleration*>(_dataObj);
        if (_reader == nullptr || _data == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!readRadiossData(_data, *_h5Group)) return false;
        if (!readDataObject(_data, *_h5Group)) return false;

        // 读取坐标系ID
        int systemID = -1;
        if (!h5tools->readGroupint(*_h5Group, "SystemID", &systemID)) return false;
        _data->setSystemID(systemID);
        // 读取集合ID
        int setID = -1;
        if (!h5tools->readGroupint(*_h5Group, "SetID", &setID)) return false;
        _data->setSetID(setID);
        // 读取截止频率
        double cutOffFreq = 1.65; // 默认值
        if (!h5tools->readGroupDouble(*_h5Group, "CutOffFreq", &cutOffFreq)) return false;
        _data->setCutOffFreq(cutOffFreq);
        return true;
    }

    bool FITKRadiossDataProbeAccelerationAdaptor::adaptW()
    {
        _data = dynamic_cast<Radioss::FITKProbeAcceleration*>(_dataObj);
        if (_writer == nullptr || _data == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!writeRadiossData(_data, *_h5Group)) return false;
        if (!writeDataObject(_data, *_h5Group)) return false;

        // 获取数据
        int systemID = _data->getSystemID();
        int setID = _data->getSetID();
        double cutOffFreq = _data->getCutOffFreq();

        // 写入基本属性
        h5tools->writeGroupAttrInt(*_h5Group, "SystemID", systemID);
        h5tools->writeGroupAttrInt(*_h5Group, "SetID", setID);
        h5tools->writeGroupAttrDouble(*_h5Group, "CutOffFreq", cutOffFreq);

        // 写入关键字
        QString probeKeyWord = Radioss::FITKProbeAcceleration::GetFITKProbeAccelerationRadiossKeyWord();
        h5tools->writeGroupAttrStr(*_h5Group, "ProbeKeyWord", probeKeyWord.toStdString());

        return true;
    }
}