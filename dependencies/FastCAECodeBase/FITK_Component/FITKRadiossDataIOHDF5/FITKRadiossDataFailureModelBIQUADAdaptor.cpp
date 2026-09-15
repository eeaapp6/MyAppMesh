/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossDataFailureModelBIQUADAdaptor.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
//RadiossData
#include "FITK_Component/FITKRadiossData/FITKFailureModelBIQUAD.h"

namespace Interface
{
    bool FITKRadiossDataFailureModelBIQUADAdaptor::adaptR()
    {
        _data = dynamic_cast<Radioss::FITKFailureModelBIQUAD*>(_dataObj);
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
        // C1 
        double c1 = 0.0;
        if (!h5tools->readGroupDouble(*_h5Group, "C1", &c1)) return false;
        _data->setC1(c1);
        // C2 
        double c2 = 0.0;
        if (!h5tools->readGroupDouble(*_h5Group, "C2", &c2)) return false;
        _data->setC2(c2);
        // C3 
        double c3 = 0.0;
        if (!h5tools->readGroupDouble(*_h5Group, "C3", &c3)) return false;
        _data->setC3(c3);
        // C4 
        double c4 = 0.0;
        if (!h5tools->readGroupDouble(*_h5Group, "C4", &c4)) return false;
        _data->setC4(c4);
        // C5 
        double c5 = 0.0;
        if (!h5tools->readGroupDouble(*_h5Group, "C5", &c5)) return false;
        _data->setC5(c5);
        // Pthickfail 
        double pthickfail = 1.0;
        if (!h5tools->readGroupDouble(*_h5Group, "Pthickfail", &pthickfail)) return false;
        _data->setPthickfail(pthickfail);
        // MFlag 
        int mFlag = 0;
        if (!h5tools->readGroupint(*_h5Group, "MFlag", &mFlag)) return false;
        _data->setMFlag(mFlag);
        // SFlag 
        int sFlag = 1;
        if (!h5tools->readGroupint(*_h5Group, "SFlag", &sFlag)) return false;
        _data->setSFlag(sFlag);
        // Inststart 
        double inststart = 0.0;
        if (!h5tools->readGroupDouble(*_h5Group, "Inststart", &inststart)) return false;
        _data->setInststart(inststart);
        // FctIDel 
        int fctIDel = -1;
        if (!h5tools->readGroupint(*_h5Group, "FctIDel", &fctIDel)) return false;
        _data->setFctIDel(fctIDel);
        // Elref 
        double elref = 1.0;
        if (!h5tools->readGroupDouble(*_h5Group, "Elref", &elref)) return false;
        _data->setElref(elref);
        // R1 
        double r1 = 0.0;
        if (!h5tools->readGroupDouble(*_h5Group, "R1", &r1)) return false;
        _data->setR1(r1);
        // R2 
        double r2 = 0.0;
        if (!h5tools->readGroupDouble(*_h5Group, "R2", &r2)) return false;
        _data->setR2(r2);
        // R4 
        double r4 = 0.0;
        if (!h5tools->readGroupDouble(*_h5Group, "R4", &r4)) return false;
        _data->setR4(r4);
        // R5 
        double r5 = 0.0;
        if (!h5tools->readGroupDouble(*_h5Group, "R5", &r5)) return false;
        _data->setR5(r5);
        return true;
    }

    bool FITKRadiossDataFailureModelBIQUADAdaptor::adaptW()
    {
        _data = dynamic_cast<Radioss::FITKFailureModelBIQUAD*>(_dataObj);
        if (_writer == nullptr || _data == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!writeRadiossData(_data, *_h5Group))return false;
        if (!writeDataObject(_data, *_h5Group))return false;
        // C1
        h5tools->writeGroupAttrDouble(*_h5Group, "C1", _data->getC1());
        // C2 
        h5tools->writeGroupAttrDouble(*_h5Group, "C2", _data->getC2());
        // C3
        h5tools->writeGroupAttrDouble(*_h5Group, "C3", _data->getC3());
        // C4 
        h5tools->writeGroupAttrDouble(*_h5Group, "C4", _data->getC4());
        // C5
        h5tools->writeGroupAttrDouble(*_h5Group, "C5", _data->getC5());
        // Pthickfail 
        h5tools->writeGroupAttrDouble(*_h5Group, "Pthickfail", _data->getPthickfail());
        // MFlag
        h5tools->writeGroupAttrInt(*_h5Group, "MFlag", _data->getMFlag());
        // SFlag 
        h5tools->writeGroupAttrInt(*_h5Group, "SFlag", _data->getSFlag());
        // Inststart 
        h5tools->writeGroupAttrDouble(*_h5Group, "Inststart", _data->getInststart());
        // FctIDel 
        h5tools->writeGroupAttrInt(*_h5Group, "FctIDel", _data->getFctIDel());
        // Elref 
        h5tools->writeGroupAttrDouble(*_h5Group, "Elref", _data->getElref());
        // R1
        h5tools->writeGroupAttrDouble(*_h5Group, "R1", _data->getR1());
        // R2 
        h5tools->writeGroupAttrDouble(*_h5Group, "R2", _data->getR2());
        // R4 
        h5tools->writeGroupAttrDouble(*_h5Group, "R4", _data->getR4());
        // R5 
        h5tools->writeGroupAttrDouble(*_h5Group, "R5", _data->getR5());

        QString type = Radioss::FITKFailureModelBIQUAD::GetFITKFailureModelBIQUADRadiossKeyWord();
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
