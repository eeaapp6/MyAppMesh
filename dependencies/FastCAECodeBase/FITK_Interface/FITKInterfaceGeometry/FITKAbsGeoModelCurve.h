/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef  FITKABSGEOMODELCURVE_H
#define  FITKABSGEOMODELCURVE_H

#include "FITKInterfaceGeometryAPI.h"
#include "FITKAbsGeoCommand.h"
#include <array>

namespace Interface
{
    /**
     * @brief  曲线抽象类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-13
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelCurve :
        public FITKAbsGeoCommand
    {
        FITKCLASS(Interface, FITKAbsGeoModelCurve);
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        FITKAbsGeoModelCurve() = default;
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        virtual ~FITKAbsGeoModelCurve() = default;

        /* 曲线的局部坐标系数据接口 */
        /* @{ */
        std::array<double, 3> origin() { return m_Origin; }
        void setOrigin(double x, double y, double z) {
            m_Origin[0] = x;
            m_Origin[1] = y;
            m_Origin[2] = z;
        };
        void setOrigin(std::array<double, 3> xyz) {
            m_Origin = xyz;
        };
        std::array<double, 3> xDirection() { return m_XDirection; }
        std::array<double, 3> yDirection() { return m_YDirection; }
        void setYDirection(double x, double y, double z) {
            setYDirection({ x, y,z });
        };
        void setYDirection(std::array<double, 3> xyz) {
            m_YDirection = xyz;
            m_XDirection = {
                m_YDirection[1] * m_ZDirection[2] - m_ZDirection[1] * m_YDirection[2],
             m_ZDirection[0] * m_YDirection[2] - m_YDirection[0] * m_ZDirection[2],
             m_YDirection[0] * m_ZDirection[1] - m_ZDirection[0] * m_YDirection[1] };
        };
        std::array<double, 3> zDirection() { return m_ZDirection; }
        void setZDirection(double x, double y, double z) {
            setZDirection({ x,y,z });
        };
        void setZDirection(std::array<double, 3> xyz) {
            m_ZDirection = xyz;

            m_XDirection = {
                m_YDirection[1] * m_ZDirection[2] - m_ZDirection[1] * m_YDirection[2],
             m_ZDirection[0] * m_YDirection[2] - m_YDirection[0] * m_ZDirection[2],
             m_YDirection[0] * m_ZDirection[1] - m_ZDirection[0] * m_YDirection[1] };
        };
        /* @} */
    protected:
        /* 曲线的局部坐标系（非草绘坐标系） */
        /* @{ */
        std::array<double, 3> m_Origin{ { 0, 0, 0 } };
        std::array<double, 3> m_XDirection{ { 1, 0, 0 } };
        std::array<double, 3> m_YDirection{ { 0, 1, 0 } };
        std::array<double, 3> m_ZDirection{ { 0, 0, 1 } };
        /* @} */
    };

    /**
     * @brief  无限直线抽象类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-13
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelLine : public FITKAbsGeoModelCurve
    {
        FITKCLASS(Interface, FITKAbsGeoModelLine);
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        FITKAbsGeoModelLine() = default;
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        ~FITKAbsGeoModelLine() override = default;
        /**
         * @brief  获取正方向单位向量.
         * @return 正方向单位向量数组
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        std::array<double, 3> dir() const;
        /**
         * @brief  设置正方向单位向量.
         * @param  x 正方向单位向量的x值
         * @param  y 正方向单位向量的y值
         * @param  z 正方向单位向量的z值
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        void setDir(double x, double y, double z);
        /**
         * @brief  获取命令类型.
         * @return 命令类型
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    protected:
        /**
         * @brief  正方向.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        std::array<double, 3> m_Dir{};
    };
    /**
     * @brief  有限线段抽象类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-13
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelSegment : public FITKAbsGeoModelCurve
    {
        FITKCLASS(Interface, FITKAbsGeoModelSegment);
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        FITKAbsGeoModelSegment() = default;
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        ~FITKAbsGeoModelSegment() override = default;
        /**
         * @brief  获取起点.
         * @return 起点坐标数组
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        std::array<double, 3> startPoint() const;
        /**
         * @brief  设置起点.
         * @param  x 起点的x坐标值
         * @param  y 起点的y坐标值
         * @param  z 起点的z坐标值
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        void setStartPoint(double x, double y, double z);
        void setStartPoint(std::array<double, 3> xyz) { m_StartPoint = xyz; };
        /**
         * @brief  获取终点.
         * @return 终点坐标数组
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        std::array<double, 3> endPoint() const;
        /**
         * @brief  设置终点.
         * @param  x 终点的x坐标值
         * @param  y 终点的y坐标值
         * @param  z 终点的z坐标值
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        void setEndPoint(double x, double y, double z);
        void setEndPoint(std::array<double, 3> xyz) { m_EndPoint = xyz; };
        /**
        * @brief  获取命令类型.
        * @return 命令类型
        * @author YanZhiHui (chanyuantiandao@126.com)
        * @date   2024-08-13
        */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    protected:
        /**
         * @brief  起点.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        std::array<double, 3> m_StartPoint{};
        /**
         * @brief  终点.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        std::array<double, 3> m_EndPoint{};
    };
    /**
     * @brief  多段线抽象类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-13
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelWire : public FITKAbsGeoModelCurve
    {
        FITKCLASS(Interface, FITKAbsGeoModelWire);
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        FITKAbsGeoModelWire() = default;
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        ~FITKAbsGeoModelWire() override = default;
        /**
         * @brief  添加点.
         * @param  x x坐标值
         * @param  y y坐标值
         * @param  z z坐标值
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        void addPoint(double x, double y, double z);

        /**
         * @brief       清除所有点。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-07
         */
        void clearPoints();

        /**
         * @brief  返回点数量.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        int count() const;
        /**
         * @brief  返回第index个点的坐标数组.
         * @param  index 索引
         * @return 坐标数组
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        std::array<double, 3> point(int index) const;
        /**
        * @brief  获取命令类型.
        * @return 命令类型
        * @author YanZhiHui (chanyuantiandao@126.com)
        * @date   2024-08-13
        */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    protected:
        /**
         * @brief  多段线的点.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        QVector<std::array<double, 3>> m_Points{};
    };
    /**
     * @brief  圆锥曲线.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-13
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelConic : public FITKAbsGeoModelCurve
    {
        FITKCLASS(Interface, FITKAbsGeoModelConic);
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        FITKAbsGeoModelConic() = default;
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        ~FITKAbsGeoModelConic() override = default;
    };
    /**
     * @brief  圆.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-13
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelCircle : public FITKAbsGeoModelConic
    {
        FITKCLASS(Interface, FITKAbsGeoModelCircle);
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        FITKAbsGeoModelCircle() = default;
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        ~FITKAbsGeoModelCircle() override = default;
        /**
         * @brief  获取半径.
         * @return 半径
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        double radius() const;
        /**
         * @brief  设置半径.
         * @param  radius 半径
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        void setRadius(double radius);
        /**
        * @brief  获取命令类型.
        * @return 命令类型
        * @author YanZhiHui (chanyuantiandao@126.com)
        * @date   2024-08-13
        */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    protected:
        /**
         * @brief  半径.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        double m_Radius{};
    };
    /**
     * @brief  圆弧.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-13
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelCircleArc : public FITKAbsGeoModelCircle
    {
        FITKCLASS(Interface, FITKAbsGeoModelCircleArc);
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        FITKAbsGeoModelCircleArc() = default;
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        ~FITKAbsGeoModelCircleArc() override = default;
        /**
        * @brief  获取命令类型.
        * @return 命令类型
        * @author YanZhiHui (chanyuantiandao@126.com)
        * @date   2024-08-13
        */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;
        std::array<double, 3> startPoint() const { return m_StartPoint; }
        void setStartPoint(std::array<double, 3> xyz) { m_StartPoint = xyz; }
        void setStartPoint(double x, double y, double z) { m_StartPoint = { x,y,z }; }
        std::array<double, 3> endPoint() const { return m_EndPoint; }
        void setEndPoint(std::array<double, 3> xyz) { m_EndPoint = xyz; }
        void setEndPoint(double x, double y, double z) { m_EndPoint = { x,y,z }; }
    protected:
        /**
         * @brief  起始点.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-05
         */
        std::array<double, 3> m_StartPoint{};
        /**
         * @brief  终止点.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-05
         */
        std::array<double, 3> m_EndPoint{};

    };
    /**
     * @brief  三点圆.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-13
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelThreePointsCircle : public FITKAbsGeoModelCircle
    {
        FITKCLASS(Interface, FITKAbsGeoModelThreePointsCircle);
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        FITKAbsGeoModelThreePointsCircle() = default;
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        ~FITKAbsGeoModelThreePointsCircle() override = default;
        /**
        * @brief  获取命令类型.
        * @return 命令类型
        * @author YanZhiHui (chanyuantiandao@126.com)
        * @date   2024-08-13
        */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        std::array<double, 3> point(int index) const { assert(index >= 0 && index < 3); return m_Points.at(index); };
        void setPoint(int index, double x, double y, double z) { assert(index >= 0 && index < 3); m_Points[index] = { x, y,z }; };
        void setPoint(int index, std::array<double, 3> xyz) { assert(index >= 0 && index < 3); m_Points[index] = xyz; };
    protected:
        QVector<std::array<double, 3>> m_Points{ 3 };
    };
    /**
     * @brief  椭圆.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-13
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelEllipse : public FITKAbsGeoModelConic
    {
        FITKCLASS(Interface, FITKAbsGeoModelEllipse);
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        FITKAbsGeoModelEllipse() = default;
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        ~FITKAbsGeoModelEllipse() override = default;
        /**
         * @brief  获取长半径.
         * @return 长半径
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        double majorRadius() const;
        /**
         * @brief  设置长半径.
         * @param  radius 长半径
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        void setMajorRadius(double radius);
        /**
         * @brief  获取短半径.
         * @return 短半径
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        double minorRadius() const;
        /**
         * @brief  设置短半径.
         * @param  radius 短半径
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        void setMinorRadius(double radius);
        /**
        * @brief  获取命令类型.
        * @return 命令类型
        * @author YanZhiHui (chanyuantiandao@126.com)
        * @date   2024-08-13
        */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    protected:
        /**
         * @brief  长半径.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        double m_MajorRadius{};
        /**
         * @brief  短半径.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        double m_MinorRadius{};
    };
    /**
     * @brief  椭圆弧.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-13
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelEllipseArc : public FITKAbsGeoModelEllipse
    {
        FITKCLASS(Interface, FITKAbsGeoModelEllipseArc);
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        FITKAbsGeoModelEllipseArc() = default;
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        ~FITKAbsGeoModelEllipseArc() override = default;
        /**
        * @brief  获取命令类型.
        * @return 命令类型
        * @author YanZhiHui (chanyuantiandao@126.com)
        * @date   2024-08-13
        */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;
        std::array<double, 3> startPoint() const { return m_StartPoint; }
        void setStartPoint(std::array<double, 3> xyz) { m_StartPoint = xyz; }
        void setStartPoint(double x, double y, double z) { m_StartPoint = { x,y,z }; }
        std::array<double, 3> endPoint() const { return m_EndPoint; }
        void setEndPoint(std::array<double, 3> xyz) { m_EndPoint = xyz; }
        void setEndPoint(double x, double y, double z) { m_EndPoint = { x,y,z }; }
    protected:
        /**
         * @brief  起始点.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-05
         */
        std::array<double, 3> m_StartPoint{};
        /**
         * @brief  终止点.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-05
         */
        std::array<double, 3> m_EndPoint{};
    };
    /**
     * @brief  三点椭圆.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-13
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelThreePointsEllipse : public FITKAbsGeoModelConic
    {
        FITKCLASS(Interface, FITKAbsGeoModelThreePointsEllipse);
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        FITKAbsGeoModelThreePointsEllipse() = default;
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        ~FITKAbsGeoModelThreePointsEllipse() override = default;
        /**
        * @brief  获取命令类型.
        * @return 命令类型
        * @author YanZhiHui (chanyuantiandao@126.com)
        * @date   2024-08-13
        */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        std::array<double, 3> majorPoint() const { return m_MajorPoint; };
        void setMajorPoint(double x, double y, double z) { m_MajorPoint = { x, y,z }; };
        void setMajorPoint(std::array<double, 3> xyz) { m_MajorPoint = xyz; };

        std::array<double, 3> minorPoint() const { return m_MinorPoint; };
        void setMinorPoint(double x, double y, double z) { m_MinorPoint = { x, y,z }; };
        void setMinorPoint(std::array<double, 3> xyz) { m_MinorPoint = xyz; };
    protected:
        /**
         * @brief  长轴点.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-28
         */
        std::array<double, 3> m_MajorPoint{};
        /**
         * @brief  短轴点.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-28
         */
        std::array<double, 3> m_MinorPoint{};

    };
    /**
     * @brief  双曲线.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-13
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelHyperbola : public FITKAbsGeoModelConic
    {
        FITKCLASS(Interface, FITKAbsGeoModelHyperbola);
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        FITKAbsGeoModelHyperbola() = default;
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        ~FITKAbsGeoModelHyperbola() override = default;
        /**
         * @brief  获取长半径.
         * @return 长半径
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        double majorRadius() const;
        /**
         * @brief  设置长半径.
         * @param  radius 长半径
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        void setMajorRadius(double radius);
        /**
         * @brief  获取短半径.
         * @return 短半径
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        double minorRadius() const;
        /**
         * @brief  设置短半径.
         * @param  radius 短半径
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        void setMinorRadius(double radius);
        /**
        * @brief  获取命令类型.
        * @return 命令类型
        * @author YanZhiHui (chanyuantiandao@126.com)
        * @date   2024-08-13
        */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    protected:
        /**
         * @brief  长半径.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        double m_MajorRadius{};
        /**
         * @brief  短半径.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        double m_MinorRadius{};
    };
    /**
     * @brief  三点双曲线.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-13
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelThreePointsHyperbola : public FITKAbsGeoModelHyperbola
    {
        FITKCLASS(Interface, FITKAbsGeoModelThreePointsHyperbola);
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        FITKAbsGeoModelThreePointsHyperbola() = default;
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        ~FITKAbsGeoModelThreePointsHyperbola() override = default;
        /**
        * @brief  获取命令类型.
        * @return 命令类型
        * @author YanZhiHui (chanyuantiandao@126.com)
        * @date   2024-08-13
        */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        std::array<double, 3> majorPoint() const { return m_MajorPoint; };
        void setMajorPoint(double x, double y, double z) { m_MajorPoint = { x, y,z }; };
        void setMajorPoint(std::array<double, 3> xyz) { m_MajorPoint = xyz; };

        std::array<double, 3> minorPoint() const { return m_MinorPoint; };
        void setMinorPoint(double x, double y, double z) { m_MinorPoint = { x, y,z }; };
        void setMinorPoint(std::array<double, 3> xyz) { m_MinorPoint = xyz; };
    protected:
        /**
         * @brief  长轴点.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-28
         */
        std::array<double, 3> m_MajorPoint{};
        /**
         * @brief  短轴点.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-28
         */
        std::array<double, 3> m_MinorPoint{};
    };
    /**
     * @brief  双曲线弧.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-13
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelHyperbolaArc : public FITKAbsGeoModelHyperbola
    {
        FITKCLASS(Interface, FITKAbsGeoModelHyperbolaArc);
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        FITKAbsGeoModelHyperbolaArc() = default;
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        ~FITKAbsGeoModelHyperbolaArc() override = default;
        /**
        * @brief  获取命令类型.
        * @return 命令类型
        * @author YanZhiHui (chanyuantiandao@126.com)
        * @date   2024-08-13
        */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;
        std::array<double, 3> startPoint() const { return m_StartPoint; }
        void setStartPoint(std::array<double, 3> xyz) { m_StartPoint = xyz; }
        void setStartPoint(double x, double y, double z) { m_StartPoint = { x,y,z }; }
        std::array<double, 3> endPoint() const { return m_EndPoint; }
        void setEndPoint(std::array<double, 3> xyz) { m_EndPoint = xyz; }
        void setEndPoint(double x, double y, double z) { m_EndPoint = { x,y,z }; }
    protected:
        /**
         * @brief  起始点.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-05
         */
        std::array<double, 3> m_StartPoint{};
        /**
         * @brief  终止点.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-05
         */
        std::array<double, 3> m_EndPoint{};
    };
    /**
     * @brief  抛物线.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-13
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelParabola : public FITKAbsGeoModelConic
    {
        FITKCLASS(Interface, FITKAbsGeoModelParabola);
    public:
        /**
         * @brief  构造函数.
         * @param  origin 原点
         * @param  xDir x轴正方向
         * @param  yDir y轴正方向
         * @param  zDir z轴正方向
         * @param  focalLength 焦距
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        FITKAbsGeoModelParabola() = default;
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        ~FITKAbsGeoModelParabola() override = default;
        /**
         * @brief  获取焦距.
         * @return 焦距
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        double focalLength() const;
        /**
         * @brief  设置焦距.
         * @param  length  焦距
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        void setFocalLength(double length);
        /**
        * @brief  获取命令类型.
        * @return 命令类型
        * @author YanZhiHui (chanyuantiandao@126.com)
        * @date   2024-08-13
        */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    protected:
        /**
         * @brief  焦距.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        double m_FocalLength{};
    };
    /**
     * @brief  三点双曲线.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-13
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelThreePointsParabola : public FITKAbsGeoModelParabola
    {
        FITKCLASS(Interface, FITKAbsGeoModelThreePointsParabola);
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        FITKAbsGeoModelThreePointsParabola() = default;
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        ~FITKAbsGeoModelThreePointsParabola() override = default;
        /**
        * @brief  获取命令类型.
        * @return 命令类型
        * @author YanZhiHui (chanyuantiandao@126.com)
        * @date   2024-08-13
        */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        std::array<double, 3> point(int index) const { assert(index >= 0 && index < 3); return m_Points.at(index); };
        void setPoint(int index, double x, double y, double z) { assert(index >= 0 && index < 3); m_Points[index] = { x, y,z }; };
        void setPoint(int index, std::array<double, 3> xyz) { assert(index >= 0 && index < 3); m_Points[index] = xyz; };
    protected:
        QVector<std::array<double, 3>> m_Points{ 3 };
    };
    /**
     * @brief  抛物线弧.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-13
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelParabolaArc : public FITKAbsGeoModelParabola
    {
        FITKCLASS(Interface, FITKAbsGeoModelParabolaArc);
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        FITKAbsGeoModelParabolaArc() = default;
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        ~FITKAbsGeoModelParabolaArc() override = default;
        /**
        * @brief  获取命令类型.
        * @return 命令类型
        * @author YanZhiHui (chanyuantiandao@126.com)
        * @date   2024-08-13
        */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;
        std::array<double, 3> startPoint() const { return m_StartPoint; }
        void setStartPoint(std::array<double, 3> xyz) { m_StartPoint = xyz; }
        void setStartPoint(double x, double y, double z) { m_StartPoint = { x,y,z }; }
        std::array<double, 3> endPoint() const { return m_EndPoint; }
        void setEndPoint(std::array<double, 3> xyz) { m_EndPoint = xyz; }
        void setEndPoint(double x, double y, double z) { m_EndPoint = { x,y,z }; }
    protected:
        /**
         * @brief  起始点.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-05
         */
        std::array<double, 3> m_StartPoint{};
        /**
         * @brief  终止点.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-05
         */
        std::array<double, 3> m_EndPoint{};
    };
    /**
     * @brief  通过控制点创建贝塞尔曲线.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-09-07
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelBeizeByControlPoints : public FITKAbsGeoModelCurve {
        FITKCLASS(Interface, FITKAbsGeoModelBeizeByControlPoints);
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        FITKAbsGeoModelBeizeByControlPoints() = default;
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        ~FITKAbsGeoModelBeizeByControlPoints() override = default;
        /**
        * @brief  获取命令类型.
        * @return 命令类型
        * @author YanZhiHui (chanyuantiandao@126.com)
        * @date   2024-08-13
        */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        QList<std::array<double, 3>> controlPoints() const { return m_ControlPoints; }
        std::array<double, 3> controlPoint(int index) const { assert(index >= 0 && index < m_ControlPoints.size()); return m_ControlPoints.at(index); }
        void addControlPoint(std::array<double, 3> point) { m_ControlPoints.push_back(point); }
        void addControlPoint(double x, double y, double z) { m_ControlPoints.push_back({ x,y,z }); }
        void setControlPoint(int index, std::array<double, 3> point) { assert(index >= 0 && index < m_ControlPoints.size()); m_ControlPoints[index] = point; }
        void setControlPoint(int index, double x, double y, double z) { assert(index >= 0 && index < m_ControlPoints.size());  m_ControlPoints[index] = { x,y,z }; }
    protected:
        QList<std::array<double, 3>> m_ControlPoints{};
    };
    /**
     * @brief  通过线上的点创建B样条曲线.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-09-07
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelBsplineByThroughPoints : public FITKAbsGeoModelCurve {
        FITKCLASS(Interface, FITKAbsGeoModelBsplineByThroughPoints);
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        FITKAbsGeoModelBsplineByThroughPoints() = default;
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        ~FITKAbsGeoModelBsplineByThroughPoints() override = default;
        /**
        * @brief  获取命令类型.
        * @return 命令类型
        * @author YanZhiHui (chanyuantiandao@126.com)
        * @date   2024-08-13
        */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        QList<std::array<double, 3>> throughPoints() const { return m_ThroughPoints; }
        std::array<double, 3> throughPoints(int index) const { assert(index >= 0 && index < m_ThroughPoints.size()); return m_ThroughPoints.at(index); }
        void addThroughPoint(std::array<double, 3> point) { m_ThroughPoints.push_back(point); }
        void addThroughPoint(double x, double y, double z) { m_ThroughPoints.push_back({ x,y,z }); }
        void setThroughPoint(int index, std::array<double, 3> point) { assert(index >= 0 && index < m_ThroughPoints.size()); m_ThroughPoints[index] = point; }
        void setThroughPoint(int index, double x, double y, double z) { assert(index >= 0 && index < m_ThroughPoints.size());  m_ThroughPoints[index] = { x,y,z }; }
        void setThroughPoints(QList<std::array<double, 3>> points) { m_ThroughPoints = points; }

        void setThroughPointShapes(QList<Interface::VirtualShape> points) { m_ThroughPointShapes = points; }
        QList<Interface::VirtualShape> throughPointShapes() const { return m_ThroughPointShapes; }

    protected:
        QList<std::array<double, 3>> m_ThroughPoints{};
        QList<Interface::VirtualShape> m_ThroughPointShapes{};

    };
    /**
     * @brief  裁剪曲线抽象类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-13
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelTrimmedCurve : public FITKAbsGeoModelCurve
    {
        FITKCLASS(Interface, FITKAbsGeoModelTrimmedCurve);
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        FITKAbsGeoModelTrimmedCurve() = default;
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        ~FITKAbsGeoModelTrimmedCurve() override = default;
        /**
         * @brief  获取源曲线id.
         * @return 源曲线id
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        int sourceCurveId() const;
        /**
         * @brief  设置源曲线id.
         * @param  id 源曲线id
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        void setSourceCurveId(int id);
        /**
         * @brief  获取起始参数.
         * @return 起始参数
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        double startTrimmed() const;
        /**
         * @brief  设置起始参数.
         * @param  val 起始参数
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        void setStartTrimmed(double val);
        /**
         * @brief  获取终止参数.
         * @return 终止参数
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        double endTrimmed() const;
        /**
         * @brief  设置终止参数.
         * @param  val 终止参数
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        void setEndTrimmed(double val);
        /**
        * @brief  获取命令类型.
        * @return 命令类型
        * @author YanZhiHui (chanyuantiandao@126.com)
        * @date   2024-08-13
        */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;
    protected:
        /**
         * @brief  源曲线Id.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        int m_SourceCurveId{};
        /**
         * @brief  起始参数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        double m_StartTrimmed{};
        /**
         * @brief  终止参数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        double m_EndTrimmed{};
    };

    /**
    * @brief  偏移曲线.
    * @author YanZhiHui (chanyuantiandao@126.com)
    * @date   2024-08-13
    */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelOffsetCurve : public FITKAbsGeoModelCurve
    {
        FITKCLASS(Interface, FITKAbsGeoModelOffsetCurve);
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        FITKAbsGeoModelOffsetCurve() = default;
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-13
         */
        ~FITKAbsGeoModelOffsetCurve() override = default;
        /**
        * @brief  获取命令类型.
        * @return 命令类型
        * @author YanZhiHui (chanyuantiandao@126.com)
        * @date   2024-08-13
        */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        VirtualShape sourceCurve() const { return m_SourceCurve; }
        void setSourceCurve(VirtualShape curve) { m_SourceCurve = curve; }
        std::array<double, 3> dir() const { return m_Dir; }
        void setDir(double x, double y, double z) { m_Dir = { x,y,z }; }
        void setDir(std::array<double, 3> dir) { m_Dir = dir; }
        double offset() const { return m_Offset; }
        void setOffset(double offset) { m_Offset = offset; }

    protected:
        VirtualShape m_SourceCurve{};
        std::array<double, 3> m_Dir{};
        double m_Offset{};
    };

    /**
     * @brief  线在面上的投影.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-27
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelCurveProjectionOnSurface :
        public FITKAbsGeoModelCurve
    {
        FITKCLASS(Interface, FITKAbsGeoModelCurveProjectionOnSurface);
    public:
        FITKAbsGeoModelCurveProjectionOnSurface() = default;
        virtual ~FITKAbsGeoModelCurveProjectionOnSurface() = default;
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;
        VirtualShape sourceCurve() const;
        void setSourceCurve(VirtualShape curve);
        VirtualShape sourceSurface() const;
        void setSourceSurface(VirtualShape surface);
    protected:
        VirtualShape m_SourceCurve{};
        VirtualShape m_SourceSurface{};
    };
    /**
     * @brief  面和面的交线.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-27
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelSurfaceIntersectionSurface :
        public FITKAbsGeoModelCurve
    {
        FITKCLASS(Interface, FITKAbsGeoModelSurfaceIntersectionSurface);
    public:
        FITKAbsGeoModelSurfaceIntersectionSurface() = default;
        virtual ~FITKAbsGeoModelSurfaceIntersectionSurface() = default;
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;
        VirtualShape sourceSurface1() const;
        void setSourceSurface1(VirtualShape surface);
        VirtualShape sourceSurface2() const;
        void setSourceSurface2(VirtualShape surface);
    protected:
        VirtualShape m_SourceSurface1{};
        VirtualShape m_SourceSurface2{};
        int m_UseResultIndex{ 0 };
    };

    /**
     * @brief  桥接曲线.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-27
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelBridgeCurve :
        public FITKAbsGeoModelCurve
    {
        FITKCLASS(Interface, FITKAbsGeoModelBridgeCurve);
    public:
        FITKAbsGeoModelBridgeCurve() = default;
        virtual ~FITKAbsGeoModelBridgeCurve() = default;
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;
        VirtualShape sourceCurve1() const;
        void setSourceCurve1(VirtualShape curve);
        bool useStartEnd1() const;
        void setUseStartEnd1(bool use);
        double parameter1() const;
        void setParameter1(double p);
        VirtualShape sourceCurve2() const;
        void setSourceCurve2(VirtualShape curve);
        bool useStartEnd2() const;
        void setUseStartEnd2(bool use);
        double parameter2() const;
        void setParameter2(double p);
    protected:
        VirtualShape m_SourceCurve1{};
        bool m_UseStartEnd1{ true };
        double m_Parameter1{ 1.0 };
        VirtualShape m_SourceCurve2{};
        bool m_UseStartEnd2{ true };
        double m_Parameter2{ 1.0 };
    };


    /**
     * @brief  面的边线.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-27
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelSurfaceEdge :
        public FITKAbsGeoModelCurve
    {
        FITKCLASS(Interface, FITKAbsGeoModelSurfaceEdge);
    public:
        FITKAbsGeoModelSurfaceEdge() = default;
        virtual ~FITKAbsGeoModelSurfaceEdge() = default;
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;
        VirtualShape sourceSurface() const;
        void setSourceSurface(VirtualShape surface);
        int edgeVirtualTopoId() const;
        void setEdgeVirtualTopoId(int id);
    protected:
        VirtualShape m_SourceSurface{};
        int m_EdgeVirtualTopoId{};
    };
}

#endif // !FITKABSGEOMODELCURVE_H
