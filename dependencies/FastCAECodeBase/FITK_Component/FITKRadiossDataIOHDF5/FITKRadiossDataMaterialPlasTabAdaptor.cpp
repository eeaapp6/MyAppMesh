/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossDataMaterialPlasTabAdaptor.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
//RadiossData
#include "FITK_Component/FITKRadiossData/FITKMaterialPlasTab.h"

namespace Interface
{
    bool FITKRadiossDataMaterialPlasTabAdaptor::adaptR()
    {
        _plasTabData = dynamic_cast<Radioss::FITKMaterialPlasTab*>(_dataObj);
        if (_reader == nullptr || _plasTabData == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!readRadiossData(_plasTabData, *_h5Group)) return false;
        if (!readDataObject(_plasTabData, *_h5Group)) return false;

        int failID = -1;
        bool isFail = false;
        if (!h5tools->readGroupint(*_h5Group, "FailID", &failID)) return false;
        if (!h5tools->readGroupBool(*_h5Group, "FailState", &isFail)) return false;
        _plasTabData->setFailID(failID);
        _plasTabData->setFailState(isFail);

        Radioss::FITKMaterialPlasTabData& materialData = _plasTabData->getMaterialData();
        // 读取基础材料参数
        if (!h5tools->readGroupDouble(*_h5Group, "Density", &materialData.m_density)) return false;
        if (!h5tools->readGroupDouble(*_h5Group, "YoungsModulus", &materialData.m_youngsModulus)) return false;
        if (!h5tools->readGroupDouble(*_h5Group, "PoissonsRatio", &materialData.m_poissonsRatio)) return false;
        // 读取失效相关参数
        if (!h5tools->readGroupDouble(*_h5Group, "FailureStrain", &materialData.m_failureStrain)) return false;
        if (!h5tools->readGroupDouble(*_h5Group, "TensileFailureStrain", &materialData.m_tensileFailureStrain)) return false;
        if (!h5tools->readGroupDouble(*_h5Group, "MaxTensileFailureStrain", &materialData.m_maxTensileFailureStrain)) return false;
        // 读取函数数量和标志参数
        if (!h5tools->readGroupint(*_h5Group, "NumberOfFunctions", &materialData.m_numberOfFunctions)) return false;
        if (!h5tools->readGroupint(*_h5Group, "SmoothStrainRateOptFlag", &materialData.m_smoothStrainRateOptFlag)) return false;
        if (!h5tools->readGroupint(*_h5Group, "VPFlag", &materialData.m_vpFlag)) return false;
        // 读取硬化系数和截止频率
        if (!h5tools->readGroupDouble(*_h5Group, "HardeningCoefficient", &materialData.m_Chard)) return false;
        if (!h5tools->readGroupDouble(*_h5Group, "StrainRateSmoothCutoff", &materialData.m_Fcut)) return false;
        // 读取拉伸应变删除阈值
        if (!h5tools->readGroupDouble(*_h5Group, "TensileStrainForElementDeletion", &materialData.m_tensileStrainForElementDeletion)) return false;
        // 读取屈服函数相关参数
        if (!h5tools->readGroupint(*_h5Group, "FctIDp", &materialData.m_fctIDp)) return false;
        if (!h5tools->readGroupDouble(*_h5Group, "FscaleFactor", &materialData.m_fscaleFactor)) return false;
        // 读取杨氏模量函数相关参数
        if (!h5tools->readGroupint(*_h5Group, "FctIODE", &materialData.m_fctIODE)) return false;
        if (!h5tools->readGroupDouble(*_h5Group, "Eint", &materialData.m_Eint)) return false;
        if (!h5tools->readGroupDouble(*_h5Group, "Ce", &materialData.m_Ce)) return false;
        // 读取列表数据 - 函数ID列表
        if (materialData.m_numberOfFunctions > 0) {
            std::vector<int> fctIDVector;
            fctIDVector.resize(materialData.m_numberOfFunctions);
            if (!h5tools->readGroupAttrIntArray(*_h5Group, "FctID", fctIDVector.data(), 1, materialData.m_numberOfFunctions)) return false;
            materialData.m_fctID.clear();
            for (int id : fctIDVector) {
                materialData.m_fctID.append(id);
            }
            // 读取函数缩放因子列表
            std::vector<double> fctScaleVector;
            fctScaleVector.resize(materialData.m_numberOfFunctions);
            if (!h5tools->readGroupAttrDoubleArray(*_h5Group, "FctScale", fctScaleVector.data(), 1, materialData.m_numberOfFunctions)) return false;
            materialData.m_fctScale.clear();
            for (double scale : fctScaleVector) {
                materialData.m_fctScale.append(scale);
            }
            // 读取应变率列表
            std::vector<double> epsilonVector;
            epsilonVector.resize(materialData.m_numberOfFunctions);
            if (!h5tools->readGroupAttrDoubleArray(*_h5Group, "Epsilon", epsilonVector.data(), 1, materialData.m_numberOfFunctions)) return false;
            materialData.m_epsilon.clear();
            for (double eps : epsilonVector) {
                materialData.m_epsilon.append(eps);
            }
        }
        std::string describe{};
        if (!h5tools->readGroupStr(*_h5Group, "MaterialDescribe", describe)) return false;
        _plasTabData->setMaterialDescribe(QString::fromStdString(describe));
        return true;
    }

    bool FITKRadiossDataMaterialPlasTabAdaptor::adaptW()
    {
        _plasTabData = dynamic_cast<Radioss::FITKMaterialPlasTab*>(_dataObj);
        if (_writer == nullptr || _plasTabData == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!writeRadiossData(_plasTabData, *_h5Group))return false;
        if (!writeDataObject(_plasTabData, *_h5Group))return false;
        //获取数据
        Radioss::FITKMaterialPlasTabData materialData = _plasTabData->getMaterialDataReadOnly();

        // 写入基础材料参数
        h5tools->writeGroupAttrDouble(*_h5Group, "Density", materialData.m_density);
        h5tools->writeGroupAttrDouble(*_h5Group, "YoungsModulus", materialData.m_youngsModulus);
        h5tools->writeGroupAttrDouble(*_h5Group, "PoissonsRatio", materialData.m_poissonsRatio);

        // 写入失效相关参数
        h5tools->writeGroupAttrDouble(*_h5Group, "FailureStrain", materialData.m_failureStrain);
        h5tools->writeGroupAttrDouble(*_h5Group, "TensileFailureStrain", materialData.m_tensileFailureStrain);
        h5tools->writeGroupAttrDouble(*_h5Group, "MaxTensileFailureStrain", materialData.m_maxTensileFailureStrain);

        // 写入函数数量和标志参数
        h5tools->writeGroupAttrInt(*_h5Group, "NumberOfFunctions", materialData.m_numberOfFunctions);
        h5tools->writeGroupAttrInt(*_h5Group, "SmoothStrainRateOptFlag", materialData.m_smoothStrainRateOptFlag);
        h5tools->writeGroupAttrInt(*_h5Group, "VPFlag", materialData.m_vpFlag);

        // 写入硬化系数和截止频率
        h5tools->writeGroupAttrDouble(*_h5Group, "HardeningCoefficient", materialData.m_Chard);
        h5tools->writeGroupAttrDouble(*_h5Group, "StrainRateSmoothCutoff", materialData.m_Fcut);

        // 写入拉伸应变删除阈值
        h5tools->writeGroupAttrDouble(*_h5Group, "TensileStrainForElementDeletion", materialData.m_tensileStrainForElementDeletion);

        // 写入屈服函数相关参数
        h5tools->writeGroupAttrInt(*_h5Group, "FctIDp", materialData.m_fctIDp);
        h5tools->writeGroupAttrDouble(*_h5Group, "FscaleFactor", materialData.m_fscaleFactor);

        // 写入杨氏模量函数相关参数
        h5tools->writeGroupAttrInt(*_h5Group, "FctIODE", materialData.m_fctIODE);
        h5tools->writeGroupAttrDouble(*_h5Group, "Eint", materialData.m_Eint);
        h5tools->writeGroupAttrDouble(*_h5Group, "Ce", materialData.m_Ce);

        // 写入列表数据 - 函数ID列表
        if (materialData.m_numberOfFunctions > 0) {
            std::vector<int> fctIDVector;
            fctIDVector.reserve(materialData.m_fctID.size());
            for (int i = 0; i < materialData.m_fctID.size(); ++i) {
                fctIDVector.push_back(materialData.m_fctID[i]);
            }
            h5tools->writeGroupAttrIntArray(*_h5Group, "FctID", fctIDVector.data(), 1, materialData.m_fctID.size());

            // 写入函数缩放因子列表
            std::vector<double> fctScaleVector;
            fctScaleVector.reserve(materialData.m_fctScale.size());
            for (int i = 0; i < materialData.m_fctScale.size(); ++i) {
                fctScaleVector.push_back(materialData.m_fctScale[i]);
            }
            h5tools->writeGroupAttrDoubleArray(*_h5Group, "FctScale", fctScaleVector.data(), 1, materialData.m_fctScale.size());

            // 写入应变率列表
            std::vector<double> epsilonVector;
            epsilonVector.reserve(materialData.m_epsilon.size());
            for (int i = 0; i < materialData.m_epsilon.size(); ++i) {
                epsilonVector.push_back(materialData.m_epsilon[i]);
            }
            h5tools->writeGroupAttrDoubleArray(*_h5Group, "Epsilon", epsilonVector.data(), 1, materialData.m_epsilon.size());
        }
        QString type = Radioss::FITKMaterialPlasTab::GetFITKMaterialPlasTabRadiossKeyWord();
        h5tools->writeGroupAttrStr(*_h5Group, "MaterialType", type.toStdString());

        int materialID = _plasTabData->getMaterialID();
        QString describe = _plasTabData->getMaterialDescribe();
        h5tools->writeGroupAttrStr(*_h5Group, "MaterialDescribe", describe.toStdString());
        h5tools->writeGroupAttrInt(*_h5Group, "MaterialID", materialID);

        h5tools->writeGroupAttrInt(*_h5Group, "FailID", _plasTabData->getFailID());
        h5tools->writeGroupAttrBool(*_h5Group, "FailState", _plasTabData->isFailState());
        return true;
    }
}
