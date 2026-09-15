/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKAbsGeoModelShellFromSolid.cpp
 * @brief       实体抽壳形成壳面几何命令抽象类实现。
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2026-07-06
 *********************************************************************/

#include "FITKAbsGeoModelShellFromSolid.h"

namespace Interface
{
    // 命令类型。
    //@{
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelShellFromSolid::getGeometryCommandType()
    {
        // 实体抽壳形成壳面。
        return FITKGeoEnum::FITKGeometryComType::FGTShellFromSolid;
    }

    void FITKAbsGeoModelShellFromSolid::addSolid(VirtualShape solid)
    {
        m_solids.append(solid);
    }

    void FITKAbsGeoModelShellFromSolid::setSolids(const QList<VirtualShape>& solids)
    {
        m_solids = solids;
    }

    QList<VirtualShape> FITKAbsGeoModelShellFromSolid::solids() const
    {
        return m_solids;
    }
    //@}
}
