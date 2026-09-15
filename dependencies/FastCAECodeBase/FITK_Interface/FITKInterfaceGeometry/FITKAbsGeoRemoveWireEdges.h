/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKAbsGeoRemoveWireEdges.h
 * @brief       移除线边命令数据抽象类。
 *
 * @author      FastCAE
 * @date        2026-07-01
 *********************************************************************/

#ifndef FITKABSGEOREMOVEWIREDGES_H
#define FITKABSGEOREMOVEWIREDGES_H

#include "FITKInterfaceGeometryAPI.h"
#include "FITKAbsGeoCommand.h"

namespace Interface
{
    /**
     * @brief       移除线边命令数据抽象类。
     * @author      FastCAE
     * @date        2026-07-01
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoRemoveWireEdges : public FITKAbsGeoCommand
    {
        FITKCLASS(Interface, FITKAbsGeoRemoveWireEdges);
    public:
        FITKAbsGeoRemoveWireEdges() = default;
        ~FITKAbsGeoRemoveWireEdges() override = default;

        /**
         * @brief       获取几何命令类型。
         * @return      命令类型
         * @author      FastCAE
         * @date        2026-07-01
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        /**
         * @brief       添加线边。
         * @param[in]   edge：线边
         * @author      FastCAE
         * @date        2026-07-01
         */
        void add(VirtualShape edge);

        /**
         * @brief       添加线边组。
         * @param[in]   edges：线边组
         * @author      FastCAE
         * @date        2026-07-01
         */
        void add(QList<VirtualShape> edges);

        /**
         * @brief       设置线边组。
         * @param[in]   edges：线边组
         * @author      FastCAE
         * @date        2026-07-01
         */
        void set(QList<VirtualShape> edges);

        /**
         * @brief       获取线边组。
         * @return      线边组
         * @author      FastCAE
         * @date        2026-07-01
         */
        QList<VirtualShape> getVShapes();

    protected:
        /**
         * @brief       线边组。
         * @author      FastCAE
         * @date        2026-07-01
         */
        QList<VirtualShape> m_tempVShapes{};
    };
}

#endif // FITKABSGEOREMOVEWIREDGES_H
