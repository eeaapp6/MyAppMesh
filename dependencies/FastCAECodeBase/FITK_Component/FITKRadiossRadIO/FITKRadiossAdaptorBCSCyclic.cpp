/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossAdaptorBCSCyclic.h"

#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossBCS.h"

#include "FITKRadiossRadReader.h"
#include "FITKRadiossRadWriter.h"
#include "RadKeyLineSpliter.h"
#include "RadWriteInformationMapper.h"


namespace Radioss
{
    QString FITKRadiossAdaptorBCSCyclic::getAdaptorClass()
    {
        return "Radioss::FITKRadiossAdaptorBCSCyclic";
    }

    bool FITKRadiossAdaptorBCSCyclic::adaptR()
    {
        if (_caseData == nullptr || _reader == nullptr) return false;
        
        //创建周期边界条件数据对象
        FITKRadiossBCSCyclic* bcs = new FITKRadiossBCSCyclic();
        //读取关键字
        RadKeyLineSpliter *keyLineSpliter = _reader->getKeyLineSpliter();
        if (!keyLineSpliter) return false;

        //获取id和关键字
        int id = keyLineSpliter->getId();
        QString keyLine = keyLineSpliter->getCurrentLine();

        bool ok = false;
        while (_reader && !_reader->atEnd())
        {
            QString line = _reader->readLine();
            if (line.startsWith("/"))
            {
                _reader->backLine();
                break;
            }
            //读取边界条件数据
            ok = this->readBCS(bcs);
        }
        if (!ok)
        {
            _reader->consoleMessage(3, QString("Read BCS Error : %1 %2").arg(keyLine).arg(bcs->getDataObjectName()));
            return false;
        }
        _reader->consoleMessage(1, QString("Read BCS : %1 %2").arg(keyLine).arg(bcs->getDataObjectName()));
        _caseData->getBCSManager()->appendDataObj(bcs);
        return true;
    }

    bool FITKRadiossAdaptorBCSCyclic::adaptW()
    {
        //写出周期边界条件数据
        FITKRadiossBCSCyclic* bc = dynamic_cast<FITKRadiossBCSCyclic*>(_dataObj);
        if (bc == nullptr || _writer == nullptr) return false;

        QTextStream* stream = _writer->getStream();
        RadWriteInformationMapper* infoMapper = _writer->getWriteInpInformationMapper();
        if (stream == nullptr || infoMapper == nullptr) return false;
        //写出关键字行  ID 从1开始递增
        int radID = infoMapper->getMaxRadBcID() + 1;
        //建立映射关系
        infoMapper->insertBcIDMap(bc->getDataObjectID(), radID);
        *stream << bc->GetFITKRadiossBCSCyclicRadiossKeyWord() << "/" << radID << endl;
        //写出名称
        *stream << bc->getDataObjectName() << endl;
        //节点组ID
        int grnd1 = infoMapper->getRadIDByNodeSetID(bc->getGroupID1());
        int grnd2 = infoMapper->getRadIDByNodeSetID(bc->getGroupID2());

        *stream << QString("#%1%2%3").arg("skew_ID", 9).arg("grnd_ID1", 10).arg("grnd_ID2", 10) << endl;
        *stream << gapStr(bc->getCoordinateSystemID()) << gapStr(grnd1)
            << gapStr(grnd2) << endl;

        return true;
    }

    bool FITKRadiossAdaptorBCSCyclic::readBCS(FITKRadiossBCSCyclic * bcs)
    {
        if (bcs == nullptr || _reader == nullptr) return false;

        QString line = _reader->previousLine().trimmed();
        //名称
        bcs->setDataObjectName(line);
        //读取边界条件信息
       //读取边界条件信息
        QStringList lines;
        while (!_reader->atEnd())
        {
            line = _reader->readLine();
            if (line.startsWith("#")) continue;
            if (line.startsWith("/"))
            {
                _reader->backLine();
                break;
            }
            lines.append(line);
        }
        //与手册说明不符
        if (lines.size() != 1)
        {
            _reader->consoleMessage(3, QString("Invalid BCS : %1.").arg(bcs->getDataObjectName()));
            return false;
        }

        QStringList bcsInfo = lines[0].simplified().split(" ");
        bool ok = false;

        int skewId = lines[0].mid(0, 10).trimmed().toInt(&ok);
        if (ok)  bcs->setCoordinateSystemID(skewId);

        int grnodId1 = lines[0].mid(10, 10).trimmed().toInt(&ok);
        if (ok) {
            int grnodId2 = lines[0].mid(20, 10).trimmed().toInt(&ok);
            if(ok)  bcs->setGroupID(grnodId1, grnodId2);
        }
        return true;
    }
}


