/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   FITKAbsGeoModelPoint.h
 * @brief  抽象点.
 * @author YanZhiHui (chanyuantiandao@126.com)
 * @date   2024-08-27
 */
#ifndef  FITKABSGEOMODELPOINT_H
#define  FITKABSGEOMODELPOINT_H

#include "FITKInterfaceGeometryAPI.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"
#include <QVector>
#include <array>

namespace Interface
{
    /**
     * @brief  坐标点.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-27
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelPoint :
        public FITKAbsGeoCommand
    {
        FITKCLASS(Interface, FITKAbsGeoModelPoint);
    public:
        FITKAbsGeoModelPoint() = default;
        FITKAbsGeoModelPoint(double x, double y, double z);
        virtual ~FITKAbsGeoModelPoint() = default;
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        double x() const;
        void setX(double x);
        double y() const;
        void setY(double y);
        double z() const;
        void setZ(double z);
        void setCoord(double x, double y, double z);

    protected:
        double m_X{ 0.0 };
        double m_Y{ 0.0 };
        double m_Z{ 0.0 };
    };
    /**
     * @brief  曲线的端点.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-27
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelCurveEnd :
        public FITKAbsGeoModelPoint
    {
        FITKCLASS(Interface, FITKAbsGeoModelCurveEnd);
    public:
        FITKAbsGeoModelCurveEnd() = default;
        virtual ~FITKAbsGeoModelCurveEnd() = default;
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;
        VirtualShape sourceCurve() const;
        void setSourceCurve(VirtualShape curve);
        bool useStartEnd() const;
        void setUseStartEnd(bool use);
    protected:
        VirtualShape m_SourceCurve{};
        bool m_UseStartEnd{ true };
    };
    /**
     * @brief  距离曲线的端点指定距离.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-27
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelCurveEndDistance :
        public FITKAbsGeoModelPoint
    {
        FITKCLASS(Interface, FITKAbsGeoModelCurveEndDistance);
    public:
        FITKAbsGeoModelCurveEndDistance() = default;
        virtual ~FITKAbsGeoModelCurveEndDistance() = default;
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;
        VirtualShape sourceCurve() const;
        void setSourceCurve(VirtualShape curve);
        bool useStartEnd() const;
        void setUseStartEnd(bool use);
        double distance() const;
        void setDistance(double distance);
    protected:
        VirtualShape m_SourceCurve{};
        bool m_UseStartEnd{ true };
        double m_Distance{};
    };
    /**
     * @brief  指定曲线比例创建点.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-27
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelCurveRadio :
        public FITKAbsGeoModelPoint
    {
        FITKCLASS(Interface, FITKAbsGeoModelCurveRadio);
    public:
        FITKAbsGeoModelCurveRadio() = default;
        virtual ~FITKAbsGeoModelCurveRadio() = default;
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;
        VirtualShape sourceCurve() const;
        void setSourceCurve(VirtualShape curve);
        double radio() const;
        void setRadio(double radio);
    protected:
        VirtualShape m_SourceCurve{};
        double m_Radio{};
    };
    /**
     * @brief  点在线上的投影.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-27
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelPointProjectionOnCurve :
        public FITKAbsGeoModelPoint
    {
        FITKCLASS(Interface, FITKAbsGeoModelPointProjectionOnCurve);
    public:
        FITKAbsGeoModelPointProjectionOnCurve() = default;
        virtual ~FITKAbsGeoModelPointProjectionOnCurve() = default;
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;
        VirtualShape sourcePoint() const;
        void setSourcePoint(VirtualShape point);
        VirtualShape sourceCurve() const;
        void setSourceCurve(VirtualShape curve);
        QVector<std::array<double, 3>> getResultPoints() { return m_Projections; };
    protected:
        VirtualShape m_SourcePoint{};
        VirtualShape m_SourceCurve{};
        QVector<std::array<double, 3>> m_Projections{};
    };
    /**
     * @brief  两条线的交点.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-27
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelTwoCurveIntersection :
        public FITKAbsGeoModelPoint
    {
        FITKCLASS(Interface, FITKAbsGeoModelTwoCurveIntersection);
    public:
        FITKAbsGeoModelTwoCurveIntersection() = default;
        virtual ~FITKAbsGeoModelTwoCurveIntersection() = default;
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;
        VirtualShape sourceCurve1() const;
        VirtualShape sourceCurve2() const;
        void setSourceCurve(VirtualShape curve1, VirtualShape curve2);
        QVector<std::array<double, 3>> getResultPoints() { return m_Intersections; };
        bool isNearestPoint() const { return m_IsNearestPoint; }
        void setNearestPoint(bool nearest) { m_IsNearestPoint = nearest; }
    protected:
        VirtualShape m_SourceCurve1{};
        VirtualShape m_SourceCurve2{};
        QVector<std::array<double, 3>> m_Intersections{};
        /**
         * @brief  是否求最近点（否则为交点）.
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-10-21
         */
        bool m_IsNearestPoint{ false };
    };

    /**
     * @brief  点在面上的投影.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-27
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelPointProjectionOnSurface :
        public FITKAbsGeoModelPoint
    {
        FITKCLASS(Interface, FITKAbsGeoModelPointProjectionOnSurface);
    public:
        FITKAbsGeoModelPointProjectionOnSurface() = default;
        virtual ~FITKAbsGeoModelPointProjectionOnSurface() = default;
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;
        VirtualShape sourcePoint() const;
        void setSourcePoint(VirtualShape point);
        VirtualShape sourceSurface() const;
        void setSourceSurface(VirtualShape surface);
        QVector<std::array<double, 3>> getResultPoints() { return m_Projections; };
    protected:
        VirtualShape m_SourcePoint{};
        VirtualShape m_SourceSurface{};
        QVector<std::array<double, 3>> m_Projections{};
    };
    /**
     * @brief  线和面的交点.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-27
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelCurveIntersectionSurface :
        public FITKAbsGeoModelPoint
    {
        FITKCLASS(Interface, FITKAbsGeoModelCurveIntersectionSurface);
    public:
        FITKAbsGeoModelCurveIntersectionSurface() = default;
        virtual ~FITKAbsGeoModelCurveIntersectionSurface() = default;
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;
        VirtualShape sourceCurve() const;
        void setSourceCurve(VirtualShape curve);
        VirtualShape sourceSurface() const;
        void setSourceSurface(VirtualShape surface);
        QVector<std::array<double, 3>> getResultPoints() { return m_Intersections; };
    protected:
        VirtualShape m_SourceCurve{};
        VirtualShape m_SourceSurface{};
        QVector<std::array<double, 3>> m_Intersections{};
    };

    /**
     * @brief  在线、面上交互生成点.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-27
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelInteractionPoint :
        public FITKAbsGeoModelPoint
    {
        FITKCLASS(Interface, FITKAbsGeoModelInteractionPoint);
    public:
        FITKAbsGeoModelInteractionPoint() = default;
        virtual ~FITKAbsGeoModelInteractionPoint() = default;
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;
        VirtualShape source() const { return m_Source; }
        void setSource(VirtualShape source) { m_Source = source; }
        int type() const { return m_Type; }
        void setType(int type) { m_Type = type; }
    protected:
        VirtualShape m_Source{};
      int m_Type {};
    };
}

#endif // !FITKABSGEOMODELPOINT_H
