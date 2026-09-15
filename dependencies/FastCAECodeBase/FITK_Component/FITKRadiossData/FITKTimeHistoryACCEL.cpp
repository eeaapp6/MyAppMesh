/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKTimeHistoryACCEL.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include <QSet>

namespace Radioss
{
    FITKTimeHistoryACCEL::~FITKTimeHistoryACCEL()
    {
    }

    FITKTimeHistoryACCEL::TimeHistoryType FITKTimeHistoryACCEL::getTimeHistoryType()
    {
        return TimeHistoryType::TimeHistory_ACCEL;
    }

    void FITKTimeHistoryACCEL::setVarlableGroupType(THACCELVariableGroup type)
    {
        m_varlableGroupType = type;
    }

    FITKTimeHistoryACCEL::THACCELVariableGroup FITKTimeHistoryACCEL::getVarlableGroupType()
    {
        return THACCELVariableGroup(m_varlableGroupType);
    }


    void FITKTimeHistoryACCEL::setCustomizeVariable(QList<TimeHistoryACCELVariable> variables)
    {
        for (TimeHistoryACCELVariable var : variables)
            m_customizeVarlable.append(var);
    }

    QList<FITKTimeHistoryACCEL::TimeHistoryACCELVariable> FITKTimeHistoryACCEL::getCustomizeVariable()
    {
        QList<TimeHistoryACCELVariable> typeList;
        for (int var : m_customizeVarlable)
            typeList.append(TimeHistoryACCELVariable(var));
        return typeList;
    }

    QStringList FITKTimeHistoryACCEL::getVariableKeyWords(bool isRemoveInclude)
    {
        QSet<QString> set{};
        for (int var : m_customizeVarlable) {
            Core::FITKEnumTransfer<TimeHistoryACCELVariable> et;
            bool ok = false;
            QString str = et.toString((TimeHistoryACCELVariable)var, ok);
            if (ok) set += str;
        }
        QStringList keyWords = set.toList();
        if(!isRemoveInclude)
            return keyWords;
        THACCELVariableGroup groupType = (THACCELVariableGroup)m_varlableGroupType;
        if(groupType == VGACCEL_DEF){
            keyWords.removeAll("AX");
            keyWords.removeAll("AY");
            keyWords.removeAll("AZ");
            keyWords.append("DEF");
        }
        else if (groupType == VGACCEL_W) {
            keyWords.removeAll("WX");
            keyWords.removeAll("WY");
            keyWords.removeAll("WZ");
            keyWords.append("W");
        }
        return keyWords;
    }

} // namespace Radioss

 