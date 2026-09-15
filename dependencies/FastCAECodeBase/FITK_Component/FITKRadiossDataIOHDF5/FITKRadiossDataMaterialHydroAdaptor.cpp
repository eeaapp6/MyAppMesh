/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossDataMaterialHydroAdaptor.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
//RadiossData
#include "FITK_Component/FITKRadiossData/FITKMaterialHydro.h"

namespace Interface
{
    bool FITKRadiossDataMaterialHydroAdaptor::adaptR()
    {
        _data = dynamic_cast<Radioss::FITKMaterialHydro*>(_dataObj);
        if (_reader == nullptr || _data == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!readRadiossData(_data, *_h5Group)) return false;
        if (!readDataObject(_data, *_h5Group)) return false;

        int failID = -1;
        bool isFail = false;
        if (!h5tools->readGroupint(*_h5Group, "FailID", &failID)) return false;
        if (!h5tools->readGroupBool(*_h5Group, "FailState", &isFail)) return false;
        _data->setFailID(failID);
        _data->setFailState(isFail);

        Radioss::FITKMaterialHydroData& materialData = _data->getMaterialData();
        // 读取流体材料参数
        int eosID = -1;
        if (!h5tools->readGroupint(*_h5Group, "EOSID", &eosID)) return false;
        _data->setEOSID(eosID);
        if (!h5tools->readGroupDouble(*_h5Group, "Density", &materialData.m_density)) return false;
        if (!h5tools->readGroupDouble(*_h5Group, "EOS", &materialData.m_R_density)) return false;
        if (!h5tools->readGroupDouble(*_h5Group, "KinematicViscosity", &materialData.m_kinematicViscosity)) return false;
        if (!h5tools->readGroupDouble(*_h5Group, "PressureCut", &materialData.m_pressureCut)) return false;
        // 读取材料描述
        std::string describe{};
        if (!h5tools->readGroupStr(*_h5Group, "MaterialDescribe", describe)) return false;
        _data->setMaterialDescribe(QString::fromStdString(describe));
        return true;
    }

    bool FITKRadiossDataMaterialHydroAdaptor::adaptW()
    {
        _data = dynamic_cast<Radioss::FITKMaterialHydro*>(_dataObj);
        if (_writer == nullptr || _data == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!writeRadiossData(_data, *_h5Group)) return false;
        if (!writeDataObject(_data, *_h5Group)) return false;
        // 获取数据
        const Radioss::FITKMaterialHydroData& materialData = _data->getMaterialDataReadOnly();
        // 写入流体材料参数
        h5tools->writeGroupAttrInt(*_h5Group, "EOSID", _data->getEOSID());
        h5tools->writeGroupAttrDouble(*_h5Group, "Density", materialData.m_density);
        h5tools->writeGroupAttrDouble(*_h5Group, "EOS", materialData.m_R_density);
        h5tools->writeGroupAttrDouble(*_h5Group, "KinematicViscosity", materialData.m_kinematicViscosity);
        h5tools->writeGroupAttrDouble(*_h5Group, "PressureCut", materialData.m_pressureCut);
        // 写入材料类型、ID与描述
        QString type = Radioss::FITKMaterialHydro::GetFITKMaterialHydroRadiossKeyWord();
        h5tools->writeGroupAttrStr(*_h5Group, "MaterialType", type.toStdString());
        int materialID = _data->getMaterialID();
        QString describe = _data->getMaterialDescribe();
        h5tools->writeGroupAttrStr(*_h5Group, "MaterialDescribe", describe.toStdString());
        h5tools->writeGroupAttrInt(*_h5Group, "MaterialID", materialID);

        h5tools->writeGroupAttrInt(*_h5Group, "FailID", _data->getFailID());
        h5tools->writeGroupAttrBool(*_h5Group, "FailState", _data->isFailState());

        return true;
    }
}
