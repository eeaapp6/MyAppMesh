/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossAdaptorSurfaceSeg.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossMeshModel.h"
#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSurfaceSeg.h"

#include "FITKRadiossRadReader.h"
#include "FITKRadiossRadWriter.h"
#include "RadKeyLineSpliter.h"
#include "RadReadInformationMapper.h"
#include "RadWriteInformationMapper.h"

namespace Radioss
{
    QString FITKRadiossAdaptorSurfaceSeg::getAdaptorClass()
    {
        return "Radioss::FITKRadiossAdaptorSurfaceSeg";
    }

    bool FITKRadiossAdaptorSurfaceSeg::adaptR()
    {
        //错误判断，获取必要的工具类
        if (_caseData == nullptr || _reader == nullptr) return false;
        RadKeyLineSpliter* spliter = _reader->getKeyLineSpliter();
        if (!spliter)return false;
        RadReadInformationMapper* mapper = _reader->getReadRadInformationMapper();
        if (!mapper)return false;
        Radioss::FITKRadiossMeshModel* radiossMeshModel = _caseData->getMeshModel();
        if (!radiossMeshModel)return false;
        Interface::FITKComponentManager* componentManager = radiossMeshModel->getComponentManager();
        if (!componentManager)return false;

        //读取surface ID
        const int surfaceID = spliter->getId(0);
        if (surfaceID <= 0) return false;

        const QString setName = _reader->readLine();
        if (setName.isEmpty()) return false;

        Radioss::FITKRadiossSurfaceSeg * surfaceSeg = new Radioss::FITKRadiossSurfaceSeg();
        surfaceSeg->setModel(radiossMeshModel);
        //读取节点列表
        while (!_reader->atEnd())
        {
            const QString line = _reader->readLine();
            //读取结束标记
            if (line.startsWith("/"))
            {
                _reader->backLine();
                break;
            }

            QStringList nodeStrList = line.split(" ", Qt::SkipEmptyParts);
            if (nodeStrList.size() < 4)
            {
                _reader->consoleMessage(2, QString("Invalid Surface element line : %1").arg(line));
                continue;
            }
            //单元节点id
            QList<int> nodeList;
            const int radId = nodeStrList[0].toInt(); 
            for (int i = 1; i < nodeStrList.size(); ++i) 
            {
                int radID_node = nodeStrList[i].toInt();
                nodeList.append(mapper->getNodeIDByRadID(radID_node));
            }
            surfaceSeg->addSurface( nodeList);
        }
        if (surfaceSeg->getAllElement().isEmpty()) {
            delete surfaceSeg;
            surfaceSeg = nullptr;
            return false;
        }
        surfaceSeg->setDataObjectName(setName);

        //设置单元面集到RadiossMeshModel
        componentManager->appendDataObj(surfaceSeg);
        mapper->insertSurfaceSetIDMap(surfaceID, surfaceSeg->getDataObjectID());

        return true;
    }

    bool FITKRadiossAdaptorSurfaceSeg::adaptW()
    {
        Radioss::FITKRadiossSurfaceSeg * surfaceSeg = dynamic_cast<Radioss::FITKRadiossSurfaceSeg*>(_dataObj);
        if (surfaceSeg == nullptr || _writer == nullptr) return false;

        QTextStream* stream = _writer->getStream();
        RadWriteInformationMapper* infoMapper = _writer->getWriteInpInformationMapper();
        if (stream == nullptr || infoMapper == nullptr) return false;
        //写出关键字行  ID 从1开始递增
        int radID = infoMapper->getMaxRadSurfaceID() + 1;
        //建立映射关系
        infoMapper->insertSurfaceIDMap(surfaceSeg->getDataObjectID(), radID);
        *stream << surfaceSeg->GetFITKRadiossSurfaceSegRadiossKeyWord() << "/" << radID << endl;
        //写出名称
        *stream << surfaceSeg->getDataObjectName() << endl;
        QList<int> eleidList = surfaceSeg->getAllElement();
        qSort(eleidList);
        RadWriteInformationMapper* wMapper =  _writer->getWriteInpInformationMapper();

        for (auto eleid : eleidList)
        {
            //单元中的可能会有多个面
            QList<QList<int>> surfaceList = surfaceSeg->getSurface(eleid,true) ;
            //写出每一个面
            for (QList<int> asurf : surfaceList)
            {
                if(asurf.isEmpty())
                    continue;
                *stream << gapStr(wMapper->getNextSegSurfaceID());
                for (int aNodeID : asurf)
                    *stream << gapStr(aNodeID);
                *stream << endl;
            }
//             *stream << gapStr(id);
//             for (auto nodeID : )
//             {
//                 *stream << gapStr(nodeID);
//             }
            
        }

        return true;

    }
}