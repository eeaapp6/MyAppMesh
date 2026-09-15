/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossAdaptorEOSPolynomial.h"

#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKEquationOfStatePolynomial.h"

#include "FITKRadiossRadReader.h"
#include "FITKRadiossRadWriter.h"
#include "RadKeyLineSpliter.h"
#include "RadReadInformationMapper.h"
#include "RadWriteInformationMapper.h"

namespace Radioss
{
    QString FITKRadiossAdaptorEOSPolynomial::getAdaptorClass()
    {
        return "Radioss::FITKRadiossAdaptorEOSPolynomial";
    }

    bool FITKRadiossAdaptorEOSPolynomial::adaptR()
    {
        if (_caseData == nullptr || _reader == nullptr) return false;

        // 读取关键字
        RadKeyLineSpliter* keyLineSpliter = _reader->getKeyLineSpliter();
        if (!keyLineSpliter) return false;
        int pid = keyLineSpliter->getId();
        QString keyLine = keyLineSpliter->getCurrentLine();
        //获取
        FITKEquationOfStatePolynomial* eos = new FITKEquationOfStatePolynomial();
        if (eos == nullptr) return false;

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
            ok = this->readEOS(eos);
        }
        if (!ok)
        {
            _reader->consoleMessage(1, QString("Read EOS Error : %1 %2").arg(keyLine).arg(eos->getDataObjectName()));
            return false;
        }
        _reader->consoleMessage(1, QString("Read EOS : %1 %2").arg(keyLine).arg(eos->getDataObjectName()));
        _caseData->getEquationOfState()->appendDataObj(eos);
        //设置rad材料ID
        eos->addMaterialID(pid);

        return true;
    }

    bool FITKRadiossAdaptorEOSPolynomial::adaptW()
    {
        FITKEquationOfStatePolynomial* eos = dynamic_cast<FITKEquationOfStatePolynomial*>(_dataObj);
        if (eos == nullptr || _writer == nullptr) return false;
        //若没有材料ID,不输出
        QList<int> matIDs = eos->getMaterialIDs();
        if (matIDs.isEmpty())return true;
        QTextStream* stream = _writer->getStream();
        RadWriteInformationMapper* infoMapper = _writer->getWriteInpInformationMapper();
        if (stream == nullptr || infoMapper == nullptr) return false;
        for (int matID : matIDs) {
            //写出关键字行 和 材料ID 
            int radMatID = infoMapper->getRadIDByMaterialID(matID);
            if (radMatID < 1)continue;
            *stream << eos->GetFITKEquationOfStatePolynomialRadiossKeyWord() << "/" << radMatID << endl;
            //写出属性名称
            *stream << eos->getDataObjectName() << endl;

            *stream << QString("#%1%2%3%4").arg("C0", 19).arg("C1", 20).arg("C2", 20).
                arg("C3", 20) << endl;
            *stream << gapStr(eos->getC0()) << gapStr(eos->getC1()) << gapStr(eos->getC2()) <<
                gapStr(eos->getC3()) << endl;
            *stream << QString("#%1%2%3%4%5").arg("C4", 19).arg("C5", 20).arg("E0", 20).
                arg("Psh", 20).arg("RHO_0", 20) << endl;
            *stream << gapStr(eos->getC4()) << gapStr(eos->getC5()) << gapStr(eos->getE0()) <<
                gapStr(eos->getPsh()) << gapStr(eos->getP0()) << endl;
        }
        return true;
    }

    bool FITKRadiossAdaptorEOSPolynomial::readEOS(FITKEquationOfStatePolynomial * eos)
    {
        if (eos == nullptr || _reader == nullptr) return false;
        FITKEquationOfStateManager* fMgr = _caseData->getEquationOfState();
        if (!fMgr)return false;
        //设置属性名称
        QString line = _reader->previousLine().trimmed();
        eos->setDataObjectName(fMgr->checkName(line));
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
            _reader->consoleMessage(3, QString("Invalid EOS : %1.").arg(eos->getDataObjectName()));
            return false;
        }
        //读取参数并赋值
        this->readParam(lines, eos);

        return true;
    }

    void FITKRadiossAdaptorEOSPolynomial::readParam(const QStringList & lines, Radioss::FITKEquationOfStatePolynomial * eos)
    {
        if (lines.size() != 2 || !eos) 
            return;
        bool ok = false;
        QHash<int, double> param{};

        QString par = lines.value(0);
        //第一行参数
        for (int i = 0; i < 4; ++i)
        {
            double value = par.mid(20 * i, 20).trimmed().toDouble(&ok);
            if (ok) param.insert(i, value);
        }
        //第二行参数
        par = lines.value(1);
        for (int i = 0; i < 5; ++i)
        {
            double value = par.mid(20 * i, 20).trimmed().toDouble(&ok);
            if (ok) param.insert(i+10, value);
        }

        eos->setC0(param.value(0, eos->getC0()));
        eos->setC1(param.value(1, eos->getC1()));
        eos->setC2(param.value(2, eos->getC2()));
        eos->setC3(param.value(3, eos->getC3()));
        eos->setC4(param.value(10, eos->getC4()));
        eos->setC5(param.value(11, eos->getC5()));
        eos->setE0(param.value(12, eos->getE0()));
        eos->setPsh(param.value(13, eos->getPsh()));
        eos->setP0(param.value(14, eos->getP0()));
    }
}