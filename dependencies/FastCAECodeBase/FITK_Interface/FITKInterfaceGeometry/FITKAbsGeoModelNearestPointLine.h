/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    FITKAbsGeoModelNearestPointLine.h
 * @brief   Nearest Point Line Command Class
 * @date    2025-03-27
 * @author  libaojun
 *********************************************************************/

#ifndef __FITKABSGEOMODELNEARESTPOINTLINE_H__
#define __FITKABSGEOMODELNEARESTPOINTLINE_H__

#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoEnum.h"

namespace Interface
{
    /**
    * @brief 这个类的功能是给定一个三维点和一个三维曲线或者面，生成这个点到曲线或面的最短距离的连线
    * @date 2025-03-27
    * @author libaojun
    */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelNearestPointLine : public FITKAbsGeoCommand
    {
        FITKCLASS(Interface, FITKAbsGeoModelNearestPointLine);
        Q_OBJECT
    public:
        /**
         * @brief 形状类型枚举
         * @date 2025-03-27
         * @author libaojun
         */
        enum ShapeType
        {
            Curve,
            Surface
        };
        Q_ENUM(ShapeType)

    public:
        /**
         * @brief Construct a new FITKAbsGeoModelNearestPointLine object
         * @date 2025-03-27
         * @author libaojun
         */
        explicit FITKAbsGeoModelNearestPointLine() = default;
        virtual ~FITKAbsGeoModelNearestPointLine() = 0;

        /**
       * @brief 获取几何命令类型
       * @return FITKGeoEnum::FITKGeometryComType
       * @author LiBaojun (libaojunqd@foxmail.com)
       * @date 2024-07-12
       */
        virtual FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;


        /**
         * @brief 设置三维点
         * @param point 三维点
         * @date 2025-03-27
         * @author libaojun
         */
        void setPoint(const double* point);

        /**
         * @brief 获取三维点
         * @return 三维点
         * @date 2025-03-27
         * @author libaojun
         */
        void getPoint(double* point) const;

        /**
         * @brief 设置三维曲线或面
         * @param shape 三维曲线或面
         * @date 2025-03-27
         * @author libaojun
         */
        void setShape(const VirtualShape& shape);

        /**
         * @brief 获取三维曲线或面
         * @return 三维曲线或面
         * @date 2025-03-27
         * @author libaojun
         */
        VirtualShape getShape() const;

        /**
         * @brief 设置形状类型
         * @param type 形状类型
         * @date 2025-03-27
         * @author libaojun
         */
        void setShapeType(ShapeType type);

        /**
         * @brief 获取形状类型
         * @return 形状类型
         * @date 2025-03-27
         * @author libaojun
         */
        ShapeType getShapeType() const;
        /**
        * @brief 设置最近点
        * @param nearestPoint 最近点
        * @date 2025-03-27
        * @author libaojun
        */
        void setNearestPoint(const double* nearestPoint);
        /**
         * @brief 获取最近点
         * @param nearestPoint 最近点
         * @date 2025-03-27
         * @author libaojun
         */
        void getNearestPoint(double* nearestPoint) const;

    protected:
        double _point[3] = { 0,0,0 }; ///< 三维点
        double _nearestPoint[3] = { 0,0,0 }; ///< 最近点
        VirtualShape _shape; ///< 三维曲线或面
        ShapeType _shapeType{ Curve }; ///< 形状类型
    };
}

#endif