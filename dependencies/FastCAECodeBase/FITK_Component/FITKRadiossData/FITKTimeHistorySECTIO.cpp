/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKTimeHistorySECTIO.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include <QSet>

namespace Radioss
{
    FITKTimeHistorySECTIO::~FITKTimeHistorySECTIO()
    {
    }

    FITKTimeHistorySECTIO::TimeHistoryType FITKTimeHistorySECTIO::getTimeHistoryType()
    {
        return TimeHistoryType::TimeHistory_SECTIO;
    }

    void FITKTimeHistorySECTIO::setVarlableGroupType(THSECTIOVariableGroup type)
    {
        m_varlableGroupType = type;
    }

    FITKTimeHistorySECTIO::THSECTIOVariableGroup FITKTimeHistorySECTIO::getVarlableGroupType()
    {
        return THSECTIOVariableGroup(m_varlableGroupType);
    }

    void FITKTimeHistorySECTIO::setCustomizeVariable(QList<TimeHistorySECTIOVariable> variables)
    {
        for (TimeHistorySECTIOVariable var : variables)
            m_customizeVarlable.append(var);
    }

    QList<FITKTimeHistorySECTIO::TimeHistorySECTIOVariable> FITKTimeHistorySECTIO::getCustomizeVariable()
    {
        QList<TimeHistorySECTIOVariable> typeList;
        for (int var : m_customizeVarlable)
            typeList.append(TimeHistorySECTIOVariable(var));
        return typeList;
    }

    QStringList FITKTimeHistorySECTIO::getVariableKeyWords(bool isRemoveInclude)
    {
        QSet<QString> set{};
        for (int var : m_customizeVarlable) {
            Core::FITKEnumTransfer<TimeHistorySECTIOVariable> et;
            bool ok = false;
            QString str = et.toString((TimeHistorySECTIOVariable)var, ok);
            if (ok) set += str;
        }
        QStringList keyWords = set.toList();
        if (!isRemoveInclude)
            return keyWords;
        auto remove1 = [&]() {
            keyWords.removeAll("FNX");
            keyWords.removeAll("FNY");
            keyWords.removeAll("FNZ");
        };
        auto remove2 = [&]() {
            keyWords.removeAll("FTX");
            keyWords.removeAll("FTY");
            keyWords.removeAll("FTZ");
        };
        auto remove3 = [&]() {
            keyWords.removeAll("M1");
            keyWords.removeAll("M2");
            keyWords.removeAll("M3");
        };
        THSECTIOVariableGroup groupType = (THSECTIOVariableGroup)m_varlableGroupType;
        if (groupType == VGSECTIO_DEF) {
            remove1();
            remove2();
            remove3();
            keyWords.removeAll("FN");
            keyWords.removeAll("FT");
            keyWords.removeAll("M");
            keyWords.append("DEF");
        }
        else if (groupType == VGSECTIO_FN) {
            remove1();
            keyWords.append("FN");
        }
        else if (groupType == VGSECTIO_FT) {
            remove2();
            keyWords.append("FT");
        }
        else if (groupType == VGSECTIO_M) {
            remove3();
            keyWords.append("M");
        }
        else if (groupType == VGSECTIO_CENTER) {
            keyWords.removeAll("CX");
            keyWords.removeAll("CY");
            keyWords.removeAll("CZ");
            keyWords.append("CENTER");
        }
        else if (groupType == VGSECTIO_GLOBAL) {
            remove1();
            remove2();
            keyWords.removeAll("FN");
            keyWords.removeAll("FT");
            keyWords.removeAll("MX");
            keyWords.removeAll("MY");
            keyWords.removeAll("MZ");
            keyWords.append("GLOBAL");
        }
        else if (groupType == VGSECTIO_LOCAL) {
            remove3();
            keyWords.removeAll("M");
            keyWords.removeAll("F1");
            keyWords.removeAll("F2");
            keyWords.removeAll("F3");
            keyWords.append("LOCAL");
        }
        return keyWords;
    }

} // namespace Radioss

