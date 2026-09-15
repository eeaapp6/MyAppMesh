/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossAdaptorInteractionTiedDefault.h"
#include "FITK_Component/FITKRadiossData/FITKInteractionTied.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"

#include "FITKRadiossRadReader.h"
#include "FITKRadiossRadWriter.h"
#include "RadKeyLineSpliter.h"

namespace Radioss
{
    QString FITKRadiossAdaptorInteractionTiedDefault::getAdaptorClass()
    {
        return "Radioss::FITKInteractionKinematicTiedDefault";
    }
    bool FITKRadiossAdaptorInteractionTiedDefault::adaptR()
    {
        if (_caseData == nullptr || _reader == nullptr) return false;

        // 读取关键字
        RadKeyLineSpliter* keyLineSpliter = _reader->getKeyLineSpliter();
        if (!keyLineSpliter) return false;
        int pid = keyLineSpliter->getId();
        QString keyLine = keyLineSpliter->getCurrentLine();

        FITKInteractionKinematicTiedDefault* Val = new FITKInteractionKinematicTiedDefault();
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
            ok = this->readDefTie(Val);
        }
        if (!ok)
        {
            _reader->consoleMessage(3, QString("Read Default Tie Error : %1").arg(keyLine));
            return false;
        }
        _reader->consoleMessage(1, QString("Read Default Tie : %1").arg(keyLine));
        _caseData->getDefaultInteractionManager()->appendDataObj(Val);
        return true;
    }

    bool FITKRadiossAdaptorInteractionTiedDefault::adaptW()
    {
        FITKInteractionKinematicTiedDefault* val = dynamic_cast<FITKInteractionKinematicTiedDefault*>(_dataObj);
        if (val == nullptr || _writer == nullptr) return false;

        QTextStream* stream = _writer->getStream();
        if (stream == nullptr) return false;

        *stream << val->GetFITKInteractionKinematicTiedDefaultRadiossKeyWord() << endl;

        int spot = val->getSpotWeldFormulationFlag();
        // 写出默认绑定数据
        *stream << endl;
        *stream << QString("#%1%2%3%4").arg("Ignore", 29).arg("Spotflag", 10)
            .arg("Isearch", 20).arg("Idel2", 10)<< endl;
        *stream << gapStr(val->getIgnoreFlag(),30,false) << gapStr(spot, 10, false)
            << gapStr(val->getSearchFormulationFlag(), 20, false)<< gapStr(val->getNodeDeletionFlag(), 10, false) << endl;
        
        if (spot == 25 || spot == 27 || spot == 28) {
            *stream << QString("#%1").arg("Istf", 69) << endl;
            *stream << gapStr(val->getIstfFlagDefault(), 70, false) << endl;
        }
        return true;
    }

    bool FITKRadiossAdaptorInteractionTiedDefault::readDefTie(FITKInteractionKinematicTiedDefault* val)
    {
        if (val == nullptr || _reader == nullptr) return false;
        //参数存储
        QStringList lines;

        /****  由于_reader->readLine()会跳过空行，手动加一行空 ****/
        lines.append("");

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
        if (lines.size() < 2|| lines.size()>3)
        {
            _reader->consoleMessage(3, "Invalid Default : /DEFAULT/INTER/TYPE2");
            return false;
        }
        //读取默认固体属性参数、赋值
        this->readParam(lines, val);
        return true;
    }
    bool FITKRadiossAdaptorInteractionTiedDefault::readParam(const QStringList& lines, FITKInteractionKinematicTiedDefault* tie)
    {
        if (lines.size() <2 || tie == nullptr) return false;

        //第1行为空
        //第2行
        QString line = lines[1];
        QHash<int, int> shellValPar;

        bool ok = false;
        int Ignore = line.mid(20, 10).trimmed().toInt(&ok);
        if (ok) shellValPar.insert(0, Ignore);

        int Spotflag = line.mid(30, 10).trimmed().toInt(&ok);
        if (ok) shellValPar.insert(1, Spotflag);

        int Isearch = line.mid(50, 10).trimmed().toInt(&ok);
        if (ok) shellValPar.insert(2, Isearch);

        int Idel2 = line.mid(60, 10).trimmed().toInt(&ok);
        if (ok) shellValPar.insert(3, Idel2);
        //第3行
        line = lines.value(2);
        int Istf = line.mid(60, 10).trimmed().toInt(&ok);
        if (ok) shellValPar.insert(4, Istf);

        //获取对应参数值，如果不存在则为默认值
        tie->setIgnoreFlag(shellValPar.value(0, tie->getIgnoreFlag()));
        tie->setSpotWeldFormulationFlag(shellValPar.value(1, tie->getSpotWeldFormulationFlag()));
        tie->setSearchFormulationFlag(shellValPar.value(2, tie->getSearchFormulationFlag()));
        tie->setNodeDeletionFlag(shellValPar.value(3, tie->getNodeDeletionFlag()));
        tie->setIstfFlagDefault(shellValPar.value(4, tie->getIstfFlagDefault()));

        return true;
    }
}

