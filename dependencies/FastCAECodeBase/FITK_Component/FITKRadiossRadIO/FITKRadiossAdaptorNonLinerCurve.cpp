/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossAdaptorNonLinerCurve.h"

#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKNonLinerCurve.h"

#include "FITKRadiossRadWriter.h"
#include "FITKRadiossRadReader.h"
#include "RadWriteInformationMapper.h"
#include "RadKeyLineSpliter.h"
#include "RadReadInformationMapper.h"

namespace Radioss
{
    QString FITKRadiossAdaptorNonLinerCurve::getAdaptorClass()
    {
        return "Radioss::FITKRadiossAdaptorNonLinerCurve";
    }

    bool FITKRadiossAdaptorNonLinerCurve::adaptR()
    {
        if (_caseData == nullptr || _reader == nullptr) return false;
        
        // 读取关键字
        RadKeyLineSpliter *keyLineSpliter = _reader->getKeyLineSpliter();
        RadReadInformationMapper* mapper = _reader->getReadRadInformationMapper();
        if (!keyLineSpliter||!mapper) return false;
        int pid = keyLineSpliter->getId();
        QString keyLine = keyLineSpliter->getCurrentLine();
        //获取nonLinerCurve
        FITKNonLinerCurve* nonLinerCurve = new FITKNonLinerCurve();
        if (nonLinerCurve == nullptr) return false;
        
        bool ok = false;
        while (_reader && !_reader->atEnd())
        {
            QString line = _reader->readLine();
            //其他关键字
            if (line.startsWith("/"))
            {
                _reader->backLine();
                break;
            }
            //读取非线性函数
            ok = this->readFunct(nonLinerCurve);
        }
        if (!ok)
        {
            _reader->consoleMessage(3, QString("Read funct Error : %1").arg(keyLine));
            return false;
        }
        _reader->consoleMessage(1, QString("Read Funct : %1").arg(keyLine));
        _caseData->getNonLinerCurveManager()->appendDataObj(nonLinerCurve);
        //添加映射
        mapper->insertCurveIDMap(pid, nonLinerCurve->getDataObjectID());
        return true;
    }

    bool FITKRadiossAdaptorNonLinerCurve::adaptW()
    {
        // 写出非线性函数数据
        FITKNonLinerCurve* curve = dynamic_cast<FITKNonLinerCurve*>(_dataObj);
        if (curve == nullptr || _writer == nullptr) return false;

        QTextStream* stream = _writer->getStream();
        RadWriteInformationMapper* infoMapper = _writer->getWriteInpInformationMapper();
        if (stream == nullptr || infoMapper == nullptr) return false;
        //写出关键字行  ID 从1开始递增
        int radID = infoMapper->getMaxRadCurveID() + 1;
        //建立映射关系
        infoMapper->insertCurveIDMap(curve->getDataObjectID(), radID);
        *stream << curve->GetFITKNonLinerCurveRadiossKeyWord() << "/" << radID << endl;
        //写出名称
        *stream << curve->getDataObjectName() << endl;
        //写出坐标
        QList<Radioss::FITKNonLinerCurve::FITKNonLinerCurvePoint> points = curve->getPoints();
        for (auto point : points) {
            *stream << QString("%1").arg(QString::number(point.x, 'e', 6), 20)
                << QString("%1").arg(QString::number(point.y, 'e', 6), 20) << endl;
        }

        return true;
    }

    bool FITKRadiossAdaptorNonLinerCurve::readFunct(FITKNonLinerCurve * nonLinerCurve)
    {
        if (nonLinerCurve == nullptr || _reader == nullptr) return false;
        //设置函数名称
        QString line = _reader->previousLine().trimmed();
        nonLinerCurve->setDataObjectName(line);

        while (!_reader->atEnd())
        {
            line = _reader->readLine();
            if (line.startsWith("/"))
            {
                _reader->backLine();
                break;
            }
            QStringList funct = line.simplified().split(" ");
            if (funct.size() != 2)
            {
                _reader->consoleMessage(3, QString("Invalid Funct line : %1.").arg(line));
                continue;
            }
            // 函数点
            double x = funct[0].toDouble();
            double y = funct[1].toDouble();
            FITKNonLinerCurve::FITKNonLinerCurvePoint point;
            point.x = x;
            point.y = y;
            nonLinerCurve->addPoint(point);
            _reader->sendCurrentPercent();
        } 
        return true;
    }


}


