/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossDataPropBeamSPRAdaptor.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
//RadiossData
#include "FITK_Component/FITKRadiossData/FITKPropBeamSPR.h"

namespace Interface
{
    bool FITKRadiossDataPropBeamSPRAdaptor::adaptR()
    {
        _beamSPRData = dynamic_cast<Radioss::FITKPropBeamSPR*>(_dataObj);
        if (_reader == nullptr || _beamSPRData == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!readRadiossData(_beamSPRData, *_h5Group)) return false;
        if (!readDataObject(_beamSPRData, *_h5Group)) return false;
        // 获取属性值对象
        Radioss::FITKPropValueBeamSPR* propValue = _beamSPRData->getValue();
        if (propValue == nullptr) return false;
        // 读取基本属性参数
        double doubleValue = 0.0;
        int intValue = 0;
        if (!h5tools->readGroupDouble(*_h5Group, "Mass", &doubleValue)) return false;
        propValue->setMass(doubleValue);
        if (!h5tools->readGroupDouble(*_h5Group, "Inertia", &doubleValue)) return false;
        propValue->setInertia(doubleValue);
        if (!h5tools->readGroupint(*_h5Group, "SkewID", &intValue)) return false;
        propValue->setSkewID(intValue);
        if (!h5tools->readGroupint(*_h5Group, "SensID", &intValue)) return false;
        propValue->setSensID(intValue);
        if (!h5tools->readGroupint(*_h5Group, "Isflag", &intValue)) return false;
        propValue->setIsflag(intValue);
        if (!h5tools->readGroupint(*_h5Group, "Ifail", &intValue)) return false;
        propValue->setIfail(intValue);
        if (!h5tools->readGroupint(*_h5Group, "Ileng", &intValue)) return false;
        propValue->setIleng(intValue);
        if (!h5tools->readGroupint(*_h5Group, "Ifail2", &intValue)) return false;
        propValue->setIfail2(intValue);
        if (!h5tools->readGroupDouble(*_h5Group, "V0", &doubleValue)) return false;
        propValue->setV0(doubleValue);
        if (!h5tools->readGroupDouble(*_h5Group, "W0", &doubleValue)) return false;
        propValue->setW0(doubleValue);
        if (!h5tools->readGroupDouble(*_h5Group, "Fcut", &doubleValue)) return false;
        propValue->setFcut(doubleValue);
        if (!h5tools->readGroupint(*_h5Group, "Fsmooth", &intValue)) return false;
        propValue->setFsmooth(intValue);
        // 读取不同维度的属性值
        if (!readValueDOF(propValue->getValueDOF(Radioss::DOF_SPR::Tension_Compression), _beamSPRData->getStrDOF(Radioss::DOF_SPR::Tension_Compression))) return false;
        if (!readValueDOF(propValue->getValueDOF(Radioss::DOF_SPR::Shear_XY), _beamSPRData->getStrDOF(Radioss::DOF_SPR::Shear_XY))) return false;
        if (!readValueDOF(propValue->getValueDOF(Radioss::DOF_SPR::Shear_XZ), _beamSPRData->getStrDOF(Radioss::DOF_SPR::Shear_XZ))) return false;
        if (!readValueDOF(propValue->getValueDOF(Radioss::DOF_SPR::Torsion), _beamSPRData->getStrDOF(Radioss::DOF_SPR::Torsion))) return false;
        if (!readValueDOF(propValue->getValueDOF(Radioss::DOF_SPR::Bending_Y), _beamSPRData->getStrDOF(Radioss::DOF_SPR::Bending_Y))) return false;
        if (!readValueDOF(propValue->getValueDOF(Radioss::DOF_SPR::Bending_Z), _beamSPRData->getStrDOF(Radioss::DOF_SPR::Bending_Z))) return false;
        return true;
    }

    bool FITKRadiossDataPropBeamSPRAdaptor::adaptW()
    {
        _beamSPRData = dynamic_cast<Radioss::FITKPropBeamSPR*>(_dataObj);
        if (_writer == nullptr || _beamSPRData == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!writeRadiossData(_beamSPRData, *_h5Group))return false;
        if (!writeDataObject(_beamSPRData, *_h5Group))return false;
        // 获取属性值对象
        Radioss::FITKPropValueBeamSPR* propValue = _beamSPRData->getValue();
        if (propValue == nullptr) return false;
        // 写入基本属性参数
        h5tools->writeGroupAttrDouble(*_h5Group, "Mass", propValue->getMass());
        h5tools->writeGroupAttrDouble(*_h5Group, "Inertia", propValue->getInertia());
        h5tools->writeGroupAttrInt(*_h5Group, "SkewID", propValue->getSkewID());
        h5tools->writeGroupAttrInt(*_h5Group, "SensID", propValue->getSensID());
        h5tools->writeGroupAttrInt(*_h5Group, "Isflag", propValue->getIsflag());
        h5tools->writeGroupAttrInt(*_h5Group, "Ifail", propValue->getIfail());
        h5tools->writeGroupAttrInt(*_h5Group, "Ileng", propValue->getIleng());
        h5tools->writeGroupAttrInt(*_h5Group, "Ifail2", propValue->getIfail2());
        h5tools->writeGroupAttrDouble(*_h5Group, "V0", propValue->getV0());
        h5tools->writeGroupAttrDouble(*_h5Group, "W0", propValue->getW0());
        h5tools->writeGroupAttrDouble(*_h5Group, "Fcut", propValue->getFcut());
        h5tools->writeGroupAttrInt(*_h5Group, "Fsmooth", propValue->getFsmooth());
        // 写入属性类型信息
        QString type = Radioss::FITKPropBeamSPR::GetFITKPropBeamSPRRadiossKeyWord();
        h5tools->writeGroupAttrStr(*_h5Group, "PropertyType", type.toStdString());
        // 写入不同维度的属性值
        if (!writeValueDOF(propValue->getValueDOF(Radioss::DOF_SPR::Tension_Compression), _beamSPRData->getStrDOF(Radioss::DOF_SPR::Tension_Compression))) return false;
        if (!writeValueDOF(propValue->getValueDOF(Radioss::DOF_SPR::Shear_XY), _beamSPRData->getStrDOF(Radioss::DOF_SPR::Shear_XY))) return false;
        if (!writeValueDOF(propValue->getValueDOF(Radioss::DOF_SPR::Shear_XZ), _beamSPRData->getStrDOF(Radioss::DOF_SPR::Shear_XZ))) return false;
        if (!writeValueDOF(propValue->getValueDOF(Radioss::DOF_SPR::Torsion), _beamSPRData->getStrDOF(Radioss::DOF_SPR::Torsion))) return false;
        if (!writeValueDOF(propValue->getValueDOF(Radioss::DOF_SPR::Bending_Y), _beamSPRData->getStrDOF(Radioss::DOF_SPR::Bending_Y))) return false;
        if (!writeValueDOF(propValue->getValueDOF(Radioss::DOF_SPR::Bending_Z), _beamSPRData->getStrDOF(Radioss::DOF_SPR::Bending_Z))) return false;
        return true;
    }
    bool FITKRadiossDataPropBeamSPRAdaptor::readValueDOF(Radioss::ValueDOF& valueDOF,QString name)
    {
        // 打开子组
        H5::Group dofGroup = _h5Group->openGroup(name.toStdString());
        if (!dofGroup.getId()) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        // 读取其他属性参数
        double doubleValue = 0.0;
        int intValue = 0;
        if (!h5tools->readGroupDouble(dofGroup, "K", &doubleValue)) return false;
        valueDOF.K = doubleValue;
        if (!h5tools->readGroupDouble(dofGroup, "C", &doubleValue)) return false;
        valueDOF.C = doubleValue;
        if (!h5tools->readGroupDouble(dofGroup, "A", &doubleValue)) return false;
        valueDOF.A = doubleValue;
        if (!h5tools->readGroupDouble(dofGroup, "B", &doubleValue)) return false;
        valueDOF.B = doubleValue;
        if (!h5tools->readGroupDouble(dofGroup, "D", &doubleValue)) return false;
        valueDOF.D = doubleValue;
        if (!h5tools->readGroupint(dofGroup, "fct_ID1", &intValue)) return false;
        valueDOF.fct_ID1 = intValue;
        if (!h5tools->readGroupint(dofGroup, "H", &intValue)) return false;
        valueDOF.H = intValue;
        if (!h5tools->readGroupint(dofGroup, "fct_ID2", &intValue)) return false;
        valueDOF.fct_ID2 = intValue;
        if (!h5tools->readGroupint(dofGroup, "fct_ID3", &intValue)) return false;
        valueDOF.fct_ID3 = intValue;
        if (!h5tools->readGroupint(dofGroup, "fct_ID4", &intValue)) return false;
        valueDOF.fct_ID4 = intValue;
        if (!h5tools->readGroupDouble(dofGroup, "min", &doubleValue)) return false;
        valueDOF.min = doubleValue;
        if (!h5tools->readGroupDouble(dofGroup, "max", &doubleValue)) return false;
        valueDOF.max = doubleValue;
        if (!h5tools->readGroupDouble(dofGroup, "F", &doubleValue)) return false;
        valueDOF.F = doubleValue;
        if (!h5tools->readGroupDouble(dofGroup, "E", &doubleValue)) return false;
        valueDOF.E = doubleValue;
        if (!h5tools->readGroupDouble(dofGroup, "Ascale", &doubleValue)) return false;
        valueDOF.Ascale = doubleValue;
        if (!h5tools->readGroupDouble(dofGroup, "Hscale", &doubleValue)) return false;
        valueDOF.Hscale = doubleValue;
        if (!h5tools->readGroupDouble(dofGroup, "vc", &doubleValue)) return false;
        valueDOF.vc = doubleValue;
        if (!h5tools->readGroupDouble(dofGroup, "n", &doubleValue)) return false;
        valueDOF.n = doubleValue;
        if (!h5tools->readGroupDouble(dofGroup, "a", &doubleValue)) return false;
        valueDOF.a = doubleValue;
        if (!h5tools->readGroupDouble(dofGroup, "e", &doubleValue)) return false;
        valueDOF.e = doubleValue;
        return true;
    }

    bool FITKRadiossDataPropBeamSPRAdaptor::writeValueDOF(const Radioss::ValueDOF& valueDOF, QString name)
    {
        // 创建子组
        H5::Group dofGroup = _h5Group->createGroup(name.toStdString());
        if (!dofGroup.getId()) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        // 写入其他属性参数
        h5tools->writeGroupAttrDouble(dofGroup, "K", valueDOF.K);
        h5tools->writeGroupAttrDouble(dofGroup, "C", valueDOF.C);
        h5tools->writeGroupAttrDouble(dofGroup, "A", valueDOF.A);
        h5tools->writeGroupAttrDouble(dofGroup, "B", valueDOF.B);
        h5tools->writeGroupAttrDouble(dofGroup, "D", valueDOF.D);
        h5tools->writeGroupAttrInt(dofGroup, "fct_ID1", valueDOF.fct_ID1);
        h5tools->writeGroupAttrInt(dofGroup, "H", valueDOF.H);
        h5tools->writeGroupAttrInt(dofGroup, "fct_ID2", valueDOF.fct_ID2);
        h5tools->writeGroupAttrInt(dofGroup, "fct_ID3", valueDOF.fct_ID3);
        h5tools->writeGroupAttrInt(dofGroup, "fct_ID4", valueDOF.fct_ID4);
        h5tools->writeGroupAttrDouble(dofGroup, "min", valueDOF.min);
        h5tools->writeGroupAttrDouble(dofGroup, "max", valueDOF.max);
        h5tools->writeGroupAttrDouble(dofGroup, "F", valueDOF.F);
        h5tools->writeGroupAttrDouble(dofGroup, "E", valueDOF.E);
        h5tools->writeGroupAttrDouble(dofGroup, "Ascale", valueDOF.Ascale);
        h5tools->writeGroupAttrDouble(dofGroup, "Hscale", valueDOF.Hscale);
        h5tools->writeGroupAttrDouble(dofGroup, "vc", valueDOF.vc);
        h5tools->writeGroupAttrDouble(dofGroup, "n", valueDOF.n);
        h5tools->writeGroupAttrDouble(dofGroup, "a", valueDOF.a);
        h5tools->writeGroupAttrDouble(dofGroup, "e", valueDOF.e);
        return true;
    }
}
