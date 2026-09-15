/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKTimeHistoryRBODY.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include <QSet>

namespace Radioss
{
    FITKTimeHistoryRBODY::~FITKTimeHistoryRBODY()
    {
    }

    FITKTimeHistoryRBODY::TimeHistoryType FITKTimeHistoryRBODY::getTimeHistoryType()
    {
        return TimeHistoryType::TimeHistory_RBODY;
    }

    void FITKTimeHistoryRBODY::setVarlableGroupType(THRBODYVariableGroup type)
    {
        m_varlableGroupType = type;
    }

    FITKTimeHistoryRBODY::THRBODYVariableGroup FITKTimeHistoryRBODY::getVarlableGroupType()
    {
        return THRBODYVariableGroup(m_varlableGroupType);
    }

    void FITKTimeHistoryRBODY::setCustomizeVariable(QList<TimeHistoryRBODYVariable> variables)
    {
        for (TimeHistoryRBODYVariable var : variables)
            m_customizeVarlable.append(var);
    }

    QList<FITKTimeHistoryRBODY::TimeHistoryRBODYVariable> FITKTimeHistoryRBODY::getCustomizeVariable()
    {
        QList<TimeHistoryRBODYVariable> typeList;
        for (int var : m_customizeVarlable)
            typeList.append(TimeHistoryRBODYVariable(var));
        return typeList;
    }

    QStringList FITKTimeHistoryRBODY::getVariableKeyWords(bool isRemoveInclude)
    {
        QSet<QString> set{};
        for (int var : m_customizeVarlable) {
            Core::FITKEnumTransfer<TimeHistoryRBODYVariable> et;
            bool ok = false;
            QString str = et.toString((TimeHistoryRBODYVariable)var, ok);
            if (ok) set += str;
        }
        QStringList keyWords = set.toList();
        if (!isRemoveInclude)
            return keyWords;
        THRBODYVariableGroup groupType = (THRBODYVariableGroup)m_varlableGroupType;
        if (groupType == VGRBODY_DEF) {
            keyWords.removeAll("FX");
            keyWords.removeAll("FY");
            keyWords.removeAll("FZ");
            keyWords.removeAll("MX");
            keyWords.removeAll("MY");
            keyWords.removeAll("MZ");
            keyWords.removeAll("RX");
            keyWords.removeAll("RY");
            keyWords.removeAll("RZ");
            keyWords.removeAll("F");
            keyWords.removeAll("M");
            keyWords.removeAll("R");
            keyWords.append("DEF");
        }
        else if (groupType == VGRBODY_F) {
            keyWords.removeAll("FX");
            keyWords.removeAll("FY");
            keyWords.removeAll("FZ");
            keyWords.append("F");
        }
        else if (groupType == VGRBODY_M) {
            keyWords.removeAll("MX");
            keyWords.removeAll("MY");
            keyWords.removeAll("MZ");
            keyWords.append("M");
        }
        else if (groupType == VGRBODY_R) {
            keyWords.removeAll("RX");
            keyWords.removeAll("RY");
            keyWords.removeAll("RZ");
            keyWords.append("R");
        }
        else if (groupType == VGRBODY_FI) {
            keyWords.removeAll("FXI");
            keyWords.removeAll("FYI");
            keyWords.removeAll("FZI");
            keyWords.append("FI");
        }
        else if (groupType == VGRBODY_MI) {
            keyWords.removeAll("MXI");
            keyWords.removeAll("MYI");
            keyWords.removeAll("MZI");
            keyWords.append("MI");
        }
        return keyWords;
    }

} // namespace Radioss

