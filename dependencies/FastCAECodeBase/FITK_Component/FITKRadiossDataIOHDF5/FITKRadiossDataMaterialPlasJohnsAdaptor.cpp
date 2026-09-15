/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossDataMaterialPlasJohnsAdaptor.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
//RadiossData
#include "FITK_Component/FITKRadiossData/FITKMaterialPlasJohns.h"

namespace Interface
{
    bool FITKRadiossDataMaterialPlasJohnsAdaptor::adaptR()
    {
        _plasJohnsData = dynamic_cast<Radioss::FITKMaterialPlasJohns*>(_dataObj);
        if (_reader == nullptr || _plasJohnsData == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!readRadiossData(_plasJohnsData, *_h5Group)) return false;
        if (!readDataObject(_plasJohnsData, *_h5Group)) return false;

        int failID = -1;
        bool isFail = false;
        if (!h5tools->readGroupint(*_h5Group, "FailID", &failID)) return false;
        if (!h5tools->readGroupBool(*_h5Group, "FailState", &isFail)) return false;
        _plasJohnsData->setFailID(failID);
        _plasJohnsData->setFailState(isFail);

        Radioss::FITKMaterialPlasJohnsData& materialData = _plasJohnsData->getMaterialData();
        // 读取基础材料参数
        if (!h5tools->readGroupDouble(*_h5Group, "Density", &materialData.m_density)) return false;
        if (!h5tools->readGroupDouble(*_h5Group, "YoungsModulus", &materialData.m_youngsModulus)) return false;
        if (!h5tools->readGroupDouble(*_h5Group, "PoissonsRatio", &materialData.m_poissonsRatio)) return false;
        // 读取Johnson-Cook模型特有参数
        if (!h5tools->readGroupDouble(*_h5Group, "YieldStress", &materialData.m_yieldStress)) return false;
        if (!h5tools->readGroupDouble(*_h5Group, "PlasticHardening", &materialData.m_plasticHardening)) return false;
        if (!h5tools->readGroupDouble(*_h5Group, "PlasticHardeningExponent", &materialData.m_plasticHardeningExponent)) return false;
        // 读取失效相关参数
        if (!h5tools->readGroupDouble(*_h5Group, "FailureStrain", &materialData.m_failureStrain)) return false;
        if (!h5tools->readGroupDouble(*_h5Group, "MaxStress", &materialData.m_maxStress)) return false;
        if (!h5tools->readGroupDouble(*_h5Group, "UTS", &materialData.m_UTS)) return false;
        if (!h5tools->readGroupDouble(*_h5Group, "EpsilonUTS", &materialData.m_EpsilonUTS)) return false;
        // 读取应变率相关参数
        if (!h5tools->readGroupDouble(*_h5Group, "StrainRateCoefficient", &materialData.m_c)) return false;
        if (!h5tools->readGroupDouble(*_h5Group, "ReferenceStrainRate", &materialData.m_Epsilon0)) return false;
        if (!h5tools->readGroupint(*_h5Group, "StrainRateFlag", &materialData.m_ICC)) return false;
        if (!h5tools->readGroupint(*_h5Group, "StrainRateSmoothFlag", &materialData.m_Fsmooth)) return false;
        if (!h5tools->readGroupDouble(*_h5Group, "StrainRateSmoothCutoff", &materialData.m_Fcut)) return false;
        // 读取温度相关参数
        if (!h5tools->readGroupDouble(*_h5Group, "HardeningCoefficient", &materialData.m_Chard)) return false;
        if (!h5tools->readGroupDouble(*_h5Group, "TemperatureExponent", &materialData.m_tempExponent)) return false;
        if (!h5tools->readGroupDouble(*_h5Group, "MeltingTemperature", &materialData.m_meltingTemperature)) return false;
        if (!h5tools->readGroupDouble(*_h5Group, "SpecificHeat", &materialData.m_rhoCp)) return false;
        if (!h5tools->readGroupDouble(*_h5Group, "ReferenceTemperature", &materialData.m_Tref)) return false;
        // 读取材料类型
        std::string typeStr = {};
        if (!h5tools->readGroupStr(*_h5Group, "PlasJohnsType", typeStr)) return false;
        Core::FITKEnumTransfer<Radioss::FITKMaterialPlasJohns::MaterialPlasJohnsType> fitkEleTypeTrafer;
        bool isValid = false;
        Radioss::FITKMaterialPlasJohns::MaterialPlasJohnsType type = fitkEleTypeTrafer.fromString(QString::fromStdString(typeStr), isValid);
        if (!isValid) return false;
        _plasJohnsData->setType(type);
        std::string describe{};
        if (!h5tools->readGroupStr(*_h5Group, "MaterialDescribe", describe)) return false;
        _plasJohnsData->setMaterialDescribe(QString::fromStdString(describe));
        return true;
    }

    bool FITKRadiossDataMaterialPlasJohnsAdaptor::adaptW()
    {
        _plasJohnsData = dynamic_cast<Radioss::FITKMaterialPlasJohns*>(_dataObj);
        if (_writer == nullptr || _plasJohnsData == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!writeRadiossData(_plasJohnsData, *_h5Group))return false;
        if (!writeDataObject(_plasJohnsData, *_h5Group))return false;
        //获取数据
        Radioss::FITKMaterialPlasJohnsData materialData = _plasJohnsData->getMaterialDataReadOnly();

        // 写入基础材料参数
        h5tools->writeGroupAttrDouble(*_h5Group, "Density", materialData.m_density);
        h5tools->writeGroupAttrDouble(*_h5Group, "YoungsModulus", materialData.m_youngsModulus);
        h5tools->writeGroupAttrDouble(*_h5Group, "PoissonsRatio", materialData.m_poissonsRatio);

        // 写入Johnson-Cook模型特有参数
        h5tools->writeGroupAttrDouble(*_h5Group, "YieldStress", materialData.m_yieldStress);
        h5tools->writeGroupAttrDouble(*_h5Group, "PlasticHardening", materialData.m_plasticHardening);
        h5tools->writeGroupAttrDouble(*_h5Group, "PlasticHardeningExponent", materialData.m_plasticHardeningExponent);

        // 写入失效相关参数
        h5tools->writeGroupAttrDouble(*_h5Group, "FailureStrain", materialData.m_failureStrain);
        h5tools->writeGroupAttrDouble(*_h5Group, "MaxStress", materialData.m_maxStress);
        h5tools->writeGroupAttrDouble(*_h5Group, "UTS", materialData.m_UTS);
        h5tools->writeGroupAttrDouble(*_h5Group, "EpsilonUTS", materialData.m_EpsilonUTS);

        // 写入应变率相关参数
        h5tools->writeGroupAttrDouble(*_h5Group, "StrainRateCoefficient", materialData.m_c);
        h5tools->writeGroupAttrDouble(*_h5Group, "ReferenceStrainRate", materialData.m_Epsilon0);
        h5tools->writeGroupAttrInt(*_h5Group, "StrainRateFlag", materialData.m_ICC);
        h5tools->writeGroupAttrInt(*_h5Group, "StrainRateSmoothFlag", materialData.m_Fsmooth);
        h5tools->writeGroupAttrDouble(*_h5Group, "StrainRateSmoothCutoff", materialData.m_Fcut);

        // 写入温度相关参数
        h5tools->writeGroupAttrDouble(*_h5Group, "HardeningCoefficient", materialData.m_Chard);
        h5tools->writeGroupAttrDouble(*_h5Group, "TemperatureExponent", materialData.m_tempExponent);
        h5tools->writeGroupAttrDouble(*_h5Group, "MeltingTemperature", materialData.m_meltingTemperature);
        h5tools->writeGroupAttrDouble(*_h5Group, "SpecificHeat", materialData.m_rhoCp);
        h5tools->writeGroupAttrDouble(*_h5Group, "ReferenceTemperature", materialData.m_Tref);

        // 写入材料类型信息
        Radioss::FITKMaterialPlasJohns::MaterialPlasJohnsType plasJohnsType = _plasJohnsData->getType();
        Core::FITKEnumTransfer<Radioss::FITKMaterialPlasJohns::MaterialPlasJohnsType> fitkEleTypeTrafer;
        bool isValid = false;
        QString typeStr = fitkEleTypeTrafer.toString(plasJohnsType, isValid);
        if (!isValid) return false;
        h5tools->writeGroupAttrStr(*_h5Group, "PlasJohnsType", typeStr.toStdString());

        QString type = Radioss::FITKMaterialPlasJohns::GetFITKMaterialPlasJohnsRadiossKeyWord();
        h5tools->writeGroupAttrStr(*_h5Group, "MaterialType", type.toStdString());

        int materialID = _plasJohnsData->getMaterialID();
        QString describe = _plasJohnsData->getMaterialDescribe();
        h5tools->writeGroupAttrStr(*_h5Group, "MaterialDescribe", describe.toStdString());
        h5tools->writeGroupAttrInt(*_h5Group, "MaterialID", materialID);

        h5tools->writeGroupAttrInt(*_h5Group, "FailID", _plasJohnsData->getFailID());
        h5tools->writeGroupAttrBool(*_h5Group, "FailState", _plasJohnsData->isFailState());
        return true;
    }
}
