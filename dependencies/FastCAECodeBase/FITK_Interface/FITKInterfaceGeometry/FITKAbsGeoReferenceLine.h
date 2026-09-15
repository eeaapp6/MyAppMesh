/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef  FITKABSGEOREFERENCELINE_H
#define  FITKABSGEOREFERENCELINE_H

#include "FITKInterfaceGeometryAPI.h"
#include "FITKAbsGeoDatum.h"
#include <array>

namespace Interface
{
    /**
     * @brief  曲线抽象类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-13
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoReferenceLine :
        public FITKAbsGeoDatumLine
    {
        FITKCLASS(Interface, FITKAbsGeoReferenceLine);
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        FITKAbsGeoReferenceLine() = default;
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        virtual ~FITKAbsGeoReferenceLine() = default;
        /**
         * @brief  获取基准元素类型.
         * @return 基准元素类型
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-04
         */
        FITKGeoEnum::FITKDatumType getDatumType() override;
    };
    /**
     * @brief  有限线段抽象类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-13
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoReferenceSegment : public FITKAbsGeoReferenceLine {
        FITKCLASS(Interface, FITKAbsGeoReferenceSegment);
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        FITKAbsGeoReferenceSegment() = default;
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        ~FITKAbsGeoReferenceSegment() override = default;
        /**
         * @brief  获取起点.
         * @return 起点坐标数组
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        std::array<double, 3> startPoint() const { return { m_pos[0] ,m_pos[1] ,m_pos[2] }; };
        /**
         * @brief  设置起点.
         * @param  x 起点的x坐标值
         * @param  y 起点的y坐标值
         * @param  z 起点的z坐标值
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        void setStartPoint(double x, double y, double z) { m_pos[0] = x; m_pos[1] = y; m_pos[2] = z; };
        void setStartPoint(std::array<double, 3> xyz) { m_pos[0] = xyz[0]; m_pos[1] = xyz[1]; m_pos[2] = xyz[2]; };
        /**
         * @brief  获取终点.
         * @return 终点坐标数组
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        std::array<double, 3> endPoint() const { return { m_pos2[0] ,m_pos2[1] ,m_pos2[2] }; };
        /**
         * @brief  设置终点.
         * @param  x 终点的x坐标值
         * @param  y 终点的y坐标值
         * @param  z 终点的z坐标值
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        void setEndPoint(double x, double y, double z) { m_pos2[0] = x; m_pos2[1] = y; m_pos2[2] = z; };
        void setEndPoint(std::array<double, 3> xyz) { m_pos2[0] = xyz[0]; m_pos2[1] = xyz[1]; m_pos2[2] = xyz[2]; };
        /**
         * @brief   获取基准元素类型。[重写]
         * @return  基准元素类型
         * @author  @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        FITKGeoEnum::FITKDatumType getDatumType() override;

    };


    /**
     * @brief  平面相交线抽象类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2025-07-08
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoReferenceLineByIntersectPlanes : public FITKAbsGeoReferenceLine {
        FITKCLASS(Interface, FITKAbsGeoReferenceLineByIntersectPlanes);
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2025-07-08
         */
        FITKAbsGeoReferenceLineByIntersectPlanes() = default;
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2025-07-08
         */
        ~FITKAbsGeoReferenceLineByIntersectPlanes() override = default;
        /**
         * @brief   获取基准元素类型。[重写]
         * @return  基准元素类型
         * @author  @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2025-07-08
         */
        FITKGeoEnum::FITKDatumType getDatumType() override;

        void setPlane1(QVector<double> plane) { m_Plane1 = plane; }
        QVector<double> plane1() const { return m_Plane1; }
        void setPlane2(QVector<double> plane) { m_Plane2 = plane; }
        QVector<double> plane2() const { return m_Plane2; }

    protected:
        /*@{*/
        /// plane的格式：position[3], normal[3], uDirection[3]
        QVector<double> m_Plane1{};
        QVector<double> m_Plane2{};
        /*@}*/

    };


    /**
     * @brief  旋转线抽象类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2025-07-08
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoReferenceLineByRotateLine : public FITKAbsGeoReferenceLine {
        FITKCLASS(Interface, FITKAbsGeoReferenceLineByRotateLine);
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2025-07-08
         */
        FITKAbsGeoReferenceLineByRotateLine() = default;
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2025-07-08
         */
        ~FITKAbsGeoReferenceLineByRotateLine() override = default;
        /**
         * @brief   获取基准元素类型。[重写]
         * @return  基准元素类型
         * @author  @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2025-07-08
         */
        FITKGeoEnum::FITKDatumType getDatumType() override;
        void setLine(QVector<double> start, QVector<double> end) {
            m_LineStart = { start[0], start[1], start[2] };
            m_LineEnd = { end[0], end[1], end[2] };
        }
        std::array<double, 3> lineStart() const { return m_LineStart; }
        std::array<double, 3> lineEnd() const { return m_LineEnd; }

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
         * @brief  旋转直线起点.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2025-07-10
         */
        std::array<double, 3> m_LineStart{};
        /**
         * @brief  旋转直线终点.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2025-07-10
         */
        std::array<double, 3> m_LineEnd{};
        /**
         * @brief  旋转轴起点.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2025-07-10
         */
        std::array<double, 3> m_AxisStart{};
        /**
         * @brief  旋转轴终点.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2025-07-10
         */
        std::array<double, 3> m_AxisEnd{};
        /**
         * @brief  旋转角度
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @data   2025-07-10
         */
        double m_Angle{};
    };

}

#endif // !FITKABSGEOREFERENCELINE_H
