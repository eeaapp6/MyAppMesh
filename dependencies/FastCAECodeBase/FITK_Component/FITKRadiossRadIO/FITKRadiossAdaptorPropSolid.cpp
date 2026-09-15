/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossAdaptorPropSolid.h"

#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKPropSolid.h"

#include "FITKRadiossRadReader.h"
#include "FITKRadiossRadWriter.h"
#include "RadKeyLineSpliter.h"
#include "RadReadInformationMapper.h"
#include "RadWriteInformationMapper.h"

#include <QList>

namespace Radioss
{
    QString FITKRadiossAdaptorPropSolid::getAdaptorClass()
    {
        return "Radioss::FITKRadiossAdaptorPropSolid";
    }

    bool FITKRadiossAdaptorPropSolid::adaptR()
    {
        if (_caseData == nullptr || _reader == nullptr) return false;
        Radioss::RadReadInformationMapper *mapper = _reader->getReadRadInformationMapper();
        if (!mapper) return false;

        // 读取关键字
        RadKeyLineSpliter *keyLineSpliter = _reader->getKeyLineSpliter();
        if (!keyLineSpliter) return false;
        int pid = keyLineSpliter->getId();
        QString keyLine = keyLineSpliter->getCurrentLine();
        //获取solid
        FITKPropSolid* solid = new FITKPropSolid();
        if (solid == nullptr) return false;
        
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
            ok = this->readPropSolid(solid);
        }
        if (!ok)
        {
            _reader->consoleMessage(1, QString("Read Prop Error : %1 %2").arg(keyLine).arg(solid->getDataObjectName()));
            return false;
        }
        _reader->consoleMessage(1, QString( "Read Prop : %1 %2").arg(keyLine).arg(solid->getDataObjectName()));
        _caseData->getSectionManager()->appendDataObj(solid);
        
        //插入属性ID映射
        mapper->insertPropertyIDMap(pid, solid->getDataObjectID());
        return true;
    }

    bool FITKRadiossAdaptorPropSolid::adaptW()
    {
        FITKPropSolid* solid = dynamic_cast<FITKPropSolid*>(_dataObj);
        if (solid == nullptr || _writer == nullptr) return false;
        FITKPropSolidValue* solidValue = solid->getValue();
        if(!solidValue) return false;

        QTextStream* stream = _writer->getStream();
        RadWriteInformationMapper* infoMapper = _writer->getWriteInpInformationMapper();
        if (stream == nullptr || infoMapper == nullptr) return false;
        //写出关键字行  ID 从1开始递增
        int radID = infoMapper->getMaxRadPropertyID() + 1;
        //建立映射关系
        infoMapper->insertPropertyIDMap(solid->getDataObjectID(), radID);
        *stream << solid->GetFITKPropSolidRadiossKeyWord() << "/" << radID << endl;
        //写出属性名称
        *stream << solid->getDataObjectName() << endl;

        *stream << QString("#%1%2%3%4%5%6%7%8").arg("Isolid", 9).arg("Ismstr", 10).arg("Icpre", 20).arg("Itetra10", 10)
            .arg("Inpts", 10).arg("Itetra4", 10).arg("Iframe", 10).arg("dn", 20) << endl;
        *stream << gapStr(solidValue->getIsolid(), 10, false)<<gapStr(solidValue->getIsmstr(), 10, false) << gapStr(solidValue->getIcpre(),20)
            << gapStr(solidValue->getItetra10(), 10, false) << gapStr(222/*无接口*/) << gapStr(solidValue->getItetra4(), 10, false)
            << gapStr(solidValue->getIFrame(), 10, false) << gapStr(solid->getDn()) << endl;

        *stream << QString("#%1%2%3%4%5").arg("qa", 19).arg("qb", 20).arg("h", 20).arg("Lamv", 20).arg("Muv", 20) << endl;
        *stream << gapStr(solid->getQa()) << gapStr(solid->getQb()) << gapStr(solid->getH()) 
            << gapStr(solid->getLambdaV()) << gapStr(solid->getMuV()) << endl;

        *stream << QString("#%1").arg("tmin", 19) << endl;
        *stream << gapStr(solid->getMinTimeStep()) << endl;

        if (false/*activate Sol2SPH option,未找到接口*/) {
            *stream << QString("#%1%2").arg("Ndir", 9).arg("sphpartID", 10) << endl;
            *stream << gapStr(solid->getNdir(), 10, false) << gapStr(solid->getSPHPartID()) << endl;
        }

        return true;
    }

    bool FITKRadiossAdaptorPropSolid::readPropSolid(FITKPropSolid * solid)
    {
        if (solid == nullptr || _reader == nullptr) return false;
        //设置属性名称
        QString line = _reader->previousLine().trimmed();
        solid->setDataObjectName(line);
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
        if (lines.size() < 3)
        {
            _reader->consoleMessage(3, QString("Invalid Prop : %1.").arg(solid->getDataObjectName()));
            return false;
        }
        //读取属性参数
        this->readParam(lines);
        //属性对象赋值
        this->setPropData(solid);
        return true;
    }

    void FITKRadiossAdaptorPropSolid::readParam(const QStringList & lines)
    {
        if (lines.size() < 3) return;
        //第一行参数
        QString par0 = lines[0];
        bool ok = false;
        for (int i = 0; i < 8; ++i) 
        {
            int par = par0.mid(10 * i, 10).trimmed().toInt(&ok);
            if (ok) _solidPar.insert(i, par);
        }
        double dn = lines[0].mid(80, 20).trimmed().toDouble(&ok);
        if(ok) _solidPar.insert(8, dn);
        //第二行参数
        QString par1 = lines[1];
        for (int i = 0; i < 5; ++i)
        {
            double par = par1.mid(20 * i, 20).trimmed().toDouble(&ok);
            if (ok)
                _solidPar.insert(i + 10, par);
        }
        //第三行参数
        QStringList par3 = lines[2].simplified().split(' ');
        double tmin = par3[0].toDouble(&ok);
        if (ok) _solidPar.insert(20, tmin);
        //第四行参数
        if (lines.size() == 4)
        {
            QStringList par4 = lines[3].simplified().split(' ');
            int ndir = par4[0].toInt(&ok);
            if (ok) _solidPar.insert(30, ndir);
            int sphID = par4[1].toInt(&ok);
            if (ok) _solidPar.insert(31, sphID);
        }
    }

    void FITKRadiossAdaptorPropSolid::setPropData(FITKPropSolid * solid)
    {
        if (solid == nullptr) return;

        FITKPropSolidValue* solidValue = solid->getValue();
        // SolidValue 参数设置
        solidValue->setIsolid(_solidPar.value(0, solidValue->getIsolid()));
        solidValue->setIsmstr(_solidPar.value(1, solidValue->getIsmstr()));
        solidValue->setIcpre(_solidPar.value(3, solidValue->getIcpre()));
        solidValue->setItetra4(_solidPar.value(4, solidValue->getItetra4()));
        solidValue->setItetra10(_solidPar.value(5, solidValue->getItetra10()));
        solidValue->setImas(_solidPar.value(6, solidValue->getImas()));
        solidValue->setIFrame(_solidPar.value(7, solidValue->getIFrame()));
        // Solid 参数设置
        solid->setDn(_solidPar.value(8, solid->getDn()));
        solid->setQa(_solidPar.value(10, solid->getQa()));
        solid->setQb(_solidPar.value(11, solid->getQb()));
        solid->setH(_solidPar.value(12, solid->getH()));
        solid->setLambdaV(_solidPar.value(13, solid->getLambdaV()));
        solid->setMuV(_solidPar.value(14, solid->getMuV()));
        solid->setMinTimeStep(_solidPar.value(20, solid->getMinTimeStep()));
        solid->setNdir(_solidPar.value(30, solid->getNdir()));
        solid->setSPHPartID(_solidPar.value(31, solid->getSPHPartID()));
    }
}


