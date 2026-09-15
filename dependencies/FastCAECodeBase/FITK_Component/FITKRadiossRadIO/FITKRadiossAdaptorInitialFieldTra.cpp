/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossAdaptorInitialFieldTra.h"

#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKInitialField.h"

#include "FITKRadiossRadReader.h"
#include "FITKRadiossRadWriter.h"
#include "RadKeyLineSpliter.h"
#include "RadWriteInformationMapper.h"


namespace Radioss
{
    QString FITKRadiossAdaptorInitialFieldTra::getAdaptorClass()
    {
        return "Radioss::FITKRadiossAdaptorInitialFieldTra";
    }

    bool FITKRadiossAdaptorInitialFieldTra::adaptR()
    {
        if (_caseData == nullptr || _reader == nullptr) return false;
        
        // 创建边界条件数据对象
        FITKInitialFieldTra* inivel = new FITKInitialFieldTra();
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

    bool FITKRadiossAdaptorInitialFieldTra::adaptW()
    {
        //写出初速度数据
        FITKInitialFieldTra* inivel = dynamic_cast<FITKInitialFieldTra*>(_dataObj);
        if (inivel == nullptr || _writer == nullptr) return false;

        QTextStream* stream = _writer->getStream();
        RadWriteInformationMapper* infoMapper = _writer->getWriteInpInformationMapper();
        if (stream == nullptr || infoMapper == nullptr) return false;
        //写出关键字行  ID 从1开始递增
        int radID = infoMapper->getMaxRadInitialFieldID() + 1;
        //建立映射关系
        infoMapper->insertInitialFieldIDMap(inivel->getDataObjectID(), radID);
        *stream << inivel->GetFITKInitialFieldTraRadiossKeyWord() << "/" << radID << endl;
        //写出名称
        *stream << inivel->getDataObjectName() << endl;
        //节点组ID
        int grnd = infoMapper->getRadIDByNodeSetID(inivel->getSetID());

        *stream << QString("#%1%2%3%4%5").arg("VX", 19).arg("VY", 20).arg("VZ", 20).arg("grnd_ID", 10)
            .arg("skew_ID", 10) << endl;
        *stream << gapStr(inivel->getValue(0)) << gapStr(inivel->getValue(1)) <<gapStr(inivel->getValue(2))
            << gapStr(grnd) << gapStr(inivel->getSystemID()) << endl;

        return true;
    }

    bool FITKRadiossAdaptorInitialFieldTra::readInivel(FITKInitialFieldTra * inivel)
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
        if (lines.empty())
        {
            _reader->consoleMessage(3, QString("Invalid Inivel : %1.").arg(inivel->getDataObjectName()));
            return false;
        }
        //读取属性参数
        this->readParam(lines, inivel);
        return true;
    }

    void FITKRadiossAdaptorInitialFieldTra::readParam(const QStringList & lines, FITKInitialFieldTra * inivel)
    {
        if (lines.empty() || inivel == nullptr) return;
        QString par0 = lines[0];
        bool ok = false;
        //初始速度值
        double value[3]{ 0.0,0.0,0.0 };
        double vx = par0.mid(0, 20).trimmed().toDouble(&ok);
        if (ok) value[0] = vx;
        double vy = par0.mid(20, 20).trimmed().toDouble(&ok);
        if (ok) value[1] = vy;
        double vz = par0.mid(40, 20).trimmed().toDouble(&ok);
        if (ok) value[2] = vz;
        inivel->setValue(value);
        //节点组Id
        int grndId = par0.mid(60, 10).trimmed().toInt(&ok);
        if (ok) inivel->setSetID(grndId);
        //局部坐标系Id
        int skewId = par0.mid(70, 10).trimmed().toInt(&ok);
        if (ok) inivel->setSystemID(skewId);
    }

}


