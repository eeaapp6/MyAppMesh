/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossDataMaterialPlasBritAdaptor.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
//RadiossData
#include "FITK_Component/FITKRadiossData/FITKMaterialPlasBrit.h"

namespace Interface
{
    bool FITKRadiossDataMaterialPlasBritAdaptor::adaptR()
    {
        _data = dynamic_cast<Radioss::FITKMaterialPlasBrit*>(_dataObj);
        if (_reader == nullptr || _data == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (!readRadiossData(_data, *_h5Group)) return false;
        if (!readDataObject(_data, *_h5Group)) return false;
        if (h5tools == nullptr) return false;

        int failID = -1;
        bool isFail = false;
        if (!h5tools->readGroupint(*_h5Group, "FailID", &failID)) return false;
        if (!h5tools->readGroupBool(*_h5Group, "FailState", &isFail)) return false;
        _data->setFailID(failID);
        _data->setFailState(isFail);

        Radioss::FITKMaterialPlasBritData& materialData = _data->getMaterialData();
        // 读取基础材料参数
        if (!h5tools->readGroupDouble(*_h5Group, "Density", &materialData.m_density)) return false;
        if (!h5tools->readGroupDouble(*_h5Group, "YoungsModulus", &materialData.m_youngsModulus)) return false;
        if (!h5tools->readGroupDouble(*_h5Group, "PoissonsRatio", &materialData.m_poissonsRatio)) return false;
        // 读取塑性相关参数
        if (!h5tools->readGroupDouble(*_h5Group, "PlasticityYieldStress", &materialData.m_plasticityYieldStress)) return false;
        if (!h5tools->readGroupDouble(*_h5Group, "PlasticityHardeningParameter", &materialData.m_plasticityHardeningParameter)) return false;
        if (!h5tools->readGroupDouble(*_h5Group, "PlasticityHardeningExponent", &materialData.m_plasticityHardeningExponent)) return false;
        if (!h5tools->readGroupDouble(*_h5Group, "PlasticityMaximumStress", &materialData.m_plasticityMaximumStress)) return false;
        // 读取应变率相关参数
        if (!h5tools->readGroupDouble(*_h5Group, "StrainRateCoefficient", &materialData.m_strainRateCoefficient)) return false;
        if (!h5tools->readGroupDouble(*_h5Group, "ReferenceStrainRate", &materialData.m_referenceStrainRate)) return false;
        if (!h5tools->readGroupint(*_h5Group, "StrainRateComputation", &materialData.m_strainRateComputation)) return false;
        if (!h5tools->readGroupint(*_h5Group, "StrainRateSmoothing", &materialData.m_strainRateSmoothing)) return false;
        if (!h5tools->readGroupDouble(*_h5Group, "CutoffFreq", &materialData.m_cutoffFreq)) return false;
        // 读取方向1拉伸失效相关参数
        if (!h5tools->readGroupDouble(*_h5Group, "TensileFilureStrain1", &materialData.m_tensileFilureStrain1)) return false;
        if (!h5tools->readGroupDouble(*_h5Group, "MaximumTensileFilureStrain1", &materialData.m_maximumTensileFilureStrain1)) return false;
        if (!h5tools->readGroupDouble(*_h5Group, "MaximumDamageFactor1", &materialData.m_maximumDamageFactor1)) return false;
        if (!h5tools->readGroupDouble(*_h5Group, "MaximumTensileStrain1", &materialData.m_maximumTensileStrain1)) return false;
        // 读取方向2拉伸失效相关参数
        if (!h5tools->readGroupDouble(*_h5Group, "TensileFilureStrain2", &materialData.m_tensileFilureStrain2)) return false;
        if (!h5tools->readGroupDouble(*_h5Group, "MaximumTensileFilureStrain2", &materialData.m_maximumTensileFilureStrain2)) return false;
        if (!h5tools->readGroupDouble(*_h5Group, "MaximumDamageFactor2", &materialData.m_maximumDamageFactor2)) return false;
        if (!h5tools->readGroupDouble(*_h5Group, "MaximumTensileStrain2", &materialData.m_maximumTensileStrain2)) return false;
        // 读取材料描述与基础数据
        std::string describe{};
        if (!h5tools->readGroupStr(*_h5Group, "MaterialDescribe", describe)) return false;
        _data->setMaterialDescribe(QString::fromStdString(describe));
        return true;
    }

    bool FITKRadiossDataMaterialPlasBritAdaptor::adaptW()
    {
        _data = dynamic_cast<Radioss::FITKMaterialPlasBrit*>(_dataObj);
        if (_writer == nullptr || _data == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!writeRadiossData(_data, *_h5Group)) return false;
        if (!writeDataObject(_data, *_h5Group)) return false;
        // 获取数据
        Radioss::FITKMaterialPlasBritData materialData = _data->getMaterialDataReadOnly();
        // 写入基础材料参数
        h5tools->writeGroupAttrDouble(*_h5Group, "Density", materialData.m_density);
        h5tools->writeGroupAttrDouble(*_h5Group, "YoungsModulus", materialData.m_youngsModulus);
        h5tools->writeGroupAttrDouble(*_h5Group, "PoissonsRatio", materialData.m_poissonsRatio);
        // 写入塑性相关参数
        h5tools->writeGroupAttrDouble(*_h5Group, "PlasticityYieldStress", materialData.m_plasticityYieldStress);
        h5tools->writeGroupAttrDouble(*_h5Group, "PlasticityHardeningParameter", materialData.m_plasticityHardeningParameter);
        h5tools->writeGroupAttrDouble(*_h5Group, "PlasticityHardeningExponent", materialData.m_plasticityHardeningExponent);
        h5tools->writeGroupAttrDouble(*_h5Group, "PlasticityMaximumStress", materialData.m_plasticityMaximumStress);
        // 写入应变率相关参数
        h5tools->writeGroupAttrDouble(*_h5Group, "StrainRateCoefficient", materialData.m_strainRateCoefficient);
        h5tools->writeGroupAttrDouble(*_h5Group, "ReferenceStrainRate", materialData.m_referenceStrainRate);
        h5tools->writeGroupAttrInt(*_h5Group, "StrainRateComputation", materialData.m_strainRateComputation);
        h5tools->writeGroupAttrInt(*_h5Group, "StrainRateSmoothing", materialData.m_strainRateSmoothing);
        h5tools->writeGroupAttrDouble(*_h5Group, "CutoffFreq", materialData.m_cutoffFreq);
        // 写入方向1拉伸失效相关参数
        h5tools->writeGroupAttrDouble(*_h5Group, "TensileFilureStrain1", materialData.m_tensileFilureStrain1);
        h5tools->writeGroupAttrDouble(*_h5Group, "MaximumTensileFilureStrain1", materialData.m_maximumTensileFilureStrain1);
        h5tools->writeGroupAttrDouble(*_h5Group, "MaximumDamageFactor1", materialData.m_maximumDamageFactor1);
        h5tools->writeGroupAttrDouble(*_h5Group, "MaximumTensileStrain1", materialData.m_maximumTensileStrain1);
        // 写入方向2拉伸失效相关参数
        h5tools->writeGroupAttrDouble(*_h5Group, "TensileFilureStrain2", materialData.m_tensileFilureStrain2);
        h5tools->writeGroupAttrDouble(*_h5Group, "MaximumTensileFilureStrain2", materialData.m_maximumTensileFilureStrain2);
        h5tools->writeGroupAttrDouble(*_h5Group, "MaximumDamageFactor2", materialData.m_maximumDamageFactor2);
        h5tools->writeGroupAttrDouble(*_h5Group, "MaximumTensileStrain2", materialData.m_maximumTensileStrain2);
        // 写入材料类型、ID与描述
        QString type = Radioss::FITKMaterialPlasBrit::GetFITKMaterialPlasBritRadiossKeyWord();
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
