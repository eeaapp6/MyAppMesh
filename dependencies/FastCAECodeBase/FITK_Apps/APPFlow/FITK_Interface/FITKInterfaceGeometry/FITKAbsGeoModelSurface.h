/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef  FITKABSGEOMODELSURFACE_H
#define  FITKABSGEOMODELSURFACE_H

#include "FITKInterfaceGeometryAPI.h"
#include "FITKAbsGeoCommand.h"
#include <array>

namespace Interface
{
    /**
     * @brief  曲面抽象类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-16
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelSurface :
        public FITKAbsGeoCommand
    {
        FITKCLASS(Interface, FITKAbsGeoModelSurface);
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        FITKAbsGeoModelSurface() = default;
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        virtual ~FITKAbsGeoModelSurface() = default;
    };

    /**
     * @brief  封闭曲线形成的面.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-16
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelClosedWireSurface :
        public FITKAbsGeoModelSurface
    {
        FITKCLASS(Interface, FITKAbsGeoModelClosedWireSurface);
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        FITKAbsGeoModelClosedWireSurface() = default;
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        virtual ~FITKAbsGeoModelClosedWireSurface() = default;
        /**
         * @brief  获取几何命令类型.
         * @return 命令类型
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        void addEdge(VirtualShape edge) { m_Edges.push_back(edge); };
        void setEdges(QList<Interface::VirtualShape> edges) { m_Edges = edges; };
        QList<Interface::VirtualShape> edges() const { return m_Edges; }
    protected:
        QList<Interface::VirtualShape> m_Edges{};
    };

    /**
     * @brief  偏移曲面抽象类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-16
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelOffsetSurface :
        public FITKAbsGeoModelSurface
    {
        FITKCLASS(Interface, FITKAbsGeoModelOffsetSurface);
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        FITKAbsGeoModelOffsetSurface() = default;
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        virtual ~FITKAbsGeoModelOffsetSurface() = default;
        /**
         * @brief  获取几何命令类型.
         * @return 命令类型
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        VirtualShape sourceSurface() const { return m_SourceSurface; }
        void setSourceSurface(VirtualShape shape) { m_SourceSurface = shape; }
        double offset() const { return m_Offset; };
        void setOffset(double offset) { m_Offset = offset; };
    protected:
        /**
         * @brief  源曲面虚拓扑.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-23
         */
        VirtualShape m_SourceSurface{};
        /**
         * @brief  偏移距离.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        double m_Offset{};
    };

    /**
     * @brief  拉伸曲面抽象类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-16
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelExtrudeSurface :
        public FITKAbsGeoModelSurface
    {
        FITKCLASS(Interface, FITKAbsGeoModelExtrudeSurface);
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        FITKAbsGeoModelExtrudeSurface() = default;
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        virtual ~FITKAbsGeoModelExtrudeSurface() = default;
        /**
         * @brief  获取几何命令类型.
         * @return 命令类型
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        VirtualShape sourceCurve() const { return m_SourceCurve; }
        void setSourceCurve(VirtualShape shape) { m_SourceCurve = shape; }
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
         * @brief  源曲线的虚拓扑.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        VirtualShape m_SourceCurve{};
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
     * @brief  旋转曲面抽象类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-16
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelRevolSurface :
        public FITKAbsGeoModelSurface
    {
        FITKCLASS(Interface, FITKAbsGeoModelRevolSurface);
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        FITKAbsGeoModelRevolSurface() = default;
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        virtual ~FITKAbsGeoModelRevolSurface() = default;
        /**
         * @brief  获取几何命令类型.
         * @return 命令类型
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        VirtualShape sourceCurve() const { return m_SourceCurve; }
        void setSourveCurve(VirtualShape shape) { m_SourceCurve = shape; }
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
         * @brief  源曲线的虚拓扑.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        VirtualShape m_SourceCurve{};
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
     * @brief  扫略曲面抽象类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-16
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelSweepSurface :
        public FITKAbsGeoModelSurface
    {
        FITKCLASS(Interface, FITKAbsGeoModelSweepSurface);
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        FITKAbsGeoModelSweepSurface() = default;
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        virtual ~FITKAbsGeoModelSweepSurface() = default;
        /**
         * @brief  获取几何命令类型.
         * @return 命令类型
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;


        VirtualShape profile() const { return m_Profiles.at(0); }
        void setProfile(VirtualShape shape) { m_Profiles = { shape }; }
        QList<VirtualShape> profiles() const { return m_Profiles; }
        void setProfiles(QList<VirtualShape> shapes) { m_Profiles = shapes; }
        VirtualShape curve() const { return m_Curves.at(0); }
        void setCurve(VirtualShape shape) { m_Curves = { shape }; }
        QList<VirtualShape> curves() const { return m_Curves; }
        void setCurves(QList<VirtualShape> shapes) { m_Curves = shapes; }
    protected:
        /**
         * @brief  截面曲线的虚拓扑.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        QList<VirtualShape> m_Profiles{};
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
    class FITKInterfaceGeometryAPI FITKAbsGeoModelMultiSectionSurface :
        public FITKAbsGeoModelSurface
    {
        FITKCLASS(Interface, FITKAbsGeoModelMultiSectionSurface);
    public:
        /**
         * @brief  构造函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        FITKAbsGeoModelMultiSectionSurface() = default;
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-16
         */
        virtual ~FITKAbsGeoModelMultiSectionSurface() = default;
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

    /**
     * @brief  桥接曲面.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-27
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelBridgeSurface :
        public FITKAbsGeoModelSurface
    {
        FITKCLASS(Interface, FITKAbsGeoModelBridgeSurface);
    public:
        FITKAbsGeoModelBridgeSurface() = default;
        virtual ~FITKAbsGeoModelBridgeSurface() = default;
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;
        VirtualShape sourceSurface1() const { return m_SourceSurface1; }
        void setSourceSurface1(VirtualShape surface) { m_SourceSurface1 = surface; }
        int edgeVirtualTopoId1() const { return m_EdgeVirtualTopoId1; }
        void setEdgeVirtualTopoId1(int id) { m_EdgeVirtualTopoId1 = id; }
        VirtualShape sourceSurface2() const { return m_SourceSurface2; }
        void setSourceSurface2(VirtualShape surface) { m_SourceSurface2 = surface; }
        int edgeVirtualTopoId2() const { return m_EdgeVirtualTopoId2; }
        void setEdgeVirtualTopoId2(int id) { m_EdgeVirtualTopoId2 = id; }
    protected:
        VirtualShape m_SourceSurface1{};
        int m_EdgeVirtualTopoId1{};
        VirtualShape m_SourceSurface2{};
        int m_EdgeVirtualTopoId2{};
    };


    /**
     * @brief  体的面.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-27
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelSolidSurface :
        public FITKAbsGeoModelSurface
    {
        FITKCLASS(Interface, FITKAbsGeoModelSolidSurface);
    public:
        FITKAbsGeoModelSolidSurface() = default;
        virtual ~FITKAbsGeoModelSolidSurface() = default;
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;
        VirtualShape sourceSolid() const { return m_SourceSolid; }
        void setSourceSolid(VirtualShape solid) { m_SourceSolid = solid; }
        int faceVirtualTopoId() const { return m_FaceVirtualTopoId; }
        void setFaceVirtualTopoId(int id) { m_FaceVirtualTopoId = id; }
    protected:
        VirtualShape m_SourceSolid{};
        int m_FaceVirtualTopoId{};
    };

}

#endif // !FITKABSGEOMODELSURFACE_H
