/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossAdaptorPropShell.h"

#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKPropShell.h"

#include "FITKRadiossRadReader.h"
#include "FITKRadiossRadWriter.h"
#include "RadKeyLineSpliter.h"
#include "RadReadInformationMapper.h"
#include "RadWriteInformationMapper.h"

#include <QList>

namespace Radioss
{
    QString FITKRadiossAdaptorPropShell::getAdaptorClass()
    {
        return "Radioss::FITKRadiossAdaptorPropShell";
    }

    bool FITKRadiossAdaptorPropShell::adaptR()
    {
        if (_caseData == nullptr || _reader == nullptr) return false;

        Radioss::RadReadInformationMapper *mapper = _reader->getReadRadInformationMapper();
        if (!mapper) return false;

        // 读取关键字
        RadKeyLineSpliter *keyLineSpliter = _reader->getKeyLineSpliter();
        if (!keyLineSpliter) return false;
        int pid = keyLineSpliter->getId();
        QString keyLine = keyLineSpliter->getCurrentLine();
        //获取shell
        FITKPropShell* shell = new FITKPropShell();
        if (shell == nullptr) return false;
        
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
            ok = this->readPropShell(shell);
        }
        if (!ok)
        {
            _reader->consoleMessage(1, QString("Read Prop Error : %1 %2").arg(keyLine).arg(shell->getDataObjectName()));
            return false;
        }
        _reader->consoleMessage(1, QString( "Read Prop : %1 %2").arg(keyLine).arg(shell->getDataObjectName()));
        _caseData->getSectionManager()->appendDataObj(shell);

        mapper->insertPropertyIDMap(pid, shell->getDataObjectID());
        return true;
    }

    bool FITKRadiossAdaptorPropShell::adaptW()
    {
        FITKPropShell* shell = dynamic_cast<FITKPropShell*>(_dataObj);
        if ( shell== nullptr || _writer == nullptr) return false;
        FITKPropShellValue* shellValue = shell->getValue();
        if (!shellValue) return false;

        QTextStream* stream = _writer->getStream();
        RadWriteInformationMapper* infoMapper = _writer->getWriteInpInformationMapper();
        if (stream == nullptr || infoMapper == nullptr) return false;
        //写出关键字行  ID 从1开始递增
        int radID = infoMapper->getMaxRadPropertyID() + 1;
        //建立映射关系
        infoMapper->insertPropertyIDMap(shell->getDataObjectID(), radID);
        *stream << shell->GetFITKPropShellRadiossKeyWord() << "/" << radID << endl;
        //写出属性名称
        *stream << shell->getDataObjectName() << endl;

        *stream << QString("#%1%2%3%4%5%6").arg("Ishell", 9).arg("Ismstr", 10).arg("Ish3n", 10).
            arg("Idril", 10).arg("Ipinch", 10).arg("P_thickFail", 30) << endl;
        *stream << gapStr(shellValue->getIshell(),10,false) << gapStr(shellValue->getIsmstr(), 10, false) << gapStr(shellValue->getIsh3n(), 10, false)
            << gapStr(shellValue->getIdril(), 10, false) << gapStr(shell->getIpinch(), 10, false) << gapStr(shell->getPthickFail(),30) << endl;

        *stream << QString("#%1%2%3%4%5").arg("Hm", 19).arg("Hf", 20).arg("Hr", 20).arg("Dm", 20).arg("Dn", 20) << endl;
        *stream << gapStr(shell->getHm()) << gapStr(shell->getHf()) << gapStr(shell->getHr())
            << gapStr(shell->getDm()) << gapStr(shell->getDn()) << endl;

        *stream << QString("#%1%2%3%4%5").arg("N", 9).arg("Thick", 30).arg("Ashear", 20).arg("Ithick", 20).arg("Iplas", 10) << endl;
        *stream << gapStr(shell->getN(),10,false) << gapStr(shell->getThick(),30) << gapStr(shell->getAshear()) 
            << gapStr(shellValue->getIthick(),20,false) << gapStr(shellValue->getIplas(),10,false) << endl;

        return true;
    }

    bool FITKRadiossAdaptorPropShell::readPropShell(FITKPropShell * shell)
    {
        if (shell == nullptr || _reader == nullptr) return false;
        //设置属性名称
        QString line = _reader->previousLine().trimmed();
        shell->setDataObjectName(line);
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
        if (lines.size() != 3)
        {
            _reader->consoleMessage(3, QString("Invalid Prop : %1.").arg(shell->getDataObjectName()));
            return false;
        }
        //读取属性参数
        this->readParam(lines);
        //属性对象赋值
        this->setPropData(shell);
        return true;
    }

    void FITKRadiossAdaptorPropShell::readParam(const QStringList & lines)
    {
        if (lines.size() != 3) return;
        //第一行参数
        QString par0 = lines[0];
        bool ok = false;
        for (int i = 0; i < 5; ++i) 
        {
            int par = par0.mid(10 * i, 10).trimmed().toInt(&ok);
            if (ok) _shellPar.insert(i, par);
        }
        double dn = lines[0].mid(60, 20).trimmed().toDouble(&ok);
        if(ok) _shellPar.insert(5, dn);
        //第二行参数
        QString par1 = lines[1];
        for (int i = 0; i < 5; ++i)
        {
            double par = par1.mid(20 * i, 20).trimmed().toDouble(&ok);
            if (ok)
                _shellPar.insert(i + 10, par);
        }
        //第三行参数
        QString par2 = lines[2];
        for (int i = 0; i < 5; ++i)
        {
            double par = par2.mid(20 * i, 20).trimmed().toDouble(&ok);
            if (ok)
                _shellPar.insert(i + 20, par);
        }

    }

    void FITKRadiossAdaptorPropShell::setPropData(FITKPropShell * shell)
    {
        if (shell == nullptr) return;

        FITKPropShellValue* shellValue = shell->getValue();
        // ShellValue 参数设置
        shellValue->setIshell(_shellPar.value(0, shellValue->getIshell()));
        shellValue->setIsmstr(_shellPar.value(1, shellValue->getIsmstr()));
        shellValue->setIthick(_shellPar.value(23, shellValue->getIthick()));
        shellValue->setIplas(_shellPar.value(24, shellValue->getIplas()));
        shellValue->setIsh3n(_shellPar.value(2, shellValue->getIsh3n()));
        shellValue->setIdril(_shellPar.value(3, shellValue->getIdril()));
        // Shell 参数设置
        shell->setIpinch(_shellPar.value(4, shell->getIpinch()));
        shell->setPthickFail(_shellPar.value(5, shell->getPthickFail()));
        shell->setHm(_shellPar.value(10, shell->getHm()));
        shell->setHf(_shellPar.value(11, shell->getHf()));
        shell->setHr(_shellPar.value(12, shell->getHr()));
        shell->setDm(_shellPar.value(13, shell->getDm()));
        shell->setDn(_shellPar.value(14, shell->getDn()));
        shell->setN(_shellPar.value(20, shell->getN()));
        shell->setThick(_shellPar.value(21, shell->getThick()));
        shell->setAshear(_shellPar.value(22, shell->getAshear()));
    }
}


