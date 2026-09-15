/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossRadWriter.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSolution.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossResultRequest.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSolverSettings.h"
#include "FITKRadiossAdaptorCase.h"
#include "RadWriteInformationMapper.h"
#include "FITKRadiossAbstractAdaptor.h"
#include "FITKRadiossAdaptorSolverSetting.h"
#include "FITKRadiossAdaptorSolverRequest.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"
#include <QFile>


namespace Radioss
{
    FITKRadiossRadWriter::FITKRadiossRadWriter(bool & success)
        : _success(success)
    {
        _informationWriteRadMapper = new RadWriteInformationMapper();
    }

    FITKRadiossRadWriter::~FITKRadiossRadWriter()
    {
        if (nullptr != _informationWriteRadMapper)
        {
            delete _informationWriteRadMapper;
            _informationWriteRadMapper = nullptr;
        }
    }

    void FITKRadiossRadWriter::run()
    {
        // 执行写入操作
        Radioss::FITKRadiossCase* caseData = dynamic_cast<Radioss::FITKRadiossCase*>(_data);
        //_informationWriteRadMapper->setCaseData(caseData);
        if (!caseData)
        {
            consoleMessage(3, "Failed to get Radioss case data.");
            _success = false;
            return;
        }
        //错误判断
        const QString fileName = _fileName;
        if (fileName.isEmpty())
        {
            consoleMessage(3, "File name is empty.");
            _success = false;
            return;
        }
        if (!fileName.toLower().endsWith(".rad"))
        {
            consoleMessage(3, "File name is not a .rad file.");
            _success = false;
            return;
        }
        //文件名称处理
        QString fileName0000{},fileName0001{};
        if (fileName.endsWith("_0000.rad", Qt::CaseInsensitive) ||
            fileName.endsWith("_0001.rad", Qt::CaseInsensitive))
        {
            fileName0000 = fileName.left(fileName.length() - 9) + "_0000.rad";
            fileName0001 = fileName.left(fileName.length() - 9) + "_0001.rad";
        }
        else
        {
            fileName0000 = fileName.left(fileName.length()-4) + "_0000.rad";
            fileName0001 = fileName.left(fileName.length()-4) + "_0001.rad";
        }

        _success = true;
        // 0000文件写入
        _success &= this->write0000File(fileName0000, caseData);
        if (!_success)
        {
            consoleMessage(3, QString("Failed to write file %1").arg(fileName0000));
            return;
        }
        // 0001文件写入
        _success &= this->write0001File(fileName0001, caseData);
        if (!_success)
        {
            consoleMessage(3, QString("Failed to write file %1").arg(fileName0001));
        }
        this->sendCalculateProgress(100);
    }

    RadWriteInformationMapper *FITKRadiossRadWriter::getWriteInpInformationMapper()
    {
        return _informationWriteRadMapper;
    }

    void FITKRadiossRadWriter::sendCalculateProgress(int Progress)
    {
        emit sendProcessSig(this, Progress);
    }   

    void FITKRadiossRadWriter::consoleMessage(int level, const QString& str)
    {
        // 打印控制台消息
        switch (level)
        {
        case 1:
            AppFrame::FITKMessageNormal(str);
            break;
        case 2:
            AppFrame::FITKMessageWarning(str);
            break;
        case 3:
            AppFrame::FITKMessageError(str);
            break;
        default:
            AppFrame::FITKMessageError(str);
            break;
        }
    }
    bool FITKRadiossRadWriter::write0000File(const QString &fileName, 
        Radioss::FITKRadiossCase *caseData)
    {
        this->setFileName(fileName);
        if(caseData == nullptr || !this->openFileStream())
            return false; 

        FITKRadiossAdaptorCase* adaptor = FITKIOADAPTORFACTORY->createT
                             <FITKRadiossAdaptorCase>("rad", caseData);
        if (adaptor == nullptr) return false;
        adaptor->setDataObject(caseData);
        adaptor->setFileWriter(this);
        bool ok = adaptor->adaptW();
        delete adaptor;
        this->closeFileStream();

        return ok;
    }
    bool FITKRadiossRadWriter::write0001File(const QString &fileName, 
        Radioss::FITKRadiossCase *caseData)
    {
        this->setFileName(fileName);
        if(caseData == nullptr || !this->openFileStream())
            return false; 

        FITKRadiossAbstractAdaptor* adaptor = FITKIOADAPTORFACTORY->createT
            <FITKRadiossAbstractAdaptor>("rad", caseData->getSolverSettings());
        if (adaptor == nullptr) return false;
        adaptor->setDataObject(caseData);
        adaptor->setFileWriter(this);
        bool ok = adaptor->adaptW();
        delete adaptor;

        adaptor = FITKIOADAPTORFACTORY->createT
            <FITKRadiossAbstractAdaptor>("rad", caseData->getResultRequest());
        if (adaptor == nullptr) return false;
        adaptor->setDataObject(caseData);
        adaptor->setFileWriter(this);
        ok = adaptor->adaptW();
        delete adaptor;

        this->closeFileStream();
        return true;
    }
    
}
// End of FITKRadiossRadWriter.cpp