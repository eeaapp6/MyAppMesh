/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossDataBCSAdaptor.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
//RadiossData
#include "FITK_Component/FITKRadiossData/FITKRadiossBCS.h"

namespace Interface
{
    bool FITKRadiossDataBCSAdaptor::adaptR()
    {
        _data = dynamic_cast<Radioss::FITKRadiossBCS*>(_dataObj);
        if (_reader == nullptr || _data == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!readRadiossData(_data, *_h5Group)) return false;
        if (!readDataObject(_data, *_h5Group)) return false;

        // 读取坐标系ID
        int coordinateSystemID = 0;
        if (!h5tools->readGroupint(*_h5Group, "CoordinateSystemID", &coordinateSystemID)) return false;
        _data->setCoordinateSystemID(coordinateSystemID);
        // 读取组ID1
        int groupID1 = 0;
        if (!h5tools->readGroupint(*_h5Group, "GroupID1", &groupID1)) return false;
        // 读取组ID2
        int groupID2 = 0;
        if (!h5tools->readGroupint(*_h5Group, "GroupID2", &groupID2)) return false;
        _data->setGroupID(groupID1, groupID2);
        // 读取6个自由度的固定状态
        for (int i = 0; i < 6; ++i) {
            std::string attrName = "FreedomFixed_" + std::to_string(i);
            bool isFixed = true; // 默认全部约束
            if (!h5tools->readGroupBool(*_h5Group, attrName.c_str(), &isFixed)) {
                // 如果读取失败，保持默认值
                isFixed = true;
            }
            _data->setFreedomFixed(i + 1, isFixed); // 索引从1开始
        }
        return true;
    }

    bool FITKRadiossDataBCSAdaptor::adaptW()
    {
        _data = dynamic_cast<Radioss::FITKRadiossBCS*>(_dataObj);
        if (_writer == nullptr || _data == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!writeRadiossData(_data, *_h5Group))return false;
        if (!writeDataObject(_data, *_h5Group))return false;
        //获取数据
        // 写入坐标系ID
        h5tools->writeGroupAttrInt(*_h5Group, "CoordinateSystemID", _data->getCoordinateSystemID());
        // 写入组ID
        h5tools->writeGroupAttrInt(*_h5Group, "GroupID1", _data->getGroupID1());
        h5tools->writeGroupAttrInt(*_h5Group, "GroupID2", _data->getGroupID2());
        // 写入6个自由度的固定状态
        for (int i = 0; i < 6; ++i) {
            std::string attrName = "FreedomFixed_" + std::to_string(i);
            h5tools->writeGroupAttrBool(*_h5Group, attrName.c_str(), _data->isFreedomFixed(i + 1));
        }

        return true;
    }
}
