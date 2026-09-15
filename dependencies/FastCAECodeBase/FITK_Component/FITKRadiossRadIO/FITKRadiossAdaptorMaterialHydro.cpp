/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossAdaptorMaterialHydro.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKMaterialHydro.h"

#include "FITKRadiossRadReader.h"
#include "FITKRadiossRadWriter.h"
#include "RadKeyLineSpliter.h"
#include "RadReadInformationMapper.h"
#include "RadWriteInformationMapper.h"

namespace Radioss
{
    QString FITKRadiossAdaptorMaterialHydro::getAdaptorClass()
    {
        //返回数据类名 带命名空间
        return "Radioss::FITKRadiossAdaptorMaterialHydro";
    }
    bool FITKRadiossAdaptorMaterialHydro::adaptR()
    {
        if (_caseData == nullptr || _reader == nullptr) return false;
        // 创建塑性材料数据对象
        FITKMaterialHydro* material = new FITKMaterialHydro();
        //读取关键字
        RadKeyLineSpliter* keyLineSpliter = _reader->getKeyLineSpliter();
        RadReadInformationMapper* infoMapper = _reader->getReadRadInformationMapper();
        if (!keyLineSpliter || !infoMapper) return false;

        //获取id和关键字
        int mid = keyLineSpliter->getId(0);
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
    bool FITKRadiossAdaptorMaterialHydro::adaptW()
    {
        FITKMaterialHydro* material = dynamic_cast<FITKMaterialHydro*>(_dataObj);
        if (material == nullptr || _writer == nullptr) return false;

        FITKMaterialHydroData& matData = material->getMaterialData();
        QTextStream* stream = _writer->getStream();
        RadWriteInformationMapper* infoMapper = _writer->getWriteInpInformationMapper();
        if (stream == nullptr || infoMapper == nullptr) return false;
        //写出关键字行  ID 从1开始递增
        int radID = infoMapper->getMaxRadMaterialID() + 1;
        //建立映射关系
        infoMapper->insertMaterialIDMap(material->getDataObjectID(), radID);
        *stream << material->GetFITKMaterialHydroRadiossKeyWord() << "/" << radID << endl;
        //写出材料名称
        *stream << material->getDataObjectName() << endl;
        //RHO_I,RHO_0
        *stream << QString("#%1%2").arg("RHO_I", 19).arg("RHO_0", 20) << endl;
        *stream << gapStr(matData.m_density) << gapStr(matData.m_R_density) << endl;
        //Knu、Pmin
        *stream << QString("#%1%2").arg("Knu", 19).arg("Pmin", 20) << endl;
        *stream << gapStr(matData.m_kinematicViscosity) << gapStr(matData.m_pressureCut) << endl;
       
        return true;
    }
    bool FITKRadiossAdaptorMaterialHydro::readMaterial(FITKMaterialHydro* mat)
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
    bool FITKRadiossAdaptorMaterialHydro::readParam(const QStringList& lines, FITKMaterialHydro* mat)
    {
        if (lines.size() != 2 || mat == nullptr) return false;
        FITKMaterialHydroData& matData = mat->getMaterialData();

        QHash<int, double> matPara;
        bool ok = false;
        //RHO_I,RHO_0
        QStringList matPar = lines[0].simplified().split(' ');
        double RHO_I = matPar.value(0).toDouble(&ok);
        if (ok) matPara.insert(0, RHO_I);
        double RHO_0 = matPar.value(1).toDouble(&ok);
        if (ok) matPara.insert(1, RHO_0);
        //Knu、Pmin
        matPar = lines[1].simplified().split(' ');
        double Knu = matPar.value(0).toDouble(&ok);
        if (ok) matPara.insert(2, Knu);
        double Pmin = matPar.value(1).toDouble(&ok);
        if (ok) matPara.insert(3, Pmin);

        //获取对应参数值，如果不存在则为默认值
        matData.m_density = matPara.value(0, matData.m_density);
        matData.m_R_density = matPara.value(1, matData.m_R_density);
        matData.m_kinematicViscosity = matPara.value(2, matData.m_kinematicViscosity);
        matData.m_pressureCut = matPara.value(3, matData.m_pressureCut);

        return true;
    }
}

