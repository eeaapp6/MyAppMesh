/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossDataPropSPHAdaptor.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
//RadiossData
#include "FITK_Component/FITKRadiossData/FITKPropSPH.h"

namespace Interface
{
    bool FITKRadiossDataPropSPHAdaptor::adaptR()
    {
        _data = dynamic_cast<Radioss::FITKPropSPH*>(_dataObj);
        if (_reader == nullptr || _data == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!readRadiossData(_data, *_h5Group)) return false;
        if (!readDataObject(_data, *_h5Group)) return false;
        // 获取属性值对象
        Radioss::FITKPropSPHValue* propValue = _data->getValue();
        if (propValue == nullptr) return false;
        int intValue = 0;
        if (!h5tools->readGroupint(*_h5Group, "SkewID", &intValue)) return false;
        propValue->setSkewID(intValue);

        if (!h5tools->readGroupint(*_h5Group, "HID", &intValue)) return false;
        propValue->setHID(intValue);

        if (!h5tools->readGroupint(*_h5Group, "OrderID", &intValue)) return false;
        propValue->setOrderID(intValue);

        double doubleValue = 0.0;
        if (!h5tools->readGroupDouble(*_h5Group, "MP", &doubleValue)) return false;
        propValue->setMP(doubleValue);

        if (!h5tools->readGroupDouble(*_h5Group, "QA", &doubleValue)) return false;
        propValue->setQA(doubleValue);

        if (!h5tools->readGroupDouble(*_h5Group, "QB", &doubleValue)) return false;
        propValue->setQB(doubleValue);

        if (!h5tools->readGroupDouble(*_h5Group, "ACS", &doubleValue)) return false;
        propValue->setACS(doubleValue);

        if (!h5tools->readGroupDouble(*_h5Group, "H", &doubleValue)) return false;
        propValue->setH(doubleValue);

        if (!h5tools->readGroupDouble(*_h5Group, "Stab", &doubleValue)) return false;
        propValue->setStab(doubleValue);
        return true;
    }

    bool FITKRadiossDataPropSPHAdaptor::adaptW()
    {
        _data = dynamic_cast<Radioss::FITKPropSPH*>(_dataObj);
        if (_writer == nullptr || _data == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!writeRadiossData(_data, *_h5Group))return false;
        if (!writeDataObject(_data, *_h5Group))return false;
        // 获取属性值对象
        Radioss::FITKPropSPHValue* propValue = _data->getValue();
        if (propValue == nullptr) return false;

        // 写入int类型属性参数
        h5tools->writeGroupAttrInt(*_h5Group, "SkewID", propValue->getSkewID());
        h5tools->writeGroupAttrInt(*_h5Group, "HID", propValue->getHID());
        h5tools->writeGroupAttrInt(*_h5Group, "OrderID", propValue->getOrderID());

        // 写入double类型属性参数
        h5tools->writeGroupAttrDouble(*_h5Group, "MP", propValue->getMP());
        h5tools->writeGroupAttrDouble(*_h5Group, "QA", propValue->getQA());
        h5tools->writeGroupAttrDouble(*_h5Group, "QB", propValue->getQB());
        h5tools->writeGroupAttrDouble(*_h5Group, "ACS", propValue->getACS());
        h5tools->writeGroupAttrDouble(*_h5Group, "H", propValue->getH());
        h5tools->writeGroupAttrDouble(*_h5Group, "Stab", propValue->getStab());

        // 写入属性类型信息
        QString type = Radioss::FITKPropSPH::GetFITKPropSPHRadiossKeyWord();
        h5tools->writeGroupAttrStr(*_h5Group, "PropertyType", type.toStdString());
        return true;
    }
}
