/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossDataHDF5Reader.h" 
//Kernel
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"

#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossDataIOHDF5/FITKRadiossDataCaseAdaptor.h"

#include <H5Cpp.h>
#include <QDebug>

namespace IO
{

    void FITKRadiossDataHDF5Reader::run()
    {
        *_resultMark = false;

        //auto commList = dynamic_cast<Interface::FITKGeoCommandList*>(_data);
        //if (!commList || !_h5File) return;
        *_resultMark = true;
        *_resultMark &= readVersion();

        *_resultMark &= readRadiossDatas();
    }

    void FITKRadiossDataHDF5Reader::consoleMessage(int level, const QString &str)
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


    bool FITKRadiossDataHDF5Reader::readRadiossDatas()
    {
        Radioss::FITKRadiossCase* caseData = dynamic_cast<Radioss::FITKRadiossCase*>(_data);
        if (!caseData) return false;
        if (!caseData || !_h5File) return false;
        bool isR = true;
        H5::Group casesGroup = _h5File->openGroup("Radioss");
        // 获取适配器
        Interface::FITKRadiossDataCaseAdaptor* adaptor = FITKIOADAPTORFACTORY->createT<Interface::FITKRadiossDataCaseAdaptor>("HDF5", caseData);
        if (adaptor == nullptr) return false;
        adaptor->setH5Group(&casesGroup);
        adaptor->setDataObject(caseData);
        adaptor->setReader(this);
        bool r = adaptor->adaptR();
        if (!r) consoleMessage(3, QString("Read Radioss Data Errors, CaseName : %1").arg(caseData->getDataObjectName()));
        isR &= r;
        delete adaptor;
        return isR;
    }

    bool FITKRadiossDataHDF5Reader::readVersion()
    {
        //写出HDF5管理功能版本号
        std::string name = "Version";
        if (!_h5File->nameExists(name)) return false;
        H5::Group versionGroup = _h5File->openGroup(name);
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
    //@}
}   // namespace IO
