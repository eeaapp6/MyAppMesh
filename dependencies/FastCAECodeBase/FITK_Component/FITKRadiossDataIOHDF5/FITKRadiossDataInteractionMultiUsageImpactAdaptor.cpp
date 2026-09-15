/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossDataInteractionMultiUsageImpactAdaptor.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
//RadiossData
#include "FITK_Component/FITKRadiossData/FITKInteractionMultiUsageImpact.h"

namespace Interface
{
    bool FITKRadiossDataInteractionMultiUsageImpactAdaptor::adaptR()
    {
        _data = dynamic_cast<Radioss::FITKInteractionMultiUsageImpact*>(_dataObj);
        if (_reader == nullptr || _data == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!readRadiossData(_data, *_h5Group)) return false;
        if (!readDataObject(_data, *_h5Group)) return false;
        // 读取主表面和节点组ID
        int intValue = 0;
        if (!h5tools->readGroupint(*_h5Group, "SurfaceID", &intValue)) return false;
        _data->setMasterSurfaceID(intValue);
        if (!h5tools->readGroupint(*_h5Group, "NodeGroupID", &intValue)) return false;
        _data->setNodeGroupID(intValue);
        // 获取属性值对象
        Radioss::FITKInteractionMultiUsageImpactDefaultValue* propValue = _data->getValues();
        if (propValue == nullptr) return false;
        // 读取必须属性值参数（默认值参数）
        if (!h5tools->readGroupint(*_h5Group, "Istf", &intValue)) return false;
        propValue->_Istf = intValue;
        if (!h5tools->readGroupint(*_h5Group, "Igap", &intValue)) return false;
        propValue->_Igap = intValue;
        if (!h5tools->readGroupint(*_h5Group, "Ibag", &intValue)) return false;
        propValue->_Ibag = intValue;
        if (!h5tools->readGroupint(*_h5Group, "Idel", &intValue)) return false;
        propValue->_Idel = intValue;
        if (!h5tools->readGroupint(*_h5Group, "Irem_gap", &intValue)) return false;
        propValue->_Irem_gap = intValue;
        if (!h5tools->readGroupint(*_h5Group, "Irem_i2", &intValue)) return false;
        propValue->_Irem_i2 = intValue;
        if (!h5tools->readGroupint(*_h5Group, "Inacti", &intValue)) return false;
        propValue->_Inacti = intValue;
        if (!h5tools->readGroupint(*_h5Group, "Iform", &intValue)) return false;
        propValue->_Iform = intValue;
        // 读取相互作用基本参数（成员变量）
        bool boolValue = 0;
        if (!h5tools->readGroupBool(*_h5Group, "Ithe", &boolValue)) return false;
        _data->_Ithe = boolValue;
        if (!h5tools->readGroupint(*_h5Group, "Icurv", &intValue)) return false;
        _data->_Icurv = intValue;
        if (!h5tools->readGroupint(*_h5Group, "Iadm", &intValue)) return false;
        _data->_Iadm = intValue;
        if (!h5tools->readGroupint(*_h5Group, "Itied", &intValue)) return false;
        _data->_Itied = intValue;
        if (!h5tools->readGroupint(*_h5Group, "Ibc", &intValue)) return false;
        _data->_Ibc = intValue;
        if (!h5tools->readGroupint(*_h5Group, "Ifric", &intValue)) return false;
        _data->_Ifric = intValue;
        if (!h5tools->readGroupint(*_h5Group, "Ifiltr", &intValue)) return false;
        _data->_Ifiltr = intValue;
        if (!h5tools->readGroupint(*_h5Group, "sens_ID", &intValue)) return false;
        _data->_sens_ID = intValue;
        if (!h5tools->readGroupint(*_h5Group, "fct_IDf", &intValue)) return false;
        _data->_fct_IDf = intValue;
        if (!h5tools->readGroupint(*_h5Group, "fric_ID", &intValue)) return false;
        _data->_fric_ID = intValue;

        double doubleValue = 0.0;
        if (!h5tools->readGroupDouble(*_h5Group, "Fscale_gap", &doubleValue)) return false;
        _data->_Fscale_gap = doubleValue;
        if (!h5tools->readGroupDouble(*_h5Group, "GAPmax", &doubleValue)) return false;
        _data->_GAPmax = doubleValue;
        if (!h5tools->readGroupDouble(*_h5Group, "Fpenmax", &doubleValue)) return false;
        _data->_Fpenmax = doubleValue;
        if (!h5tools->readGroupDouble(*_h5Group, "STmin", &doubleValue)) return false;
        _data->_STmin = doubleValue;
        if (!h5tools->readGroupDouble(*_h5Group, "STmax", &doubleValue)) return false;
        _data->_STmax = doubleValue;
        if (!h5tools->readGroupDouble(*_h5Group, "mesh_size", &doubleValue)) return false;
        _data->_mesh_size = doubleValue;
        if (!h5tools->readGroupDouble(*_h5Group, "dtmin", &doubleValue)) return false;
        _data->_dtmin = doubleValue;
        if (!h5tools->readGroupDouble(*_h5Group, "Stfac", &doubleValue)) return false;
        _data->_Stfac = doubleValue;
        if (!h5tools->readGroupDouble(*_h5Group, "Fric", &doubleValue)) return false;
        _data->_Fric = doubleValue;
        if (!h5tools->readGroupDouble(*_h5Group, "Gapmin", &doubleValue)) return false;
        _data->_Gapmin = doubleValue;
        if (!h5tools->readGroupDouble(*_h5Group, "Tstart", &doubleValue)) return false;
        _data->_Tstart = doubleValue;
        if (!h5tools->readGroupDouble(*_h5Group, "Tstop", &doubleValue)) return false;
        _data->_Tstop = doubleValue;
        if (!h5tools->readGroupDouble(*_h5Group, "VisS", &doubleValue)) return false;
        _data->_VisS = doubleValue;
        if (!h5tools->readGroupDouble(*_h5Group, "VisF", &doubleValue)) return false;
        _data->_VisF = doubleValue;
        if (!h5tools->readGroupDouble(*_h5Group, "Bumult", &doubleValue)) return false;
        _data->_Bumult = doubleValue;
        if (!h5tools->readGroupDouble(*_h5Group, "Xfreq", &doubleValue)) return false;
        _data->_Xfreq = doubleValue;
        if (!h5tools->readGroupDouble(*_h5Group, "AscaleF", &doubleValue)) return false;
        _data->_AscaleF = doubleValue;
        // 读取可选数据参数
        Radioss::MultiUsageImpactOptionalData& optionalData = _data->getOptionalData();
        if (!h5tools->readGroupint(*_h5Group, "node_ID1", &intValue)) return false;
        optionalData.node_ID1 = intValue;
        if (!h5tools->readGroupint(*_h5Group, "node_ID2", &intValue)) return false;
        optionalData.node_ID2 = intValue;
        if (!h5tools->readGroupDouble(*_h5Group, "C1", &doubleValue)) return false;
        optionalData.C1 = doubleValue;
        if (!h5tools->readGroupDouble(*_h5Group, "C2", &doubleValue)) return false;
        optionalData.C2 = doubleValue;
        if (!h5tools->readGroupDouble(*_h5Group, "C3", &doubleValue)) return false;
        optionalData.C3 = doubleValue;
        if (!h5tools->readGroupDouble(*_h5Group, "C4", &doubleValue)) return false;
        optionalData.C4 = doubleValue;
        if (!h5tools->readGroupDouble(*_h5Group, "C5", &doubleValue)) return false;
        optionalData.C5 = doubleValue;
        if (!h5tools->readGroupDouble(*_h5Group, "C6", &doubleValue)) return false;
        optionalData.C6 = doubleValue;
        if (!h5tools->readGroupint(*_h5Group, "NRadm", &intValue)) return false;
        optionalData.NRadm = intValue;
        if (!h5tools->readGroupDouble(*_h5Group, "Padm", &doubleValue)) return false;
        optionalData.Padm = doubleValue;
        if (!h5tools->readGroupDouble(*_h5Group, "Angladm", &doubleValue)) return false;
        optionalData.Angladm = doubleValue;
        if (!h5tools->readGroupDouble(*_h5Group, "Kthe", &doubleValue)) return false;
        optionalData.Kthe = doubleValue;
        if (!h5tools->readGroupint(*_h5Group, "fct_IDK", &intValue)) return false;
        optionalData.fct_IDK = intValue;
        if (!h5tools->readGroupDouble(*_h5Group, "Tint", &doubleValue)) return false;
        optionalData.Tint = doubleValue;
        if (!h5tools->readGroupint(*_h5Group, "Ithe_form", &intValue)) return false;
        optionalData.Ithe_form = intValue;
        if (!h5tools->readGroupDouble(*_h5Group, "AscaleK", &doubleValue)) return false;
        optionalData.AscaleK = doubleValue;
        if (!h5tools->readGroupDouble(*_h5Group, "Frad", &doubleValue)) return false;
        optionalData.Frad = doubleValue;
        if (!h5tools->readGroupDouble(*_h5Group, "Drad", &doubleValue)) return false;
        optionalData.Drad = doubleValue;
        if (!h5tools->readGroupDouble(*_h5Group, "Fheats", &doubleValue)) return false;
        optionalData.Fheats = doubleValue;
        if (!h5tools->readGroupDouble(*_h5Group, "Fheatm", &doubleValue)) return false;
        optionalData.Fheatm = doubleValue;
        return true;
    }

    bool FITKRadiossDataInteractionMultiUsageImpactAdaptor::adaptW()
    {
        _data = dynamic_cast<Radioss::FITKInteractionMultiUsageImpact*>(_dataObj);
        if (_writer == nullptr || _data == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!writeRadiossData(_data, *_h5Group)) return false;
        if (!writeDataObject(_data, *_h5Group)) return false;
        // 写入主表面和节点组ID
        h5tools->writeGroupAttrInt(*_h5Group, "SurfaceID", _data->getMasterSurfaceID());
        h5tools->writeGroupAttrInt(*_h5Group, "NodeGroupID", _data->getNodeGroupID());
        // 获取属性值对象
        Radioss::FITKInteractionMultiUsageImpactDefaultValue* propValue = _data->getValues();
        if (propValue == nullptr) return false;
        // 写入必须属性值参数（默认值参数）
        h5tools->writeGroupAttrInt(*_h5Group, "Istf", propValue->_Istf);
        h5tools->writeGroupAttrInt(*_h5Group, "Igap", propValue->_Igap);
        h5tools->writeGroupAttrInt(*_h5Group, "Ibag", propValue->_Ibag);
        h5tools->writeGroupAttrInt(*_h5Group, "Idel", propValue->_Idel);
        h5tools->writeGroupAttrInt(*_h5Group, "Irem_gap", propValue->_Irem_gap);
        h5tools->writeGroupAttrInt(*_h5Group, "Irem_i2", propValue->_Irem_i2);
        h5tools->writeGroupAttrInt(*_h5Group, "Inacti", propValue->_Inacti);
        h5tools->writeGroupAttrInt(*_h5Group, "Iform", propValue->_Iform);
        // 写入相互作用基本参数（成员变量）
        h5tools->writeGroupAttrBool(*_h5Group, "Ithe", _data->_Ithe);
        h5tools->writeGroupAttrInt(*_h5Group, "Icurv", _data->_Icurv);
        h5tools->writeGroupAttrInt(*_h5Group, "Iadm", _data->_Iadm);
        h5tools->writeGroupAttrInt(*_h5Group, "Itied", _data->_Itied);
        h5tools->writeGroupAttrInt(*_h5Group, "Ibc", _data->_Ibc);
        h5tools->writeGroupAttrInt(*_h5Group, "Ifric", _data->_Ifric);
        h5tools->writeGroupAttrInt(*_h5Group, "Ifiltr", _data->_Ifiltr);
        h5tools->writeGroupAttrInt(*_h5Group, "sens_ID", _data->_sens_ID);
        h5tools->writeGroupAttrInt(*_h5Group, "fct_IDf", _data->_fct_IDf);
        h5tools->writeGroupAttrInt(*_h5Group, "fric_ID", _data->_fric_ID);
        h5tools->writeGroupAttrDouble(*_h5Group, "Fscale_gap", _data->_Fscale_gap);
        h5tools->writeGroupAttrDouble(*_h5Group, "GAPmax", _data->_GAPmax);
        h5tools->writeGroupAttrDouble(*_h5Group, "Fpenmax", _data->_Fpenmax);
        h5tools->writeGroupAttrDouble(*_h5Group, "STmin", _data->_STmin);
        h5tools->writeGroupAttrDouble(*_h5Group, "STmax", _data->_STmax);
        h5tools->writeGroupAttrDouble(*_h5Group, "mesh_size", _data->_mesh_size);
        h5tools->writeGroupAttrDouble(*_h5Group, "dtmin", _data->_dtmin);
        h5tools->writeGroupAttrDouble(*_h5Group, "Stfac", _data->_Stfac);
        h5tools->writeGroupAttrDouble(*_h5Group, "Fric", _data->_Fric);
        h5tools->writeGroupAttrDouble(*_h5Group, "Gapmin", _data->_Gapmin);
        h5tools->writeGroupAttrDouble(*_h5Group, "Tstart", _data->_Tstart);
        h5tools->writeGroupAttrDouble(*_h5Group, "Tstop", _data->_Tstop);
        h5tools->writeGroupAttrDouble(*_h5Group, "VisS", _data->_VisS);
        h5tools->writeGroupAttrDouble(*_h5Group, "VisF", _data->_VisF);
        h5tools->writeGroupAttrDouble(*_h5Group, "Bumult", _data->_Bumult);
        h5tools->writeGroupAttrDouble(*_h5Group, "Xfreq", _data->_Xfreq);
        h5tools->writeGroupAttrDouble(*_h5Group, "AscaleF", _data->_AscaleF);
        // 写入可选数据参数
        Radioss::MultiUsageImpactOptionalData& optionalData = _data->getOptionalData();
        h5tools->writeGroupAttrInt(*_h5Group, "node_ID1", optionalData.node_ID1);
        h5tools->writeGroupAttrInt(*_h5Group, "node_ID2", optionalData.node_ID2);

        h5tools->writeGroupAttrDouble(*_h5Group, "C1", optionalData.C1);
        h5tools->writeGroupAttrDouble(*_h5Group, "C2", optionalData.C2);
        h5tools->writeGroupAttrDouble(*_h5Group, "C3", optionalData.C3);
        h5tools->writeGroupAttrDouble(*_h5Group, "C4", optionalData.C4);
        h5tools->writeGroupAttrDouble(*_h5Group, "C5", optionalData.C5);
        h5tools->writeGroupAttrDouble(*_h5Group, "C6", optionalData.C6);

        h5tools->writeGroupAttrInt(*_h5Group, "NRadm", optionalData.NRadm);
        h5tools->writeGroupAttrDouble(*_h5Group, "Padm", optionalData.Padm);
        h5tools->writeGroupAttrDouble(*_h5Group, "Angladm", optionalData.Angladm);

        h5tools->writeGroupAttrDouble(*_h5Group, "Kthe", optionalData.Kthe);
        h5tools->writeGroupAttrInt(*_h5Group, "fct_IDK", optionalData.fct_IDK);
        h5tools->writeGroupAttrDouble(*_h5Group, "Tint", optionalData.Tint);
        h5tools->writeGroupAttrInt(*_h5Group, "Ithe_form", optionalData.Ithe_form);
        h5tools->writeGroupAttrDouble(*_h5Group, "AscaleK", optionalData.AscaleK);
        h5tools->writeGroupAttrDouble(*_h5Group, "Frad", optionalData.Frad);
        h5tools->writeGroupAttrDouble(*_h5Group, "Drad", optionalData.Drad);
        h5tools->writeGroupAttrDouble(*_h5Group, "Fheats", optionalData.Fheats);
        h5tools->writeGroupAttrDouble(*_h5Group, "Fheatm", optionalData.Fheatm);
        // 写入类型信息
        QString type = Radioss::FITKInteractionMultiUsageImpact::GetFITKInteractionMultiUsageImpactRadiossKeyWord();
        h5tools->writeGroupAttrStr(*_h5Group, "InteractionType", type.toStdString());
        return true;
    }
}
