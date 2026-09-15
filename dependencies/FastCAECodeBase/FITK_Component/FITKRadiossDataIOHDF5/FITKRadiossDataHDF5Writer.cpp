/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossDataHDF5Writer.h"

//Kernel
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossDataIOHDF5/FITKRadiossDataCaseAdaptor.h"

#include <H5Cpp.h>
#include <QDebug>

namespace IO
{

    void FITKRadiossDataHDF5Writer::run()
    {
        *_resultMark = false;
        Radioss::FITKRadiossCase* caseData = dynamic_cast<Radioss::FITKRadiossCase*>(_data);
        if (!caseData || !_h5File) return;
        *_resultMark = true;
        *_resultMark &= writeVersion();
        *_resultMark &= writeRadiossDatas();
        this->sendCalculateProgress(100);
    }

    void FITKRadiossDataHDF5Writer::sendCalculateProgress(int Progress)
    {

    }

    void FITKRadiossDataHDF5Writer::consoleMessage(int level, const QString &str)
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

    bool FITKRadiossDataHDF5Writer::writeRadiossDatas()
    {
        Radioss::FITKRadiossCase* caseData = dynamic_cast<Radioss::FITKRadiossCase*>(_data);
        if (!caseData) return false;
        if (!caseData || !_h5File) return false;
        bool isW = true;
        H5::Group casesGroup = _h5File->createGroup("Radioss");
        // 获取适配器
        Interface::FITKRadiossDataCaseAdaptor* adaptor = FITKIOADAPTORFACTORY->createT<Interface::FITKRadiossDataCaseAdaptor>("HDF5", caseData);
        if (adaptor == nullptr) return false;
        adaptor->setH5Group(&casesGroup);
        adaptor->setDataObject(caseData);
        adaptor->setWriter(this);
        bool w = adaptor->adaptW();
        if (!w) consoleMessage(3, QString("Write Radioss Data Errors, CaseName : %1").arg(caseData->getDataObjectName()));
        isW &= w;
        delete adaptor;
        return isW;
    }
   
    bool FITKRadiossDataHDF5Writer::writeVersion()
    {
        QString name = "Version";
        H5::Group versionGroup = _h5File->createGroup(name.toStdString());
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
