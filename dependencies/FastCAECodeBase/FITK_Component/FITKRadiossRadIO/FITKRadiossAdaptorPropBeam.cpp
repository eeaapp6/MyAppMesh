/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossAdaptorPropBeam.h"

#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKPropBeam.h"

#include "FITKRadiossRadReader.h"
#include "FITKRadiossRadWriter.h"
#include "RadKeyLineSpliter.h"
#include "RadReadInformationMapper.h"
#include "RadWriteInformationMapper.h"

namespace Radioss
{
    QString FITKRadiossAdaptorPropBeam::getAdaptorClass()
    {
        return "Radioss::FITKRadiossAdaptorPropBeam";
    }
    bool FITKRadiossAdaptorPropBeam::adaptR()
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
        FITKPropBeam* beam = new FITKPropBeam();
        if (beam == nullptr) return false;

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
            ok = this->readPropBeam(beam);
        }
        if (!ok)
        {
            _reader->consoleMessage(1, QString("Read Prop Error : %1 %2").arg(keyLine).arg(beam->getDataObjectName()));
            return false;
        }
        _reader->consoleMessage(1, QString("Read Prop : %1 %2").arg(keyLine).arg(beam->getDataObjectName()));
        _caseData->getSectionManager()->appendDataObj(beam);

        mapper->insertPropertyIDMap(pid, beam->getDataObjectID());
        return true;
    }

    bool FITKRadiossAdaptorPropBeam::adaptW()
    {
        FITKPropBeam* beam = dynamic_cast<FITKPropBeam*>(_dataObj);
        if (beam == nullptr || _writer == nullptr) return false;
        FITKPropValueBeam* value = beam->getValue();
        if (!value) return false;

        QTextStream* stream = _writer->getStream();
        RadWriteInformationMapper* infoMapper = _writer->getWriteInpInformationMapper();
        if (stream == nullptr || infoMapper == nullptr) return false;
        //写出关键字行  ID 从1开始递增
        int radID = infoMapper->getMaxRadPropertyID() + 1;
        //建立映射关系
        infoMapper->insertPropertyIDMap(beam->getDataObjectID(), radID);
        *stream << beam->GetFITKPropBeamRadiossKeyWord() << "/" << radID << endl;
        //写出属性名称
        *stream << beam->getDataObjectName() << endl;

        *stream << QString("#%1").arg("Ismstr", 19) << endl;
        *stream << gapStr(value->getSmallStrainOption(),20,false) << endl;

        *stream << QString("#%1%2").arg("dm", 19).arg("df", 20)<< endl;
        *stream << gapStr(value->getBeamMembraneDamping()) << gapStr(value->getBeamFlexuralDamping())<<endl;

        *stream << QString("#%1%2%3%4").arg("Area", 19).arg("Iyy", 20).arg("Izz", 20).arg("Ixx", 20) << endl;
        *stream << gapStr(value->getCrossSection())<<gapStr(value->getAreaMomentInertiaIyy())<<
            gapStr(value->getAreaMomentInertiaIzz())<<gapStr(value->getAreaMomentInertiaIxx())<< endl;

        //自由度
        QString DOF = QString("   %1%2%3 %4%5%6").arg(value->isRotationDOF1(0)).arg(value->isRotationDOF1(1))
            .arg(value->isRotationDOF1(2)).arg(value->isRotationDOF2(0)).arg(value->isRotationDOF2(1))
            .arg(value->isRotationDOF2(2));
        *stream << QString("#%1%2").arg("wDof", 9).arg("Ishear", 10) << endl;
        *stream << DOF << gapStr(value->isNeglectsShear()) << endl;

        return true;
    }

    bool FITKRadiossAdaptorPropBeam::readPropBeam(FITKPropBeam * beam)
    {
        if (beam == nullptr || _reader == nullptr) return false;
        //设置属性名称
        QString line = _reader->previousLine().trimmed();
        beam->setDataObjectName(line);
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
        if (lines.size() !=4)
        {
            _reader->consoleMessage(3, QString("Invalid Prop : %1.").arg(beam->getDataObjectName()));
            return false;
        }
        //读取属性参数并赋值
        this->readParam(lines, beam);

        return true;
    }
    void FITKRadiossAdaptorPropBeam::readParam(const QStringList & lines, Radioss::FITKPropBeam* beam)
    {
        if (lines.size() != 4||!beam) return;
        FITKPropValueBeam* beamV = beam->getValue();
        if (!beamV)return;

        //第一行
        QString par = lines[0];
        int Ismstr = par.mid(10 , 10).trimmed().toInt();
        bool ok = false;

        double beamPar[6] = { 0 };
        //第二行
        par = lines[1];
        for (int i = 0; i < 2; ++i){
            double parVal = par.mid(10 * i, 20).trimmed().toDouble(&ok);
            if (ok) beamPar[i] = parVal;
        }
        //第三行
        par = lines[2];
        for (int i = 0; i < 4; ++i){
            double parVal = par.mid(20 * i, 20).trimmed().toDouble(&ok);
            if (ok) beamPar[i+2] = parVal;
        }
        // 第四行
        par = lines[3];
        QStringList rList = par.simplified().split(" ");
        QString rotations1 = rList[0];
        QString rotations2 = rList[1];
        int Ishear = 0;
        if(rList.size()==3)
            Ishear = rList[2].toInt();
        bool x1 = QString(rotations1.at(0)).toInt();
        bool y1 = QString(rotations1.at(1)).toInt();
        bool z1 = QString(rotations1.at(2)).toInt();
        bool x2 = QString(rotations2.at(0)).toInt();
        bool y2 = QString(rotations2.at(1)).toInt();
        bool z2 = QString(rotations2.at(2)).toInt();

        //赋值
        beamV->setSmallStrainOption(Ismstr);
        beamV->setBeamMembraneDamping(beamPar[0]);
        beamV->setBeamFlexuralDamping(beamPar[1]);
        beamV->setCrossSection(beamPar[2]);
        beamV->setAreaMomentInertiaIyy(beamPar[3]);
        beamV->setAreaMomentInertiaIzz(beamPar[4]);
        beamV->setAreaMomentInertiaIxx(beamPar[5]);
        beamV->setNeglectsShear(Ishear);

        beamV->setRotationDOF1(0, x1);
        beamV->setRotationDOF1(1, y1);
        beamV->setRotationDOF1(2, z1);
        beamV->setRotationDOF2(0, x2);
        beamV->setRotationDOF2(1, y2);
        beamV->setRotationDOF2(2, z2);
    }
}