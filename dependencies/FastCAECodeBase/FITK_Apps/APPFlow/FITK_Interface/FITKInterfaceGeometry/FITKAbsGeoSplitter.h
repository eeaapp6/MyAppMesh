/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   FITKAbsGeoSplitter.h
 * @brief  模型分割.
 * @author YanZhiHui (chanyuantiandao@126.com)
 * @date   2024-09-13
 */
#ifndef FITKABSGEOSPLITTER_H
#define FITKABSGEOSPLITTER_H


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
    class FITKInterfaceGeometryAPI FITKAbsGeoSplitter :
        public FITKAbsGeoCommand
    {
        FITKCLASS(Interface, FITKAbsGeoSplitter);
    public:
        FITKAbsGeoSplitter() = default;
        virtual ~FITKAbsGeoSplitter() = default;

        VirtualShape sourceShape() const { return m_SourceShape; }
        void setSourceShape(VirtualShape shape) { m_SourceShape = shape; }
        VirtualShape toolShape() const { return m_ToolShape; }
        void setToolShape(VirtualShape shape) { m_ToolPoint = {}; m_ToolShape = shape; }
        std::array<double, 3> toolPoint() const { return m_ToolPoint; }
        void setToolPoint(std::array<double, 3> point) { m_ToolShape.reset(); m_ToolPoint = { point.at(0), point.at(1), point.at(2) }; }

    protected:
        /**
         * @brief  要被分割的形状.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-13
         */
        VirtualShape m_SourceShape{};
        /**
         * @brief  分割工具形状.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-13
         */
        VirtualShape m_ToolShape{};
        /**
         * @brief  分割工具点（交互生成）.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-13
         */
        std::array<double,3> m_ToolPoint{};
    };

    /**
     * @brief  曲线分割抽象类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-09-10
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoCurveSplitter :
        public FITKAbsGeoSplitter
    {
        FITKCLASS(Interface, FITKAbsGeoCurveSplitter);
    public:
        FITKAbsGeoCurveSplitter() = default;
        ~FITKAbsGeoCurveSplitter() override = default;
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        std::array<double, 3> point() const { return m_Point; }
        void setPoint(std::array<double, 3> xyz) { m_Point = xyz; }
        void setPoint(double x, double y, double z) { m_Point = { x,y,z }; }
    protected:
        /**
         * @brief  保存点分割时候的坐标.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-10
         */
        std::array<double, 3> m_Point{};
    };

    /**
     * @brief  面分割抽象类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-09-10
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoSurfaceSplitter :
        public FITKAbsGeoSplitter
    {
        FITKCLASS(Interface, FITKAbsGeoSurfaceSplitter);
    public:
        FITKAbsGeoSurfaceSplitter() = default;
        ~FITKAbsGeoSurfaceSplitter() override = default;
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    };

    /**
     * @brief  体分割抽象类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-09-10
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoSolidSplitter :
        public FITKAbsGeoSplitter
    {
        FITKCLASS(Interface, FITKAbsGeoSolidSplitter);
    public:
        FITKAbsGeoSolidSplitter() = default;
        ~FITKAbsGeoSolidSplitter() override = default;
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    };
}
#endif // FITKABSGEOSPLITTER_H
