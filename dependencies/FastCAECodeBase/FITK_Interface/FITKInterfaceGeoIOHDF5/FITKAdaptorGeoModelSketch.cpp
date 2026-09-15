/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAdaptorGeoModelSketch.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoSketch2D.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeoIOHDF5/FITKAdaptorGeoCommand.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoSketchDeleter.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include <QMetaEnum>

namespace Interface
{
    bool  FITKAdaptorGeoModelSketch::readParameters()
    {
        Interface::FITKAbsGeoSketch2D* cmd = this->getDataObjectAs<Interface::FITKAbsGeoSketch2D>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);

        double pos[3] = {};
        double normal[3] = {};
        double up[3] = {};
        int count = 0;
        std::string type = {};
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Position, pos, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Normal, normal, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Up, up, 1, 3)) return false;
        if (!h5tools->readGroupStr(std::get<1>(paramGroup), FGKW_Type, type)) return false;
        QMetaEnum metaEnum = QMetaEnum::fromType<Interface::FITKAbsGeoSketch2D::SketchUseFor>();
        Interface::FITKAbsGeoSketch2D::SketchUseFor cmdType = (Interface::FITKAbsGeoSketch2D::SketchUseFor)metaEnum.keyToValue(type.c_str());
        cmd->setPosition(pos);
        cmd->setNormal(normal);
        cmd->setUp(up);
        cmd->setUserFor(cmdType);
        bool isR = true;
        std::tuple<bool, H5::Group> childsGroup = h5tools->openGp(std::get<1>(paramGroup), FGKW_Childs);
        if (!h5tools->readGroupint(std::get<1>(childsGroup), FGKW_Count, &count)) return false;
        for (int i = 0; i < count; i++)
        {
            //根据索引映射读取几何名称
            H5::Attribute geoNameAttribute = std::get<1>(childsGroup).openAttribute(std::to_string(i));
            std::string geoName = {};
            H5::StrType datatype(H5::PredType::C_S1, H5T_VARIABLE);
            geoNameAttribute.read(datatype, geoName);

            std::tuple<bool, H5::Group> geoGroup = h5tools->openGp(std::get<1>(childsGroup), geoName.c_str());

            std::string geoType = {};
            if (!h5tools->readGroupStr(std::get<1>(geoGroup), FGKW_CommandType, geoType)) return false;
            if (geoType.empty())return false;
            //实例几何命令类型
            Core::FITKAbstractNDataObject* geo = getGeoCommand(QString::fromStdString(geoType));
            if (!geo) return false;
            cmd->Interface::FITKAbsGeoSketch2D::appendDataObj(geo);
            Interface::FITKAbsGeoCommand* comm = dynamic_cast<Interface::FITKAbsGeoCommand*>(geo);
            if (!comm) return false;
            Interface::FITKAdaptorGeoCommand* adaptor = FITKIOADAPTORFACTORY->createT<Interface::FITKAdaptorGeoCommand>("HDF5", comm);
            if (adaptor == nullptr)
            {
                delete comm;
                return false;
            }
            adaptor->setH5Group(&std::get<1>(geoGroup));
            adaptor->setDataObject(comm);
            adaptor->setReader(_reader);
            bool r = adaptor->adaptR();
            isR &= r;
            delete adaptor;
        }
        return isR;
    }
    bool FITKAdaptorGeoModelSketch::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoSketch2D* cmd = this->getDataObjectAs<Interface::FITKAbsGeoSketch2D>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        double pos[3] = {};
        double normal[3] = {};
        double up[3] = {};
        cmd->getPosition(pos);
        cmd->getNormal(normal);
        cmd->getUp(up);
        FITKAbsGeoSketch2D::SketchUseFor type = cmd->userFor();
        bool ok = false;
        Core::FITKEnumTransfer<Interface::FITKAbsGeoSketch2D::SketchUseFor> enumTransfer;
        QString cmdTypeStr = enumTransfer.toString(type, ok);
        if (!ok) return false;
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Position, pos, 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Normal, normal, 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Up, up, 1, 3);
        h5tools->writeGroupAttrStr(std::get<1>(paramGroup), FGKW_Type, cmdTypeStr.toStdString());

        std::tuple<bool, H5::Group> childGroup = h5tools->createGp(std::get<1>(paramGroup), FGKW_Childs);
        bool isW = true;
        int count = cmd->getDataCount();
        for (int i = 0; i < count; i++)
        {
            Interface::FITKAbsGeoCommand* comm = cmd->getDataByIndex(i);
            if (!comm) continue;
            auto adaptor = FITKIOADAPTORFACTORY->createT<Interface::FITKAdaptorGeoCommand>("HDF5", comm);
            if (adaptor == nullptr) return false;
            adaptor->setH5Group(&std::get<1>(childGroup));
            adaptor->setDataObject(cmd->getDataByIndex(i));
            adaptor->setWriter(_writer);
            bool w = adaptor->adaptW();
            isW &= w;
            delete adaptor;
        }
        h5tools->writeGroupAttrInt(std::get<1>(childGroup), FGKW_Count, count);
        return isW;
    }

    Core::FITKAbstractNDataObject* FITKAdaptorGeoModelSketch::getGeoCommand(QString enumName)
    {
        if (enumName.isEmpty()) return nullptr;
        auto metaEnum = QMetaEnum::fromType<Interface::FITKGeoEnum::FITKGeometryComType>();
        int valueID = metaEnum.keyToValue(enumName.toLocal8Bit());
        if (valueID < 0) return nullptr;

        Interface::FITKInterfaceGeometryFactory* fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        if (!fac) return nullptr;

        return fac->createCommand((Interface::FITKGeoEnum::FITKGeometryComType)valueID);
    }

    bool  FITKAdaptorModelSketchDeleter::readParameters()
    {
        Interface::FITKAbsGeoSketchDeleter* cmd = this->getDataObjectAs<Interface::FITKAbsGeoSketchDeleter>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        QList<Interface::VirtualShape> entities = {};
        int count = {};
        //读取参数
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_Count, &count)) return false;
        for (int i = 0; i < count; i++)
        {
            int entitie[3] = {};
            if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), entitie, 1, 3)) return false;
            VirtualShape shape{ entitie[0], entitie[1], entitie[2] };
            entities.append(shape);
        }
        cmd->setToBeRemovedEntities(entities);

        return true;
    }
    bool FITKAdaptorModelSketchDeleter::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoSketchDeleter* cmd = this->getDataObjectAs<Interface::FITKAbsGeoSketchDeleter>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        QList<Interface::VirtualShape> entities = cmd->toBeRemovedEntities();
        int count = entities.size();
        //写出参数
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_Count, count);
        for (int i = 0; i < count; i++)
        {
            int entitie[3]{ entities[i].CmdId, entities[i].VirtualTopoId,entities[i].VirtualTopoIndex };
            h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), entitie, 1, 3);
        }
        return true;
    }
}



