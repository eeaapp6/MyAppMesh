/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossAdaptorFailBIQUAD.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKFailureModelBIQUAD.h"

#include "FITKRadiossRadReader.h"
#include "FITKRadiossRadWriter.h"
#include "RadKeyLineSpliter.h"
#include "RadReadInformationMapper.h"
#include "RadWriteInformationMapper.h"

namespace Radioss
{
    QString Radioss::FITKRadiossAdaptorFailBIQUAD::getAdaptorClass()
    {
        //返回数据类名 带命名空间
        return "Radioss::FITKRadiossAdaptorFailBIQUAD";
    }

    bool FITKRadiossAdaptorFailBIQUAD::adaptR()
    {
        if (_caseData == nullptr || _reader == nullptr) return false;

        // 读取关键字
        RadKeyLineSpliter* keyLineSpliter = _reader->getKeyLineSpliter();
        if (!keyLineSpliter) return false;
        int pid = keyLineSpliter->getId();
        QString keyLine = keyLineSpliter->getCurrentLine();
        //获取
        FITKFailureModelBIQUAD* fail = new FITKFailureModelBIQUAD();
        if (fail == nullptr) return false;

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
            //读取失效数据
            ok = this->readFailure(fail);
        }
        if (!ok)
        {
            _reader->consoleMessage(1, QString("Read Failure Error"));
            return false;
        }
        _reader->consoleMessage(1, QString("Read Failure :/FAIL/BIQUAD"));
        FITKRadiossFailureModelManager* fMgr = _caseData->getFailureModelManager();
        if (!fMgr)return false;
        fail->setDataObjectName(fMgr->checkName("Failure-1"));
        fMgr->appendDataObj(fail);
        //设置rad材料ID
        fail->addMaterialID(pid);

        return true;
    }
    bool FITKRadiossAdaptorFailBIQUAD::adaptW()
    {
        FITKFailureModelBIQUAD* fail = dynamic_cast<FITKFailureModelBIQUAD*>(_dataObj);
        if (fail == nullptr || _writer == nullptr) return false;
        //若没有材料ID,不输出
        QList<int> matIDs = fail->getMaterialIDs();
        if (matIDs.isEmpty())return true;
        QTextStream* stream = _writer->getStream();
        RadWriteInformationMapper* infoMapper = _writer->getWriteInpInformationMapper();
        if (stream == nullptr || infoMapper == nullptr) return false;

        for (int matID : matIDs) {
            //写出关键字行 和 材料ID 
            int radMatID = infoMapper->getRadIDByMaterialID(matID);
            *stream << fail->GetFITKFailureModelBIQUADRadiossKeyWord() << "/" << radMatID << endl;
            //写出失效参数
            *stream << QString("#%1%2%3%4%5").arg("c1", 19).arg("c2", 20).arg("c3", 20)
                .arg("c4", 20).arg("c5", 20) << endl;
            *stream << gapStr(fail->getC1()) << gapStr(fail->getC2()) << gapStr(fail->getC3())
                << gapStr(fail->getC4()) << gapStr(fail->getC5()) << endl;

            *stream << QString("#%1%2%3%4%5%6").arg("P_thickfail", 19).arg("M_Flag", 10)
                .arg("S_Flag", 10).arg("Inst_start", 20).arg("fct_IDel", 20).arg("El_ref", 20) << endl;
            *stream << gapStr(fail->getPthickfail()) << gapStr(fail->getMFlag(), 10, false)
                << gapStr(fail->getSFlag(), 10, false) << gapStr(fail->getInststart())
                << gapStr(fail->getFctIDel(), 20, false) << gapStr(fail->getElref()) << endl;
            //判断M-Flag,若等于99，写出r1	r2	r4	r5
            int m_flag = fail->getMFlag();
            if (m_flag == 99)
            {
                *stream << QString("#%1%2%3%4").arg("r1", 19).arg("r2", 20).arg("r4", 20).arg("r5", 20) << endl;
                *stream << gapStr(fail->getR1()) << gapStr(fail->getR2())
                    << gapStr(fail->getR4()) << gapStr(fail->getR5()) << endl;
            }
        }
        //写出radID 从1开始递增
        int radID = infoMapper->getMaxRadFailureID() + 1;
        //建立映射关系
        infoMapper->insertFailureIDMap(fail->getDataObjectID(), radID);
        //写出失效ID
        *stream << QString("#%1").arg("fail_ID", 9) << endl;
        *stream << gapStr(radID) << endl;
        return true;
    }
    bool FITKRadiossAdaptorFailBIQUAD::readFailure(FITKFailureModelBIQUAD* fail)
    {
        if (fail == nullptr || _reader == nullptr) return false;
        //无名称，回退一行
        _reader->backLine();
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
        if (lines.size() < 2)
        {
            _reader->consoleMessage(3, QString("Invalid Failure : %1.").arg(fail->getDataObjectName()));
            return false;
        }
        //读取材料参数
        this->readParam(lines);
        //材料对象赋值
        this->setFailureData(fail);

        return true;
    }

    void FITKRadiossAdaptorFailBIQUAD::readParam(const QStringList& lines)
    {
        if (lines.size() < 2 || _reader == nullptr) return;

        //c1	c2	c3	c4	c5
        QList<QPair<QString, int>> failPar = { {"c1", 20}, {"c2", 20},{"c3", 20 }, { "c4", 20 }, { "c5", 20 } };
        splitParam(lines[0], failPar);

        //P_thickfail	M-Flag	S-Flag	Inst_start	 	fct_IDel	El_ref
        failPar = { {"P_thickfail", 20}, {"M_Flag", 10},{"S_Flag", 10 }, { "Inst_start", 20 }, { "fct_IDel", 20 }, { "El_ref", 20 } };
        splitParam(lines[1], failPar);

        //判断M-Flag,若等于99，读取r1	r2	r4	r5	
        int m_flag = static_cast<int>(_failParam.value("M_Flag", 0));
        if (m_flag == 99) 
        {
            failPar = { {"r1", 20}, {"r2", 20},{"r4", 20 }, { "r5", 20 } };
            splitParam(lines.value(2), failPar);
        }
        //失效ID
        if (lines.size() < 3) return;
        QStringList failIDstr = lines.value(lines.size() - 1).simplified().split(' ');
        if (failIDstr.size() != 1)return;
        bool ok = false;
        int failID = failIDstr[0].toInt(&ok);
        if (ok)
            _failParam.insert("fail_ID", failID);
    }

    void FITKRadiossAdaptorFailBIQUAD::splitParam(const QString& line, const QList<QPair<QString, int>>& par)
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
                _failParam.insert(parKey, parVal);
            split += width;
        }
    }

    void FITKRadiossAdaptorFailBIQUAD::setFailureData(FITKFailureModelBIQUAD* fail)
    {
        Radioss::RadReadInformationMapper* mapper = _reader->getReadRadInformationMapper();
        if (!mapper||!fail) return;

        fail->setC1(_failParam.value("c1", fail->getC1()));
        fail->setC2(_failParam.value("c2", fail->getC2()));
        fail->setC3(_failParam.value("c3", fail->getC3()));
        fail->setC4(_failParam.value("c4", fail->getC4()));
        fail->setC5(_failParam.value("c5", fail->getC5()));
        fail->setPthickfail(_failParam.value("P_thickfail", fail->getPthickfail()));
        fail->setMFlag(static_cast<int>(_failParam.value("M_Flag", fail->getMFlag())));
        fail->setSFlag(static_cast<int>(_failParam.value("S_Flag", fail->getSFlag())));
        fail->setInststart(_failParam.value("Inst_start", fail->getInststart()));
        fail->setFctIDel(static_cast<int>(_failParam.value("fct_IDel", fail->getFctIDel())));
        fail->setElref(_failParam.value("El_ref", fail->getElref()));

        //判断M-Flag,若等于99，设置r1	r2	r4	r5
        int m_flag = static_cast<int>(_failParam.value("M_Flag", 0));
        if (m_flag == 99)
        {
            fail->setR1(_failParam.value("r1", fail->getR1()));
            fail->setR2(_failParam.value("r2", fail->getR2()));
            fail->setR4(_failParam.value("r4", fail->getR4()));
            fail->setR5(_failParam.value("r5", fail->getR5()));
        }
        //设置失效ID
        int failID = static_cast<int>(_failParam.value("fail_ID", -1));
        if (failID != -1)
            mapper->insertFailureIDMap(failID, fail->getDataObjectID());
    }
}