/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef  FITKABSGEOMODELCUT_H
#define  FITKABSGEOMODELCUT_H

#include "FITKInterfaceGeometryAPI.h"
#include "FITKAbsGeoCommand.h"
#include <array>

namespace Interface
{
    /**
        * @brief  实体切割抽象类.
        * @author YanZhiHui (chanyuantiandao@126.com)
        * @date   2024-08-16
        */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelCut :
        public FITKAbsGeoCommand
    {
        FITKCLASS(Interface, FITKAbsGeoModelCut);
    public:
        /**
            * @brief  构造函数.
            * @author YanZhiHui (chanyuantiandao@126.com)
            * @date   2024-08-16
            */
        FITKAbsGeoModelCut() = default;
        /**
            * @brief  析构函数.
            * @author YanZhiHui (chanyuantiandao@126.com)
            * @date   2024-08-16
            */
        virtual ~FITKAbsGeoModelCut() = default;
    };


    /**
        * @brief  拉伸切割抽象类.
        * @author YanZhiHui (chanyuantiandao@126.com)
        * @date   2024-08-16
        */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelExtrudeCut :
        public FITKAbsGeoModelCut
    {
        FITKCLASS(Interface, FITKAbsGeoModelExtrudeCut);
    public:
        /**
            * @brief  构造函数.
            * @author YanZhiHui (chanyuantiandao@126.com)
            * @date   2024-08-16
            */
        FITKAbsGeoModelExtrudeCut() = default;
        /**
            * @brief  析构函数.
            * @author YanZhiHui (chanyuantiandao@126.com)
            * @date   2024-08-16
            */
        virtual ~FITKAbsGeoModelExtrudeCut() = default;
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
        * @brief  旋转切割抽象类.
        * @author YanZhiHui (chanyuantiandao@126.com)
        * @date   2024-08-16
        */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelRevolCut :
        public FITKAbsGeoModelCut
    {
        FITKCLASS(Interface, FITKAbsGeoModelRevolCut);
    public:
        /**
            * @brief  构造函数.
            * @author YanZhiHui (chanyuantiandao@126.com)
            * @date   2024-08-16
            */
        FITKAbsGeoModelRevolCut() = default;
        /**
            * @brief  析构函数.
            * @author YanZhiHui (chanyuantiandao@126.com)
            * @date   2024-08-16
            */
        virtual ~FITKAbsGeoModelRevolCut() = default;
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
        int angle() const { return m_Angle; };
        void setAngle(int degree) { m_Angle = degree; };
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
        * @brief  扫略切割抽象类.
        * @author YanZhiHui (chanyuantiandao@126.com)
        * @date   2024-08-16
        */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelSweepCut :
        public FITKAbsGeoModelCut
    {
        FITKCLASS(Interface, FITKAbsGeoModelSweepCut);
    public:
        /**
            * @brief  构造函数.
            * @author YanZhiHui (chanyuantiandao@126.com)
            * @date   2024-08-16
            */
        FITKAbsGeoModelSweepCut() = default;
        /**
            * @brief  析构函数.
            * @author YanZhiHui (chanyuantiandao@126.com)
            * @date   2024-08-16
            */
        virtual ~FITKAbsGeoModelSweepCut() = default;
        /**
            * @brief  获取几何命令类型.
            * @return 命令类型
            * @author YanZhiHui (chanyuantiandao@126.com)
            * @date   2024-08-16
            */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        VirtualShape profile() const { return m_Profile; }
        void setProfile(VirtualShape shape) { m_Profile = shape; }
        VirtualShape curve() const { return m_Curve; }
        void setCurve(VirtualShape shape) { m_Curve = shape; }
        QList<VirtualShape> curves() const { return m_Curves; }
        void setCurves(QList<VirtualShape> shapes) { m_Curves = shapes; }

        /**
         * @brief       获取是否反转路径边。
         * @return      是否反转路径边
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-03-18
         */
        bool flipPath() const { return m_flipPath; }

        /**
         * @brief       设置是否反转路径边。
         * @param[in]   flip：是否反转路径边
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-03-18
         */
        void setFlipPath(bool flip) { m_flipPath = flip; }

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
        VirtualShape m_Curve{};
        QList<VirtualShape> m_Curves{};

        /**
         * @brief       是否反转路径边。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-03-18
         */
        bool m_flipPath = false;

    };

    /**
        * @brief  多截面扫略切割抽象类.
        * @author YanZhiHui (chanyuantiandao@126.com)
        * @date   2024-08-16
        */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelMultiSectionCut :
        public FITKAbsGeoModelCut
    {
        FITKCLASS(Interface, FITKAbsGeoModelMultiSectionCut);
    public:
        /**
            * @brief  构造函数.
            * @author YanZhiHui (chanyuantiandao@126.com)
            * @date   2024-08-16
            */
        FITKAbsGeoModelMultiSectionCut() = default;
        /**
            * @brief  析构函数.
            * @author YanZhiHui (chanyuantiandao@126.com)
            * @date   2024-08-16
            */
        virtual ~FITKAbsGeoModelMultiSectionCut() = default;
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

#endif // !FITKABSGEOMODELCUT_H
