/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossLSDynaReader.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITKRadiossLSDynaCaseAdaptor.h"
#include "FITKRadiossLSDynaLineParser.h"

namespace Radioss
{
    FITKRadiossLSDynaReader::FITKRadiossLSDynaReader(bool & success)
        : _success(success)
    {
    }

    void FITKRadiossLSDynaReader::run()
    {
        _success = false;

        Radioss::FITKRadiossCase* caseData = dynamic_cast<Radioss::FITKRadiossCase*>(_data);
        if (!caseData)
        {
            consoleMessage(3, "Failed to get Radioss case data.");
            return;
        }

        if (!this->loadFile())
        {
            consoleMessage(3, "Failed to load LS-DYNA file.");
            return;
        }

        consoleMessage(1, "Starting to parse LS-DYNA file...");

        caseData->clearData();

        do
        {
            QString line = this->readLine();
            if (line.isEmpty()) continue;

            if (!line.startsWith("*")) continue;
            // 解析关键字
            QString keyword = FITKRadiossLSDynaLineParser::parseKeyword(line);
            if (keyword.isEmpty()) continue;
            // 获取适配器
            FITKRadiossLSDynaAbstractAdaptor* adaptor = createAdaptor(keyword);
            if (!adaptor)
            {
                consoleMessage(2, QString("No adaptor found for keyword: %1").arg(keyword));
                continue;
            }
            adaptor->setFileReader(this);
            adaptor->setCaseData(caseData);
            adaptor->adaptR();
            delete adaptor;

        } while (!this->atEnd());

        this->sendCurrentPercent(100);

        _success = true;
        if (_success)
        {
            consoleMessage(1, "LS-DYNA file parsing completed successfully.");
        }
        else
        {
            consoleMessage(3, "LS-DYNA file parsing failed.");
        }
    }

    FITKRadiossLSDynaAbstractAdaptor* FITKRadiossLSDynaReader::createAdaptor(const QString& keyword)
    {
        // 根据关键字获取适配器
        return FITKRadiossLSDynaAbstractAdaptor::createAdaptor(keyword);
    }

    void FITKRadiossLSDynaReader::consoleMessage(int level, const QString& str)
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
