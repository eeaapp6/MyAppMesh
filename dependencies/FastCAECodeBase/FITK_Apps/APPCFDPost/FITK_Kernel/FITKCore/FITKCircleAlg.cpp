/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKCircleAlg.h"
#include "FITKVec3DAlg.h"
#include <cmath>

namespace Core
{
    QPair<FITKPoint, FITKPoint> GetCircleCenter(const FITKPoint& p1, const FITKPoint& p2,
                                                const FITKPoint& another, const double r)
    {
        QPair<FITKPoint, FITKPoint> centers;
        //弦 P1 P2中点
        const FITKPoint mid((p1.x() + p2.x()) / 2, (p1.y() + p2.y()) / 2, (p1.z() + p2.z()) / 2);
        //弦长度的一半，与半径可形成直角三角形
        const double d = Distance(p1, mid);
        if (d > r) return centers;
        //直角三角形的另一边长度
        const double offset = sqrt(r*r - d * d);
        const FITKVec3D m2(p2, mid);
        const FITKVec3D mo(another, mid);
        const FITKVec3D mz = CrossProduct(m2, mo);
        //半径方向
        FITKVec3D dir = CrossProduct(m2, mz);
        dir.normalize();

        //弦中心点向两侧偏移分别得到两个圆心位置
        centers.first = FITKPoint(mid.x() + dir.x()* offset,
                                  mid.y() + dir.y()* offset,
                                  mid.z() + dir.z()* offset);

        centers.second = FITKPoint(mid.x() - dir.x()* offset,
                                   mid.y() - dir.y()* offset,
                                   mid.z() - dir.z()* offset);
        return centers;
    }

    FITKPoint GetMidPointOnArc(const FITKPoint& p1, const FITKPoint& p2, const FITKPoint& center, const double r)
    {
        //弦 P1 P2中点
        const FITKPoint mid((p1.x() + p2.x()) / 2, (p1.y() + p2.y()) / 2, (p1.z() + p2.z()) / 2);
        const double d = Distance(p1, mid);
        if (d >= r) return FITKPoint();
        //圆心到弦中点的方向
        FITKVec3D dir(mid,center);
        dir.normalize();

        //圆心偏移
        return Add(center, dir, 1, r);
    }

 
    void GetCircleBy3Points(const FITKPoint& p1, const FITKPoint& p2, const FITKPoint& p3, 
        FITKPoint& center, FITKVec3D& normal, double& r)
    {
        //计算圆心
        double a1 = (p1.y()*p2.z() - p2.y() * p1.z() - p1.y() * p3.z() + p3.y() * p1.z() + p2.y() * p3.z() - p3.y() * p2.z()),
            b1 = -(p1.x()*p2.z() - p2.x() * p1.z() - p1.x() * p3.z() + p3.x() * p1.z() + p2.x() * p3.z() - p3.x() * p2.z()),
            c1 = (p1.x()*p2.y() - p2.x() * p1.y() - p1.x() * p3.y() + p3.x() * p1.y() + p2.x() * p3.y() - p3.x() * p2.y()),
            d1 = -(p1.x()*p2.y()*p3.z() - p1.x() * p3.y()*p2.z() - p2.x() * p1.y()*p3.z() + p2.x() * p3.y()*p1.z() + p3.x() * p1.y()*p2.z() - p3.x() * p2.y()*p1.z());

        double a2 = 2 * (p2.x() - p1.x()),
            b2 = 2 * (p2.y() - p1.y()),
            c2 = 2 * (p2.z() - p1.z()),
            d2 = p1.x() * p1.x() + p1.y() * p1.y() + p1.z() * p1.z() - p2.x() * p2.x() - p2.y() * p2.y() - p2.z() * p2.z();

        double a3 = 2 * (p3.x() - p1.x()),
            b3 = 2 * (p3.y() - p1.y()),
            c3 = 2 * (p3.z() - p1.z()),
            d3 = p1.x() * p1.x() + p1.y() * p1.y() + p1.z() * p1.z() - p3.x() * p3.x() - p3.y() * p3.y() - p3.z() * p3.z();

        double x = -(b1*c2*d3 - b1 * c3*d2 - b2 * c1*d3 + b2 * c3*d1 + b3 * c1*d2 - b3 * c2*d1)
            / (a1*b2*c3 - a1 * b3*c2 - a2 * b1*c3 + a2 * b3*c1 + a3 * b1*c2 - a3 * b2*c1);
        double y = (a1*c2*d3 - a1 * c3*d2 - a2 * c1*d3 + a2 * c3*d1 + a3 * c1*d2 - a3 * c2*d1)
            / (a1*b2*c3 - a1 * b3*c2 - a2 * b1*c3 + a2 * b3*c1 + a3 * b1*c2 - a3 * b2*c1);
        double z = -(a1*b2*d3 - a1 * b3*d2 - a2 * b1*d3 + a2 * b3*d1 + a3 * b1*d2 - a3 * b2*d1)
            / (a1*b2*c3 - a1 * b3*c2 - a2 * b1*c3 + a2 * b3*c1 + a3 * b1*c2 - a3 * b2*c1);
        //计算半径
        r = sqrt((p1.x() - x)*(p1.x() - x) + (p1.y() - y)*(p1.y() - y) + (p1.z() - z)*(p1.z() - z));
        //返回圆心
        center = FITKPoint(x, y, z);
        //计算三个点法向
        const FITKVec3D v1(p1, p2);
        const FITKVec3D v2(p1, p3);
        normal = CrossProduct(v1, v2);
        normal.normalize();
    }

}




