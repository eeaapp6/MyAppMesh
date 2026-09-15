/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossAdaptorPropBeamSPR.h"

#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKPropBeamSPR.h"

#include "FITKRadiossRadReader.h"
#include "FITKRadiossRadWriter.h"
#include "RadKeyLineSpliter.h"
#include "RadReadInformationMapper.h"
#include "RadWriteInformationMapper.h"

#include <QList>

namespace Radioss
{
    QString FITKRadiossAdaptorPropBeamSPR::getAdaptorClass()
    {
        return "Radioss::FITKRadiossAdaptorPropBeamSPR";
    }
    bool FITKRadiossAdaptorPropBeamSPR::adaptR()
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
        FITKPropBeamSPR* beam = new FITKPropBeamSPR();
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

    bool FITKRadiossAdaptorPropBeamSPR::adaptW()
    {
        FITKPropBeamSPR* beam = dynamic_cast<FITKPropBeamSPR*>(_dataObj);
        if (beam == nullptr || _writer == nullptr) return false;
        FITKPropValueBeamSPR* value = beam->getValue();
        if (!value) return false;

        QTextStream* stream = _writer->getStream();
        RadWriteInformationMapper* infoMapper = _writer->getWriteInpInformationMapper();
        if (stream == nullptr || infoMapper == nullptr) return false;
        //写出关键字行  ID 从1开始递增
        int radID = infoMapper->getMaxRadPropertyID() + 1;
        //建立映射关系
        infoMapper->insertPropertyIDMap(beam->getDataObjectID(), radID);
        *stream << beam->GetFITKPropBeamSPRRadiossKeyWord() << "/" << radID << endl;
        //写出属性名称
        *stream << beam->getDataObjectName() << endl;

        *stream << QString("#%1%2%3%4%5%6%7%8").arg("Mass", 19).arg("Inertia", 20).arg("SkewID", 10).arg("SensID", 10)
            .arg("Isflag", 10).arg("Ifail", 10).arg("Ileng", 10).arg("Ifail2", 10) << endl;
        *stream << gapStr(value->getMass()) << gapStr(value->getInertia()) << gapStr(value->getSkewID())
            << gapStr(value->getSensID()) << gapStr(value->getIsflag(), 10, false) << gapStr(value->getIfail(), 10, false)
            << gapStr(value->getIleng(), 10, false) << gapStr(value->getIfail2(), 10, false) << endl;

        // 6个维度参数写出
        QList<DOF_SPR> dofList = { DOF_SPR::Tension_Compression, DOF_SPR::Shear_XY, DOF_SPR::Shear_XZ,
            DOF_SPR::Torsion, DOF_SPR::Bending_Y, DOF_SPR::Bending_Z };
        for (DOF_SPR dof : dofList)
        {
            ValueDOF& dofValue = value->getValueDOF(dof);
            *stream << QString("#%1%2%3%4%5").arg("K", 19).arg("C", 20).arg("A", 20).arg("B", 20).arg("D", 20) << endl;
            *stream << gapStr(dofValue.K) << gapStr(dofValue.C) << gapStr(dofValue.A) << gapStr(dofValue.B) << gapStr(dofValue.D) << endl;

            *stream << QString("#%1%2%3%4%5%6%7").arg("fct_ID1", 9).arg("H", 10).arg("fct_ID2", 10).arg("fct_ID3", 10).arg("fct_ID4", 10).
                arg("min", 30).arg("max", 20) << endl;
            *stream << gapStr(dofValue.fct_ID1) << gapStr(dofValue.H, 10, false) << gapStr(dofValue.fct_ID2)
                << gapStr(dofValue.fct_ID3) << gapStr(dofValue.fct_ID4) << gapStr(dofValue.min,30) << gapStr(dofValue.max) << endl;

            *stream << QString("#%1%2%3%4").arg("F", 19).arg("E", 20).arg("Ascale", 20).arg("Hscale", 20) << endl;
            *stream << gapStr(dofValue.F) << gapStr(dofValue.E) << gapStr(dofValue.Ascale) << gapStr(dofValue.Hscale) << endl;
        }

        //第20行参数
        *stream << QString("#%1%2%3%4").arg("v0", 19).arg("w0", 20).arg("Fcut", 20).arg("Fsmooth", 10) << endl;
        *stream << gapStr(value->getV0()) << gapStr(value->getW0()) << gapStr(value->getFcut()) << gapStr(value->getFsmooth(), 10, false) << endl;

        //第21行到26行参数
        *stream << QString("#%1%2%3%4").arg(QString("c"), 19).arg("n", 20).arg("alpha", 20).arg("beta", 20) << endl;
        for (DOF_SPR dof : dofList)
        {
            ValueDOF& dofValue = value->getValueDOF(dof);
            *stream << gapStr(dofValue.vc) << gapStr(dofValue.n) << gapStr(dofValue.a) << gapStr(dofValue.e) << endl;
        }

        return true;
    }

    bool FITKRadiossAdaptorPropBeamSPR::readPropBeam(FITKPropBeamSPR* beam)
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
        if (lines.size() < 26)
        {
            _reader->consoleMessage(3, QString("Invalid Prop : %1.").arg(beam->getDataObjectName()));
            return false;
        }
        //读取属性参数
        this->readParam(lines);
        //属性对象赋值
        this->setPropData(beam);
        return true;
    }
    void FITKRadiossAdaptorPropBeamSPR::readParam(const QStringList& lines)
    {
        if (lines.size() != 26) return;
        //第一行
        QString par = lines[0];
        bool ok = false;
        for (int i = 0; i < 8; ++i)
        {
            double parVal{};
            if(i<2)
                parVal = par.mid(10 * i, 20).trimmed().toDouble(&ok);
            else
                parVal = par.mid(10 * i + 20, 10).trimmed().toInt(&ok);
            if (ok) _beamPar.insert(i, parVal);
        }
        /********** 6个维度的参数，第2行到第19行 ******************/
        for (int num = 0; num < 18; num+=3)
        {
            par = lines[num+1];
            for (int i = 0; i < 5; ++i)
            {
                double parVal = par.mid(20 * i, 20).trimmed().toDouble(&ok);
                if (ok) _beamPar.insert(i + (num + 1)*10, parVal);
            }
            par = lines[num+2];
            for (int i = 0; i < 7; ++i)
            {
                double parVal{};
                if (i < 5)
                    parVal = par.mid(10 * i, 10).trimmed().toInt(&ok);
                else if (i == 5)
                    parVal = par.mid(60, 20).trimmed().toDouble(&ok);
                else
                    parVal = par.mid(80, 20).trimmed().toDouble(&ok);
                if (ok) _beamPar.insert(i + (num + 2) * 10, parVal);
            }
            par = lines[num+3];
            for (int i = 0; i < 4; ++i)
            {
                double parVal = par.mid(20 * i, 20).trimmed().toDouble(&ok);
                if (ok) _beamPar.insert(i + (num + 3) * 10, parVal);
            }
        }
        //第20行
        par = lines[19];
        for (int i = 0; i < 4; ++i)
        {
            double parVal{};
            if (i < 3)
                parVal = par.mid(20 * i, 20).trimmed().toDouble(&ok);
            else if (i == 3)
                parVal = par.mid(60, 10).trimmed().toDouble(&ok);
            if (ok) _beamPar.insert(i + 19 * 10, parVal);
        }
        //第21行到第26行
        for (int num = 20; num < 26; ++num)
        {
            par = lines[num];
            for (int i = 0; i < 4; ++i)
            {
                double parVal= par.mid(20 * i, 20).trimmed().toDouble(&ok);
                if (ok) _beamPar.insert(i + num * 10, parVal);
            }
        }
    }
    void FITKRadiossAdaptorPropBeamSPR::setPropData(FITKPropBeamSPR* beam)
    {
        if (beam == nullptr) return;

        FITKPropValueBeamSPR* value = beam->getValue();
        if (!value) return;
        // 第1行参数设置
        value->setMass(_beamPar.value(0, value->getMass()));
        value->setInertia(_beamPar.value(1, value->getInertia()));
        value->setSkewID(_beamPar.value(2, value->getSkewID()));
        value->setSensID(_beamPar.value(3, value->getSensID()));
        value->setIsflag(_beamPar.value(4, value->getIsflag()));
        value->setIfail(_beamPar.value(5, value->getIfail()));
        value->setIleng(_beamPar.value(6, value->getIleng()));
        value->setIfail2(_beamPar.value(7, value->getIfail2()));
        // 6个维度参数设置
        for (int dof = DOF_SPR::Tension_Compression; dof <= DOF_SPR::Bending_Z; ++dof)
        {
            ValueDOF& dofValue = value->getValueDOF((DOF_SPR)dof);
            int key = (dof - 1) * 3 + 1;
            dofValue.K = _beamPar.value(key * 10 + 0, dofValue.K);
            dofValue.C = _beamPar.value(key * 10 + 1, dofValue.C);
            dofValue.A = _beamPar.value(key * 10 + 2, dofValue.A);
            dofValue.B = _beamPar.value(key * 10 + 3, dofValue.B);
            dofValue.D = _beamPar.value(key * 10 + 4, dofValue.D);
            dofValue.fct_ID1 = _beamPar.value((key + 1) * 10 + 0, dofValue.fct_ID1);
            dofValue.H = _beamPar.value((key + 1) * 10 + 1, dofValue.H);
            dofValue.fct_ID2 = _beamPar.value((key + 1) * 10 + 2, dofValue.fct_ID2);
            dofValue.fct_ID3 = _beamPar.value((key + 1) * 10 + 3, dofValue.fct_ID3);
            dofValue.fct_ID4 = _beamPar.value((key + 1) * 10 + 4, dofValue.fct_ID4);
            dofValue.min = _beamPar.value((key + 1) * 10 + 5, dofValue.min);
            dofValue.max = _beamPar.value((key + 1) * 10 + 6, dofValue.max);
            dofValue.F = _beamPar.value((key + 2) * 10 + 0, dofValue.F);
            dofValue.E = _beamPar.value((key + 2) * 10 + 1, dofValue.E);
            dofValue.Ascale = _beamPar.value((key + 2) * 10 + 2, dofValue.Ascale);
            dofValue.Hscale = _beamPar.value((key + 2) * 10 + 3, dofValue.Hscale);
        }
        // 第20行参数设置
        value->setV0(_beamPar.value(190, value->getV0()));
        value->setW0(_beamPar.value(191, value->getW0()));
        value->setFcut(_beamPar.value(192, value->getFcut()));
        value->setFsmooth(_beamPar.value(193, value->getFsmooth()));
        // 第21行到第26行参数设置
        for (int key = 20; key < 26; ++key)
        {
            ValueDOF& dofValue = value->getValueDOF((DOF_SPR)(key-19));
            dofValue.vc = _beamPar.value(key * 10 + 0, dofValue.vc);
            dofValue.n = _beamPar.value(key * 10 + 1, dofValue.n);
            dofValue.a = _beamPar.value(key * 10 + 2, dofValue.a);
            dofValue.e = _beamPar.value(key * 10 + 3, dofValue.e);
        }
    }
}

