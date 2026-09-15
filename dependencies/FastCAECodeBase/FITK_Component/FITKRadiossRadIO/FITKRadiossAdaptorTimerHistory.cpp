/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossAdaptorTimerHistory.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSolution.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractProbe.h"
#include "FITK_Component/FITKRadiossData/FITKTimeHistoryACCEL.h"
#include "FITK_Component/FITKRadiossData/FITKTimeHistoryINTER.h"
#include "FITK_Component/FITKRadiossData/FITKTimeHistoryRBODY.h"
#include "FITK_Component/FITKRadiossData/FITKTimeHistoryRWALL.h"
#include "FITK_Component/FITKRadiossData/FITKTimeHistorySECTIO.h"
#include "FITK_Component/FITKRadiossData/FITKTimeHistorySPHCEL.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"

#include "FITKRadiossRadReader.h"
#include "FITKRadiossRadWriter.h"
#include "RadKeyLineSpliter.h"
#include "RadReadInformationMapper.h"
#include "RadWriteInformationMapper.h"

namespace Radioss
{
    QString FITKRadiossAdaptorTimerHistory::getAdaptorClass()
    {
        return "Radioss::FITKRadiossAdaptorTimerHistory";
    }

    bool FITKRadiossAdaptorTimerHistory::adaptR()
    {
        if (_caseData == nullptr || _reader == nullptr) return false;
        RadKeyLineSpliter* spliter = _reader->getKeyLineSpliter();
        RadReadInformationMapper* mapper = _reader->getReadRadInformationMapper();
        if (!mapper || !spliter)return false;
        FITKRadiossSolution* solution = _caseData->getCurrentSolution();
        if (!solution)return false;
        FITKTimeHistoryManager* thMgr = solution->getTimeHietoryManager();
        if (!thMgr)return false;

        //读取rad ID和名称
        const int radID = spliter->getId(0);
        if (radID <= 0) return false;
        const QString name = _reader->readLine();
        if (name.isEmpty()) return false;

        //判断类型
        QString strType = spliter->getKeys();
        FITKAbstractTimeHistory* th{};
        if (strType == "/TH/ACCEL")
            th = new FITKTimeHistoryACCEL;
        else if (strType == "/TH/INTER")
            th = new FITKTimeHistoryINTER;
        else if (strType == "/TH/RBODY")
            th = new FITKTimeHistoryRBODY;
        else if (strType == "/TH/RWALL")
            th = new FITKTimeHistoryRWALL;
        else if (strType == "/TH/SECTIO")
            th = new FITKTimeHistorySECTIO;
        else if (strType == "/TH/SPHCEL")
            th = new FITKTimeHistorySPHCEL;

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
            _reader->consoleMessage(3, QString("Invalid /TH : %1.").arg(name));
            return false;
        }
        //读取赋予参数
        if (!this->readParam(lines, th)) {
            delete th;
            return false;
        }
        th->setDataObjectName(name);
        thMgr->appendDataObj(th);

        return true;
    }

    bool FITKRadiossAdaptorTimerHistory::adaptW()
    {
        FITKAbstractTimeHistory*  th = dynamic_cast<FITKAbstractTimeHistory*>(_dataObj);
        if (th == nullptr || _writer == nullptr) return false;

        QStringList variableKeyWords = th->getVariableKeyWords();
        QList<int> objIDs = th->getObjIDs();
        FITKAbstractTimeHistory::TimeHistoryType thType = th->getTimeHistoryType();
        if (variableKeyWords.isEmpty()|| (objIDs.isEmpty()&& thType!= FITKAbstractTimeHistory::TimeHistory_SPHCEL) )return false;

        QTextStream* stream = _writer->getStream();
        RadWriteInformationMapper* infoMapper = _writer->getWriteInpInformationMapper();
        if (stream == nullptr || infoMapper == nullptr) return false;
        //写出关键字行  ID 从1开始递增
        int radID = infoMapper->getMaxRadTimeHID() + 1;
        //建立映射关系
        infoMapper->insertTimeHIDMap(th->getDataObjectID(), radID);
        QString keyWords{};

        if (thType == FITKAbstractTimeHistory::TimeHistory_ACCEL)
            keyWords = "/TH/ACCEL/";
        else if (thType == FITKAbstractTimeHistory::TimeHistory_INTER) 
            keyWords = "/TH/INTER/";
        else if (thType == FITKAbstractTimeHistory::TimeHistory_RWALL) 
            keyWords = "/TH/RWALL/";
        else if (thType == FITKAbstractTimeHistory::TimeHistory_RBODY) 
            keyWords = "/TH/RBODY/";
        else if (thType == FITKAbstractTimeHistory::TimeHistory_SECTIO) 
            keyWords = "/TH/SECTIO/";
        else if (thType == FITKAbstractTimeHistory::TimeHistory_SPHCEL)
            keyWords = "/TH/SPHCEL/";

        *stream << keyWords << radID << endl;

        //写名称
        *stream << th->getDataObjectName() << endl;

        //写出变量组
        *stream << "#     var1";
        for (int i = 2; i <= 10; ++i)
            *stream << QString("      var%1").arg(i);
        *stream << endl;
        for (QString keyWords : variableKeyWords)
            *stream << keyWords.leftJustified(10, ' ');
        *stream << endl;

        //写出对象ID
        *stream << "#     Obj1";
        for (int i = 2; i <= 10; ++i)
            *stream << QString("      Obj%1").arg(i);
        *stream << endl;
        if (thType == FITKAbstractTimeHistory::TimeHistory_ACCEL) {
            for (int objID : objIDs)
                *stream << gapStr(infoMapper->getRadIDByAccelID(objID));
        }
        else if (thType == FITKAbstractTimeHistory::TimeHistory_INTER) {
            for (int objID : objIDs)
                *stream << gapStr(infoMapper->getRadIDByInteractionID(objID));
        }
        else if (thType == FITKAbstractTimeHistory::TimeHistory_RWALL) {
            for (int objID : objIDs)
                *stream << gapStr(infoMapper->getRadIDByRWallID(objID));
        }
        else if (thType == FITKAbstractTimeHistory::TimeHistory_RBODY) {
            for (int objID : objIDs)
                *stream << gapStr(infoMapper->getRadIDByConnectionID(objID));
        }
        else if (thType == FITKAbstractTimeHistory::TimeHistory_SECTIO) {
            for (int objID : objIDs)
                *stream << gapStr(infoMapper->getRadIDBySectionID(objID));
        }
        else if (thType == FITKAbstractTimeHistory::TimeHistory_SPHCEL) {
            FITKTimeHistorySPHCEL* tH = dynamic_cast<FITKTimeHistorySPHCEL*>(th);
            if (!tH) return false;
            int objID = tH->getSPHNodeID();
            QString name = tH->getSPHNodeName();
            *stream << gapStr(objID)<<QString("          %1").arg(name);
        }
        *stream << endl;

        return true;
    }

    bool FITKRadiossAdaptorTimerHistory::readParam(const QStringList & lines, FITKAbstractTimeHistory * th)
    {
        if(lines.size()!=2||!th)return false;
        QStringList keyWords = lines[0].simplified().split(" ");
        QStringList objStrIDs = lines[1].simplified().split(" ");
        QList<int> objIDs{};
        for (QString objID: objStrIDs) {
            bool ok = false;
            int id = objID.toInt(&ok);
            if (ok)objIDs.append(id);
        }
        if (keyWords.isEmpty() || objIDs.isEmpty())return false;

        FITKAbstractTimeHistory::TimeHistoryType thType = th->getTimeHistoryType();

        if (thType == FITKAbstractTimeHistory::TimeHistoryType::TimeHistory_ACCEL){
            if(!readACCEL(th, keyWords))return false;
        }
        else if (thType == FITKAbstractTimeHistory::TimeHistoryType::TimeHistory_INTER){
            if (!readINTER(th, keyWords, lines[0]))return false;
        }
        else if (thType == FITKAbstractTimeHistory::TimeHistoryType::TimeHistory_RWALL){
            if (!readRWALL(th, keyWords))return false;
        }
        else if (thType == FITKAbstractTimeHistory::TimeHistoryType::TimeHistory_RBODY){
            if (!readRBODY(th, keyWords))return false;
        }
        else if (thType == FITKAbstractTimeHistory::TimeHistoryType::TimeHistory_SECTIO){
            if (!readSECTIO(th, keyWords))return false;
        }
        else if (thType == FITKAbstractTimeHistory::TimeHistoryType::TimeHistory_SPHCEL) {
            if (!readSPHCEL(th, keyWords, lines[1]))return false;
        }
        else
            return false;

        if(thType != FITKAbstractTimeHistory::TimeHistoryType::TimeHistory_SPHCEL)
            th->setObjIDs(objIDs);

        return true;
    }

    bool FITKRadiossAdaptorTimerHistory::readACCEL(FITKAbstractTimeHistory* th, QStringList& keyWords)
    {
        FITKTimeHistoryACCEL* tH = dynamic_cast<FITKTimeHistoryACCEL*>(th);
        if (!tH) return false;
        //变量组类型
        if (keyWords.contains("DEF"))
            tH->setVarlableGroupType(FITKTimeHistoryACCEL::THACCELVariableGroup::VGACCEL_DEF);
        else if (keyWords.contains("W"))
            tH->setVarlableGroupType(FITKTimeHistoryACCEL::THACCELVariableGroup::VGACCEL_W);
        //其余变量关键字
        QList<FITKTimeHistoryACCEL::TimeHistoryACCELVariable> variables;
        // 枚举转换字符 
        Core::FITKEnumTransfer<FITKTimeHistoryACCEL::TimeHistoryACCELVariable> fitkEleTypeTrafer;
        bool isValid = false;
        for (QString keyWord : keyWords) {
            auto keyType = fitkEleTypeTrafer.fromString(keyWord, isValid);
            if (!isValid) continue;
            variables.append(keyType);
        }
        tH->setCustomizeVariable(variables);
        return true;
    }

    bool FITKRadiossAdaptorTimerHistory::readINTER(FITKAbstractTimeHistory* th, QStringList& keyWords, QString _keyWord_)
    {
        FITKTimeHistoryINTER* tH = dynamic_cast<FITKTimeHistoryINTER*>(th);
        if (!tH) return false;
        //变量组类型
        if (keyWords.contains("DEF"))
            tH->setVarlableGroupType(FITKTimeHistoryINTER::THINTERVariableGroup::VGINTER_DEF);
        else if (keyWords.contains("FN"))
            tH->setVarlableGroupType(FITKTimeHistoryINTER::THINTERVariableGroup::VGINTER_FN);
        else if (keyWords.contains("FT"))
            tH->setVarlableGroupType(FITKTimeHistoryINTER::THINTERVariableGroup::VGINTER_FT);
        else if (keyWords.contains("|FN|"))
            tH->setVarlableGroupType(FITKTimeHistoryINTER::THINTERVariableGroup::VGINTER_AbsoluteFN);
        else if (keyWords.contains("|FT|"))
            tH->setVarlableGroupType(FITKTimeHistoryINTER::THINTERVariableGroup::VGINTER_AbsoluteF);
        //其余变量关键字
        QList<FITKTimeHistoryINTER::TimeHistoryINTERVariable> variables;
        _keyWord_ = _keyWord_.replace("|", "_");// TH/INTER有绝对值符号,用下划线代替
        QStringList oKeyWords = _keyWord_.simplified().split(" ");
        // 枚举转换字符 
        Core::FITKEnumTransfer<FITKTimeHistoryINTER::TimeHistoryINTERVariable> fitkEleTypeTrafer;
        bool isValid = false;
        for (QString keyWord : oKeyWords) {
            auto keyType = fitkEleTypeTrafer.fromString(keyWord, isValid);
            if (!isValid) continue;
            variables.append(keyType);
        }
        tH->setCustomizeVariable(variables);
        return true;
    }

    bool FITKRadiossAdaptorTimerHistory::readRWALL(FITKAbstractTimeHistory* th, QStringList& keyWords)
    {
        FITKTimeHistoryRWALL* tH = dynamic_cast<FITKTimeHistoryRWALL*>(th);
        if (!tH) return false;
        //变量组类型
        if (keyWords.contains("DEF"))
            tH->setVarlableGroupType(FITKTimeHistoryRWALL::THRWALLVariableGroup::VGRWALL_DEF);
        else if (keyWords.contains("FN"))
            tH->setVarlableGroupType(FITKTimeHistoryRWALL::THRWALLVariableGroup::VGRWALL_FN);
        else if (keyWords.contains("FT"))
            tH->setVarlableGroupType(FITKTimeHistoryRWALL::THRWALLVariableGroup::VGRWALL_FT);
        //其余变量关键字
        QList<FITKTimeHistoryRWALL::TimeHistoryRWALLVariable> variables;
        // 枚举转换字符 
        Core::FITKEnumTransfer<FITKTimeHistoryRWALL::TimeHistoryRWALLVariable> fitkEleTypeTrafer;
        bool isValid = false;
        for (QString keyWord : keyWords) {
            auto keyType = fitkEleTypeTrafer.fromString(keyWord, isValid);
            if (!isValid) continue;
            variables.append(keyType);
        }
        tH->setCustomizeVariable(variables);
        return true;
    }

    bool FITKRadiossAdaptorTimerHistory::readRBODY(FITKAbstractTimeHistory* th, QStringList& keyWords)
    {
        FITKTimeHistoryRBODY* tH = dynamic_cast<FITKTimeHistoryRBODY*>(th);
        if (!tH) return false;
        //变量组类型
        if (keyWords.contains("DEF"))
            tH->setVarlableGroupType(FITKTimeHistoryRBODY::THRBODYVariableGroup::VGRBODY_DEF);
        else if (keyWords.contains("F"))
            tH->setVarlableGroupType(FITKTimeHistoryRBODY::THRBODYVariableGroup::VGRBODY_F);
        else if (keyWords.contains("M"))
            tH->setVarlableGroupType(FITKTimeHistoryRBODY::THRBODYVariableGroup::VGRBODY_M);
        else if (keyWords.contains("R"))
            tH->setVarlableGroupType(FITKTimeHistoryRBODY::THRBODYVariableGroup::VGRBODY_R);
        else if (keyWords.contains("FI"))
            tH->setVarlableGroupType(FITKTimeHistoryRBODY::THRBODYVariableGroup::VGRBODY_FI);
        else if (keyWords.contains("MI"))
            tH->setVarlableGroupType(FITKTimeHistoryRBODY::THRBODYVariableGroup::VGRBODY_MI);
        //其余变量关键字
        QList<FITKTimeHistoryRBODY::TimeHistoryRBODYVariable> variables;
        // 枚举转换字符 
        Core::FITKEnumTransfer<FITKTimeHistoryRBODY::TimeHistoryRBODYVariable> fitkEleTypeTrafer;
        bool isValid = false;
        for (QString keyWord : keyWords) {
            auto keyType = fitkEleTypeTrafer.fromString(keyWord, isValid);
            if (!isValid) continue;
            variables.append(keyType);
        }
        tH->setCustomizeVariable(variables);
        return true;
    }

    bool FITKRadiossAdaptorTimerHistory::readSECTIO(FITKAbstractTimeHistory* th, QStringList& keyWords)
    {
        FITKTimeHistorySECTIO* tH = dynamic_cast<FITKTimeHistorySECTIO*>(th);
        if (!tH) return false;
        //变量组类型
        if (keyWords.contains("DEF"))
            tH->setVarlableGroupType(FITKTimeHistorySECTIO::THSECTIOVariableGroup::VGSECTIO_DEF);
        else if (keyWords.contains("FN"))
            tH->setVarlableGroupType(FITKTimeHistorySECTIO::THSECTIOVariableGroup::VGSECTIO_FN);
        else if (keyWords.contains("FT"))
            tH->setVarlableGroupType(FITKTimeHistorySECTIO::THSECTIOVariableGroup::VGSECTIO_FT);
        else if (keyWords.contains("M"))
            tH->setVarlableGroupType(FITKTimeHistorySECTIO::THSECTIOVariableGroup::VGSECTIO_M);
        else if (keyWords.contains("CENTER"))
            tH->setVarlableGroupType(FITKTimeHistorySECTIO::THSECTIOVariableGroup::VGSECTIO_CENTER);
        else if (keyWords.contains("GLOBAL"))
            tH->setVarlableGroupType(FITKTimeHistorySECTIO::THSECTIOVariableGroup::VGSECTIO_GLOBAL);
        else if (keyWords.contains("LOCAL"))
            tH->setVarlableGroupType(FITKTimeHistorySECTIO::THSECTIOVariableGroup::VGSECTIO_LOCAL);
        //其余变量关键字
        QList<FITKTimeHistorySECTIO::TimeHistorySECTIOVariable> variables;
        // 枚举转换字符 
        Core::FITKEnumTransfer<FITKTimeHistorySECTIO::TimeHistorySECTIOVariable> fitkEleTypeTrafer;
        bool isValid = false;
        for (QString keyWord : keyWords) {
            auto keyType = fitkEleTypeTrafer.fromString(keyWord, isValid);
            if (!isValid) continue;
            variables.append(keyType);
        }
        tH->setCustomizeVariable(variables);
        return true;
    }

    bool FITKRadiossAdaptorTimerHistory::readSPHCEL(FITKAbstractTimeHistory* th, QStringList& keyWords, QString idAndName)
    {
        FITKTimeHistorySPHCEL* tH = dynamic_cast<FITKTimeHistorySPHCEL*>(th);
        if (!tH) return false;
        //变量组类型
        if (keyWords.contains("DEF"))
            tH->setVarlableGroupType(FITKTimeHistorySPHCEL::THSPHCELVariableGroup::VGSPHCEL_DEF);
        if (keyWords.contains("ALL"))
            tH->setVarlableGroupType(FITKTimeHistorySPHCEL::THSPHCELVariableGroup::VGSPHCEL_ALL);
        //其余变量关键字
        QList<FITKTimeHistorySPHCEL::TimeHistorySPHCELVariable> variables;
        // 枚举转换字符 
        Core::FITKEnumTransfer<FITKTimeHistorySPHCEL::TimeHistorySPHCELVariable> fitkEleTypeTrafer;
        bool isValid = false;
        for (QString keyWord : keyWords) {
            auto keyType = fitkEleTypeTrafer.fromString(keyWord, isValid);
            if (!isValid) continue;
            variables.append(keyType);
        }
        tH->setCustomizeVariable(variables);
        //设置SPH粒子节点ID和名称
        bool ok = false;
        int parVal = idAndName.mid(0, 10).simplified().toInt(&ok);
        if (ok)
            tH->setSPHNodeID(parVal);
        QString name = idAndName.mid(10).simplified();
        tH->setSPHNodeName(name);

        return true;
    }


}