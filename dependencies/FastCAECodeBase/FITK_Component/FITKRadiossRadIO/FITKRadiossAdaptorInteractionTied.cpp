/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossAdaptorInteractionTied.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKInteractionTied.h"

#include "FITKRadiossRadReader.h"
#include "FITKRadiossRadWriter.h"
#include "RadKeyLineSpliter.h"
#include "RadReadInformationMapper.h"
#include "RadWriteInformationMapper.h"

QString Radioss::FITKRadiossAdaptorInteractionTied::getAdaptorClass()
{
    //返回数据类名 带命名空间
    return "Radioss::FITKRadiossAdaptorInteractionTied";
}

bool Radioss::FITKRadiossAdaptorInteractionTied::adaptR()
{
    if (_caseData == nullptr || _reader == nullptr) return false;
    // 创建Tie数据对象
    FITKInteractionKinematicTied* tie = new FITKInteractionKinematicTied();
    //读取关键字
    RadKeyLineSpliter *keyLineSpliter = _reader->getKeyLineSpliter();
    RadReadInformationMapper *infoMapper = _reader->getReadRadInformationMapper();
    if (!keyLineSpliter || !infoMapper) return false;

    //获取id和关键字
    int radID = keyLineSpliter->getId(0);
    QString keyLine = keyLineSpliter->getCurrentLine();

    bool ok = false;
    while (_reader && !_reader->atEnd())
    {
        QString line = _reader->readLine();
        if (line.startsWith("/"))
        {
            _reader->backLine();
            break;
        }
        //读取材料数据
        ok = this->readInteraction(tie);
    }
    if (!ok)
    {
        _reader->consoleMessage(3, QString("Read Tie Error : %1 %2").arg(keyLine).arg(tie->getDataObjectName()));
        return false;
    }
    _reader->consoleMessage(1, QString("Read Tie : %1 %2").arg(keyLine).arg(tie->getDataObjectName()));
    _caseData->getInteractionManager()->appendDataObj(tie);
    infoMapper->insertInteractionIDMap(radID, tie->getDataObjectID());
    return true;
}

bool Radioss::FITKRadiossAdaptorInteractionTied::adaptW()
{
    FITKInteractionKinematicTied* tie = dynamic_cast<FITKInteractionKinematicTied*>(_dataObj);
    if (tie == nullptr || _writer == nullptr) return false;
    FITKInteractionKinematicTiedDefault* defTie = tie->getValue();
    if (!defTie) return false;
    QTextStream* stream = _writer->getStream();
    RadWriteInformationMapper* infoMapper = _writer->getWriteInpInformationMapper();
    if (stream == nullptr || infoMapper == nullptr) return false;
    //写出关键字行  ID 从1开始递增
    int radID = infoMapper->getMaxRadInteractionID() + 1;
    //建立映射关系
    infoMapper->insertInteractionIDMap(tie->getDataObjectID(), radID);
    *stream << tie->GetFITKInteractionKinematicTiedRadiossKeyWord() << "/" << radID << endl;
    //写出名称
    *stream << tie->getDataObjectName() << endl;
    //节点组ID
    int grnd = infoMapper->getRadIDByNodeSetID(tie->getNodeGroupID());
    //主面ID
    int surfID = infoMapper->getRadIDBySurfaceID(tie->getMasterSurfaceID());
    //Spot标志
    int Spotflag = defTie->getSpotWeldFormulationFlag();

    *stream << QString("#%1%2%3%4%5%6%7%8").arg("grnd_IDs", 9).arg("surf_IDm", 10).arg("Ignore", 10)
        .arg("Spotflag", 10).arg("Level", 10).arg("Isearch", 10).arg("Idel2", 10).arg("dsearch", 30) << endl;

    *stream << gapStr(grnd) << gapStr(surfID)
            << gapStr(defTie->getIgnoreFlag(),10,false) << gapStr(Spotflag, 10, false)
            << gapStr(tie->getLevel(), 10, false) << gapStr(defTie->getSearchFormulationFlag(), 10, false)
            << gapStr(defTie->getNodeDeletionFlag(), 10, false) << gapStr(tie->getSearchDistance(),30) << endl;
    
    if (Spotflag == 20 || Spotflag == 21 || Spotflag == 22)
    {
        TiedOptionalData opData = tie->getOptionalData();

        *stream << QString("#%1%2%3%4%5%6%7%8").arg("Rupt", 9).arg("Ifilter", 10).arg("fct_IDsr", 10)
            .arg("fct_IDsn", 10).arg("fct_IDst", 10).arg("Isym", 10).arg("Max_N_Dist", 20).arg("Max_T_Dist", 20) << endl;
        *stream << gapStr(opData.Rupt, 10, false) << gapStr(opData.Ifilter, 10, false) << gapStr(opData.fct_IDsr)
            << gapStr(opData.fct_IDsn) << gapStr(opData.fct_IDst) << gapStr(opData.Isym, 10, false)
            << gapStr(opData.Max_N_Dist) << gapStr(opData.Max_T_Dist) << endl;

        *stream << QString("#%1%2%3%4%5").arg("Fscale_stress", 19).arg("Fscale_str_rate", 20).arg("Fscale_dist", 20)
            .arg("Alpha", 20).arg("Area", 20) << endl;
        *stream << gapStr(opData.Fscale_stress) << gapStr(opData.Fscale_str_rate) << gapStr(opData.Fscale_dist)
            << gapStr(opData.Alpha) << gapStr(opData.Area) << endl;
    }
    if (Spotflag == 25 || Spotflag == 27 || Spotflag == 28)
    {
        TiedOptionalData opData = tie->getOptionalData();
        
        *stream << QString("#%1%2%3").arg("Stfac", 19).arg("Visc", 20).arg("Istf", 30) << endl;
        *stream << gapStr(opData.Stfac) << gapStr(opData.Visc) << gapStr(opData.Istf, 30, false) << endl;
    }
    return true;
}

bool Radioss::FITKRadiossAdaptorInteractionTied::readInteraction(FITKInteractionKinematicTied * tie)
{
    if (tie == nullptr || _reader == nullptr) return false;
    //设置名称
    QString line = _reader->previousLine().trimmed();
    tie->setDataObjectName(line);
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
    if (lines.size() < 1|| lines.size() >4)
    {
        _reader->consoleMessage(3, QString("Invalid Tie : %1.").arg(tie->getDataObjectName()));
        return false;
    }
    //读取材料参数
    this->readParam(lines, tie);
    return true;
}

void Radioss::FITKRadiossAdaptorInteractionTied::readParam(const QStringList & lines, FITKInteractionKinematicTied * tie)
{
    if (lines.empty() || tie == nullptr) return;
    FITKInteractionKinematicTiedDefault* defTie = tie->getValue();
    if (!defTie) return;

    QString par = lines[0];
    bool ok = false;
    //从面
    int grnd_IDs = par.mid(0, 10).trimmed().toInt(&ok);
    if (ok) tie->setNodeGroupID(grnd_IDs);
    //主面
    int surf_IDm = par.mid(10, 10).trimmed().toInt(&ok);
    if (ok) tie->setMasterSurfaceID(surf_IDm);
    //忽略标志
    int Ignore = par.mid(20, 10).trimmed().toInt(&ok);
    if (ok) defTie->setIgnoreFlag(Ignore);
    //点焊公式标志
    int Spotflag = par.mid(30, 10).trimmed().toInt(&ok);
    if (ok) defTie->setSpotWeldFormulationFlag(Spotflag);
    //层级
    int Level = par.mid(40, 10).trimmed().toInt(&ok);
    if (ok) tie->setLevel(Level);
    //搜索公式标志
    int Isearch = par.mid(50, 10).trimmed().toInt(&ok);
    if (ok) defTie->setSearchFormulationFlag(Isearch);
    //节点删除阈值/标志
    int Idel2 = par.mid(60, 10).trimmed().toInt(&ok);
    if (ok) defTie->setNodeDeletionFlag(Idel2);
    //搜索距离
    double dsearch = par.mid(80, 20).trimmed().toDouble(&ok);
    if (ok) tie->setSearchDistance(dsearch);
   
    if(Spotflag==20|| Spotflag == 21|| Spotflag == 22)
    {
        TiedOptionalData& opData = tie->getOptionalData();
        //第2行
        par = lines.value(1);
        int Rupt= par.mid(0, 10).trimmed().toInt(&ok);
        if (ok) opData.Rupt = Rupt;
        int Ifilter = par.mid(10, 10).trimmed().toInt(&ok);
        if (ok) opData.Ifilter = Ifilter;
        int fct_IDsr = par.mid(20, 10).trimmed().toInt(&ok);
        if (ok) opData.fct_IDsr = fct_IDsr;
        int fct_IDsn = par.mid(30, 10).trimmed().toInt(&ok);
        if (ok) opData.fct_IDsn = fct_IDsn;
        int fct_IDst = par.mid(40, 10).trimmed().toInt(&ok);
        if (ok) opData.fct_IDst = fct_IDst;
        int Isym = par.mid(50, 10).trimmed().toInt(&ok);
        if (ok) opData.Isym = Isym;
        double Max_N_Dist = par.mid(60, 20).trimmed().toDouble(&ok);
        if (ok) opData.Max_N_Dist = Max_N_Dist;
        double Max_T_Dist = par.mid(80, 20).trimmed().toDouble(&ok);
        if (ok) opData.Max_T_Dist = Max_T_Dist;

        //第3行
        par = lines.value(2);
        double Fscale_stress = par.mid(0, 20).trimmed().toDouble(&ok);
        if (ok) opData.Fscale_stress = Fscale_stress;
        double Fscale_str_rate = par.mid(20, 20).trimmed().toDouble(&ok);
        if (ok) opData.Fscale_str_rate = Fscale_str_rate;
        double Fscale_dist = par.mid(40, 20).trimmed().toDouble(&ok);
        if (ok) opData.Fscale_dist = Fscale_dist;
        double Alpha = par.mid(60, 20).trimmed().toDouble(&ok);
        if (ok) opData.Alpha = Alpha;
        double Area = par.mid(80, 20).trimmed().toDouble(&ok);
        if (ok) opData.Area = Area;
    }
    if(Spotflag==25||Spotflag == 27 || Spotflag == 28)
    {
        TiedOptionalData& opData = tie->getOptionalData();
        //第2行
        par = lines.value(1);
        double Stfac = par.mid(0, 20).trimmed().toDouble(&ok);
        if (ok) opData.Stfac = Stfac;
        double Visc = par.mid(20, 20).trimmed().toDouble(&ok);
        if (ok) opData.Visc = Visc;
        int Istf = par.mid(60, 10).trimmed().toInt(&ok);
        if (ok) opData.Istf = Istf;
    }
   
}
