/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKTimeHistorySPHCEL.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include <QSet>

namespace Radioss
{
    FITKTimeHistorySPHCEL::~FITKTimeHistorySPHCEL()
    {
    }

    FITKTimeHistorySPHCEL::TimeHistoryType FITKTimeHistorySPHCEL::getTimeHistoryType()
    {
        return TimeHistoryType::TimeHistory_SPHCEL;
    }

    void FITKTimeHistorySPHCEL::setVarlableGroupType(THSPHCELVariableGroup type)
    {
        m_varlableGroupType = type;
    }

    FITKTimeHistorySPHCEL::THSPHCELVariableGroup FITKTimeHistorySPHCEL::getVarlableGroupType()
    {
        return THSPHCELVariableGroup(m_varlableGroupType);
    }

    void FITKTimeHistorySPHCEL::setCustomizeVariable(QList<TimeHistorySPHCELVariable> variables)
    {
        for (TimeHistorySPHCELVariable var : variables)
            m_customizeVarlable.append(var);
    }

    QList<FITKTimeHistorySPHCEL::TimeHistorySPHCELVariable> FITKTimeHistorySPHCEL::getCustomizeVariable()
    {
        QList<TimeHistorySPHCELVariable> typeList;
        for (int var : m_customizeVarlable)
            typeList.append(TimeHistorySPHCELVariable(var));
        return typeList;
    }

    void FITKTimeHistorySPHCEL::setSPHNodeID(int id)
    {
        //设置SPH粒子节点ID
        m_nodeID = id;
    }

    int FITKTimeHistorySPHCEL::getSPHNodeID()
    {
        //获取SPH粒子节点ID
        return m_nodeID;
    }

    void FITKTimeHistorySPHCEL::setSPHNodeName(QString name)
    {
        //设置SPH粒子名称
        m_nodeName = name;
    }

    QString FITKTimeHistorySPHCEL::getSPHNodeName()
    {
        //获取SPH粒子名称
        return m_nodeName;
    }

    QStringList FITKTimeHistorySPHCEL::getVariableKeyWords(bool isRemoveInclude)
    {
        QSet<QString> set{};
        for (int var : m_customizeVarlable) {
            Core::FITKEnumTransfer<TimeHistorySPHCELVariable> et;
            bool ok = false;
            QString str = et.toString((TimeHistorySPHCELVariable)var, ok);
            if (ok) set += str;
        }
        QStringList keyWords = set.toList();
        if (!isRemoveInclude)
            return keyWords;
        THSPHCELVariableGroup groupType = (THSPHCELVariableGroup)m_varlableGroupType;
        if (groupType == VGSPHCEL_ALL) {
            keyWords.clear();
            keyWords.append("ALL");
        }
        else if (groupType == VGSPHCEL_DEF) {
            keyWords.removeAll("OFF");
            keyWords.removeAll("SX");
            keyWords.removeAll("SY");
            keyWords.removeAll("SZ");
            keyWords.removeAll("SXY");
            keyWords.removeAll("SYZ");
            keyWords.removeAll("SXZ");
            keyWords.removeAll("IE");
            keyWords.removeAll("DENS");
            keyWords.removeAll("PLAS");
            keyWords.removeAll("TEMP");
            keyWords.append("DEF");
        }
        return keyWords;
    }

} // namespace Radioss

