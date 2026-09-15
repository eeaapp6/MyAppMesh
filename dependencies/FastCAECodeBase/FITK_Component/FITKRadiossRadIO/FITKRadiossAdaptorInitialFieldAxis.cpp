/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossAdaptorInitialFieldAxis.h"

#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKInitialField.h"

#include "FITKRadiossRadReader.h"
#include "FITKRadiossRadWriter.h"
#include "RadKeyLineSpliter.h"
#include "RadWriteInformationMapper.h"

namespace Radioss
{
    QString FITKRadiossAdaptorInitialFieldAxis::getAdaptorClass()
    {
        return "Radioss::FITKRadiossAdaptorInitialFieldAxis";
    }

    bool FITKRadiossAdaptorInitialFieldAxis::adaptR()
    {
        if (_caseData == nullptr || _reader == nullptr) return false;

        // 创建边界条件数据对象
        FITKInitialFieldAxis* inivel = new FITKInitialFieldAxis();
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
            //读取初速度数据
            ok = this->readInivel(inivel);
        }
        if (!ok)
        {
            _reader->consoleMessage(3, QString("Read Inivel Error : %1 %2").arg(keyLine).arg(inivel->getDataObjectName()));
            return false;
        }
        _reader->consoleMessage(1, QString("Read Inivel : %1 %2").arg(keyLine).arg(inivel->getDataObjectName()));
        _caseData->getInitialFieldManager()->appendDataObj(inivel);
        return ok;
    }

    bool FITKRadiossAdaptorInitialFieldAxis::adaptW()
    {
        //写出初始角速度数据
        FITKInitialFieldAxis* inivel = dynamic_cast<FITKInitialFieldAxis*>(_dataObj);
        if (inivel == nullptr || _writer == nullptr) return false;

        QTextStream* stream = _writer->getStream();
        RadWriteInformationMapper* infoMapper = _writer->getWriteInpInformationMapper();
        if (stream == nullptr || infoMapper == nullptr) return false;
        //写出关键字行  ID 从1开始递增
        int radID = infoMapper->getMaxRadInitialFieldID() + 1;
        //建立映射关系
        infoMapper->insertInitialFieldIDMap(inivel->getDataObjectID(), radID);
        *stream << inivel->GetFITKInitialFieldAxisRadiossKeyWord() << "/" << radID << endl;
        //写出名称
        *stream << inivel->getDataObjectName() << endl;
        //节点组ID
        int grnd = infoMapper->getRadIDByNodeSetID(inivel->getSetID());

        *stream << QString("#%1%2%3").arg("Dir", 9).arg("frame_ID", 10).arg("grnd_ID", 10) << endl;
        *stream << QString("         %1").arg(inivel->getDir()) << gapStr(inivel->getFrameID())
            << gapStr(grnd) << endl;
        *stream << QString("#%1%2%3%4").arg("Vx", 19).arg("Vy", 20).arg("Vz", 20).arg("vr", 20) << endl;
        *stream << gapStr(inivel->getValue(0)) << gapStr(inivel->getValue(1)) << gapStr(inivel->getValue(2))
            << gapStr(inivel->getVr()) << endl;
        return true;
    }

    bool FITKRadiossAdaptorInitialFieldAxis::readInivel(FITKInitialFieldAxis * inivel)
    {
        if (inivel == nullptr || _reader == nullptr) return false;

        QString line = _reader->previousLine().trimmed();
        //名称
        inivel->setDataObjectName(line);
        //读取重力信息
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
        //说明与手册不符
        if (lines.size()!=2)
        {
            _reader->consoleMessage(3, QString("Invalid Inivel : %1.").arg(inivel->getDataObjectName()));
            return false;
        }
        //读取属性参数
        this->readParam(lines, inivel);
        return true;
    }

    void FITKRadiossAdaptorInitialFieldAxis::readParam(const QStringList & lines, FITKInitialFieldAxis * inivel)
    {
        if (lines.size() != 2 || inivel == nullptr) return;
        bool ok = false;

        QString par = lines[0];
        //方向
        QString Dir = par.mid(0, 10).trimmed();
        if(!Dir.isEmpty())inivel->setDir(Dir);
        //frame_ID
        int frame_ID = par.mid(10, 10).trimmed().toInt(&ok);
        if (ok) inivel->setFrameID(frame_ID);
        //grnd_ID
        int grnd_ID = par.mid(20, 10).trimmed().toInt(&ok);
        if (ok) inivel->setSetID(grnd_ID);

        par = lines[1];
        double value[3]{ 0.0,0.0,0.0 };
        //Vx
        double Vx = par.mid(0, 20).trimmed().toDouble(&ok);
        if (ok) value[0] = Vx;  
        //Vy
        double Vy = par.mid(20, 20).trimmed().toDouble(&ok);
        if (ok) value[1] = Vy;       
        //Vz
        double Vz = par.mid(40, 20).trimmed().toDouble(&ok);
        if (ok) value[2] = Vz;     
        //vr
        double vr = par.mid(60, 20).trimmed().toDouble(&ok);
        if (ok) inivel->setVr(vr);
        //初始角速度值
        inivel->setValue(value);
    }
}