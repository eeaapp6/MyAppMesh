/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossDataPropSandwichShellAdaptor.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
//RadiossData
#include "FITK_Component/FITKRadiossData/FITKPropSandwichShell.h"

namespace Interface
{
    bool FITKRadiossDataPropSandwichShellAdaptor::adaptR()
    {
        _data = dynamic_cast<Radioss::FITKPropSandwichShell*>(_dataObj);
        if (_reader == nullptr || _data == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!readRadiossData(_data, *_h5Group)) return false;
        if (!readDataObject(_data, *_h5Group)) return false;
        // 获取属性值对象
        Radioss::FITKPropSandwichShellValue* propValue = _data->getValue();
        if (propValue == nullptr) return false;
        // 读取int类型属性
        int intValue = 0;
        if (!h5tools->readGroupint(*_h5Group, "Ishell", &intValue)) return false;
        propValue->setIshell(intValue);
        if (!h5tools->readGroupint(*_h5Group, "Ismstr", &intValue)) return false;
        propValue->setIsmstr(intValue);
        if (!h5tools->readGroupint(*_h5Group, "Ish3n", &intValue)) return false;
        propValue->setIsh3n(intValue);
        if (!h5tools->readGroupint(*_h5Group, "Idril", &intValue)) return false;
        propValue->setIdril(intValue);
        if (!h5tools->readGroupint(*_h5Group, "N", &intValue)) return false;
        propValue->setN(intValue);
        if (!h5tools->readGroupint(*_h5Group, "Istrain", &intValue)) return false;
        propValue->setIstrain(intValue);
        if (!h5tools->readGroupint(*_h5Group, "Ithick", &intValue)) return false;
        propValue->setIthick(intValue);
        if (!h5tools->readGroupint(*_h5Group, "Iplas", &intValue)) return false;
        propValue->setIplas(intValue);
        if (!h5tools->readGroupint(*_h5Group, "SkewID", &intValue)) return false;
        propValue->setSkewID(intValue);
        if (!h5tools->readGroupint(*_h5Group, "Iorth", &intValue)) return false;
        propValue->setIorth(intValue);
        // 读取bool类型属性
        bool boolValue = false;
        if (!h5tools->readGroupBool(*_h5Group, "Ipos", &boolValue)) return false;
        propValue->setIpos(boolValue);
        // 读取double类型属性
        double doubleValue = 0.0;
        if (!h5tools->readGroupDouble(*_h5Group, "PThickfail", &doubleValue)) return false;
        propValue->setPThickfail(doubleValue);
        if (!h5tools->readGroupDouble(*_h5Group, "Hm", &doubleValue)) return false;
        propValue->setHm(doubleValue);
        if (!h5tools->readGroupDouble(*_h5Group, "Hf", &doubleValue)) return false;
        propValue->setHf(doubleValue);
        if (!h5tools->readGroupDouble(*_h5Group, "Hr", &doubleValue)) return false;
        propValue->setHr(doubleValue);
        if (!h5tools->readGroupDouble(*_h5Group, "Dm", &doubleValue)) return false;
        propValue->setDm(doubleValue);
        if (!h5tools->readGroupDouble(*_h5Group, "Dn", &doubleValue)) return false;
        propValue->setDn(doubleValue);
        if (!h5tools->readGroupDouble(*_h5Group, "Thick", &doubleValue)) return false;
        propValue->setThick(doubleValue);
        if (!h5tools->readGroupDouble(*_h5Group, "Ashear", &doubleValue)) return false;
        propValue->setAshear(doubleValue);
        if (!h5tools->readGroupDouble(*_h5Group, "Vx", &doubleValue)) return false;
        propValue->setVx(doubleValue);
        if (!h5tools->readGroupDouble(*_h5Group, "Vy", &doubleValue)) return false;
        propValue->setVy(doubleValue);
        if (!h5tools->readGroupDouble(*_h5Group, "Vz", &doubleValue)) return false;
        propValue->setVz(doubleValue);
        // 读取夹层层数据（QVector<SandwichShellLayerValue>）
        int layerCount = propValue->getN();
        for (int i = 0; i < layerCount; ++i)
        {
            // 为每个层创建独立的HDF5子分组
            std::string layerGroupName = "Layer_" + std::to_string(i);
            H5::Group layerGroup = _h5Group->openGroup(layerGroupName);
            Radioss::SandwichShellLayerValue layerValue;
            // 读取当前层的int类型属性
            if (!h5tools->readGroupint(layerGroup, "MatID", &intValue)) return false;
            layerValue._matID = intValue;
            // 读取当前层的double类型属性
            if (!h5tools->readGroupDouble(layerGroup, "Angle", &doubleValue)) return false;
            layerValue._angle = doubleValue;
            if (!h5tools->readGroupDouble(layerGroup, "Thickness", &doubleValue)) return false;
            layerValue._thickness = doubleValue;
            if (!h5tools->readGroupDouble(layerGroup, "PositionZ", &doubleValue)) return false;
            layerValue._positionZ = doubleValue;
            if (!h5tools->readGroupDouble(layerGroup, "RelFWeight", &doubleValue)) return false;
            layerValue._relFWeight = doubleValue;
            // 设置当前层数据到属性对象
            propValue->setLayerValue(layerValue, i);
        }
        return true;
    }

    bool FITKRadiossDataPropSandwichShellAdaptor::adaptW()
    {
        _data = dynamic_cast<Radioss::FITKPropSandwichShell*>(_dataObj);
        if (_writer == nullptr || _data == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!writeRadiossData(_data, *_h5Group))return false;
        if (!writeDataObject(_data, *_h5Group))return false;
        // 获取属性值对象
        Radioss::FITKPropSandwichShellValue* propValue = _data->getValue();
        if (propValue == nullptr) return false;
        // 写入int类型属性
        h5tools->writeGroupAttrInt(*_h5Group, "Ishell", propValue->getIshell());
        h5tools->writeGroupAttrInt(*_h5Group, "Ismstr", propValue->getIsmstr());
        h5tools->writeGroupAttrInt(*_h5Group, "Ish3n", propValue->getIsh3n());
        h5tools->writeGroupAttrInt(*_h5Group, "Idril", propValue->getIdril());
        h5tools->writeGroupAttrInt(*_h5Group, "N", propValue->getN());
        h5tools->writeGroupAttrInt(*_h5Group, "Istrain", propValue->getIstrain());
        h5tools->writeGroupAttrInt(*_h5Group, "Ithick", propValue->getIthick());
        h5tools->writeGroupAttrInt(*_h5Group, "Iplas", propValue->getIplas());
        h5tools->writeGroupAttrInt(*_h5Group, "SkewID", propValue->getSkewID());
        h5tools->writeGroupAttrInt(*_h5Group, "Iorth", propValue->getIorth());
        // 写入bool类型属性
        h5tools->writeGroupAttrBool(*_h5Group, "Ipos", propValue->getIpos());
        // 写入double类型属性
        h5tools->writeGroupAttrDouble(*_h5Group, "PThickfail", propValue->getPThickfail());
        h5tools->writeGroupAttrDouble(*_h5Group, "Hm", propValue->getHm());
        h5tools->writeGroupAttrDouble(*_h5Group, "Hf", propValue->getHf());
        h5tools->writeGroupAttrDouble(*_h5Group, "Hr", propValue->getHr());
        h5tools->writeGroupAttrDouble(*_h5Group, "Dm", propValue->getDm());
        h5tools->writeGroupAttrDouble(*_h5Group, "Dn", propValue->getDn());
        h5tools->writeGroupAttrDouble(*_h5Group, "Thick", propValue->getThick());
        h5tools->writeGroupAttrDouble(*_h5Group, "Ashear", propValue->getAshear());
        h5tools->writeGroupAttrDouble(*_h5Group, "Vx", propValue->getVx());
        h5tools->writeGroupAttrDouble(*_h5Group, "Vy", propValue->getVy());
        h5tools->writeGroupAttrDouble(*_h5Group, "Vz", propValue->getVz());

        // 写入夹层层数据（QVector<SandwichShellLayerValue>）
        int layerCount = propValue->getN();
        for (int i = 0; i < layerCount; ++i)
        {
            // 为每个层创建独立的HDF5子分组
            std::string layerGroupName = "Layer_" + std::to_string(i);
            H5::Group group = _h5Group->createGroup(layerGroupName);

            Radioss::SandwichShellLayerValue layerValue = propValue->getLayerValue(i);
            // 写入当前层的int类型属性
            h5tools->writeGroupAttrInt(group, "MatID", layerValue._matID);
            // 写入当前层的double类型属性
            h5tools->writeGroupAttrDouble(group, "Angle", layerValue._angle);
            h5tools->writeGroupAttrDouble(group, "Thickness", layerValue._thickness);
            h5tools->writeGroupAttrDouble(group, "PositionZ", layerValue._positionZ);
            h5tools->writeGroupAttrDouble(group, "RelFWeight", layerValue._relFWeight);
        }
        // 写入属性类型信息
        QString type = Radioss::FITKPropSandwichShell::GetFITKPropSandwichShellRadiossKeyWord();
        h5tools->writeGroupAttrStr(*_h5Group, "PropertyType", type.toStdString());
        return true;
    }
}
