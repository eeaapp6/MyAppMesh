/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossAdaptorNodeGroup.h"
#include "FITKRadiossRadReader.h"
#include "FITKRadiossRadWriter.h"
#include "RadKeyLineSpliter.h"
#include "RadReadInformationMapper.h"
#include "RadWriteInformationMapper.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossMeshModel.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossNodeGroup.h"
#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"

#include <QSet>

namespace Radioss
{
    QString FITKRadiossAdaptorNodeGroup::getAdaptorClass()
    {
        return "Radioss::FITKRadiossAdaptorNodeGroup";
    }

    bool FITKRadiossAdaptorNodeGroup::adaptR()
    {
        //错误判断，获取必要的工具类
        if(_caseData == nullptr || _reader == nullptr) return false;
        RadKeyLineSpliter* spliter = _reader->getKeyLineSpliter(); 
        if (!spliter)return false;
        RadReadInformationMapper* mapper = _reader->getReadRadInformationMapper();
        if (!mapper)return false;
        Radioss::FITKRadiossMeshModel* radiossMeshModel = _caseData->getMeshModel();
        if (!radiossMeshModel)return false;
        Interface::FITKComponentManager* componentManager = radiossMeshModel->getComponentManager();
        if (!componentManager)return false;
        //获取part管理器
        FITKRadiossPartManager * partManager = _caseData->getMeshModel()->getPartsManager();
        if (partManager == nullptr) return nullptr;


        //读取Group ID
        const int nodeGroupID = spliter->getId(0);
        if(nodeGroupID <= 0) return false;
        //判断节点组类型
        int type = -1;
        QString strType = spliter->getKeys();
        if (strType =="/GRNOD/NODE")
            type = 1;
        else if(strType =="/GRNOD/PART")
            type = 2;
        else if (strType =="/GRNOD/SURF")
            type = 3;
        else if (strType == "/GRNOD/GRNOD")
            type = 4;
        if(type<0)return false;

        const QString setName = _reader->getCurrentLine();
        if(setName.isEmpty()) return false;

        //读取节点列表
        QList<int> idList;
        while (!_reader->atEnd())
        {
            QString line = _reader->readLine();
            //读取结束标记
            if(line.startsWith("/"))
            {
                _reader->backLine();
                break;
            }
            switch (type)
            {
            case 1: {
                QStringList nodeStrList = line.split(" ", Qt::SkipEmptyParts);
                for (const QString& nodeStr : nodeStrList)
                {
                    bool ok = false;
                    int nodeRadID = nodeStr.toInt(&ok);
                    if (!ok || nodeRadID < 1)continue;
                    idList.append(nodeRadID);
                }
                break;
            }
            case 2: 
            case 3:
            case 4:{
                QStringList strIDList = line.split(" ", Qt::SkipEmptyParts);
                do {
                    for (auto partIDStr : strIDList) {
                        bool ok = false;
                        int partRadID = partIDStr.toInt(&ok);
                        if (!ok)continue;
                        idList.append(partRadID);
                    }
                    line = _reader->readLine();
                    strIDList = line.split(" ", Qt::SkipEmptyParts);
                    if (strIDList.isEmpty())
                        break;
                } while (strIDList[0].toInt() > 0);
                _reader->backLine();
                break;
            }
            default:
                break;
            }    
        }    
        if(idList.isEmpty()) return false;
        //创建节点集合
        FITKRadiossNodeGroup* nodeGroup = new FITKRadiossNodeGroup;
        FITKRadiossNodeGroupRadIOInfo* info = nodeGroup->getRadIOInfo();
        if (!info) {
            delete nodeGroup;
            return false;
        }
        nodeGroup->setDataObjectName(setName);
        info->setRadiossKeyWord(strType);
        info->setRadiossIDs(idList);

        //设置节点集合到RadiossMeshModel
        componentManager->appendDataObj(nodeGroup);
        mapper->insertNodeGroupIDMap(nodeGroupID, nodeGroup->getDataObjectID());

        return true;
    }

    bool FITKRadiossAdaptorNodeGroup::adaptW()
    {
        Interface::FITKModelSet* modelSet = dynamic_cast<Interface::FITKModelSet*>(_dataObj);
        if (modelSet == nullptr || _writer == nullptr) return false;

        QTextStream* stream = _writer->getStream();
        RadWriteInformationMapper* infoMapper = _writer->getWriteInpInformationMapper();
        if (stream == nullptr || infoMapper == nullptr) return false;
        //写出关键字行  ID 从1开始递增
        int radID = infoMapper->getMaxRadNodeSetID() + 1;
        //建立映射关系
        infoMapper->insertNodeSetIDMap(modelSet->getDataObjectID(), radID);
        *stream << "/GRNOD/NODE" << "/" << radID << endl;

        //写出名称
        *stream << modelSet->getDataObjectName() << endl;

        QList<int> idList = modelSet->getAbsoluteMember();
        //列数，当列数到10换行
        int column = 1;
        for (auto id : idList) {
            *stream << gapStr(id);
            if (column == 10) {
                *stream << endl;
                column = 0;
            }
            ++column;
        }
        if(column!=1)
            *stream << endl;
        return true;

    }
}


