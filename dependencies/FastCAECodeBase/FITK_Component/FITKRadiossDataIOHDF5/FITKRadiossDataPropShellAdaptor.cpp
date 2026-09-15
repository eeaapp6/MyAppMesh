/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossDataPropShellAdaptor.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
//RadiossData
#include "FITK_Component/FITKRadiossData/FITKPropShell.h"

namespace Interface
{
    bool FITKRadiossDataPropShellAdaptor::adaptR()
    {
        _shellData = dynamic_cast<Radioss::FITKPropShell*>(_dataObj);
        if (_reader == nullptr || _shellData == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!readRadiossData(_shellData, *_h5Group)) return false;
        if (!readDataObject(_shellData, *_h5Group)) return false;
        // 获取属性值对象
        Radioss::FITKPropShellValue* propValue = _shellData->getValue();
        if (propValue == nullptr) return false;
        // 读取属性值参数
        int intValue = 0;
        if (!h5tools->readGroupint(*_h5Group, "Ishell", &intValue)) return false;
        propValue->setIshell(intValue);

        if (!h5tools->readGroupint(*_h5Group, "Ismstr", &intValue)) return false;
        propValue->setIsmstr(intValue);

        if (!h5tools->readGroupint(*_h5Group, "Ithick", &intValue)) return false;
        propValue->setIthick(intValue);

        if (!h5tools->readGroupint(*_h5Group, "Iplas", &intValue)) return false;
        propValue->setIplas(intValue);

        if (!h5tools->readGroupint(*_h5Group, "Ish3n", &intValue)) return false;
        propValue->setIsh3n(intValue);

        if (!h5tools->readGroupint(*_h5Group, "Idril", &intValue)) return false;
        propValue->setIdril(intValue);

        // 读取壳单元属性参数
        double doubleValue = 0.0;
        if (!h5tools->readGroupint(*_h5Group, "Ipinch", &intValue)) return false;
        _shellData->setIpinch(intValue);

        if (!h5tools->readGroupDouble(*_h5Group, "PthickFail", &doubleValue)) return false;
        _shellData->setPthickFail(doubleValue);

        if (!h5tools->readGroupDouble(*_h5Group, "Hm", &doubleValue)) return false;
        _shellData->setHm(doubleValue);

        if (!h5tools->readGroupDouble(*_h5Group, "Hf", &doubleValue)) return false;
        _shellData->setHf(doubleValue);

        if (!h5tools->readGroupDouble(*_h5Group, "Hr", &doubleValue)) return false;
        _shellData->setHr(doubleValue);

        if (!h5tools->readGroupDouble(*_h5Group, "Dm", &doubleValue)) return false;
        _shellData->setDm(doubleValue);

        if (!h5tools->readGroupDouble(*_h5Group, "Dn", &doubleValue)) return false;
        _shellData->setDn(doubleValue);

        if (!h5tools->readGroupint(*_h5Group, "N", &intValue)) return false;
        _shellData->setN(intValue);

        if (!h5tools->readGroupDouble(*_h5Group, "Thick", &doubleValue)) return false;
        _shellData->setThick(doubleValue);

        if (!h5tools->readGroupDouble(*_h5Group, "Ashear", &doubleValue)) return false;
        _shellData->setAshear(doubleValue);
        return true;
    }

    bool FITKRadiossDataPropShellAdaptor::adaptW()
    {
        _shellData = dynamic_cast<Radioss::FITKPropShell*>(_dataObj);
        if (_writer == nullptr || _shellData == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!writeRadiossData(_shellData, *_h5Group))return false;
        if (!writeDataObject(_shellData, *_h5Group))return false;
        // 获取属性值对象
        Radioss::FITKPropShellValue* propValue = _shellData->getValue();
        if (propValue == nullptr) return false;

        // 写入属性值参数
        h5tools->writeGroupAttrInt(*_h5Group, "Ishell", propValue->getIshell());
        h5tools->writeGroupAttrInt(*_h5Group, "Ismstr", propValue->getIsmstr());
        h5tools->writeGroupAttrInt(*_h5Group, "Ithick", propValue->getIthick());
        h5tools->writeGroupAttrInt(*_h5Group, "Iplas", propValue->getIplas());
        h5tools->writeGroupAttrInt(*_h5Group, "Ish3n", propValue->getIsh3n());
        h5tools->writeGroupAttrInt(*_h5Group, "Idril", propValue->getIdril());

        // 写入壳单元属性参数
        h5tools->writeGroupAttrInt(*_h5Group, "Ipinch", _shellData->getIpinch());
        h5tools->writeGroupAttrDouble(*_h5Group, "PthickFail", _shellData->getPthickFail());
        h5tools->writeGroupAttrDouble(*_h5Group, "Hm", _shellData->getHm());
        h5tools->writeGroupAttrDouble(*_h5Group, "Hf", _shellData->getHf());
        h5tools->writeGroupAttrDouble(*_h5Group, "Hr", _shellData->getHr());
        h5tools->writeGroupAttrDouble(*_h5Group, "Dm", _shellData->getDm());
        h5tools->writeGroupAttrDouble(*_h5Group, "Dn", _shellData->getDn());
        h5tools->writeGroupAttrInt(*_h5Group, "N", _shellData->getN());
        h5tools->writeGroupAttrDouble(*_h5Group, "Thick", _shellData->getThick());
        h5tools->writeGroupAttrDouble(*_h5Group, "Ashear", _shellData->getAshear());

        // 写入属性类型信息
        QString type = Radioss::FITKPropShell::GetFITKPropShellRadiossKeyWord();
        h5tools->writeGroupAttrStr(*_h5Group, "PropertyType", type.toStdString());
        return true;
    }
}
