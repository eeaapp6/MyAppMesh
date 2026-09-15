/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKAbsGeoMergeEdges.h
 * @brief       合并边抽象命令。
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2026-01-22
 *********************************************************************/

#ifndef __FITKABSGEOMERGEEDGES_H__
#define __FITKABSGEOMERGEEDGES_H__

#include "FITKAbsGeoCommand.h"
#include "FITKInterfaceGeometryAPI.h"
#include <array>

namespace Interface 
{
    /**
     * @brief       合并边抽象命令。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2026-01-22
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoMergeEdges : public FITKAbsGeoCommand
    {
        FITKCLASS(Interface, FITKAbsGeoMergeEdges);
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-01-22
         */
        FITKAbsGeoMergeEdges() = default;

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-01-22
         */
        virtual ~FITKAbsGeoMergeEdges() = default;

        /**
         * @brief       获取几何命令类型。[重写]
         * @return      命令类型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-01-22
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        /**
         * @brief       获取合并边列表。
         * @return      合并边信息。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-01-22
         */
        QList<VirtualShape> edges() const { return m_Edges; }

        /**
         * @brief       保存合并边列表。
         * @param[in]   edges：合并边信息
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-01-22
         */
        void setEdges(QList<VirtualShape> edges){ m_Edges= edges;}

    protected:
        /**
         * @brief       合并边信息。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-01-22
         */
        QList<VirtualShape> m_Edges{};

    };
}
#endif // !__FITKABSGEOMERGEEDGES_H__
