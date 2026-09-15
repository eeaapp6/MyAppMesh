/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKTriangleAlg.h"
#include "FITKVec3DAlg.h"
#include <cmath>

namespace Core
{
    double GetTriangleArea(const FITKPoint& p1, const FITKPoint& p2, const FITKPoint& p3)
    {
        //通过海伦公式获取三角形的面积
        double l1 = Distance(p1, p2);
        double l2 = Distance(p3, p2);
        double l3 = Distance(p1, p3);
        double le = (l1 + l2 + l3) / 2;
        return sqrt(le*(le - l1)*(le - l2)*(le - l3));
    }

    void GetTriEdgeLength(const FITKPoint& p1, const FITKPoint& p2, const FITKPoint& p3, double* length)
    {
        //根据两点坐标直接算出边长
        length[0] = sqrt((p1.x() - p2.x())*(p1.x() - p2.x()) + (p1.y() - p2.y())*(p1.y() - p2.y()) + (p1.z() - p2.z())*(p1.z() - p2.z()));
        length[1] = sqrt((p2.x() - p3.x())*(p2.x() - p3.x()) + (p2.y() - p3.y())*(p2.y() - p3.y()) + (p2.z() - p3.z())*(p2.z() - p3.z()));
        length[2] = sqrt((p3.x() - p1.x())*(p3.x() - p1.x()) + (p3.y() - p1.y())*(p3.y() - p1.y()) + (p3.z() - p1.z())*(p3.z() - p1.z()));
    }

    bool IsPointInsideTriangle(const FITKPoint& p1, const FITKPoint& p2, const FITKPoint& p3, const FITKPoint& piont)
    {
        //如果点在内部则，三角形上的顶点与内部点构成的三个三角形的面积和等于三顶点组成的三角形的面积
        double area = GetTriangleArea(p1, p2, p3);
        double area1 = GetTriangleArea(p1, p2, piont);
        double area2 = GetTriangleArea(p1, p3, piont);
        double area3 = GetTriangleArea(p2, p3, piont);
        return area - (area1 + area2 + area3) >= -1e-14;
    }

    double FITKCoreAPI GetTet4Volume(const FITKPoint& p1, const FITKPoint& p2, const FITKPoint& p3, const FITKPoint& tip)
    {
        //计算体积 v = (OAXOB)*OC *1/6
        Core::FITKVec3D OA = Core::Subtract(p1, tip);
        Core::FITKVec3D OB = Core::Subtract(p2, tip);
        Core::FITKVec3D OC = Core::Subtract(p3, tip);

        Core::FITKVec3D c = Core::CrossProduct(OA, OB);
        return Core::DotProduct(c, OC) / 6.0;
    }

}
