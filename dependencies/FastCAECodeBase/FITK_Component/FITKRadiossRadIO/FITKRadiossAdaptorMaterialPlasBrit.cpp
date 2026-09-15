/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossAdaptorMaterialPlasBrit.h"

#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKMaterialPlasBrit.h"

#include "FITKRadiossRadReader.h"
#include "FITKRadiossRadWriter.h"
#include "RadKeyLineSpliter.h"
#include "RadReadInformationMapper.h"
#include "RadWriteInformationMapper.h"

namespace Radioss
{
    QString FITKRadiossAdaptorMaterialPlasBrit::getAdaptorClass()
    {
        //返回数据类名 带命名空间
        return "Radioss::FITKRadiossAdaptorMaterialPlasBrit";
    }

    bool FITKRadiossAdaptorMaterialPlasBrit::adaptR()
    {
        if (_caseData == nullptr || _reader == nullptr) return false;
        // 创建塑性材料数据对象
        FITKMaterialPlasBrit* material = new FITKMaterialPlasBrit();
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

    bool FITKRadiossAdaptorMaterialPlasBrit::adaptW()
    {
        FITKMaterialPlasBrit* material = dynamic_cast<FITKMaterialPlasBrit*>(_dataObj);
        if (material == nullptr || _writer == nullptr) return false;

        QTextStream* stream = _writer->getStream();
        RadWriteInformationMapper* infoMapper = _writer->getWriteInpInformationMapper();
        if (stream == nullptr || infoMapper == nullptr) return false;
        //写出关键字行  ID 从1开始递增
        int radID = infoMapper->getMaxRadMaterialID() + 1;
        //建立映射关系
        infoMapper->insertMaterialIDMap(material->getDataObjectID(), radID);
        *stream << material->GetFITKMaterialPlasBritRadiossKeyWord() << "/" << radID << endl;
        //写出材料名称
        *stream << material->getDataObjectName() << endl;
        //写出材料参数
        FITKMaterialPlasBritData data = material->getMaterialDataReadOnly();
        //密度
        *stream << QString("#%1").arg("RHO_I", 19) << endl;
        *stream << gapStr(data.m_density) << endl;
        //杨氏模量、泊松比
        *stream << QString("#%1%2").arg("E", 19).arg("v", 20) << endl;
        *stream << gapStr(data.m_youngsModulus) << gapStr(data.m_poissonsRatio) << endl;
        //a,b,n,SIG_max0
        *stream << QString("#%1%2%3%4").arg("a", 19).arg("b", 20).arg("n", 20).arg("SIG_max0", 40) << endl;
        *stream << gapStr(data.m_plasticityYieldStress) << gapStr(data.m_plasticityHardeningParameter)
            << gapStr(data.m_plasticityHardeningExponent) << gapStr(data.m_plasticityMaximumStress,40) << endl;
        //c,EPS_DOT_0,ICC,Fsmooth,Fcut
        *stream << QString("#%1%2%3%4%5").arg("c", 19).arg("EPS_DOT_0", 20).arg("ICC", 10).arg("Fsmooth", 10).arg("Fcut", 20) << endl;
        *stream << gapStr(data.m_strainRateCoefficient) << gapStr(data.m_referenceStrainRate)
            << gapStr(data.m_strainRateComputation, 10,false) << gapStr(data.m_strainRateSmoothing, 10,false)
            << gapStr(data.m_cutoffFreq, 20) << endl;
        //EPS_t1,EPS_m1,d_max1,EPS_f1
        *stream << QString("#%1%2%3%4").arg("EPS_t1", 19).arg("EPS_m1", 20).arg("d_max1", 20).arg("EPS_f1", 20) << endl;
        *stream << gapStr(data.m_tensileFilureStrain1) << gapStr(data.m_maximumTensileFilureStrain1)
            << gapStr(data.m_maximumDamageFactor1) << gapStr(data.m_maximumTensileStrain1) << endl;
        //EPS_t2,EPS_m2,d_max2,EPS_f2
        *stream << QString("#%1%2%3%4").arg("EPS_t2", 19).arg("EPS_m2", 20).arg("d_max2", 20).arg("EPS_f2", 20) << endl;
        *stream << gapStr(data.m_tensileFilureStrain2) << gapStr(data.m_maximumTensileFilureStrain2)
            << gapStr(data.m_maximumDamageFactor2) << gapStr(data.m_maximumTensileStrain2) << endl;

        return true;
    }

    bool FITKRadiossAdaptorMaterialPlasBrit::readMaterial(FITKMaterialPlasBrit * mat)
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
        if (lines.size() != 6)
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

    void FITKRadiossAdaptorMaterialPlasBrit::readParam(const QStringList & lines)
    {
        if (lines.size() != 6 || _reader == nullptr) return;
        //密度
        double pi = lines[0].mid(0, 20).simplified().toDouble();
        _materialParam.insert("RHO_I", pi);
        //杨氏模量、泊松比
        QList<QPair<QString, int>> matPar = { {"E", 20}, {"v", 20}};
        splitParam(lines[1], matPar);
        //a,b,n,SIG_max0
        matPar = { {"a", 20}, {"b", 20},{"n", 20 }, { "SIG_max0", 40 }};
        splitParam(lines[2], matPar);
        //c,EPS_DOT_0,ICC,Fsmooth,Fcut
        matPar = { {"c", 20}, {"EPS_DOT_0", 20},{"ICC", 10 }, { "Fsmooth", 10 }, { "Fcut", 20 } };
        splitParam(lines[3], matPar);
        //EPS_t1,EPS_m1,d_max1,EPS_f1
        matPar = { {"EPS_t1", 20}, {"EPS_m1", 20},{"d_max1", 20 }, { "EPS_f1", 20 } };
        splitParam(lines[4], matPar);
        //EPS_t2,EPS_m2,d_max2,EPS_f2
        matPar = { {"EPS_t2", 20}, {"EPS_m2", 20},{"d_max2", 20 }, { "EPS_f2", 20 } };
        splitParam(lines[5], matPar);

    }

    void FITKRadiossAdaptorMaterialPlasBrit::splitParam(const QString & line, const QList<QPair<QString, int>>& par)
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

    void FITKRadiossAdaptorMaterialPlasBrit::setMatData(FITKMaterialPlasBrit * mat)
    {
        if (mat == nullptr) return;
        //获取材料对象
        FITKMaterialPlasBritData& data = mat->getMaterialData();
        //对材料对象赋值
        data.m_density = _materialParam.value("RHO_I", data.m_density);
        data.m_youngsModulus = _materialParam.value("E", data.m_youngsModulus);
        data.m_poissonsRatio = _materialParam.value("v", data.m_poissonsRatio);
        data.m_plasticityYieldStress = _materialParam.value("a", data.m_plasticityYieldStress);
        data.m_plasticityHardeningParameter = _materialParam.value("b", data.m_plasticityHardeningParameter);
        data.m_plasticityHardeningExponent = _materialParam.value("n", data.m_plasticityHardeningExponent);
        data.m_plasticityMaximumStress = _materialParam.value("SIG_max0", data.m_plasticityMaximumStress);
        data.m_strainRateCoefficient = _materialParam.value("c", data.m_strainRateCoefficient);
        data.m_referenceStrainRate = _materialParam.value("EPS_DOT_0", data.m_referenceStrainRate);
        data.m_strainRateComputation = static_cast<int>(_materialParam.value("ICC", data.m_strainRateComputation));
        data.m_strainRateSmoothing = static_cast<int>(_materialParam.value("Fsmooth", data.m_strainRateSmoothing));
        data.m_cutoffFreq = _materialParam.value("Fcut", data.m_cutoffFreq);
        data.m_tensileFilureStrain1 = _materialParam.value("EPS_t1", data.m_tensileFilureStrain1);
        data.m_maximumTensileFilureStrain1 = _materialParam.value("EPS_m1", data.m_maximumTensileFilureStrain1);
        data.m_maximumDamageFactor1 = _materialParam.value("d_max1", data.m_maximumDamageFactor1);
        data.m_maximumTensileStrain1 = _materialParam.value("EPS_f1", data.m_maximumTensileStrain1);
        data.m_tensileFilureStrain2 = _materialParam.value("EPS_t2", data.m_tensileFilureStrain2);
        data.m_maximumTensileFilureStrain2 = _materialParam.value("EPS_m2", data.m_maximumTensileFilureStrain2);
        data.m_maximumDamageFactor2 = _materialParam.value("d_max2", data.m_maximumDamageFactor2);
        data.m_maximumTensileStrain2 = _materialParam.value("EPS_f2", data.m_maximumTensileStrain2);
    }

}