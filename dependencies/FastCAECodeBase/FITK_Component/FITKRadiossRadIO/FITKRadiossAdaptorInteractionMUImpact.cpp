/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossAdaptorInteractionMUImpact.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKInteractionMultiUsageImpact.h"

#include "FITKRadiossRadReader.h"
#include "FITKRadiossRadWriter.h"
#include "RadKeyLineSpliter.h"
#include "RadReadInformationMapper.h"
#include "RadWriteInformationMapper.h"

namespace Radioss
{
    QString FITKRadiossAdaptorInteractionMUImpact::getAdaptorClass()
    {
        return "Radioss::FITKRadiossAdaptorInteractionMCImpact";
    }

    bool FITKRadiossAdaptorInteractionMUImpact::adaptR()
    {
        if (_caseData == nullptr || _reader == nullptr) return false;
        // 创建数据对象
        FITKInteractionMultiUsageImpact* muImpact = new FITKInteractionMultiUsageImpact();
        //读取关键字
        RadKeyLineSpliter *keyLineSpliter = _reader->getKeyLineSpliter();
        RadReadInformationMapper *infoMapper = _reader->getReadRadInformationMapper();
        if (!keyLineSpliter || !infoMapper) return false;

        //获取关键字
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
            //读取数据
            ok = this->readInteraction(muImpact);
        }
        if (!ok)
        {
            _reader->consoleMessage(3, QString("Read Multi Usage Impact Error : %1 %2").arg(keyLine).arg(muImpact->getDataObjectName()));
            return false;
        }
        _reader->consoleMessage(1, QString("Read Multi Usage Impact : %1 %2").arg(keyLine).arg(muImpact->getDataObjectName()));
        _caseData->getInteractionManager()->appendDataObj(muImpact);
        infoMapper->insertInteractionIDMap(radID, muImpact->getDataObjectID());
        return true;
    }

    bool FITKRadiossAdaptorInteractionMUImpact::adaptW()
    {
        FITKInteractionMultiUsageImpact* MUImpact = dynamic_cast<FITKInteractionMultiUsageImpact*>(_dataObj);
        if (MUImpact == nullptr || _writer == nullptr) return false;

        //可选参数
        MultiUsageImpactOptionalData& optionalData = MUImpact->getOptionalData();
        //部分必须参数
        FITKInteractionMultiUsageImpactDefaultValue* defValue = MUImpact->getValues();

        QTextStream* stream = _writer->getStream();
        RadWriteInformationMapper* infoMapper = _writer->getWriteInpInformationMapper();
        if (stream == nullptr || infoMapper == nullptr) return false;
        //写入关键字
        int radID = infoMapper->getMaxRadInteractionID() + 1;
        infoMapper->insertInteractionIDMap(MUImpact->getDataObjectID(), radID);
        *stream << MUImpact->GetFITKInteractionMultiUsageImpactRadiossKeyWord() << "/" << radID << endl;
        //写出名称
        *stream << MUImpact->getDataObjectName() << endl;
        //主面surf_ID
        int surf_ID = infoMapper->getRadIDBySurfaceID(MUImpact->getMasterSurfaceID());
        //从节点组grnd_ID
        int grnd_ID = infoMapper->getRadIDByNodeSetID(MUImpact->getNodeGroupID());

        //写出参数
        *stream << QString("#%1%2%3%4%5%6%7%8%9").arg("Slav_id", 9).arg("Mast_id", 10).arg("Istf", 10).arg("Ithe", 10)
            .arg("Igap", 10).arg("Ibag", 20).arg("Idel", 10).arg("Icurv", 10).arg("Iadm", 10) << endl;
        *stream << gapStr(grnd_ID) << gapStr(surf_ID) << gapStr(defValue->_Istf) << gapStr(MUImpact->_Ithe)
            << gapStr(defValue->_Igap) << gapStr(defValue->_Ibag,20) << gapStr(defValue->_Idel) << gapStr(MUImpact->_Icurv)
            << gapStr(MUImpact->_Iadm) << endl;

        *stream << QString("#%1%2%3%4").arg("Fscalegap", 19).arg("Gap_max", 20).arg("Fpenmax", 20).arg("Itied", 30) << endl;
        *stream << gapStr(MUImpact->_Fscale_gap) << gapStr(MUImpact->_GAPmax) << gapStr(MUImpact->_Fpenmax) 
            << gapStr(MUImpact->_Itied,30) << endl;

        *stream << QString("#%1%2%3%4%5%6").arg("STmin", 19).arg("STmax", 20).arg("mesh_size", 20).arg("dtmin", 20)
            .arg("Irem_gap", 10).arg("Irem_i2", 10) << endl;
        *stream << gapStr(MUImpact->_STmin) << gapStr(MUImpact->_STmax) << gapStr(MUImpact->_mesh_size) << gapStr(MUImpact->_dtmin)
            << gapStr(defValue->_Irem_gap) << gapStr(defValue->_Irem_i2) << endl;

        if (MUImpact->_Icurv == 1 || MUImpact->_Icurv == 2)
        {
            *stream << QString("#%1%2").arg("node_ID1", 9).arg("node_ID2", 10) << endl;
            *stream << gapStr(optionalData.node_ID1) << gapStr(optionalData.node_ID2) << endl;
        }

        *stream << QString("#%1%2%3%4%5").arg("Stfac", 19).arg("Fric", 20).arg("Gapmin", 20)
            .arg("Tstart", 20).arg("Tstop", 20) << endl;
        *stream << gapStr(MUImpact->_Stfac) << gapStr(MUImpact->_Fric) << gapStr(MUImpact->_Gapmin)
            << gapStr(MUImpact->_Tstart) << gapStr(MUImpact->_Tstop) << endl;

        *stream << QString("#%1%2%3%4%5").arg("IBC", 9).arg("Inacti", 30).arg("VisS", 20)
            .arg("VisF", 20).arg("Bumult", 20) << endl;
        *stream << QString("       %1").arg(MUImpact->_Ibc, 3, 10, QLatin1Char('0')) << gapStr(defValue->_Inacti, 30) 
            << gapStr(MUImpact->_VisS) << gapStr(MUImpact->_VisF) << gapStr(MUImpact->_Bumult) << endl;

        *stream << QString("#%1%2%3%4%5%6%7%8").arg("Ifric", 9).arg("Ifiltr", 10).arg("Xfreq", 20)
            .arg("Iform", 10).arg("sens_ID", 10).arg("fct_IDf", 10).arg("AscaleF", 20).arg("fric_ID", 10) << endl;
        *stream << gapStr(MUImpact->_Ifric) << gapStr(MUImpact->_Ifiltr) << gapStr(MUImpact->_Xfreq)
            << gapStr(defValue->_Iform) << gapStr(MUImpact->_sens_ID) << gapStr(MUImpact->_fct_IDf)
            << gapStr(MUImpact->_AscaleF) << gapStr(MUImpact->_fric_ID) << endl;

        if (MUImpact->_Ifric > 0)
        {
            *stream << QString("#%1%2%3%4%5").arg("C1", 19).arg("C2", 20).arg("C3", 20).arg("C4", 20).arg("C5", 20) << endl;
            *stream << gapStr(optionalData.C1) << gapStr(optionalData.C2) << gapStr(optionalData.C3)
                << gapStr(optionalData.C4) << gapStr(optionalData.C5) << endl;
        }
        if (MUImpact->_Ifric > 1)
        {
            *stream << QString("#%1").arg("C6", 19) << endl;
            *stream << gapStr(optionalData.C6) << endl;
        }
        if (MUImpact->_Iadm == 2)
        {
            *stream << QString("#%1%2%3").arg("NRadm", 9).arg("Padm", 20).arg("Angladm", 20) << endl;
            *stream << gapStr(optionalData.NRadm) << gapStr(optionalData.Padm) << gapStr(optionalData.Angladm) << endl;
        }
        if (MUImpact->_Ithe == 1)
        {
            *stream << QString("#%1%2%3%4%5").arg("Kthe", 19).arg("fct_IDK", 10).arg("Tint", 30)
                .arg("Ithe_form", 10).arg("AscaleK", 20) << endl;
            *stream << gapStr(optionalData.Kthe) << gapStr(optionalData.fct_IDK) << gapStr(optionalData.Tint, 30)
                << gapStr(optionalData.Ithe_form) << gapStr(optionalData.AscaleK) << endl;
            *stream << QString("#%1%2%3%4").arg("Frad", 19).arg("Drad", 20).arg("Fheats", 20).arg("Fheatm", 20) << endl;
            *stream << gapStr(optionalData.Frad) << gapStr(optionalData.Drad) << gapStr(optionalData.Fheats)
                << gapStr(optionalData.Fheatm) << endl;
        }



        return true;
    }

    bool FITKRadiossAdaptorInteractionMUImpact::readInteraction(FITKInteractionMultiUsageImpact * MUImpact)
    {
        if (MUImpact == nullptr || _reader == nullptr) return false;
        //设置名称
        QString line = _reader->previousLine().trimmed();
        MUImpact->setDataObjectName(line);
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

        //读取参数
        this->readParam(lines);
        //对象赋值
        this->setInteractionData(MUImpact);
        return true;
    }

    void FITKRadiossAdaptorInteractionMUImpact::readParam(const QStringList & lines)
    {
        if (lines.size() < 6)return;
        int lineCount = 0;
        //第1行
        QList<QPair<QString, double>> pairList = { {"Slav_id", 10}, {"Mast_id", 10}, {"Istf", 10}, {"Ithe", 10}
                ,{"Igap", 10}, {"", 10}, {"Ibag", 10}, {"Idel", 10}, {"Icurv", 10}, {"Iadm", 10} };
        splitParam(lines.value(lineCount++), pairList);
        //第2行
        pairList= { {"Fscalegap", 20}, {"Gap_max", 20}, {"Fpenmax", 20}, {"", 20}, {"Itied", 10} };
        splitParam(lines.value(lineCount++), pairList);
        //第3行
        pairList = { {"STmin", 20}, {"STmax", 20}, {"mesh_size", 20}, {"dtmin", 20}, {"Irem_gap", 10}, {"Irem_i2", 10} };
        splitParam(lines.value(lineCount++), pairList);
        //第4行到最后
        if (_MUIParam.value("Icurv", 0) == 1 || _MUIParam.value("Icurv", 0) == 2)
        {
            pairList = { {"node_ID1", 10}, {"node_ID2", 10} };
            splitParam(lines.value(lineCount++), pairList);
        }
        pairList = { {"Stfac", 20}, {"Fric", 20}, {"Gapmin", 20}, {"Tstart", 20}, {"Tstop", 20} };
        splitParam(lines.value(lineCount++), pairList);
        pairList = { {"IBC", 10}, {"", 20}, {"Inacti", 10}, {"VisS", 20}, {"VisF", 20}, {"Bumult", 20} };
        splitParam(lines.value(lineCount++), pairList);
        pairList = { {"Ifric", 10}, {"Ifiltr", 10}, {"Xfreq", 20}, {"Iform", 10}, {"sens_ID", 10}, {"fct_IDf", 10}
                , {"AscaleF", 20} , {"fric_ID", 10} };
        splitParam(lines.value(lineCount++), pairList);
        //可选参数
        if (_MUIParam.value("Ifric", 0) > 0 )
        {
            pairList = { {"C1", 20}, {"C2", 20}, {"C3", 20}, {"C4", 20}, {"C5", 20} };
            splitParam(lines.value(lineCount++), pairList);
        }
        if (_MUIParam.value("Ifric", 0) > 1)
        {
            pairList = { {"C6", 20} };
            splitParam(lines.value(lineCount++), pairList);
        }
        if (_MUIParam.value("Iadm", 0) == 2)
        {
            pairList = { {"NRadm", 10}, {"Padm", 20}, {"Angladm", 20} };
            splitParam(lines.value(lineCount++), pairList);
        }
        if (_MUIParam.value("Ithe", 0) == 1)
        {
            pairList = { {"Kthe", 20}, {"fct_IDK", 10}, {"Tint", 30}, {"Ithe_form", 10}, {"AscaleK", 20} };
            splitParam(lines.value(lineCount++), pairList);
            pairList = { {"Frad", 20}, {"Drad", 20}, {"Fheats", 20}, {"Fheatm", 20} };
            splitParam(lines.value(lineCount++), pairList);
        }

    }

    void FITKRadiossAdaptorInteractionMUImpact::splitParam(const QString & line, const QList<QPair<QString, double>>& par)
    {
        if (line.isEmpty())return;
        //字符串分割位置
        int split = 0;
        for (auto it = par.begin(); it != par.end(); ++it)
        {
            bool ok = false;
            const QString& parKey = it->first;
            int width = it->second;
            if (parKey.isEmpty()) {
                split += width;
                continue;
            }
            if (width > 10) {
                double parVal = line.mid(split, width).simplified().toDouble(&ok);
                //如果转double为空值，则跳过
                if (ok)
                    _MUIParam.insert(parKey, parVal);
            }
            else {
                int parVal = line.mid(split, width).simplified().toInt(&ok);
                //如果转int为空值，则跳过
                if (ok)
                    _MUIParam.insert(parKey, parVal);
            }
            split += width;
        }
    }

    void FITKRadiossAdaptorInteractionMUImpact::setInteractionData(FITKInteractionMultiUsageImpact * MUImpact)
    {
        if (MUImpact == nullptr) return;
        Radioss::RadReadInformationMapper* mapper = _reader->getReadRadInformationMapper();
        if (!mapper) return ;
        //可选参数
        MultiUsageImpactOptionalData& optionalData = MUImpact->getOptionalData();
        //部分必须参数
        FITKInteractionMultiUsageImpactDefaultValue* defValue = MUImpact->getValues();
        //第1行
        MUImpact->setNodeGroupID(_MUIParam.value("Slav_id", -1));
        MUImpact->setMasterSurfaceID(_MUIParam.value("Mast_id", -1));
        defValue->_Istf = _MUIParam.value("Istf", defValue->_Istf);
        MUImpact->_Ithe = _MUIParam.value("Ithe", MUImpact->_Ithe);
        defValue->_Igap = _MUIParam.value("Igap", defValue->_Igap);
        defValue->_Ibag = _MUIParam.value("Ibag", defValue->_Ibag);
        defValue->_Idel = _MUIParam.value("Idel", defValue->_Idel);
        MUImpact->_Icurv = _MUIParam.value("Icurv", MUImpact->_Icurv);
        MUImpact->_Iadm = _MUIParam.value("Iadm", MUImpact->_Iadm);
        //第2行
        MUImpact->_Fscale_gap = _MUIParam.value("Fscalegap", MUImpact->_Fscale_gap);
        MUImpact->_GAPmax = _MUIParam.value("Gap_max", MUImpact->_GAPmax);
        MUImpact->_Fpenmax = _MUIParam.value("Fpenmax", MUImpact->_Fpenmax);
        MUImpact->_Itied = _MUIParam.value("Itied", MUImpact->_Itied);
        //第3行
        MUImpact->_STmin = _MUIParam.value("STmin", MUImpact->_STmin);
        MUImpact->_STmax = _MUIParam.value("STmax", MUImpact->_STmax);
        MUImpact->_mesh_size = _MUIParam.value("mesh_size", MUImpact->_mesh_size);
        MUImpact->_dtmin = _MUIParam.value("dtmin", MUImpact->_dtmin);
        defValue->_Irem_gap = _MUIParam.value("Irem_gap", defValue->_Irem_gap);
        defValue->_Irem_i2 = _MUIParam.value("Irem_i2", defValue->_Irem_i2);
        //第4行（可选）
        if (MUImpact->_Icurv == 1 || MUImpact->_Icurv == 2)
        {
            optionalData.node_ID1 = _MUIParam.value("node_ID1", optionalData.node_ID1);
            optionalData.node_ID2 = _MUIParam.value("node_ID2", optionalData.node_ID2);
        }
        //剩余行
        MUImpact->_Stfac = _MUIParam.value("Stfac", MUImpact->_Stfac);
        MUImpact->_Fric = _MUIParam.value("Fric", MUImpact->_Fric);
        MUImpact->_Gapmin = _MUIParam.value("Gapmin", MUImpact->_Gapmin);
        MUImpact->_Tstart = _MUIParam.value("Tstart", MUImpact->_Tstart);
        MUImpact->_Tstop = _MUIParam.value("Tstop", MUImpact->_Tstop);

        MUImpact->_Ibc = _MUIParam.value("IBC", MUImpact->_Ibc);
        defValue->_Inacti = _MUIParam.value("Inacti", defValue->_Inacti);
        MUImpact->_VisS = _MUIParam.value("VisS", MUImpact->_VisS);
        MUImpact->_VisF = _MUIParam.value("VisF", MUImpact->_VisF);
        MUImpact->_Bumult = _MUIParam.value("Bumult", MUImpact->_Bumult);

        MUImpact->_Ifric = _MUIParam.value("Ifric", MUImpact->_Ifric);
        MUImpact->_Ifiltr = _MUIParam.value("Ifiltr", MUImpact->_Ifiltr);
        MUImpact->_Xfreq = _MUIParam.value("Xfreq", MUImpact->_Xfreq);
        defValue->_Iform = _MUIParam.value("Iform", defValue->_Iform);
        MUImpact->_sens_ID = _MUIParam.value("sens_ID", MUImpact->_sens_ID);
        MUImpact->_fct_IDf = _MUIParam.value("fct_IDf", MUImpact->_fct_IDf);
        MUImpact->_AscaleF = _MUIParam.value("AscaleF", MUImpact->_AscaleF);
        MUImpact->_fric_ID = _MUIParam.value("fric_ID", MUImpact->_fric_ID);

        //可选参数
        if (MUImpact->_Ifric > 0)
        {
            optionalData.C1 = _MUIParam.value("C1", optionalData.C1);
            optionalData.C2 = _MUIParam.value("C2", optionalData.C2);
            optionalData.C3 = _MUIParam.value("C3", optionalData.C3);
            optionalData.C4 = _MUIParam.value("C4", optionalData.C4);
            optionalData.C5 = _MUIParam.value("C5", optionalData.C5);
        }
        if (MUImpact->_Ifric > 1)
        {
            optionalData.C6 = _MUIParam.value("C6", optionalData.C6);
        }
        if (MUImpact->_Iadm == 2)
        {
            optionalData.NRadm = _MUIParam.value("NRadm", optionalData.NRadm);
            optionalData.Padm = _MUIParam.value("Padm", optionalData.Padm);
            optionalData.Angladm = _MUIParam.value("Angladm", optionalData.Angladm);
        }
        if (MUImpact->_Ithe == 1)
        {
            optionalData.Kthe = _MUIParam.value("Kthe", optionalData.Kthe);
            optionalData.fct_IDK = _MUIParam.value("fct_IDK", optionalData.fct_IDK);
            optionalData.Tint = _MUIParam.value("Tint", optionalData.Tint);
            optionalData.Ithe_form = _MUIParam.value("Ithe_form", optionalData.Ithe_form);
            optionalData.AscaleK = _MUIParam.value("AscaleK", optionalData.AscaleK);
            optionalData.Frad = _MUIParam.value("Frad", optionalData.Frad);
            optionalData.Drad = _MUIParam.value("Drad", optionalData.Drad);
            optionalData.Fheats = _MUIParam.value("Fheats", optionalData.Fheats);
            optionalData.Fheatm = _MUIParam.value("Fheatm", optionalData.Fheatm);
        }
    }
}