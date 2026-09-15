/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   FITKAbsGeoReferencePoint.h
 * @brief  抽象点.
 * @author YanZhiHui (chanyuantiandao@126.com)
 * @date   2024-08-27
 */
#ifndef  FITKABSGEOREFERENCEPOINT_H
#define  FITKABSGEOREFERENCEPOINT_H

#include "FITKInterfaceGeometryAPI.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoDatum.h"
#include <QVector>
#include <array>

namespace Interface
{
    /**
     * @brief  坐标点.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-27
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoReferencePoint :
        public FITKAbsGeoDatumPoint
    {
    public:
        FITKAbsGeoReferencePoint() = default;
        virtual ~FITKAbsGeoReferencePoint() override = default;
        /**
         * @brief  获取基准元素类型.
         * @return 基准元素类型
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-04
         */
        Interface::FITKGeoEnum::FITKDatumType getDatumType() override;

        double x() const { return m_pos[0]; };
        void setX(double x) { m_pos[0] = x; };
        double y() const { return m_pos[1]; };
        void setY(double y) { m_pos[1] = y; };
        double z() const { return m_pos[2]; };
        void setZ(double z) { m_pos[2] = z; };
        void setCoord(double x, double y, double z) {
            m_pos[0] = x; m_pos[1] = y; m_pos[2] = z;
        };

    };

    /**
     * @brief  指定曲线比例创建点.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-27
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoReferenceCurveRadio :
        public FITKAbsGeoReferencePoint
    {
        FITKCLASS(Interface, FITKAbsGeoReferenceCurveRadio);
    public:
        FITKAbsGeoReferenceCurveRadio() = default;
        virtual ~FITKAbsGeoReferenceCurveRadio() = default;
        /**
         * @brief  获取基准元素类型.
         * @return 基准元素类型
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-04
         */
        Interface::FITKGeoEnum::FITKDatumType getDatumType() override;
        VirtualShape sourceCurve() const { return m_SourceCurve; };
        void setSourceCurve(VirtualShape curve) { m_SourceCurve = curve; };
        double radio() const { return m_Radio; }
        void setRadio(double radio) { m_Radio = radio; }
    protected:
        VirtualShape m_SourceCurve{};
        double m_Radio{};
    };
}

#endif // !FITKABSGEOREFERENCEPOINT_H
