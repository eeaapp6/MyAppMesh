/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossDataFailureModelJOHNSONAdaptor.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
//RadiossData
#include "FITK_Component/FITKRadiossData/FITKFailureModelJOHNSON.h"

namespace Interface
{
    bool FITKRadiossDataFailureModelJOHNSONAdaptor::adaptR()
    {
        _data = dynamic_cast<Radioss::FITKFailureModelJOHNSON*>(_dataObj);
        if (_reader == nullptr || _data == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!readRadiossData(_data, *_h5Group)) return false;
        if (!readDataObject(_data, *_h5Group)) return false;

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
        // D1 
        double d1 = 0.0;
        if (!h5tools->readGroupDouble(*_h5Group, "D1", &d1)) return false;
        _data->setD1(d1);
        // D2 
        double d2 = 0.0;
        if (!h5tools->readGroupDouble(*_h5Group, "D2", &d2)) return false;
        _data->setD2(d2);
        // D3 
        double d3 = 0.0;
        if (!h5tools->readGroupDouble(*_h5Group, "D3", &d3)) return false;
        _data->setD3(d3);
        // D4 
        double d4 = 0.0;
        if (!h5tools->readGroupDouble(*_h5Group, "D4", &d4)) return false;
        _data->setD4(d4);
        // D5 
        double d5 = 0.0;
        if (!h5tools->readGroupDouble(*_h5Group, "D5", &d5)) return false;
        _data->setD5(d5);
        // Eps0 
        double eps0 = 0.0;
        if (!h5tools->readGroupDouble(*_h5Group, "Eps0", &eps0)) return false;
        _data->setEps0(eps0);
        // Ifailsh 
        int ifailsh = 1;
        if (!h5tools->readGroupint(*_h5Group, "Ifailsh", &ifailsh)) return false;
        _data->setIfailsh(ifailsh);
        // Ifailso 
        int ifailso = 1;
        if (!h5tools->readGroupint(*_h5Group, "Ifailso", &ifailso)) return false;
        _data->setIfailso(ifailso);
        // Dadv 
        double dadv = 1.0;
        if (!h5tools->readGroupDouble(*_h5Group, "Dadv", &dadv)) return false;
        _data->setDadv(dadv);
        // Ixfem 
        int ixfem = 0;
        if (!h5tools->readGroupint(*_h5Group, "Ixfem", &ixfem)) return false;
        _data->setIxfem(ixfem);
        return true;
    }

    bool FITKRadiossDataFailureModelJOHNSONAdaptor::adaptW()
    {
        _data = dynamic_cast<Radioss::FITKFailureModelJOHNSON*>(_dataObj);
        if (_writer == nullptr || _data == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!writeRadiossData(_data, *_h5Group))return false;
        if (!writeDataObject(_data, *_h5Group))return false;
        // D1
        h5tools->writeGroupAttrDouble(*_h5Group, "D1", _data->getD1());
        // D2 
        h5tools->writeGroupAttrDouble(*_h5Group, "D2", _data->getD2());
        // D3
        h5tools->writeGroupAttrDouble(*_h5Group, "D3", _data->getD3());
        // D4 
        h5tools->writeGroupAttrDouble(*_h5Group, "D4", _data->getD4());
        // D5
        h5tools->writeGroupAttrDouble(*_h5Group, "D5", _data->getD5());
        // Eps0 
        h5tools->writeGroupAttrDouble(*_h5Group, "Eps0", _data->getEps0());
        // Ifailsh
        h5tools->writeGroupAttrInt(*_h5Group, "Ifailsh", _data->getIfailsh());
        // Ifailso 
        h5tools->writeGroupAttrInt(*_h5Group, "Ifailso", _data->getIfailso());
        // Dadv 
        h5tools->writeGroupAttrDouble(*_h5Group, "Dadv", _data->getDadv());
        // Ixfem 
        h5tools->writeGroupAttrInt(*_h5Group, "Ixfem", _data->getIxfem());

        QString type = Radioss::FITKFailureModelJOHNSON::GetFITKFailureModelJOHNSONRadiossKeyWord();
        h5tools->writeGroupAttrStr(*_h5Group, "FailureModelType", type.toStdString());
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
        return true;
    }
}