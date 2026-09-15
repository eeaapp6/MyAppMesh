/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKAbsGeoWireEdgeFromEdge.h
 * @brief       根据边创建线边抽象命令。
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2026-07-07
 *********************************************************************/

#ifndef __FITKABSGEOWIREEDGEFROMEDGE_H__
#define __FITKABSGEOWIREEDGEFROMEDGE_H__

#include "FITKAbsGeoCommand.h"
#include "FITKInterfaceGeometryAPI.h"

namespace Interface
{
    /**
     * @brief       根据边创建线边抽象命令。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2026-07-07
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoWireEdgeFromEdge : public FITKAbsGeoCommand
    {
        FITKCLASS(Interface, FITKAbsGeoWireEdgeFromEdge);
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-07-07
         */
        FITKAbsGeoWireEdgeFromEdge() = default;

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-07-07
         */
        virtual ~FITKAbsGeoWireEdgeFromEdge() = default;

        /**
         * @brief       获取几何命令类型。[重写]
         * @return      命令类型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-07-07
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        /**
         * @brief       获取源边拓扑列表。
         * @return      源边拓扑列表
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-07-07
         */
        QList<VirtualShape> edges() const;

        /**
         * @brief       保存源边拓扑列表。
         * @param[in]   edges：源边拓扑列表
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-07-07
         */
        void setEdges(QList<VirtualShape> edges);

    protected:
        /**
         * @brief       源边拓扑列表。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-07-07
         */
        QList<VirtualShape> m_Edges{};
    };
}

#endif // !__FITKABSGEOWIREEDGEFROMEDGE_H__
