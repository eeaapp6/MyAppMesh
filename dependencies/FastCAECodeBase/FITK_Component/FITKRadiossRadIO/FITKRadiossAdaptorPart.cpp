/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

//RadiossData
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossNodesParts.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossMeshModel.h"
#include "FITK_Interface/FITKInterfaceModel/FITKElementFactory.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractElement.h"
//Adaptor
#include "FITKRadiossAdaptorPart.h"
#include "FITKRadiossAdaptorPartElements.h"

#include "FITKRadiossRadReader.h"
#include "FITKRadiossRadWriter.h"
#include "RadKeyLineSpliter.h"
#include "RadReadInformationMapper.h"
#include "RadWriteInformationMapper.h"

#include <QList>

namespace Radioss
{
    QString FITKRadiossAdaptorPart::getAdaptorClass()
    {
        return "Radioss::FITKRadiossAdaptorPart";
    }

    bool FITKRadiossAdaptorPart::adaptR()
    {
        if (_caseData == nullptr || _reader == nullptr) return false;
        
        // 读取关键字
        RadKeyLineSpliter *keyLineSpliter = _reader->getKeyLineSpliter();
        if (!keyLineSpliter) return false;
        int pid = keyLineSpliter->getId();
        QString keyLine = keyLineSpliter->getCurrentLine();
        //获取映射表及part
        RadReadInformationMapper* radReadInfMapper = _reader->getReadRadInformationMapper();
        FITKRadiossPart* part = radReadInfMapper->getRadiossPartByFilePartID(pid);

        if (part == nullptr) return false;
        
        bool ok = false;
        _reader->backLine(); 
        while (_reader && !_reader->atEnd())
        {
            QString line = _reader->readLine();
           
            if(line.toUpper().startsWith("/PART"))
            {
                // 读取Part信息
                ok = this->readPart(part);
                break;
            }
            else if (line.startsWith("/"))
            {
                ok = this->readElement(part);
                //更新拓扑关系
                if(!ok || _caseData == nullptr) break;
                FITKRadiossMeshModel * meshModel = _caseData->getMeshModel();
                if(meshModel == nullptr) break;
                FITKRadiossMeshTopo * meshTopo = meshModel->getMeshTopo();
                if(meshTopo == nullptr) break;
                meshTopo->addPartTopo(part);
            
                break;
            }
            else 
                break;
        }
        if (!ok)
        {
            _reader->consoleMessage(3, QString("Read Part Error: %1 %2").arg(keyLine).arg(part->getDataObjectName()));
            return false;
        }
        _reader->consoleMessage(1, QString( "Read Part : %1 %2").arg(keyLine).arg(part->getDataObjectName()));
        return true;
    }

    bool FITKRadiossAdaptorPart::adaptW()
    {
        FITKRadiossPart* part = dynamic_cast<FITKRadiossPart*>(_dataObj);
        if (part == nullptr || _writer == nullptr) return false;

        QTextStream* stream = _writer->getStream();
        RadWriteInformationMapper* infoMapper = _writer->getWriteInpInformationMapper();
        if (stream == nullptr || infoMapper == nullptr) return false;
        //写出关键字行  ID 从1开始递增
        int radID = infoMapper->getMaxRadPartID() + 1;
        //建立映射关系
        infoMapper->insertPartIDMap(part->getDataObjectID(), radID);
        *stream << part->GetFITKRadiossPartRadiossKeyWord() << "/" << radID << endl;
        //写出part名称
        *stream << part->getDataObjectName() << endl;

        int propId = infoMapper->getRadIDByPropertyID(part->getPropertyID());
        int matId = infoMapper->getRadIDByMaterialID(part->getMaterialID());
        int subsetId = infoMapper->getRadIDBySubsetID(part->getSubsetID());
        double thick = part->getThickness();
        QString thickStr = QString(thick > 0 ? gapStr(thick) : gapStr(-1, 20));
        //属性ID,材料ID,子集ID，厚度
        *stream << gapStr(propId) << gapStr(matId)
            << gapStr(subsetId) << thickStr << endl;

        //写出该part的网格单元
        this->writeElement(part);

        return true;
    }

    bool FITKRadiossAdaptorPart::writeElement(FITKRadiossPart * part)
    {
        if (part == nullptr || _writer == nullptr) return false;
        FITKRadiossAdaptorPartElements adaptor(_reader, _writer, part);
        return adaptor.writeElements();
    }

    bool FITKRadiossAdaptorPart::readElement(FITKRadiossPart * part)
    {
        if (part == nullptr || _reader == nullptr) return false;
        FITKRadiossAdaptorPartElements adaptor(_reader, _writer, part);
        return adaptor.readElements();
    }

    bool FITKRadiossAdaptorPart::readPart(FITKRadiossPart * part)
    {
        if (part == nullptr || _reader == nullptr) return false;

        QString line = _reader->readLine();
        // 名称
        part->setDataObjectName(line);
        //读取Part的属性ID、材料ID、子集ID、厚度
        line = _reader->readLine();
        QStringList lines = line.simplified().split(" ");
        //说明信息与手册不符
        if (lines.size() < 2 || lines.size() > 4)
        {
            _reader->consoleMessage(3, QString("Invalid Part : %1.").arg(part->getDataObjectName()));
            return false;
        }      
        //part中暂存文件中的ID，后续在mapper中进行转换
        part->setPropertyID(lines[0].toInt());
        part->setMaterialID(lines[1].toInt());
        if (lines.size() == 3) {
            if(lines[2].contains("."))
                part->setThickness(lines[2].toInt());
            else
                part->setSubsetID(lines[2].toInt());
        }
        if (lines.size() == 4) part->setThickness(lines[3].toInt());

        return true;
    }
}


