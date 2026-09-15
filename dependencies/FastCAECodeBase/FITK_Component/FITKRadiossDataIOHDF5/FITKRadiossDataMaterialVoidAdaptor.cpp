/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossDataMaterialVoidAdaptor.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
//RadiossData
#include "FITK_Component/FITKRadiossData/FITKMaterialElastic.h"

namespace Interface
{
    bool FITKRadiossDataMaterialVoidAdaptor::adaptR()
    {
        _voidData = dynamic_cast<Radioss::FITKMaterialVoid*>(_dataObj);
        if (_reader == nullptr || _voidData == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!readRadiossData(_voidData, *_h5Group)) return false;
        if (!readDataObject(_voidData, *_h5Group)) return false;
        double density = -1;
        double youngsModulus = -1;
        double poissonsRatio = -1;
        std::string describe{};

        int failID = -1;
        bool isFail = false;
        if (!h5tools->readGroupint(*_h5Group, "FailID", &failID)) return false;
        if (!h5tools->readGroupBool(*_h5Group, "FailState", &isFail)) return false;
        _voidData->setFailID(failID);
        _voidData->setFailState(isFail);

        if (!h5tools->readGroupDouble(*_h5Group, "Density", &density)) return false;
        if (!h5tools->readGroupDouble(*_h5Group, "YoungsModulus", &youngsModulus)) return false;
        if (!h5tools->readGroupDouble(*_h5Group, "PoissonsRatio", &poissonsRatio)) return false;
        if (!h5tools->readGroupStr(*_h5Group, "MaterialDescribe", describe)) return false;
        _voidData->setMaterialDescribe(QString::fromStdString(describe));
        _voidData->setDensity(density);
        _voidData->setYoungsModulus(youngsModulus);
        _voidData->setPoissonsRatio(poissonsRatio);
        return true;
    }

    bool FITKRadiossDataMaterialVoidAdaptor::adaptW()
    {
        _voidData = dynamic_cast<Radioss::FITKMaterialVoid*>(_dataObj);
        if (_writer == nullptr || _voidData == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!writeRadiossData(_voidData, *_h5Group))return false;
        if (!writeDataObject(_voidData, *_h5Group))return false;
        //获取数据
        double density = _voidData->getDensity();
        double youngsModulus = _voidData->getYoungsModulus();
        double poissonsRatio = _voidData->getPoissonsRatio();
        QString type = Radioss::FITKMaterialVoid::GetFITKMaterialVoidRadiossKeyWord();

        h5tools->writeGroupAttrDouble(*_h5Group, "Density", density);
        h5tools->writeGroupAttrDouble(*_h5Group, "YoungsModulus", youngsModulus);
        h5tools->writeGroupAttrDouble(*_h5Group, "PoissonsRatio", poissonsRatio);
        h5tools->writeGroupAttrStr(*_h5Group, "MaterialType", type.toStdString());

        int materialID = _voidData->getMaterialID();
        QString describe = _voidData->getMaterialDescribe();
        h5tools->writeGroupAttrStr(*_h5Group, "MaterialDescribe", describe.toStdString());
        h5tools->writeGroupAttrInt(*_h5Group, "MaterialID", materialID);

        h5tools->writeGroupAttrInt(*_h5Group, "FailID", _voidData->getFailID());
        h5tools->writeGroupAttrBool(*_h5Group, "FailState", _voidData->isFailState());
        return true;
    }
}
