/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossAdaptorPropShellSandwich.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKPropSandwichShell.h"

#include "FITKRadiossRadReader.h"
#include "FITKRadiossRadWriter.h"
#include "RadKeyLineSpliter.h"
#include "RadReadInformationMapper.h"
#include "RadWriteInformationMapper.h"

namespace Radioss
{
    QString Radioss::FITKRadiossAdaptorPropShellSandwich::getAdaptorClass()
    {
        //返回数据类名 带命名空间
        return "Radioss::FITKRadiossAdaptorPropShellSandwich";
    }

    bool FITKRadiossAdaptorPropShellSandwich::adaptR()
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
        FITKPropSandwichShell* shell = new FITKPropSandwichShell();
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
            //读取属性数据
            ok = this->readProp(shell);
        }
        if (!ok)
        {
            _reader->consoleMessage(1, QString("Read Prop Error : %1 %2").arg(keyLine).arg(shell->getDataObjectName()));
            return false;
        }
        _reader->consoleMessage(1, QString("Read Prop : %1 %2").arg(keyLine).arg(shell->getDataObjectName()));
        _caseData->getSectionManager()->appendDataObj(shell);

        mapper->insertPropertyIDMap(pid, shell->getDataObjectID());
        return true;
    }

    bool FITKRadiossAdaptorPropShellSandwich::adaptW()
    {
        FITKPropSandwichShell* shell = dynamic_cast<FITKPropSandwichShell*>(_dataObj);
        if (shell == nullptr || _writer == nullptr) return false;
        FITKPropSandwichShellValue* shellValue = shell->getValue();
        if (!shellValue) return false;

        QTextStream* stream = _writer->getStream();
        RadWriteInformationMapper* infoMapper = _writer->getWriteInpInformationMapper();
        if (stream == nullptr || infoMapper == nullptr) return false;
        //写出关键字行  ID 从1开始递增
        int radID = infoMapper->getMaxRadPropertyID() + 1;
        //建立映射关系
        infoMapper->insertPropertyIDMap(shell->getDataObjectID(), radID);
        *stream << shell->GetFITKPropSandwichShellRadiossKeyWord() << "/" << radID << endl;
        //写出属性名称
        *stream << shell->getDataObjectName() << endl;

        //Ishell Ismstr Ish3n	Idril	  	 	P_thickfail	
        *stream << QString("#%1%2%3%4%5").arg("Ishell", 9).arg("Ismstr", 10).arg("Ish3n", 10).
            arg("Idril", 10).arg("P_thickfail", 40) << endl;
        *stream << gapStr(shellValue->getIshell(), 10, false) << gapStr(shellValue->getIsmstr(), 10, false) << gapStr(shellValue->getIsh3n(), 10, false)
            << gapStr(shellValue->getIdril(), 10, false) << gapStr(shellValue->getPThickfail(), 40) << endl;
        
        //hm	hf	hr	dm	dn
        *stream << QString("#%1%2%3%4%5").arg("hm", 19).arg("hf", 20).arg("hr", 20).
            arg("dm", 20).arg("dn", 20) << endl;
        *stream << gapStr(shellValue->getHm()) << gapStr(shellValue->getHf()) << gapStr(shellValue->getHr())
            << gapStr(shellValue->getDm()) << gapStr(shellValue->getDn()) << endl;
        //N	Istrain	Thick	Ashear	 	Ithick	Iplas
        int layNum = shellValue->getN();
        *stream << QString("#%1%2%3%4%5%6").arg("N", 9).arg("Istrain", 10).arg("Thick", 20).
            arg("Ashear", 20).arg("Ithick", 20).arg("Iplas", 10) << endl;
        *stream << gapStr(layNum, 10, false) << gapStr(shellValue->getIstrain(), 10, false)
            << gapStr(shellValue->getThick()) << gapStr(shellValue->getAshear())
            << gapStr(shellValue->getIthick(), 20, false) << gapStr(shellValue->getIplas(), 10, false) << endl;
        //Vx	Vy	Vz	Skew_ID	Iorth	Ipos
        *stream << QString("#%1%2%3%4%5%6").arg("Vx", 19).arg("Vy", 20).arg("Vz", 20).
            arg("Skew_ID", 10).arg("Iorth", 10).arg("Ipos", 10) << endl;
        *stream << gapStr(shellValue->getVx()) << gapStr(shellValue->getVy()) << gapStr(shellValue->getVz())
            << gapStr(shellValue->getSkewID()) << gapStr(shellValue->getIorth(), 10, false)
            << gapStr(shellValue->getIpos(), 10, false) << endl;
        //各层参数 Phi  t	Z	mat_ID	 	 	F_weight
        *stream << QString("#%1%2%3%4%5").arg("Phi", 19).arg("t", 20).arg("Z", 20).
            arg("mat_ID", 10).arg("F_weight", 30) << endl;
        for (int i = 0; i < layNum; ++i)
        {
            SandwichShellLayerValue layerValue = shellValue->getLayerValue(i);
            *stream << gapStr(layerValue._angle) << gapStr(layerValue._thickness) << gapStr(layerValue._positionZ)
                << gapStr(infoMapper->getRadIDByMaterialID(layerValue._matID)) << gapStr(layerValue._relFWeight, 30, false) << endl;
        }
        
        return true;
    }

    bool FITKRadiossAdaptorPropShellSandwich::readProp(FITKPropSandwichShell * shell)
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
        if (lines.size() < 4)
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
    void FITKRadiossAdaptorPropShellSandwich::readParam(const QStringList & lines)
    {
        if (lines.size() <4  || _reader == nullptr) return;
        //Ishell Ismstr Ish3n	Idril	  	 	P_thickfail	
        QList<QPair<QString, int>> propPar = { {"Ishell", 10}, {"Ismstr", 10},
            {"Ish3n", 10}, {"Idril", 10}, {"P_thickfail", 40} };
        splitParam(lines[0], propPar);
        //hm	hf	hr	dm	dn
        propPar = { {"hm", 20}, {"hf", 20},{"hr", 20 }, { "dm", 20 }, { "dn", 20 } };
        splitParam(lines[1], propPar);
        //N	Istrain	Thick	Ashear	 	Ithick	Iplas
        propPar = { {"N", 10}, {"Istrain", 10},{"Thick", 20 }, { "Ashear", 20 },
            { "Ithick", 20 }, { "Iplas", 10 } };
        splitParam(lines[2], propPar);
        //Vx	Vy	Vz	Skew_ID	Iorth	Ipos
        propPar = { {"Vx", 20}, {"Vy", 20},{"Vz", 20 }, { "Skew_ID", 10 },
            { "Iorth", 10 }, { "Ipos", 10 } };
        splitParam(lines[3], propPar);

        int layNum = _propParam.value("N", 1);
        if (lines.size() != layNum + 4)
            return;
        //各层参数 Phi  t	Z	mat_ID	 	 	F_weight
        for (int i = 0; i < layNum;++i) 
        {
            QString name_i = QString::number(i + 1);
            propPar = { {QString::number(i * 10), 20}, {QString::number(i * 10+1), 20},
                {QString::number(i * 10 + 2), 20 }, { QString::number(i * 10 + 3), 10 }
                ,{ QString::number(i * 10 + 4), 30 } };
            splitParam(lines[4+i], propPar);
        }

    }

    void FITKRadiossAdaptorPropShellSandwich::splitParam(const QString & line, const QList<QPair<QString, int>>& par)
    {
        //字符串分割位置
        int split = 0;
        for (auto it = par.begin(); it != par.end(); ++it)
        {
            bool ok = false;
            const QString& parKey = it->first;
            int width = it->second;
            double parVal = line.mid(split, width).simplified().toDouble(&ok);
            //如果转double为空值，则跳过
            if (ok)
                _propParam.insert(parKey, parVal);
            split += width;
        }
    }

    void FITKRadiossAdaptorPropShellSandwich::setPropData(FITKPropSandwichShell * prop)
    {
        if (prop == nullptr) return;
        FITKPropSandwichShellValue* value = prop->getValue();

        //Ishell Ismstr Ish3n	Idril	  	 	P_thickfail
        value->setIshell(_propParam.value("Ishell", value->getIshell()));
        value->setIsmstr(_propParam.value("Ismstr", value->getIsmstr()));
        value->setIsh3n(_propParam.value("Ish3n", value->getIsh3n()));
        value->setIdril(_propParam.value("Idril", value->getIdril()));
        value->setPThickfail(_propParam.value("P_thickfail", value->getPThickfail()));
        //hm	hf	hr	dm	dn
        value->setHm(_propParam.value("hm", value->getHm()));
        value->setHf(_propParam.value("hf", value->getHf()));
        value->setHr(_propParam.value("hr", value->getHr()));
        value->setDm(_propParam.value("dm", value->getDm()));
        value->setDn(_propParam.value("dn", value->getDn()));
        //N	Istrain	Thick	Ashear	 	Ithick	Iplas
        value->setN(_propParam.value("N", value->getN()));
        value->setIstrain(_propParam.value("Istrain", value->getIstrain()));
        value->setThick(_propParam.value("Thick", value->getThick()));
        value->setAshear(_propParam.value("Ashear", value->getAshear()));
        value->setIthick(_propParam.value("Ithick", value->getIthick()));
        value->setIplas(_propParam.value("Iplas", value->getIplas()));
        //Vx	Vy	Vz	Skew_ID	Iorth	Ipos
        value->setVx(_propParam.value("Vx", value->getVx()));
        value->setVy(_propParam.value("Vy", value->getVy()));
        value->setVz(_propParam.value("Vz", value->getVz()));
        value->setSkewID(_propParam.value("Skew_ID", value->getSkewID()));
        value->setIorth(_propParam.value("Iorth", value->getIorth()));
        value->setIpos(_propParam.value("Ipos", value->getIpos()));

        //各层参数 Phi  t	Z	mat_ID	 	 	F_weight
        int layNum = value->getN();
        for (int i = 0; i < layNum; ++i)
        {
            SandwichShellLayerValue layV;
            layV._angle = _propParam.value(QString::number(i * 10), layV._angle);
            layV._thickness= _propParam.value(QString::number(i * 10+1), layV._thickness);
            layV._positionZ = _propParam.value(QString::number(i * 10+2), layV._positionZ);
            layV._matID = _propParam.value(QString::number(i * 10+3), layV._matID);
            layV._relFWeight = _propParam.value(QString::number(i * 10+4), layV._relFWeight);

            value->setLayerValue(layV,i);
        }
    }
}