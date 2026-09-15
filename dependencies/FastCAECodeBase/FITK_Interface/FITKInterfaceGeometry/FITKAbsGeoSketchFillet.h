/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   FITKAbsGeoSketchFillet.h
 * @brief  草图倒角
 * @author YanZhiHui (chanyuantiandao@126.com)
 * @date   2025-04-27
 */
#ifndef FITKABSGEOSKETCHFILLET_H
#define FITKABSGEOSKETCHFILLET_H


#include "FITKAbsGeoCommand.h"
#include "FITKInterfaceGeometryAPI.h"
#include <array>

namespace Interface {
    /**
     * @brief  草图倒角.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2025-04-27
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoSketchFillet :
        public FITKAbsGeoCommand
    {
        FITKCLASS(Interface, FITKAbsGeoSplitter);
    public:
        FITKAbsGeoSketchFillet() = default;
        virtual ~FITKAbsGeoSketchFillet() = default;
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;
        VirtualShape firstEdge() const { return m_FirstEdge; }
        void setFirstEdge(VirtualShape shape) { m_FirstEdge = shape; }
        VirtualShape secondEdge() const { return m_SecondEdge; }
        void setSecondEdge(VirtualShape shape) { m_SecondEdge = shape; }
        double radius() const { return m_Radius; }
        void setRadius(double radius) { m_Radius = radius; }

    protected:
        /**
         * @brief  第一条边.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2025-04-27
         */
        VirtualShape m_FirstEdge{};
        /**
         * @brief  第二条边.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2025-04-27
         */
        VirtualShape m_SecondEdge{};
        /**
         * @brief  圆角半径.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2025-04-27
         */
        double m_Radius{};
    };

}
#endif // !FITKABSGEOSKETCHFILLET_H
