/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossAdaptorBCS.h"

#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossBCS.h"

#include "FITKRadiossRadReader.h"
#include "FITKRadiossRadWriter.h"
#include "RadKeyLineSpliter.h"
#include "RadWriteInformationMapper.h"


namespace Radioss
{
    QString FITKRadiossAdaptorBCS::getAdaptorClass()
    {
        return "Radioss::FITKRadiossAdaptorBCS";
    }

    bool FITKRadiossAdaptorBCS::adaptR()
    {
        if (_caseData == nullptr || _reader == nullptr) return false;
        
        // 创建边界条件数据对象
        FITKRadiossBCS* bcs = new FITKRadiossBCS();
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

    bool FITKRadiossAdaptorBCS::adaptW()
    {
        FITKRadiossBCS* bc = dynamic_cast<FITKRadiossBCS*>(_dataObj);
        if (bc == nullptr || _writer == nullptr) return false;

        QTextStream* stream = _writer->getStream();
        RadWriteInformationMapper* infoMapper = _writer->getWriteInpInformationMapper();
        if (stream == nullptr || infoMapper == nullptr) return false;
        //写出关键字行  ID 从1开始递增
        int radID = infoMapper->getMaxRadBcID() + 1;
        //建立映射关系
        infoMapper->insertBcIDMap(bc->getDataObjectID(), radID);
        *stream << bc->GetFITKRadiossBCSRadiossKeyWord() << "/" << radID << endl;
        //写出名称
        *stream << bc->getDataObjectName() << endl;
        //节点组ID
        int grnd = infoMapper->getRadIDByNodeSetID(bc->getGroupID1());
        //自由度
        QString DOF = QString("   %1%2%3 %4%5%6").arg(bc->isFreedomFixed(1)).arg(bc->isFreedomFixed(2)).arg(bc->isFreedomFixed(3))
            .arg(bc->isFreedomFixed(4)).arg(bc->isFreedomFixed(5)).arg(bc->isFreedomFixed(6));

        *stream << QString("#%1%2%3%4").arg("Tra", 5).arg("rot", 4).arg("skew_ID", 10).arg("grnod_ID", 10) << endl;
        *stream << DOF << gapStr(bc->getCoordinateSystemID()) << gapStr(grnd) << endl;

        return true;
    }

    bool FITKRadiossAdaptorBCS::readBCS(FITKRadiossBCS * bcs)
    {
        if (bcs == nullptr || _reader == nullptr) return false;
        bool ok = false;
        QString line = _reader->previousLine().trimmed();
        //名称
        bcs->setDataObjectName(line);
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

        ok = this->readParam(lines, bcs);
        return ok;
    }

    bool FITKRadiossAdaptorBCS::readParam(const QStringList& lines, FITKRadiossBCS * bcs)
    {
        if (lines.empty() || bcs == nullptr) return false;
        QString par = lines[0];
        QStringList bcsInfo = par.simplified().split(" ");
        QString translations = bcsInfo[0];
        QString rotations = bcsInfo[1];
        int tx = QString(translations.at(0)).toInt();
        int ty = QString(translations.at(1)).toInt();
        int tz = QString(translations.at(2)).toInt();
        int rx = QString(rotations.at(0)).toInt();
        int ry = QString(rotations.at(1)).toInt();
        int rz = QString(rotations.at(2)).toInt();

        bcs->setFreedomFixed(1, tx);
        bcs->setFreedomFixed(2, ty);
        bcs->setFreedomFixed(3, tz);
        bcs->setFreedomFixed(4, rx);
        bcs->setFreedomFixed(5, ry);
        bcs->setFreedomFixed(6, rz);

        bool ok = false;
        int skewId = par.mid(10, 10).trimmed().toInt(&ok);
        if (ok) bcs->setCoordinateSystemID(skewId);

        int grnodId = par.mid(20, 10).trimmed().toInt(&ok);
        if (ok) bcs->setGroupID(grnodId);

        return true;
    }
}


