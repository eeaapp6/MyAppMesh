/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

//#include "FITKRadiossDataMaterialFailBiquadAdaptor.h"
//#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
//#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
//#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
//#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
////RadiossData
//#include "FITK_Component/FITKRadiossData/FITKMaterialFailBiquad.h"
//
//namespace Interface
//{
//    bool FITKRadiossDataMaterialFailBiquadAdaptor::adaptR()
//    {
//        _data = dynamic_cast<Radioss::FITKMaterialFailBiquad*>(_dataObj);
//        if (_reader == nullptr || _data == nullptr || _h5Group == nullptr) return false;
//        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
//        if (h5tools == nullptr) return false;
//        if (!readRadiossData(_data, *_h5Group)) return false;
//        if (!readDataObject(_data, *_h5Group)) return false;
//
//        Radioss::FITKMaterialFailBiquadData& materialData = _data->getMaterialData();
//        // 读取BIQUAD失效材料参数
//        if (!h5tools->readGroupDouble(*_h5Group, "C1", &materialData.m_c1)) return false;
//        if (!h5tools->readGroupDouble(*_h5Group, "C2", &materialData.m_c2)) return false;
//        if (!h5tools->readGroupDouble(*_h5Group, "C3", &materialData.m_c3)) return false;
//        if (!h5tools->readGroupDouble(*_h5Group, "C4", &materialData.m_c4)) return false;
//        if (!h5tools->readGroupDouble(*_h5Group, "C5", &materialData.m_c5)) return false;
//        if (!h5tools->readGroupDouble(*_h5Group, "PThickFail", &materialData.m_pThickFail)) return false;
//        if (!h5tools->readGroupint(*_h5Group, "MFlag", &materialData.m_mFlag)) return false;
//
//        // 读取材料描述
//        std::string describe{};
//        if (!h5tools->readGroupStr(*_h5Group, "MaterialDescribe", describe)) return false;
//        _data->setMaterialDescribe(QString::fromStdString(describe));
//        return true;
//    }
//
//    bool FITKRadiossDataMaterialFailBiquadAdaptor::adaptW()
//    {
//        _data = dynamic_cast<Radioss::FITKMaterialFailBiquad*>(_dataObj);
//        if (_writer == nullptr || _data == nullptr || _h5Group == nullptr) return false;
//        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
//        if (h5tools == nullptr) return false;
//        if (!writeRadiossData(_data, *_h5Group)) return false;
//        if (!writeDataObject(_data, *_h5Group)) return false;
//
//        // 获取数据
//        const Radioss::FITKMaterialFailBiquadData& materialData = _data->getMaterialDataReadOnly();
//        // 写入BIQUAD失效材料参数
//        h5tools->writeGroupAttrDouble(*_h5Group, "C1", materialData.m_c1);
//        h5tools->writeGroupAttrDouble(*_h5Group, "C2", materialData.m_c2);
//        h5tools->writeGroupAttrDouble(*_h5Group, "C3", materialData.m_c3);
//        h5tools->writeGroupAttrDouble(*_h5Group, "C4", materialData.m_c4);
//        h5tools->writeGroupAttrDouble(*_h5Group, "C5", materialData.m_c5);
//        h5tools->writeGroupAttrDouble(*_h5Group, "PThickFail", materialData.m_pThickFail);
//        h5tools->writeGroupAttrInt(*_h5Group, "MFlag", materialData.m_mFlag);
//
//        // 写入材料类型、ID与描述
//        QString type = Radioss::FITKMaterialFailBiquad::GetFITKMaterialFailBiquadRadiossKeyWord();
//        h5tools->writeGroupAttrStr(*_h5Group, "MaterialType", type.toStdString());
//        int materialID = _data->getMaterialID();
//        QString describe = _data->getMaterialDescribe();
//        h5tools->writeGroupAttrStr(*_h5Group, "MaterialDescribe", describe.toStdString());
//        h5tools->writeGroupAttrInt(*_h5Group, "MaterialID", materialID);
//        return true;
//    }
//}