/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRWall.h"

namespace Radioss
{

    FITKAbstractRWall::RWallType FITKRWallPlane::getType() const
    {
        return FITKAbstractRWall::RWallType::RWallPLANE;
    }
    void FITKRWallPlane::setBasicPoint(double x, double y, double z)
    {
        this->setMCoordinates(x, y, z);
    }

    void FITKRWallPlane::getBasicPoint(double& x, double& y, double& z) const
    {
        this->getMCoordinates(x, y, z);
    }

    void FITKRWallPlane::setNormalVector(double nx, double ny, double nz)
    {
        this->setM1Coordinates(m_coordinatesM[0]+nx,m_coordinatesM[1]+ny,m_coordinatesM[2]+ nz);
    }

    void FITKRWallPlane::getNormalVector(double& nx, double& ny, double& nz) const
    {
        nx = m_coordinatesM1[0] - m_coordinatesM[0];
        ny = m_coordinatesM1[1] - m_coordinatesM[1];
        nz = m_coordinatesM1[2] - m_coordinatesM[2];
    }

    FITKAbstractRWall::RWallType FITKRWallCylinder::getType() const
    {
        return FITKAbstractRWall::RWallType::RWallCYL;
    }
    void FITKRWallCylinder::setBasicPoint(double x, double y, double z)
    {
        this->setMCoordinates(x, y, z);
    }
    void FITKRWallCylinder::getBasicPoint(double& x, double& y, double& z) const
    {
        this->getMCoordinates(x, y, z);
    }
    void FITKRWallCylinder::setAxisVector(double nx, double ny, double nz)
    {
        this->setM1Coordinates(m_coordinatesM[0] + nx, m_coordinatesM[1] + ny, m_coordinatesM[2] + nz);
    }
    void FITKRWallCylinder::getAxisVector(double& nx, double& ny, double& nz) const
    {
        nx = m_coordinatesM1[0] - m_coordinatesM[0];
        ny = m_coordinatesM1[1] - m_coordinatesM[1];
        nz = m_coordinatesM1[2] - m_coordinatesM[2];
    }

    bool FITKRWallCylinder::calcBoxInterPoint(double * coor1, double * coor2, double * boxP1, double * boxP2)
    {
        if (!coor1 || !coor2 || !boxP1 || !boxP2)
            return false;
        //获取最小值与最大值
        float tmin = -std::numeric_limits<float>::max();
        float tmax = std::numeric_limits<float>::max();
        //获取圆柱的底面圆心和方向
        double p[3] = { 0 }, axis[3] = { 0 };
        this->getBasicPoint(p[0], p[1], p[2]);
        this->getAxisVector(axis[0], axis[1], axis[2]);
        // 处理方向向量为零的特殊情况
        if (qFuzzyIsNull(axis[0]) && qFuzzyIsNull(axis[1]) && qFuzzyIsNull(axis[2]))
            return false;
        //确保盒子的最高点与最低点
        double boxMaxP[3] = { 0 }, boxMinP[3] = { 0 };
        boxMaxP[0] = qMax(boxP1[0], boxP2[0]);
        boxMaxP[1] = qMax(boxP1[1], boxP2[1]);
        boxMaxP[2] = qMax(boxP1[2], boxP2[2]);
        boxMinP[0] = qMin(boxP1[0], boxP2[0]);
        boxMinP[1] = qMin(boxP1[1], boxP2[1]);
        boxMinP[2] = qMin(boxP1[2], boxP2[2]);
        //确保底面点一定在盒子里
        if (p[0] - boxMinP[0] < 0)
            boxMinP[0] = p[0];
        if (p[0] - boxMaxP[0] > 0)
            boxMaxP[0] = p[0];
        if (p[1] - boxMinP[1] < 0)
            boxMinP[1] = p[1];
        if (p[1] - boxMaxP[1] > 0)
            boxMaxP[1] = p[1];
        if (p[2] - boxMinP[2] < 0)
            boxMinP[2] = p[2];
        if (p[2] - boxMaxP[2] > 0)
            boxMaxP[2] = p[2];
        // 计算直线参数t的范围
        float tMin = -INFINITY;//预设范围为最小与最大值
        float tMax = INFINITY;

        // 处理X轴
        if (!qFuzzyIsNull(axis[0])) {
            const float tx1 = (boxMinP[0] - p[0]) / axis[0];
            const float tx2 = (boxMaxP[0] - p[0]) / axis[0];

            tMin = qMax(tMin, qMin(tx1, tx2));
            tMax = qMin(tMax, qMax(tx1, tx2));
        }
        // 直线平行于X轴，检查是否在X范围内
        else if (p[0] < boxMinP[0] - 1e-8 || p[0] > boxMaxP[0] + 1e-8) {
            return false;
        }

        // 处理Y轴
        if (!qFuzzyIsNull(axis[1])) {
            const float ty1 = (boxMinP[1] - p[1]) / axis[1];
            const float ty2 = (boxMaxP[1] - p[1]) / axis[1];

            tMin = qMax(tMin, qMin(ty1, ty2));
            tMax = qMin(tMax, qMax(ty1, ty2));
        }
        // 直线平行于Y轴，检查是否在Y范围内
        else if (p[1] < boxMinP[1] - 1e-8 || p[1] > boxMaxP[1] + 1e-8) {
            return false;
        }

        // 处理Z轴
        if (!qFuzzyIsNull(axis[2])) {
            const float tz1 = (boxMinP[2] - p[2]) / axis[2];
            const float tz2 = (boxMaxP[2] - p[2]) / axis[2];

            tMin = qMax(tMin, qMin(tz1, tz2));
            tMax = qMin(tMax, qMax(tz1, tz2));
        }
        // 直线平行于Z轴，检查是否在Z范围内
        else if (p[2] < boxMinP[2] - 1e-8 || p[2] > boxMaxP[2] + 1e-8) {
            return false;
        }

        // 检查是否存在有效交点
        if (tMin > tMax) {
            return false; // 无交点
        }

        // 计算交点
        auto computePoint = [&](float t, double* point) {
            if (point == nullptr) return false;
            point[0] = p[0] + t * axis[0];
            point[1] = p[1] + t * axis[1];
            point[2] = p[2] + t * axis[2];
            return true;
        };

        // 添加进入点和离开点
        computePoint(tMin, coor1);
        computePoint(tMax, coor2);

        //// 检查点是否在包围盒内（考虑浮点误差）
        //auto isPointInBox = [&](const QVector3D& p) {
        //    return (p[0] >= minCorner[0] - 1e-8 && p[0] <= maxCorner[0] + 1e-8 &&
        //        p[1] >= minCorner[1] - 1e-8 && p[1] <= maxCorner[1] + 1e-8 &&
        //        p[2] >= minCorner[2] - 1e-8 && p[2] <= maxCorner[2] + 1e-8);
        //};
        
        //if (isPointInBox(point1)) {
        //    intersections.append(point1);
        //}
        
        //if (isPointInBox(point2) &&
        //    (!intersections.isEmpty() || !qFuzzyCompare(tMin, tMax))) {
        //    // 避免重复点
        //    if (intersections.isEmpty() ||
        //        !qFuzzyCompare(point1[0], point2[0]) ||
        //        !qFuzzyCompare(point1[1], point2[1]) ||
        //        !qFuzzyCompare(point1[2], point2[2])) {
        //        intersections.append(point2);
        //    }
        //}
        
        //// 按参数t排序，确保顺序正确
        //if (intersections.size() == 2 && tMin > tMax) {
        //    std::swap(intersections[0], intersections[1]);
        //}
        return true;
    }

    FITKAbstractRWall::RWallType FITKRWallSpherical::getType() const
    {
        return FITKAbstractRWall::RWallType::RWallSPHER;
    }

    void FITKRWallSpherical::setCenter(double x, double y, double z)
    {
        this->setMCoordinates(x, y, z);
    }

    void FITKRWallSpherical::getCenter(double& x, double& y, double& z) const
    {
        this->getMCoordinates(x, y, z);
    }

    void FITKRWallSpherical::setRadius(double r)
    {
        this->setDiameter(r * 2.0);
    }

    double FITKRWallSpherical::getRadius() const
    {
        return this->getDiameter() / 2.0;
    }

}
