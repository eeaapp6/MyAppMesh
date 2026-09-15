/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossAdaptorFailJOHNSON.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKFailureModelJOHNSON.h"

#include "FITKRadiossRadReader.h"
#include "FITKRadiossRadWriter.h"
#include "RadKeyLineSpliter.h"
#include "RadReadInformationMapper.h"
#include "RadWriteInformationMapper.h"

namespace Radioss
{
    QString FITKRadiossAdaptorFailJOHNSON::getAdaptorClass()
    {
        return "Radioss::FITKRadiossAdaptorFailJOHNSON";
    }
    bool FITKRadiossAdaptorFailJOHNSON::adaptR()
    {
        if (_caseData == nullptr || _reader == nullptr) return false;

        // 读取关键字
        RadKeyLineSpliter* keyLineSpliter = _reader->getKeyLineSpliter();
        if (!keyLineSpliter) return false;
        int pid = keyLineSpliter->getId();
        QString keyLine = keyLineSpliter->getCurrentLine();
        //获取
        FITKFailureModelJOHNSON* fail = new FITKFailureModelJOHNSON();
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
    bool FITKRadiossAdaptorFailJOHNSON::adaptW()
    {
        FITKFailureModelJOHNSON* fail = dynamic_cast<FITKFailureModelJOHNSON*>(_dataObj);
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
            *stream << fail->GetFITKFailureModelJOHNSONRadiossKeyWord() << "/" << radMatID << endl;
            //写出参数行
            *stream << QString("#%1%2%3%4%5").arg("D1", 19).arg("D2", 20).arg("D3", 20)
                .arg("D4", 20).arg("D5", 20) << endl;
            *stream << gapStr(fail->getD1()) << gapStr(fail->getD2()) << gapStr(fail->getD3())
                << gapStr(fail->getD4()) << gapStr(fail->getD5()) << endl;
            *stream << QString("#%1%2%3%4%5").arg("eps0", 19).arg("Ifailsh", 10)
                .arg("Ifailso", 10).arg("Dadv", 40).arg("Ixfem", 20) << endl;
            *stream << gapStr(fail->getEps0()) << gapStr(fail->getIfailsh(), 10, false)
                << gapStr(fail->getIfailso(), 10, false) << gapStr(fail->getDadv(), 40)
                << gapStr(fail->getIxfem(), 20, false) << endl;
        }
        //写出radID 从1开始递增
        int radID = infoMapper->getMaxRadFailureID() + 1;
        //建立映射关系,同一个失效对象只记录一次
        infoMapper->insertFailureIDMap(fail->getDataObjectID(), radID);
        *stream << QString("#%1").arg("fail_ID", 9) << endl;
        *stream << gapStr(radID) << endl;
        return true;
    }
    bool FITKRadiossAdaptorFailJOHNSON::readFailure(FITKFailureModelJOHNSON* fail)
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
        if (lines.size() != 2)
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
    void FITKRadiossAdaptorFailJOHNSON::readParam(const QStringList& lines)
    {
        if (lines.size() < 2 || _reader == nullptr) return;
        //D1	D2	D3	D4	D5
        QList<QPair<QString, int>> failPar = { {"D1", 20}, {"D2", 20},{"D3", 20 }, { "D4", 20 }, { "D5", 20 } };
        splitParam(lines[0], failPar);
        //eps0	Ifailsh	Ifailso	Dadv	Ixfem
        failPar = { {"eps0", 20}, {"Ifailsh", 10},{"Ifailso", 10 }, { "Dadv", 40 }, { "Ixfem", 20 } };
        splitParam(lines[1], failPar);
        //失效ID
        if (lines.size() < 3) return;
        QStringList failIDstr = lines.value(lines.size() - 1).simplified().split(' ');
        if (failIDstr.size() != 1)return;
        bool ok = false;
        int failID = failIDstr[0].toInt(&ok);
        if (ok)
            _failParam.insert("fail_ID", failID);
    }
    void FITKRadiossAdaptorFailJOHNSON::splitParam(const QString& line, const QList<QPair<QString, int>>& par)
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
    void FITKRadiossAdaptorFailJOHNSON::setFailureData(FITKFailureModelJOHNSON* fail)
    {
        Radioss::RadReadInformationMapper* mapper = _reader->getReadRadInformationMapper();
        if (!mapper || !fail) return;

        fail->setD1(_failParam.value("D1", fail->getD1()));
        fail->setD2(_failParam.value("D2", fail->getD2()));
        fail->setD3(_failParam.value("D3", fail->getD3()));
        fail->setD4(_failParam.value("D4", fail->getD4()));
        fail->setD5(_failParam.value("D5", fail->getD5()));
        fail->setEps0(_failParam.value("eps0", fail->getEps0()));
        fail->setIfailsh(static_cast<int>(_failParam.value("Ifailsh", fail->getIfailsh())));
        fail->setIfailso(static_cast<int>(_failParam.value("Ifailso", fail->getIfailso())));
        fail->setDadv(_failParam.value("Dadv", fail->getDadv()));
        fail->setIxfem(static_cast<int>(_failParam.value("Ixfem", fail->getIxfem())));
        //设置失效ID
        int failID = static_cast<int>(_failParam.value("fail_ID", -1));
        if (failID != -1)
            mapper->insertFailureIDMap(failID, fail->getDataObjectID());
    }
    
}