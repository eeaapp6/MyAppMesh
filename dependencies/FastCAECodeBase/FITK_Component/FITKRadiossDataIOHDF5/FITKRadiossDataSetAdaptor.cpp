/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossDataSetAdaptor.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
#include "FITK_Interface/FITKInterfaceModel/FITKModelSet.h"
#include "FITK_Interface/FITKInterfaceModel/FITKModelEnum.h"
//RadiossData
#include "FITK_Component/FITKRadiossData/FITKRadiossNodeGroup.h"

namespace Interface
{
    bool FITKRadiossDataSetAdaptor::adaptR()
    {
        _setData = dynamic_cast<Radioss::FITKRadiossNodeGroup*>(_dataObj);
        if (_reader == nullptr || _setData == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!readRadiossData(_setData, *_h5Group)) return false;
        if (!readDataObject(_setData, *_h5Group)) return false;
        std::string setTypeStr = {};
        bool internal = false;
        bool generated = false;
        std::vector<int> memberVector{};
        int count = {};
        int id = 0;
        int modelId = 0;
        if (!h5tools->readGroupStr(*_h5Group, "SetType", setTypeStr))return false;
        if (!h5tools->readGroupBool(*_h5Group, "isInternal", &internal))return false;
        if (!h5tools->readGroupBool(*_h5Group, "isGenerated", &generated))return false;
        if (!h5tools->readGroupint(*_h5Group, "MemberCount", &count))return false;
        if (!h5tools->readGroupint(*_h5Group, "ModelID", &modelId))return false;
        int colorArray[3] = {};
        if (!h5tools->readGroupAttrIntArray(*_h5Group, "Color", colorArray, 1, 3)) return false;
        QColor color = {};
        color.setRed(colorArray[0]);
        color.setGreen(colorArray[1]);
        color.setBlue(colorArray[2]);
        _setData->setColor(color);
        _setData->setModel(modelId);
        Core::FITKEnumTransfer<FITKModelEnum::FITKModelSetType> fitkEleTypeTrafer;
        bool isValid = false;
        FITKModelEnum::FITKModelSetType setType = fitkEleTypeTrafer.fromString(QString::fromStdString(setTypeStr), isValid);
        if (!isValid) return false;

        _setData->setModelSetType(setType);
        _setData->setInternal(internal);
        _setData->setGenerated(generated);
        if (count > 0)
        {
            // 分配vector空间（避免内存越界）
            memberVector.resize(count);
            // 读取1行×memberCount列的int数组（与adaptW的写入维度对齐）
            if (!h5tools->readGroupAttrIntArray(*_h5Group,"Member",memberVector.data(),1, count))return false;
            // 将vector转换为QList，赋值给_setData
            QList<int> setMember;
            for (int memberId : memberVector)
            {
                _setData->appendMember(memberId);
            }
        }
        else
        {
            // 成员数量为0，设置空的QList
            _setData->setAbsoluteMember(QList<int>());
        }
        return true;
    }

    bool FITKRadiossDataSetAdaptor::adaptW()
    {
        _setData = dynamic_cast<Radioss::FITKRadiossNodeGroup*>(_dataObj);
        if (_writer == nullptr || _setData == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!writeRadiossData(_setData, *_h5Group))return false;
        if (!writeDataObject(_setData, *_h5Group))return false;
        //获取数据
        FITKModelEnum::FITKModelSetType setType = _setData->getModelSetType();
        Core::FITKEnumTransfer<FITKModelEnum::FITKModelSetType> fitkSetTypeTrafer;
        bool isValid = false;
        QString setTypeStr = fitkSetTypeTrafer.toString(setType, isValid);
        if(!isValid) return false;
        bool internal = _setData->isInternal();
        bool generated = _setData->isGenerated();
        QList<int> setMember = _setData->getMember();
        int count = setMember.size();
        int colorArray[3] = {};
        colorArray[0] = _setData->getColor().red();
        colorArray[1] = _setData->getColor().green();
        colorArray[2] = _setData->getColor().blue();
        h5tools->writeGroupAttrIntArray(*_h5Group, "Color", colorArray, 1, 3);
        h5tools->writeGroupAttrInt(*_h5Group, "ModelID", _setData->getAbsModelID());
        //写出数据
        h5tools->writeGroupAttrStr(*_h5Group, "SetType", setTypeStr.toStdString());
        h5tools->writeGroupAttrBool(*_h5Group, "isInternal", internal);
        h5tools->writeGroupAttrBool(*_h5Group, "isGenerated", generated);
        h5tools->writeGroupAttrInt(*_h5Group, "MemberCount", count);
        if (count != 0)
        {
            //将QList<int>转换为int数组
            std::vector<int> memberVector;
            memberVector.reserve(count);
            for (int i = 0; i < count; ++i) {
                memberVector.push_back(setMember[i]);
            }
            h5tools->writeGroupAttrIntArray(*_h5Group, "Member", memberVector.data(), 1, count);
        }
        return true;
    }
}
