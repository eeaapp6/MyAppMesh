/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossDataPropBeamAdaptor.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
//RadiossData
#include "FITK_Component/FITKRadiossData/FITKPropBeam.h"

namespace Interface
{
    bool FITKRadiossDataPropBeamAdaptor::adaptR()
    {
        _data = dynamic_cast<Radioss::FITKPropBeam*>(_dataObj);
        if (_reader == nullptr || _data == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!readRadiossData(_data, *_h5Group)) return false;
        if (!readDataObject(_data, *_h5Group)) return false;
        // 获取属性值对象
        Radioss::FITKPropValueBeam* propValue = _data->getValue();
        if (propValue == nullptr) return false;
        // 读取梁属性参数
        int smallStrainOption = 0;
        if (!h5tools->readGroupint(*_h5Group, "SmallStrainOption", &smallStrainOption)) return false;
        propValue->setSmallStrainOption(smallStrainOption);

        double beamMembraneDamping = 0.0;
        if (!h5tools->readGroupDouble(*_h5Group, "BeamMembraneDamping", &beamMembraneDamping)) return false;
        propValue->setBeamMembraneDamping(beamMembraneDamping);

        double beamFlexuralDamping = 0.01;
        if (!h5tools->readGroupDouble(*_h5Group, "BeamFlexuralDamping", &beamFlexuralDamping)) return false;
        propValue->setBeamFlexuralDamping(beamFlexuralDamping);

        double crossSection = 0.0;
        if (!h5tools->readGroupDouble(*_h5Group, "CrossSection", &crossSection)) return false;
        propValue->setCrossSection(crossSection);

        double areaMomentInertiaIyy = 0.0;
        if (!h5tools->readGroupDouble(*_h5Group, "AreaMomentInertiaIyy", &areaMomentInertiaIyy)) return false;
        propValue->setAreaMomentInertiaIyy(areaMomentInertiaIyy);

        double areaMomentInertiaIzz = 0.0;
        if (!h5tools->readGroupDouble(*_h5Group, "AreaMomentInertiaIzz", &areaMomentInertiaIzz)) return false;
        propValue->setAreaMomentInertiaIzz(areaMomentInertiaIzz);

        double areaMomentInertiaIxx = 0.0;
        if (!h5tools->readGroupDouble(*_h5Group, "AreaMomentInertiaIxx", &areaMomentInertiaIxx)) return false;
        propValue->setAreaMomentInertiaIxx(areaMomentInertiaIxx);

        bool isNeglectsShear = false;
        if (!h5tools->readGroupBool(*_h5Group, "NeglectsShear", &isNeglectsShear)) return false;
        propValue->setNeglectsShear(isNeglectsShear);
        // 读取节点1旋转自由度
        for (int i = 0; i < 3; ++i) {
            std::string attrName = "RotationDOF1_" + std::to_string(i);
            bool dofValue = true;
            if (!h5tools->readGroupBool(*_h5Group, attrName.c_str(), &dofValue)) return false;
            propValue->setRotationDOF1(i, dofValue);
        }
        // 读取节点2旋转自由度
        for (int i = 0; i < 3; ++i) {
            std::string attrName = "RotationDOF2_" + std::to_string(i);
            bool dofValue = true;
            if (!h5tools->readGroupBool(*_h5Group, attrName.c_str(), &dofValue)) return false;
            propValue->setRotationDOF2(i, dofValue);
        }
        return true;
    }

    bool FITKRadiossDataPropBeamAdaptor::adaptW()
    {
        _data = dynamic_cast<Radioss::FITKPropBeam*>(_dataObj);
        if (_writer == nullptr || _data == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!writeRadiossData(_data, *_h5Group)) return false;
        if (!writeDataObject(_data, *_h5Group)) return false;
        // 获取属性值对象
        Radioss::FITKPropValueBeam* propValue = _data->getValue();
        if (propValue == nullptr) return false;
        // 写入梁属性参数
        h5tools->writeGroupAttrInt(*_h5Group, "SmallStrainOption", propValue->getSmallStrainOption());
        h5tools->writeGroupAttrDouble(*_h5Group, "BeamMembraneDamping", propValue->getBeamMembraneDamping());
        h5tools->writeGroupAttrDouble(*_h5Group, "BeamFlexuralDamping", propValue->getBeamFlexuralDamping());
        h5tools->writeGroupAttrDouble(*_h5Group, "CrossSection", propValue->getCrossSection());
        h5tools->writeGroupAttrDouble(*_h5Group, "AreaMomentInertiaIyy", propValue->getAreaMomentInertiaIyy());
        h5tools->writeGroupAttrDouble(*_h5Group, "AreaMomentInertiaIzz", propValue->getAreaMomentInertiaIzz());
        h5tools->writeGroupAttrDouble(*_h5Group, "AreaMomentInertiaIxx", propValue->getAreaMomentInertiaIxx());
        h5tools->writeGroupAttrBool(*_h5Group, "NeglectsShear", propValue->isNeglectsShear());
        // 写入节点1旋转自由度
        for (int i = 0; i < 3; ++i) {
            std::string attrName = "RotationDOF1_" + std::to_string(i);
            h5tools->writeGroupAttrBool(*_h5Group, attrName.c_str(), propValue->isRotationDOF1(i));
        }
        // 写入节点2旋转自由度
        for (int i = 0; i < 3; ++i) {
            std::string attrName = "RotationDOF2_" + std::to_string(i);
            h5tools->writeGroupAttrBool(*_h5Group, attrName.c_str(), propValue->isRotationDOF2(i));
        }
        // 写入属性类型信息
        QString type = Radioss::FITKPropBeam::GetFITKPropBeamRadiossKeyWord();
        h5tools->writeGroupAttrStr(*_h5Group, "PropertyType", type.toStdString());

        return true;
    }
}