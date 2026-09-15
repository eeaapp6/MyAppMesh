/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKAbsGeoOperNBodyUnite.h
 * @brief       多实体布尔求和。
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2025-07-08
 *********************************************************************/

#ifndef __FITKABSGEOOPERNBODYUNITE_H__
#define __FITKABSGEOOPERNBODYUNITE_H__

#include "FITKInterfaceGeometryAPI.h"
#include "FITKAbsGeoCommand.h"

namespace Interface
{
    class FITKAbsGeoShapeAgent;

    /**
     * @brief       多实体布尔求和。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-07-08
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoOperNBodyUnite : public FITKAbsGeoCommand
    {
        FITKCLASS(Interface, FITKAbsGeoOperNBodyUnite);
        Q_OBJECT
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-08
         */
        explicit FITKAbsGeoOperNBodyUnite() = default;

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-08
         */
        virtual ~FITKAbsGeoOperNBodyUnite() = default;

        /**
         * @brief       获取几何命令类型。[重写]
         * @return      几何命令类型。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-08
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        /**
         * @brief       添加需要合并的几何实体拓扑。
         * @param[in]   shape：几何实体拓扑信息
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-08
         */
        void addShape(VirtualShape & shape);

        /**
         * @brief       获取合并实体拓扑信息列表。
         * @return      拓扑信息列表
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-08
         */
        const QList<VirtualShape> & getShapes();

        /**
         * @brief       清空需要合并的几何实体拓扑列表。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-08
         */
        void clearShapes();

    protected:
        /**
         * @brief       合并实体拓扑信息列表。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-08
         */
        QList<VirtualShape> m_shapes;

    };
}

#endif // !__FITKABSGEOOPERNBODYUNITE_H__
