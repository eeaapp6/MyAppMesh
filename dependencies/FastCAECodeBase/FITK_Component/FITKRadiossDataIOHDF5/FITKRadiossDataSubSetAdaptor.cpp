/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossDataSubSetAdaptor.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
//RadiossData
#include "FITK_Component/FITKRadiossData/FITKRadiossNodesParts.h"

namespace Interface
{
    bool FITKRadiossDataSubSetAdaptor::adaptR()
    {
        _subSetData = dynamic_cast<Radioss::FITKRadiossSubSet*>(_dataObj);
        if (_reader == nullptr || _subSetData == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!readRadiossData(_subSetData, *_h5Group)) return false;
        if (!readDataObject(_subSetData, *_h5Group)) return false;
        std::vector<int> partIDVector{};
        int count = {};
        if (!h5tools->readGroupint(*_h5Group, "PartCount", &count))return false;
        if (count == 0)return true;
        partIDVector.resize(count);
        if (!h5tools->readGroupAttrIntArray(*_h5Group,"PartIDs", partIDVector.data(),1, count))return false;
        for (int partID : partIDVector)
        {
            _subSetData->addPartID(partID);
        }
        return true;
    }

    bool FITKRadiossDataSubSetAdaptor::adaptW()
    {
        _subSetData = dynamic_cast<Radioss::FITKRadiossSubSet*>(_dataObj);
        if (_writer == nullptr || _subSetData == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!writeRadiossData(_subSetData, *_h5Group))return false;
        if (!writeDataObject(_subSetData, *_h5Group))return false;
        QList<int> partIDs = _subSetData->getPartIDs();
        int count = partIDs.size();
        //写出数据
        h5tools->writeGroupAttrInt(*_h5Group, "PartCount", count);
        if (count == 0)return true;
        //将QList<int>转换为int数组
        std::vector<int> partIDVector;
        partIDVector.reserve(count);
        for (int i = 0; i < count; ++i) {
            partIDVector.push_back(partIDs[i]);
        }
        h5tools->writeGroupAttrIntArray(*_h5Group, "PartIDs", partIDVector.data(), 1, count);
        return true;
    }
}
