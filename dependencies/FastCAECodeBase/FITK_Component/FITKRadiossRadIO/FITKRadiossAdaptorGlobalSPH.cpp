/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossAdaptorGlobalSPH.h"

#include "FITK_Component/FITKRadiossData/FITKRadiossSolverSettings.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"

#include "FITKRadiossRadReader.h"
#include "FITKRadiossRadWriter.h"
#include "RadKeyLineSpliter.h"

namespace Radioss
{
    QString FITKRadiossAdaptorGlobalSPH::getAdaptorClass()
    {
        return "Radioss::FITKRadiossAdaptorGlobalSPH";
    }

    bool FITKRadiossAdaptorGlobalSPH::adaptR()
    {
        if (_caseData == nullptr || _reader == nullptr) return false;

        // 读取关键字
        RadKeyLineSpliter* keyLineSpliter = _reader->getKeyLineSpliter();
        if (!keyLineSpliter) return false;
        int pid = keyLineSpliter->getId();
        QString keyLine = keyLineSpliter->getCurrentLine();

        FITKRadiossSolverSettings * solverSetting = _caseData->getSolverSettings();
        if (!solverSetting) return false;
        FITKRadiossSolverSPHGlobal * sph = solverSetting->getGlobalSPHParam();
        if (!sph) return false;
        sph->enable(true);
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
            ok = this->readGlobalSPH(sph);
        }
        if (!ok)
        {
            _reader->consoleMessage(3, QString("Read SPH Global : %1").arg(keyLine));
            return false;
        }
        _reader->consoleMessage(1, QString("Read SPH Global: %1").arg(keyLine));

        return true;
    }

    bool FITKRadiossAdaptorGlobalSPH::adaptW()
    {
        FITKRadiossSolverSPHGlobal* sph = dynamic_cast<FITKRadiossSolverSPHGlobal*>(_dataObj);
        if (sph == nullptr || _writer == nullptr) return false;

        QTextStream* stream = _writer->getStream();
        if (stream == nullptr) return false;

        *stream << sph->GetFITKRadiossSolverSPHGlobalRadiossKeyWord() << endl;

        // 写出数据
        *stream << QString("#%1%2%3%4%5").arg("Alpha_sort", 19).arg("Maxsph", 10)
            .arg("Lneigh", 10).arg("Nneigh", 10).arg("Isol2sph", 10) << endl;
        *stream << gapStr(sph->getAsort(), 20) << gapStr(sph->getMaxsph(), 10,false) << gapStr(sph->getLneigh(), 10, false)
            << gapStr(sph->getNneigh(), 10, false) << gapStr(sph->getIsol2sph(), 10, false) << endl;

        return true;
    }

    bool FITKRadiossAdaptorGlobalSPH::readGlobalSPH(FITKRadiossSolverSPHGlobal * sph)
    {
        if (sph == nullptr || _reader == nullptr) return false;
        //参数存储
        QStringList lines;
        _reader->backLine();
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
        if (lines.size() !=1)
        {
            _reader->consoleMessage(3, "Invalid SPH Global : /SPHGLO");
            return false;
        }
        //读取参数、赋值
        this->readParam(lines, sph);
        return true;
    }

    bool FITKRadiossAdaptorGlobalSPH::readParam(const QStringList & lines, FITKRadiossSolverSPHGlobal * sph)
    {
        if (lines.size() !=1 || sph == nullptr) return false;

        QString line = lines[0];
        bool ok = false;
        double Alpha_sort = line.mid(0, 20).trimmed().toDouble(&ok);
        if (ok) sph->setAsort(Alpha_sort);

        int Maxsph = line.mid(20, 10).trimmed().toInt(&ok);
        if (ok) sph->setMaxsph(Maxsph);

        int  Lneigh = line.mid(30, 10).trimmed().toInt(&ok);
        if (ok) sph->setLneigh(Lneigh);

        int Nneigh = line.mid(40, 10).trimmed().toInt(&ok);
        if (ok) sph->setNneigh(Nneigh);

        int Isol2sph = line.mid(50, 10).trimmed().toInt(&ok);
        if (ok) sph->setIsol2sph(Isol2sph);

        return true;
    }
}