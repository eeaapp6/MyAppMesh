/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossLSDynaWriter.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITKRadiossLSDynaCaseAdaptor.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

namespace Radioss
{
    FITKRadiossLSDynaWriter::FITKRadiossLSDynaWriter(bool & success)
        : _success(success)
    {
    }

    void FITKRadiossLSDynaWriter::run()
    {
        _success = false;
        // 获取算例
        Radioss::FITKRadiossCase* caseData = dynamic_cast<Radioss::FITKRadiossCase*>(_data);
        if (!caseData)
        {
            consoleMessage(3, "Failed to get Radioss case data.");
            return;
        }
        // 打开文件流
        if (!this->openFileStream())
        {
            consoleMessage(3, "Failed to open file for writing.");
            return;
        }

        consoleMessage(1, "Starting to write LS-DYNA file...");
        // 获取适配器
        FITKRadiossLSDynaCaseAdaptor* adaptor = FITKIOADAPTORFACTORY->createT<FITKRadiossLSDynaCaseAdaptor>("k", caseData);
        if (adaptor == nullptr)
        {
            consoleMessage(3, "Failed to create LSDYNA Case adaptor.");
            this->closeFileStream();
            return;
        }
        adaptor->setDataObject(caseData);
        adaptor->setFileWriter(this);
        adaptor->setCaseData(caseData);
        bool ok = adaptor->adaptW();
        delete adaptor;
        // 关闭文件流
        this->closeFileStream();

        _success = ok;
        if (_success)
        {
            consoleMessage(1, "LS-DYNA file writing completed successfully.");
        }
        else
        {
            consoleMessage(3, "LS-DYNA file writing failed.");
        }
    }

    void FITKRadiossLSDynaWriter::consoleMessage(int level, const QString& str)
    {
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
}
// End of FITKRadiossLsDynaWriter.cpp
