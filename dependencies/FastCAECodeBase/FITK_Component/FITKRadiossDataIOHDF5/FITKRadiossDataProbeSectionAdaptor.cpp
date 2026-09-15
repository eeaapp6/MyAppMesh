/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossDataProbeSectionAdaptor.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
//RadiossData
#include "FITK_Component/FITKRadiossData/FITKProbeSection.h"

namespace Interface
{
    bool FITKRadiossDataProbeSectionAdaptor::adaptR()
    {
        _data = dynamic_cast<Radioss::FITKProbeSection*>(_dataObj);
        if (_reader == nullptr || _data == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!readRadiossData(_data, *_h5Group)) return false;
        if (!readDataObject(_data, *_h5Group)) return false;

        // 读取单点集合1 ID
        int setID1 = -1;
        if (!h5tools->readGroupint(*_h5Group, "SetID1", &setID1)) return false;
        _data->setSetID1(setID1);
        // 读取单点集合2 ID
        int setID2 = -1;
        if (!h5tools->readGroupint(*_h5Group, "SetID2", &setID2)) return false;
        _data->setSetID2(setID2);
        // 读取单点集合3 ID
        int setID3 = -1;
        if (!h5tools->readGroupint(*_h5Group, "SetID3", &setID3)) return false;
        _data->setSetID3(setID3);
        // 读取节点组 ID
        int grndID = -1;
        if (!h5tools->readGroupint(*_h5Group, "GrndID", &grndID)) return false;
        _data->setGrndID(grndID);
        // 读取保存标志
        int isave = 0;
        if (!h5tools->readGroupint(*_h5Group, "Isave", &isave)) return false;
        _data->setIsave(isave);
        // 读取移动坐标系ID
        int frameID = -1;
        if (!h5tools->readGroupint(*_h5Group, "FrameID", &frameID)) return false;
        _data->setFrameID(frameID);
        // 读取时间步长
        double dt = 0.1;
        if (!h5tools->readGroupDouble(*_h5Group, "Dt", &dt)) return false;
        _data->setDt(dt);
        // 读取指数移动平均滤波常数
        double alpha = 0.0;
        if (!h5tools->readGroupDouble(*_h5Group, "Alpha", &alpha)) return false;
        _data->setAlpha(alpha);
        // 读取文件名
        std::string fileName{};
        if (!h5tools->readGroupStr(*_h5Group, "FileName", fileName)) return false;
        _data->setFileName(QString::fromStdString(fileName));
        // 读取Brick组ID
        int grbricID = -1;
        if (!h5tools->readGroupint(*_h5Group, "GrbricID", &grbricID)) return false;
        _data->setGrbricID(grbricID);
        // 读取Shell组ID
        int grshelID = -1;
        if (!h5tools->readGroupint(*_h5Group, "GrshelID", &grshelID)) return false;
        _data->setGrshelID(grshelID);
        // 读取Truss组ID
        int grtrusID = -1;
        if (!h5tools->readGroupint(*_h5Group, "GrtrusID", &grtrusID)) return false;
        _data->setGrtrusID(grtrusID);
        // 读取Beam组ID
        int grbeamID = -1;
        if (!h5tools->readGroupint(*_h5Group, "GrbeamID", &grbeamID)) return false;
        _data->setGrbeamID(grbeamID);
        // 读取Spring组ID
        int grsprgID = -1;
        if (!h5tools->readGroupint(*_h5Group, "GrsprgID", &grsprgID)) return false;
        _data->setGrsprgID(grsprgID);
        // 读取Triangle组ID
        int grtriaID = -1;
        if (!h5tools->readGroupint(*_h5Group, "GrtriaID", &grtriaID)) return false;
        _data->setGrtriaID(grtriaID);
        // 读取局部系统中心标志
        int iframe = 0;
        if (!h5tools->readGroupint(*_h5Group, "Iframe", &iframe)) return false;
        _data->setIframe(iframe);
        // 读取单元组类型
        std::string eleTypeStr{};
        if (!h5tools->readGroupStr(*_h5Group, "ElementGroupType", eleTypeStr)) return false;
        Core::FITKEnumTransfer<Radioss::FITKRadiossElementGroup::ElementGroupType> eleTypeTransfer;
        bool isEleTypeValid = false;
        Radioss::FITKRadiossElementGroup::ElementGroupType eleType
            = eleTypeTransfer.fromString(QString::fromStdString(eleTypeStr), isEleTypeValid);
        if (!isEleTypeValid) return false;
        _data->setElementGroupType(eleType);
        // 读取接口ID列表
        int ninter = 0;
        if (!h5tools->readGroupint(*_h5Group, "Ninter", &ninter)) return false;
        if (ninter > 0) {
            QList<int> interIDs;
            int* interArray = new int[ninter];
            if (h5tools->readGroupAttrIntArray(*_h5Group, "InterIDs", interArray, 1, ninter)) {
                for (int i = 0; i < ninter; ++i) {
                    interIDs.append(interArray[i]);
                }
                _data->setInterIDs(interIDs);
            }
            delete[] interArray;
        }
        return true;
    }

    bool FITKRadiossDataProbeSectionAdaptor::adaptW()
    {
        _data = dynamic_cast<Radioss::FITKProbeSection*>(_dataObj);
        if (_writer == nullptr || _data == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!writeRadiossData(_data, *_h5Group)) return false;
        if (!writeDataObject(_data, *_h5Group)) return false;
        // 获取数据
        int setID1 = _data->getSetID1();
        int setID2 = _data->getSetID2();
        int setID3 = _data->getSetID3();
        int grndID = _data->getGrndID();
        int isave = _data->getIsave();
        int frameID = _data->getFrameID();
        double dt = _data->getDt();
        double alpha = _data->getAlpha();
        QString fileName = _data->getFileName();
        int grbricID = _data->getGrbricID();
        int grshelID = _data->getGrshelID();
        int grtrusID = _data->getGrtrusID();
        int grbeamID = _data->getGrbeamID();
        int grsprgID = _data->getGrsprgID();
        int grtriaID = _data->getGrtriaID();
        int iframe = _data->getIframe();
        auto eleType = _data->getElementGroupType();
        QList<int> interIDs = _data->getInterIDs();
        int ninter = _data->getNinter();
        // 写入基本属性
        h5tools->writeGroupAttrInt(*_h5Group, "SetID1", setID1);
        h5tools->writeGroupAttrInt(*_h5Group, "SetID2", setID2);
        h5tools->writeGroupAttrInt(*_h5Group, "SetID3", setID3);
        h5tools->writeGroupAttrInt(*_h5Group, "GrndID", grndID);
        h5tools->writeGroupAttrInt(*_h5Group, "Isave", isave);
        h5tools->writeGroupAttrInt(*_h5Group, "FrameID", frameID);
        h5tools->writeGroupAttrDouble(*_h5Group, "Dt", dt);
        h5tools->writeGroupAttrDouble(*_h5Group, "Alpha", alpha);
        h5tools->writeGroupAttrStr(*_h5Group, "FileName", fileName.toStdString());
        h5tools->writeGroupAttrInt(*_h5Group, "GrbricID", grbricID);
        h5tools->writeGroupAttrInt(*_h5Group, "GrshelID", grshelID);
        h5tools->writeGroupAttrInt(*_h5Group, "GrtrusID", grtrusID);
        h5tools->writeGroupAttrInt(*_h5Group, "GrbeamID", grbeamID);
        h5tools->writeGroupAttrInt(*_h5Group, "GrsprgID", grsprgID);
        h5tools->writeGroupAttrInt(*_h5Group, "GrtriaID", grtriaID);
        h5tools->writeGroupAttrInt(*_h5Group, "Iframe", iframe);
        h5tools->writeGroupAttrInt(*_h5Group, "Ninter", ninter);
        // 写入枚举类型
        Core::FITKEnumTransfer<Radioss::FITKRadiossElementGroup::ElementGroupType> eleTypeTransfer;
        bool isEleTypeValid = false;
        QString eleTypeStr = eleTypeTransfer.toString(eleType, isEleTypeValid);
        if (!isEleTypeValid) return false;
        h5tools->writeGroupAttrStr(*_h5Group, "ElementGroupType", eleTypeStr.toStdString());
        // 写入接口ID列表
        if (ninter > 0) {
            int* interArray = new int[ninter];
            for (int i = 0; i < ninter; ++i) {
                interArray[i] = interIDs[i];
            }
            h5tools->writeGroupAttrIntArray(*_h5Group, "InterIDs", interArray, 1, ninter);
            delete[] interArray;
        }
        // 写入关键字
        QString probeKeyWord = Radioss::FITKProbeSection::GetFITKProbeSectionRadiossKeyWord();
        h5tools->writeGroupAttrStr(*_h5Group, "ProbeKeyWord", probeKeyWord.toStdString());
        return true;
    }
}