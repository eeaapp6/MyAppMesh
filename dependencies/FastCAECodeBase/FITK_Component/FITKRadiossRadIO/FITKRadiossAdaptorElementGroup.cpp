/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossAdaptorElementGroup.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossMeshModel.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossElementGroup.h"
#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"

#include "RadReadInformationMapper.h"
#include "RadWriteInformationMapper.h"
#include "RadKeyLineSpliter.h"
#include "FITKRadiossRadReader.h"
#include "FITKRadiossRadWriter.h"

#include <QSet>

namespace Radioss
{
    QString FITKRadiossAdaptorElementGroup::getAdaptorClass()
    {
        return "Radioss::FITKRadiossAdaptorElementGroup";
    }

    bool FITKRadiossAdaptorElementGroup::adaptR()
    {
        if (_caseData == nullptr || _reader == nullptr) return false;
        RadKeyLineSpliter* spliter = _reader->getKeyLineSpliter();
        RadReadInformationMapper* mapper = _reader->getReadRadInformationMapper();
        if (!mapper||!spliter)return false;
        Radioss::FITKRadiossMeshModel* radiossMeshModel = _caseData->getMeshModel();
        if (!radiossMeshModel)return false;
        Interface::FITKComponentManager* componentManager = radiossMeshModel->getComponentManager();
        if (!componentManager)return false;

        //读取Group ID和名称
        const int nodeGroupID = spliter->getId(0);
        if (nodeGroupID <= 0) return false;
        const QString setName = _reader->readLine();
        if (setName.isEmpty()) return false;

        QList<int> idList;
        //判断元素组类型
        QString strType = spliter->getKeys();
        if (strType.contains("/BOX")) 
        {
            /*QString line = _reader->readLine();
            bool ok = false;
            int boxID = line.trimmed().toInt(&ok);
            if (ok)idList.append(boxID);*/
        }
        else if(strType.contains("/GEN_INCR"))
        {
            //to do 
        }
        else 
        {
            if (!readListParam(idList))return false;
        }
        if(idList.isEmpty())return false;

        FITKRadiossElementGroup* eleGroup = new FITKRadiossElementGroup;
        FITKRadiossElementGroupRadIOInfo* info = eleGroup->getRadIOInfo();
        if (!info)return false;

        info->setRadiossKeyWord(strType);
        info->setRadiossIDs(idList);
        eleGroup->setDataObjectName(setName);
        //设置节点集合到RadiossMeshModel
        componentManager->appendDataObj(eleGroup);
        mapper->insertElementGroupIDMap(nodeGroupID, eleGroup->getDataObjectID());
        return true;
    }

    bool FITKRadiossAdaptorElementGroup::adaptW()
    {
        FITKRadiossElementGroup*  modelSet = dynamic_cast<FITKRadiossElementGroup*>(_dataObj);
        if (modelSet == nullptr || _writer == nullptr) return false;

        QTextStream* stream = _writer->getStream();
        RadWriteInformationMapper* infoMapper = _writer->getWriteInpInformationMapper();
        if (stream == nullptr || infoMapper == nullptr) return false;
        //写出关键字行  ID 从1开始递增
        int radID = infoMapper->getMaxRadEleSetID() + 1;
        //建立映射关系
        infoMapper->insertEleSetIDMap(modelSet->getDataObjectID(), radID);
        QString keyWords = modelSet->getElementGroupKeyWords();
        *stream << keyWords+"/" << keyWords.right(4)+"/" << radID << endl;

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
        if (column != 1)
            *stream << endl;
        return true;
    }

    bool FITKRadiossAdaptorElementGroup::readListParam(QList<int>& idList)
    {
        idList.clear();
        QString line = _reader->readLine();
        QStringList strIDList = line.split(" ", Qt::SkipEmptyParts);
        if (strIDList.isEmpty()) {
            _reader->backLine();
            return false;
        }
        do {
            for (auto idStr : strIDList) {
                bool ok = false;
                int radID = idStr.toInt(&ok);
                if (!ok)continue;
                idList.append(radID);
            }
            line = _reader->readLine();
            strIDList = line.split(" ", Qt::SkipEmptyParts);
            if (strIDList.isEmpty())
                break;
        } while (strIDList[0].toInt() > 0);

        _reader->backLine();
        if (idList.isEmpty())return false;

        return true;
    }
}

