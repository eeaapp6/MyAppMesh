/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKAbsGeoModelShellFromSolid.h
 * @brief       实体抽壳形成壳面几何命令抽象类。
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2026-07-06
 *********************************************************************/

#ifndef __FITKABSGEOMODELSHELLFROMSOLID_H__
#define __FITKABSGEOMODELSHELLFROMSOLID_H__

#include "FITKAbsGeoCommand.h"
#include "FITKInterfaceGeometryAPI.h"

namespace Interface
{
    /**
     * @brief       实体抽壳形成壳面几何命令抽象类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2026-07-06
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelShellFromSolid :
        public FITKAbsGeoCommand
    {
        FITKCLASS(Interface, FITKAbsGeoModelShellFromSolid);
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-07-06
         */
        FITKAbsGeoModelShellFromSolid() = default;

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-07-06
         */
        virtual ~FITKAbsGeoModelShellFromSolid() = default;

        /**
         * @brief       获取几何命令类型。
         * @return      几何命令类型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-07-06
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        /**
         * @brief       添加源实体拓扑。
         * @param[in]   solid：源实体虚拓扑
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-07-06
         */
        void addSolid(VirtualShape solid);

        /**
         * @brief       设置源实体拓扑列表。
         * @param[in]   solids：源实体虚拓扑列表
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-07-06
         */
        void setSolids(const QList<VirtualShape>& solids);

        /**
         * @brief       获取源实体拓扑列表。
         * @return      源实体虚拓扑列表
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-07-06
         */
        QList<VirtualShape> solids() const;

    protected:
        /**
         * @brief       源实体虚拓扑列表。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-07-06
         */
        QList<VirtualShape> m_solids{};
    };
}

#endif // __FITKABSGEOMODELSHELLFROMSOLID_H__
