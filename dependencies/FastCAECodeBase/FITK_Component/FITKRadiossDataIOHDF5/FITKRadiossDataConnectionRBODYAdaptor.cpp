/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossDataConnectionRBODYAdaptor.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
//RadiossData
#include "FITK_Component/FITKRadiossData/FITKConnectionRBODY.h"

namespace Interface
{
    bool FITKRadiossDataConnectionRBODYAdaptor::adaptR()
    {
        _data = dynamic_cast<Radioss::FITKConnectionRBODY*>(_dataObj);
        if (_reader == nullptr || _data == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!readRadiossData(_data, *_h5Group)) return false;
        if (!readDataObject(_data, *_h5Group)) return false;

        // 读取主节点ID
        int mainNodeID = -1;
        if (!h5tools->readGroupint(*_h5Group, "MainNodeID", &mainNodeID)) return false;
        _data->setMainNodeID(mainNodeID);

        // 读取传感器ID
        int sensorID = -1;
        if (!h5tools->readGroupint(*_h5Group, "SensorID", &sensorID)) return false;
        _data->setSensorID(sensorID);

        // 读取局部坐标系ID
        int systemID = -1;
        if (!h5tools->readGroupint(*_h5Group, "SystemID", &systemID)) return false;
        _data->setSystemID(systemID);

        // 读取惯性类型
        std::string inertiaTypeStr{};
        if (!h5tools->readGroupStr(*_h5Group, "InertiaType", inertiaTypeStr)) return false;
        Core::FITKEnumTransfer<Radioss::FITKConnectionRBODY::RbodyIspherType> inertiaTypeTransfer;
        bool isInertiaTypeValid = false;
        Radioss::FITKConnectionRBODY::RbodyIspherType inertiaType
            = inertiaTypeTransfer.fromString(QString::fromStdString(inertiaTypeStr), isInertiaTypeValid);
        if (!isInertiaTypeValid) return false;
        _data->setInertiaType(inertiaType);

        // 读取质量
        double mass = 0.0;
        if (!h5tools->readGroupDouble(*_h5Group, "Mass", &mass)) return false;
        _data->setMass(mass);

        // 读取节点集合ID
        int nodeSetID = -1;
        if (!h5tools->readGroupint(*_h5Group, "NodeSetID", &nodeSetID)) return false;
        _data->setSecondaryNodesGroupID(nodeSetID);

        // 读取是否停用刚性墙
        bool isDeactivationRigidWall = false;
        if (!h5tools->readGroupBool(*_h5Group, "IsDeactivationRigidWall", &isDeactivationRigidWall)) return false;
        _data->setDeactivationRigidWall(isDeactivationRigidWall);

        // 读取重心计算类型
        std::string gravityCenterTypeStr{};
        if (!h5tools->readGroupStr(*_h5Group, "GravityCenterComputation", gravityCenterTypeStr)) return false;
        Core::FITKEnumTransfer<Radioss::FITKConnectionRBODY::GravityCenterComputationType> gravityCenterTypeTransfer;
        bool isGravityCenterTypeValid = false;
        Radioss::FITKConnectionRBODY::GravityCenterComputationType gravityCenterType
            = gravityCenterTypeTransfer.fromString(QString::fromStdString(gravityCenterTypeStr), isGravityCenterTypeValid);
        if (!isGravityCenterTypeValid) return false;
        _data->setGravityCenterComputation(gravityCenterType);

        // 读取包络面ID
        int surfID = -1;
        if (!h5tools->readGroupint(*_h5Group, "EnvelopeSurface", &surfID)) return false;
        _data->setEnvelopeSurface(surfID);

        // 读取惯性值
        double inertiaValue[6] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
        if (!h5tools->readGroupAttrDoubleArray(*_h5Group, "InertiaValue", inertiaValue,1, 6)) return false;
        _data->setInertiaValue(inertiaValue);

        // 读取是否考虑领域分解
        bool isDomainDecomposition = false;
        if (!h5tools->readGroupBool(*_h5Group, "IsDomainDecomposition", &isDomainDecomposition)) return false;
        _data->setDomainDecomposition(isDomainDecomposition);

        // 读取失效标准
        bool ifail = false;
        if (!h5tools->readGroupBool(*_h5Group, "Ifail", &ifail)) return false;
        _data->setIfail(ifail);

        // 读取可选数据（如果Ifail为true）
        if (ifail != 0) {
            double fn = 1e20;
            double ft = 1e20;
            int expN = 2;
            int expT = 2;

            if (!h5tools->readGroupDouble(*_h5Group, "FN", &fn)) return false;
            if (!h5tools->readGroupDouble(*_h5Group, "FT", &ft)) return false;
            if (!h5tools->readGroupint(*_h5Group, "ExpN", &expN)) return false;
            if (!h5tools->readGroupint(*_h5Group, "ExpT", &expT)) return false;

            Radioss::RBODY_OptionData& optionData = _data->getOptionData();
            optionData.FN = fn;
            optionData.FT = ft;
            optionData.expN = expN;
            optionData.expT = expT;
        }
        return true;
    }

    bool FITKRadiossDataConnectionRBODYAdaptor::adaptW()
    {
        _data = dynamic_cast<Radioss::FITKConnectionRBODY*>(_dataObj);
        if (_writer == nullptr || _data == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!writeRadiossData(_data, *_h5Group)) return false;
        if (!writeDataObject(_data, *_h5Group)) return false;

        // 获取数据
        int mainNodeID = _data->getMainNodeID();
        int sensorID = _data->getSensorID();
        int systemID = _data->getSystemID();
        auto inertiaType = _data->getInertiaType();
        double mass = _data->getMass();
        int nodeSetID = _data->getSecondaryNodesGroupID();
        bool isDeactivationRigidWall = _data->getDeactivationRigidWall();
        auto gravityCenterType = _data->getGravityCenterComputation();
        int surfID = _data->getEnvelopeSurface();
        double inertiaValue[6];
        _data->getInertiaValue(inertiaValue);
        bool isDomainDecomposition = _data->getDomainDecomposition();
        bool ifail = _data->getIfail();
        auto& optionData = _data->getOptionData();

        // 写入基本属性
        h5tools->writeGroupAttrInt(*_h5Group, "MainNodeID", mainNodeID);
        h5tools->writeGroupAttrInt(*_h5Group, "SensorID", sensorID);
        h5tools->writeGroupAttrInt(*_h5Group, "SystemID", systemID);

        // 写入枚举类型
        Core::FITKEnumTransfer<Radioss::FITKConnectionRBODY::RbodyIspherType> inertiaTypeTransfer;
        bool isInertiaTypeValid = false;
        QString inertiaTypeStr = inertiaTypeTransfer.toString(inertiaType, isInertiaTypeValid);
        if (!isInertiaTypeValid) return false;
        h5tools->writeGroupAttrStr(*_h5Group, "InertiaType", inertiaTypeStr.toStdString());

        h5tools->writeGroupAttrDouble(*_h5Group, "Mass", mass);
        h5tools->writeGroupAttrInt(*_h5Group, "NodeSetID", nodeSetID);
        h5tools->writeGroupAttrBool(*_h5Group, "IsDeactivationRigidWall", isDeactivationRigidWall);

        Core::FITKEnumTransfer<Radioss::FITKConnectionRBODY::GravityCenterComputationType> gravityCenterTypeTransfer;
        bool isGravityCenterTypeValid = false;
        QString gravityCenterTypeStr = gravityCenterTypeTransfer.toString(gravityCenterType, isGravityCenterTypeValid);
        if (!isGravityCenterTypeValid) return false;
        h5tools->writeGroupAttrStr(*_h5Group, "GravityCenterComputation", gravityCenterTypeStr.toStdString());

        h5tools->writeGroupAttrInt(*_h5Group, "EnvelopeSurface", surfID);
        h5tools->writeGroupAttrDoubleArray(*_h5Group, "InertiaValue", inertiaValue, 1,6);
        h5tools->writeGroupAttrBool(*_h5Group, "IsDomainDecomposition", isDomainDecomposition);
        h5tools->writeGroupAttrBool(*_h5Group, "Ifail", ifail);

        // 写入可选数据（如果Ifail为true）
        if (ifail) {
            h5tools->writeGroupAttrDouble(*_h5Group, "FN", optionData.FN);
            h5tools->writeGroupAttrDouble(*_h5Group, "FT", optionData.FT);
            h5tools->writeGroupAttrInt(*_h5Group, "ExpN", optionData.expN);
            h5tools->writeGroupAttrInt(*_h5Group, "ExpT", optionData.expT);
        }
        // 写入关键字
        QString connectionKeyWord = Radioss::FITKConnectionRBODY::GetFITKConnectionRBODYRadiossKeyWord();
        h5tools->writeGroupAttrStr(*_h5Group, "ConnectionKeyWord", connectionKeyWord.toStdString());
        return true;
    }
}