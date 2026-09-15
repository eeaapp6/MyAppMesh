/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossAdaptorMaterialPlasJohns.h"

#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKMaterialPlasJohns.h"

#include "FITKRadiossRadReader.h"
#include "FITKRadiossRadWriter.h"
#include "RadKeyLineSpliter.h"
#include "RadReadInformationMapper.h"
#include "RadWriteInformationMapper.h"
#include <QTextStream>

namespace Radioss
{
    QString FITKRadiossAdaptorMaterialPlasJohns::getAdaptorClass()
    {
        return "Radioss::FITKRadiossAdaptorMaterialPlasJohns";
    }

    bool FITKRadiossAdaptorMaterialPlasJohns::adaptR()
    {
        if (_caseData == nullptr || _reader == nullptr) return false;
        // 创建塑性材料数据对象
        FITKMaterialPlasJohns* material = new FITKMaterialPlasJohns();
        //读取关键字
        RadKeyLineSpliter *keyLineSpliter = _reader->getKeyLineSpliter();
        RadReadInformationMapper *infoMapper = _reader->getReadRadInformationMapper();
        if (!keyLineSpliter || !infoMapper) return false;

        //获取id和关键字
        int mid = keyLineSpliter->getId(0);
        //int uid = keyLineSpliter->getId(1);
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
            ok = this->readMaterial(material);
        }
        if (!ok)
        {
            _reader->consoleMessage(3, QString("Read Material Error : %1 %2").arg(keyLine).arg(material->getDataObjectName()));
            return false;
        }
        _reader->consoleMessage(1, QString("Read Material : %1 %2").arg(keyLine).arg(material->getDataObjectName()));
        _caseData->getMaterialManager()->appendDataObj(material);
        infoMapper->insertMaterialIDMap(mid, material->getDataObjectID());
        return true;
    }

    bool FITKRadiossAdaptorMaterialPlasJohns::adaptW()
    {
        FITKMaterialPlasJohns* material = dynamic_cast<FITKMaterialPlasJohns*>(_dataObj);
        if( material == nullptr || _writer == nullptr) return false;

        QTextStream* stream = _writer->getStream();
        RadWriteInformationMapper* infoMapper = _writer->getWriteInpInformationMapper();
        if(stream == nullptr || infoMapper == nullptr) return false;
        //写出关键字行  ID 从1开始递增
        int radID = infoMapper->getMaxRadMaterialID() + 1;
        //建立映射关系
        infoMapper->insertMaterialIDMap(material->getDataObjectID(), radID);
        *stream << material->GetFITKMaterialPlasJohnsRadiossKeyWord()<<"/"<< radID << endl;
        //写出材料名称
        *stream << material->getDataObjectName() << endl;

         //写出塑性材料参数
        FITKMaterialPlasJohnsData data = material->getMaterialDataReadOnly();
        auto type = material->getType();
        //密度
        *stream << QString("#%1").arg("RHO_I", 19) << endl;
        *stream << gapStr(data.m_density) << endl;

        //杨氏模量、泊松比、输入类型标志
        *stream << QString("#%1%2%3").arg("E", 19).arg("Nu", 20).arg("Iflag", 10) << endl;
        *stream << gapStr(data.m_youngsModulus) << gapStr(data.m_poissonsRatio) << gapStr(type,10,false) <<endl;

        //屈服应力、塑性硬化参数、塑料硬化指数、失效塑性应变、最大应力
        if(type== FITKMaterialPlasJohns::MaterialPlasJohnsType::MATERIAL_PLAS_JOHNS_TYPE_CLASSIC)
        {
            *stream << QString("#%1%2%3%4%5").arg("a", 19).arg("b", 20).arg("n", 20).arg("EPS_p_max", 20).arg("SIG_max0", 20) << endl;
            *stream << gapStr(data.m_yieldStress) << gapStr(data.m_plasticHardening) << gapStr(data.m_plasticHardeningExponent)
                << gapStr(data.m_failureStrain) << gapStr(data.m_maxStress) << endl;
        }
        //屈服应力、极限拉伸应力、工程应变、应变率系数、失效塑性应变、最大应力
        else if(type == FITKMaterialPlasJohns::MaterialPlasJohnsType::MATERIAL_PLAS_JOHNS_TYPE_SIMPLE)
        {
            *stream << QString("#%1%2%3%4%5").arg("SIG_y", 19).arg("UTS", 20).arg("EPS_UTS", 20).arg("EPS_p_max", 20).arg("SIG_max0", 20) << endl;
            *stream << gapStr(data.m_yieldStress) << gapStr(data.m_UTS) << gapStr(data.m_EpsilonUTS)
                << gapStr(data.m_failureStrain) << gapStr(data.m_maxStress) << endl;
        }
        //应变率、参考应变率、应变率计算标志、应变速率平滑标志、应变率平滑频率、硬化系数
        *stream << QString("#%1%2%3%4%5%6").arg("c", 19).arg("EPS_DOT_0", 20).arg("ICC", 10).arg("Fsmooth", 10).arg("F_cut", 20).arg("Chard", 20) << endl;
        *stream << gapStr(data.m_c) << gapStr(data.m_Epsilon0) << gapStr(data.m_ICC,10,false)
            << gapStr(data.m_Fsmooth,10,false) << gapStr(data.m_Fcut) << gapStr(data.m_Chard) << endl;

        //温度指数、熔化温度、比热容、参考温度
        *stream << QString("#%1%2%3%4").arg("m", 19).arg("T_melt", 20).arg("rhoC_p", 20).arg("T_r", 20) << endl;
        *stream << gapStr(data.m_tempExponent) << gapStr(data.m_meltingTemperature) << gapStr(data.m_rhoCp)
            << gapStr(data.m_Tref) << endl;

        return true;
    }

    bool FITKRadiossAdaptorMaterialPlasJohns::readMaterial(FITKMaterialPlasJohns* mat)
    {
        if (mat == nullptr || _reader == nullptr) return false;
        //设置材料名称
        QString line = _reader->previousLine().trimmed();
        mat->setDataObjectName(line);
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
        if (lines.size() != 5)
        {
            _reader->consoleMessage(3, QString("Invalid Material : %1.").arg(mat->getDataObjectName()));
            return false;
        }
        //读取材料参数
        this->readParam(lines);
        //材料对象赋值
        this->setMatData(mat);
        return true;
    }

    void FITKRadiossAdaptorMaterialPlasJohns::readParam(const QStringList & lines)
    {
        if (lines.size() != 5 || _reader == nullptr) return;
        //密度
        double pi = lines[0].mid(0, 20).simplified().toDouble();
        _materialParam.insert("RHO_I", pi);
        //杨氏模量、泊松比、输入类型标志
        QList<QPair<QString, int>> matPar1 = { {"E", 20}, {"Nu", 20}, {"Iflag", 10} };
        splitParam(lines[1], matPar1);
        int Iflag = _materialParam.value("Iflag") ;       
        //屈服应力、塑性硬化参数、塑料硬化指数、失效塑性应变、最大应力
        if (Iflag == 0)
        {
            QList<QPair<QString, int>> matPar2 = { {"a", 20}, {"b", 20}, {"n", 20}, {"EPS_p_max", 20}, {"SIG_max0", 20} };
            splitParam(lines[2], matPar2);
        }
        //屈服应力、极限拉伸应力、工程应变、应变率系数、失效塑性应变、最大应力
        else if (Iflag == 1)
        {
            QList<QPair<QString, int>> matPar2 = { {"SIG_y", 20}, {"UTS", 20}, {"EPS_UTS", 20}, {"EPS_p_max", 20}, {"SIG_max0", 20} };
            splitParam(lines[2], matPar2);
        }
        else
            _reader->consoleMessage(2, QString("Invalid Parameter : %1").arg(lines[1]));
        //应变率、参考应变率、应变率计算标志、应变速率平滑标志、应变率平滑频率、硬化系数
        QList<QPair<QString, int>> matPar3 = { {"c", 20}, {"EPS_DOT_0", 20}, {"ICC", 10}, {"Fsmooth", 10}, {"F_cut", 20}, {"Chard", 20} };
        splitParam(lines[3], matPar3);
        //温度指数、熔化温度、比热容、参考温度
        QList<QPair<QString, int>> matPar4 = { {"m", 20}, {"T_melt", 20}, {"rhoC_p", 20}, {"T_r", 20} };
        splitParam(lines[4], matPar4);
    }

    void FITKRadiossAdaptorMaterialPlasJohns::splitParam(const QString & line, const QList<QPair<QString, int>>& par)
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
                _materialParam.insert(parKey, parVal);
            split += width;
        }
    }

    void FITKRadiossAdaptorMaterialPlasJohns::setMatData(FITKMaterialPlasJohns* mat)
    {
        if (mat == nullptr) return;
        //获取塑性材料对象
        FITKMaterialPlasJohnsData& data = mat->getMaterialData();
        //对材料对象赋值
        data.m_density = _materialParam.value("RHO_I", data.m_density);
        data.m_youngsModulus = _materialParam.value("E", data.m_youngsModulus);
        data.m_poissonsRatio = _materialParam.value("Nu", data.m_poissonsRatio);
        int Iflag = _materialParam.value("Iflag", 0);
        mat->setType((FITKMaterialPlasJohns::MaterialPlasJohnsType)Iflag);
        if (Iflag == 0)
        {
            data.m_yieldStress = _materialParam.value("a", data.m_yieldStress);
            data.m_plasticHardening = _materialParam.value("b", data.m_plasticHardening);
            data.m_plasticHardeningExponent = _materialParam.value("n", data.m_plasticHardeningExponent);
            data.m_failureStrain = _materialParam.value("EPS_p_max", data.m_failureStrain);
            data.m_maxStress = _materialParam.value("SIG_max0", data.m_maxStress);
        }
        if (Iflag == 1)
        {
            data.m_yieldStress = _materialParam.value("SIG_y", data.m_yieldStress);
            data.m_UTS = _materialParam.value("UTS", data.m_UTS);
            data.m_EpsilonUTS = _materialParam.value("EPS_UTS", data.m_EpsilonUTS);
            data.m_failureStrain = _materialParam.value("EPS_p_max", data.m_failureStrain);
            data.m_maxStress = _materialParam.value("SIG_max0", data.m_maxStress);
        }
        data.m_c = _materialParam.value("c", data.m_c);
        data.m_Epsilon0 = _materialParam.value("EPS_DOT_0", data.m_Epsilon0);
        data.m_ICC = _materialParam.value("ICC", data.m_ICC);
        data.m_Fsmooth = _materialParam.value("Fsmooth", data.m_Fsmooth);
        data.m_Fcut = _materialParam.value("F_cut", data.m_Fcut);
        data.m_Chard = _materialParam.value("Chard", data.m_Chard);
        data.m_tempExponent = _materialParam.value("m", data.m_tempExponent);
        data.m_meltingTemperature = _materialParam.value("T_melt", data.m_meltingTemperature);
        data.m_rhoCp = _materialParam.value("rhoC_p", data.m_rhoCp);
        data.m_Tref = _materialParam.value("T_r", data.m_Tref);
    }
}


