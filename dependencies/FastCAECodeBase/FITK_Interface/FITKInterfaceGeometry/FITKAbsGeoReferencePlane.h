/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef  FITKABSGEOREFERENCEPLANE_H
#define  FITKABSGEOREFERENCEPLANE_H

#include "FITKInterfaceGeometryAPI.h"
#include "FITKAbsGeoDatum.h"
#include <array>

namespace Interface
{
    /**
     * @brief  参考面抽象类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-16
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoReferencePlane :
        public FITKAbsGeoDatumPlane
    {
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        FITKAbsGeoReferencePlane() = default;
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        virtual ~FITKAbsGeoReferencePlane() = default;

        void setPosition(double x, double y, double z) {
            m_pos[0] = x; m_pos[1] = y; m_pos[2] = z;
        }
        void setNormal(double x, double y, double z) {
            m_nor[0] = x; m_nor[1] = y; m_nor[2] = z;
        }
        void setUp(double x, double y, double z) {
            m_up[0] = x; m_up[1] = y; m_up[2] = z;
        }
    };


    /**
     * @brief  偏移参考面抽象类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-16
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoReferenceOffsetPlane :
        public FITKAbsGeoReferencePlane
    {
        FITKCLASS(Interface, FITKAbsGeoReferenceOffsetPlane);
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        FITKAbsGeoReferenceOffsetPlane() = default;
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        virtual ~FITKAbsGeoReferenceOffsetPlane() = default;
        /**
         * @brief  获取基准面类型.
         * @return 命令类型
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        FITKGeoEnum::FITKDatumType getDatumType() override;

        VirtualShape sourceSurface() const { return m_SourceSurface; }
        void setSourceSurface(VirtualShape shape) { m_SourceSurface = shape; }

        std::array<double, 3> sourcePlaneOrigin() const { return m_SourcePlaneOrigin; };
        void setSourcePlaneOrigin(std::array<double, 3> ori) { m_SourcePlaneOrigin = ori; };
        void setSourcePlaneOrigin(double x, double y, double z) { m_SourcePlaneOrigin = { x,y,z }; };
        std::array<double, 3> sourcePlaneNormal() const { return m_SourcePlaneNormal; };
        void setSourcePlaneNormal(std::array<double, 3> normal) { m_SourcePlaneNormal = normal; };
        void setSourcePlaneNormal(double x, double y, double z) { m_SourcePlaneNormal = { x,y,z }; };
        std::array<double, 3> sourcePlaneUp() const { return m_SourcePlaneUp; };
        void setSourcePlaneUp(std::array<double, 3> up) { m_SourcePlaneUp = up; };
        void setSourcePlaneUp(double x, double y, double z) { m_SourcePlaneUp = { x,y,z }; };

        std::array<double, 3> direction() const { return m_Direction; };
        void setDirection(std::array<double, 3> dir) { m_Direction = dir; };
        void setDirection(double x, double y, double z) { m_Direction = { x,y,z }; };
        double offset() const { return m_Offset; };
        void setOffset(double offset) { m_Offset = offset; };
    protected:
        /// 兼容OCC版本
        VirtualShape m_SourceSurface{};
        /**@{*/
        /// 源面
        std::array<double, 3> m_SourcePlaneOrigin{};
        std::array<double, 3> m_SourcePlaneNormal{};
        std::array<double, 3> m_SourcePlaneUp{};
        /**@}*/
        /**
         * @brief  偏移方向.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-23
         */
        std::array<double, 3> m_Direction{};
        /**
         * @brief  偏移距离.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        double m_Offset{};
    };

    /**
     * @brief  三点创建参考面抽象类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-16
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoReferenceThreePointsPlane :
        public FITKAbsGeoReferencePlane
    {
        FITKCLASS(Interface, FITKAbsGeoReferenceThreePointsPlane);
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        FITKAbsGeoReferenceThreePointsPlane() = default;
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        virtual ~FITKAbsGeoReferenceThreePointsPlane() = default;
        /**
         * @brief  获取基准面类型.
         * @return 命令类型
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        FITKGeoEnum::FITKDatumType getDatumType() override;

        std::array<double, 3> point(int index) {
            assert(index >= 0 && index < 3);
            return m_Points[index];
        }
        void setPoint(int index, std::array<double, 3> xyz) {
            assert(index >= 0 && index < 3);
            m_Points[index] = xyz;
        }
        void setPoint(int index, double x, double y, double z) {
            assert(index >= 0 && index < 3);
            m_Points[index] = { x,y,z };
        }
    protected:
        /**
         * @brief  面上的三点.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-04
         */
        std::array<std::array<double, 3>, 3> m_Points{};
    };

    /**
     * @brief  方程（Ax+By+Cz+D=0）创建参考面抽象类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-16
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoReferenceEquationPlane :
        public FITKAbsGeoReferencePlane
    {
        FITKCLASS(Interface, FITKAbsGeoReferenceEquationPlane);
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        FITKAbsGeoReferenceEquationPlane() = default;
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        virtual ~FITKAbsGeoReferenceEquationPlane() = default;
        /**
         * @brief  获取基准面类型.
         * @return 命令类型
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        FITKGeoEnum::FITKDatumType getDatumType() override;

        double a() const { return m_A; }
        void setA(double val) { m_A = val; }
        double b() const { return m_B; }
        void setB(double val) { m_B = val; }
        double c() const { return m_C; }
        void setC(double val) { m_C = val; }
        double d() const { return m_D; }
        void setD(double val) { m_D = val; }
    protected:
        /* 平面方程（Ax+By+Cz+D=0）系数 */
        /* @{ */
        double m_A{ 0 };
        double m_B{ 0 };
        double m_C{ 0 };
        double m_D{ 0 };
        /* @} */
    };
    /**
     * @brief  点和法线创建参考面抽象类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-16
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoReferencePointAndDirectionPlane :
        public FITKAbsGeoReferencePlane
    {
        FITKCLASS(Interface, FITKAbsGeoReferencePointAndDirectionPlane);
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        FITKAbsGeoReferencePointAndDirectionPlane() = default;
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        virtual ~FITKAbsGeoReferencePointAndDirectionPlane() = default;
        /**
         * @brief  获取基准面类型.
         * @return 命令类型
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        FITKGeoEnum::FITKDatumType getDatumType() override;

        std::array<double, 3> point() {
            return m_Point;
        }
        void setPoint(std::array<double, 3> xyz) {
            m_Point = xyz;
        }
        void setPoint(double x, double y, double z) {
            m_Point = { x,y,z };
        }

        std::array<double, 3> direction() {
            return m_Direction;
        }
        void setDirection(std::array<double, 3> xyz) {
            m_Direction = xyz;
        }
        void setDirection(double x, double y, double z) {
            m_Direction = { x,y,z };
        }

    protected:
        /**
         * @brief  平面上的一点.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        std::array<double, 3> m_Point{};
        /**
         * @brief  平面的法线法向.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        std::array<double, 3> m_Direction{};
    };


    /**
     * @brief  旋转已有面创建参考面抽象类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2025-07-02
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoReferenceRotateFromPlanePlane :
        public FITKAbsGeoReferencePlane
    {
        FITKCLASS(Interface, FITKAbsGeoReferenceRotateFromPlanePlane);
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2025-07-02
         */
        FITKAbsGeoReferenceRotateFromPlanePlane() = default;
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2025-07-02
         */
        virtual ~FITKAbsGeoReferenceRotateFromPlanePlane() = default;
        /**
         * @brief  获取基准面类型.
         * @return 命令类型
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2025-07-02
         */
        FITKGeoEnum::FITKDatumType getDatumType() override;

        void setPlane(QVector<double> plane) {
            m_PlanePos = { plane[0], plane[1], plane[2] };
            m_PlaneNor = { plane[3], plane[4], plane[5] };
            m_PlaneUp = { plane[6], plane[7], plane[8] };
        }
        std::array<double, 3> planePosition() const { return m_PlanePos; }
        std::array<double, 3> planeNormal() const { return m_PlaneNor; }
        std::array<double, 3> planeUp() const { return m_PlaneUp; }

        void setAxis(QVector<double> start, QVector<double> end) {
            m_AxisStart = { start[0], start[1], start[2] };
            m_AxisEnd = { end[0], end[1], end[2] };
        }
        std::array<double, 3> axisStart() const { return m_AxisStart; }
        std::array<double, 3> axisEnd() const { return m_AxisEnd; }

        void setAngle(double angle) { m_Angle = angle; }
        double angle() const { return m_Angle; }

    protected:
        /**
         * @brief  平面上的一点.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2025-07-04
         */
        std::array<double, 3> m_PlanePos{};
        /**
         * @brief  平面上的法线.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2025-07-04
         */
        std::array<double, 3> m_PlaneNor{};
        /**
         * @brief  平面上的向上方向.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2025-07-04
         */
        std::array<double, 3> m_PlaneUp{};
        /**
         * @brief  旋转轴起点.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2025-07-04
         */
        std::array<double, 3> m_AxisStart{};
        /**
         * @brief  旋转轴终点.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2025-07-04
         */
        std::array<double, 3> m_AxisEnd{};
        /**
         * @brief  旋转角度
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @data   2025-07-04
         */
        double m_Angle{};
    };
}

#endif // !FITKABSGEOREFERENCEPLANE_H
