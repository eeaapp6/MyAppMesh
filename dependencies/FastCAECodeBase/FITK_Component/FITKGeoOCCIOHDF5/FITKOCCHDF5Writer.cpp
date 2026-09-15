/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCHDF5Writer.h"

//Kernel
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"
//Geomerty
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeoIOHDF5/FITKAdaptorGeoCommand.h"
#include "FITK_Interface/FITKInterfaceGeoIOHDF5/FITKAdaptorGeoDatum.h"
// #include "FITK_Interface/FITKInterfaceGeoIOHDF5/FITKInterfaceHDF5AdaptorDatum.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
#include <H5Cpp.h>

namespace IO
{

    void FITKOCCHDF5Writer::run()
    {
        *_resultMark = false;
        auto commList = dynamic_cast<Interface::FITKGeoCommandList*>(_data);
        if (!commList || !_h5File) return;
        *_resultMark = true;
        *_resultMark &= writeInfo();
        //*_resultMark &= writeVersion();
        *_resultMark &= writeCommDatas();
        *_resultMark &= writeDatumDatas();

        this->sendCalculateProgress(100);
    }

    void FITKOCCHDF5Writer::sendCalculateProgress(int Progress)
    {

    }

    void FITKOCCHDF5Writer::consoleMessage(int level, const QString &str)
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

    bool FITKOCCHDF5Writer::writeCommDatas()
    {
        auto commList = dynamic_cast<Interface::FITKGeoCommandList*>(_data);
        if (!commList || !_h5File) return false;
        bool isW = true;
        H5::Group casesGroup = _h5File->createGroup("Geometry");
        this->writeVersion(&casesGroup);
        int count = commList->getDataCount();
        for (int i = 0; i < count; i++)
        {
            Interface::FITKAbsGeoCommand* comm = commList->getDataByIndex(i);
            if (!comm) continue;
            // 获取适配器
            auto adaptor = FITKIOADAPTORFACTORY->createT<Interface::FITKAdaptorGeoCommand>("HDF5", comm);
            if (adaptor == nullptr) return false;
            adaptor->setH5Group(&casesGroup);
            adaptor->setDataObject(comm);
            adaptor->setWriter(this);
            bool w = adaptor->adaptW();
            if (!w) consoleMessage(3, QString("Write Command Errors, CaseName : %1").arg(comm->getDataObjectName()));
            isW &= w;
            delete adaptor;

        }

        //写出几何命令列表长度
        hsize_t       dim[2] = { 1, 1 };
        H5::DataSpace SpaceThree(2, dim);
        H5::Attribute transAttr = casesGroup.createAttribute("Count", H5::PredType::NATIVE_INT, SpaceThree);
        transAttr.write(H5::PredType::NATIVE_INT, &count);
        return isW;
    }

    bool FITKOCCHDF5Writer::writeDatumDatas()
    {
        auto commList = dynamic_cast<Interface::FITKGeoCommandList*>(_data);
        if (!commList || !_h5File) return false;
        IO::FITKHDF5FileTool* tool = this->getHDF5FileTool();
        if (!tool) return false;
        bool isW = true;

        H5::Group principalDatumsGroup = _h5File->createGroup("PrincipalDatums");
        Interface::FITKDatumList* datumList = commList->getDatumManager();
        if (!datumList) return false;
        Interface::FITKAbsGeoDatumLine* datumLineX = datumList->getPrincipalAxis(Interface::FITKGeoEnum::DatumAxisType::DAT_Principal_X);
        Interface::FITKAbsGeoDatumLine* datumLineY = datumList->getPrincipalAxis(Interface::FITKGeoEnum::DatumAxisType::DAT_Principal_Y);
        Interface::FITKAbsGeoDatumLine* datumLineZ = datumList->getPrincipalAxis(Interface::FITKGeoEnum::DatumAxisType::DAT_Principal_Z);
        if (!datumLineX || !datumLineY || !datumLineZ) return false;
        Interface::FITKAbsGeoDatumPlane* datumPlaneXOY = datumList->getPrincipalPlane(Interface::FITKGeoEnum::DatumPlnType::DPT_Principal_XOY);
        Interface::FITKAbsGeoDatumPlane* datumPlaneYOZ = datumList->getPrincipalPlane(Interface::FITKGeoEnum::DatumPlnType::DPT_Principal_YOZ);
        Interface::FITKAbsGeoDatumPlane* datumPlaneZOX = datumList->getPrincipalPlane(Interface::FITKGeoEnum::DatumPlnType::DPT_Principal_ZOX);
        if (!datumPlaneXOY || !datumPlaneYOZ || !datumPlaneZOX) return false;

        tool->writeGroupAttrInt(principalDatumsGroup, "DatumLineX", datumLineX->getDataObjectID());
        tool->writeGroupAttrInt(principalDatumsGroup, "DatumLineY", datumLineY->getDataObjectID());
        tool->writeGroupAttrInt(principalDatumsGroup, "DatumLineZ", datumLineZ->getDataObjectID());
        tool->writeGroupAttrInt(principalDatumsGroup, "DatumPlaneXOY", datumPlaneXOY->getDataObjectID());
        tool->writeGroupAttrInt(principalDatumsGroup, "DatumPlaneYOZ", datumPlaneYOZ->getDataObjectID());
        tool->writeGroupAttrInt(principalDatumsGroup, "DatumPlaneZOX", datumPlaneZOX->getDataObjectID());

        H5::Group datumsGroup = _h5File->createGroup("Datums");
        int count = datumList->getDataCount();
        for (int i = 0; i < count; i++)
        {
            Interface::FITKAbsGeoDatum* datum = datumList->getDataByIndex(i);
            if (!datum)  continue;
            // 获取适配器
            auto adaptor = FITKIOADAPTORFACTORY->createT<Interface::FITKAdaptorGeoCommand>("HDF5", datum);
            if (adaptor == nullptr) return false;
            adaptor->setH5Group(&datumsGroup);
            adaptor->setDataObject(datum);
            adaptor->setWriter(this);
            bool w = adaptor->adaptW();
            if (!w) consoleMessage(3, QString("Write Datum Errors, CaseName : %1").arg(datum->getDataObjectName()));
            isW &= w;
            delete adaptor;
        }

        //写出几何命令列表长度
        hsize_t       dim[2] = { 1, 1 };
        H5::DataSpace SpaceThree(2, dim);
        H5::Attribute transAttr = datumsGroup.createAttribute("Count", H5::PredType::NATIVE_INT, SpaceThree);
        transAttr.write(H5::PredType::NATIVE_INT, &count);
        return isW;
    }

    bool FITKOCCHDF5Writer::writeInfo()
    {
        H5::Group InfoGroup = _h5File->createGroup("Info");
        H5::DataSpace dataspace;
        H5::StrType   datatypeStr(H5::PredType::C_S1, H5T_VARIABLE);
        // 创建字符串类属性
        H5::Attribute desAttr = InfoGroup.createAttribute("SoftWareName", datatypeStr, dataspace);
        std::string softWareName = "CADOCCAPP";
        desAttr.write(datatypeStr, softWareName);
        return true;
    }

    bool FITKOCCHDF5Writer::writeVersion(H5::Group* g)
    {
        if (_version < 0) return true;
        QString name = "Version";
        H5::Group versionGroup = g->createGroup(name.toStdString());
        hsize_t       dim[2] = { 1, 1 };
        H5::DataSpace SpaceThree(2, dim);
        H5::Attribute transAttr = versionGroup.createAttribute(name.toStdString(), H5::PredType::NATIVE_DOUBLE, SpaceThree);
        transAttr.write(H5::PredType::NATIVE_DOUBLE, &_version);
        return true;
    }

    // Base.
    //@{


    //@}
}   // namespace IO
