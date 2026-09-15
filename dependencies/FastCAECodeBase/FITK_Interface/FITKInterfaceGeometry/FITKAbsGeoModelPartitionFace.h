/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   FITKAbsGeoModelPartitionFace.h
 * @brief  面分块
 * @author YanZhiHui (chanyuantiandao@126.com)
 * @date   2025-04-23
 */
#ifndef  FITKABSGEOMODELPARTITIONFACE_H
#define  FITKABSGEOMODELPARTITIONFACE_H

#include "FITKInterfaceGeometryAPI.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"
#include <QVector>
#include <array>

namespace Interface
{
    class FITKAbsGeoDatumPlane;
    /**
     * @brief  面分块.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2025-04-23
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelPartitionFace :
        public FITKAbsGeoCommand
    {
        FITKCLASS(Interface, FITKAbsGeoModelPartitionFace);
    public:
        FITKAbsGeoModelPartitionFace() = default;
        ~FITKAbsGeoModelPartitionFace() override = default;

        void setFaces(QList<Interface::VirtualShape> faces) { m_Faces = faces; }
        QList<Interface::VirtualShape> faces() const { return m_Faces; }

    protected:
        QList<Interface::VirtualShape> m_Faces{};
    };

    /**
     * @brief  使用草图进行面分块.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2025-04-23
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelPartitionFaceWithSketch :
        public FITKAbsGeoModelPartitionFace
    {
        FITKCLASS(Interface, FITKAbsGeoModelPartitionFaceWithSketch);
    public:
        FITKAbsGeoModelPartitionFaceWithSketch() = default;
        ~FITKAbsGeoModelPartitionFaceWithSketch() override = default;
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        void setSketch(int id) { m_SketchId = id; }
        int sketch() const { return m_SketchId; }

    protected:
        int m_SketchId{};
    };

    /**
     * @brief  使用两点进行面分块.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2025-04-23
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelPartitionFaceWithTwoPoints :
        public FITKAbsGeoModelPartitionFace
    {
        FITKCLASS(Interface, FITKAbsGeoModelPartitionFaceWithTwoPoints);
    public:
        FITKAbsGeoModelPartitionFaceWithTwoPoints() = default;
        ~FITKAbsGeoModelPartitionFaceWithTwoPoints() override = default;
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        void setStartPoint(QVector<double> point) { m_StartPoint = point; }
        QVector<double> startPoint() const { return m_StartPoint; }
        void setEndPoint(QVector<double> point) { m_EndPoint = point; }
        QVector<double> endPoint() const { return m_EndPoint; }

    protected:
        QVector<double> m_StartPoint{};
        QVector<double> m_EndPoint{};
    };


    /**
     * @brief  使用基准面分割.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2025-05-06
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelPartitionFaceWithDatumPlane :
        public FITKAbsGeoModelPartitionFace
    {
        FITKCLASS(Interface, FITKAbsGeoModelPartitionFaceWithDatumPlane);
    public:
        FITKAbsGeoModelPartitionFaceWithDatumPlane() = default;
        ~FITKAbsGeoModelPartitionFaceWithDatumPlane() override = default;
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        FITKAbsGeoDatumPlane* datumPlane() const { return m_DatumPlane; }
        void setDatumPlane(FITKAbsGeoDatumPlane* plane) { m_DatumPlane = plane; }
    protected:
        FITKAbsGeoDatumPlane* m_DatumPlane{};
    };


    /**
     * @brief  使用曲线路径分割.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2025-05-07
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelPartitionFaceWithCurvedPath :
        public FITKAbsGeoModelPartitionFace
    {
        FITKCLASS(Interface, FITKAbsGeoModelPartitionFaceWithCurvedPath);
    public:
        FITKAbsGeoModelPartitionFaceWithCurvedPath() = default;
        ~FITKAbsGeoModelPartitionFaceWithCurvedPath() override = default;
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;


        void setFace(VirtualShape face) { m_Faces = { face }; }
        VirtualShape face() const { return m_Faces.size() >= 1 ? m_Faces.at(0) : VirtualShape(); };

        void setFirstEdge(VirtualShape edge) { m_FirstEdge = edge; }
        VirtualShape firstEdge() const { return m_FirstEdge; }
        void setSecondEdge(VirtualShape edge) { m_SecondEdge = edge; }
        VirtualShape secondEdge() const { return m_SecondEdge; }
        void setFirstPoint(QVector<double> point) { m_FirstPoint = point; }
        QVector<double> firstPoint() const { return m_FirstPoint; }
        void setSecondPoint(QVector<double> point) { m_SecondPoint = point; }
        QVector<double> secondPoint() const { return m_SecondPoint; }
    protected:
        VirtualShape m_FirstEdge{};
        VirtualShape m_SecondEdge{};
        QVector<double> m_FirstPoint{};
        QVector<double> m_SecondPoint{};

    };


    /**
     * @brief  使用扩展面分割.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2025-05-07
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelPartitionFaceWithExtendFace :
        public FITKAbsGeoModelPartitionFace
    {
        FITKCLASS(Interface, FITKAbsGeoModelPartitionFaceWithExtendFace);
    public:
        FITKAbsGeoModelPartitionFaceWithExtendFace() = default;
        ~FITKAbsGeoModelPartitionFaceWithExtendFace() override = default;
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        void setExtendFace(VirtualShape face) { m_ExtendFace = face; }
        VirtualShape face() const { return m_ExtendFace; };
    protected:
        VirtualShape m_ExtendFace{};
    };


    /**
     * @brief  使用相交面分割.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2025-05-07
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelPartitionFaceWithIntersectFace :
        public FITKAbsGeoModelPartitionFace
    {
        FITKCLASS(Interface, FITKAbsGeoModelPartitionFaceWithIntersectFace);
    public:
        FITKAbsGeoModelPartitionFaceWithIntersectFace() = default;
        ~FITKAbsGeoModelPartitionFaceWithIntersectFace() override = default;
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        void setToolFaces(QList<Interface::VirtualShape> faces) { m_ToolFaces = faces; }
        QList<Interface::VirtualShape> toolFace() const { return m_ToolFaces; }
    protected:
        QList<Interface::VirtualShape> m_ToolFaces{};
    };


    /**
     * @brief  使用投影边分割.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2025-05-07
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelPartitionFaceWithProjectEdges :
        public FITKAbsGeoModelPartitionFace
    {
        FITKCLASS(Interface, FITKAbsGeoModelPartitionFaceWithProjectEdges);
    public:
        FITKAbsGeoModelPartitionFaceWithProjectEdges() = default;
        ~FITKAbsGeoModelPartitionFaceWithProjectEdges() override = default;
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;
        void setProjectEdges(QList<Interface::VirtualShape> edges) { m_ProjectEdges = edges; }
        QList<Interface::VirtualShape> projectEdges() const { return m_ProjectEdges; }
    protected:
        QList<Interface::VirtualShape> m_ProjectEdges{};

    };


}

#endif // !FITKABSGEOMODELPARTITIONFACE_H
