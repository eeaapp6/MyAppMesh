/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef  FITKABSGEOMODELSOLID_H
#define  FITKABSGEOMODELSOLID_H

#include "FITKInterfaceGeometryAPI.h"
#include "FITKAbsGeoCommand.h"
#include <array>

namespace Interface
{
    /**
     * @brief  实体抽象类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-16
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelSolid :
        public FITKAbsGeoCommand
    {
        FITKCLASS(Interface, FITKAbsGeoModelSolid);
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        FITKAbsGeoModelSolid() = default;
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        virtual ~FITKAbsGeoModelSolid() = default;
    };

    /**
     * @brief  封闭曲面形成的体.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-16
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelClosedSurfaceSolid :
        public FITKAbsGeoModelSolid
    {
        FITKCLASS(Interface, FITKAbsGeoModelClosedSurfaceSolid);
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        FITKAbsGeoModelClosedSurfaceSolid() = default;
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        virtual ~FITKAbsGeoModelClosedSurfaceSolid() = default;
        /**
         * @brief  获取几何命令类型.
         * @return 命令类型
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        void addFace(VirtualShape face) { m_Faces.push_back(face); };
        void setFaces(QList<Interface::VirtualShape> faces) { m_Faces = faces; };
        QList<Interface::VirtualShape> faces() const { return m_Faces; }
    protected:
        QList<Interface::VirtualShape> m_Faces{};
    };

    /**
     * @brief  拉伸实体抽象类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-16
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelExtrudeSolid :
        public FITKAbsGeoModelSolid
    {
        FITKCLASS(Interface, FITKAbsGeoModelExtrudeSolid);
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        FITKAbsGeoModelExtrudeSolid() = default;
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        virtual ~FITKAbsGeoModelExtrudeSolid() = default;
        /**
         * @brief  获取几何命令类型.
         * @return 命令类型
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        VirtualShape sourceSurface() const { return m_SourceSurface; }
        void setSourceSurface(VirtualShape shape) { m_SourceSurface = shape; }
        VirtualShape upToFace() const { return m_UpToFace; }
        void setUpToFace(VirtualShape shape) { m_UpToFace = shape; }
        std::array<double, 3> direction() const { return m_Direction; };
        void setDirection(double x, double y, double z) { m_Direction = { x, y, z }; };
        void setDirection(std::array<double, 3> xyz) { m_Direction = xyz; };
        double length() const { return m_Length; }
        void setLength(double len) { m_Length = len; }
        int extrudeType() const { return m_ExtrudeType; }
        void setExtrudeType(int type) { m_ExtrudeType = type; }
    protected:
        /**
         * @brief  源曲面的虚拓扑.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        VirtualShape m_SourceSurface{};
        /**
         * @brief  设置拉伸的终止面.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        VirtualShape m_UpToFace{};
        /**
         * @brief  拉伸方向.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        std::array<double, 3> m_Direction{};
        /**
         * @brief  拉伸长度.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        double m_Length{};
        /**
         * @brief  拉伸方式(1: 指定距离拉伸；2：拉伸到指定面).
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        int m_ExtrudeType = 1;
    };

    /**
     * @brief  旋转实体抽象类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-16
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelRevolSolid :
        public FITKAbsGeoModelSolid
    {
        FITKCLASS(Interface, FITKAbsGeoModelRevolSolid);
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        FITKAbsGeoModelRevolSolid() = default;
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        virtual ~FITKAbsGeoModelRevolSolid() = default;
        /**
         * @brief  获取几何命令类型.
         * @return 命令类型
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        VirtualShape sourceSurface() const { return m_SourceSurface; }
        void setSourceSurface(VirtualShape shape) { m_SourceSurface = shape; }
        std::array<double, 3> rotateAxisPoint1() const { return m_RotateAxisPoint1; };
        void setRotateAxisPoint1(double x, double y, double z) { m_RotateAxisPoint1 = { x, y, z }; };
        void setRotateAxisPoint1(std::array<double, 3> xyz) { m_RotateAxisPoint1 = xyz; };
        std::array<double, 3> rotateAxisPoint2() const { return m_RotateAxisPoint2; };
        void setRotateAxisPoint2(double x, double y, double z) { m_RotateAxisPoint2 = { x, y, z }; };
        void setRotateAxisPoint2(std::array<double, 3> xyz) { m_RotateAxisPoint2 = xyz; };
        double angle() const { return m_Angle; };
        void setAngle(double degree) { m_Angle = degree; };
    protected:
        /**
         * @brief  源曲面的虚拓扑.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        VirtualShape m_SourceSurface{};
        /**
         * @brief  两点定义旋转轴.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
         /*@{*/
        std::array<double, 3> m_RotateAxisPoint1{};
        std::array<double, 3> m_RotateAxisPoint2{};
        /*@}*/
        /**
         * @brief  旋转角度.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        double m_Angle{};
    };
    /**
     * @brief  扫略体抽象类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-16
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelSweepSolid :
        public FITKAbsGeoModelSolid
    {
        FITKCLASS(Interface, FITKAbsGeoModelSweepSolid);
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        FITKAbsGeoModelSweepSolid() = default;
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        virtual ~FITKAbsGeoModelSweepSolid() = default;
        /**
         * @brief  获取几何命令类型.
         * @return 命令类型
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        VirtualShape profile() const { return m_Profile; }
        void setProfile(VirtualShape shape) { m_Profile = shape; }
        VirtualShape curve() const { return m_Curves.at(0); }
        void setCurve(VirtualShape shape) { m_Curves = { shape }; }
        QList<VirtualShape> curves() const { return m_Curves; }
        void setCurves(QList<VirtualShape> shapes) { m_Curves = shapes; }
    protected:
        /**
         * @brief  截面的虚拓扑.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        VirtualShape m_Profile{};
        /**
         * @brief  脊线的虚拓扑.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        QList<VirtualShape> m_Curves{};
    };

    /**
     * @brief  多截面扫略抽象类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-16
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelMultiSectionSolid :
        public FITKAbsGeoModelSolid
    {
        FITKCLASS(Interface, FITKAbsGeoModelMultiSectionSolid);
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        FITKAbsGeoModelMultiSectionSolid() = default;
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        virtual ~FITKAbsGeoModelMultiSectionSolid() = default;
        /**
         * @brief  获取几何命令类型.
         * @return 命令类型
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        QList<VirtualShape> sections() const { return m_Sections; }
        VirtualShape section(int index) const { assert(index >= 0 && index < m_Sections.size()); return m_Sections.at(index); }
        void setSection(int index, VirtualShape section) { assert(index >= 0 && index < m_Sections.size()); m_Sections[index] = section; }
        void setSections(QList<VirtualShape> sections) { m_Sections = sections; }
        void addSection(VirtualShape section) { m_Sections.push_back(section); }

    protected:
        /**
         * @brief  截面曲线的虚拓扑.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        QList<VirtualShape> m_Sections{};
    };
}

#endif // !FITKABSGEOMODELSOLID_H
