/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossEquationOfStatePolynomialAdaptor.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
//RadiossData
#include "FITK_Component/FITKRadiossData/FITKEquationOfStatePolynomial.h"

namespace Interface
{
    bool FITKRadiossEquationOfStatePolynomialAdaptor::adaptR()
    {
        _data = dynamic_cast<Radioss::FITKEquationOfStatePolynomial*>(_dataObj);
        if (_reader == nullptr || _data == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!readRadiossData(_data, *_h5Group)) return false;
        if (!readDataObject(_data, *_h5Group)) return false;
        // 读取基类FITKAbstractEquationOfState的参数
        int count = 0;
        if (!h5tools->readGroupint(*_h5Group, "MaterialIDCount", &count)) return false;
        std::vector<int> materialIDVector{};
        // 分配vector空间（避免内存越界）
        materialIDVector.resize(count);
        if (!h5tools->readGroupAttrIntArray(*_h5Group, "MaterialIDs", materialIDVector.data(), 1, count))return false;
        for (int materialID : materialIDVector)
        {
            _data->addMaterialID(materialID);
        }
        double doubleValue = 0.0;
        if (!h5tools->readGroupDouble(*_h5Group, "P0", &doubleValue)) return false;
        _data->setP0(doubleValue);
        // 读取FITKEquationOfStatePolynomial参数
        if (!h5tools->readGroupDouble(*_h5Group, "C0", &doubleValue)) return false;
        _data->setC0(doubleValue);
        if (!h5tools->readGroupDouble(*_h5Group, "C1", &doubleValue)) return false;
        _data->setC1(doubleValue);
        if (!h5tools->readGroupDouble(*_h5Group, "C2", &doubleValue)) return false;
        _data->setC2(doubleValue);
        if (!h5tools->readGroupDouble(*_h5Group, "C3", &doubleValue)) return false;
        _data->setC3(doubleValue);
        if (!h5tools->readGroupDouble(*_h5Group, "C4", &doubleValue)) return false;
        _data->setC4(doubleValue);
        if (!h5tools->readGroupDouble(*_h5Group, "C5", &doubleValue)) return false;
        _data->setC5(doubleValue);
        if (!h5tools->readGroupDouble(*_h5Group, "E0", &doubleValue)) return false;
        _data->setE0(doubleValue);
        if (!h5tools->readGroupDouble(*_h5Group, "Psh", &doubleValue)) return false;
        _data->setPsh(doubleValue);
        return true;
    }

    bool FITKRadiossEquationOfStatePolynomialAdaptor::adaptW()
    {
        _data = dynamic_cast<Radioss::FITKEquationOfStatePolynomial*>(_dataObj);
        if (_writer == nullptr || _data == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!writeRadiossData(_data, *_h5Group))return false;
        if (!writeDataObject(_data, *_h5Group))return false;
        //获取数据
        // 写入基类FITKAbstractEquationOfState的参数
        QList<int> materialIDs = _data->getMaterialIDs();
        int count = materialIDs.size();
        //将QList<int>转换为int数组
        std::vector<int> materialIDVector;
        materialIDVector.reserve(count);
        for (int i = 0; i < count; ++i) {
            materialIDVector.push_back(materialIDs[i]);
        }
        h5tools->writeGroupAttrInt(*_h5Group, "MaterialIDCount", count);
        h5tools->writeGroupAttrIntArray(*_h5Group, "MaterialIDs", materialIDVector.data(), 1, count);
        h5tools->writeGroupAttrDouble(*_h5Group, "P0", _data->getP0());
        // 写入FITKEquationOfStatePolynomial参数
        h5tools->writeGroupAttrDouble(*_h5Group, "C0", _data->getC0());
        h5tools->writeGroupAttrDouble(*_h5Group, "C1", _data->getC1());
        h5tools->writeGroupAttrDouble(*_h5Group, "C2", _data->getC2());
        h5tools->writeGroupAttrDouble(*_h5Group, "C3", _data->getC3());
        h5tools->writeGroupAttrDouble(*_h5Group, "C4", _data->getC4());
        h5tools->writeGroupAttrDouble(*_h5Group, "C5", _data->getC5());
        h5tools->writeGroupAttrDouble(*_h5Group, "E0", _data->getE0());
        h5tools->writeGroupAttrDouble(*_h5Group, "Psh", _data->getPsh());
        QString type = Radioss::FITKEquationOfStatePolynomial::GetFITKEquationOfStatePolynomialRadiossKeyWord();
        h5tools->writeGroupAttrStr(*_h5Group, "EOSType", type.toStdString());
        return true;
    }
}
