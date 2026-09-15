/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossDataInteractionTiedAdaptor.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
//RadiossData
#include "FITK_Component/FITKRadiossData/FITKInteractionTied.h"

namespace Interface
{
    bool FITKRadiossDataInteractionTiedAdaptor::adaptR()
    {
        _tiedData = dynamic_cast<Radioss::FITKInteractionKinematicTied*>(_dataObj);
        if (_reader == nullptr || _tiedData == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!readRadiossData(_tiedData, *_h5Group)) return false;
        if (!readDataObject(_tiedData, *_h5Group)) return false;
        // 获取属性值对象
        Radioss::FITKInteractionKinematicTiedDefault* propValue = _tiedData->getValue();
        if (propValue == nullptr) return false;
        // 读取必须属性值参数
        int intValue = 0;
        if (!h5tools->readGroupint(*_h5Group, "IgnoreFlag", &intValue)) return false;
        propValue->setIgnoreFlag(intValue);
        if (!h5tools->readGroupint(*_h5Group, "SpotWeldFormulationFlag", &intValue)) return false;
        propValue->setSpotWeldFormulationFlag(intValue);
        if (!h5tools->readGroupint(*_h5Group, "SearchFormulationFlag", &intValue)) return false;
        propValue->setSearchFormulationFlag(intValue);
        if (!h5tools->readGroupint(*_h5Group, "NodeDeletionFlag", &intValue)) return false;
        propValue->setNodeDeletionFlag(intValue);
        if (!h5tools->readGroupint(*_h5Group, "IstfFlagDefault", &intValue)) return false;
        propValue->setIstfFlagDefault(intValue);
        // 读取可选数据参数
        Radioss::TiedOptionalData& optionalData = _tiedData->getOptionalData();
        if (!h5tools->readGroupint(*_h5Group, "Rupt", &intValue)) return false;
        optionalData.Rupt = intValue;
        if (!h5tools->readGroupint(*_h5Group, "Ifilter", &intValue)) return false;
        optionalData.Ifilter = intValue;
        if (!h5tools->readGroupint(*_h5Group, "fct_IDsr", &intValue)) return false;
        optionalData.fct_IDsr = intValue;
        if (!h5tools->readGroupint(*_h5Group, "fct_IDsn", &intValue)) return false;
        optionalData.fct_IDsn = intValue;
        if (!h5tools->readGroupint(*_h5Group, "fct_IDst", &intValue)) return false;
        optionalData.fct_IDst = intValue;
        if (!h5tools->readGroupint(*_h5Group, "Isym", &intValue)) return false;
        optionalData.Isym = intValue;
        if (!h5tools->readGroupint(*_h5Group, "Istf", &intValue)) return false;
        optionalData.Istf = intValue;

        if (!h5tools->readGroupint(*_h5Group, "SurfaceID", &intValue)) return false;
        _tiedData->setMasterSurfaceID(intValue);
        if (!h5tools->readGroupint(*_h5Group, "NodeGroupID", &intValue)) return false;
        _tiedData->setNodeGroupID(intValue);

        double doubleValue = 0.0;
        if (!h5tools->readGroupDouble(*_h5Group, "Max_N_Dist", &doubleValue)) return false;
        optionalData.Max_N_Dist = doubleValue;
        if (!h5tools->readGroupDouble(*_h5Group, "Max_T_Dist", &doubleValue)) return false;
        optionalData.Max_T_Dist = doubleValue;
        if (!h5tools->readGroupDouble(*_h5Group, "Fscale_stress", &doubleValue)) return false;
        optionalData.Fscale_stress = doubleValue;
        if (!h5tools->readGroupDouble(*_h5Group, "Fscale_str_rate", &doubleValue)) return false;
        optionalData.Fscale_str_rate = doubleValue;
        if (!h5tools->readGroupDouble(*_h5Group, "Fscale_dist", &doubleValue)) return false;
        optionalData.Fscale_dist = doubleValue;
        if (!h5tools->readGroupDouble(*_h5Group, "Alpha", &doubleValue)) return false;
        optionalData.Alpha = doubleValue;
        if (!h5tools->readGroupDouble(*_h5Group, "Area", &doubleValue)) return false;
        optionalData.Area = doubleValue;
        if (!h5tools->readGroupDouble(*_h5Group, "Stfac", &doubleValue)) return false;
        optionalData.Stfac = doubleValue;
        if (!h5tools->readGroupDouble(*_h5Group, "Visc", &doubleValue)) return false;
        optionalData.Visc = doubleValue;
        // 读取相互作用基本参数
        if (!h5tools->readGroupint(*_h5Group, "Level", &intValue)) return false;
        _tiedData->setLevel(intValue);
        if (!h5tools->readGroupDouble(*_h5Group, "SearchDistance", &doubleValue)) return false;
        _tiedData->setSearchDistance(doubleValue);
        return true;
    }

    bool FITKRadiossDataInteractionTiedAdaptor::adaptW()
    {
        _tiedData = dynamic_cast<Radioss::FITKInteractionKinematicTied*>(_dataObj);
        if (_writer == nullptr || _tiedData == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!writeRadiossData(_tiedData, *_h5Group))return false;
        if (!writeDataObject(_tiedData, *_h5Group))return false;
       // 获取属性值对象
        Radioss::FITKInteractionKinematicTiedDefault* propValue = _tiedData->getValue();
        if (propValue == nullptr) return false;
        // 写入必须属性值参数
        h5tools->writeGroupAttrInt(*_h5Group, "IgnoreFlag", propValue->getIgnoreFlag());
        h5tools->writeGroupAttrInt(*_h5Group, "SpotWeldFormulationFlag", propValue->getSpotWeldFormulationFlag());
        h5tools->writeGroupAttrInt(*_h5Group, "SearchFormulationFlag", propValue->getSearchFormulationFlag());
        h5tools->writeGroupAttrInt(*_h5Group, "NodeDeletionFlag", propValue->getNodeDeletionFlag());
        h5tools->writeGroupAttrInt(*_h5Group, "IstfFlagDefault", propValue->getIstfFlagDefault());
        h5tools->writeGroupAttrInt(*_h5Group, "SurfaceID", _tiedData->getMasterSurfaceID());
        h5tools->writeGroupAttrInt(*_h5Group, "NodeGroupID", _tiedData->getNodeGroupID());
        // 写入可选数据参数
        Radioss::TiedOptionalData& optionalData = _tiedData->getOptionalData();

        h5tools->writeGroupAttrInt(*_h5Group, "Rupt", optionalData.Rupt);
        h5tools->writeGroupAttrInt(*_h5Group, "Ifilter", optionalData.Ifilter);
        h5tools->writeGroupAttrInt(*_h5Group, "fct_IDsr", optionalData.fct_IDsr);
        h5tools->writeGroupAttrInt(*_h5Group, "fct_IDsn", optionalData.fct_IDsn);
        h5tools->writeGroupAttrInt(*_h5Group, "fct_IDst", optionalData.fct_IDst);
        h5tools->writeGroupAttrInt(*_h5Group, "Isym", optionalData.Isym);
        h5tools->writeGroupAttrInt(*_h5Group, "Istf", optionalData.Istf);

        h5tools->writeGroupAttrDouble(*_h5Group, "Max_N_Dist", optionalData.Max_N_Dist);
        h5tools->writeGroupAttrDouble(*_h5Group, "Max_T_Dist", optionalData.Max_T_Dist);
        h5tools->writeGroupAttrDouble(*_h5Group, "Fscale_stress", optionalData.Fscale_stress);
        h5tools->writeGroupAttrDouble(*_h5Group, "Fscale_str_rate", optionalData.Fscale_str_rate);
        h5tools->writeGroupAttrDouble(*_h5Group, "Fscale_dist", optionalData.Fscale_dist);
        h5tools->writeGroupAttrDouble(*_h5Group, "Alpha", optionalData.Alpha);
        h5tools->writeGroupAttrDouble(*_h5Group, "Area", optionalData.Area);
        h5tools->writeGroupAttrDouble(*_h5Group, "Stfac", optionalData.Stfac);
        h5tools->writeGroupAttrDouble(*_h5Group, "Visc", optionalData.Visc);
        // 写入相互作用基本参数
        h5tools->writeGroupAttrInt(*_h5Group, "Level", _tiedData->getLevel());
        h5tools->writeGroupAttrDouble(*_h5Group, "SearchDistance", _tiedData->getSearchDistance());
        // 写入类型信息
        QString type = Radioss::FITKInteractionKinematicTied::GetFITKInteractionKinematicTiedRadiossKeyWord();
        h5tools->writeGroupAttrStr(*_h5Group, "InteractionType", type.toStdString());
        return true;
    }
}
