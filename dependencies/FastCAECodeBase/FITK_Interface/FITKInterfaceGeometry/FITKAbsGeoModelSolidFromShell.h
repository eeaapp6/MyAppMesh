/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKAbsGeoModelSolidFromShell.h
 * @brief       封闭壳面形成实体几何命令抽象类。
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2026-07-06
 *********************************************************************/

#ifndef __FITKABSGEOMODELSOLIDFROMSHELL_H__
#define __FITKABSGEOMODELSOLIDFROMSHELL_H__

#include "FITKAbsGeoCommand.h"
#include "FITKInterfaceGeometryAPI.h"

namespace Interface
{
    /**
     * @brief       封闭壳面形成实体几何命令抽象类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2026-07-06
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelSolidFromShell :
        public FITKAbsGeoCommand
    {
        FITKCLASS(Interface, FITKAbsGeoModelSolidFromShell);
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-07-06
         */
        FITKAbsGeoModelSolidFromShell() = default;

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-07-06
         */
        virtual ~FITKAbsGeoModelSolidFromShell() = default;

        /**
         * @brief       获取几何命令类型。
         * @return      几何命令类型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-07-06
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        /**
         * @brief       添加源面。
         * @param[in]   face：源面虚拓扑
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-07-06
         */
        void addFace(VirtualShape face);

        /**
         * @brief       设置源面列表。
         * @param[in]   faces：源面虚拓扑列表
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-07-06
         */
        void setFaces(const QList<VirtualShape>& faces);

        /**
         * @brief       获取源面列表。
         * @return      源面虚拓扑列表
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-07-06
         */
        QList<VirtualShape> faces() const;

    protected:
        /**
         * @brief       源面虚拓扑列表。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-07-06
         */
        QList<VirtualShape> m_faces{};
    };
}

#endif // __FITKABSGEOMODELSOLIDFROMSHELL_H__
