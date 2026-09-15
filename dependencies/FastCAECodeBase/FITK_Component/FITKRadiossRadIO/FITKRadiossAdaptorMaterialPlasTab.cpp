/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossAdaptorMaterialPlasTab.h"

#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKMaterialPlasTab.h"

#include "FITKRadiossRadReader.h"
#include "FITKRadiossRadWriter.h"
#include "RadKeyLineSpliter.h"
#include "RadReadInformationMapper.h"
#include "RadWriteInformationMapper.h"
#include <QTextStream>

namespace Radioss
{
    QString FITKRadiossAdaptorMaterialPlasTab::getAdaptorClass()
    {
        return "Radioss::FITKRadiossAdaptorMaterialPlasTab";
    }

    bool FITKRadiossAdaptorMaterialPlasTab::adaptR()
    {
        if (_caseData == nullptr || _reader == nullptr) return false;
        // 创建塑性材料数据对象
        FITKMaterialPlasTab* material = new FITKMaterialPlasTab();
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

    bool FITKRadiossAdaptorMaterialPlasTab::adaptW()
    {
        FITKMaterialPlasTab* material = dynamic_cast<FITKMaterialPlasTab*>(_dataObj);
        if( material == nullptr || _writer == nullptr) return false;

        QTextStream* stream = _writer->getStream();
        RadWriteInformationMapper* infoMapper = _writer->getWriteInpInformationMapper();
        if(stream == nullptr || infoMapper == nullptr) return false;
        //写出关键字行  ID 从1开始递增
        int radID = infoMapper->getMaxRadMaterialID() + 1;
        //建立映射关系
        infoMapper->insertMaterialIDMap(material->getDataObjectID(), radID);
        *stream << material->GetFITKMaterialPlasTabRadiossKeyWord()<<"/"<< radID << endl;
        //写出材料名称
        *stream << material->getDataObjectName() << endl;

        //写出塑性材料参数
        FITKMaterialPlasTabData data = material->getMaterialDataReadOnly();
        //密度
        *stream << QString("#%1").arg("Pi", 19) << endl;
        *stream << gapStr(data.m_density) << endl;

        //杨氏模量、泊松比、失效应变、拉伸失效应变、最大拉伸失效应变
        *stream << QString("#%1%2%3%4%5").arg("E",19).arg("v", 20).arg("Emaxp", 20).arg("Et", 20).arg("Em", 20) << endl;
        *stream << gapStr(data.m_youngsModulus)<< gapStr(data.m_poissonsRatio)<< gapStr(data.m_failureStrain)
            << gapStr(data.m_tensileFailureStrain)<< gapStr(data.m_maxTensileFailureStrain) << endl;

        //函数个数、平滑应变率设置、硬化指数、应变率滤波的截止频率、元素缺失拉伸应变、应变率选择标志
        *stream << QString("#%1%2%3%4%5%6").arg("Nfunct", 9).arg("Fsmooth", 10).arg("Chard", 20).arg("Fcut", 20).arg("Ef", 20).arg("VP", 20) << endl;
        *stream << gapStr(data.m_numberOfFunctions,10,false) << gapStr(data.m_smoothStrainRateOptFlag,10,false) << gapStr(data.m_Chard) 
            << gapStr(data.m_Fcut) << gapStr(data.m_tensileStrainForElementDeletion) << gapStr(data.m_vpFlag,20,false) << endl;

        //屈服因子与压力的函数、fct_IDp尺度因子、杨氏模量比例因子的函数、无限塑性应变时的饱和杨氏模量、杨氏模量演化参数
        *stream << QString("#%1%2%3%4%5").arg("fct_IDp", 9).arg("Fscale", 20).arg("fct_IDE", 10).arg("Einf", 20).arg("CE", 20) << endl;
        *stream << gapStr(data.m_fctIDp) << gapStr(data.m_fscaleFactor) << gapStr(data.m_fctIODE,10,false)
            << gapStr(data.m_Eint) << gapStr(data.m_Ce) << endl;

        if (data.m_numberOfFunctions < 1)return true;
        if(data.m_fctID.size()!= data.m_numberOfFunctions|| 
            data.m_fctScale.size() != data.m_numberOfFunctions ||
            data.m_epsilon.size() != data.m_numberOfFunctions ) return false;

        //函数ID
        int row = 1;
        for (int i = 0; i < data.m_numberOfFunctions;++i) {
            int id = infoMapper->getRadIDByCurveID(data.m_fctID[i]);
            *stream << gapStr(id);
            if (row == 5) {
                *stream << endl;
                row = 1;
            }
            ++row;
        }
        if(row!=5) *stream << endl;
        //尺度因子
        row = 1;
        for (int i = 0; i < data.m_numberOfFunctions; ++i) {
            *stream << gapStr(data.m_fctScale[i]);
            if (row == 5) {
                *stream << endl;
                row = 1;
            }
            ++row;
        }
        if (row != 5) *stream << endl;
        // ε
        row = 1;
        for (int i = 0; i < data.m_numberOfFunctions; ++i) {
            *stream << gapStr(data.m_epsilon[i]);
            if (row == 5) {
                *stream << endl;
                row = 1;
            }
            ++row;
        }
        if (row != 5) *stream << endl;

        return true;
    }

    bool FITKRadiossAdaptorMaterialPlasTab::readMaterial(FITKMaterialPlasTab* mat)
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
        //if (lines.size() != 7)
        //{
        //    _reader->consoleMessage(3, QString("Invalid Material : %1.").arg(mat->getDataObjectName()));
        //    return false;
        //}
        //读取材料参数
        this->readParam(lines);
        //材料对象赋值
        this->setMatData(mat);
        return true;
    }

    void FITKRadiossAdaptorMaterialPlasTab::readParam(const QStringList & lines)
    {
        if( lines.size() < 4 )return;
        //密度
        double pi = lines[0].mid(0, 20).simplified().toDouble();
        _materialParam.insert("Pi", pi);
        //杨氏模量、泊松比、失效应变、拉伸失效应变、最大拉伸失效应变
        QList<QPair<QString, double>> matPar1 = { {"E", 20}, {"v", 20}, {"Emaxp", 20}, {"Et", 20}, {"Em", 20} };
        splitParam(lines[1], matPar1);
        //函数个数、平滑应变率设置、硬化指数、应变率滤波的截止频率、元素缺失拉伸应变、应变率选择标志
        QList<QPair<QString, double>> matPar2 = { {"Nfunct", 10}, {"Fsmooth", 10}, {"Chard", 20}, {"Fcut", 20}, { "Ef" , 20}, {"VP", 20} };
        splitParam(lines[2], matPar2);
        //屈服因子与压力的函数、fct_IDp尺度因子、杨氏模量比例因子的函数、无限塑性应变时的饱和杨氏模量、杨氏模量演化参数
        QList<QPair<QString, double>> matPar3 = { {"fct_IDp", 10}, {"Fscale", 20}, {"fct_IDE", 10}, {"Einf", 20}, { "CE" , 20} };
        splitParam(lines[3], matPar3);

        //获取函数个数
        int funcNum = _materialParam.value("Nfunct",1);
        if (funcNum < 1)return;
        //最后一行个数
        int endLineNum = funcNum % 5;
        //当前splitParam行数
        int line = 4;
        //函数ID和尺度因子和e的总数
        int total = 1;
        //函数ID
        for (int i = 0; i <= funcNum/5; ++i) {
            QList<QPair<QString, double>> matPar{};
            if (i < funcNum / 5) 
            {
                for (int j = 0; j < 5; ++j) {
                    matPar.append({ QString::number(total++), 10 });
                }
            }
            else if (i == funcNum / 5)
            {
                for (int j = 0; j < endLineNum; ++j) {
                    matPar.append({ QString::number(total++), 10 });
                }
            }
            splitParam(lines.value(line), matPar);
            line++;          
        }
        //尺度因子
        for (int i = 0; i <= funcNum / 5; ++i) {
            QList<QPair<QString, double>> matPar{};
            if (i < funcNum / 5)
            {
                for (int j = 0; j < 5; ++j) {
                    matPar.append({ QString::number(total++), 20 });
                }
            }
            else if (i == funcNum / 5)
            {
                for (int j = 0; j < endLineNum; ++j) {
                    matPar.append({ QString::number(total++), 20 });
                }
            }
            splitParam(lines.value(line), matPar);
            line++;
        }
        //e
        for (int i = 0; i <= funcNum / 5; ++i) {
            QList<QPair<QString, double>> matPar{};
            if (i < funcNum / 5)
            {
                for (int j = 0; j < 5; ++j) {
                    matPar.append({ QString::number(total++), 20 });
                }
            }
            else if (i == funcNum / 5)
            {
                for (int j = 0; j < endLineNum; ++j) {
                    matPar.append({ QString::number(total++), 20 });
                }
            }
            splitParam(lines.value(line), matPar);
            line++;
        }
        //total应当 等于 函数个数乘3
    }

    void FITKRadiossAdaptorMaterialPlasTab::splitParam(const QString & line, const QList<QPair<QString, double>>& par)
    {
        //字符串分割位置
        int split = 0;
        for (auto it = par.begin(); it != par.end(); ++it)
        {
            bool ok = false;
            const QString& parKey = it->first;
            int width = it->second;
            if (width > 10) {
                double parVal = line.mid(split, width).simplified().toDouble(&ok);
                //如果转double为空值，则跳过
                if (ok)
                    _materialParam.insert(parKey, parVal);
            }
            else {
                int parVal = line.mid(split, width).simplified().toInt(&ok);
                //如果转double为空值，则跳过
                if (ok)
                    _materialParam.insert(parKey, parVal);
            }
            split += width;
        }
    }

    void FITKRadiossAdaptorMaterialPlasTab::setMatData(FITKMaterialPlasTab* mat)
    {
        if (mat == nullptr) return;
        //获取塑性材料对象
        FITKMaterialPlasTabData& data = mat->getMaterialData();
        //对材料对象赋值
        data.m_density = _materialParam.value("Pi", data.m_density);
        data.m_youngsModulus = _materialParam.value("E", data.m_youngsModulus);
        data.m_poissonsRatio = _materialParam.value("v", data.m_poissonsRatio);
        data.m_failureStrain = _materialParam.value("Emaxp", data.m_failureStrain);
        data.m_tensileFailureStrain = _materialParam.value("Et", data.m_tensileFailureStrain);
        data.m_maxTensileFailureStrain = _materialParam.value("Em", data.m_maxTensileFailureStrain);
        data.m_numberOfFunctions = _materialParam.value("Nfunct", data.m_numberOfFunctions);
        data.m_smoothStrainRateOptFlag = _materialParam.value("Fsmooth", data.m_smoothStrainRateOptFlag);
        data.m_Chard = _materialParam.value("Chard", data.m_Chard);
        data.m_Fcut = _materialParam.value("Fcut", data.m_Fcut);
        data.m_tensileStrainForElementDeletion = _materialParam.value("Ef", data.m_tensileStrainForElementDeletion);
        data.m_vpFlag = _materialParam.value("VP", data.m_vpFlag);
        data.m_fctIDp = _materialParam.value("fct_IDp", data.m_fctIDp);
        data.m_fscaleFactor = _materialParam.value("Fscale", data.m_fscaleFactor);
        data.m_fctIODE = _materialParam.value("fct_IDE", data.m_fctIODE);
        data.m_Eint = _materialParam.value("Einf", data.m_Eint);
        data.m_Ce = _materialParam.value("CE", data.m_Ce);

        int funcNum = data.m_numberOfFunctions;
        if (funcNum < 1)return;
        data.m_fctID.clear();
        data.m_fctScale.clear();
        data.m_epsilon.clear();
        //函数ID
        for (int i = 1; i <= funcNum; ++i) {
            data.m_fctID.append( _materialParam.value(QString::number(i), -1));
        }
        //尺度因子
        for (int i = funcNum + 1; i <= funcNum * 2; ++i) {
            data.m_fctScale.append(_materialParam.value(QString::number(i), 1.0));
        }
        //e
        for (int i = funcNum*2 + 1; i <= funcNum * 3; ++i) {
            data.m_epsilon.append(_materialParam.value(QString::number(i), 0.0));
        }
    }
}


