/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCHDF5Reader.h" 
//Kernel
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"
//Geomerty
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoDatum.h"
#include "FITK_Interface/FITKInterfaceGeoIOHDF5/FITKAdaptorGeoCommand.h"
#include "FITK_Interface/FITKInterfaceGeoIOHDF5/FITKAdaptorGeoDatum.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"

#include <H5Cpp.h>
#include <QMetaEnum>

namespace IO
{

    void FITKOCCHDF5Reader::run()
    {
        *_resultMark = false;

        auto commList = dynamic_cast<Interface::FITKGeoCommandList*>(_data);
        if (!commList || !_h5File) return;
        if (!readInfo()) return;
        *_resultMark = true;
        //*_resultMark &= readVersion();
        *_resultMark &= readCommDatas();
        *_resultMark &= readDatumDatas();
    }

    void FITKOCCHDF5Reader::consoleMessage(int level, const QString &str)
    {
        qDebug() << str;
        //判断消息等级
        switch (level)
        {
        case 1:AppFrame::FITKMessageNormal(str); break;
        case 2: AppFrame::FITKMessageWarning(str); break;
        case 3:AppFrame::FITKMessageError(str); break;
        default: AppFrame::FITKMessageError(str); break;
        }
    }


    bool FITKOCCHDF5Reader::readCommDatas()
    {
        Interface::FITKGeoCommandList* commList = dynamic_cast<Interface::FITKGeoCommandList*>(_data);
        if (!commList || !_h5File) return false;
        IO::FITKHDF5FileTool* h5tools = this->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        bool isR = true;
        std::tuple<bool, H5::Group> casesGroup = h5tools->openGp(*_h5File, FGKW_Geometry);
        if (std::get<0>(casesGroup) == false) return false;
        this->readVersion(&std::get<1>(casesGroup));
        if (!h5tools->CheckGoupHaveAttribute(std::get<1>(casesGroup), FGKW_Count)) return false;
        int count = 0;
        if(!h5tools->readGroupint(std::get<1>(casesGroup), FGKW_Count, &count)) return false;
        for (int i = 0; i < count; i++)
        {
            //根据索引映射读取几何名称
            H5::Attribute geoNameAttribute = std::get<1>(casesGroup).openAttribute(std::to_string(i));
            std::string geoName = {};
            H5::StrType datatype(H5::PredType::C_S1, H5T_VARIABLE);
            geoNameAttribute.read(datatype, geoName);

            if (!std::get<1>(casesGroup).nameExists(geoName))
            {
                consoleMessage(3, QString("Read Geo Error, CaseName : %1").arg(QString::fromStdString(geoName)));
                return false;
            }

            std::tuple<bool, H5::Group> geoGroup = h5tools->openGp(std::get<1>(casesGroup), geoName.c_str());
            std::string geoType = {};
            if (!h5tools->readGroupStr(std::get<1>(geoGroup), FGKW_CommandType, geoType)) return false;
            if (geoType.empty())return false;
            //实例几何命令类型
            Core::FITKAbstractNDataObject* geo = getGeoCommand(QString::fromStdString(geoType));
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
            adaptor->setReader(this);
            bool r = adaptor->adaptR();
            if (!r)
            {
                consoleMessage(3, QString("Read Command Errors, Command : %1").arg(comm->getDataObjectName()));
                delete comm;
                return false;
            }
            comm->triangleShanpe();
            commList->appendDataObj(comm);
            isR &= r;
            delete adaptor;
        }
        return  isR;
    }

    bool FITKOCCHDF5Reader::readDatumDatas()
    {
        Interface::FITKGeoCommandList* commList = dynamic_cast<Interface::FITKGeoCommandList*>(_data);
        if (!commList || !_h5File) return false;
        IO::FITKHDF5FileTool* h5tools = this->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        bool isR = true;
        Interface::FITKDatumList* datumList = commList->getDatumManager();
        if (!datumList) return false;
        std::tuple<bool, H5::Group> principalDatumsGroup = h5tools->openGp(*_h5File, "PrincipalDatums");
        if (std::get<0>(principalDatumsGroup) == false) return false;

        Interface::FITKAbsGeoDatumLine* datumLineX = datumList->getPrincipalAxis(Interface::FITKGeoEnum::DatumAxisType::DAT_Principal_X);
        Interface::FITKAbsGeoDatumLine* datumLineY = datumList->getPrincipalAxis(Interface::FITKGeoEnum::DatumAxisType::DAT_Principal_Y);
        Interface::FITKAbsGeoDatumLine* datumLineZ = datumList->getPrincipalAxis(Interface::FITKGeoEnum::DatumAxisType::DAT_Principal_Z);
        if (!datumLineX || !datumLineY || !datumLineZ) return false;
        Interface::FITKAbsGeoDatumPlane* datumPlaneXOY = datumList->getPrincipalPlane(Interface::FITKGeoEnum::DatumPlnType::DPT_Principal_XOY);
        Interface::FITKAbsGeoDatumPlane* datumPlaneYOZ = datumList->getPrincipalPlane(Interface::FITKGeoEnum::DatumPlnType::DPT_Principal_YOZ);
        Interface::FITKAbsGeoDatumPlane* datumPlaneZOX = datumList->getPrincipalPlane(Interface::FITKGeoEnum::DatumPlnType::DPT_Principal_ZOX);
        if (!datumPlaneXOY || !datumPlaneYOZ || !datumPlaneZOX) return false;

        int datumLineXID, datumLineYID, datumLineZID,
            datumPlaneXOYID, datumPlaneYOZID, datumPlaneZOXID;

        if (!h5tools->readGroupint(std::get<1>(principalDatumsGroup), "DatumLineX", &datumLineXID))return false;
        if (!h5tools->readGroupint(std::get<1>(principalDatumsGroup), "DatumLineY", &datumLineYID))return false;
        if (!h5tools->readGroupint(std::get<1>(principalDatumsGroup), "DatumLineZ", &datumLineZID))return false;
        if (!h5tools->readGroupint(std::get<1>(principalDatumsGroup), "DatumPlaneXOY", &datumPlaneXOYID))return false;
        if (!h5tools->readGroupint(std::get<1>(principalDatumsGroup), "DatumPlaneYOZ", &datumPlaneYOZID))return false;
        if (!h5tools->readGroupint(std::get<1>(principalDatumsGroup), "DatumPlaneZOX", &datumPlaneZOXID))return false;

        Core::FITKAbstractDataIDAlter dataIDAlter;
        dataIDAlter.modifyDataID(datumLineX, datumLineXID);
        dataIDAlter.modifyDataID(datumLineY, datumLineYID);
        dataIDAlter.modifyDataID(datumLineZ, datumLineZID);
        dataIDAlter.modifyDataID(datumPlaneXOY, datumPlaneXOYID);
        dataIDAlter.modifyDataID(datumPlaneYOZ, datumPlaneYOZID);
        dataIDAlter.modifyDataID(datumPlaneZOX, datumPlaneZOXID);

        std::tuple<bool, H5::Group> casesGroup = h5tools->openGp(*_h5File, "Datums");
        if (std::get<0>(casesGroup) == false) return false;
        if (!h5tools->CheckGoupHaveAttribute(std::get<1>(casesGroup), FGKW_Count)) return false;
        int count = 0;
        if (!h5tools->readGroupint(std::get<1>(casesGroup), FGKW_Count, &count)) return false;
        for (int i = 0; i < count; i++)
        {
            //根据索引映射读取几何名称
            H5::Attribute geoNameAttribute = std::get<1>(casesGroup).openAttribute(std::to_string(i));
            std::string geoName = {};
            H5::StrType datatype(H5::PredType::C_S1, H5T_VARIABLE);
            geoNameAttribute.read(datatype, geoName);

            if (!std::get<1>(casesGroup).nameExists(geoName))
            {
                consoleMessage(3, QString("Read Geo Error, CaseName : %1").arg(QString::fromStdString(geoName)));
                return false;
            }

            std::tuple<bool, H5::Group> geoGroup = h5tools->openGp(std::get<1>(casesGroup), geoName.c_str());
            std::string geoType = {};
            if (!h5tools->readGroupStr(std::get<1>(geoGroup), "Type", geoType)) return false;
            if (geoType.empty())return false;
            //实例几何命令类型
            Core::FITKAbstractNDataObject* geo = getGeoDatum(QString::fromStdString(geoType));
            Interface::FITKAbsGeoDatum* comm = dynamic_cast<Interface::FITKAbsGeoDatum*>(geo);
            if (!comm) return false;
            Interface::FITKAdaptorGeoCommand* adaptor = FITKIOADAPTORFACTORY->createT<Interface::FITKAdaptorGeoCommand>("HDF5", comm);
            if (adaptor == nullptr)
            {
                delete comm;
                return false;
            }

            adaptor->setH5Group(&std::get<1>(geoGroup));
            adaptor->setDataObject(comm);
            adaptor->setReader(this);
            bool r = adaptor->adaptR();
            if (!r)
            {
                consoleMessage(3, QString("Read Command Errors, Command : %1").arg(comm->getDataObjectName()));
                delete comm;
                return false;
            }
            datumList->appendDataObj(comm);
            isR &= r;
            delete adaptor;
        }
        return true;
    }

    bool FITKOCCHDF5Reader::readInfo()
    {
        // 校验信息
        /*if (!_h5File->nameExists("Info")) return false;
        H5::Group infoGroup = _h5File->openGroup("Info");
        H5::StrType datatype(H5::PredType::C_S1, H5T_VARIABLE);
        H5::Attribute att = infoGroup.openAttribute("SoftWareName");
        std::string str{};
        att.read(datatype, str);
        bool flag = str == "CADOCCAPP";
        if (!flag)
            consoleMessage(3, "Error! Software Information Does Not Match HDF5 Information!");*/
        return true;
    }

    bool FITKOCCHDF5Reader::readVersion(H5::Group* g)
    {
        //写出HDF5管理功能版本号
        std::string name = "Version";
        if (!g->nameExists(name)) return false;
        H5::Group versionGroup = g->openGroup(name);
        if (!versionGroup.attrExists(name)) return false;
        auto attrVersion = versionGroup.openAttribute(name);
        double version = -1;
        attrVersion.read(H5::PredType::NATIVE_DOUBLE, &version);
        consoleMessage(1, QString("HDF5 SoftWare Version : %1, HDF5 File Version : %2").arg(_version).arg(version));
        if (version > _version)
        {
            consoleMessage(2, "Read Error, File Version Mismatch.");
            return false;
        }
        return true;
    }

    Core::FITKAbstractNDataObject* FITKOCCHDF5Reader::getGeoCommand(QString enumName)
    {
        if (enumName.isEmpty()) return nullptr;
        auto metaEnum = QMetaEnum::fromType<Interface::FITKGeoEnum::FITKGeometryComType>();
        int valueID = metaEnum.keyToValue(enumName.toLocal8Bit());
        if (valueID < 0) return nullptr;

        Interface::FITKInterfaceGeometryFactory* fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        if (!fac) return nullptr;

        return fac->createCommand((Interface::FITKGeoEnum::FITKGeometryComType)valueID);
    }

    Core::FITKAbstractNDataObject* FITKOCCHDF5Reader::getGeoDatum(QString enumName)
    {
        if (enumName.isEmpty()) return nullptr;
        auto metaEnum = QMetaEnum::fromType<Interface::FITKGeoEnum::FITKDatumType>();
        int valueID = metaEnum.keyToValue(enumName.toLocal8Bit());
        if (valueID < 0) return nullptr;

        Interface::FITKInterfaceGeometryFactory* fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        if (!fac) return nullptr;

        return fac->createDatum((Interface::FITKGeoEnum::FITKDatumType)valueID);
    }
    //@}
}   // namespace IO
