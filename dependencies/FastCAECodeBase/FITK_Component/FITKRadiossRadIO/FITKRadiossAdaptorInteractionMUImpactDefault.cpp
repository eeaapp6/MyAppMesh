/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossAdaptorInteractionMUImpactDefault.h"
#include "FITK_Component/FITKRadiossData/FITKInteractionMultiUsageImpact.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"

#include "FITKRadiossRadReader.h"
#include "FITKRadiossRadWriter.h"
#include "RadKeyLineSpliter.h"

namespace Radioss
{
    QString FITKRadiossAdaptorInteractionMUImpactDefault::getAdaptorClass()
    {
        return "Radioss::FITKRadiossAdaptorInteractionMUImpactDefault";
    }

    bool FITKRadiossAdaptorInteractionMUImpactDefault::adaptR()
    {
        if (_caseData == nullptr || _reader == nullptr) return false;

        // 读取关键字
        RadKeyLineSpliter* keyLineSpliter = _reader->getKeyLineSpliter();
        if (!keyLineSpliter) return false;
        int pid = keyLineSpliter->getId();
        QString keyLine = keyLineSpliter->getCurrentLine();

        FITKInteractionMultiUsageImpactDefaultValue* Val = new FITKInteractionMultiUsageImpactDefaultValue();
        if (Val == nullptr) return false;

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
            //读取默认数据
            ok = this->readDefMultiUsageImpact(Val);
        }
        if (!ok)
        {
            _reader->consoleMessage(3, QString("Read Default MultiUsageImpact Error : %1").arg(keyLine));
            return false;
        }
        _reader->consoleMessage(1, QString("Read Default MultiUsageImpact : %1").arg(keyLine));
        _caseData->getDefaultInteractionManager()->appendDataObj(Val);
        return true;
    }

    bool FITKRadiossAdaptorInteractionMUImpactDefault::adaptW()
    {
        FITKInteractionMultiUsageImpactDefaultValue* val = dynamic_cast<FITKInteractionMultiUsageImpactDefaultValue*>(_dataObj);
        if (val == nullptr || _writer == nullptr) return false;

        QTextStream* stream = _writer->getStream();
        if (stream == nullptr) return false;

        *stream << val->GetFITKInteractionMultiUsageImpactDefaultValueRadiossKeyWord() << endl;

        // 写出默认数据
        *stream << endl;
        *stream << QString("#%1%2%3%4").arg("Istf", 29).arg("Igap", 20)
            .arg("Ibag", 20).arg("Idel", 10) << endl;
        *stream << gapStr(val->_Istf, 30, false) << gapStr(val->_Igap, 20, false)
            << gapStr(val->_Ibag, 20, false) << gapStr(val->_Idel, 10, false) << endl;

        *stream << endl;
        *stream << QString("#%1%2").arg("Irem_gap", 89).arg("Irem_i2", 10)<< endl;
        *stream << gapStr(val->_Irem_gap, 90, false) << gapStr(val->_Irem_i2, 10, false) << endl;

        *stream << endl;
        *stream << QString("#%1").arg("Inacti", 39) << endl;
        *stream << gapStr(val->_Inacti, 40, false) << endl;

        *stream << QString("#%1").arg("Iform", 49) << endl;
        *stream << gapStr(val->_Iform, 50, false) << endl;

        return true;
    }

    bool FITKRadiossAdaptorInteractionMUImpactDefault::readDefMultiUsageImpact(FITKInteractionMultiUsageImpactDefaultValue * MUImpact)
    {
        if (MUImpact == nullptr || _reader == nullptr) return false;
        //参数存储
        QStringList lines;

        while (!_reader->atEnd())
        {
            QString line = _reader->readLine();
            if (line.startsWith("#")) continue;
            if (line.startsWith("/"))
            {
                _reader->backLine();
                break;
            }
            lines.append(line);
        }
        //与手册说明不符
        if (lines.size() !=4 )
        {
            _reader->consoleMessage(3, "Invalid Default : /DEFAULT/INTER/TYPE7");
            return false;
        }
        //读取默认参数、赋值
        this->readParam(lines, MUImpact);
        return true;
    }

    bool FITKRadiossAdaptorInteractionMUImpactDefault::readParam(const QStringList & lines, FITKInteractionMultiUsageImpactDefaultValue * MUImpact)
    {
        if (lines.size() !=4 || MUImpact == nullptr) return false;
        bool ok = false;

        QString line = lines[0];
        int Istf = line.mid(20, 10).trimmed().toInt(&ok);
        if (ok) MUImpact->_Istf = Istf;
        int Igap = line.mid(40, 10).trimmed().toInt(&ok);
        if (ok) MUImpact->_Igap = Igap;
        int Ibag = line.mid(60, 10).trimmed().toInt(&ok);
        if (ok) MUImpact->_Ibag = Ibag;
        int Idel = line.mid(70, 10).trimmed().toInt(&ok);
        if (ok) MUImpact->_Idel = Idel;

        line = lines.value(1);
        int Irem_gap = line.mid(80, 10).trimmed().toInt(&ok);
        if (ok) MUImpact->_Irem_gap = Irem_gap;
        int Irem_i2 = line.mid(90, 10).trimmed().toInt(&ok);
        if (ok) MUImpact->_Irem_i2 = Irem_i2;

        line = lines.value(2);
        int Inacti = line.mid(30, 10).trimmed().toInt(&ok);
        if (ok) MUImpact->_Inacti = Inacti;

        line = lines.value(3);
        int Iform = line.mid(40, 10).trimmed().toInt(&ok);
        if (ok) MUImpact->_Iform = Iform;

        return true;
    }
}