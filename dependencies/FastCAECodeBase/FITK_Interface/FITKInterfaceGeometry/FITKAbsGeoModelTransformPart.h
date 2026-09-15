/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKAbsGeoModelTransformPart.h
 * @brief       模型变换命令抽象类。
 * 
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2026-04-13
 *********************************************************************/

#ifndef  __FITKABSMODELTRANSFORMPART_H__
#define  __FITKABSMODELTRANSFORMPART_H__

#include "FITKAbsGeoCommand.h"
#include "FITKGeoEnum.h"
#include "FITKInterfaceGeometryAPI.h"
#include <array>

namespace Interface
{
    /**
     * @brief       模型平移命令抽象类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2026-04-13
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelTranslatePart : public FITKAbsGeoCommand
    {
        FITKCLASS(Interface, FITKAbsGeoModelTranslatePart);
        Q_OBJECT
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        FITKAbsGeoModelTranslatePart() = default;

        /**
         * @brief       析构函数。[虚函数]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        virtual ~FITKAbsGeoModelTranslatePart() = default;

        /**
         * @brief       获取数据类型。[重写]
         * @return      数据类型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        /**
         * @brief       设置平移起点坐标。
         * @param[in]   x：X坐标
         * @param[in]   y：Y坐标
         * @param[in]   z：Z坐标
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        void setTranslateFrom(double x, double y, double z);

        /**
         * @brief       获取平移起点坐标。
         * @param[out]  x：X坐标
         * @param[out]  y：Y坐标
         * @param[out]  z：Z坐标
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        void getTranslateFrom(double& x, double& y, double& z) const;

        /**
         * @brief       设置平移终点坐标。
         * @param[in]   x：X坐标
         * @param[in]   y：Y坐标
         * @param[in]   z：Z坐标
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        void setTranslateTo(double x, double y, double z);

        /**
         * @brief       获取平移终点坐标。
         * @param[out]  x：X坐标
         * @param[out]  y：Y坐标
         * @param[out]  z：Z坐标
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        void getTranslateTo(double& x, double& y, double& z) const;

    protected:
        /**
         * @brief       平移起点坐标。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        double m_from[3]{ 0.0, 0.0, 0.0 };

        /**
         * @brief       平移终点坐标。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        double m_to[3]{ 0.0, 0.0, 0.0 };

    };

    /**
     * @brief       模型旋转命令抽象类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2026-04-13
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelRotatePart : public FITKAbsGeoCommand
    {
        FITKCLASS(Interface, FITKAbsGeoModelRotatePart);
        Q_OBJECT
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        FITKAbsGeoModelRotatePart() = default;

        /**
         * @brief       析构函数。[虚函数]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        virtual ~FITKAbsGeoModelRotatePart() = default;

        /**
         * @brief       获取数据类型。[重写]
         * @return      数据类型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        /**
         * @brief       设置旋转轴起点坐标。
         * @param[in]   x：X坐标
         * @param[in]   y：Y坐标
         * @param[in]   z：Z坐标
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        void setRotateOrigin(double x, double y, double z);

        /**
         * @brief       获取旋转轴起点坐标。
         * @param[out]  x：X坐标
         * @param[out]  y：Y坐标
         * @param[out]  z：Z坐标
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        void getRotateOrigin(double& x, double& y, double& z) const;

        /**
         * @brief       设置旋转轴方向向量。
         * @param[in]   x：X分量
         * @param[in]   y：Y分量
         * @param[in]   z：Z分量
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        void setRotateDirection(double x, double y, double z);

        /**
         * @brief       获取旋转轴方向向量。
         * @param[out]  x：X分量
         * @param[out]  y：Y分量
         * @param[out]  z：Z分量
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        void getRotateDirection(double& x, double& y, double& z) const;

        /**
         * @brief       设置旋转角度。
         * @param[in]   angle：旋转角度（弧度）
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        void setRotateAngle(double angle);

        /**
         * @brief       获取旋转角度。
         * @return      旋转角度（弧度）
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        double getRotateAngle() const;

    protected:
        /**
         * @brief       旋转轴起点坐标。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        double m_origin[3]{ 0.0, 0.0, 0.0 };

        /**
         * @brief       旋转轴方向向量。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        double m_direction[3]{ 0.0, 0.0, 1.0 };

        /**
         * @brief       旋转角度（弧度）。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        double m_angle{ 0.0 };

    };

    /**
     * @brief       模型缩放命令抽象类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2026-04-13
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelScalePart : public FITKAbsGeoCommand
    {
        FITKCLASS(Interface, FITKAbsGeoModelScalePart);
        Q_OBJECT
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        FITKAbsGeoModelScalePart() = default;

        /**
         * @brief       析构函数。[虚函数]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        virtual ~FITKAbsGeoModelScalePart() = default;

        /**
         * @brief       获取数据类型。[重写]
         * @return      数据类型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        /**
         * @brief       设置缩放中心坐标。
         * @param[in]   x：X坐标
         * @param[in]   y：Y坐标
         * @param[in]   z：Z坐标
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        void setScaleOrigin(double x, double y, double z);

        /**
         * @brief       获取缩放中心坐标。
         * @param[out]  x：X坐标
         * @param[out]  y：Y坐标
         * @param[out]  z：Z坐标
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        void getScaleOrigin(double& x, double& y, double& z) const;

        /**
         * @brief       设置缩放倍率。
         * @param[in]   factor：缩放倍率
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        void setScaleFactor(double factor);

        /**
         * @brief       获取缩放倍率。
         * @return      缩放倍率
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        double getScaleFactor() const;

    protected:
        /**
         * @brief       缩放中心坐标。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        double m_origin[3]{ 0.0, 0.0, 0.0 };

        /**
         * @brief       缩放倍率。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        double m_scaleFactor{ 1.0 };

    };

    /**
     * @brief       模型镜像命令抽象类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2026-04-13
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelMirrorPart : public FITKAbsGeoCommand
    {
        FITKCLASS(Interface, FITKAbsGeoModelMirrorPart);
        Q_OBJECT
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        FITKAbsGeoModelMirrorPart() = default;

        /**
         * @brief       析构函数。[虚函数]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        virtual ~FITKAbsGeoModelMirrorPart() = default;

        /**
         * @brief       获取数据类型。[重写]
         * @return      数据类型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        /**
         * @brief       设置镜像平面中心坐标。
         * @param[in]   x：X坐标
         * @param[in]   y：Y坐标
         * @param[in]   z：Z坐标
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        void setMirrorOrigin(double x, double y, double z);

        /**
         * @brief       获取镜像平面中心坐标。
         * @param[out]  x：X坐标
         * @param[out]  y：Y坐标
         * @param[out]  z：Z坐标
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        void getMirrorOrigin(double& x, double& y, double& z) const;

        /**
         * @brief       设置镜像平面位置类型。
         * @param[in]   type：平面位置类型（XY/XZ/YZ）
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        void setMirrorPlaneType(FITKGeoEnum::PlaneType type);

        /**
         * @brief       获取镜像平面位置类型。
         * @return      平面位置类型（XY/XZ/YZ）
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        FITKGeoEnum::PlaneType getMirrorPlaneType() const;

        /**
         * @brief       设置X方向旋转角度。
         * @param[in]   angle：旋转角度（度）
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        void setRotateX(double angle);

        /**
         * @brief       获取X方向旋转角度。
         * @return      旋转角度（度）
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        double getRotateX() const;

        /**
         * @brief       设置Y方向旋转角度。
         * @param[in]   angle：旋转角度（度）
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        void setRotateY(double angle);

        /**
         * @brief       获取Y方向旋转角度。
         * @return      旋转角度（度）
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        double getRotateY() const;

        /**
         * @brief       设置Z方向旋转角度。
         * @param[in]   angle：旋转角度（度）
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        void setRotateZ(double angle);

        /**
         * @brief       获取Z方向旋转角度。
         * @return      旋转角度（度）
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        double getRotateZ() const;

        /**
         * @brief       设置是否保留原模型。
         * @param[in]   keep：是否保留原模型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-07-03
         */
        void setKeepOrigin(bool keep);

        /**
         * @brief       获取是否保留原模型。
         * @return      是否保留原模型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-07-03
         */
        bool getKeepOrigin() const;

        /**
         * @brief       设置自定义平面法向量。
         * @param[in]   x：X分量
         * @param[in]   y：Y分量
         * @param[in]   z：Z分量
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-07-03
         */
        void setCustomPlaneNormal(double x, double y, double z);

        /**
         * @brief       获取自定义平面法向量。
         * @param[out]  x：X分量
         * @param[out]  y：Y分量
         * @param[out]  z：Z分量
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-07-03
         */
        void getCustomPlaneNormal(double& x, double& y, double& z) const;

    protected:
        /**
         * @brief       镜像平面中心坐标。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        double m_origin[3]{ 0.0, 0.0, 0.0 };

        /**
         * @brief       镜像平面位置类型。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        FITKGeoEnum::PlaneType m_planeType{ FITKGeoEnum::PT_XY };

        /**
         * @brief       自定义平面法向量。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-07-03
         */
        double m_customPlaneNormal[3]{ 0.0, 0.0, 1.0 };

        /**
         * @brief       X方向旋转角度（度）。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        double m_rotateX{ 0.0 };

        /**
         * @brief       Y方向旋转角度（度）。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        double m_rotateY{ 0.0 };

        /**
         * @brief       Z方向旋转角度（度）。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        double m_rotateZ{ 0.0 };

        /**
         * @brief       是否保留原模型。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-07-03
         */
        bool m_keepOrigin = false;

    };

    /**
     * @brief       模型线性阵列命令抽象类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2026-06-23
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelRectangularPatternPart : public FITKAbsGeoCommand
    {
        FITKCLASS(Interface, FITKAbsGeoModelRectangularPatternPart);
        Q_OBJECT
    public:
        /**
         * @brief       阵列定义方式。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        enum Definition
        {
            Spacing,
            Total,
            NumberAndSpacing,
        };

        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        FITKAbsGeoModelRectangularPatternPart() = default;
        /**
         * @brief       析构函数。[虚函数]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        virtual ~FITKAbsGeoModelRectangularPatternPart() = default;

        /**
         * @brief       获取数据类型。[重写]
         * @return      数据类型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        /**
         * @brief       设置第一方向定义方式。
         * @param[in]   d：定义方式
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        void setFirstDefinition(Definition d) { m_firstDefinition = d; }

        /**
         * @brief       获取第一方向定义方式。
         * @return      定义方式
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        Definition firstDefinition() const { return m_firstDefinition; }

        /**
         * @brief       设置第一方向阵列数量。
         * @param[in]   n：阵列数量
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        void setFirstCount(int n) { m_firstCount = n; }

        /**
         * @brief       获取第一方向阵列数量。
         * @return      阵列数量
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        int firstCount() const { return m_firstCount; }

        /**
         * @brief       设置第一方向阵列间距。
         * @param[in]   v：阵列间距
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        void setFirstSpacing(double v) { m_firstSpacing = v; }

        /**
         * @brief       获取第一方向阵列间距。
         * @return      阵列间距
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        double firstSpacing() const { return m_firstSpacing; }

        /**
         * @brief       设置第一方向阵列总长。
         * @param[in]   v：阵列总长
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        void setFirstTotal(double v) { m_firstTotal = v; }

        /**
         * @brief       获取第一方向阵列总长。
         * @return      阵列总长
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        double firstTotal() const { return m_firstTotal; }

        /**
         * @brief       设置第一方向向量。
         * @param[in]   x：X分量
         * @param[in]   y：Y分量
         * @param[in]   z：Z分量
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        void setFirstDirection(double x, double y, double z) { m_firstDirection = { x, y, z }; }

        /**
         * @brief       设置第一方向向量。
         * @param[in]   dir：方向向量
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        void setFirstDirection(std::array<double, 3> dir) { m_firstDirection = dir; }

        /**
         * @brief       获取第一方向向量。
         * @return      方向向量
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        std::array<double, 3> firstDirection() const { return m_firstDirection; }

        /**
         * @brief       设置第二方向定义方式。
         * @param[in]   d：定义方式
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        void setSecondDefinition(Definition d) { m_secondDefinition = d; }

        /**
         * @brief       获取第二方向定义方式。
         * @return      定义方式
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        Definition secondDefinition() const { return m_secondDefinition; }

        /**
         * @brief       设置第二方向阵列数量。
         * @param[in]   n：阵列数量
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        void setSecondCount(int n) { m_secondCount = n; }

        /**
         * @brief       获取第二方向阵列数量。
         * @return      阵列数量
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        int secondCount() const { return m_secondCount; }

        /**
         * @brief       设置第二方向阵列间距。
         * @param[in]   v：阵列间距
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        void setSecondSpacing(double v) { m_secondSpacing = v; }

        /**
         * @brief       获取第二方向阵列间距。
         * @return      阵列间距
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        double secondSpacing() const { return m_secondSpacing; }

        /**
         * @brief       设置第二方向阵列总长。
         * @param[in]   v：阵列总长
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        void setSecondTotal(double v) { m_secondTotal = v; }

        /**
         * @brief       获取第二方向阵列总长。
         * @return      阵列总长
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        double secondTotal() const { return m_secondTotal; }

        /**
         * @brief       设置第二方向向量。
         * @param[in]   x：X分量
         * @param[in]   y：Y分量
         * @param[in]   z：Z分量
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        void setSecondDirection(double x, double y, double z) { m_secondDirection = { x, y, z }; }

        /**
         * @brief       设置第二方向向量。
         * @param[in]   dir：方向向量
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        void setSecondDirection(std::array<double, 3> dir) { m_secondDirection = dir; }

        /**
         * @brief       获取第二方向向量。
         * @return      方向向量
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        std::array<double, 3> secondDirection() const { return m_secondDirection; }

    protected:
        /**
         * @brief       第一方向定义方式。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        Definition m_firstDefinition{ Spacing };

        /**
         * @brief       第一方向阵列数量。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        int m_firstCount{ 1 };

        /**
         * @brief       第一方向阵列间距。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        double m_firstSpacing{ 0.0 };

        /**
         * @brief       第一方向阵列总长。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        double m_firstTotal{ 0.0 };

        /**
         * @brief       第一方向向量。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        std::array<double, 3> m_firstDirection{ 1.0, 0.0, 0.0 };

        /**
         * @brief       第二方向定义方式。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        Definition m_secondDefinition{ Spacing };

        /**
         * @brief       第二方向阵列数量。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        int m_secondCount{ 1 };

        /**
         * @brief       第二方向阵列间距。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        double m_secondSpacing{ 0.0 };

        /**
         * @brief       第二方向阵列总长。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        double m_secondTotal{ 0.0 };

        /**
         * @brief       第二方向向量。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        std::array<double, 3> m_secondDirection{ 0.0, 1.0, 0.0 };
    };

    /**
     * @brief       模型环形阵列命令抽象类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2026-06-23
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelCircularPatternPart : public FITKAbsGeoCommand
    {
        FITKCLASS(Interface, FITKAbsGeoModelCircularPatternPart);
        Q_OBJECT
    public:
        /**
         * @brief       阵列定义方式。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        enum Definition
        {
            Spacing,
            Total,
            NumberAndSpacing,
        };

        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        FITKAbsGeoModelCircularPatternPart() = default;

        /**
         * @brief       析构函数。[虚函数]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        virtual ~FITKAbsGeoModelCircularPatternPart() = default;

        /**
         * @brief       获取数据类型。[重写]
         * @return      数据类型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        /**
         * @brief       设置轴向定义方式。
         * @param[in]   d：定义方式
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        void setFirstDefinition(Definition d) { m_firstDefinition = d; }

        /**
         * @brief       获取轴向定义方式。
         * @return      定义方式
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        Definition firstDefinition() const { return m_firstDefinition; }

        /**
         * @brief       设置轴向阵列数量。
         * @param[in]   n：阵列数量
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        void setAxisCount(int n) { m_axisCount = n; }

        /**
         * @brief       获取轴向阵列数量。
         * @return      阵列数量
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        int axisCount() const { return m_axisCount; }

        /**
         * @brief       设置轴向阵列间距。
         * @param[in]   v：阵列间距
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        void setAxisSpacing(double v) { m_axisSpacing = v; }

        /**
         * @brief       获取轴向阵列间距。
         * @return      阵列间距
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        double axisSpacing() const { return m_axisSpacing; }

        /**
         * @brief       设置轴向阵列总长。
         * @param[in]   v：阵列总长
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        void setAxisTotal(double v) { m_axisTotal = v; }

        /**
         * @brief       获取轴向阵列总长。
         * @return      阵列总长
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        double axisTotal() const { return m_axisTotal; }

        /**
         * @brief       设置阵列轴。
         * @param[in]   axis：阵列轴
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        void setAxis(Interface::VirtualShape axis) { m_axis = axis; }

        /**
         * @brief       获取阵列轴。
         * @return      阵列轴
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        Interface::VirtualShape axis() const { return m_axis; }

        /**
         * @brief       设置是否反向。
         * @param[in]   r：是否反向
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        void reverse(bool r) { m_isReverse = r; }

        /**
         * @brief       获取是否反向。
         * @return      是否反向
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        bool isReverse() const { return m_isReverse; }

        /**
         * @brief       设置环向定义方式。
         * @param[in]   d：定义方式
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        void setSecondDefinition(Definition d) { m_secondDefinition = d; }

        /**
         * @brief       获取环向定义方式。
         * @return      定义方式
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        Definition secondDefinition() const { return m_secondDefinition; }

        /**
         * @brief       设置环向阵列数量。
         * @param[in]   n：阵列数量
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        void setCrownCount(int n) { m_crownCount = n; }

        /**
         * @brief       获取环向阵列数量。
         * @return      阵列数量
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        int crownCount() const { return m_crownCount; }

        /**
         * @brief       设置环向阵列间距。
         * @param[in]   v：阵列间距
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        void setCrownSpacing(double v) { m_crownSpacing = v; }

        /**
         * @brief       获取环向阵列间距。
         * @return      阵列间距
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        double crownSpacing() const { return m_crownSpacing; }

        /**
         * @brief       设置环向阵列总长。
         * @param[in]   v：阵列总长
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        void setCrownTotal(double v) { m_crownTotal = v; }

        /**
         * @brief       获取环向阵列总长。
         * @return      阵列总长
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        double crownTotal() const { return m_crownTotal; }

    protected:
        /**
         * @brief       轴向定义方式。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        Definition m_firstDefinition{ Spacing };

        /**
         * @brief       轴向阵列数量。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        int m_axisCount{ 1 };

        /**
         * @brief       轴向阵列间距。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        double m_axisSpacing{ 0.0 };

        /**
         * @brief       轴向阵列总长。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        double m_axisTotal{ 0.0 };

        /**
         * @brief       阵列轴。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        Interface::VirtualShape m_axis{};

        /**
         * @brief       是否反向。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        bool m_isReverse{ false };

        /**
         * @brief       环向定义方式。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        Definition m_secondDefinition{ Spacing };

        /**
         * @brief       环向阵列数量。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        int m_crownCount{ 1 };

        /**
         * @brief       环向阵列间距。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        double m_crownSpacing{ 0.0 };

        /**
         * @brief       环向阵列总长。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        double m_crownTotal{ 0.0 };
    };

}

#endif // !__FITKABSMODELTRANSFORMPART_H__
