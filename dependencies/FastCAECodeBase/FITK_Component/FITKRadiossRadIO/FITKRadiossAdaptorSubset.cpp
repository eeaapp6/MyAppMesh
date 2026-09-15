/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossAdaptorSubset.h"

#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossMeshModel.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossNodesParts.h"

#include "FITKRadiossRadReader.h"
#include "FITKRadiossRadWriter.h"
#include "RadKeyLineSpliter.h"
#include "RadReadInformationMapper.h"
#include "RadWriteInformationMapper.h"

namespace Radioss
{
    QString FITKRadiossAdaptorSubset::getAdaptorClass()
    {
        return "Radioss::FITKRadiossAdaptorSubset";
    }

    bool FITKRadiossAdaptorSubset::adaptR()
    {
        if (_caseData == nullptr || _reader == nullptr) return false;
        
        // 创建边界条件数据对象
        FITKRadiossSubSet* subset = new FITKRadiossSubSet();
        //读取关键字
        RadKeyLineSpliter *keyLineSpliter = _reader->getKeyLineSpliter();
        RadReadInformationMapper* mapper = _reader->getReadRadInformationMapper();
        if (!keyLineSpliter||!mapper) return false;

        //获取id和关键字
        int id = keyLineSpliter->getId();
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
            //读取边界条件数据
            ok = this->readSubset(subset);
        }
        if (!ok)
        {
            _reader->consoleMessage(3, QString("Read Subset Error : %1 %2").arg(keyLine).arg(subset->getDataObjectName()));
            return false;
        }
        _reader->consoleMessage(1, QString("Read Subset : %1 %2").arg(keyLine).arg(subset->getDataObjectName()));
        _caseData->getMeshModel()->getSubSetManager()->appendDataObj(subset);
        //添加映射
        mapper->insertSubsetIDMap(id, subset->getDataObjectID());

        return true;
    }

    bool FITKRadiossAdaptorSubset::adaptW()
    {
        FITKRadiossSubSet* subset = dynamic_cast<FITKRadiossSubSet*>(_dataObj);
        if (subset == nullptr || _writer == nullptr) return false;

        QTextStream* stream = _writer->getStream();
        RadWriteInformationMapper* infoMapper = _writer->getWriteInpInformationMapper();
        if (stream == nullptr || infoMapper == nullptr) return false;
        //写出关键字行  ID 从1开始递增
        int radID = infoMapper->getMaxRadSubsetID() + 1;
        //建立映射关系
        infoMapper->insertSubsetIDMap(subset->getDataObjectID(), radID);
        *stream << subset->GetFITKRadiossSubSetRadiossKeyWord() << "/" << radID << endl;
        //写出名称
        *stream << subset->getDataObjectName() << endl;
        if(!subset->isCombination())
            return true;

        //子集的子集ID  
        QList<int> idList{};
        for (int i = 0; i < subset->getDataCount();++i) 
        {
            int radID = infoMapper->getRadIDBySubsetID(subset->getIDByIndex(i));
            idList.append(radID);
        }
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

    bool FITKRadiossAdaptorSubset::readSubset(FITKRadiossSubSet * subset)
    {
        if (subset == nullptr || _reader == nullptr) return false;

        QString line = _reader->previousLine().trimmed();
        //名称
        subset->setDataObjectName(line);
        //读取子集信息
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
        //为空则说明不是嵌套子集
        if (lines.empty()) return true;
        QList<int> idList;
        for (auto line :lines) 
        {
            QStringList idStrList = line.simplified().split(" ");
            for (const QString& idStr : idStrList)
            {
                bool ok = false;
                int radID = idStr.toInt(&ok);
                if (!ok || radID < 1)continue;
                idList.append(radID);
            }
        }
        subset->m_subsetRadIDs = idList;

        return true;
    }
}


