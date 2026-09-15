/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKVec3DAlg.h"
#include <cmath>

namespace Core
{

    FITKPoint FITKCoreAPI Add(const FITKPoint& a, const FITKPoint& b, const double aFac, const double bFac)
    {
        double c[3] = { 0,0,0 };
 
        c[1] = a.y()*aFac + b.y()*bFac;
        c[0] = a.x()*aFac + b.x()*bFac;
        c[2] = a.z()*aFac + b.z()*bFac;

        // 创建并返回新的FITKPoint对象，存储结果
        return FITKPoint(c);
    }

    FITKPoint FITKCoreAPI Subtract(const FITKPoint& a, const FITKPoint& b, const double aFac, const double bFac )
    {
        double c[3] = { 0,0,0 };
 
        c[0] = a.x()*aFac - b.x()*bFac;
        c[1] = a.y()*aFac - b.y()*bFac;
        c[2] = a.z()*aFac - b.z()*bFac;

        // 创建并返回新的FITKPoint对象，存储结果
        return FITKPoint(c);
    }

    /**
     * 计算两个三维点（FITKPoint类型）的向量叉积，并返回新的FITKPoint对象。
     *
     * @param a 第一个输入向量（FITKPoint类型）
     * @param b 第二个输入向量（FITKPoint类型）
     * @return 叉积结果（FITKPoint类型）
     */
    FITKPoint CrossProduct(const FITKPoint& a, const FITKPoint& b)
    {
        double c[3] = { 0,0,0 };

        // 根据叉积定义计算各个分量
        c[0] = a.y() * b.z() - a.z() * b.y();
        c[1] = a.z() * b.x() - a.x() * b.z();
        c[2] = a.x() * b.y() - a.y() * b.x();

        // 创建并返回新的FITKPoint对象，存储叉积结果
        return FITKPoint(c);
    }

    /**
     * 计算两个三维点（FITKPoint类型）的向量点积，并返回标量结果。
     *
     * @param a 第一个输入向量（FITKPoint类型）
     * @param b 第二个输入向量（FITKPoint类型）
     * @return 点积结果（double类型）
     */
    double DotProduct(const FITKPoint & a, const FITKPoint & b)
    {
        // 根据点积定义计算标量结果
        return a.x() * b.x() + a.y() * b.y() + a.z() * b.z();
    }

    /**
     * 将三维点P绕给定轴线Axis按指定角度Angle进行旋转，并返回旋转后的FITKPoint对象。
     *
     * @param P 待旋转的三维点（FITKPoint类型）
     * @param Axis 旋转轴线
     * @param Angle 旋转角度，以弧度为单位（double类型）
     * @return 旋转后的三维点（FITKPoint类型）
     */
    FITKPoint RotateAroundAxis(const FITKPoint& P,  FITKVec3D& Axis, double Angle)
    {
        Axis.normalize();
        double cosTheta = std::cos(Angle);
        double sinTheta = std::sin(Angle);
        double kP = DotProduct(Axis, P); // 计算点P在旋转轴上的投影长度
        FITKPoint kCrossP = CrossProduct(Axis, P); // 计算点P相对于旋转轴的垂直向量

        // 应用罗德里格斯公式计算旋转后的点坐标
        FITKPoint pt(
            P.x() * cosTheta + kCrossP.x() * sinTheta + Axis.x() * kP * (1.0 - cosTheta),
            P.y() * cosTheta + kCrossP.y() * sinTheta + Axis.y() * kP * (1.0 - cosTheta),
            P.z() * cosTheta + kCrossP.z() * sinTheta + Axis.z() * kP * (1.0 - cosTheta)
        );

        return pt; // 返回旋转后的FITKPoint对象
    }

    FITKPoint FITKCoreAPI RotateAroundAnyAxis(const FITKPoint& P, const FITKPoint& BasicPt, FITKVec3D& Axis, double Angle)
    {
        //计算点平移到原点
        FITKPoint tp = Add(P, BasicPt, 1.0, -1.0);
        //旋转
        tp = RotateAroundAxis(tp, Axis, Angle);
        //计算结果平移回来
        tp = Add(tp, BasicPt);
        //输出
        return tp;
    }

    double FITKCoreAPI Distance(const FITKPoint& a, const FITKPoint& b)
    {
        double dx = a.x() - b.x();
        double dy = a.y() - b.y();
        double dz = a.z() - b.z();
        return sqrt(dx*dx + dy * dy + dz * dz);
    }

    double FITKCoreAPI AngleBetweenVec(const FITKVec3D& v1, const FITKVec3D& v2)
    {
        //点乘
        const double dp = DotProduct(v1, v2);
        //角度余弦值
        const double arcAng = dp / (v1.getNorm()*v2.getNorm());
        //边界值处理，避免定义域溢出
        if (fabs(arcAng-1) < 1e-6) return 0;
        else if ((fabs(arcAng +1) < 1e-6)) return FITK_PI;
        //角度值
        return std::acos(arcAng);
    }

    FITKPoint FITKCoreAPI ProjectPointToAPlane(const FITKPoint &a, const FITKPoint &P0, const FITKVec3D &N)
    {
        // Create a normalized copy of the normal vector
        FITKVec3D normalizedN = N;
        normalizedN.normalize();
        
        // Calculate vector from P0 to point a
        FITKPoint vec_P0_to_a = Subtract(a, P0);
        
        // Calculate the distance from point a to the plane along the normal direction
        double distance = DotProduct(vec_P0_to_a, normalizedN);
        
        // Calculate the projection by subtracting the normal component
        FITKPoint projection = Subtract(a, normalizedN, 1.0, distance);
        
        return projection;
    }
}
