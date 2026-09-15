/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKTimeHistoryRWALL.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include <QSet>

namespace Radioss
{
    FITKTimeHistoryRWALL::~FITKTimeHistoryRWALL()
    {
    }

    FITKTimeHistoryRWALL::TimeHistoryType FITKTimeHistoryRWALL::getTimeHistoryType()
    {
        return TimeHistoryType::TimeHistory_RWALL;
    }

    void FITKTimeHistoryRWALL::setVarlableGroupType(THRWALLVariableGroup type)
    {
        m_varlableGroupType = type;
    }

    FITKTimeHistoryRWALL::THRWALLVariableGroup FITKTimeHistoryRWALL::getVarlableGroupType()
    {
        return THRWALLVariableGroup(m_varlableGroupType);
    }

    void FITKTimeHistoryRWALL::setCustomizeVariable(QList<TimeHistoryRWALLVariable> variables)
    {
        for (TimeHistoryRWALLVariable var : variables)
            m_customizeVarlable.append(var);
    }

    QList<FITKTimeHistoryRWALL::TimeHistoryRWALLVariable> FITKTimeHistoryRWALL::getCustomizeVariable()
    {
        QList<TimeHistoryRWALLVariable> typeList;
        for (int var : m_customizeVarlable)
            typeList.append(TimeHistoryRWALLVariable(var));
        return typeList;
    }

    QStringList FITKTimeHistoryRWALL::getVariableKeyWords(bool isRemoveInclude)
    {
        QSet<QString> set{};
        for (int var : m_customizeVarlable) {
            Core::FITKEnumTransfer<TimeHistoryRWALLVariable> et;
            bool ok = false;
            QString str = et.toString((TimeHistoryRWALLVariable)var, ok);
            if (ok) set += str;
        }
        QStringList keyWords = set.toList();
        if (!isRemoveInclude)
            return keyWords;
        THRWALLVariableGroup groupType = (THRWALLVariableGroup)m_varlableGroupType;
        if (groupType == VGRWALL_DEF) {
            keyWords.removeAll("FNX");
            keyWords.removeAll("FNY");
            keyWords.removeAll("FNZ");
            keyWords.removeAll("FTX");
            keyWords.removeAll("FTY");
            keyWords.removeAll("FTZ");
            keyWords.removeAll("FN");
            keyWords.removeAll("FT");
            keyWords.append("DEF");
        }
        else if (groupType == VGRWALL_FN) {
            keyWords.removeAll("FNX");
            keyWords.removeAll("FNY");
            keyWords.removeAll("FNZ");
            keyWords.append("FN");
        }
        else if (groupType == VGRWALL_FT) {
            keyWords.removeAll("FTX");
            keyWords.removeAll("FTY");
            keyWords.removeAll("FTZ");
            keyWords.append("FT");
        }
        return keyWords;
    }

} // namespace Radioss

