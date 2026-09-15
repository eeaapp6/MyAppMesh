/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    FITKAbsGeoModelExtractCenter.h
 * @brief   Extract Center Command Class
 * @date    2025-03-27
 * @author  libaojun
 *********************************************************************/

#ifndef __FITKABSGEOMODELEXTRACTCENTER_H__
#define __FITKABSGEOMODELEXTRACTCENTER_H__

#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoEnum.h"

namespace Interface
{
    /**
    * @brief 这个类的功能是提取球面或者圆弧的圆心
    * @date 2025-03-27
    * @autor libaojun
    */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelExtractCenter : public FITKAbsGeoCommand
    {
        FITKCLASS(Interface, FITKAbsGeoModelExtractCenter);
        Q_OBJECT
    public:
        /**
         * @brief 形状类型枚举
         * @date 2025-03-27
         * @author libaojun
         */
        enum ShapeType
        {
            Sphere,  ///< 球面
            Arc      ///< 圆弧
        };
        Q_ENUM(ShapeType)

    public:
        /**
         * @brief Construct a new FITKAbsGeoModelExtractCenter object
         * @date 2025-03-27
         * @author libaojun
         */
        explicit FITKAbsGeoModelExtractCenter() = default;
        virtual ~FITKAbsGeoModelExtractCenter() = 0;

        /**
         * @brief 获取几何命令类型
         * @return FITKGeoEnum::FITKGeometryComType
         * @author libaojun
         * @date 2025-03-27
         */
        virtual FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

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
         * @brief 设置圆心
         * @param center 圆心
         * @date 2025-03-27
         * @author libaojun
         */
        void setCenter(const double* center);

        /**
         * @brief 获取圆心
         * @param center 圆心
         * @date 2025-03-27
         * @author libaojun
         */
        void getCenter(double* center) const;

    protected:
       
        VirtualShape _shape; ///< 三维曲线或面
        ShapeType _shapeType{ Sphere }; ///< 形状类型

        double _center[3] = { 0,0,0 }; ///< 圆心
    };
}

#endif
