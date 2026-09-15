/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossAdaptorBEMDAA.h"

#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKBEMDoubleAsymptoticApproximation.h"

#include "FITKRadiossRadReader.h"
#include "FITKRadiossRadWriter.h"
#include "RadKeyLineSpliter.h"
#include "RadReadInformationMapper.h"
#include "RadWriteInformationMapper.h"

namespace Radioss
{
    QString FITKRadiossAdaptorBEMDAA::getAdaptorClass()
    {
        return "Radioss::FITKRadiossAdaptorBEMDAA";
    }

    bool FITKRadiossAdaptorBEMDAA::adaptR()
    {
        if (_caseData == nullptr || _reader == nullptr) return false;

        FITKBEMDoubleAsymptoticApproximation* daa = new FITKBEMDoubleAsymptoticApproximation();

        RadKeyLineSpliter* keyLineSpliter = _reader->getKeyLineSpliter();
        RadReadInformationMapper* infoMapper = _reader->getReadRadInformationMapper();
        if (!keyLineSpliter || !infoMapper) return false;

        int daaId = keyLineSpliter->getId(0);
        QString keyLine = keyLineSpliter->getCurrentLine();

        QString line = _reader->readLine().trimmed();
        if (!line.isEmpty() && !line.startsWith("#") && !line.startsWith("/")) {
            daa->setDataObjectName(line);
        }

        QStringList lines;
        while (_reader && !_reader->atEnd()) {
            QString line = _reader->readLine();
            if (line.startsWith("/")) {
                _reader->backLine();
                break;
            }
            lines.append(line);
        }

        if (lines.isEmpty()) {
            _reader->consoleMessage(3, QString("Read BEM/DAA Error : %1 %2").arg(keyLine).arg(daa->getDataObjectName()));
            return false;
        }

        bool ok = this->readParam(lines, daa);
        if (!ok) {
            _reader->consoleMessage(3, QString("Read BEM/DAA Error : %1 %2").arg(keyLine).arg(daa->getDataObjectName()));
            return false;
        }

        _reader->consoleMessage(1, QString("Read BEM/DAA : %1 %2").arg(keyLine).arg(daa->getDataObjectName()));
        _caseData->getBEMManager()->appendDataObj(daa);
        infoMapper->insertBEMIDMap(daaId, daa->getDataObjectID());

        return true;
    }

    bool FITKRadiossAdaptorBEMDAA::adaptW()
    {
        FITKBEMDoubleAsymptoticApproximation* daa = dynamic_cast<FITKBEMDoubleAsymptoticApproximation*>(_dataObj);
        if (daa == nullptr || _writer == nullptr) return false;

        QTextStream* stream = _writer->getStream();
        RadWriteInformationMapper* infoMapper = _writer->getWriteInpInformationMapper();
        if (stream == nullptr || infoMapper == nullptr) return false;

        int radID = infoMapper->getMaxRadBEMID() + 1;
        infoMapper->insertBEMIDMap(daa->getDataObjectID(), radID);

        *stream << "/BEM/DAA" << "/" << radID << endl;
        *stream << daa->getDataObjectName() << endl;

        *stream << "#     Isur   Grav-id" << endl;
        *stream << gapStr(infoMapper->getRadIDBySurfaceID(daa->getSurfId()), 10) << gapStr(infoMapper->getRadIDByGravityID(daa->getGravId()), 10) << endl;

        *stream << "#                rho                 ssp" << endl;
        *stream << gapStr(daa->getRho(), 20) << gapStr(daa->getC(), 20) << endl;

        *stream << "#                 xs                  ys                  zs" << endl;
        *stream << gapStr(daa->getXs(), 20) << gapStr(daa->getYs(), 20) << gapStr(daa->getZs(), 20) << endl;

        *stream << "#    Iform      Ipri     Ipres                         Kform  Freesurf Afterflow    Integr" << endl;
        *stream << gapStr(daa->getIform(), 10) << gapStr(daa->getIpri(), 10) << gapStr(daa->getIpres(), 10)
               << gapStr(daa->getKform(), 30) << gapStr(daa->getFreesurf(), 10) << gapStr(daa->getAfterflow(), 10) << gapStr(daa->getIntegr(), 10) << endl;

        if (daa->getIform() == 0) {
            *stream << "#               Pmax               Theta" << endl;
            *stream << gapStr(daa->getPm(), 20) << gapStr(daa->getTheta(), 20) << endl;
        }

        *stream << "#                 xc                  yc                  zc" << endl;
        *stream << gapStr(daa->getXc(), 20) << gapStr(daa->getYc(), 20) << gapStr(daa->getZc(), 20) << endl;

        *stream << "#                 xa                  ya                  za" << endl;
        *stream << gapStr(daa->getXa(), 20) << gapStr(daa->getYa(), 20) << gapStr(daa->getZa(), 20) << endl;

        *stream << "#               dirx                diry                dirz" << endl;
        *stream << gapStr(daa->getDirX(), 20) << gapStr(daa->getDirY(), 20) << gapStr(daa->getDirZ(), 20) << endl;

        return true;
    }

    bool FITKRadiossAdaptorBEMDAA::readParam(const QStringList& lines, FITKBEMDoubleAsymptoticApproximation* daa)
    {
        if (lines.isEmpty() || daa == nullptr) return false;

        QVector<QStringList> paramGroups;
        QList<int> nonEmptyIndices;

        for (int i = 0; i < lines.size(); ++i) {
            QString line = lines[i].trimmed();
            if (line.isEmpty() || line.startsWith("#")) continue;
            QStringList tokens = line.simplified().split(" ", Qt::SkipEmptyParts);
            if (!tokens.isEmpty()) {
                paramGroups.append(tokens);
                nonEmptyIndices.append(i);
            }
        }

        if (paramGroups.size() >= 1) {
            QStringList& tokens = paramGroups[0];
            if (tokens.size() >= 1) daa->setSurfId(tokens[0].toInt());
            if (tokens.size() >= 2) daa->setGravId(tokens[1].toInt());
        }

        if (paramGroups.size() >= 2) {
            QStringList& tokens = paramGroups[1];
            if (tokens.size() >= 1) daa->setRho(tokens[0].toDouble());
            if (tokens.size() >= 2) daa->setC(tokens[1].toDouble());
        }

        if (paramGroups.size() >= 3) {
            QStringList& tokens = paramGroups[2];
            if (tokens.size() >= 1) daa->setXs(tokens[0].toDouble());
            if (tokens.size() >= 2) daa->setYs(tokens[1].toDouble());
            if (tokens.size() >= 3) daa->setZs(tokens[2].toDouble());
        }

        if (paramGroups.size() >= 4) {
            QStringList& tokens = paramGroups[3];
            if (tokens.size() >= 1) daa->setIform(tokens[0].toInt());
            if (tokens.size() >= 2) daa->setIpri(tokens[1].toInt());
            if (tokens.size() >= 3) daa->setIpres(tokens[2].toInt());
            if (tokens.size() >= 4) daa->setKform(tokens[3].toInt());
            if (tokens.size() >= 5) daa->setFreesurf(tokens[4].toInt());
            if (tokens.size() >= 6) daa->setAfterflow(tokens[5].toInt());
            if (tokens.size() >= 7) daa->setIntegr(tokens[6].toInt());
        }

        if (paramGroups.size() >= 5) {
            QStringList& tokens = paramGroups[4];
            if (tokens.size() >= 1) daa->setPm(tokens[0].toDouble());
            if (tokens.size() >= 2) daa->setTheta(tokens[1].toDouble());
        }

        if (paramGroups.size() >= 6) {
            QStringList& tokens = paramGroups[5];
            if (tokens.size() >= 1) daa->setXc(tokens[0].toDouble());
            if (tokens.size() >= 2) daa->setYc(tokens[1].toDouble());
            if (tokens.size() >= 3) daa->setZc(tokens[2].toDouble());
        }

        if (paramGroups.size() >= 7) {
            QStringList& tokens = paramGroups[6];
            if (tokens.size() >= 1) daa->setXa(tokens[0].toDouble());
            if (tokens.size() >= 2) daa->setYa(tokens[1].toDouble());
            if (tokens.size() >= 3) daa->setZa(tokens[2].toDouble());
        }

        if (paramGroups.size() >= 8) {
            QStringList& tokens = paramGroups[7];
            if (tokens.size() >= 1) daa->setDirX(tokens[0].toDouble());
            if (tokens.size() >= 2) daa->setDirY(tokens[1].toDouble());
            if (tokens.size() >= 3) daa->setDirZ(tokens[2].toDouble());
        }

        return true;
    }
}
