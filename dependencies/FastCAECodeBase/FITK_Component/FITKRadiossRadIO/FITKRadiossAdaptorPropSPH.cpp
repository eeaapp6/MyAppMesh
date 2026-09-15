/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossAdaptorPropSPH.h"

#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKPropSPH.h"

#include "FITKRadiossRadReader.h"
#include "FITKRadiossRadWriter.h"
#include "RadKeyLineSpliter.h"
#include "RadReadInformationMapper.h"
#include "RadWriteInformationMapper.h"

namespace Radioss
{
    QString FITKRadiossAdaptorPropSPH::getAdaptorClass()
    {
        return "Radioss::FITKRadiossAdaptorPropSPH";
    }

    bool FITKRadiossAdaptorPropSPH::adaptR()
    {
        if (_caseData == nullptr || _reader == nullptr) return false;

        Radioss::RadReadInformationMapper* mapper = _reader->getReadRadInformationMapper();
        if (!mapper) return false;

        // 读取关键字
        RadKeyLineSpliter* keyLineSpliter = _reader->getKeyLineSpliter();
        if (!keyLineSpliter) return false;
        int pid = keyLineSpliter->getId();
        QString keyLine = keyLineSpliter->getCurrentLine();
        //获取
        FITKPropSPH* prop = new FITKPropSPH();

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
            //读取实体属性数据
            ok = this->readPropSPH(prop);
        }
        if (!ok)
        {
            _reader->consoleMessage(1, QString("Read Prop Error : %1 %2").arg(keyLine).arg(prop->getDataObjectName()));
            return false;
        }
        _reader->consoleMessage(1, QString("Read Prop : %1 %2").arg(keyLine).arg(prop->getDataObjectName()));
        _caseData->getSectionManager()->appendDataObj(prop);

        mapper->insertPropertyIDMap(pid, prop->getDataObjectID());
        return true;
    }

    bool FITKRadiossAdaptorPropSPH::adaptW()
    {
        FITKPropSPH* prop = dynamic_cast<FITKPropSPH*>(_dataObj);
        if (prop == nullptr || _writer == nullptr) return false;
        FITKPropSPHValue* propV = prop->getValue();
        if (!propV) return false;

        QTextStream* stream = _writer->getStream();
        RadWriteInformationMapper* infoMapper = _writer->getWriteInpInformationMapper();
        if (stream == nullptr || infoMapper == nullptr) return false;
        //写出关键字行  ID 从1开始递增
        int radID = infoMapper->getMaxRadPropertyID() + 1;
        //建立映射关系
        infoMapper->insertPropertyIDMap(prop->getDataObjectID(), radID);
        *stream << prop->GetFITKPropSPHRadiossKeyWord() << "/" << radID << endl;
        //写出属性名称
        *stream << prop->getDataObjectName() << endl;

        *stream << QString("#%1%2%3%4%5%6").arg("mp", 19).arg("qa", 20).arg("qb", 20).
            arg("acs", 20).arg("skew_ID", 10).arg("h_ID", 10) << endl;
        *stream << gapStr(propV->getMP()) << gapStr(propV->getQA()) << gapStr(propV->getQB()) <<
            gapStr(propV->getACS()) << gapStr(propV->getSkewID()) << gapStr(propV->getHID(), 10, false) << endl;

        *stream << QString("#%1%2%3").arg("order", 9).arg("h", 20).arg("Stab", 20) << endl;
        *stream << gapStr(propV->getOrderID(), 10, false) << gapStr(propV->getH()) << gapStr(propV->getStab()) << endl;

        return true;
    }

    bool FITKRadiossAdaptorPropSPH::readPropSPH(FITKPropSPH * prop)
    {
        if (prop == nullptr || _reader == nullptr) return false;
        //设置属性名称
        QString line = _reader->previousLine().trimmed();
        prop->setDataObjectName(line);
        //参数存储
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
        if (lines.size() != 2)
        {
            _reader->consoleMessage(3, QString("Invalid Prop : %1.").arg(prop->getDataObjectName()));
            return false;
        }
        //读取属性参数并赋值
        this->readParam(lines, prop);

        return true;
    }

    void FITKRadiossAdaptorPropSPH::readParam(const QStringList & lines, Radioss::FITKPropSPH * prop)
    {
        if (lines.size() != 2 || !prop) return;
        FITKPropSPHValue* propV = prop->getValue();
        if (!propV)return;

        bool ok = false;
        //第一行
        QString par = lines[0];
        double mp = par.mid(0, 20).trimmed().toDouble(&ok);
        if (ok) propV->setMP(mp);
        double qa = par.mid(20, 20).trimmed().toDouble(&ok);
        if (ok) propV->setQA(qa);
        double qb = par.mid(40, 20).trimmed().toDouble(&ok);
        if (ok) propV->setQB(qb);
        double acs = par.mid(60, 20).trimmed().toDouble(&ok);
        if (ok) propV->setACS(acs);
        int skew_ID = par.mid(80, 10).trimmed().toInt(&ok);
        if (ok) propV->setSkewID(skew_ID);
        int h_ID = par.mid(80, 10).trimmed().toInt(&ok);
        if (ok) propV->setHID(h_ID);
        //第二行
        par = lines[1];
        int order = par.mid(0, 10).trimmed().toInt(&ok);
        if (ok) propV->setOrderID(order);
        double h = par.mid(10, 20).trimmed().toDouble(&ok);
        if (ok) propV->setH(h);
        double Stab = par.mid(30, 20).trimmed().toDouble(&ok);
        if (ok) propV->setStab(Stab);
    }
}