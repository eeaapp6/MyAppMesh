/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossDataPropSolidAdaptor.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
//RadiossData
#include "FITK_Component/FITKRadiossData/FITKPropSolid.h"

namespace Interface
{
    bool FITKRadiossDataPropSolidAdaptor::adaptR()
    {
        _solidData = dynamic_cast<Radioss::FITKPropSolid*>(_dataObj);
        if (_reader == nullptr || _solidData == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!readRadiossData(_solidData, *_h5Group)) return false;
        if (!readDataObject(_solidData, *_h5Group)) return false;
        // 获取属性值对象
        Radioss::FITKPropSolidValue* propValue = _solidData->getValue();
        if (propValue == nullptr) return false;

        // 读取属性值参数
        int intValue = 0;
        if (!h5tools->readGroupint(*_h5Group, "Isolid", &intValue)) return false;
        propValue->setIsolid(intValue);

        if (!h5tools->readGroupint(*_h5Group, "Ismstr", &intValue)) return false;
        propValue->setIsmstr(intValue);

        if (!h5tools->readGroupint(*_h5Group, "Icpre", &intValue)) return false;
        propValue->setIcpre(intValue);

        if (!h5tools->readGroupint(*_h5Group, "Itetra4", &intValue)) return false;
        propValue->setItetra4(intValue);

        if (!h5tools->readGroupint(*_h5Group, "Itetra10", &intValue)) return false;
        propValue->setItetra10(intValue);

        if (!h5tools->readGroupint(*_h5Group, "Imas", &intValue)) return false;
        propValue->setImas(intValue);

        if (!h5tools->readGroupint(*_h5Group, "IFrame", &intValue)) return false;
        propValue->setIFrame(intValue);

        // 读取实体单元属性参数
        double doubleValue = 0.0;
        if (!h5tools->readGroupDouble(*_h5Group, "Dn", &doubleValue)) return false;
        _solidData->setDn(doubleValue);

        if (!h5tools->readGroupDouble(*_h5Group, "Qa", &doubleValue)) return false;
        _solidData->setQa(doubleValue);

        if (!h5tools->readGroupDouble(*_h5Group, "Qb", &doubleValue)) return false;
        _solidData->setQb(doubleValue);

        if (!h5tools->readGroupDouble(*_h5Group, "H", &doubleValue)) return false;
        _solidData->setH(doubleValue);

        if (!h5tools->readGroupDouble(*_h5Group, "LambdaV", &doubleValue)) return false;
        _solidData->setLambdaV(doubleValue);

        if (!h5tools->readGroupDouble(*_h5Group, "MuV", &doubleValue)) return false;
        _solidData->setMuV(doubleValue);

        if (!h5tools->readGroupDouble(*_h5Group, "MinTimeStep", &doubleValue)) return false;
        _solidData->setMinTimeStep(doubleValue);

        if (!h5tools->readGroupint(*_h5Group, "Ndir", &intValue)) return false;
        _solidData->setNdir(intValue);

        if (!h5tools->readGroupint(*_h5Group, "SPHPartID", &intValue)) return false;
        _solidData->setSPHPartID(intValue);
        return true;
    }

    bool FITKRadiossDataPropSolidAdaptor::adaptW()
    {
        _solidData = dynamic_cast<Radioss::FITKPropSolid*>(_dataObj);
        if (_writer == nullptr || _solidData == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!writeRadiossData(_solidData, *_h5Group))return false;
        if (!writeDataObject(_solidData, *_h5Group))return false;
        //获取数据
        // 获取属性值对象
        Radioss::FITKPropSolidValue* propValue = _solidData->getValue();
        if (propValue == nullptr) return false;

        // 写入属性值参数
        h5tools->writeGroupAttrInt(*_h5Group, "Isolid", propValue->getIsolid());
        h5tools->writeGroupAttrInt(*_h5Group, "Ismstr", propValue->getIsmstr());
        h5tools->writeGroupAttrInt(*_h5Group, "Icpre", propValue->getIcpre());
        h5tools->writeGroupAttrInt(*_h5Group, "Itetra4", propValue->getItetra4());
        h5tools->writeGroupAttrInt(*_h5Group, "Itetra10", propValue->getItetra10());
        h5tools->writeGroupAttrInt(*_h5Group, "Imas", propValue->getImas());
        h5tools->writeGroupAttrInt(*_h5Group, "IFrame", propValue->getIFrame());

        // 写入实体单元属性参数
        h5tools->writeGroupAttrDouble(*_h5Group, "Dn", _solidData->getDn());
        h5tools->writeGroupAttrDouble(*_h5Group, "Qa", _solidData->getQa());
        h5tools->writeGroupAttrDouble(*_h5Group, "Qb", _solidData->getQb());
        h5tools->writeGroupAttrDouble(*_h5Group, "H", _solidData->getH());
        h5tools->writeGroupAttrDouble(*_h5Group, "LambdaV", _solidData->getLambdaV());
        h5tools->writeGroupAttrDouble(*_h5Group, "MuV", _solidData->getMuV());
        h5tools->writeGroupAttrDouble(*_h5Group, "MinTimeStep", _solidData->getMinTimeStep());
        h5tools->writeGroupAttrInt(*_h5Group, "Ndir", _solidData->getNdir());
        h5tools->writeGroupAttrInt(*_h5Group, "SPHPartID", _solidData->getSPHPartID());

        // 写入属性类型信息
        QString type = Radioss::FITKPropSolid::GetFITKPropSolidRadiossKeyWord();
        h5tools->writeGroupAttrStr(*_h5Group, "PropertyType", type.toStdString());
        return true;
    }
}
