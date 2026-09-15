/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossAdaptorProbeAccel.h"
#include "FITK_Component/FITKRadiossData/FITKProbeAcceleration.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSolution.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossMeshModel.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossNodeGroup.h"
#include "FITK_Interface/FITKInterfaceModel/FITKModelSet.h"
#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"

#include "RadReadInformationMapper.h"
#include "RadWriteInformationMapper.h"
#include "FITKRadiossRadReader.h"
#include "FITKRadiossRadWriter.h"
#include "RadKeyLineSpliter.h"

namespace Radioss
{
    QString FITKRadiossAdaptorProbeAccel::getAdaptorClass()
    {
        return "Radioss::FITKRadiossAdaptorProbeAccel";
    }
    bool FITKRadiossAdaptorProbeAccel::adaptR()
    {
        if (_caseData == nullptr || _reader == nullptr) return false;

        // 读取关键字
        RadKeyLineSpliter *keyLineSpliter = _reader->getKeyLineSpliter();
        RadReadInformationMapper *infoMapper = _reader->getReadRadInformationMapper();
        if (!keyLineSpliter || !infoMapper) return false;

        int pid = keyLineSpliter->getId();
        QString keyLine = keyLineSpliter->getCurrentLine();

        FITKProbeAcceleration* accel = new FITKProbeAcceleration();
        if (accel == nullptr) return false;

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
            //读取数据
            ok = this->readAccel(accel);
        }
        if (!ok)
        {
            _reader->consoleMessage(3, QString("Read Accelero Error : %1").arg(keyLine));
            return false;
        }
        _reader->consoleMessage(1, QString("Read Accelero : %1").arg(keyLine));
        _caseData->getCurrentSolution()->getProbeManager()->appendDataObj(accel);
        infoMapper->insertProbeAccelIDMap(pid, accel->getDataObjectID());
        return true;
    }
    bool FITKRadiossAdaptorProbeAccel::adaptW()
    {
        //写出数据
        FITKProbeAcceleration* accel = dynamic_cast<FITKProbeAcceleration*>(_dataObj);
        if (accel == nullptr || _writer == nullptr) return false;

        QTextStream* stream = _writer->getStream();
        RadWriteInformationMapper* infoMapper = _writer->getWriteInpInformationMapper();
        if (stream == nullptr || infoMapper == nullptr) return false;
        //写出关键字行  ID 从1开始递增
        int radID = infoMapper->getMaxRadAccelID() + 1;
        //建立映射关系
        infoMapper->insertAccelIDMap(accel->getDataObjectID(), radID);
        *stream << accel->GetFITKProbeAccelerationRadiossKeyWord() << "/" << radID << endl;
        //写出名称
        *stream << accel->getDataObjectName() << endl;

        *stream << QString("#%1%2%3").arg("node_ID", 9).arg("skew_ID", 10).arg("Fcut", 30) << endl;
        *stream << gapStr(accel->getNodeID()) << gapStr(accel->getSystemID()) 
            << gapStr(accel->getCutOffFreq(),30) << endl;
        return true;
    }
    bool FITKRadiossAdaptorProbeAccel::readAccel(FITKProbeAcceleration * accel)
    {
        if (accel == nullptr || _reader == nullptr) return false;

        QString line = _reader->previousLine().trimmed();
        //名称
        accel->setDataObjectName(line);
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
        if (lines.size()!=1)
        {
            _reader->consoleMessage(3, "Invalid Accelero : /ACCEL");
            return false;
        }
        //读取参数、赋值
        this->readParam(lines, accel);
        return true;
    }
    bool FITKRadiossAdaptorProbeAccel::readParam(const QStringList & lines, FITKProbeAcceleration * accel)
    {
        if (lines.size() !=1|| accel == nullptr) return false;
        RadReadInformationMapper* mapper = _reader->getReadRadInformationMapper();
        if (!mapper)return false;
        QString line = lines[0];
        QHash<int, double> ValPar;

        bool ok = false;
        int nodeID = line.mid(0, 10).trimmed().toInt(&ok);
        if (ok) ValPar.insert(0, nodeID);

        int skewID = line.mid(10, 10).trimmed().toInt(&ok);
        if (ok) ValPar.insert(1, skewID);

        double Fcut = line.mid(30, 20).trimmed().toDouble(&ok);
        if (ok) ValPar.insert(2, Fcut);

        int NodeID = mapper->getNodeIDByRadID(nodeID);
        if (NodeID > 0) {
            Radioss::FITKRadiossMeshModel* radiossMeshModel = _caseData->getMeshModel();
            if (!radiossMeshModel)return false;
            Interface::FITKComponentManager* componentManager = radiossMeshModel->getComponentManager();
            if (!componentManager)return false;
            Interface::FITKModelSet* modelSet = new FITKRadiossNodeGroup();
            modelSet->setDataObjectName(componentManager->checkName("Single_point_Set-1"));
            modelSet->setModelSetType(Interface::FITKModelEnum::FITKModelSetType::FMSNode);
            modelSet->appendMember(NodeID);
            modelSet->setModel(radiossMeshModel);
            componentManager->appendDataObj(modelSet);

            accel->setSetID(modelSet->getDataObjectID());
        }
        //获取对应参数值，如果不存在则为默认值
        accel->setSystemID(ValPar.value(1, accel->getSystemID()));
        accel->setCutOffFreq(ValPar.value(2, accel->getCutOffFreq()));
    }
}