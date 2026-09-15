/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossAdaptorMaterialElastic.h"

#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKMaterialElastic.h"

#include "FITKRadiossRadReader.h"
#include "FITKRadiossRadWriter.h"
#include "RadKeyLineSpliter.h"
#include "RadReadInformationMapper.h"
#include "RadWriteInformationMapper.h"

namespace Radioss
{
    QString FITKRadiossAdaptorMaterialElastic::getAdaptorClass()
    {
        //返回数据类名 带命名空间
        return "Radioss::FITKRadiossAdaptorMaterialElastic";
    }
    bool FITKRadiossAdaptorMaterialElastic::adaptR()
    {
        if (_caseData == nullptr || _reader == nullptr) return false;
        // 创建塑性材料数据对象
        FITKMaterialElastic* material = new FITKMaterialElastic();
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
    bool FITKRadiossAdaptorMaterialElastic::adaptW()
    {
        FITKMaterialElastic* material = dynamic_cast<FITKMaterialElastic*>(_dataObj);
        if (material == nullptr || _writer == nullptr) return false;

        QTextStream* stream = _writer->getStream();
        RadWriteInformationMapper* infoMapper = _writer->getWriteInpInformationMapper();
        if (stream == nullptr || infoMapper == nullptr) return false;
        //写出关键字行  ID 从1开始递增
        int radID = infoMapper->getMaxRadMaterialID() + 1;
        //建立映射关系
        infoMapper->insertMaterialIDMap(material->getDataObjectID(), radID);
        *stream << material->GetFITKMaterialElasticRadiossKeyWord() << "/" << radID << endl;
        //写出材料名称
        *stream << material->getDataObjectName() << endl;

        //密度
        *stream << "#              RHO_I" << endl;
        *stream << QString("%1").arg(QString::number(material->getDensity(),'e', 6),20) << endl;

        //杨氏模量、泊松比
        *stream << "#                  E                  Nu" << endl;
        *stream << QString("%1").arg(QString::number(material->getYoungsModulus(), 'e', 6), 20) 
            << QString("%1").arg(QString::number(material->getPoissonsRatio(), 'e', 6), 20) << endl;

        return true;
    }

    bool FITKRadiossAdaptorMaterialElastic::readMaterial(FITKMaterialElastic * mat)
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
        if (lines.size() != 2)
        {
            _reader->consoleMessage(3, QString("Invalid Material : %1.").arg(mat->getDataObjectName()));
            return false;
        }
        //读取材料参数
        this->readParam(lines, mat);
        return true;
    }

    bool FITKRadiossAdaptorMaterialElastic::readParam(const QStringList & lines, FITKMaterialElastic* mat)
    {
        if (lines.size() != 2 || mat == nullptr) return false;

        QHash<int, double> matPara;
        bool ok = false;
        //材料密度
        double Pi = lines[0].simplified().toDouble(&ok);
        if (ok) matPara.insert(0, Pi);
        //杨氏模量、泊松比
        QStringList matParEv = lines[1].simplified().split(' ');
        double E = matParEv.value(0).toDouble(&ok);
        if (ok) matPara.insert(1, E);
        double v = matParEv.value(1).toDouble(&ok);
        if (ok) matPara.insert(2, v);

        //获取对应参数值，如果不存在则为默认值
        mat->setDensity(matPara.value(0, mat->getDensity()));
        mat->setYoungsModulus(matPara.value(1, mat->getYoungsModulus()));
        mat->setPoissonsRatio(matPara.value(2, mat->getPoissonsRatio()));

        return true;
    }
}

