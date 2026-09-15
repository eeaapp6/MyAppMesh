/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   FITKAbsGeoStitch.h
 * @brief  缝合面
 * @author Yanzhihui (chanyuantiandao@126.com)
 * @date   2025-03-25
 */
#ifndef FITKABSGEOSTITCH_H
#define FITKABSGEOSTITCH_H


#include "FITKAbsGeoCommand.h"
#include "FITKAbsGeoDatum.h"
#include "FITKInterfaceGeometryAPI.h"
#include <array>

namespace Interface {
    /**
     * @brief  分割基类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-09-10
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoStitch :
        public FITKAbsGeoCommand
    {
        FITKCLASS(Interface, FITKAbsGeoStitch);
    public:
        FITKAbsGeoStitch() = default;
        virtual ~FITKAbsGeoStitch() = default;
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        double tolerance() const { return m_Tolerance; }
        void setTolerance(double tol) { m_Tolerance = tol; }
        QList<VirtualShape> edges() const { return m_Edges; }
        void setEdges(QList<VirtualShape> edges){ m_Edges= edges;}

    protected:
        /**
         * @brief  最大容差
         * @author Yanzhihui (chanyuantiandao@126.com)
         * @date   2025-03-25
         */
        double m_Tolerance{};
        /**
         * @brief  拼接使用的边
         * @author Yanzhihui (chanyuantiandao@126.com)
         * @date   2025-03-25
         */
        QList<VirtualShape> m_Edges{};
    };
}
#endif // !FITKABSGEOSTITCH_H
