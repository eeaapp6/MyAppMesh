/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKAbsGeoModelSolidFromShell.cpp
 * @brief       封闭壳面形成实体几何命令抽象类实现。
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2026-07-06
 *********************************************************************/

#include "FITKAbsGeoModelSolidFromShell.h"

namespace Interface
{
    // 命令类型。
    //@{
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelSolidFromShell::getGeometryCommandType()
    {
        // 封闭壳面形成实体。
        return FITKGeoEnum::FITKGeometryComType::FGTSolidFromShell;
    }

    void FITKAbsGeoModelSolidFromShell::addFace(VirtualShape face)
    {
        m_faces.append(face);
    }

    void FITKAbsGeoModelSolidFromShell::setFaces(const QList<VirtualShape>& faces)
    {
        m_faces = faces;
    }

    QList<VirtualShape> FITKAbsGeoModelSolidFromShell::faces() const
    {
        return m_faces;
    }
}
