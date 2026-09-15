/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbstractTimeHistory.h"

namespace Radioss
{
    FITKAbstractTimeHistory::~FITKAbstractTimeHistory()
    {
    }

    FITKAbstractTimeHistory::TimeHistoryType FITKAbstractTimeHistory::getTimeHistoryType()
    {
        return FITKAbstractTimeHistory::TimeHistoryType::TimeHistory_Unknown;
    }

    void FITKAbstractTimeHistory::setObjIDs(QList<int> objIDs)
    {
        m_objID = objIDs;
    }

    QList<int> FITKAbstractTimeHistory::getObjIDs()
    {
        return m_objID;
    }

    QList<FITKAbstractTimeHistory*> FITKTimeHistoryManager::getTimeHistoryByType(FITKAbstractTimeHistory::TimeHistoryType type)
    {
        QList<FITKAbstractTimeHistory*> dataList;
        //通过类型获取时间历程
        int count = this->getDataCount();
        for (int i = 0; i < count; ++i)
        {
            FITKAbstractTimeHistory* data = this->getDataByIndex(i);
            if (!data) continue;
            if (data->getTimeHistoryType() != type) continue;
            dataList.append(data);
        }
        return dataList;
    }

} // namespace Radioss

 