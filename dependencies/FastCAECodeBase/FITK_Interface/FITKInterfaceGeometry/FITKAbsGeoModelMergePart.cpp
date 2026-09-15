/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbsGeoModelMergePart.h"

namespace Interface
{
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelMergePart::getGeometryCommandType()
    {
        return FITKGeoEnum::FGTMergePart;
    }

    void FITKAbsGeoModelMergePart::setMergeOperType(GeoBoolOperType type)
    {
        // 设置布尔类型。
        m_boolOperType = type;
    }

    FITKAbsGeoModelMergePart::GeoBoolOperType FITKAbsGeoModelMergePart::getMergeOperType() const
    {
        return m_boolOperType;
    }

    void FITKAbsGeoModelMergePart::setCommandIds(const QList<int>& commandIds)
    {
        // 保存合并部件ID列表。
        m_cmdIds = commandIds;
    }

    QList<int> FITKAbsGeoModelMergePart::getCommandIds() const
    {
        return m_cmdIds;
    }

    void FITKAbsGeoModelMergePart::addCommandId(int commandId)
    {
        if (!m_cmdIds.contains(commandId))
        {
            m_cmdIds.append(commandId);
        }
    }

    void FITKAbsGeoModelMergePart::clearCommandIds()
    {
        m_cmdIds.clear();
    }
}
