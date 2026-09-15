/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKTimeHistoryINTER.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include <QSet>

namespace Radioss
{
    FITKTimeHistoryINTER::~FITKTimeHistoryINTER()
    {
    }

    FITKTimeHistoryINTER::TimeHistoryType FITKTimeHistoryINTER::getTimeHistoryType()
    {
        return TimeHistoryType::TimeHistory_INTER;
    }

    void FITKTimeHistoryINTER::setVarlableGroupType(THINTERVariableGroup type)
    {
        m_varlableGroupType = type;
    }

    FITKTimeHistoryINTER::THINTERVariableGroup FITKTimeHistoryINTER::getVarlableGroupType()
    {
        return THINTERVariableGroup(m_varlableGroupType);
    }

    void FITKTimeHistoryINTER::setCustomizeVariable(QList<TimeHistoryINTERVariable> variables)
    {
        for (TimeHistoryINTERVariable var : variables)
            m_customizeVarlable.append(var);
    }

    QList<FITKTimeHistoryINTER::TimeHistoryINTERVariable> FITKTimeHistoryINTER::getCustomizeVariable()
    {
        QList<TimeHistoryINTERVariable> typeList;
        for (int var : m_customizeVarlable)
            typeList.append(TimeHistoryINTERVariable(var));
        return typeList;
    }

    QStringList FITKTimeHistoryINTER::getVariableKeyWords(bool isRemoveInclude)
    {
        QSet<QString> set{};
        for (int var : m_customizeVarlable) {
            Core::FITKEnumTransfer<TimeHistoryINTERVariable> et;
            bool ok = false;
            QString str = et.toString((TimeHistoryINTERVariable)var, ok);
            if (!ok)continue;
            str.replace("_","|");
            set += str;
        }
        QStringList keyWords = set.toList();
        if (!isRemoveInclude)
            return keyWords;
        THINTERVariableGroup groupType = (THINTERVariableGroup)m_varlableGroupType;
        if (groupType == VGINTER_DEF) {
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
        else if (groupType == VGINTER_FN) {
            keyWords.removeAll("FNX");
            keyWords.removeAll("FNY");
            keyWords.removeAll("FNZ");
            keyWords.append("FN");
        }
        else if (groupType == VGINTER_FT) {
            keyWords.removeAll("FTX");
            keyWords.removeAll("FTY");
            keyWords.removeAll("FTZ");
            keyWords.append("FT");
        }
        else if (groupType == VGINTER_AbsoluteFN) {
            keyWords.removeAll("|FNX|");
            keyWords.removeAll("|FNY|");
            keyWords.removeAll("|FNZ|");
            keyWords.removeAll("||FN||");
            keyWords.append("|FN|");
        }
        else if (groupType == VGINTER_AbsoluteF) {
            keyWords.removeAll("|FX|");
            keyWords.removeAll("|FY|");
            keyWords.removeAll("|FZ|");
            keyWords.removeAll("||F||");
            keyWords.append("|F|");
        }
        return keyWords;
    }

} // namespace Radioss

